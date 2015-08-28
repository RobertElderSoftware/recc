#ifndef __TYPES_PARSER_struct_normalized_declaration_set_H__
#define __TYPES_PARSER_struct_normalized_declaration_set_H__
/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/

#ifndef __TYPES_PARSER_struct_parser_node_H__
#include "struct_parser_node.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_struct_normalized_specifier_ptr_list_H__
#include "../../types/data-structures/struct_struct_normalized_specifier_ptr_list.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_struct_normalized_declarator_ptr_list_H__
#include "../../types/data-structures/struct_struct_normalized_declarator_ptr_list.h"
#endif
#ifndef __TYPES_PARSER_enum_normalized_declaration_type_H__
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
