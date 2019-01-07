

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

/*GETS_REPLACED_WITH_INCLUDES*/

struct linker_state{
	struct struct_linker_region_list regions;
	struct struct_l2_item_ptr_list created_sw_items;
	struct struct_linker_file_ptr_list linker_files;
	struct struct_linker_file_ptr_list reordered_linker_files;
	struct unsigned_char_list file_output;
	struct unsigned_char_list symbol_output;
	struct unsigned_char_list l2_lexer_output;
	struct memory_pool_collection * memory_pool_collection;
	struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map external_symbols;
	struct unsigned_char_ptr_list * in_files;
	unsigned char * symbol_file;
	unsigned char * out_file;
	unsigned int starting_offset;
	enum entity_type entity_type;
	unsigned char * offset;
	unsigned int page_align_permission_regions;
	unsigned int only_metadata;
	unsigned int next_internal_identifier_id;
};
