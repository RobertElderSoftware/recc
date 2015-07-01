#ifndef __LEXER_H__
#define __LEXER_H__
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

#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif
#ifndef __STDLIB_H_DEFINED__
#include <stdlib.h>
#endif
#ifndef __IO_H__
#include "io.h"
#endif
#ifndef __struct_c_lexer_token_ptr_list__H__DEFINED__
#include "data-structures/struct_c_lexer_token_ptr_list.h"
#endif
#ifndef __struct_build_script_lexer_token_ptr_list__H__DEFINED__
#include "data-structures/struct_build_script_lexer_token_ptr_list.h"
#endif
#ifndef __struct_asm_lexer_token_ptr_list__H__DEFINED__
#include "data-structures/struct_asm_lexer_token_ptr_list.h"
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "data-structures/unsigned_char_list.h"
#endif
#ifndef __MEMORY_POOLER__H__DEFINED__
#include "data-structures/memory_pooler.h"
#endif

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

enum build_script_token_type {
	B_SPACE,
	B_NEWLINE,
	B_CODE_GENERATE,
	B_PREPROCESS,
	B_LINK,
	B_SYMBOLS,
	B_TO,
	B_SEMICOLON_CHAR,
	B_COMMA_CHAR,
	B_FILENAME
};

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

struct common_lexer_state {
	unsigned char * buf;
	unsigned char * filename;
	struct memory_pooler_collection * memory_pooler_collection;
	struct unsigned_char_list * buffered_output;
	unsigned int position;
	unsigned int current_line;
	unsigned int buffer_size;
	unsigned int pad;
};

struct c_lexer_state{
	struct struct_c_lexer_token_ptr_list tokens;
	struct common_lexer_state c;
};

struct build_script_lexer_state{
	struct struct_build_script_lexer_token_ptr_list tokens;
	struct common_lexer_state c;
};

struct asm_lexer_state{
	struct struct_asm_lexer_token_ptr_list tokens;
	struct common_lexer_state c;
};

struct c_lexer_token{
	unsigned char * first_byte;
	unsigned char * last_byte;
	enum c_token_type type;
	unsigned int pad;
};

struct build_script_lexer_token{
	unsigned char * first_byte;
	unsigned char * last_byte;
	enum build_script_token_type type;
	unsigned int pad;
};

struct asm_lexer_token{
	unsigned char * first_byte;
	unsigned char * last_byte;
	enum asm_token_type type;
	unsigned int pad;
};

int lex_c(struct c_lexer_state *, unsigned char *, unsigned char *, unsigned int);
int lex_build_script(struct build_script_lexer_state *, unsigned char *, unsigned char *, unsigned int);
int lex_asm(struct asm_lexer_state *, unsigned char *, unsigned char *, unsigned int);
void destroy_c_lexer_state(struct c_lexer_state *);
void destroy_build_script_lexer_state(struct build_script_lexer_state *);
void destroy_asm_lexer_state(struct asm_lexer_state *);
const char ** get_c_token_type_names(void);
const char ** get_build_script_token_type_names(void);
const char ** get_asm_token_type_names(void);
unsigned int count_newlines_in_comment(struct c_lexer_token *);

#endif
