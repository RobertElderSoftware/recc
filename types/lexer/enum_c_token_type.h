#ifndef __TYPES_LEXER_enum_c_token_type_H__
#define __TYPES_LEXER_enum_c_token_type_H__
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

enum c_token_type {
	SPACE,
	NEWLINE,
	COMMENT,
	AUTO,
	BREAK,
	CASE,
	CHAR,
	CONST,
	CONTINUE,
	DEFAULT,
	DO,
	DOUBLE,
	ELSE,
	ENUM,
	EXTERN,
	FLOAT,
	FOR,
	GOTO,
	IF,
	INT,
	LONG,
	REGISTER,
	RETURN,
	SHORT,
	SIGNED,
	SIZEOF,
	STATIC,
	STRUCT,
	SWITCH,
	TYPEDEF,
	UNION,
	UNSIGNED,
	VOID,
	VOLATILE,
	WHILE,
	IDENTIFIER,
	CONSTANT_HEX,
	CONSTANT_EXPONENT,
	CONSTANT_FLOAT_SMALL,
	CONSTANT_FLOAT_LARGE,
	CONSTANT_DECIMAL,
	CONSTANT_CHARACTER,
	STRING_LITERAL,
	ELLIPSIS,
	RIGHT_ASSIGN,
	LEFT_ASSIGN,
	ADD_ASSIGN,
	SUB_ASSIGN,
	MUL_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN,
	AND_ASSIGN,
	XOR_ASSIGN,
	OR_ASSIGN,
	RIGHT_OP,
	LEFT_OP,
	INC_OP,
	DEC_OP,
	PTR_OP,
	AND_OP,
	OR_OP,
	LE_OP,
	GE_OP,
	EQ_OP,
	NE_OP,
	SEMICOLON_CHAR,
	OPEN_BRACE_CHAR,
	CLOSE_BRACE_CHAR,
	OPEN_SQUARE_BRACKET_CHAR,
	CLOSE_SQUARE_BRACKET_CHAR,
	COMMA_CHAR,
	COLON_CHAR,
	EQUALS_CHAR,
	OPEN_PAREN_CHAR,
	CLOSE_PAREN_CHAR,
	DOT_CHAR,
	AMPERSAND_CHAR,
	EXCLAMATION_MARK_CHAR,
	TILDE_CHAR,
	MINUS_CHAR,
	PLUS_CHAR,
	MULTIPLY_CHAR,
	DIVIDE_CHAR,
	PERCENT_CHAR,
	OPEN_ANGLE_BRACKET_CHAR,
	CLOSE_ANGLE_BRACKET_CHAR,
	CARET_CHAR,
	PIPE_CHAR,
	QUESTION_MARK_CHAR,
	NUMBER_SIGN_CHAR
};

#endif
