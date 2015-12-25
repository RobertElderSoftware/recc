#ifndef TYPES_PARSER_struct_normalized_declarator_H__
#define TYPES_PARSER_struct_normalized_declarator_H__
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

#ifndef TYPES_PARSER_struct_parser_node_H__
#include "struct_parser_node.h"
#endif
#ifndef TYPES_PARSER_enum_normalized_declarator_type_H__
#include "enum_normalized_declarator_type.h"
#endif

struct normalized_declarator{
	/*  
	    Used for abstracting away struct_declarator_list, init_declarator_list etc.
	*/
	struct parser_node * declarator;
	enum normalized_declarator_type type;
	unsigned int pad;
};

#endif
