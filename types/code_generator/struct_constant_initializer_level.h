#ifndef __TYPES_CODE_GENERATOR_struct_constant_initializer_level_H__
#define __TYPES_CODE_GENERATOR_struct_constant_initializer_level_H__
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

#ifndef __TYPES_CODE_GENERATOR_struct_compile_time_constant_H__
#include "struct_compile_time_constant.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_struct_constant_initializer_level_ptr_list_H__
#include "../../types/data-structures/struct_struct_constant_initializer_level_ptr_list.h"
#endif

struct constant_initializer_level{
	struct compile_time_constant * constant; /*  Will be set only if this is not an initializer list */
	struct struct_constant_initializer_level_ptr_list children;
};

#endif
