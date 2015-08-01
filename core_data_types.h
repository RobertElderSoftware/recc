#ifndef __CORE_DATA_TYPES__H__DEFINED__
#define __CORE_DATA_TYPES__H__DEFINED__
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

/*  Data types required to be defined/declared by our data structures */

#ifndef __TYPES_PARSER_struct_type_description_H__
#include "types/parser/struct_type_description.h"
#endif

#ifndef __TYPES_LINKER_struct_asm_instruction_h__
#include "types/linker/struct_asm_instruction.h"
#endif

#ifndef __TYPES_LINKER_struct_linker_symbol_h__
#include "types/linker/struct_linker_symbol.h"
#endif

#ifndef __TYPES_struct_heap_ptr_index_pair_H__
#include "types/struct_heap_ptr_index_pair.h"
#endif

struct parser_operation;
struct normalized_specifier;
struct normalized_declarator;
struct normalized_declaration_element;
struct memory_pool;
struct build_script_lexer_token;
struct asm_lexer_token;
struct unsigned_char_list;
struct linker_object;
struct asm_lexer_state;
struct namespace_object;
struct scope_level;
struct switch_frame;
struct constant_description;
struct constant_initializer_level;
struct type_traversal;
struct macro_parameter;
struct special_macro_definition;
struct macro_definition;
struct struct_c_lexer_token_ptr_list;
struct if_branch;
struct struct_unsigned_char_list;
struct c_lexer_state;
struct preprocessor_file_context;


/*  Required for sorting and searching map keys TODO:  Move to types folder. */
struct void_ptr_to_unsigned_int_key_value_pair{
	void * key;
	unsigned int value;
};
struct unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair{
	unsigned char * key;
	unsigned char * value;
};
struct unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair{
	unsigned char * key;
	struct macro_parameter * value;
};
struct unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair{
	unsigned char * key;
	struct macro_definition * value;
};
struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair{
	unsigned char * key;
	struct special_macro_definition * value;
};
struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair{
	struct c_lexer_token * key;
	struct c_lexer_token * value;
};
struct struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair{
	struct c_lexer_token * key;
	unsigned char * value;
};
struct unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair{
	unsigned char * key;
	struct constant_description * value;
};
struct unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair{
	unsigned char * key;
	struct linker_symbol * value;
};
struct unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair{
	unsigned char * key;
	struct namespace_object * value;
};

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

enum parser_operation_type{
	INCREMENT_NEWLINE_COUNT,
	PROCESS_CONSTANT,
	ACCEPT_IDENTIFIER,
	ADVANCE_TOKEN_POSITION,
	ADVANCE_PARSER_POSITION,
	INCREMENT_SCOPE_DEPTH,
	DECREMENT_SCOPE_DEPTH,
	RECORD_EXTERN_DECLARATION,
	SAVE_DECLARATION_ELEMENTS,
	NAMESPACE_MODIFICATION,
	CREATE_PARSER_NODE_TREE,
	SET_NEXT_NAMESPACE_OBJECT,
	SET_PREVIOUS_NAMESPACE_OBJECT,
	SET_CURRENT_FUNCTION
};

struct parser_operation{
	void * data;
	enum parser_operation_type type;
	unsigned int pad;
};

struct preloader_instruction{
	unsigned int instruction_registers[3];
	enum preloader_instruction_type type;
	unsigned int constant_is_negative;
	unsigned int constant;
	unsigned int pad;
};


#ifndef __TYPES_struct_memory_pool_collection_H__
#include "types/struct_memory_pool_collection.h"
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

#ifndef __TYPES_LEXER_struct_asm_lexer_token_H__
#include "types/lexer/struct_asm_lexer_token.h"
#endif
#ifndef __TYPES_PARSER_enum_node_type_H__
#include "types/parser/enum_node_type.h"
#endif

struct parser_node;

struct parser_node{
	struct parser_node * next;
	struct parser_node * first_child;
	struct c_lexer_token * c_lexer_token;
	enum node_type type;
	unsigned int pad;
};

#define BITS_PER_BRANCH_DIST  9u
#define BITS_PER_LITERAL     16u
#define BITS_PER_OP_CODE      5u
#define OP_CODE_OFFSET       27u
#define BITS_PER_REGISTER     9u
#define ra_OFFSET            18u
#define rb_OFFSET            (ra_OFFSET - BITS_PER_REGISTER)
#define rc_OFFSET            (rb_OFFSET - BITS_PER_REGISTER)

#define UNSHIFTED_OP_CODE_MASK   (0xFFFFFFFF >> (32u - BITS_PER_OP_CODE))
#define OP_CODE_MASK             (UNSHIFTED_OP_CODE_MASK << OP_CODE_OFFSET)
#define UNSHIFTED_REGISTER_MASK  (0xFFFFFFFF >> (32u - BITS_PER_REGISTER))
#define ra_MASK                  (UNSHIFTED_REGISTER_MASK << ra_OFFSET)
#define rb_MASK                  (UNSHIFTED_REGISTER_MASK << rb_OFFSET)
#define rc_MASK                  (UNSHIFTED_REGISTER_MASK << rc_OFFSET)
#define LITERAL_MASK             (0xFFFFFFFF >> (32u - BITS_PER_LITERAL))
#define BRANCH_DISTANCE_MASK     (0xFFFFFFFF >> (32u - BITS_PER_BRANCH_DIST))

#define BRANCH_DISTANCE_SIGN_BIT 0x100

#define MAX_LL_CONSTANT 0xFFFF
#define MAX_BRANCH_POS 255
#define MAX_BRANCH_NEG 256

#endif
