#ifndef __TYPES_PARSER_struct_type_description_H__
#define __TYPES_PARSER_struct_type_description_H__
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

#ifndef __TYPES_PARSER_enum_value_type_H__
#include "enum_value_type.h"
#endif
#ifndef __TYPES_PARSER_struct_normalized_declaration_element_H__
#include "struct_normalized_declaration_element.h"
#endif
#ifndef __TYPES_PARSER_struct_normalized_declarator_H__
#include "struct_normalized_declarator.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_struct_normalized_specifier_ptr_list_H__
#include "../../data-structures/struct_struct_normalized_specifier_ptr_list.h"
#endif
#ifndef __TYPES_PARSER_struct_parser_node_H__
#include "struct_parser_node.h"
#endif
#ifndef __TYPES_PARSER_struct_scope_level_H__
#include "struct_scope_level.h"
#endif

struct type_description{
        struct normalized_declaration_element * source_element; /* Needed for anonymous struct/union/enum */
        struct normalized_declarator * declarator;
        struct struct_normalized_specifier_ptr_list * specifiers;
        struct parser_node * context;
        struct scope_level * source_scope_level;
        enum value_type value_type;
        unsigned int pad;
};

#endif
