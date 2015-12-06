#ifndef __TYPES_PRELOADER_struct_preloader_instruction_H__
#define __TYPES_PRELOADER_struct_preloader_instruction_H__
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

#ifndef __TYPES_PRELOADER_enum_preloader_instruction_type_H__
#include "enum_preloader_instruction_type.h"
#endif

struct preloader_instruction{
	unsigned int instruction_registers[3];
	enum preloader_instruction_type type;
	unsigned int constant_is_negative;
	unsigned int constant;
	unsigned int pad;
};

#endif
