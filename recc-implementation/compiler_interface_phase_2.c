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

#include "compiler_interface_phase_2.h"


unsigned int make_target(struct build_state * state, struct entity * target){
	assert(!make_header_target(state, target));
	return 0;
}

struct build_state * create_build_state(void){
	struct build_state * state = (struct build_state *)malloc(sizeof(struct build_state));
	unsigned_char_ptr_to_struct_entity_ptr_map_create(&state->targets, struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare);
	struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_create(&state->relationships, struct_struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare);
	return state;
}

void destroy_build_state(struct build_state * state){
	unsigned int i;
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_entity_ptr_map_keys(&state->targets);
	struct struct_entity_relationship_list relationship_keys = struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_keys(&state->relationships);
	unsigned int num_targets = unsigned_char_ptr_list_size(&keys);
	unsigned int num_relationships = struct_entity_relationship_list_size(&relationship_keys);
	for(i = 0; i < num_targets; i++){
		unsigned char * k = unsigned_char_ptr_list_get(&keys, i);
		struct entity * target = unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, k);
		destroy_entity(target);
	}
	for(i = 0; i < num_relationships; i++){
		struct entity_relationship k = struct_entity_relationship_list_get(&relationship_keys, i);
		struct struct_entity_ptr_list * list = struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_get(&state->relationships, k);
		struct_entity_ptr_list_destroy(list);
		free(list);
	}

	/*  Delete keys after map contents are freed */
	for(i = 0; i < num_targets; i++){
		free(unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_list_destroy(&keys);
	struct_entity_relationship_list_destroy(&relationship_keys);
	unsigned_char_ptr_to_struct_entity_ptr_map_destroy(&state->targets);
	struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_destroy(&state->relationships);
	free(state);
}
