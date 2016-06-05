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

#include "compiler_interface_phase_1.h"

int main(void){
	struct bootstrap_state bootstrap_state;
	struct bootstrap_state * b = &bootstrap_state;
	bootstrap_state_create(b);
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_MAP, make_beb_list(2, "unsigned char *", "struct entity *"), make_beb_list(1, "recc-implementation/struct_entity.h"), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_MAP, make_beb_list(2, "struct entity_relationship", "struct struct_entity_ptr_list *"), make_beb_list(2, "recc-implementation/struct_entity_relationship.h", "generated/struct_struct_entity_ptr_list.h"), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_MAP, make_beb_list(2, "unsigned char *", "unsigned char *"), make_beb_list(0), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_KEY_VALUE_PAIR, make_beb_list(2, "unsigned char *", "struct entity *"), make_beb_list(1, "recc-implementation/struct_entity.h"), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_KEY_VALUE_PAIR, make_beb_list(2, "struct entity_relationship", "struct struct_entity_ptr_list *"), make_beb_list(2, "recc-implementation/struct_entity_relationship.h", "generated/struct_struct_entity_ptr_list.h"), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_KEY_VALUE_PAIR, make_beb_list(2, "unsigned char *", "unsigned char *"), make_beb_list(0), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_LIST, make_beb_list(1, "unsigned char *"), make_beb_list(0), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_LIST, make_beb_list(1, "unsigned char"), make_beb_list(0), make_beb_list(0))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_LIST, make_beb_list(1, "struct entity *"), make_beb_list(0), make_beb_list(1, "recc-implementation/struct_entity.h"))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_LIST, make_beb_list(1, "struct entity_relationship"), make_beb_list(0), make_beb_list(1, "recc-implementation/struct_entity_relationship.h"))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_LIST, make_beb_list(1, "struct struct_entity_ptr_list *"), make_beb_list(0), make_beb_list(1, "generated/struct_struct_entity_ptr_list.h"))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_BINARY_SEARCH, make_beb_list(1, "struct unsigned_char_ptr_to_struct_entity_ptr_key_value_pair"), make_beb_list(0), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair.h"))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_BINARY_SEARCH, make_beb_list(1, "struct unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair"), make_beb_list(0), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair.h"))));
	cleanup_files_for_type(make_generated_files(b, make_files_for_type(b, GENERATED_FILE_TYPE_BINARY_SEARCH, make_beb_list(1, "struct struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair"), make_beb_list(0), make_beb_list(1, "generated/struct_struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair.h"))));

	bootstrap_state_output_makefiles(b, "");
	bootstrap_state_destroy(b);
	return 0;
}
