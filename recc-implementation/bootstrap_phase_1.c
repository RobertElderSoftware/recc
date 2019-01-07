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

#include "compiler_interface_phase_1.h"

void create_map_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer headers);

void create_map_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer headers){
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_MAP_ALGORITHM, "recc-implementation/templates/generic_algorithm_map.c", binary_exponential_buffer_copy(&types), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_MAP_HEADER, "recc-implementation/templates/generic_header_map.h", binary_exponential_buffer_copy(&types), binary_exponential_buffer_copy(&headers))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_MAP_TYPE, "recc-implementation/templates/generic_type_map.h", types, headers)));
}

void create_list_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer headers);

void create_list_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer headers){
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_LIST_ALGORITHM, "recc-implementation/templates/generic_algorithm_list.c", binary_exponential_buffer_copy(&types), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_LIST_HEADER, "recc-implementation/templates/generic_header_list.h", binary_exponential_buffer_copy(&types), binary_exponential_buffer_copy(&headers))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_LIST_TYPE, "recc-implementation/templates/generic_type_list.h", types, headers)));
}


void create_kvp_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer headers);

void create_kvp_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer headers){
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_KEY_VALUE_PAIR_TYPE, "recc-implementation/templates/generic_type_key_value_pair.h", types, headers)));
}


void create_binary_search_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer headers);

void create_binary_search_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer headers){
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_BINARY_SEARCH_ALGORITHM, "recc-implementation/templates/generic_algorithm_binary_search.c", binary_exponential_buffer_copy(&types), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_BINARY_SEARCH_HEADER, "recc-implementation/templates/generic_header_binary_search.h", types, headers)));
}

void create_compare_files(struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer, struct binary_exponential_buffer, const char *);

void create_compare_files(struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer algorithm_headers, struct binary_exponential_buffer header_headers, const char * algorithm_template){
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_COMPARE_ALGORITHM, algorithm_template, binary_exponential_buffer_copy(&types), algorithm_headers)));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(GENERATED_FILE_TYPE_COMPARE_HEADER, "recc-implementation/templates/comparison-functions/simple_compare.h", types, header_headers)));
}

int main(void){
	struct bootstrap_state bootstrap_state;
	struct bootstrap_state * b = &bootstrap_state;
	bootstrap_state_create(b);
	create_compare_files(b, make_beb_list(1, "struct unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair"), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_compare.h"), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair.h"), "recc-implementation/templates/comparison-functions/strcmp_compare.c");
	create_compare_files(b, make_beb_list(1, "struct unsigned_char_ptr_to_struct_entity_ptr_key_value_pair"), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare.h"), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair.h"), "recc-implementation/templates/comparison-functions/strcmp_compare.c");
	create_compare_files(b, make_beb_list(1, "struct struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair"), make_beb_list(1, "generated/struct_struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare.h"), make_beb_list(1, "generated/struct_struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair.h"), "recc-implementation/templates/comparison-functions/entity_relationship_compare.c");
	create_map_files(b, make_beb_list(2, "unsigned char *", "struct entity *"), make_beb_list(1, "recc-implementation/struct_entity.h"));
	create_map_files(b, make_beb_list(2, "struct entity_relationship", "struct struct_entity_ptr_list *"), make_beb_list(2, "recc-implementation/struct_entity_relationship.h", "generated/struct_struct_entity_ptr_list.h"));
	create_map_files(b, make_beb_list(2, "unsigned char *", "unsigned char *"), make_beb_list(0));


	create_kvp_files(b, make_beb_list(2, "unsigned char *", "struct entity *"), make_beb_list(1, "recc-implementation/struct_entity.h"));
	create_kvp_files(b, make_beb_list(2, "struct entity_relationship", "struct struct_entity_ptr_list *"), make_beb_list(2, "recc-implementation/struct_entity_relationship.h", "generated/struct_struct_entity_ptr_list.h"));
	create_kvp_files(b, make_beb_list(2, "unsigned char *", "unsigned char *"), make_beb_list(0));


	create_list_files(b, make_beb_list(1, "unsigned char *"), make_beb_list(0));
	create_list_files(b, make_beb_list(1, "unsigned char"), make_beb_list(0));
	create_list_files(b, make_beb_list(1, "struct entity *"), make_beb_list(1, "recc-implementation/struct_entity.h"));
	create_list_files(b, make_beb_list(1, "struct entity_relationship"), make_beb_list(1, "recc-implementation/struct_entity_relationship.h"));
	create_list_files(b, make_beb_list(1, "struct struct_entity_ptr_list *"), make_beb_list(1, "generated/struct_struct_entity_ptr_list.h"));
	create_binary_search_files(b, make_beb_list(1, "struct unsigned_char_ptr_to_struct_entity_ptr_key_value_pair"), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair.h"));
	create_binary_search_files(b, make_beb_list(1, "struct unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair"), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair.h"));
	create_binary_search_files(b, make_beb_list(1, "struct struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair"), make_beb_list(1, "generated/struct_struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair.h"));


	bootstrap_state_output_makefiles(b, "");
	bootstrap_state_destroy(b);
	return 0;
}
