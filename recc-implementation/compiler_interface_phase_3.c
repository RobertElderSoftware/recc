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

#include "compiler_interface_phase_3.h"


unsigned char * get_parent_symbol_file(struct build_state * state, struct entity * target){
	struct unsigned_char_ptr_list parent_keys = get_entity_name_list_for_relationship(state, target, RELATION_TYPE_IS_DEPENDED_ON_AT_BUILD_TIME_BY);
	unsigned int i;
	unsigned char * rtn = (unsigned char *)0;
	(void)state;
	for(i = 0; i < unsigned_char_ptr_list_size(&parent_keys); i++){
		struct entity * p = unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, unsigned_char_ptr_list_get(&parent_keys, i));
		if(p->type == ENTITY_TYPE_SYMBOL_FILE){
			assert(!rtn && "Two symbol files?");
			rtn = unsigned_char_ptr_list_get(&parent_keys, i);
		}
	}
	unsigned_char_ptr_list_destroy(&parent_keys);
	return rtn;
}

unsigned int make_target(struct build_state * state, struct entity * target){
	assert(!make_header_target(state, target) || !make_c_compiler_target(state, target));
	return 0;
}

struct build_state * create_build_state(void){
	struct build_state * state = (struct build_state *)malloc(sizeof(struct build_state));
	unsigned_char_ptr_to_struct_entity_ptr_map_create(&state->targets, struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare);
	struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_create(&state->relationships, struct_struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare);
	state->memory_pool_collection = (struct memory_pool_collection*)malloc(sizeof(struct memory_pool_collection));
	memory_pool_collection_create(state->memory_pool_collection);

	struct_c_lexer_token_memory_pool_create(state->memory_pool_collection);
	struct_regex_computation_node_memory_pool_create(state->memory_pool_collection);
	struct_regex_parser_node_memory_pool_create(state->memory_pool_collection);
	struct_parser_node_memory_pool_create(state->memory_pool_collection);
	struct_l2_parser_node_memory_pool_create(state->memory_pool_collection);
	struct_l2_lexer_token_memory_pool_create(state->memory_pool_collection);
	struct_type_description_memory_pool_create(state->memory_pool_collection);
	struct_l2_item_memory_pool_create(state->memory_pool_collection);
	struct_linker_symbol_memory_pool_create(state->memory_pool_collection);
	heap_memory_pool_create(state->memory_pool_collection);
	l2_token_matcher_create(state->memory_pool_collection);
	c_token_matcher_create(state->memory_pool_collection);
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
	unsigned_char_ptr_to_struct_entity_ptr_map_destroy(&state->targets);
	struct_entity_relationship_list_destroy(&relationship_keys);
	struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_destroy(&state->relationships);
	heap_memory_pool_destroy(state->memory_pool_collection);

	struct_c_lexer_token_memory_pool_destroy(state->memory_pool_collection);
	struct_parser_node_memory_pool_destroy(state->memory_pool_collection);
	struct_l2_parser_node_memory_pool_destroy(state->memory_pool_collection);
	struct_l2_lexer_token_memory_pool_destroy(state->memory_pool_collection);
	struct_type_description_memory_pool_destroy(state->memory_pool_collection);
	struct_linker_symbol_memory_pool_destroy(state->memory_pool_collection);
	struct_l2_item_memory_pool_destroy(state->memory_pool_collection);

	l2_token_matcher_destroy(state->memory_pool_collection);
	c_token_matcher_destroy(state->memory_pool_collection);

	struct_regex_parser_node_memory_pool_destroy(state->memory_pool_collection);
	struct_regex_computation_node_memory_pool_destroy(state->memory_pool_collection);

	memory_pool_collection_destroy(state->memory_pool_collection);


	free(state->memory_pool_collection);
	free(state);
}
