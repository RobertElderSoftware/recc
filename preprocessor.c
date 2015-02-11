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

#include "preprocessor.h"

const char * g_stdio_h = "stdio.h";
const char * g_string_h = "string.h";
const char * g_stdlib_h = "stdlib.h";
const char * g_assert_h = "assert.h";

int tokcmp(const char * s1, const char * s2, unsigned int compare_length){
	unsigned int i = 0;
	while(s1[i] && s2[i] && i < compare_length){
		char c = s1[i] - s2[i];
		if(c){
			return c;
		}
		i++;
	}
	if(i < compare_length){
		return s1[i] - s2[i];
	}else{
		return 0;
	}
}

void skip_whitespace(struct c_lexer_state * state, unsigned int * current_token){
	while(
		does_token_match(state, SPACE, *current_token) ||
		does_token_match(state, NEWLINE, *current_token) ||
		does_token_match(state, COMMENT, *current_token)
	){
		*current_token = *current_token + 1;
	}
}

unsigned int does_token_match(struct c_lexer_state * state, enum c_token_type type, unsigned int index){
	return index < struct_c_lexer_token_ptr_list_size(&state->tokens) && struct_c_lexer_token_ptr_list_get(&state->tokens, index)->type == type;
}

int preprocess_tokens(struct memory_pooler_collection * memory_pooler_collection, struct c_lexer_state * c_lexer_state, struct unsigned_char_list * output_list, unsigned char * file_directory, struct unsigned_char_ptr_to_unsigned_char_ptr_map * define_map){
	unsigned int i;
	const char * include = "include";
	const char * define = "define";
	const char * ifndef = "ifndef";
	const char * endif = "endif";
	const char * va_list_str = "va_list";
	const char * va_start_str = "va_start";
	const char * va_arg_str = "va_arg";
	const char * va_end_str = "va_end";
	unsigned int num_tokens = struct_c_lexer_token_ptr_list_size(&c_lexer_state->tokens);
	struct c_lexer_token ** tokens = struct_c_lexer_token_ptr_list_data(&c_lexer_state->tokens);
	unsigned int non_whitespace_encountered = 0;
	unsigned int ignore_everything = 0;
	(void)output_list;
	for(i = 0; i < num_tokens; i++){
		struct c_lexer_token * token = tokens[i];
		unsigned int replaced = 0;
		if(!ignore_everything && token->type == IDENTIFIER){
			non_whitespace_encountered = 1;
			if(!tokcmp((const char *)token->first_byte, va_list_str, (unsigned int)strlen(va_list_str) -1)){
				unsigned_char_list_add(output_list, 'u');
				unsigned_char_list_add(output_list, 'n');
				unsigned_char_list_add(output_list, 's');
				unsigned_char_list_add(output_list, 'i');
				unsigned_char_list_add(output_list, 'g');
				unsigned_char_list_add(output_list, 'n');
				unsigned_char_list_add(output_list, 'e');
				unsigned_char_list_add(output_list, 'd');
				unsigned_char_list_add(output_list, ' ');
				unsigned_char_list_add(output_list, 'i');
				unsigned_char_list_add(output_list, 'n');
				unsigned_char_list_add(output_list, 't');
				unsigned_char_list_add(output_list, ' ');
				unsigned_char_list_add(output_list, '*');
				replaced = 1;
			} else if(!tokcmp((const char *)token->first_byte, va_start_str, (unsigned int)strlen(va_start_str) -1)){
				unsigned int current_token = i + 1;
				unsigned int identifier1_index = 0;
				unsigned int identifier2_index = 0;
				unsigned int abort = 0;

				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, OPEN_PAREN_CHAR, current_token)){ current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, IDENTIFIER, current_token)){ identifier1_index = current_token; current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, COMMA_CHAR, current_token)){ current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, IDENTIFIER, current_token)){ identifier2_index = current_token; current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, CLOSE_PAREN_CHAR, current_token)){ current_token++; } else {abort = 1;}

				if(!abort){
					unsigned char * c;
					for(c = tokens[identifier1_index]->first_byte; c < tokens[identifier1_index]->last_byte +1; c++){
						unsigned_char_list_add(output_list, *c);
					}
					unsigned_char_list_add(output_list, ' ');
					unsigned_char_list_add(output_list, '=');
					unsigned_char_list_add(output_list, ' ');
					unsigned_char_list_add(output_list, '(');
					unsigned_char_list_add(output_list, 'u');
					unsigned_char_list_add(output_list, 'n');
					unsigned_char_list_add(output_list, 's');
					unsigned_char_list_add(output_list, 'i');
					unsigned_char_list_add(output_list, 'g');
					unsigned_char_list_add(output_list, 'n');
					unsigned_char_list_add(output_list, 'e');
					unsigned_char_list_add(output_list, 'd');
					unsigned_char_list_add(output_list, ' ');
					unsigned_char_list_add(output_list, 'i');
					unsigned_char_list_add(output_list, 'n');
					unsigned_char_list_add(output_list, 't');
					unsigned_char_list_add(output_list, '*');
					unsigned_char_list_add(output_list, ')');
					unsigned_char_list_add(output_list, '&');
					for(c = tokens[identifier2_index]->first_byte; c < tokens[identifier2_index]->last_byte +1; c++){
						unsigned_char_list_add(output_list, *c);
					}
					i = (current_token -1);
					replaced = 1;
				}
			} else if(!tokcmp((const char *)token->first_byte, va_arg_str, (unsigned int)strlen(va_arg_str) -1)){
				unsigned int current_token = i + 1;
				unsigned int identifier1_index = 0;
				unsigned int identifier2_index = 0;
				unsigned int abort = 0;

				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, OPEN_PAREN_CHAR, current_token)){ current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, IDENTIFIER, current_token)){ identifier1_index = current_token; current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, COMMA_CHAR, current_token)){ current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				while(
					does_token_match(c_lexer_state, MULTIPLY_CHAR, current_token) ||
					does_token_match(c_lexer_state, SPACE, current_token) ||
					does_token_match(c_lexer_state, NEWLINE, current_token) ||
					does_token_match(c_lexer_state, IDENTIFIER, current_token) ||
					does_token_match(c_lexer_state, UNSIGNED, current_token) ||
					does_token_match(c_lexer_state, CONST, current_token) ||
					does_token_match(c_lexer_state, CHAR, current_token) ||
					does_token_match(c_lexer_state, INT, current_token) ||
					does_token_match(c_lexer_state, LONG, current_token) ||
					does_token_match(c_lexer_state, FLOAT, current_token) ||
					does_token_match(c_lexer_state, DOUBLE, current_token) ||
					does_token_match(c_lexer_state, STRUCT, current_token)
				){
					identifier2_index = current_token;
					current_token++;
				}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, CLOSE_PAREN_CHAR, current_token)){ current_token++; } else {abort = 1;}

				if(!abort){
					unsigned char * c;
					unsigned_char_list_add(output_list, '*');
					unsigned_char_list_add(output_list, '(');
					unsigned_char_list_add(output_list, 'u');
					unsigned_char_list_add(output_list, 'n');
					unsigned_char_list_add(output_list, 's');
					unsigned_char_list_add(output_list, 'i');
					unsigned_char_list_add(output_list, 'g');
					unsigned_char_list_add(output_list, 'n');
					unsigned_char_list_add(output_list, 'e');
					unsigned_char_list_add(output_list, 'd');
					unsigned_char_list_add(output_list, ' ');
					unsigned_char_list_add(output_list, 'i');
					unsigned_char_list_add(output_list, 'n');
					unsigned_char_list_add(output_list, 't');
					unsigned_char_list_add(output_list, '*');
					unsigned_char_list_add(output_list, ')');
					unsigned_char_list_add(output_list, '(');
					for(c = tokens[identifier1_index]->first_byte; c < tokens[identifier1_index]->last_byte +1; c++){
						unsigned_char_list_add(output_list, *c);
					}
					unsigned_char_list_add(output_list, '=');
					unsigned_char_list_add(output_list, '(');
					for(c = tokens[identifier1_index]->first_byte; c < tokens[identifier1_index]->last_byte +1; c++){
						unsigned_char_list_add(output_list, *c);
					}
					unsigned_char_list_add(output_list, '-');
					unsigned_char_list_add(output_list, '1');
					unsigned_char_list_add(output_list, ')');
					unsigned_char_list_add(output_list, ')');
					(void)identifier2_index; /* TODO: support for other data types not implemented */
					replaced = 1;
					i = (current_token -1);
				}
			} else if(!tokcmp((const char *)token->first_byte, va_end_str, (unsigned int)strlen(va_end_str) -1)){
				unsigned int current_token = i + 1;
				unsigned int abort = 0;
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, OPEN_PAREN_CHAR, current_token)){ current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, IDENTIFIER, current_token)){ current_token++; } else {abort = 1;}
				skip_whitespace(c_lexer_state, &current_token);
				if(does_token_match(c_lexer_state, CLOSE_PAREN_CHAR, current_token)){ current_token++; } else {abort = 1;}

				if(!abort){
					/* va_end is just ignored */
					replaced = 1;
					i = (current_token -1);
				}
			}else{
				unsigned int current_token = i;
				unsigned char * define_key;
				unsigned char * define_value;
				unsigned char * c;
				define_key = copy_string(tokens[current_token]->first_byte, tokens[current_token]->last_byte);
				define_value = unsigned_char_ptr_to_unsigned_char_ptr_map_get(define_map, define_key);
				if(define_value){
					for(c = define_value; *c; c++){
						unsigned_char_list_add(output_list, *c);
					}

					replaced = 1;
					i = current_token;
				}
				free(define_key);
			}
		} else if(token->type == NEWLINE){
			non_whitespace_encountered = 0;
		} else if(token->type == NUMBER_SIGN_CHAR && !non_whitespace_encountered){
			unsigned int current_token = i+1;
			non_whitespace_encountered = 1;

			/* White space is allowed after the # token */
			skip_whitespace(c_lexer_state, &current_token);

			/*  File includes */
			if(!ignore_everything && !tokcmp((const char *)tokens[current_token]->first_byte, include, (unsigned int)strlen(include) -1)){
				current_token++;
				skip_whitespace(c_lexer_state, &current_token);
				if(tokens[current_token]->type == OPEN_ANGLE_BRACKET_CHAR){
					struct unsigned_char_list standard_include;
					unsigned_char_list_create(&standard_include);
					current_token++;
					while(!does_token_match(c_lexer_state, CLOSE_ANGLE_BRACKET_CHAR, current_token)){
						unsigned char * c;
						for(c = tokens[current_token]->first_byte; c < tokens[current_token]->last_byte +1; c++){
							unsigned_char_list_add(&standard_include, *c);
						}
						current_token++;
					}
					current_token++;
					unsigned_char_list_add(&standard_include, 0); /* Null terminator */
					/* Not much support for standard library */
					if(
						!unsigned_strcmp(unsigned_char_list_data(&standard_include), (unsigned char *)g_stdio_h) ||
						!unsigned_strcmp(unsigned_char_list_data(&standard_include), (unsigned char *)g_stdlib_h) ||
						!unsigned_strcmp(unsigned_char_list_data(&standard_include), (unsigned char *)g_assert_h) ||
						!unsigned_strcmp(unsigned_char_list_data(&standard_include), (unsigned char *)g_string_h)
					){
						struct unsigned_char_list file_to_include;
						struct unsigned_char_list preprocessed_output;
						unsigned char * c;
						unsigned int num_chars;
						unsigned int g;
						const char * stdlib_directory = "stdlib/";
						unsigned_char_list_create(&file_to_include);
						unsigned_char_list_create(&preprocessed_output);
						c = (unsigned char *)stdlib_directory;
						/* Add file directory */
						while(*c){
							unsigned_char_list_add(&file_to_include, *c);
							c++;
						}
						/* Add file being included */
						for(g = 0; g < unsigned_char_list_size(&standard_include); g++){
							unsigned_char_list_add(&file_to_include, unsigned_char_list_get(&standard_include, g));
						}
						/* Null terminator */
						unsigned_char_list_add(&file_to_include, 0);

						get_preprocessed_output_from_file(memory_pooler_collection, unsigned_char_list_data(&file_to_include), &preprocessed_output, define_map);
						num_chars = unsigned_char_list_size(&preprocessed_output);
						/* Output the result of the header file. */
						for(g = 0; g < num_chars; g++){
							unsigned_char_list_add(output_list, unsigned_char_list_get(&preprocessed_output, g));
						}

						unsigned_char_list_destroy(&file_to_include);
						unsigned_char_list_destroy(&preprocessed_output);
					}else{
						unsigned char * c = unsigned_char_list_data(&standard_include);
						unsigned_char_list_add(output_list, '/');
						unsigned_char_list_add(output_list, '*');
						unsigned_char_list_add(output_list, 'T');
						unsigned_char_list_add(output_list, 'O');
						unsigned_char_list_add(output_list, 'D');
						unsigned_char_list_add(output_list, 'O');
						unsigned_char_list_add(output_list, ' ');
						while(*c){
							unsigned_char_list_add(output_list, *c);
							c++;
						}
						unsigned_char_list_add(output_list, '*');
						unsigned_char_list_add(output_list, '/');
					}
					unsigned_char_list_destroy(&standard_include);
				}else if(tokens[current_token]->type == STRING_LITERAL){
					struct unsigned_char_list file_to_include;
					struct unsigned_char_list preprocessed_output;
					unsigned char * c;
					unsigned int num_chars;
					unsigned int g;
					unsigned_char_list_create(&file_to_include);
					unsigned_char_list_create(&preprocessed_output);
					c = file_directory;
					/* Add file directory */
					while(*c){
						unsigned_char_list_add(&file_to_include, *c);
						c++;
					}
					/* Add file being included */
					for(c = tokens[current_token]->first_byte + 1; c < tokens[current_token]->last_byte; c++){
						unsigned_char_list_add(&file_to_include, *c);
					}
					/* Null terminator */
					unsigned_char_list_add(&file_to_include, 0);

					get_preprocessed_output_from_file(memory_pooler_collection, unsigned_char_list_data(&file_to_include), &preprocessed_output, define_map);
					num_chars = unsigned_char_list_size(&preprocessed_output);
					/* Output the result of the header file. */
					for(g = 0; g < num_chars; g++){
						unsigned_char_list_add(output_list, unsigned_char_list_get(&preprocessed_output, g));
					}

					current_token++;
					unsigned_char_list_destroy(&file_to_include);
					unsigned_char_list_destroy(&preprocessed_output);
				}else{
					assert(0 && "Unexpect #include syntax");
				}

			}else if(!ignore_everything && !tokcmp((const char *)tokens[current_token]->first_byte, define, (unsigned int)strlen(define) -1)){
				unsigned char * define_key;
				unsigned char * define_value;
				unsigned char * previous_value;
				current_token++;
				while(does_token_match(c_lexer_state, SPACE, current_token)){ current_token++; }

				define_key = copy_string(tokens[current_token]->first_byte, tokens[current_token]->last_byte);
				current_token++;
				while(does_token_match(c_lexer_state, SPACE, current_token)){ current_token++; }

				if(does_token_match(c_lexer_state, NEWLINE, current_token)){
					define_value = malloc(1);
					define_value[0] = 0;
				}else{
					define_value = copy_string(tokens[current_token]->first_byte, tokens[current_token]->last_byte);
					current_token++;
				}

				if((previous_value = unsigned_char_ptr_to_unsigned_char_ptr_map_get(define_map, define_key))){
					assert(0 && "Macro redefinition not implemented.\n");
				}else{
					unsigned_char_ptr_to_unsigned_char_ptr_map_put(define_map, define_key, define_value);
				}
			}else if(!ignore_everything && !tokcmp((const char *)tokens[current_token]->first_byte, ifndef, (unsigned int)strlen(ifndef) -1)){
				unsigned char * previous_value;
				unsigned char * define_key;
				current_token++;
				while(does_token_match(c_lexer_state, SPACE, current_token)){ current_token++; }
				define_key = copy_string(tokens[current_token]->first_byte, tokens[current_token]->last_byte);
				current_token++;
				previous_value = unsigned_char_ptr_to_unsigned_char_ptr_map_get(define_map, define_key);
				if(previous_value){
					/* If this define value was defined, start ignoring everything until an endif */
					ignore_everything = 1;
				}
				free(define_key);
			}else if(!tokcmp((const char *)tokens[current_token]->first_byte, endif, (unsigned int)strlen(endif) -1)){
				current_token++;
				ignore_everything = 0;
			}else{
				if(!ignore_everything){
					unsigned char * c;
					printf("Found: ");
					for(c = tokens[current_token]->first_byte; c < tokens[current_token]->last_byte + 1; c++){
						printf("%c", *c);
					}
					printf("\n");
					assert(0 && "Unknown macro.\n");
				}
			}

			if(!ignore_everything){
				i = (current_token -1);
				replaced = 1;
			}
		}else if(token->type == SPACE){
			/* Do nothing to keep non_whitespace_encountered in current state */
		}else{
			non_whitespace_encountered = 1;
		}

		if(!replaced && !ignore_everything){
			unsigned char * c;
			for(c = tokens[i]->first_byte; c < tokens[i]->last_byte +1; c++){
				unsigned_char_list_add(output_list, *c);
			}
		}
	}
	return 0;
}

unsigned char * convert_filename_to_directory(unsigned char * str){
	unsigned char * end = get_null_terminator(str);
	while((end - 1) != str){
		if(*(end - 1) == '/'){
			*end = 0;
			return str;
		}
		end--;
	}
	str[0] = 0;
	return str;
}

	int get_preprocessed_output_from_file(struct memory_pooler_collection * memory_pooler_collection, unsigned char * in_file, struct unsigned_char_list * out_characters, struct unsigned_char_ptr_to_unsigned_char_ptr_map * define_map){
	struct unsigned_char_list lexer_output;
	struct unsigned_char_list file_input;
	struct c_lexer_state c_lexer_state;
	int rtn;
	unsigned int i; 
	unsigned_char_list_create(&lexer_output);
	c_lexer_state.c.buffered_output = &lexer_output;
	c_lexer_state.c.memory_pooler_collection = memory_pooler_collection; 
	unsigned_char_list_create(&file_input);
	/*  Read the file we want to preprocess */
	add_file_to_buffer(&file_input, (char*)in_file);

	/*  Using the c lexer for the unpreprocessed input allow us to preprocess the input easier */
	rtn = lex_c(&c_lexer_state, in_file, unsigned_char_list_data(&file_input), unsigned_char_list_size(&file_input));

	/*  Typically, there is only output if there is an error.  There could also be debug info output */
	for(i = 0; i < unsigned_char_list_size(&lexer_output); i++){
		printf("%c", unsigned_char_list_get(&lexer_output, i));
	}
	unsigned_char_list_destroy(&lexer_output);

	if(rtn){
		unsigned_char_list_destroy(&file_input);
		printf("Lexical analysis failed during preprocessing of %s\n", in_file);
		return rtn;
	}else{
		unsigned char * file_directory = convert_filename_to_directory(copy_string(in_file, get_null_terminator(in_file)));
		int result = preprocess_tokens(memory_pooler_collection, &c_lexer_state, out_characters, file_directory, define_map);
		if(result){
			printf("Preprocessing failed for %s\n", in_file);
		}
		unsigned_char_list_destroy(&file_input);
		destroy_c_lexer_state(&c_lexer_state);
		free(file_directory);
		return result;
	}
}


void free_define_map(struct unsigned_char_ptr_to_unsigned_char_ptr_map * map){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_unsigned_char_ptr_map_keys(map);
	unsigned int size = unsigned_char_ptr_list_size(&keys);
	unsigned int i;
	for(i = 0; i < size; i++){
		free(unsigned_char_ptr_to_unsigned_char_ptr_map_get(map, unsigned_char_ptr_list_get(&keys, i)));
	}
	/*  Need to do this twice because we're deleting the data under the keys of the map */
	for(i = 0; i < size; i++){
		free(unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_to_unsigned_char_ptr_map_destroy(map);
	unsigned_char_ptr_list_destroy(&keys);
}

int do_preprocess(struct memory_pooler_collection * memory_pooler_collection, unsigned char * in_file, unsigned char * out_file){
	int rtn; 
	struct unsigned_char_list file_output;
	struct unsigned_char_ptr_to_unsigned_char_ptr_map define_map;
	unsigned_char_list_create(&file_output);

	unsigned_char_ptr_to_unsigned_char_ptr_map_create(&define_map, unsigned_strcmp);
	if(!(rtn = get_preprocessed_output_from_file(memory_pooler_collection, in_file, &file_output, &define_map))){
		output_buffer_to_file(&file_output, (char*)out_file);
	}else{
		printf("Nothing to output.  Preprocessing failed for %s\n", in_file);
	}

	free_define_map(&define_map);
	unsigned_char_list_destroy(&file_output);
	return rtn;
}
