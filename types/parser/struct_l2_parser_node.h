#ifndef TYPES_PARSER_struct_l2_parser_node_H__
#define TYPES_PARSER_struct_l2_parser_node_H__
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

#ifndef TYPES_PARSER_enum_l2_node_type_H__
#include "enum_l2_node_type.h"
#endif
#ifndef TYPES_LEXER_struct_l2_lexer_token_H__
#include "../../types/lexer/struct_l2_lexer_token.h"
#endif

struct l2_parser_node;

struct l2_parser_node{
	struct l2_parser_node * next;
	struct l2_parser_node * first_child;
	struct l2_lexer_token * l2_lexer_token;
	enum l2_node_type type;
	unsigned int pad;
};

#endif
