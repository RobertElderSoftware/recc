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
#include <unistd.h>
#include <assert.h>
#include "../data-structures/unsigned_char_list.h"
#include "../data-structures/unsigned_char_ptr_list.h"
#include "../io.h"
#include "../preprocessor.h"
#include "../lexer.h"
#include "../parser.h"
#include "../code_generator.h"

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
	const char * file_to_open = "test/basic-putchar-a.c";
	struct c_lexer_state c_lexer_state;
	struct memory_pooler_collection memory_pooler_collection;
	struct unsigned_char_list input_characters;
	int rtn = 0;
	struct parser_state parser_state;
	struct unsigned_char_list generated_code;
	struct unsigned_char_list buffered_symbol_table;
	struct preprocessor_state * preprocessor_state;
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map macro_map;
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map disabled_macros;
	struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map disabled_tokens;
	struct unsigned_char_list preprocssed_characters;
	struct struct_c_lexer_token_ptr_list output_tokens;
	struct code_gen_state code_gen_state;
	g_format_buffer_use();
	struct_c_lexer_token_ptr_list_create(&output_tokens);
	unsigned_char_ptr_to_struct_macro_definition_ptr_map_create(&disabled_macros);
	struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_create(&disabled_tokens);
	unsigned_char_list_create(&input_characters);
	unsigned_char_list_create(&buffered_symbol_table);
	unsigned_char_list_create(&preprocssed_characters);
	memory_pooler_collection_create(&memory_pooler_collection);
	c_lexer_state.c.memory_pooler_collection = &memory_pooler_collection;
	unsigned_char_list_create(&generated_code);
	code_gen_state.buffered_output = &generated_code; 
	code_gen_state.buffered_symbol_table = &buffered_symbol_table; 
	preprocessor_state = create_preprocessor_state(&memory_pooler_collection);

	unsigned_char_ptr_to_struct_macro_definition_ptr_map_create(&macro_map);

	struct_preprocessor_file_context_ptr_list_add_end(&preprocessor_state->file_contexts, make_preprocessor_file_context(copy_null_terminated_string((unsigned char *)file_to_open)));
	printf("Begin preprocessing ");
	print_file_stack(preprocessor_state);
	if(!(rtn = get_preprocessed_output_from_file(preprocessor_state, (unsigned char *)file_to_open, &output_tokens, &macro_map, &disabled_macros, &disabled_tokens))){
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
	destroy_preprocessor_file_context(struct_preprocessor_file_context_ptr_list_pop_end(&preprocessor_state->file_contexts));

	rtn = lex_c(&c_lexer_state, (unsigned char *)file_to_open, unsigned_char_list_data(&preprocssed_characters), unsigned_char_list_size(&preprocssed_characters));
	if(!rtn){
		unsigned int i;
		for(i = 0; i < struct_c_lexer_token_ptr_list_size(&c_lexer_state.tokens); i++){
			struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_get(&c_lexer_state.tokens, i);
			printf("%s\n", get_c_token_type_names()[tok->type]);
		}
		printf("Lex was successful.\n");
	}

	parser_state.memory_pooler_collection = &memory_pooler_collection; 
	parser_state.buffered_output = &generated_code; 

	printf("Begin parsing:\n");
	if(parse(&c_lexer_state, &parser_state, unsigned_char_list_data(&preprocssed_characters))){
		printf("Parsing failed.\n");
	}else{
		printf("Full parser true:\n");
		print_parser_nodes(parser_state.top_node, 0);
	}

	if(generate_code(&parser_state, &code_gen_state)){
		printf("Parsing failed.\n");
	}else{
		unsigned int i;
		unsigned int size;
		unsigned char * data;
		printf("Code generation was successful.\n");
		size = unsigned_char_list_size(&generated_code);
		data = unsigned_char_list_data(&generated_code);

		for(i = 0; i < size; i++){
			printf("%c", data[i]);
		}
	}

	struct_c_lexer_token_ptr_list_destroy(&output_tokens);
	unsigned_char_list_destroy(&input_characters);
	unsigned_char_list_destroy(&generated_code);
	unsigned_char_list_destroy(&buffered_symbol_table);
	g_format_buffer_release();
	return 0;
}
