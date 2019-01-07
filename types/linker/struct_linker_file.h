

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

struct linker_file{ /*  Represents everything that comes from one translation unit (or assembly file) */
	unsigned char * source_file;
	struct unsigned_char_list * file_input;
	struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map internal_symbols;
	struct struct_linker_object_ptr_list object_declarations;
	struct struct_l2_item_ptr_list l2_items;
	unsigned int unused_words_before;
	unsigned int offset_declared;
	enum relocation_type relocation_type;
	unsigned int current_line;
	unsigned int linker_file_post_linking_offset; /* The offset to the instruction of this symbol after considering instructions re-written by the linker */
	unsigned int pad;
};
