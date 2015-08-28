#ifndef __TYPES_PARSER_struct_scope_level_H__
#define __TYPES_PARSER_struct_scope_level_H__
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


#ifndef __TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_list_H__
#include "../../types/data-structures/struct_unsigned_char_ptr_list.h"
#endif
#ifndef __TYPES_PARSER_struct_namespace_object_H__
#include "struct_namespace_object.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_namespace_object_ptr_map_H__
#include "../../types/data-structures/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_map.h"
#endif

struct scope_level;

struct scope_level{
	struct scope_level * parent_scope;
	struct scope_level ** scopes;
	struct unsigned_char_ptr_list evaluate_labels; /*  For continue statements  */
	struct unsigned_char_ptr_list end_labels;      /*  For break statements  */
	struct namespace_object * current_function;
	struct namespace_object * first_local_object;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map tag_namespace;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map label_namespace;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map identifier_namespace;
	unsigned int num_sub_scopes;
	unsigned int pad;
};

#endif
