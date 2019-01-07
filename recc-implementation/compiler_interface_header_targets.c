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

#include "compiler_interface_header_targets.h"

unsigned int make_header_target(struct build_state * state, struct entity * target){
	(void)state;
	switch(target->type){
		case ENTITY_TYPE_GENERATED_C_CODE:{
			unsigned int i;
			struct struct_entity_ptr_list * composed_list = get_entity_list_for_relationship(state, target, RELATION_TYPE_CONTAINS);
			struct struct_entity_ptr_list * includes_list = get_entity_list_for_relationship(state, target, RELATION_TYPE_INCLUDES);
			struct binary_exponential_buffer replace_rules;
			struct binary_exponential_buffer literal_types;
			struct binary_exponential_buffer headers;
			unsigned int is_header = 0;
			unsigned char * source_file = (unsigned char *)0;
			binary_exponential_buffer_create(&replace_rules, sizeof(struct replacement_rule *)); /*  Empty because of no additional replacments */
			binary_exponential_buffer_create(&headers, sizeof(unsigned char *));
			binary_exponential_buffer_create(&literal_types, sizeof(unsigned char *));

			if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&target->attributes, (unsigned char *)"is_header")){
				is_header = !strcmp((const char *)unsigned_char_ptr_to_unsigned_char_ptr_map_get(&target->attributes, (unsigned char *)"is_header"), "true");
			}

			if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&target->attributes, (unsigned char *)"type1")){
				binary_exponential_buffer_increment(&literal_types, 1);
				((unsigned char **)binary_exponential_buffer_data(&literal_types))[binary_exponential_buffer_size(&literal_types) -1] = unsigned_char_ptr_to_unsigned_char_ptr_map_get(&target->attributes, (unsigned char *)"type1");
			}
			if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&target->attributes, (unsigned char *)"type2")){
				binary_exponential_buffer_increment(&literal_types, 1);
				((unsigned char **)binary_exponential_buffer_data(&literal_types))[binary_exponential_buffer_size(&literal_types) -1] = unsigned_char_ptr_to_unsigned_char_ptr_map_get(&target->attributes, (unsigned char *)"type2");
			}

			make_type_replacement_rules(&replace_rules, &literal_types);

			if(includes_list){
				for(i = 0; i < struct_entity_ptr_list_size(includes_list); i++){
					unsigned char * h = struct_entity_ptr_list_get(includes_list, i)->name;
					binary_exponential_buffer_increment(&headers, 1);
					((unsigned char **)binary_exponential_buffer_data(&headers))[binary_exponential_buffer_size(&headers) -1] = h;
				}
			}
		
			if(composed_list){
				/*  If there is a source file, use that one */
				if(struct_entity_ptr_list_size(composed_list) == 1){
					source_file = struct_entity_ptr_list_get(composed_list, 0)->name;
				}else{
					printf("Found more than one composition file.");
					assert(0);
				}
			}
			make_generated_file(source_file, target->name, (unsigned char *)"../", &headers, &replace_rules, is_header);

			for(i = 0; i < binary_exponential_buffer_size(&replace_rules); i++){
				replacement_rule_destroy(((struct replacement_rule **)binary_exponential_buffer_data(&replace_rules))[i]);
			}

			binary_exponential_buffer_destroy(&headers);
			binary_exponential_buffer_destroy(&replace_rules);
			binary_exponential_buffer_destroy(&literal_types);
			target->satisfied = 1;
			return 0;
		}case ENTITY_TYPE_SOURCE:{
			assert(!get_entity_list_for_relationship(state, target, RELATION_TYPE_DEPENDS_ON_AT_BUILD_TIME) && "This target should have no dependencies.");
			target->satisfied = 1;
			return 0;
		}default:{
			return 1;
		}
	}
}
