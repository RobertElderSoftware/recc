#ifndef TYPES_PARSER_struct_l2_parser_state_H_
#define TYPES_PARSER_struct_l2_parser_state_H_
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

#ifndef TYPES_struct_memory_pool_collection_H_
#include "../recc-implementation/struct_memory_pool_collection.h"
#endif
#ifndef TYPES_LEXER_struct_l2_lexer_state_H_
#include "../lexer/struct_l2_lexer_state.h"
#endif
#ifndef TYPES_PARSER_struct_l2_parser_node_H_
#include "struct_l2_parser_node.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_unsigned_char_list_H_
#include "../data-structures/struct_unsigned_char_list.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_struct_l2_parser_operation_stack_H_
#include "../data-structures/struct_struct_l2_parser_operation_stack.h"
#endif


struct l2_parser_state{
	struct memory_pool_collection * memory_pool_collection;
	struct l2_lexer_state * l2_lexer_state;
	struct l2_parser_node * top_node;
	struct unsigned_char_list * buffered_output;
	struct struct_l2_parser_operation_stack operation_stack;
	unsigned char * buff;
	unsigned int tokens_position;
	unsigned int line_number;
};

#endif
