#ifndef __TYPES_PRELOADER_enum_preloader_instruction_type_H__
#define __TYPES_PRELOADER_enum_preloader_instruction_type_H__
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

enum preloader_instruction_type {
	ADD_INSTRUCTION,
	SUB_INSTRUCTION,
	MUL_INSTRUCTION,
	DIV_INSTRUCTION,
	BEQ_INSTRUCTION,
	BLT_INSTRUCTION,
	LOA_INSTRUCTION,
	STO_INSTRUCTION,
	LL_INSTRUCTION,
	AND_INSTRUCTION,
	OR_INSTRUCTION,
	NOT_INSTRUCTION,
	SHR_INSTRUCTION,
	SHL_INSTRUCTION,
	DW_INSTRUCTION,
	SW_INSTRUCTION
};

#endif
