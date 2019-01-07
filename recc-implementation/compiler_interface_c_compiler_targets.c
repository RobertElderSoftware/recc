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

#include "compiler_interface_c_compiler_targets.h"

unsigned int make_c_compiler_target(struct build_state * state, struct entity * target){
	struct unsigned_char_ptr_list child_keys = get_entity_name_list_for_relationship(state, target, RELATION_TYPE_DEPENDS_ON_AT_BUILD_TIME);
	unsigned int rtn;
	switch(target->type){
		case ENTITY_TYPE_L1_FILE:{
			unsigned char * symbol_file = get_parent_symbol_file(state, target);
			unsigned int page_align_permission_regions = 0;
			unsigned int only_metadata = 0;
			if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&target->attributes, (unsigned char *)"page_align_permission_regions")){
				page_align_permission_regions = !strcmp((const char *)unsigned_char_ptr_to_unsigned_char_ptr_map_get(&target->attributes, (unsigned char *)"page_align_permission_regions"), "true");
			}
			if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&target->attributes, (unsigned char *)"only_metadata")){
				only_metadata = !strcmp((const char *)unsigned_char_ptr_to_unsigned_char_ptr_map_get(&target->attributes, (unsigned char *)"only_metadata"), "true");
			}
			printf("Build process creating L1 file '%s'", target->name);
			if(symbol_file){
				printf(" with symbol file %s.\n", symbol_file);
			}else{
				printf(".\n");
			}
			do_link(state->memory_pool_collection, &child_keys, target->name, symbol_file, ENTITY_TYPE_L1_FILE, (unsigned char *)"0x0", page_align_permission_regions, only_metadata);
			target->satisfied = 1;
			rtn = 0;
			break;
		}case ENTITY_TYPE_L2_FILE:{
			unsigned char * relocation = (unsigned char *)"RELOCATABLE";
			if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&target->attributes, (unsigned char *)"relocation")){
				relocation = unsigned_char_ptr_to_unsigned_char_ptr_map_get(&target->attributes, (unsigned char *)"relocation");
			}
			/*  If it has a single child that is a preprocessed C file, then we're just building a regular l2 file. */
			if(unsigned_char_ptr_list_size(&child_keys) == 1 && unsigned_char_ptr_to_struct_entity_ptr_map_get(&state->targets, unsigned_char_ptr_list_get(&child_keys, 0))->type == ENTITY_TYPE_PREPROCESSED_FILE){
				printf("Build process creating L2 file '%s' from preprocessed file '%s'.\n", target->name, unsigned_char_ptr_list_get(&child_keys,0));
				if(do_code_generation(state->memory_pool_collection, unsigned_char_ptr_list_get(&child_keys, 0), target->name, relocation)){
					rtn = 1;
				}else{
					rtn = 0;
					target->satisfied = 1;
				}
			}else{
				/*  Otherwise, we must be linking together multiple l2 files into a big one. */
				unsigned char * symbol_file = get_parent_symbol_file(state, target);
				do_link(state->memory_pool_collection, &child_keys, target->name, symbol_file, ENTITY_TYPE_L2_FILE, relocation, 0, 0);
				target->satisfied = 1;
				rtn = 0;
			}
			break;
		}case ENTITY_TYPE_L0_FILE:{
			struct l0_generator_state * l0_generator_state;
			unsigned char * variable_name;
			unsigned char * language;
			if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&target->attributes, (unsigned char *)"variable_name")){
				variable_name = unsigned_char_ptr_to_unsigned_char_ptr_map_get(&target->attributes, (unsigned char *)"variable_name");
			}else{
				assert("Missing variable name attribute" && 0);
				variable_name = (unsigned char *)"uninitialized";
			}
			if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&target->attributes, (unsigned char *)"language")){
				language = unsigned_char_ptr_to_unsigned_char_ptr_map_get(&target->attributes, (unsigned char *)"language");
			}else{
				assert("Missing language attribute" && 0);
				language = (unsigned char *)"uninitialized";
			}
			printf("Build process creating L0 file '%s' from l1 file '%s'.\n", target->name, unsigned_char_ptr_list_get(&child_keys,0));
			l0_generator_state = l0_generator_state_create(state->memory_pool_collection, variable_name, unsigned_char_ptr_list_get(&child_keys,0), target->name, language);
			l0_generator_state_destroy(l0_generator_state);
			target->satisfied = 1;
			rtn = 0;
			break;
		}case ENTITY_TYPE_FILESYSTEM_IMPLEMENTATION:{
			printf("Build process creating filesystem implementation '%s'.\n", target->name);
			create_filesystem_impl(target->name);
			target->satisfied = 1;
			rtn = 0;
			break;
		}case ENTITY_TYPE_PREPROCESSED_FILE:{
			printf("Build process creating preprocessed file '%s' from C file '%s'.\n", target->name, unsigned_char_ptr_list_get(&child_keys,0));
			do_preprocess(state->memory_pool_collection, unsigned_char_ptr_list_get(&child_keys,0), target->name);
			target->satisfied = 1;
			rtn = 0;
			break;
		}case ENTITY_TYPE_C_FILE:{
			printf("Trying to build C file '%s'.\n", target->name);
			assert(0 && "Not supported.");
			target->satisfied = 1;
			rtn = 0;
			break;
		}default:{
			rtn = 1;
			break;
		}
	}
	unsigned_char_ptr_list_destroy(&child_keys);
	return rtn;
}
