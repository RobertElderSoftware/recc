#ifndef TYPES_PREPROCESSOR_struct_preprocessor_state_H_
#define TYPES_PREPROCESSOR_struct_preprocessor_state_H_
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

struct preprocessor_state{
	struct struct_unsigned_char_list_ptr_list tokenizable_input_buffers;  /*  The characters underlying all of our tokens */
	struct struct_preprocessor_if_branch_ptr_list preprocessor_if_branches;
	struct struct_preprocessor_file_context_ptr_list file_contexts;
	struct memory_pool_collection * memory_pool_collection;
	struct struct_c_lexer_state_ptr_list c_lexer_states;
	struct c_lexer_token * comma_token;
	struct c_lexer_token * space_token;
	struct struct_c_lexer_token_ptr_list created_tokens;
	struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_map special_macros;
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map macro_map;
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map disabled_macros;
	struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map disabled_tokens;
};

#endif
