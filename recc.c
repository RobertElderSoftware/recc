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
#include <stdio.h>
#include <stdlib.h>
#include "preprocessor.h"
#include "lexer.h"
#include "parser.h"
#include "linker.h"
#include "code_generator.h"
#include "data-structures/struct_build_script_lexer_token_ptr_list.h"
#include "data-structures/unsigned_char_list.h"
#include "data-structures/memory_pooler.h"


int process_build_script(struct memory_pooler_collection *, char *);
int execute_build_script(struct memory_pooler_collection *, struct build_script_lexer_state *);

int execute_build_script(struct memory_pooler_collection * memory_pooler_collection, struct build_script_lexer_state * state){
	unsigned int num_tokens = struct_build_script_lexer_token_ptr_list_size(&state->tokens);
	struct build_script_lexer_token ** tokens =  struct_build_script_lexer_token_ptr_list_data(&state->tokens);
	struct build_script_lexer_token * filename1;
	struct build_script_lexer_token * filename2;
	unsigned int i = 0;
	while(i < num_tokens){
		if(tokens[i]->type == B_NEWLINE){
			/* blank line */
			i++;
			continue;
		}else if(tokens[i]->type == B_LINK){
			i++;
			if(tokens[i]->type == B_SPACE){
				i++;
				if(tokens[i]->type == B_FILENAME){
					struct unsigned_char_ptr_list in_files;
					unsigned_char_ptr_list_create(&in_files);
					unsigned_char_ptr_list_add_end(&in_files, copy_string(tokens[i]->first_byte, tokens[i]->last_byte));
					i++;
					while(tokens[i]->type != B_SPACE || tokens[i]->type != B_COMMA_CHAR || tokens[i]->type != B_FILENAME){
						if(tokens[i]->type == B_COMMA_CHAR || tokens[i]->type == B_SPACE){
							i++;
						} else if(tokens[i]->type == B_FILENAME){
							unsigned_char_ptr_list_add_end(&in_files, copy_string(tokens[i]->first_byte, tokens[i]->last_byte));
							i++;
						} else if(tokens[i]->type == B_TO){
							break;
						} else {printf("Error: no space, comma, TO or filename.");}
					}

					if(tokens[i]->type == B_TO){
						i++;
						if(tokens[i]->type == B_SPACE){
							i++;
							if(tokens[i]->type == B_FILENAME){
								unsigned char * symbol_file = 0;
								filename2 = tokens[i];
								i++;
								if(tokens[i]->type == B_SPACE){
									i++;
									if(tokens[i]->type == B_SYMBOLS){
										i++;
										if(tokens[i]->type == B_SPACE){
											i++;
											if(tokens[i]->type == B_TO){
												i++;
												if(tokens[i]->type == B_SPACE){
													i++;
													if(tokens[i]->type == B_FILENAME){
														struct build_script_lexer_token * file_token;
														file_token = tokens[i];
														symbol_file = copy_string(file_token->first_byte, file_token->last_byte);
														i++;
													} else {printf("Error: filename.");}
												} else {printf("Error: no space.");}
											} else {printf("Error: no TO.");}
										} else {printf("Error: no space.");}
									} else {printf("Error: tailing space.");}
								}/* Optional */
								{
									unsigned char * f2 = copy_string(filename2->first_byte, filename2->last_byte);
									unsigned int j;
									printf("Linking ");
									for(j = 0; j < unsigned_char_ptr_list_size(&in_files); j++){
										printf("%s", unsigned_char_ptr_list_get(&in_files, j));
										if(j != unsigned_char_ptr_list_size(&in_files) -1){
											printf(",");
										}
									}
									printf(" to %s\n", f2);
									do_link(memory_pooler_collection, &in_files, f2, symbol_file);
									for(j = 0; j < unsigned_char_ptr_list_size(&in_files); j++){
										free(unsigned_char_ptr_list_get(&in_files, j));
									}
									unsigned_char_ptr_list_destroy(&in_files);
									free(f2);

									if(symbol_file){
										printf("Creating symbol file %s\n", symbol_file);
									}
									free(symbol_file);
								}
							} else {printf("Error: no filename.");}
						} else {printf("Error: no space.");}
					} else {printf("Error: no TO.");}
				} else {printf("Error: no filename.");}
			} else {printf("Error: no space.");}
		}else if(tokens[i]->type == B_CODE_GENERATE){
			i++;
			if(tokens[i]->type == B_SPACE){
				i++;
				if(tokens[i]->type == B_FILENAME){
					filename1 = tokens[i];
					i++;
					if(tokens[i]->type == B_SPACE){
						i++;
						if(tokens[i]->type == B_TO){
							i++;
							if(tokens[i]->type == B_SPACE){
								i++;
								if(tokens[i]->type == B_FILENAME){
									filename2 = tokens[i];
									i++;
									{
										unsigned int charsf1 = (unsigned int)(filename1->last_byte - filename1->first_byte) + 1;
										unsigned int charsf2 = (unsigned int)(filename2->last_byte - filename2->first_byte) + 1;
										unsigned int j;
										unsigned char * f1 = (unsigned char *)malloc(charsf1 + 1);
										unsigned char * f2 = (unsigned char *)malloc(charsf2 + 1);
										for(j = 0; j < charsf1; j++){
											f1[j] = filename1->first_byte[j];
										}
										for(j = 0; j < charsf2; j++){
											f2[j] = filename2->first_byte[j];
										}
										f1[charsf1] = 0;
										f2[charsf2] = 0;
										printf("Code generating %s to %s\n", f1, f2);
										do_code_generation(memory_pooler_collection, f1, f2);
										free(f1);
										free(f2);
									}
								} else {printf("Error: no filename.");}
							} else {printf("Error: no space.");}
						} else {printf("Error: no TO.");}
					} else {printf("Error: no space.");}
				} else {printf("Error: no filename.");}
			} else {printf("Error: no space.");}
		}else if(tokens[i]->type == B_PREPROCESS){
			i++;
			if(tokens[i]->type == B_SPACE){
				i++;
				if(tokens[i]->type == B_FILENAME){
					filename1 = tokens[i];
					i++;
					if(tokens[i]->type == B_SPACE){
						i++;
						if(tokens[i]->type == B_TO){
							i++;
							if(tokens[i]->type == B_SPACE){
								i++;
								if(tokens[i]->type == B_FILENAME){
									filename2 = tokens[i];
									i++;
									{
										unsigned int charsf1 = (unsigned int)(filename1->last_byte - filename1->first_byte) + 1;
										unsigned int charsf2 = (unsigned int)(filename2->last_byte - filename2->first_byte) + 1;
										unsigned int j;
										unsigned char * f1 = (unsigned char *)malloc(charsf1 + 1);
										unsigned char * f2 = (unsigned char *)malloc(charsf2 + 1);
										for(j = 0; j < charsf1; j++){
											f1[j] = filename1->first_byte[j];
										}
										for(j = 0; j < charsf2; j++){
											f2[j] = filename2->first_byte[j];
										}
										f1[charsf1] = 0;
										f2[charsf2] = 0;
										printf("Preprocessing %s to %s\n", f1, f2);
										do_preprocess(memory_pooler_collection, f1, f2);
										free(f1);
										free(f2);
										i++;
									}
								} else {printf("Error: no filename.");}
							} else {printf("Error: no space.");}
						} else {printf("Error: no TO.");}
					} else {printf("Error: no space.");}
				} else {printf("Error: no filename.");}
			} else {printf("Error: no space.");}
		}else{
			assert(0 && "Unable to process build script.");
		}
	}
	return 0;
}

int process_build_script(struct memory_pooler_collection * memory_pooler_collection, char * in_file){
	int rtn;
	unsigned int i;
	struct build_script_lexer_state build_script_lexer_state;
	struct unsigned_char_list lexer_output;
	struct unsigned_char_list build_script;
	unsigned_char_list_create(&lexer_output);
	unsigned_char_list_create(&build_script);
	build_script_lexer_state.c.memory_pooler_collection = memory_pooler_collection;

	add_file_to_buffer(&build_script, in_file);

	rtn = lex_build_script(&build_script_lexer_state, (unsigned char *)in_file, unsigned_char_list_data(&build_script), unsigned_char_list_size(&build_script));

	for(i = 0; i < unsigned_char_list_size(&lexer_output); i++){
		printf("%c", unsigned_char_list_get(&lexer_output, i));
	}

	if(rtn){
		printf("Lexical analysis failed during preprocessing of build script %s\n", in_file);
	}else{
		execute_build_script(memory_pooler_collection, &build_script_lexer_state);
		destroy_build_script_lexer_state(&build_script_lexer_state);
	}
	unsigned_char_list_destroy(&build_script);
	unsigned_char_list_destroy(&lexer_output);
	return rtn;
}

int main(int argc, char** argv){
	struct memory_pooler_collection memory_pooler_collection;
	memory_pooler_collection_create(&memory_pooler_collection);

	assert(argc == 2 && "Compiler was invoked with the wrong number of arguments.  Sorry, I haven't had the time to make this more user friendly.");

	process_build_script(&memory_pooler_collection, argv[1]);

	memory_pooler_collection_destroy(&memory_pooler_collection);
	return 0;
}
