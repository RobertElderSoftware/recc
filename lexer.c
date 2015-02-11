/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#include "lexer.h"

#define SHOW_LEXER_TOKENS 0

const char * c_token_type_names[90] = {
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

const char * build_script_token_type_names[10] = {
	"B_SPACE",
	"B_NEWLINE",
	"B_CODE_GENERATE",
	"B_PREPROCESS",
	"B_LINK",
	"B_SYMBOLS",
	"B_TO",
	"B_SEMICOLON_CHAR",
	"B_COMMA_CHAR",
	"B_FILENAME"
};


const char * asm_token_type_names[31] = {
	"A_SPACE",
	"A_NEWLINE",
	"A_ASM_COMMENT",
	"A_IDENTIFIER",
	"A_COLON_CHAR",
	"A_COMMA_CHAR",
	"A_MINUS_CHAR",
	"A_OFFSET",
	"A_RELOCATABLE",
	"A_IMPLEMENTS",
	"A_REQUIRES",
	"A_INTERNAL",
	"A_EXTERNAL",
	"A_REGISTER",
	"A_CONSTANT_HEX",
	"A_CONSTANT_DECIMAL",
	"A_ADD",
	"A_SUB",
	"A_MUL",
	"A_DIV",
	"A_BEQ",
	"A_BLT",
	"A_LOA",
	"A_STO",
	"A_LL",
	"A_AND",
	"A_OR",
	"A_NOT",
	"A_SHR",
	"A_SHL",
	"A_DW"
};

/*  Interfaces not in .h file to discourage external usage */
unsigned int accept_range(unsigned char, unsigned char, struct common_lexer_state *, unsigned int);
unsigned int accept(unsigned char, struct common_lexer_state *, unsigned int);
unsigned int accept_word(unsigned const char *, struct common_lexer_state *, unsigned int);
unsigned int t_D(struct common_lexer_state *, unsigned int);
unsigned int t_L(struct common_lexer_state *, unsigned int);
unsigned int t_H(struct common_lexer_state *, unsigned int);
unsigned int t_E(struct common_lexer_state *, unsigned int);
unsigned int t_FS(struct common_lexer_state *, unsigned int);
unsigned int t_IS(struct common_lexer_state *, unsigned int);
unsigned int t_comment(struct common_lexer_state *, unsigned int, unsigned int *);
unsigned int t_asm_comment(struct common_lexer_state *, unsigned int);
unsigned int t_asm_register(struct common_lexer_state *, unsigned int);
unsigned int t_keyword(unsigned const char *, struct common_lexer_state *, unsigned int);
unsigned int t_keyword_space_check(unsigned const char *, struct common_lexer_state *, unsigned int);
unsigned int t_symbol(unsigned const char *, struct common_lexer_state *, unsigned int);
unsigned int t_identifier(struct common_lexer_state *, unsigned int);
unsigned int t_constant_hex(struct common_lexer_state *, unsigned int);
unsigned int t_constant_decimal(struct common_lexer_state *, unsigned int);
unsigned int t_constant_exponent(struct common_lexer_state *, unsigned int);
unsigned int t_constant_float_small(struct common_lexer_state *, unsigned int);
unsigned int t_constant_float_large(struct common_lexer_state *, unsigned int);
unsigned int t_constant_character(struct common_lexer_state *, unsigned int);
unsigned int t_constant_string(struct common_lexer_state *, unsigned int);
unsigned int t_newline(struct common_lexer_state *, unsigned int);
unsigned int t_space(struct common_lexer_state *, unsigned int);
unsigned int t_filename(struct common_lexer_state *, unsigned int);
void show_lexer_token(struct unsigned_char_list *, const char *, unsigned char *, unsigned char *, unsigned int);

unsigned int accept_range(unsigned char lo, unsigned char hi, struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	if(common_lexer_state->buf[tentative_position] >= lo && common_lexer_state->buf[tentative_position] <= hi){
		return 1;
	}
	return 0;
}

unsigned int accept(unsigned char c, struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	return accept_range(c, c, common_lexer_state, tentative_position);
}

unsigned int accept_word(unsigned const char * c, struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int i = 0;
	while(c[i]){
		if(!(accept(c[i], common_lexer_state, tentative_position + i))){
			return 0;
		}
		i++;
	}
	return i;
}

unsigned int t_D(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	return accept_range('0','9', common_lexer_state, tentative_position);
}

unsigned int t_L(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	return accept_range('a','z', common_lexer_state, tentative_position) || accept_range('A','Z', common_lexer_state, tentative_position) || accept('_', common_lexer_state, tentative_position);
}

unsigned int t_H(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	return accept_range('a','f', common_lexer_state, tentative_position) || accept_range('A','F', common_lexer_state, tentative_position) || accept_range('0','9', common_lexer_state, tentative_position);
}

unsigned int t_E(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	if(accept('e', common_lexer_state, tentative_position + count) || accept('E', common_lexer_state, tentative_position + count)){
		count += 1;
		count += (accept('-', common_lexer_state, tentative_position + count) || accept('+', common_lexer_state, tentative_position + count));
		if(t_D(common_lexer_state, tentative_position + count)){
			count++;
			while(t_D(common_lexer_state, tentative_position + count)){
				count++;
			}
			return count;
		}
	}
	return 0;
}

unsigned int t_FS(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	return accept('f', common_lexer_state, tentative_position) || accept('F', common_lexer_state, tentative_position) || accept('l', common_lexer_state, tentative_position) || accept('L', common_lexer_state, tentative_position);
}

unsigned int t_IS(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	if(accept('u', common_lexer_state, tentative_position + count) || accept('U', common_lexer_state, tentative_position + count) || accept('l', common_lexer_state, tentative_position + count) || accept('L', common_lexer_state, tentative_position + count)){
		count += 1;
		while(accept('u', common_lexer_state, tentative_position + count) || accept('U', common_lexer_state, tentative_position + count) || accept('l', common_lexer_state, tentative_position + count) || accept('L', common_lexer_state, tentative_position + count)){
			count++;
		}
		return count;
	}
	return 0;
}

unsigned int t_comment(struct common_lexer_state * common_lexer_state, unsigned int tentative_position, unsigned int * current_line){
	unsigned int count = 0;
	if(accept('/', common_lexer_state, tentative_position + count) && accept('*', common_lexer_state, tentative_position + count + 1)){
		count += 2;
		while(1){
			if(accept('*', common_lexer_state, tentative_position + count)){
				count++;
				if(accept('/', common_lexer_state, tentative_position + count)){
					count++;
					return count;
				}
			}else{
				if(accept(0, common_lexer_state, tentative_position + count)){
					buffered_printf(common_lexer_state->buffered_output, "Unterminated comment.");
					return 0;
				}
				if(accept('\n', common_lexer_state, tentative_position + count)){
					*current_line = *current_line + 1;
				}else{
					accept_range(1,255, common_lexer_state, tentative_position + count);
				}
				count++;
			}
		}
	}
	return 0;
}

unsigned int t_asm_comment(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	if(accept(';', common_lexer_state, tentative_position + count)){
		count++;
		while(1){
			unsigned int count_before = count;
			if(accept_range(32, 126, common_lexer_state, tentative_position + count)){
				count++;
			}
			if(accept('\t', common_lexer_state, tentative_position + count)){
				count++;
			}
			if(accept('\n', common_lexer_state, tentative_position + count)){
				break;
			}
			if(count_before == count){
				assert(0 && "Bad character in asm comment.");
			}
		}
	}
	return count;
}

unsigned int t_asm_register(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	if(accept('r', common_lexer_state, tentative_position + count)){
		count++;
		while(accept_range('0', '9', common_lexer_state, tentative_position + count)){
			count++;
		}
		if(count > 1){
			return count;
		}else{
			return 0;
		}
	}
	if(accept('F', common_lexer_state, tentative_position + count)){
		count++;
		if(accept('P', common_lexer_state, tentative_position + count) || accept('R', common_lexer_state, tentative_position + count)){
			count++;
			return count;
		}else{
			return 0;
		}
	}
	if(accept('P', common_lexer_state, tentative_position + count)){
		count++;
		if(accept('C', common_lexer_state, tentative_position + count)){
			count++;
			return count;
		}else{
			return 0;
		}
	}
	if(accept('S', common_lexer_state, tentative_position + count)){
		count++;
		if(accept('P', common_lexer_state, tentative_position + count)){
			count++;
			return count;
		}else{
			return 0;
		}
	}
	if(accept('Z', common_lexer_state, tentative_position + count)){
		count++;
		if(accept('R', common_lexer_state, tentative_position + count)){
			count++;
			return count;
		}else{
			return 0;
		}
	}
	if(accept('W', common_lexer_state, tentative_position + count)){
		count++;
		if(accept('R', common_lexer_state, tentative_position + count)){
			count++;
			return count;
		}else{
			return 0;
		}
	}
	return count;
}

unsigned int t_filename(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	while(accept_range('!', '+', common_lexer_state, tentative_position + count) || accept_range('-', ';', common_lexer_state, tentative_position + count) || accept_range('A', '_', common_lexer_state, tentative_position + count) || accept_range('a', '}', common_lexer_state, tentative_position + count)){
		count += 1;
	}
	return count;
}

unsigned int t_keyword(unsigned const char * word, struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	if((count = accept_word(word, common_lexer_state, tentative_position + count))){
		if(!(t_D(common_lexer_state, tentative_position + count) || t_L(common_lexer_state, tentative_position + count)))
			return count;
	
	}
	return 0;
}

unsigned int t_keyword_space_check(unsigned const char * word, struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	if((count = accept_word(word, common_lexer_state, tentative_position + count))){
		if(t_space(common_lexer_state, tentative_position + count))
			return count;
	
	}
	return 0;
}

unsigned int t_symbol(unsigned const char * word, struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	return accept_word(word, common_lexer_state, tentative_position);
}

unsigned int t_identifier(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	if(!(count = t_L(common_lexer_state, tentative_position + count))){
		return 0;
	}

	while(t_L(common_lexer_state, tentative_position + count) || t_D(common_lexer_state, tentative_position + count)){
		count += 1;
	}
	return count;
}

unsigned int t_constant_hex(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	if(accept('0', common_lexer_state, tentative_position)){
		unsigned int count = 1;
		if(accept('x', common_lexer_state, tentative_position + count) || accept('X', common_lexer_state, tentative_position + count)){
			count++;
			if(t_H(common_lexer_state, tentative_position + count)){
				unsigned int t_IS_consumed;
				count++;
				while(t_H(common_lexer_state, tentative_position + count)){
					count++;
				}
				t_IS_consumed = t_IS(common_lexer_state, tentative_position + count);
				if(t_IS_consumed){
					count += t_IS_consumed;
				}
				return count;
			}
		}
	}
	return 0;
}

unsigned int t_constant_decimal(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	if(t_D(common_lexer_state, tentative_position)){
		unsigned int t_IS_consumed;
		unsigned int count = 1;
		while(t_D(common_lexer_state, tentative_position + count)){
			count++;
		}
		t_IS_consumed = t_IS(common_lexer_state, tentative_position + count);
		if(t_IS_consumed){
			count += t_IS_consumed;
		}
		return count;
	}
	return 0;
}

unsigned int t_constant_exponent(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	unsigned int e_size;
	if((count = t_D(common_lexer_state, tentative_position + count))){
		while(t_D(common_lexer_state, tentative_position + count)){
			count++;
		}
		if((e_size = t_E(common_lexer_state, tentative_position + count))){
			count += e_size;
			if(t_FS(common_lexer_state, tentative_position + count)){
				count++;
			}
			return count;
		}
	}
	return 0;
}

unsigned int t_constant_float_small(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	unsigned int e_size;
	while(t_D(common_lexer_state, tentative_position + count)){
		count++;
	}
	if(accept('.', common_lexer_state, tentative_position + count)){
		count++;
		if(t_D(common_lexer_state, tentative_position + count)){
			count++;
			while(t_D(common_lexer_state, tentative_position + count)){
				count++;
			}
			if((e_size = t_E(common_lexer_state, tentative_position + count))){
				count += e_size;
				if(t_FS(common_lexer_state, tentative_position + count)){
					count++;
				}
			}
			return count;
		}
	}
	return 0;
}

unsigned int t_constant_float_large(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	unsigned int e_size;
	if((count = t_D(common_lexer_state, tentative_position + count))){
		while(t_D(common_lexer_state, tentative_position + count)){
			count++;
		}
		if(accept('.', common_lexer_state, tentative_position + count)){
			count++;
			while(t_D(common_lexer_state, tentative_position + count)){
				count++;
			}
			if((e_size = t_E(common_lexer_state, tentative_position + count))){
				count += e_size;
				if(t_FS(common_lexer_state, tentative_position + count)){
					count++;
				}
			}
			return count;
		}
	}
	return 0;
}

unsigned int t_constant_character(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	if(accept('\'', common_lexer_state, tentative_position)){
		unsigned int count = 1;
		if(accept('\\', common_lexer_state, tentative_position + count)){
			accept_range(1,255, common_lexer_state, tentative_position + count);
			count +=2;
		}else{
			if(accept('\'', common_lexer_state, tentative_position + count)){
				buffered_printf(common_lexer_state->buffered_output, "Error in character constant.");
				return 0;
			}
			if(accept(0, common_lexer_state, tentative_position + count)){
				buffered_printf(common_lexer_state->buffered_output, "Unexpected end of file in constant expression.");
				return 0;
			}
			accept_range(1,255, common_lexer_state, tentative_position + count);
			count++;
		}
		while(!accept('\'', common_lexer_state, tentative_position + count)){
			if(accept('\\', common_lexer_state, tentative_position + count)){
				accept_range(1,255, common_lexer_state, tentative_position + count);
				count +=2;
			}else{
				if(accept(0, common_lexer_state, tentative_position + count)){
					buffered_printf(common_lexer_state->buffered_output, "Unexpected end of file in constant expression.");
					return 0;
				}
				accept_range(1,255, common_lexer_state, tentative_position + count);
				count++;
			}
		}
		count++;
		return count;
	}
	return 0;
}

unsigned int t_constant_string(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	if(accept('"', common_lexer_state, tentative_position)){
		unsigned int count = 1;
		if(accept('\\', common_lexer_state, tentative_position + count)){
			accept_range(1,255, common_lexer_state, tentative_position + count);
			count +=2;
		}else{
			if(accept('"', common_lexer_state, tentative_position + count)){
				count++;
				return count;
			}
			if(accept(0, common_lexer_state, tentative_position + count)){
				buffered_printf(common_lexer_state->buffered_output, "Unexpected end of file in constant expression.");
				return 0;
			}
			accept_range(1,255, common_lexer_state, tentative_position + count);
			count++;
		}
		while(!accept('"', common_lexer_state, tentative_position + count)){
			if(accept('\\', common_lexer_state, tentative_position + count)){
				accept_range(1,255, common_lexer_state, tentative_position + count);
				count +=2;
			}else{
				if(accept(0, common_lexer_state, tentative_position + count)){
					buffered_printf(common_lexer_state->buffered_output, "Unexpected end of file in constant expression.");
					return 0;
				}
				accept_range(1,255, common_lexer_state, tentative_position + count);
				count++;
			}
		}
		count++;
		return count;
	}
	return 0;
}

unsigned int t_newline(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	if(accept('\n', common_lexer_state, tentative_position)){
		return 1;
	}
	return 0;
}

unsigned int t_space(struct common_lexer_state * common_lexer_state, unsigned int tentative_position){
	unsigned int count = 0;
	while(accept(' ', common_lexer_state, tentative_position + count) || accept('\t', common_lexer_state, tentative_position + count)){
		count += 1;
	}
	return count;
}

void show_lexer_token(struct unsigned_char_list * out_buffer, const char * name, unsigned char * first_byte, unsigned char * last_byte, unsigned int enabled){
	(void)out_buffer;
	if(enabled){
		unsigned int k;
		printf(";%s '", name);
		for(k = 0; k < ((last_byte - first_byte) + 1); k++){
			if(first_byte[k] == '\n'){
				printf("\\n");
			}else{
				printf("%c", first_byte[k]);
			}
		}
		printf("'");
		printf("\n");
	}
}

int lex_c(struct c_lexer_state * c_lexer_state, unsigned char * filename, unsigned char * buffer, unsigned int buffer_size){
	struct memory_pooler * c_lexer_token_pool = memory_pooler_collection_get_pool(c_lexer_state->c.memory_pooler_collection, sizeof(struct c_lexer_token));
	c_lexer_state->c.buf = buffer;
	c_lexer_state->c.position = 0;
	c_lexer_state->c.current_line = 0;
	c_lexer_state->c.filename = filename;
	c_lexer_state->c.buffer_size = buffer_size;

	g_format_buffer_use();

	struct_c_lexer_token_ptr_list_create(&c_lexer_state->tokens);

	while(c_lexer_state->c.position < buffer_size){
		unsigned int rtn = 0;
		unsigned char * first_byte = &c_lexer_state->c.buf[c_lexer_state->c.position];
		enum c_token_type type;
		struct c_lexer_token * new_token;

		if((rtn = t_space(&c_lexer_state->c, c_lexer_state->c.position))){
			type = SPACE;
		}else if((rtn = t_newline(&c_lexer_state->c, c_lexer_state->c.position))){
			type = NEWLINE;
			c_lexer_state->c.current_line = c_lexer_state->c.current_line + rtn; /* NEWLINE token can be multiple newlines */
		}else if((rtn = t_comment(&c_lexer_state->c, c_lexer_state->c.position, &c_lexer_state->c.current_line))){
			type = COMMENT;
		}else if((rtn = t_keyword((const unsigned char *)"auto", &c_lexer_state->c, c_lexer_state->c.position))){
			type = AUTO;
		}else if((rtn = t_keyword((const unsigned char *)"break", &c_lexer_state->c, c_lexer_state->c.position))){
			type = BREAK;
		}else if((rtn = t_keyword((const unsigned char *)"case", &c_lexer_state->c, c_lexer_state->c.position))){
			type = CASE;
		}else if((rtn = t_keyword((const unsigned char *)"char", &c_lexer_state->c, c_lexer_state->c.position))){
			type = CHAR;
		}else if((rtn = t_keyword((const unsigned char *)"const", &c_lexer_state->c, c_lexer_state->c.position))){
			type = CONST;
		}else if((rtn = t_keyword((const unsigned char *)"continue", &c_lexer_state->c, c_lexer_state->c.position))){
			type = CONTINUE;
		}else if((rtn = t_keyword((const unsigned char *)"default", &c_lexer_state->c, c_lexer_state->c.position))){
			type = DEFAULT;
		}else if((rtn = t_keyword((const unsigned char *)"do", &c_lexer_state->c, c_lexer_state->c.position))){
			type = DO;
		}else if((rtn = t_keyword((const unsigned char *)"double", &c_lexer_state->c, c_lexer_state->c.position))){
			type = DOUBLE;
		}else if((rtn = t_keyword((const unsigned char *)"else", &c_lexer_state->c, c_lexer_state->c.position))){
			type = ELSE;
		}else if((rtn = t_keyword((const unsigned char *)"enum", &c_lexer_state->c, c_lexer_state->c.position))){
			type = ENUM;
		}else if((rtn = t_keyword((const unsigned char *)"extern", &c_lexer_state->c, c_lexer_state->c.position))){
			type = EXTERN;
		}else if((rtn = t_keyword((const unsigned char *)"float", &c_lexer_state->c, c_lexer_state->c.position))){
			type = FLOAT;
		}else if((rtn = t_keyword((const unsigned char *)"for", &c_lexer_state->c, c_lexer_state->c.position))){
			type = FOR;
		}else if((rtn = t_keyword((const unsigned char *)"goto", &c_lexer_state->c, c_lexer_state->c.position))){
			type = GOTO;
		}else if((rtn = t_keyword((const unsigned char *)"if", &c_lexer_state->c, c_lexer_state->c.position))){
			type = IF;
		}else if((rtn = t_keyword((const unsigned char *)"int", &c_lexer_state->c, c_lexer_state->c.position))){
			type = INT;
		}else if((rtn = t_keyword((const unsigned char *)"long", &c_lexer_state->c, c_lexer_state->c.position))){
			type = LONG;
		}else if((rtn = t_keyword((const unsigned char *)"register", &c_lexer_state->c, c_lexer_state->c.position))){
			type = REGISTER;
		}else if((rtn = t_keyword((const unsigned char *)"return", &c_lexer_state->c, c_lexer_state->c.position))){
			type = RETURN;
		}else if((rtn = t_keyword((const unsigned char *)"short", &c_lexer_state->c, c_lexer_state->c.position))){
			type = SHORT;
		}else if((rtn = t_keyword((const unsigned char *)"signed", &c_lexer_state->c, c_lexer_state->c.position))){
			type = SIGNED;
		}else if((rtn = t_keyword((const unsigned char *)"sizeof", &c_lexer_state->c, c_lexer_state->c.position))){
			type = SIZEOF;
		}else if((rtn = t_keyword((const unsigned char *)"static", &c_lexer_state->c, c_lexer_state->c.position))){
			type = STATIC;
		}else if((rtn = t_keyword((const unsigned char *)"struct", &c_lexer_state->c, c_lexer_state->c.position))){
			type = STRUCT;
		}else if((rtn = t_keyword((const unsigned char *)"switch", &c_lexer_state->c, c_lexer_state->c.position))){
			type = SWITCH;
		}else if((rtn = t_keyword((const unsigned char *)"typedef", &c_lexer_state->c, c_lexer_state->c.position))){
			type = TYPEDEF;
		}else if((rtn = t_keyword((const unsigned char *)"union", &c_lexer_state->c, c_lexer_state->c.position))){
			type = UNION;
		}else if((rtn = t_keyword((const unsigned char *)"unsigned", &c_lexer_state->c, c_lexer_state->c.position))){
			type = UNSIGNED;
		}else if((rtn = t_keyword((const unsigned char *)"void", &c_lexer_state->c, c_lexer_state->c.position))){
			type = VOID;
		}else if((rtn = t_keyword((const unsigned char *)"volatile", &c_lexer_state->c, c_lexer_state->c.position))){
			type = VOLATILE;
		}else if((rtn = t_keyword((const unsigned char *)"while", &c_lexer_state->c, c_lexer_state->c.position))){
			type = WHILE;
		}else if((rtn = t_identifier(&c_lexer_state->c, c_lexer_state->c.position))){
			type = IDENTIFIER;
		}else if((rtn = t_constant_hex(&c_lexer_state->c, c_lexer_state->c.position))){
			type = CONSTANT_HEX;
		}else if((rtn = t_constant_exponent(&c_lexer_state->c, c_lexer_state->c.position))){
			type = CONSTANT_EXPONENT;
		}else if((rtn = t_constant_float_small(&c_lexer_state->c, c_lexer_state->c.position))){
			type = CONSTANT_FLOAT_SMALL;
		}else if((rtn = t_constant_float_large(&c_lexer_state->c, c_lexer_state->c.position))){
			type = CONSTANT_FLOAT_LARGE;
		}else if((rtn = t_constant_decimal(&c_lexer_state->c, c_lexer_state->c.position))){
			type = CONSTANT_DECIMAL;
		}else if((rtn = t_constant_character(&c_lexer_state->c, c_lexer_state->c.position))){
			type = CONSTANT_CHARACTER;
		}else if((rtn = t_constant_string(&c_lexer_state->c, c_lexer_state->c.position))){
			type = STRING_LITERAL;
		}else if((rtn = t_symbol((const unsigned char *)"...", &c_lexer_state->c, c_lexer_state->c.position))){
			type = ELLIPSIS;
		}else if((rtn = t_symbol((const unsigned char *)">>=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = RIGHT_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"<<=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = LEFT_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"+=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = ADD_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"-=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = SUB_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"*=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = MUL_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"/=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = DIV_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"%=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = MOD_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"&=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = AND_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"^=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = XOR_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)"|=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = OR_ASSIGN;
		}else if((rtn = t_symbol((const unsigned char *)">>", &c_lexer_state->c, c_lexer_state->c.position))){
			type = RIGHT_OP;
		}else if((rtn = t_symbol((const unsigned char *)"<<", &c_lexer_state->c, c_lexer_state->c.position))){
			type = LEFT_OP;
		}else if((rtn = t_symbol((const unsigned char *)"++", &c_lexer_state->c, c_lexer_state->c.position))){
			type = INC_OP;
		}else if((rtn = t_symbol((const unsigned char *)"->", &c_lexer_state->c, c_lexer_state->c.position))){
			type = PTR_OP;
		}else if((rtn = t_symbol((const unsigned char *)"--", &c_lexer_state->c, c_lexer_state->c.position))){
			type = DEC_OP;
		}else if((rtn = t_symbol((const unsigned char *)"&&", &c_lexer_state->c, c_lexer_state->c.position))){
			type = AND_OP;
		}else if((rtn = t_symbol((const unsigned char *)"||", &c_lexer_state->c, c_lexer_state->c.position))){
			type = OR_OP;
		}else if((rtn = t_symbol((const unsigned char *)"<=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = LE_OP;
		}else if((rtn = t_symbol((const unsigned char *)">=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = GE_OP;
		}else if((rtn = t_symbol((const unsigned char *)"==", &c_lexer_state->c, c_lexer_state->c.position))){
			type = EQ_OP;
		}else if((rtn = t_symbol((const unsigned char *)"!=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = NE_OP;
		}else if((rtn = t_symbol((const unsigned char *)";", &c_lexer_state->c, c_lexer_state->c.position))){
			type = SEMICOLON_CHAR;
		}else if(((rtn = t_symbol((const unsigned char *)"{", &c_lexer_state->c, c_lexer_state->c.position)) || (rtn = t_symbol((const unsigned char *)"<%", &c_lexer_state->c, c_lexer_state->c.position)))){
			type = OPEN_BRACE_CHAR;
		}else if(((rtn = t_symbol((const unsigned char *)"}", &c_lexer_state->c, c_lexer_state->c.position)) || (rtn = t_symbol((const unsigned char *)"%>", &c_lexer_state->c, c_lexer_state->c.position)))){
			type = CLOSE_BRACE_CHAR;
		}else if(((rtn = t_symbol((const unsigned char *)"[", &c_lexer_state->c, c_lexer_state->c.position)) || (rtn = t_symbol((const unsigned char *)"<:", &c_lexer_state->c, c_lexer_state->c.position)))){
			type = OPEN_SQUARE_BRACKET_CHAR;
		}else if(((rtn = t_symbol((const unsigned char *)"]", &c_lexer_state->c, c_lexer_state->c.position)) || (rtn = t_symbol((const unsigned char *)":>", &c_lexer_state->c, c_lexer_state->c.position)))){
			type = CLOSE_SQUARE_BRACKET_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)",", &c_lexer_state->c, c_lexer_state->c.position))){
			type = COMMA_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)":", &c_lexer_state->c, c_lexer_state->c.position))){
			type = COLON_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"=", &c_lexer_state->c, c_lexer_state->c.position))){
			type = EQUALS_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"(", &c_lexer_state->c, c_lexer_state->c.position))){
			type = OPEN_PAREN_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)")", &c_lexer_state->c, c_lexer_state->c.position))){
			type = CLOSE_PAREN_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)".", &c_lexer_state->c, c_lexer_state->c.position))){
			type = DOT_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"&", &c_lexer_state->c, c_lexer_state->c.position))){
			type = AMPERSAND_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"!", &c_lexer_state->c, c_lexer_state->c.position))){
			type = EXCLAMATION_MARK_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"~", &c_lexer_state->c, c_lexer_state->c.position))){
			type = TILDE_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"-", &c_lexer_state->c, c_lexer_state->c.position))){
			type = MINUS_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"+", &c_lexer_state->c, c_lexer_state->c.position))){
			type = PLUS_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"*", &c_lexer_state->c, c_lexer_state->c.position))){
			type = MULTIPLY_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"/", &c_lexer_state->c, c_lexer_state->c.position))){
			type = DIVIDE_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"%", &c_lexer_state->c, c_lexer_state->c.position))){
			type = PERCENT_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"<", &c_lexer_state->c, c_lexer_state->c.position))){
			type = OPEN_ANGLE_BRACKET_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)">", &c_lexer_state->c, c_lexer_state->c.position))){
			type = CLOSE_ANGLE_BRACKET_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"^", &c_lexer_state->c, c_lexer_state->c.position))){
			type = CARET_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"|", &c_lexer_state->c, c_lexer_state->c.position))){
			type = PIPE_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"?", &c_lexer_state->c, c_lexer_state->c.position))){
			type = QUESTION_MARK_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"#", &c_lexer_state->c, c_lexer_state->c.position))){
			type = NUMBER_SIGN_CHAR;
		}else{
			unsigned int i = 0;
			buffered_printf(c_lexer_state->c.buffered_output, "Lexer stopping on character '%c' 0x(%x)\n", c_lexer_state->c.buf[c_lexer_state->c.position], c_lexer_state->c.buf[c_lexer_state->c.position]);
			for(c_lexer_state->c.position = 0; i < c_lexer_state->c.position + 100; i++){
				if(c_lexer_state->c.buf[i]){
					buffered_printf(c_lexer_state->c.buffered_output, "%c", c_lexer_state->c.buf[i]);
				}
			}
			g_format_buffer_release();
			return 1;
		}


		new_token = memory_pooler_malloc(c_lexer_token_pool);
		new_token->type = type;
		new_token->first_byte = first_byte;
		new_token->last_byte = (first_byte + rtn) - 1;

		struct_c_lexer_token_ptr_list_add(&c_lexer_state->tokens, new_token);
		show_lexer_token(c_lexer_state->c.buffered_output, get_c_token_type_names()[new_token->type], new_token->first_byte, new_token->last_byte, SHOW_LEXER_TOKENS);
		c_lexer_state->c.position += rtn;
	}
	g_format_buffer_release();
	return 0;
}

void destroy_c_lexer_state(struct c_lexer_state * c_lexer_state){
	unsigned int num_tokens = struct_c_lexer_token_ptr_list_size(&c_lexer_state->tokens);
	struct c_lexer_token ** tokens = struct_c_lexer_token_ptr_list_data(&c_lexer_state->tokens);
	unsigned int i;
	struct memory_pooler * c_lexer_token_pool = memory_pooler_collection_get_pool(c_lexer_state->c.memory_pooler_collection, sizeof(struct c_lexer_token));
	for(i = 0; i < num_tokens; i++){
		memory_pooler_free(c_lexer_token_pool, tokens[i]);
	}
	struct_c_lexer_token_ptr_list_destroy(&c_lexer_state->tokens);
}

void destroy_build_script_lexer_state(struct build_script_lexer_state * build_script_lexer_state){
	unsigned int num_tokens = struct_build_script_lexer_token_ptr_list_size(&build_script_lexer_state->tokens);
	struct build_script_lexer_token ** tokens = struct_build_script_lexer_token_ptr_list_data(&build_script_lexer_state->tokens);
	unsigned int i;
	struct memory_pooler * build_script_lexer_token_pool = memory_pooler_collection_get_pool(build_script_lexer_state->c.memory_pooler_collection, sizeof(struct build_script_lexer_token));
	for(i = 0; i < num_tokens; i++){
		memory_pooler_free(build_script_lexer_token_pool, tokens[i]);
	}
	struct_build_script_lexer_token_ptr_list_destroy(&build_script_lexer_state->tokens);
}

void destroy_asm_lexer_state(struct asm_lexer_state * asm_lexer_state){
	unsigned int num_tokens = struct_asm_lexer_token_ptr_list_size(&asm_lexer_state->tokens);
	struct asm_lexer_token ** tokens = struct_asm_lexer_token_ptr_list_data(&asm_lexer_state->tokens);
	unsigned int i;
	struct memory_pooler * asm_lexer_token_pool = memory_pooler_collection_get_pool(asm_lexer_state->c.memory_pooler_collection, sizeof(struct asm_lexer_token));
	for(i = 0; i < num_tokens; i++){
		memory_pooler_free(asm_lexer_token_pool, tokens[i]);
	}
	struct_asm_lexer_token_ptr_list_destroy(&asm_lexer_state->tokens);
}

int lex_build_script(struct build_script_lexer_state * build_script_lexer_state, unsigned char * filename, unsigned char * buffer, unsigned int buffer_size){
	struct memory_pooler * build_script_lexer_token_pool = memory_pooler_collection_get_pool(build_script_lexer_state->c.memory_pooler_collection, sizeof(struct build_script_lexer_token));
	build_script_lexer_state->c.buf = buffer;
	build_script_lexer_state->c.position = 0;
	build_script_lexer_state->c.current_line = 0;
	build_script_lexer_state->c.filename = filename;
	build_script_lexer_state->c.buffer_size = buffer_size;

	g_format_buffer_use();

	struct_build_script_lexer_token_ptr_list_create(&build_script_lexer_state->tokens);

	while(build_script_lexer_state->c.position < buffer_size){
		unsigned int rtn = 0;
		unsigned char * first_byte = &build_script_lexer_state->c.buf[build_script_lexer_state->c.position];
		enum build_script_token_type type;
		struct build_script_lexer_token * new_token;

		if((rtn = t_space(&build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_SPACE;
		}else if((rtn = t_newline(&build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_NEWLINE;
			build_script_lexer_state->c.current_line = build_script_lexer_state->c.current_line + rtn; /* NEWLINE token can be multiple newlines */
		}else if((rtn = t_keyword((const unsigned char *)"PREPROCESS", &build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_PREPROCESS;
		}else if((rtn = t_keyword((const unsigned char *)"CODE GENERATE", &build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_CODE_GENERATE;
		}else if((rtn = t_keyword((const unsigned char *)"LINK", &build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_LINK;
		}else if((rtn = t_keyword((const unsigned char *)"SYMBOLS", &build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_SYMBOLS;
		}else if((rtn = t_keyword((const unsigned char *)"TO", &build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_TO;
		}else if((rtn = t_filename(&build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_FILENAME;
		}else if((rtn = t_symbol((const unsigned char *)",", &build_script_lexer_state->c, build_script_lexer_state->c.position))){
			type = B_COMMA_CHAR;
		}else{
			unsigned int i = 0;
			buffered_printf(build_script_lexer_state->c.buffered_output, "Lexer stopping on character '%c' 0x(%x)\n", build_script_lexer_state->c.buf[build_script_lexer_state->c.position], build_script_lexer_state->c.buf[build_script_lexer_state->c.position]);
			for(build_script_lexer_state->c.position = 0; i < build_script_lexer_state->c.position + 100; i++){
				if(build_script_lexer_state->c.buf[i]){
					buffered_printf(build_script_lexer_state->c.buffered_output, "%c", build_script_lexer_state->c.buf[i]);
				}
			}
			g_format_buffer_release();
			return 1;
		}

		new_token = memory_pooler_malloc(build_script_lexer_token_pool);
		new_token->type = type;
		new_token->first_byte = first_byte;
		new_token->last_byte = (first_byte + rtn) - 1;

		struct_build_script_lexer_token_ptr_list_add(&build_script_lexer_state->tokens, new_token);
		show_lexer_token(build_script_lexer_state->c.buffered_output, get_c_token_type_names()[new_token->type], new_token->first_byte, new_token->last_byte, SHOW_LEXER_TOKENS);
		build_script_lexer_state->c.position += rtn;
	}
	g_format_buffer_release();
	return 0;
}

int lex_asm(struct asm_lexer_state * asm_lexer_state, unsigned char * filename, unsigned char * buffer, unsigned int buffer_size){
	struct memory_pooler * asm_lexer_token_pool = memory_pooler_collection_get_pool(asm_lexer_state->c.memory_pooler_collection, sizeof(struct asm_lexer_token));
	asm_lexer_state->c.buf = buffer;
	asm_lexer_state->c.position = 0;
	asm_lexer_state->c.current_line = 0;
	asm_lexer_state->c.filename = filename;
	asm_lexer_state->c.buffer_size = buffer_size;

	g_format_buffer_use();

	struct_asm_lexer_token_ptr_list_create(&asm_lexer_state->tokens);

	while(asm_lexer_state->c.position < buffer_size){
		unsigned int rtn = 0;
		unsigned char * first_byte = &asm_lexer_state->c.buf[asm_lexer_state->c.position];
		enum asm_token_type type;
		struct asm_lexer_token * new_token;

		if((rtn = t_space(&asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_SPACE;
		}else if((rtn = t_newline(&asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_NEWLINE;
			asm_lexer_state->c.current_line = asm_lexer_state->c.current_line + rtn; /* NEWLINE token can be multiple newlines */
		}else if((rtn = t_asm_comment(&asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_ASM_COMMENT;
		}else if((rtn = t_symbol((const unsigned char *)":", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_COLON_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)",", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_COMMA_CHAR;
		}else if((rtn = t_symbol((const unsigned char *)"-", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_MINUS_CHAR;
		}else if((rtn = t_keyword((const unsigned char *)"OFFSET", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_OFFSET;
		}else if((rtn = t_keyword((const unsigned char *)"RELOCATABLE", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_RELOCATABLE;
		}else if((rtn = t_keyword((const unsigned char *)"IMPLEMENTS", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_IMPLEMENTS;
		}else if((rtn = t_keyword((const unsigned char *)"REQUIRES", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_REQUIRES;
		}else if((rtn = t_keyword((const unsigned char *)"INTERNAL", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_INTERNAL;
		}else if((rtn = t_keyword((const unsigned char *)"EXTERNAL", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_EXTERNAL;
		}else if((rtn = t_asm_register(&asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_REGISTER;
		}else if((rtn = t_constant_hex(&asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_CONSTANT_HEX;
		}else if((rtn = t_constant_decimal(&asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_CONSTANT_DECIMAL;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"add", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_ADD;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"sub", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_SUB;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"mul", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_MUL;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"div", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_DIV;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"beq", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_BEQ;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"blt", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_BLT;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"loa", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_LOA;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"sto", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_STO;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"ll", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_LL;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"and", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_AND;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"or", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_OR;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"not", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_NOT;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"shr", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_SHR;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"shl", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_SHL;
		}else if((rtn = t_keyword_space_check((const unsigned char *)"dw", &asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_DW;
		}else if((rtn = t_identifier(&asm_lexer_state->c, asm_lexer_state->c.position))){
			type = A_IDENTIFIER;
		}else{
			unsigned int i = 0;
			buffered_printf(asm_lexer_state->c.buffered_output, "Lexer stopping on character '%c' 0x(%x)\n", asm_lexer_state->c.buf[asm_lexer_state->c.position], asm_lexer_state->c.buf[asm_lexer_state->c.position]);
			for(asm_lexer_state->c.position = 0; i < asm_lexer_state->c.position + 100; i++){
				if(asm_lexer_state->c.buf[i]){
					buffered_printf(asm_lexer_state->c.buffered_output, "%c", asm_lexer_state->c.buf[i]);
				}
			}
			g_format_buffer_release();
			return 1;
		}

		new_token = memory_pooler_malloc(asm_lexer_token_pool);
		new_token->type = type;
		new_token->first_byte = first_byte;
		new_token->last_byte = (first_byte + rtn) - 1;

		struct_asm_lexer_token_ptr_list_add(&asm_lexer_state->tokens, new_token);
		show_lexer_token(asm_lexer_state->c.buffered_output, get_c_token_type_names()[new_token->type], new_token->first_byte, new_token->last_byte, SHOW_LEXER_TOKENS);
		asm_lexer_state->c.position += rtn;
	}
	g_format_buffer_release();
	return 0;
}

const char ** get_c_token_type_names(void){
	return c_token_type_names;
}

const char ** get_build_script_token_type_names(void){
	return build_script_token_type_names;
}

const char ** get_asm_token_type_names(void){
	return asm_token_type_names;
}
