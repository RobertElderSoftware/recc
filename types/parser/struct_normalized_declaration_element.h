#ifndef __TYPES_PARSER_struct_normalized_declaration_element_H__
#define __TYPES_PARSER_struct_normalized_declaration_element_H__
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

#ifndef __TYPES_PARSER_struct_normalized_declaration_set_H__
#include "struct_normalized_declaration_set.h"
#endif
#ifndef __TYPES_PARSER_struct_normalized_declarator_H__
#include "struct_normalized_declarator.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_struct_normalized_specifier_ptr_list_H__
#include "../../data-structures/struct_struct_normalized_specifier_ptr_list.h"
#endif

struct normalized_declaration_element{
	/*  In the normalized_declaration_element, the normalized_declarator can
	    ONLY HAVE ONE declarator in whatever declarator list it
	    uses to describe the declaration.
	*/
	struct normalized_declaration_set * normalized_declaration_set; /* the declaration set (declarator list) this element belongs to */
	struct normalized_declarator * normalized_declarator;
	struct struct_normalized_specifier_ptr_list * normalized_specifiers;
};

#endif
