#ifndef __TYPES_CODE_GENERATOR_struct_switch_frame_H__
#define __TYPES_CODE_GENERATOR_struct_switch_frame_H__
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

#ifndef __TYPES_DATA_STRUCTURES_struct_unsigned_int_list_H__
#include "../../types/data-structures/struct_unsigned_int_list.h"
#endif

struct switch_frame{
	unsigned int condition_index;
	unsigned int has_default;
	struct unsigned_int_list values;
};

#endif
