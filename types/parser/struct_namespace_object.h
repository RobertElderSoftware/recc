#ifndef __TYPES_PARSER_struct_namespace_object_H__
#define __TYPES_PARSER_struct_namespace_object_H__
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


#ifndef __unsigned_char_ptr_to_struct_namespace_object_ptr_map__H__DEFINED__
#include "../../data-structures/unsigned_char_ptr_to_struct_namespace_object_ptr_map.h"
#endif
#ifndef __struct_normalized_declaration_element_ptr_list__H__DEFINED__
#include "../../data-structures/struct_normalized_declaration_element_ptr_list.h"
#endif
#ifndef __TYPES_PARSER_struct_scope_level_H__
#include "struct_scope_level.h"
#endif
#ifndef __TYPES_PARSER_struct_namespace_object_H__
#include "struct_namespace_object.h"
#endif
#ifndef __TYPES_PARSER_enum_object_location_H__
#include "enum_object_location.h"
#endif

struct namespace_object{
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * children;
	struct struct_normalized_declaration_element_ptr_list elements; /* All of the declaration elements related to one named symbol */
	struct scope_level * scope_level;
	struct namespace_object * previous; /* The previous declaration element (previous param, or previous local stack varable) */
	struct namespace_object * next; /* The previous declaration element (previous param, or previous local stack varable) */
	enum object_location object_location;
	unsigned int first_load;
	int offset;
	unsigned int pad;
};

#endif
