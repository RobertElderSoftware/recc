#ifndef __TYPES_CODE_GENERATOR_struct_type_traversal_H__
#define __TYPES_CODE_GENERATOR_struct_type_traversal_H__
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

#ifndef __TYPES_PARSER_enum_type_class_H__
#include "../../types/parser/enum_type_class.h"
#endif
#ifndef __TYPES_PARSER_struct_type_description_H__
#include "../../types/parser/struct_type_description.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_type_traversal_ptr_list_H__
#include "../../types/data-structures/struct_struct_type_traversal_ptr_list.h"
#endif

struct type_traversal{
	struct type_description * type_description;
	struct struct_type_traversal_ptr_list children;
	unsigned int arity; /* For arrays */
	unsigned int arrays_as_pointers; /* For arrays */
	unsigned char * parent_tag_name; /* structs, enums, unions */
	unsigned char * member_name; /* for structs, enums */
	enum type_class type_class;
	unsigned int pad;
};

#endif
