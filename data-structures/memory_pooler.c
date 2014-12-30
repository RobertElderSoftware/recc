/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/

#include "memory_pooler.h"

/*
	The purpose of the 'memory pooler' is to provide near constant time memory allocation and deallocation while minimizing the
	number of kernel calls to malloc and free.

	Pointers returned are guaranteed to be always point to the underlying object until they are freed, or this data structure is destroyed.
*/

unsigned int pooling_active = 1;  /* Easy switch to toggle between pooling and normal malloc free */

unsigned int get_block_index(unsigned int);
void * get_object_pointer(void **, unsigned int, unsigned int);

unsigned int get_block_index(unsigned int index){
	/*  Returns the 0-based index of the block of memory in our pool for a given index
	    that represents the offset of the data object at the top of the stack.
        */
	unsigned int base_1_index = index + 1;
	unsigned int shifts = 0;
	/* Compute floor(log_2(index + 1)) */
	while(base_1_index >>= 1){ shifts++; }
	return shifts;
}

void * get_object_pointer(void ** block_pointers, unsigned int index, unsigned int object_size){
	unsigned int block_index = get_block_index(index);
	return &(((char *)(block_pointers[block_index]))[object_size * ((index + 1) - (1 << block_index))]);
}

void memory_pooler_create(struct memory_pooler * p, unsigned int object_size){
	assert(object_size);
	void_ptr_list_create(&p->free_pointers);
	void_ptr_list_create(&p->block_pointers);
	p->objects_allocated = 0;
	p->next_allocation = 0;
	p->objects_to_allocate = 1;
	p->object_size = object_size;
}

void memory_pooler_destroy(struct memory_pooler * p){
	unsigned int i;
	unsigned int size = void_ptr_list_size(&p->block_pointers);
	assert((!p->objects_allocated) && "There still objects allocated in this memory pool!");
	void_ptr_list_destroy(&p->free_pointers);
	for(i = 0; i < size; i++){
		free(void_ptr_list_get(&p->block_pointers, i));
	}
	void_ptr_list_destroy(&p->block_pointers);
	p->objects_allocated = 0;
	p->next_allocation = 0;
	p->objects_to_allocate = 1;
	p->object_size = 0;
}

void * memory_pooler_malloc(struct memory_pooler * p){
	if(pooling_active){
		/*  If there was a recently freed object give back that one */
		if(void_ptr_list_size(&p->free_pointers)){
			void * rtn;
			p->objects_allocated = p->objects_allocated + 1;
			rtn = void_ptr_list_pop(&p->free_pointers);
			assert(rtn);
			return rtn;
		}
		/*  No recently freed objects, check if we need to allocate another memory block */
		if(p->objects_allocated == p->next_allocation){
			/*  Allocate a new block of size 2^n, where n increases by one each time we do this */
			void_ptr_list_add(&p->block_pointers, malloc(p->object_size * p->objects_to_allocate));
			p->next_allocation += p->objects_to_allocate;
			p->objects_to_allocate *= 2;
		}
		p->objects_allocated = p->objects_allocated + 1;
		return get_object_pointer(void_ptr_list_data(&p->block_pointers), p->objects_allocated -1, p->object_size);
	}else{
		return malloc(p->object_size);
	}
}

void memory_pooler_free(struct memory_pooler * p, void * ptr){
	if(pooling_active){
		if(ptr){
			assert(p->objects_allocated);
			p->objects_allocated = p->objects_allocated - 1;
			void_ptr_list_add(&p->free_pointers, ptr);
		}
	}else{
		free(ptr);
	}
}

void memory_pooler_collection_create(struct memory_pooler_collection * collection){
	struct_memory_pooler_ptr_list_create(&collection->pools);
	unsigned_int_list_create(&collection->sizes);
}

void memory_pooler_collection_destroy(struct memory_pooler_collection * collection){
	unsigned int num_pools = struct_memory_pooler_ptr_list_size(&collection->pools);
	unsigned int i;
	for(i = 0; i < num_pools; i++){
		struct memory_pooler * m = struct_memory_pooler_ptr_list_get(&collection->pools, i);
		memory_pooler_destroy(m);
		free(m);
	}
	struct_memory_pooler_ptr_list_destroy(&collection->pools);
	unsigned_int_list_destroy(&collection->sizes);
}


struct memory_pooler * memory_pooler_collection_get_pool(struct memory_pooler_collection * collection, unsigned int size){
	unsigned int num_pools = struct_memory_pooler_ptr_list_size(&collection->pools);
	unsigned int i;
	struct memory_pooler * new_pool;
	for(i = 0; i < num_pools; i++){
		if(size == unsigned_int_list_get(&collection->sizes, i)){
			return struct_memory_pooler_ptr_list_get(&collection->pools, i);
		}
	}
	/*  Need to create another pool for this size object */
	new_pool = malloc(sizeof(struct memory_pooler));
	memory_pooler_create(new_pool, size);
	struct_memory_pooler_ptr_list_add(&collection->pools, new_pool);
	unsigned_int_list_add(&collection->sizes, size);
	return new_pool;
}
