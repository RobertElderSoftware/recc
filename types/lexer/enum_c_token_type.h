#ifndef TYPES_LEXER_enum_c_token_type_H_
#define TYPES_LEXER_enum_c_token_type_H_
/*
    Copyright 2016 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/

#define NUM_C_TOKEN_TYPES 90

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
