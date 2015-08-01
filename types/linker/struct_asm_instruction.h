#ifndef __TYPES_LINKER_struct_asm_instruction_h__
#define __TYPES_LINKER_struct_asm_instruction_h__
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

#ifndef __TYPES_LEXER_struct_asm_lexer_token_H__
#include "../lexer/struct_asm_lexer_token.h"
#endif

struct asm_instruction{
	struct asm_lexer_token * op_token;
	struct asm_lexer_token * rx_token;
	struct asm_lexer_token * ry_token;
	struct asm_lexer_token * rz_token;
	struct asm_lexer_token * rw_token;
	struct asm_lexer_token * identifier_token;
	struct asm_lexer_token * number_token;
	unsigned int number_token_is_negative;
	unsigned int post_linking_offset; /* The offset (in # of 4 byte words) of this instruction after we've considered instructions that need to be re-written by the linker */
};

#endif
