#ifndef __TYPES_LEXER_enum_asm_token_type_H__
#define __TYPES_LEXER_enum_asm_token_type_H__
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

enum asm_token_type {
	A_SPACE,
	A_NEWLINE,
	A_ASM_COMMENT,
	A_IDENTIFIER,
	A_COLON_CHAR,
	A_COMMA_CHAR,
	A_MINUS_CHAR,
	A_OFFSET,
	A_RELOCATABLE,
	A_IMPLEMENTS,
	A_REQUIRES,
	A_INTERNAL,
	A_EXTERNAL,
	A_REGISTER,
	A_CONSTANT_HEX,
	A_CONSTANT_DECIMAL,
	A_ADD,
	A_SUB,
	A_MUL,
	A_DIV,
	A_BEQ,
	A_BLT,
	A_LOA,
	A_STO,
	A_LL,
	A_AND,
	A_OR,
	A_NOT,
	A_SHR,
	A_SHL,
	A_DW,
	A_SW
};

#endif
