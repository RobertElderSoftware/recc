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
	"CONSTANT_HEX",
	"CONSTANT_EXPONENT",
	"CONSTANT_FLOAT_SMALL",
	"CONSTANT_FLOAT_LARGE",
	"CONSTANT_DECIMAL",
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
		struct c_lexer_token * new_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);

		regex_matcher_state_run(&m->c_token_matcher, &state->c.buf[state->c.position], state->c.buffer_size - state->c.position);
		if(!m->c_token_matcher.num_unique_matches_found){
			unsigned int k;
			printf("Unable to create c lexer token on line %d of file %s: \n\"", state->c.current_line, state->c.filename);
			for(k = 0; (k < 100) && ((k + state->c.position) < state->c.buffer_size); k++){
				printf("%c", state->c.buf[k + state->c.position]);
			}
			printf("\"\n");
			assert(0 && "Lexer failure.");
		}
		/*  Pick the longest match if there are multiple */
		for(j = 1; j < m->c_token_matcher.num_unique_matches_found; j++){
			if(m->c_token_matcher.actual_matches[j]->length > m->c_token_matcher.actual_matches[match_index]->length){
				match_index = j;
			}
		}

		match_id = m->c_token_matcher.actual_matches[match_index]->match_node->match_id;
		match_len = m->c_token_matcher.actual_matches[match_index]->length;
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
		struct_c_lexer_token_memory_pool_free(c_lexer_state->c.memory_pool_collection->struct_c_lexer_token_pool, tokens[i]);
	}
	struct_c_lexer_token_ptr_list_destroy(&c_lexer_state->tokens);
}

void destroy_l2_lexer_state(struct l2_lexer_state * l2_lexer_state){
	unsigned int num_tokens = struct_l2_lexer_token_ptr_list_size(&l2_lexer_state->tokens);
	struct l2_lexer_token ** tokens = struct_l2_lexer_token_ptr_list_data(&l2_lexer_state->tokens);
	unsigned int i;
	for(i = 0; i < num_tokens; i++){
		struct_l2_lexer_token_memory_pool_free(l2_lexer_state->c.memory_pool_collection->struct_l2_lexer_token_pool, tokens[i]);
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

int lex_asm(struct l2_lexer_state * state){
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
		struct l2_lexer_token * new_token = struct_l2_lexer_token_memory_pool_malloc(m->struct_l2_lexer_token_pool);

		regex_matcher_state_run(&m->l2_token_matcher, &state->c.buf[state->c.position], state->c.buffer_size - state->c.position);
		if(!m->l2_token_matcher.num_unique_matches_found){
			unsigned int k;
			printf("Unable to create l2 lexer token on line %d of file %s: \n\"", state->c.current_line, state->c.filename);
			for(k = 0; (k < 100) && ((k + state->c.position) < state->c.buffer_size); k++){
				printf("%c", state->c.buf[k + state->c.position]);
			}
			printf("\"\n");
			assert(0 && "Lexer failure.");
		}
		/*  Pick the longest match if there are multiple */
		for(j = 1; j < m->l2_token_matcher.num_unique_matches_found; j++){
			if(m->l2_token_matcher.actual_matches[j]->length > m->l2_token_matcher.actual_matches[match_index]->length){
				match_index = j;
			}
		}

		match_id = m->l2_token_matcher.actual_matches[match_index]->match_node->match_id;
		match_len = m->l2_token_matcher.actual_matches[match_index]->length;
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
