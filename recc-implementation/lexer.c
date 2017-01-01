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
#include "lexer.h"

#define NON_IDENTIFIER_CHAR "[^a-zA-Z0-9_]"
#define C_COMMENT "(/\\*([^\\*]|\\*[^/])*\\*/)"
#define C_NEWLINE "(\\n)"
#define C_SPACE "(\\t| )+"
#define DECIMAL "([0-9])"
#define EXPONENT "([Ee][\\+\\-]?" DECIMAL "+)"
#define C_FLOAT "([fFlL])"
#define UNSIGNED_SUFFIX "([uU])"
#define LONG_SUFFIX "([lL])"
#define LONG_LONG_SUFFIX "(ll|LL)"
#define EITHER_LONG_SUFFIX "(" LONG_LONG_SUFFIX "|" LONG_SUFFIX ")"
#define INTEGER_SUFFIX "((" UNSIGNED_SUFFIX "" EITHER_LONG_SUFFIX "?)|(" EITHER_LONG_SUFFIX "" UNSIGNED_SUFFIX "?))?"
#define CHARACTER "([^']+|\\\\')"
#define STR_CHARACTER "([^\"\\\\]|\\\\.)"
#define STRING "\"(" STR_CHARACTER ")*\""
#define CRAZY_STRING STRING "(((" C_NEWLINE "|" C_SPACE "|" C_COMMENT ")*)?" STRING ")*"
#define LARGE_FLOAT DECIMAL "+\\." DECIMAL "*" EXPONENT "?" C_FLOAT "?"
#define SMALL_FLOAT "\\." DECIMAL "+" EXPONENT "?" C_FLOAT "?"

static const char * c_token_type_names[NUM_C_TOKEN_TYPES] = {
	"SPACE",
	"NEWLINE",
	"COMMENT",
	"AUTO",
	"BREAK",
	"CASE",
	"CHAR",
	"CONST",
	"CONTINUE",
	"DEFAULT",
	"DO",
	"DOUBLE",
	"ELSE",
	"ENUM",
	"EXTERN",
	"FLOAT",
	"FOR",
	"GOTO",
	"IF",
	"INT",
	"LONG",
	"REGISTER",
	"RETURN",
	"SHORT",
	"SIGNED",
	"SIZEOF",
	"STATIC",
	"STRUCT",
	"SWITCH",
	"TYPEDEF",
	"UNION",
	"UNSIGNED",
	"VOID",
	"VOLATILE",
	"WHILE",
	"IDENTIFIER",
	"CONSTANT_HEXADECIMAL",
	"CONSTANT_OCTAL",
	"CONSTANT_DECIMAL",
	"CONSTANT_EXPONENT",
	"CONSTANT_FLOAT_SMALL",
	"CONSTANT_FLOAT_LARGE",
	"CONSTANT_CHARACTER",
	"STRING_LITERAL",
	"ELLIPSIS",
	"RIGHT_ASSIGN",
	"LEFT_ASSIGN",
	"ADD_ASSIGN",
	"SUB_ASSIGN",
	"MUL_ASSIGN",
	"DIV_ASSIGN",
	"MOD_ASSIGN",
	"AND_ASSIGN",
	"XOR_ASSIGN",
	"OR_ASSIGN",
	"RIGHT_OP",
	"LEFT_OP",
	"INC_OP",
	"DEC_OP",
	"PTR_OP",
	"AND_OP",
	"OR_OP",
	"LE_OP",
	"GE_OP",
	"EQ_OP",
	"NE_OP",
	"SEMICOLON_CHAR",
	"OPEN_BRACE_CHAR",
	"CLOSE_BRACE_CHAR",
	"OPEN_SQUARE_BRACKET_CHAR",
	"CLOSE_SQUARE_BRACKET_CHAR",
	"COMMA_CHAR",
	"COLON_CHAR",
	"EQUALS_CHAR",
	"OPEN_PAREN_CHAR",
	"CLOSE_PAREN_CHAR",
	"DOT_CHAR",
	"AMPERSAND_CHAR",
	"EXCLAMATION_MARK_CHAR",
	"TILDE_CHAR",
	"MINUS_CHAR",
	"PLUS_CHAR",
	"MULTIPLY_CHAR",
	"DIVIDE_CHAR",
	"PERCENT_CHAR",
	"OPEN_ANGLE_BRACKET_CHAR",
	"CLOSE_ANGLE_BRACKET_CHAR",
	"CARET_CHAR",
	"PIPE_CHAR",
	"QUESTION_MARK_CHAR",
	"NUMBER_SIGN_CHAR"
};

static const char * l2_token_type_names[NUM_L2_TOKEN_TYPES] = {
	"L2_SPACE",
	"L2_NEWLINE",
	"L2_COMMENT",
	"L2_IDENTIFIER",
	"L2_COLON_CHAR",
	"L2_COMMA_CHAR",
	"L2_MINUS_CHAR",
	"L2_OFFSET",
	"L2_RELOCATABLE",
	"L2_IMPLEMENTS",
	"L2_IMPLEMENTED",
	"L2_REQUIRES",
	"L2_REQUIRED",
	"L2_INTERNAL",
	"L2_EXTERNAL",
	"L2_REGISTER",
	"L2_FUNCTION",
	"L2_VARIABLE",
	"L2_CONSTANT",
	"L2_STRING",
	"L2_UNRESOLVED",
	"L2_REGION",
	"L2_START",
	"L2_END",
	"L2_PERMISSION",
	"L2_CONSTANT_HEX",
	"L2_CONSTANT_DECIMAL",
	"L2_ADD",
	"L2_SUB",
	"L2_MUL",
	"L2_DIV",
	"L2_BEQ",
	"L2_BLT",
	"L2_LOA",
	"L2_STO",
	"L2_LL",
	"L2_AND",
	"L2_OR",
	"L2_NOT",
	"L2_SHR",
	"L2_SHL",
	"L2_DW",
	"L2_SW"
};

unsigned int count_newlines_in_comment(struct c_lexer_token * t){
	unsigned int count = 0;
	unsigned char * c = t->first_byte;
	while(c != t->last_byte){
		if(*c == '\n'){
			count++;
		}
		c++;
	}
	return count;
}


int lex_c(struct c_lexer_state * state){
	struct memory_pool_collection * m = state->c.memory_pool_collection;
	state->c.position = 0;
	state->c.current_line = 0;

	struct_c_lexer_token_ptr_list_create(&state->tokens);

	while(state->c.position < state->c.buffer_size){
		unsigned int match_len;
		unsigned int match_id;
		unsigned int match_index = 0;
		unsigned int j;
		unsigned int rtn = 0;
		struct c_lexer_token * new_token = struct_c_lexer_token_memory_pool_malloc(m);

		regex_matcher_state_run(m->c_token_matcher, &state->c.buf[state->c.position], state->c.buffer_size - state->c.position);
		if(!m->c_token_matcher->num_unique_matches_found){
			unsigned int k;
			printf("Unable to create c lexer token on line %d of file %s: \n\"", state->c.current_line, state->c.filename);
			for(k = 0; (k < 100) && ((k + state->c.position) < state->c.buffer_size); k++){
				printf("%c", state->c.buf[k + state->c.position]);
			}
			printf("\"\n");
			assert(0 && "Lexer failure.");
		}
		/*  Pick the longest match if there are multiple */
		for(j = 1; j < m->c_token_matcher->num_unique_matches_found; j++){
			if(m->c_token_matcher->actual_matches[j]->length > m->c_token_matcher->actual_matches[match_index]->length){
				match_index = j;
			}
		}

		match_id = m->c_token_matcher->actual_matches[match_index]->match_node->match_id;
		match_len = m->c_token_matcher->actual_matches[match_index]->length;
		/*  Keywords tokens don't include the last character */
		switch(match_id){
			case AUTO:; case BREAK:; case CASE:; case CHAR:; case CONST:;
			case CONTINUE:; case DEFAULT:; case DO:; case DOUBLE:; case ELSE:;
			case ENUM:; case EXTERN:; case FLOAT:; case FOR:; case GOTO:;
			case IF:; case INT:; case LONG:; case REGISTER:; case RETURN:;
			case SHORT:; case SIGNED:; case SIZEOF:; case STATIC:; case STRUCT:;
			case SWITCH:; case TYPEDEF:; case UNION:; case UNSIGNED:; case VOID:; case VOLATILE:; case WHILE:{
				match_len = match_len -1;
			}
		}

		new_token->type = (enum c_token_type)match_id;
		new_token->first_byte = &state->c.buf[state->c.position];
		new_token->last_byte = &state->c.buf[state->c.position + (match_len -1)];

		/*
		printf("Got token %s: \"", get_c_token_type_names()[new_token->type]);
		for(j = 0; j < match_len; j++){
			printf("%c", state->c.buf[state->c.position + j]);
		}
		printf("\"\n");
		*/

		state->c.position += match_len;

		struct_c_lexer_token_ptr_list_add_end(&state->tokens, new_token);
		state->c.position += rtn;
	}
	return 0;
}


void create_c_lexer_state(struct c_lexer_state * state, struct unsigned_char_list * buffered_output, struct memory_pool_collection * memory_pool_collection, unsigned char * in_file, unsigned char * input_buffer, unsigned int size){
	state->c.buffered_output = buffered_output;
	state->c.memory_pool_collection = memory_pool_collection;
	state->c.filename = in_file;
	state->c.buf = input_buffer;
	state->c.buffer_size = size;
}

void destroy_c_lexer_state(struct c_lexer_state * c_lexer_state){
	unsigned int num_tokens = struct_c_lexer_token_ptr_list_size(&c_lexer_state->tokens);
	struct c_lexer_token ** tokens = struct_c_lexer_token_ptr_list_data(&c_lexer_state->tokens);
	unsigned int i;
	for(i = 0; i < num_tokens; i++){
		struct_c_lexer_token_memory_pool_free(c_lexer_state->c.memory_pool_collection, tokens[i]);
	}
	struct_c_lexer_token_ptr_list_destroy(&c_lexer_state->tokens);
}

void destroy_l2_lexer_state(struct l2_lexer_state * l2_lexer_state){
	unsigned int num_tokens = struct_l2_lexer_token_ptr_list_size(&l2_lexer_state->tokens);
	struct l2_lexer_token ** tokens = struct_l2_lexer_token_ptr_list_data(&l2_lexer_state->tokens);
	unsigned int i;
	for(i = 0; i < num_tokens; i++){
		struct_l2_lexer_token_memory_pool_free(l2_lexer_state->c.memory_pool_collection, tokens[i]);
	}
	struct_l2_lexer_token_ptr_list_destroy(&l2_lexer_state->tokens);
}

void create_l2_lexer_state(struct l2_lexer_state * state, struct unsigned_char_list * buffered_output, struct memory_pool_collection * memory_pool_collection, unsigned char * in_file, unsigned char * input_buffer, unsigned int size){
	state->c.buffered_output = buffered_output;
	state->c.memory_pool_collection = memory_pool_collection;
	state->c.filename = in_file;
	state->c.buf = input_buffer;
	state->c.buffer_size = size;
}

int lex_l2(struct l2_lexer_state * state){
	struct memory_pool_collection * m = state->c.memory_pool_collection;
	state->c.position = 0;
	state->c.current_line = 0;

	struct_l2_lexer_token_ptr_list_create(&state->tokens);

	while(state->c.position < state->c.buffer_size){
		unsigned int match_len;
		unsigned int match_id;
		unsigned int match_index = 0;
		unsigned int j;
		unsigned int rtn = 0;
		struct l2_lexer_token * new_token = struct_l2_lexer_token_memory_pool_malloc(m);

		regex_matcher_state_run(m->l2_token_matcher, &state->c.buf[state->c.position], state->c.buffer_size - state->c.position);
		if(!m->l2_token_matcher->num_unique_matches_found){
			unsigned int k;
			printf("Unable to create l2 lexer token on line %d of file %s: \n\"", state->c.current_line, state->c.filename);
			for(k = 0; (k < 100) && ((k + state->c.position) < state->c.buffer_size); k++){
				printf("%c", state->c.buf[k + state->c.position]);
			}
			printf("\"\n");
			assert(0 && "Lexer failure.");
		}
		/*  Pick the longest match if there are multiple */
		for(j = 1; j < m->l2_token_matcher->num_unique_matches_found; j++){
			if(m->l2_token_matcher->actual_matches[j]->length > m->l2_token_matcher->actual_matches[match_index]->length){
				match_index = j;
			}
		}

		match_id = m->l2_token_matcher->actual_matches[match_index]->match_node->match_id;
		match_len = m->l2_token_matcher->actual_matches[match_index]->length;
		/*  Keywords tokens don't include the last character */
		switch(match_id){
			case L2_REGISTER:; case L2_OFFSET:; case L2_RELOCATABLE:; case L2_IMPLEMENTS:; case L2_IMPLEMENTED:;
			case L2_REQUIRES:; case L2_REQUIRED:; case L2_INTERNAL:; case L2_EXTERNAL:; case L2_FUNCTION:; case L2_VARIABLE:;
			case L2_CONSTANT:; case L2_STRING:; case L2_UNRESOLVED:; case L2_REGION:; case L2_START:; case L2_END:; case L2_PERMISSION:;
			case L2_ADD:; case L2_SUB:; case L2_MUL:; case L2_DIV:; case L2_BEQ:; case L2_BLT:; case L2_LOA:; case L2_STO:;
			case L2_LL:; case L2_AND:; case L2_OR:; case L2_NOT:; case L2_SHR:; case L2_SHL:; case L2_DW:; case L2_SW:{
				match_len = match_len -1;
			}
		}

		new_token->type = (enum l2_token_type)match_id;
		new_token->first_byte = &state->c.buf[state->c.position];
		new_token->last_byte = &state->c.buf[state->c.position + (match_len -1)];

		/*
		printf("Got token %s: \"", get_l2_token_type_names()[new_token->type]);
		for(j = 0; j < match_len; j++){
			printf("%c", state->c.buf[state->c.position + j]);
		}
		printf("\"\n");
		*/

		state->c.position += match_len;

		struct_l2_lexer_token_ptr_list_add_end(&state->tokens, new_token);
		state->c.position += rtn;
	}
	return 0;
}

const char ** get_c_token_type_names(void){
	return c_token_type_names;
}

const char ** get_l2_token_type_names(void){
	return l2_token_type_names;
}


void l2_token_matcher_destroy(struct memory_pool_collection * m){
	unsigned int i;
	for(i = 0; i < NUM_L2_TOKEN_TYPES; i++){
		regex_compiler_state_destroy(m->l2_token_regexes[i]);
		free((void*)m->l2_token_regexes[i]);
	}

	free((void*)m->l2_token_regexes);
	regex_matcher_state_destroy(m->l2_token_matcher);

	free(m->l2_token_matcher);
}

void c_token_matcher_destroy(struct memory_pool_collection * m){
	unsigned int i;
	for(i = 0; i < NUM_C_TOKEN_TYPES; i++){
		regex_compiler_state_destroy(m->c_token_regexes[i]);
		free((void*)m->c_token_regexes[i]);
	}

	free((void*)m->c_token_regexes);
	regex_matcher_state_destroy(m->c_token_matcher);


	free(m->c_token_matcher);
}


void add_c_token_regex(struct memory_pool_collection *, struct regex_compiler_state **, const char *, enum c_token_type, unsigned int *);
void add_c_token_regex(struct memory_pool_collection * m, struct regex_compiler_state ** compiler_states, const char * c, enum c_token_type t, unsigned int * count){
	struct regex_compiler_state * comp = (struct regex_compiler_state *)malloc(sizeof(struct regex_compiler_state));
	regex_compiler_state_create(comp, m, (unsigned char *)c, (unsigned int)t);
	regex_compiler_state_compile(comp);
	assert(!comp->errors);
	*count = *count + 1;
	compiler_states[*count -1] = comp;
}

void c_token_matcher_create(struct memory_pool_collection * m){
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
	add_c_token_regex(m, m->c_token_regexes, "0[xX][a-fA-F0-9]+" INTEGER_SUFFIX,CONSTANT_HEXADECIMAL, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "0[0-7]*" INTEGER_SUFFIX,          CONSTANT_OCTAL, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, "[1-9][0-9]*" INTEGER_SUFFIX,      CONSTANT_DECIMAL, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, DECIMAL "+" EXPONENT C_FLOAT "?",  CONSTANT_EXPONENT, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, SMALL_FLOAT,                       CONSTANT_FLOAT_SMALL, &num_regexes);
	add_c_token_regex(m, m->c_token_regexes, LARGE_FLOAT,                       CONSTANT_FLOAT_LARGE, &num_regexes);
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

	m->c_token_matcher = (struct regex_matcher_state*)malloc(sizeof(struct regex_matcher_state));
	regex_matcher_state_create(m->c_token_matcher, m->c_token_regexes, num_regexes);
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

void l2_token_matcher_create(struct memory_pool_collection * m){
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

	m->l2_token_matcher = (struct regex_matcher_state*)malloc(sizeof(struct regex_matcher_state));
	regex_matcher_state_create(m->l2_token_matcher, m->l2_token_regexes, num_regexes);
}

