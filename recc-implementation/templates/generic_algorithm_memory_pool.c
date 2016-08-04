/*
    Copyright 2016 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/
#include "T0_IDENTIFIER_memory_pool.h"
#include <stdio.h>

/*
	The purpose of the 'memory pool' is to provide near constant time memory allocation and deallocation while minimizing the
	number of kernel calls to malloc and free.

	Pointers returned are guaranteed to be always point to the underlying object until they are freed, or this data structure is destroyed.

	This data structure keeps an array of 'block pointers' that points to blocks of size 2^N:


	                                          +--------+
	                                          | Item 6 |
	                                          +--------+
	                                          +--------+
	                                          | Item 5 |
	                                          +--------+
	                     +--------+           +--------+         
	                     | Item 2 |           | Item 4 |         	
	                     +--------+           +--------+         
	+--------+           +--------+           +--------+         
	| Item 0 |           | Item 1 |           | Item 3 |         
	+--------+           +--------+           +--------+          ...
                                                                          
	block_pointers[0]    block_pointers[1]    block_pointers[2]   ...  block_pointers[...]  
	contains 2^0 blocks  contains 2^1 blocks  contains 2^2 blocks ...  contains 2^.. blocks

	When we need to allocate a new object, if there happens to be a free block we simply need to return the pointer to that item inside the block.
	If there are no free blocks, we will allocate a chunk of memory that has size (sizeof(item size) * 2^N+1) where N is log_2 of the
	the current largest number of items in a block.

	In the worst case this implementation will waste no more than half of the memory it has allocated.

	When an item is freed, the current implementation will simply add that item pointer to a list, so it can be returned for a future
	allocation request.  When this data structure is destroyed the entire blocks can be freed instead of freeing each individual item.
	
	Finally, it doesn't always make sense for the initial block size to be one, so as an additional configuration parameter you can start
	with a block size that is larger.
	
*/

static unsigned int T0_IDENTIFIER_pooling_active = 1;  /* Easy switch to toggle between pooling and normal malloc free */
static unsigned int T0_IDENTIFIER_objects_in_first_block = 256; /* Must be a non-zero power of two */
static unsigned int T0_IDENTIFIER_objects_in_first_block_log_2 = 8; /* log_2(T0_IDENTIFIER_objects_in_first_block) */

unsigned int T0_IDENTIFIER_get_block_index(unsigned int);
void * T0_IDENTIFIER_get_object_pointer(void **, unsigned int, unsigned int);

unsigned int T0_IDENTIFIER_get_block_index(unsigned int index){
	/*  Returns the 0-based index of the block of memory in our pool for a given index
	    that represents the offset of the data object at the top of the stack.
        */
	unsigned int base_1_index = index + T0_IDENTIFIER_objects_in_first_block;
	unsigned int shifts = 0;
	/* Compute floor(log_2(index + 1)) */
	while(base_1_index >>= 1){ shifts++; }
	/*printf("Returns shifts %d\n", shifts);*/
	return shifts - T0_IDENTIFIER_objects_in_first_block_log_2;
}

void * T0_IDENTIFIER_get_object_pointer(void ** block_pointers, unsigned int index, unsigned int object_size){
	unsigned int block_index = T0_IDENTIFIER_get_block_index(index);
	assert(object_size);
	/*printf("Ptr was %p start at %p, block index is %i for index %i item. Left shift %i\n", (void*)&(((char *)(block_pointers[block_index]))[object_size * ((index + 1) - (1 << block_index))]), block_pointers[block_index], block_index, index, (1 << block_index));*/
	return &(((char *)(block_pointers[block_index]))[object_size * ((index + T0_IDENTIFIER_objects_in_first_block) - (1 << (block_index + T0_IDENTIFIER_objects_in_first_block_log_2)))]);
}

void T0_IDENTIFIER_memory_pool_create(struct T0_IDENTIFIER_memory_pool * p){
	void_ptr_list_create(&p->block_pointers);
	p->objects_allocated = 0;
	p->next_preallocation = 0;
	p->num_freed_pointers = 0;
	p->freed_pointers = (void**)0;
	p->objects_to_preallocate = T0_IDENTIFIER_objects_in_first_block;
	p->object_size = sizeof(T0_LITERAL);
	p->pooling_active = T0_IDENTIFIER_pooling_active;
}

void T0_IDENTIFIER_memory_pool_destroy(struct T0_IDENTIFIER_memory_pool * p){
	unsigned int i;
	unsigned int size = void_ptr_list_size(&p->block_pointers);
	assert((!p->objects_allocated) && "There still objects allocated in this memory pool!");
	free((void*)p->freed_pointers);
	for(i = 0; i < size; i++){
		free(void_ptr_list_get(&p->block_pointers, i));
	}
	void_ptr_list_destroy(&p->block_pointers);
	p->objects_allocated = 0;
	p->next_preallocation = 0;
	p->num_freed_pointers = 0;
	p->freed_pointers = (void**)0;
	p->objects_to_preallocate = T0_IDENTIFIER_objects_in_first_block;
	p->object_size = 0;
	p->pooling_active = T0_IDENTIFIER_pooling_active;
}

T0_LITERAL * T0_IDENTIFIER_memory_pool_malloc(struct T0_IDENTIFIER_memory_pool * p){
	void * abc;
	return T0_IDENTIFIER_memory_pool_malloc_tracking(p, &abc);
}

T0_LITERAL * T0_IDENTIFIER_memory_pool_malloc_tracking(struct T0_IDENTIFIER_memory_pool * p, void ** new_region_last_item){
	if(T0_IDENTIFIER_pooling_active){
		/*  If there was a recently freed object give back that one */
		if(p->num_freed_pointers){
			void * rtn;
			p->objects_allocated = p->objects_allocated + 1;
			rtn = p->freed_pointers[p->num_freed_pointers -1];
			p->num_freed_pointers = p->num_freed_pointers -1;
			assert(rtn);
			return (T0_LITERAL *)rtn;
		}
		/*  No recently freed objects, check if we need to allocate another memory block */
		if(p->objects_allocated == p->next_preallocation){
			void * new_region = malloc(p->object_size * p->objects_to_preallocate);
			/*  If we add a new region, there is a use case to keep track of the pointer to the last item in */
			/*  region.  We can do bookkeeping on this pointer to do fast frees */
			*new_region_last_item = &((char *)new_region)[(p->objects_to_preallocate - 1) * p->object_size];
			/*  Allocate a new block of size 2^n, where n increases by one each time we do this */
			void_ptr_list_add_end(&p->block_pointers, new_region);
			free((void*)p->freed_pointers); /*  Don't need realloc because there must have been no free pointers to get here */
			p->freed_pointers = (void**)malloc((p->objects_to_preallocate + p->objects_allocated) * sizeof(void*)); /*  Worst case to free all pointers */
			p->next_preallocation = p->objects_to_preallocate;
			p->objects_to_preallocate *= 2;
		}
		p->objects_allocated = p->objects_allocated + 1;
		return (T0_LITERAL *)T0_IDENTIFIER_get_object_pointer(void_ptr_list_data(&p->block_pointers), p->objects_allocated -1, p->object_size);
	}else{
		return (T0_LITERAL *)malloc(p->object_size);
	}
}

void T0_IDENTIFIER_memory_pool_free(struct T0_IDENTIFIER_memory_pool * p, T0_LITERAL * ptr){
	if(T0_IDENTIFIER_pooling_active){
		if(ptr){
			assert(p->objects_allocated);
			p->objects_allocated = p->objects_allocated - 1;
			p->freed_pointers[p->num_freed_pointers] = (void*)ptr;
			p->num_freed_pointers = p->num_freed_pointers + 1;
		}
	}else{
		free((void*)ptr);
	}
}
