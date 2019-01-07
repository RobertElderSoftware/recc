/*
    Copyright 2019 Robert Elder Software Inc.
    
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

#include "memory_pool_collection.h"

void memory_pool_collection_create(struct memory_pool_collection * m){
	/*  The memory pool collection should only contain simple pointers to avoid dependencies on other components. */
	m->struct_regex_computation_node_pool = (struct struct_regex_computation_node_memory_pool *)0;
	m->struct_regex_parser_node_pool = (struct struct_regex_parser_node_memory_pool *)0;
	m->struct_c_lexer_token_pool = (struct struct_c_lexer_token_memory_pool *)0;
	m->struct_parser_node_pool = (struct struct_parser_node_memory_pool *)0;
	m->struct_l2_parser_node_pool = (struct struct_l2_parser_node_memory_pool *)0;
	m->struct_l2_lexer_token_pool = (struct struct_l2_lexer_token_memory_pool *)0;
	m->struct_type_description_pool = (struct struct_type_description_memory_pool *)0;
	m->struct_l2_item_pool = (struct struct_l2_item_memory_pool *)0;
	m->struct_linker_symbol_pool = (struct struct_linker_symbol_memory_pool *)0;
	m->c_token_matcher = (struct regex_matcher_state *)0;
	m->l2_token_matcher = (struct regex_matcher_state *)0;
}

void memory_pool_collection_destroy(struct memory_pool_collection * m){
	(void)m;
}


#define WORD_SIZE_LOG_2 2

void heap_memory_pool_create(struct memory_pool_collection * m){
	unsigned int i;
	unsigned int current_pool_item_size = sizeof(unsigned int);    /*  Start pool block sizes at the size of a word */
	for(i = 0; i < NUM_HEAP_POOLS; i++){
		m->heap_pools[i] = void_ptr_memory_pool_create_internal();
		m->heap_pools[i]->object_size = current_pool_item_size; /*  Set size of pool */
		current_pool_item_size = current_pool_item_size * 2;   /*  Next pool has blocks that are twice this size */
	}
	void_ptr_to_unsigned_int_map_create(&m->contiguous_heap_bock_entries, struct_void_ptr_to_unsigned_int_key_value_pair_compare);
}

void heap_memory_pool_destroy(struct memory_pool_collection * m){
	unsigned int i;
	for(i = 0; i < NUM_HEAP_POOLS; i++){
		void_ptr_memory_pool_destroy_internal(m->heap_pools[i]);
	}
	void_ptr_to_unsigned_int_map_destroy(&m->contiguous_heap_bock_entries);
}

void * heap_memory_pool_malloc(struct memory_pool_collection * m, unsigned int num_bytes){
	if(num_bytes){
		unsigned int num_bytes_copy = num_bytes;
		unsigned int pow_two = 0;
		unsigned int is_exactly_power_of_two = (num_bytes & (num_bytes - 1)) == 0;
		while((num_bytes >>= 1)) { pow_two++; } /*  Compute floor(log_2 num_bytes) */
		pow_two += !is_exactly_power_of_two;  /* This gives us ceil(log_2 num_bytes) which tells us which pool to use */
		if(pow_two < (NUM_HEAP_POOLS + WORD_SIZE_LOG_2)){
			void * last_block = (void *)0;
			void * return_value;
			unsigned int pool_index = pow_two > 1 ? (pow_two - WORD_SIZE_LOG_2) : 0;
			return_value = (void *)void_ptr_memory_pool_malloc_tracking_internal(m->heap_pools[pool_index], &last_block);
			if(last_block){
				/*  The block pointer came from a newly allocated region inside the memory pool.  Keep track of that region. */
				void_ptr_to_unsigned_int_map_put(&m->contiguous_heap_bock_entries, last_block, pool_index);
			}
			(void)num_bytes_copy;
			/*printf("Malloc return %p for item size %i\n", return_value, num_bytes_copy);*/
			return return_value;
		}else{
			assert(0 && "No considering this block size.  Make number of heap pools bigger?");
			return (void *)0;
		}
	}else{
		return (void *)0;
	}
}

/*  The heap_memory_pool_free method is highly coupled to the internal implementation of the 'memory pool'.  This is because we need to 
    be able to determine which memory pool a given pointer belongs to when we need to free it.  We do this by keeping track of a pointer
    to the last item in new blocks that get allocated inside.  When we want to free a block, we check to see if the pointer could have
    belonged to a block that was allocated by a given memory pool.  If it could, then we know which memory pool to free it from.

*/

void heap_memory_pool_free(struct memory_pool_collection * m, void * p){
	struct void_ptr_to_unsigned_int_key_value_pair * data = (struct void_ptr_to_unsigned_int_key_value_pair *)binary_exponential_buffer_data(&m->contiguous_heap_bock_entries.key_value_pairs);
	int imin = 0;
	int size = (int)binary_exponential_buffer_size(&m->contiguous_heap_bock_entries.key_value_pairs);
	int imax = size -1;
	int list_index = -1;
	/*  Search for an entry that will tell us which pool the pointer came from */
	if(size){
		while (imax >= imin) {
			int imid = imin + ((imax - imin) >> 1);
			if(data[imid].key < p){
				imin = imid + 1;
			}else if(data[imid].key > p){
				imax = imid - 1;
			}else{        
				list_index = imid;
				break;
			}
		}
		if(!(imax >= imin)){ /*  Loop terminated naturally */
			list_index = imin;
		}
	}
	if(m->heap_pools[0]->pooling_active){
		if((list_index == -1 || (!(list_index < size)))){
			assert(0 && "This should never happen.");
		}else{
			void_ptr_memory_pool_free_internal(m->heap_pools[data[list_index].value], (void**)p);
		}
	}else{
		free(p);
	}
}
