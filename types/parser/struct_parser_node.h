#ifndef TYPES_PARSER_struct_parser_node_H_
#define TYPES_PARSER_struct_parser_node_H_
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

#ifndef TYPES_PARSER_enum_node_type_H_
#include "enum_node_type.h"
#endif
#ifndef TYPES_LEXER_struct_c_lexer_token_H_
#include "../../types/lexer/struct_c_lexer_token.h"
#endif

struct parser_node;

struct parser_node{
	struct parser_node * next;
	struct parser_node * first_child;
	struct c_lexer_token * c_lexer_token;
	enum node_type type;
	unsigned int pad;
};

#endif
