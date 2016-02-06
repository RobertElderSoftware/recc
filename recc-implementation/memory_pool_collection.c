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

#include "memory_pool_collection.h"

#define NON_IDENTIFIER_CHAR "[^a-zA-Z0-9_]"
#define C_COMMENT "(/\\*([^\\*]|\\*[^/])*\\*/)"
#define C_NEWLINE "(\\n)"
#define C_SPACE "(\\t| )+"
#define DECIMAL "([0-9])"
#define EXPONENT "([Ee][\\+\\-]?" DECIMAL "+)"
#define C_FLOAT "([fFlL])"
#define C_INTEGER "([uUlL]*)"
#define CHARACTER "([^']+|\\\\')"
#define STR_CHARACTER "([^\"\\\\]|\\\\.)"
#define STRING "\"(" STR_CHARACTER ")*\""
#define CRAZY_STRING STRING "(((" C_NEWLINE "|" C_SPACE "|" C_COMMENT ")*)?" STRING ")*"
#define LARGE_FLOAT DECIMAL "+\\." DECIMAL "*" EXPONENT "?" C_FLOAT "?"
#define SMALL_FLOAT "\\." DECIMAL "+" EXPONENT "?" C_FLOAT "?"

void add_c_token_regex(struct memory_pool_collection *, struct regex_compiler_state **, const char *, enum c_token_type, unsigned int *);
void add_c_token_regex(struct memory_pool_collection * m, struct regex_compiler_state ** compiler_states, const char * c, enum c_token_type t, unsigned int * count){
	struct regex_compiler_state * comp = (struct regex_compiler_state *)malloc(sizeof(struct regex_compiler_state));
	regex_compiler_state_create(comp, m, (unsigned char *)c, (unsigned int)t);
	regex_compiler_state_compile(comp);
	assert(!comp->errors);
	*count = *count + 1;
	compiler_states[*count -1] = comp;
}

void initialize_c_token_matcher(struct memory_pool_collection * m);

void initialize_c_token_matcher(struct memory_pool_collection * m){
	unsigned int num_regexes = 0;

	m->c_token_regexes = (struct regex_compiler_state **)malloc(sizeof(struct regex_compiler_state *) * NUM_C_TOKEN_TYPES);

	add_c_token_regex(m, m->c_token_regexes, C_SPACE,                           SPACE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, C_NEWLINE,                         NEWLINE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, C_COMMENT,                         COMMENT, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "auto" NON_IDENTIFIER_CHAR,        AUTO, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "break" NON_IDENTIFIER_CHAR,       BREAK, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "case" NON_IDENTIFIER_CHAR,        CASE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "char" NON_IDENTIFIER_CHAR,        CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "const" NON_IDENTIFIER_CHAR,       CONST, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "continue" NON_IDENTIFIER_CHAR,    CONTINUE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "default" NON_IDENTIFIER_CHAR,     DEFAULT, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "do" NON_IDENTIFIER_CHAR,          DO, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "double" NON_IDENTIFIER_CHAR,      DOUBLE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "else" NON_IDENTIFIER_CHAR,        ELSE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "enum" NON_IDENTIFIER_CHAR,        ENUM, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "extern" NON_IDENTIFIER_CHAR,      EXTERN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "float" NON_IDENTIFIER_CHAR,       FLOAT, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "for" NON_IDENTIFIER_CHAR,         FOR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "goto" NON_IDENTIFIER_CHAR,        GOTO, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "if" NON_IDENTIFIER_CHAR,          IF, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "int" NON_IDENTIFIER_CHAR,         INT, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "long" NON_IDENTIFIER_CHAR,        LONG, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "register" NON_IDENTIFIER_CHAR,    REGISTER, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "return" NON_IDENTIFIER_CHAR,      RETURN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "short" NON_IDENTIFIER_CHAR,       SHORT, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "signed" NON_IDENTIFIER_CHAR,      SIGNED, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "sizeof" NON_IDENTIFIER_CHAR,      SIZEOF, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "static" NON_IDENTIFIER_CHAR,      STATIC, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "struct" NON_IDENTIFIER_CHAR,      STRUCT, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "switch" NON_IDENTIFIER_CHAR,      SWITCH, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "typedef" NON_IDENTIFIER_CHAR,     TYPEDEF, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "union" NON_IDENTIFIER_CHAR,       UNION, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "unsigned" NON_IDENTIFIER_CHAR,    UNSIGNED, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "void" NON_IDENTIFIER_CHAR,        VOID, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "volatile" NON_IDENTIFIER_CHAR,    VOLATILE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "while" NON_IDENTIFIER_CHAR,       WHILE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "[a-zA-Z_][a-zA-Z_0-9]*",          IDENTIFIER, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "0[xX][a-fA-F0-9]+" C_INTEGER,     CONSTANT_HEX, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, DECIMAL "+" EXPONENT C_FLOAT "?",  CONSTANT_EXPONENT, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, SMALL_FLOAT,                       CONSTANT_FLOAT_SMALL, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, LARGE_FLOAT,                       CONSTANT_FLOAT_LARGE, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "[0-9]+" C_INTEGER,                CONSTANT_DECIMAL, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "'" CHARACTER "'",                 CONSTANT_CHARACTER, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, CRAZY_STRING,                      STRING_LITERAL, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\.\\.\\.",                       ELLIPSIS, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, ">>=",                             RIGHT_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "<<=",                             LEFT_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\+=",                            ADD_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\-=",                            SUB_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\*=",                            MUL_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "/=",                              DIV_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "%=",                              MOD_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "&=",                              AND_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\^=",                            XOR_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\|=",                            OR_ASSIGN, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, ">>",                              RIGHT_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "<<",                              LEFT_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\+\\+",                          INC_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\-\\-",                          DEC_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\->",                            PTR_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "&&",                              AND_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\|\\|",                          OR_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "<=",                              LE_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, ">=",                              GE_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "==",                              EQ_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "!=",                              NE_OP, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, ";",                               SEMICOLON_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "{",                               OPEN_BRACE_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "}",                               CLOSE_BRACE_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\[",                             OPEN_SQUARE_BRACKET_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\]",                             CLOSE_SQUARE_BRACKET_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, ",",                               COMMA_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, ":",                               COLON_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "=",                               EQUALS_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\(",                             OPEN_PAREN_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\)",                             CLOSE_PAREN_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\.",                             DOT_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "&",                               AMPERSAND_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "!",                               EXCLAMATION_MARK_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "~",                               TILDE_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\-",                             MINUS_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\+",                             PLUS_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\*",                             MULTIPLY_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "/",                               DIVIDE_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "%",                               PERCENT_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "<",                               OPEN_ANGLE_BRACKET_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, ">",                               CLOSE_ANGLE_BRACKET_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\^",                             CARET_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\|",                             PIPE_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "\\?",                             QUESTION_MARK_CHAR, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "#",                               NUMBER_SIGN_CHAR, &num_regexes);

	regex_matcher_state_create(&m->c_token_matcher, m->c_token_regexes, num_regexes);
}

void add_l2_token_regex(struct memory_pool_collection *, struct regex_compiler_state **, const char *, enum l2_token_type, unsigned int *);
void add_l2_token_regex(struct memory_pool_collection * m, struct regex_compiler_state ** compiler_states, const char * c, enum l2_token_type t, unsigned int * count){
	struct regex_compiler_state * comp = (struct regex_compiler_state *)malloc(sizeof(struct regex_compiler_state));
	regex_compiler_state_create(comp, m, (unsigned char *)c, (unsigned int)t);
	regex_compiler_state_compile(comp);
	assert(!comp->errors);
	*count = *count + 1;
	compiler_states[*count -1] = comp;
}

void initialize_l2_token_matcher(struct memory_pool_collection * m);

void initialize_l2_token_matcher(struct memory_pool_collection * m){
	unsigned int num_regexes = 0;

	m->l2_token_regexes = (struct regex_compiler_state **)malloc(sizeof(struct regex_compiler_state *) * NUM_L2_TOKEN_TYPES);

	add_l2_token_regex(m, m->l2_token_regexes, C_SPACE,                            L2_SPACE, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, C_NEWLINE,                          L2_NEWLINE, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, ";.*",                              L2_COMMENT, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "[a-zA-Z_][a-zA-Z_0-9]*",           L2_IDENTIFIER, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, ":",                                L2_COLON_CHAR, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, ",",                                L2_COMMA_CHAR, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "\\-",                              L2_MINUS_CHAR, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "OFFSET" NON_IDENTIFIER_CHAR,       L2_OFFSET, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "RELOCATABLE" NON_IDENTIFIER_CHAR,  L2_RELOCATABLE, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "IMPLEMENTS" NON_IDENTIFIER_CHAR,   L2_IMPLEMENTS, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "IMPLEMENTED" NON_IDENTIFIER_CHAR,  L2_IMPLEMENTED, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "REQUIRES" NON_IDENTIFIER_CHAR,     L2_REQUIRES, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "REQUIRED" NON_IDENTIFIER_CHAR,     L2_REQUIRED, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "INTERNAL" NON_IDENTIFIER_CHAR,     L2_INTERNAL, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "EXTERNAL" NON_IDENTIFIER_CHAR,     L2_EXTERNAL, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "(r[0-9]+|SP|PC|WR|FP|ZR|FR)" NON_IDENTIFIER_CHAR,      L2_REGISTER, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "FUNCTION" NON_IDENTIFIER_CHAR,     L2_FUNCTION, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "VARIABLE" NON_IDENTIFIER_CHAR,     L2_VARIABLE, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "CONSTANT" NON_IDENTIFIER_CHAR,     L2_CONSTANT, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "STRING" NON_IDENTIFIER_CHAR,       L2_STRING, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "UNRESOLVED" NON_IDENTIFIER_CHAR,   L2_UNRESOLVED, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "REGION" NON_IDENTIFIER_CHAR,       L2_REGION, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "START" NON_IDENTIFIER_CHAR,        L2_START, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "END" NON_IDENTIFIER_CHAR,          L2_END, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "PERMISSION" NON_IDENTIFIER_CHAR,   L2_PERMISSION, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "0x[0-9a-fA-F]+",                   L2_CONSTANT_HEX, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "[0-9]+",                           L2_CONSTANT_DECIMAL, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "add" NON_IDENTIFIER_CHAR,          L2_ADD, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "sub" NON_IDENTIFIER_CHAR,          L2_SUB, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "mul" NON_IDENTIFIER_CHAR,          L2_MUL, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "div" NON_IDENTIFIER_CHAR,          L2_DIV, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "beq" NON_IDENTIFIER_CHAR,          L2_BEQ, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "blt" NON_IDENTIFIER_CHAR,          L2_BLT, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "loa" NON_IDENTIFIER_CHAR,          L2_LOA, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "sto" NON_IDENTIFIER_CHAR,          L2_STO, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "ll" NON_IDENTIFIER_CHAR,           L2_LL, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "and" NON_IDENTIFIER_CHAR,          L2_AND, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "or" NON_IDENTIFIER_CHAR,           L2_OR, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "not" NON_IDENTIFIER_CHAR,          L2_NOT, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "shr" NON_IDENTIFIER_CHAR,          L2_SHR, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "shl" NON_IDENTIFIER_CHAR,          L2_SHL, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "DW" NON_IDENTIFIER_CHAR,           L2_DW, &num_regexes);
	add_l2_token_regex(m, m->l2_token_regexes, "SW" NON_IDENTIFIER_CHAR,           L2_SW, &num_regexes);

	regex_matcher_state_create(&m->l2_token_matcher, m->l2_token_regexes, num_regexes);
}

void memory_pool_collection_create(struct memory_pool_collection * m){
	m->struct_regex_computation_node_pool = (struct struct_regex_computation_node_memory_pool *)malloc(sizeof(struct struct_regex_computation_node_memory_pool));
	m->struct_regex_parser_node_pool = (struct struct_regex_parser_node_memory_pool *)malloc(sizeof(struct struct_regex_parser_node_memory_pool));
	m->struct_c_lexer_token_pool = (struct struct_c_lexer_token_memory_pool *)malloc(sizeof(struct struct_c_lexer_token_memory_pool));
	m->struct_parser_node_pool = (struct struct_parser_node_memory_pool*)malloc(sizeof(struct struct_parser_node_memory_pool));
	m->struct_l2_parser_node_pool = (struct struct_l2_parser_node_memory_pool*)malloc(sizeof(struct struct_l2_parser_node_memory_pool));
	m->struct_l2_lexer_token_pool = (struct struct_l2_lexer_token_memory_pool*)malloc(sizeof(struct struct_l2_lexer_token_memory_pool));
	m->struct_type_description_pool = (struct struct_type_description_memory_pool*)malloc(sizeof(struct struct_type_description_memory_pool));
	m->struct_l2_item_pool = (struct struct_l2_item_memory_pool*)malloc(sizeof(struct struct_l2_item_memory_pool));
	m->struct_linker_symbol_pool = (struct struct_linker_symbol_memory_pool*)malloc(sizeof(struct struct_linker_symbol_memory_pool));
	m->heap_pool = (struct heap_memory_pool*)malloc(sizeof(struct heap_memory_pool));
	struct_regex_computation_node_memory_pool_create(m->struct_regex_computation_node_pool);
	struct_regex_parser_node_memory_pool_create(m->struct_regex_parser_node_pool);
	struct_c_lexer_token_memory_pool_create(m->struct_c_lexer_token_pool);
	struct_parser_node_memory_pool_create(m->struct_parser_node_pool);
	struct_l2_parser_node_memory_pool_create(m->struct_l2_parser_node_pool);
	struct_l2_lexer_token_memory_pool_create(m->struct_l2_lexer_token_pool);
	struct_type_description_memory_pool_create(m->struct_type_description_pool);
	struct_l2_item_memory_pool_create(m->struct_l2_item_pool);
	struct_linker_symbol_memory_pool_create(m->struct_linker_symbol_pool);
	heap_memory_pool_create(m->heap_pool);
	initialize_c_token_matcher(m);
	initialize_l2_token_matcher(m);
}

void memory_pool_collection_destroy(struct memory_pool_collection * m){
	unsigned int i;
	struct_c_lexer_token_memory_pool_destroy(m->struct_c_lexer_token_pool);
	struct_parser_node_memory_pool_destroy(m->struct_parser_node_pool);
	struct_l2_parser_node_memory_pool_destroy(m->struct_l2_parser_node_pool);
	struct_l2_lexer_token_memory_pool_destroy(m->struct_l2_lexer_token_pool);
	struct_type_description_memory_pool_destroy(m->struct_type_description_pool);
	struct_linker_symbol_memory_pool_destroy(m->struct_linker_symbol_pool);
	struct_l2_item_memory_pool_destroy(m->struct_l2_item_pool);
	heap_memory_pool_destroy(m->heap_pool);
	free(m->struct_c_lexer_token_pool);
	free(m->struct_parser_node_pool);
	free(m->struct_l2_parser_node_pool);
	free(m->struct_l2_lexer_token_pool);
	free(m->struct_type_description_pool);
	free(m->struct_l2_item_pool);
	free(m->struct_linker_symbol_pool);
	free(m->heap_pool);

	for(i = 0; i < NUM_C_TOKEN_TYPES; i++){
		regex_compiler_state_destroy(m->c_token_regexes[i]);
		free((void*)m->c_token_regexes[i]);
	}

	free((void*)m->c_token_regexes);
	regex_matcher_state_destroy(&m->c_token_matcher);

	for(i = 0; i < NUM_L2_TOKEN_TYPES; i++){
		regex_compiler_state_destroy(m->l2_token_regexes[i]);
		free((void*)m->l2_token_regexes[i]);
	}

	free((void*)m->l2_token_regexes);
	regex_matcher_state_destroy(&m->l2_token_matcher);

	struct_regex_parser_node_memory_pool_destroy(m->struct_regex_parser_node_pool);
	struct_regex_computation_node_memory_pool_destroy(m->struct_regex_computation_node_pool);
	free(m->struct_regex_computation_node_pool);
	free(m->struct_regex_parser_node_pool);
}
