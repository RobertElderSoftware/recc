#ifndef __TYPES_struct_build_target_H__
#define __TYPES_struct_build_target_H__
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
#ifndef __TYPES_enum_build_target_type_H__
#include "enum_build_target_type.h"
#endif
#ifndef __TYPES_struct_l0_build_info_H__
#include "struct_l0_build_info.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_build_target_ptr_map_H__
#include "../types/data-structures/struct_unsigned_char_ptr_to_struct_build_target_ptr_map.h"
#endif

struct build_target{
	struct l0_build_info * l0_info;
	unsigned char * name;
	struct unsigned_char_ptr_to_struct_build_target_ptr_map children; /*  Things this target depends on */
	struct unsigned_char_ptr_to_struct_build_target_ptr_map parents;  /*  Things that depend on this target */
	enum build_target_type type;
	unsigned int satisfied;     /*  True when this target has already been built. */
	unsigned int is_relocatable;
	unsigned int linking_offset;
};

#endif
