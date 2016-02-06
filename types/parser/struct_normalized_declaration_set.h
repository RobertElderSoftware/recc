#ifndef TYPES_PARSER_struct_normalized_declaration_set_H_
#define TYPES_PARSER_struct_normalized_declaration_set_H_
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

#ifndef TYPES_PARSER_struct_parser_node_H_
#include "struct_parser_node.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_struct_normalized_specifier_ptr_list_H_
#include "../../types/data-structures/struct_struct_normalized_specifier_ptr_list.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_struct_normalized_declarator_ptr_list_H_
#include "../../types/data-structures/struct_struct_normalized_declarator_ptr_list.h"
#endif
#ifndef TYPES_PARSER_enum_normalized_declaration_type_H_
#include "enum_normalized_declaration_type.h"
#endif

struct normalized_declaration_set;

struct normalized_declaration_set{
	/*  This is essentially a wrapper for a parser_node that
	    happens to be one of the common methods of declaring things.
	    The important point is that in the normalized_declaration_set,
	    the parser_node can have a declarator list with more than one
	    declarator in whatever declarator list it uses.
	    For example, int a, b, f(); has an init_declarator_list that
	    declares three different symbols.
	*/
	struct parser_node * set; /* a declaration, struct_declaration, parameter_declaration etc. */
	struct normalized_declaration_set * parent_normalized_declaration_set; /* Used to point to the main enum, struct or union declaration from an inner declaration. */
	struct struct_normalized_specifier_ptr_list * normalized_specifiers; /*  This entire list of specifiers gets applied to */
	struct struct_normalized_declarator_ptr_list * normalized_declarators; /*  each of these declarators */
	enum normalized_declaration_type type;
	unsigned int pad;
};

#endif
