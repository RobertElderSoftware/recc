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

#include "T0_IDENTIFIER_to_T1_IDENTIFIER_map.h"
#include <stdio.h>

void T0_IDENTIFIER_to_T1_IDENTIFIER_map_create(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map, int (*cmp)(struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *, struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)){
	map->compare = cmp;
	binary_exponential_buffer_create(&map->key_value_pairs, sizeof(struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair));
}

void T0_IDENTIFIER_to_T1_IDENTIFIER_map_destroy(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map){
	binary_exponential_buffer_destroy(&map->key_value_pairs);
}

void T0_IDENTIFIER_to_T1_IDENTIFIER_map_put(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map, T0_LITERAL key, T1_LITERAL value){
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair kvp;
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair * data;
	int closest;
	unsigned int i;
	unsigned int old_num;
	int search_result;
	kvp.key = key;
	kvp.value = value;
	/*  Make sure there is no pre-existing item with that key */
	search_result = struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_binary_search((struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair*)binary_exponential_buffer_data(&map->key_value_pairs), &kvp, (int)binary_exponential_buffer_size(&map->key_value_pairs), map->compare, struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_BINARY_SEARCH_RETURN_NEG_ONE);
	assert(-1 == search_result);
	closest = struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_binary_search((struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair*)binary_exponential_buffer_data(&map->key_value_pairs), &kvp, (int)binary_exponential_buffer_size(&map->key_value_pairs), map->compare, struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_BINARY_SEARCH_RETURN_MIN);
	old_num = binary_exponential_buffer_size(&map->key_value_pairs);
	binary_exponential_buffer_increment(&map->key_value_pairs, 1);
	data = (struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&map->key_value_pairs);

	/*  Move the array elements down by 1 */
	if(old_num){
		for(i = old_num -1 ; i >= (unsigned int)closest; i--){
			data[i + 1] = data[i];
			if(i == (unsigned int)closest){
				break;
			}
		}
		data[closest] = kvp; /*  Add the new element to its sorted position */
	}else{
		data[0] = kvp;
	}
}

T1_LITERAL T0_IDENTIFIER_to_T1_IDENTIFIER_map_get(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map, T0_LITERAL key){
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair search_kvp;
	unsigned int size = binary_exponential_buffer_size(&map->key_value_pairs);
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair * data = (struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&map->key_value_pairs);
	int found_index;
	search_kvp.key = key;
	found_index = struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_binary_search(data, &search_kvp, (int)size, map->compare, struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_BINARY_SEARCH_RETURN_NEG_ONE);
	assert(found_index != -1 && "Map item with that key was not found.  Check for exists first.");
	return data[found_index].value;
}

T1_LITERAL * T0_IDENTIFIER_to_T1_IDENTIFIER_map_exists(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map, T0_LITERAL key){
	/*  Returns 1 if an item with that key exists in the map.  0 otherwise. */
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair search_kvp;
	unsigned int size = binary_exponential_buffer_size(&map->key_value_pairs);
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair * data = (struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&map->key_value_pairs);
	int found_index;
	search_kvp.key = key;
	found_index = struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_binary_search(data, &search_kvp, (int)size, map->compare, struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_BINARY_SEARCH_RETURN_NEG_ONE);
	if(found_index == -1){
		return (T1_LITERAL *)0;
	}else{
		return &((struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&map->key_value_pairs))[found_index -1].value;
	}
}

struct T0_IDENTIFIER_to_T1_IDENTIFIER_map T0_IDENTIFIER_to_T1_IDENTIFIER_map_copy(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map){
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_map copy;
	unsigned int total_size = binary_exponential_buffer_size(&map->key_value_pairs);
	unsigned int i;
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair * src_data;
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair * dst_data;
	binary_exponential_buffer_create(&copy.key_value_pairs, sizeof(struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair));
	binary_exponential_buffer_increment(&copy.key_value_pairs, total_size);
	src_data = (struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&map->key_value_pairs);
	dst_data = (struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&copy.key_value_pairs);
	for(i = 0; i < total_size; i++){
		struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair kvp;
		kvp.key = src_data[i].key;
		kvp.value = src_data[i].value;
		dst_data[i] = kvp;
	}
	return copy;
}

struct T0_IDENTIFIER_list T0_IDENTIFIER_to_T1_IDENTIFIER_map_keys(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map){
	struct T0_IDENTIFIER_list rtn;
	unsigned int size = binary_exponential_buffer_size(&map->key_value_pairs);
	unsigned int i;
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair * data = (struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&map->key_value_pairs);
	T0_IDENTIFIER_list_create(&rtn);
	for(i = 0; i < size; i++){
		T0_IDENTIFIER_list_add_end(&rtn, data[i].key);
	}
	return rtn;
}

struct T1_IDENTIFIER_list T0_IDENTIFIER_to_T1_IDENTIFIER_map_values(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map){
	struct T1_IDENTIFIER_list rtn;
	unsigned int size = binary_exponential_buffer_size(&map->key_value_pairs);
	unsigned int i;
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair * data = (struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&map->key_value_pairs);
	T1_IDENTIFIER_list_create(&rtn);
	for(i = 0; i < size; i++){
		T1_IDENTIFIER_list_add_end(&rtn, data[i].value);
	}
	return rtn;
}

unsigned int T0_IDENTIFIER_to_T1_IDENTIFIER_map_size(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map){
	return binary_exponential_buffer_size(&map->key_value_pairs);
}

void T0_IDENTIFIER_to_T1_IDENTIFIER_map_remove(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map, T0_LITERAL key){
	/* Remove key value pair with key item from map */
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair search;
	unsigned int i = 0;
	unsigned int old_i = 0;
	unsigned int new_i = 0;
	struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair * data = (struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *)binary_exponential_buffer_data(&map->key_value_pairs);
	search.key = key;

	for(i = 0; i < binary_exponential_buffer_size(&map->key_value_pairs); i++){
		if(map->compare(&search, &data[i])){
			data[new_i] = data[old_i];
			new_i++;
		}else{
			
		}
		old_i++;
	}

	binary_exponential_buffer_decrement(&map->key_value_pairs, old_i - new_i);
}
