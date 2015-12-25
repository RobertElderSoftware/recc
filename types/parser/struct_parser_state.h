#ifndef TYPES_PARSER_struct_parser_state_H__
#define TYPES_PARSER_struct_parser_state_H__
/*
    Copyright 2015 Robert Elder Software Inc.
    
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

struct parser_state{
	struct memory_pool_collection * memory_pool_collection;
	struct c_lexer_state * c_lexer_state;
	struct scope_level * top_scope;
	struct parser_node * top_node;
	struct unsigned_char_list * buffered_output;
	struct struct_parser_operation_stack operation_stack;
	struct unsigned_char_ptr_to_struct_constant_description_ptr_map constant_map;
	unsigned char * buff;
	unsigned int tokens_position;
	unsigned int line_number;
	unsigned int current_scope_depth;
	unsigned int pad;
};

#endif
