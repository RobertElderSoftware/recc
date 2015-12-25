/*
    Copyright 2015 Robert Elder Software Inc.
    
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

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "../../data-structures/unsigned_char_list.h"
#include "../../data-structures/unsigned_char_ptr_list.h"
#include "../../recc-implementation/io.h"
#include "../../recc-implementation/preprocessor.h"
#include "../../recc-implementation/lexer.h"
#include "../../recc-implementation/parser.h"
#include "../../recc-implementation/code_generator.h"
#include "../../recc-implementation/memory_pool_collection.h"

void print_parser_nodes(struct parser_node *, unsigned int i);
void print_parser_nodes(struct parser_node * n, unsigned int i){
	if(n){
		unsigned int j;
		unsigned char * c;
		for(j = 0; j < i; j++){
			printf("-> ");
		}
		printf("%s", get_node_type_names()[n->type]);
		if(n->type == TERMINAL){
			printf(" ");
			for(c = n->c_lexer_token->first_byte; c < (n->c_lexer_token->last_byte + 1); c++){
				putchar(*c);
			}
		}
		printf("\n");
		print_parser_nodes(n->next, i);
		print_parser_nodes(n->first_child, i + 1);
	}
}

int main(void){
	const char * file_to_open = "test/c89/empty-main-return-zero.c";
	struct c_lexer_state c_lexer_state;
	struct memory_pool_collection memory_pool_collection;
	struct unsigned_char_list input_characters;
	int rtn = 0;
	struct parser_state parser_state;
	struct unsigned_char_list generated_code;
	struct unsigned_char_list lexer_output;
	struct unsigned_char_list buffered_symbol_table;
	struct preprocessor_state * preprocessor_state;
	struct unsigned_char_list preprocssed_characters;
	struct struct_c_lexer_token_ptr_list output_tokens;
	struct code_gen_state code_gen_state;
	struct_c_lexer_token_ptr_list_create(&output_tokens);
	unsigned_char_list_create(&lexer_output);
	unsigned_char_list_create(&input_characters);
	unsigned_char_list_create(&buffered_symbol_table);
	unsigned_char_list_create(&preprocssed_characters);
	memory_pool_collection_create(&memory_pool_collection);
	unsigned_char_list_create(&generated_code);
	preprocessor_state = create_preprocessor_state(&memory_pool_collection);

	if(!(rtn = get_preprocessed_output_from_file(preprocessor_state, (unsigned char *)file_to_open, &output_tokens))){
		unsigned int i;
		for(i = 0; i < struct_c_lexer_token_ptr_list_size(&output_tokens); i++){
			struct c_lexer_token * current_token = struct_c_lexer_token_ptr_list_get(&output_tokens, i);
			unsigned char * j;
			for(j = current_token->first_byte; j < (current_token->last_byte + 1); j++){
				unsigned_char_list_add_end(&preprocssed_characters, *j);
				putchar(*j);
			}
		}
	}else{
		printf("Nothing to output.  Preprocessing failed for %s\n", file_to_open);
	}
	printf("\nCompleted preprocessing\n");
	printf("\n");

	create_c_lexer_state(&c_lexer_state, &lexer_output, &memory_pool_collection, (unsigned char *)file_to_open, unsigned_char_list_data(&preprocssed_characters), unsigned_char_list_size(&preprocssed_characters));
	rtn = lex_c(&c_lexer_state);
	if(!rtn){
		unsigned int i;
		for(i = 0; i < struct_c_lexer_token_ptr_list_size(&c_lexer_state.tokens); i++){
			struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_get(&c_lexer_state.tokens, i);
			printf("%s\n", get_c_token_type_names()[tok->type]);
		}
		printf("Lex was successful.\n");
	}

	printf("Begin parsing:\n");
	
	create_parser_state(&parser_state, &memory_pool_collection, &c_lexer_state, &generated_code, unsigned_char_list_data(&preprocssed_characters));
	if(parse(&parser_state)){
		printf("Parsing failed.\n");
	}else{
		printf("Full parser true:\n");
		print_parser_nodes(parser_state.top_node, 0);
	}

	create_code_gen_state(&code_gen_state, &parser_state, &generated_code, &buffered_symbol_table);
	if(generate_code(&code_gen_state)){
		printf("Parsing failed.\n");
	}else{
		unsigned int i;
		unsigned int size;
		unsigned char * data;
		printf("Code generation was successful.\n");
		size = unsigned_char_list_size(&generated_code);
		data = unsigned_char_list_data(&generated_code);

		printf("About to print\n");
		printf("%c\n",'a');
		printf("print %d size\n", size);
		printf("print %c first \n", data[0]);
		for(i = 0; i < size; i++){
			printf("%c", data[i]);
		}
	}

	struct_c_lexer_token_ptr_list_destroy(&output_tokens);
	unsigned_char_list_destroy(&input_characters);
	unsigned_char_list_destroy(&generated_code);
	unsigned_char_list_destroy(&buffered_symbol_table);
	unsigned_char_list_destroy(&lexer_output);
	destroy_code_gen_state(&code_gen_state);
	destroy_parser_state(&parser_state);
	destroy_preprocessor_state(preprocessor_state);
	unsigned_char_list_destroy(&preprocssed_characters);
	destroy_c_lexer_state(&c_lexer_state);
	memory_pool_collection_destroy(&memory_pool_collection);
	return 0;
}
