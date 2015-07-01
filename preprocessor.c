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

#include "preprocessor.h"

static const char * comma = ",";

void removes_quotes_from_string_literal(unsigned char * str){
	unsigned char * a = str;
	unsigned char * b = str + 1;
	unsigned char * c = str + 2;
	while(*c){
		*a = *b;
		a++;
		b++;
		c++;
	}
	*a = 0;
}

unsigned char * convert_filename_to_directory(unsigned char * str){
	unsigned char * end = get_null_terminator(str);
	while((end - 1) != str){
		if(*((unsigned char *)(end - 1)) == '/'){
			*end = 0;
			return str;
		}
		end = (unsigned char *)(end -1);
	}
	str[0] = 0;
	return str;
}

unsigned int is_in_active_branch(struct preprocessor_state * state){
	unsigned int size = struct_if_branch_ptr_list_size(&state->if_branches);
	return size == 0 || struct_if_branch_ptr_list_get(&state->if_branches, size -1)->active;
}

unsigned int is_non_whitespace_inline_token(struct c_lexer_token * t){
	return !(t->type == COMMENT || t->type == SPACE);
}

struct c_lexer_token * read_until_next_token(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * input_tokens, struct struct_c_lexer_token_ptr_list * working_tokens){
	struct c_lexer_token * current_token;
	(void)state;
	while(struct_c_lexer_token_ptr_list_size(input_tokens)){
		current_token = struct_c_lexer_token_ptr_list_pop_end(input_tokens);
		struct_c_lexer_token_ptr_list_add_end(working_tokens, current_token);
		if(is_non_whitespace_inline_token(current_token)){
			return current_token;
		}
	}
	return (struct c_lexer_token *)0;
}

void release_working_tokens(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * output_tokens, struct struct_c_lexer_token_ptr_list * working_tokens){
	/*  Add the current set of working tokens to output tokens, since they have been processed. */
	unsigned int num_working_tokens = struct_c_lexer_token_ptr_list_size(working_tokens);
	struct c_lexer_token ** working_tokens_data = struct_c_lexer_token_ptr_list_data(working_tokens);
	unsigned int i;
		for(i = 0; i < num_working_tokens; i++){
			if(is_in_active_branch(state)){ /*  Don't output tokens from inactive branch */
				struct_c_lexer_token_ptr_list_add_end(output_tokens, working_tokens_data[i]);
			}
			/*  Account for newlines in comments */
			if(working_tokens_data[i]->type == COMMENT){
				unsigned int num_comment_newlines = count_newlines_in_comment(working_tokens_data[i]);
				unsigned int j;
				for(j = 0; j < num_comment_newlines; j++){
					increment_current_line(state);
				}
			}
		}
	/*  Remove all tokens from working set */
	struct_c_lexer_token_ptr_list_destroy(working_tokens);
	struct_c_lexer_token_ptr_list_create(working_tokens);
}

enum directive_type get_directive_type(struct c_lexer_token * token){
	unsigned char * str = copy_string(token->first_byte, token->last_byte);
	enum directive_type rtn;
	if(!unsigned_strcmp(str, (unsigned char *)"define")){
		rtn = DEFINE_DIRECTIVE;
	}else if(!unsigned_strcmp(str, (unsigned char *)"include")){
		rtn = INCLUDE_DIRECTIVE;
	}else if(!unsigned_strcmp(str, (unsigned char *)"pragma")){
		rtn = PRAGMA_DIRECTIVE;
	}else if(!unsigned_strcmp(str, (unsigned char *)"error")){
		rtn = ERROR_DIRECTIVE;
	}else if(!unsigned_strcmp(str, (unsigned char *)"if")){
		rtn = IF_DIRECTIVE;
	}else if(!unsigned_strcmp(str, (unsigned char *)"endif")){
		rtn = ENDIF_DIRECTIVE;
	}else if(!unsigned_strcmp(str, (unsigned char *)"ifndef")){
		rtn = IFNDEF_DIRECTIVE;
	}else if(!unsigned_strcmp(str, (unsigned char *)"ifdef")){
		rtn = IFDEF_DIRECTIVE;
	}else{
		rtn = DEFINE_DIRECTIVE;
		assert(0 && "Unknown preprocessor directive.");
	}
	free(str);
	return rtn;
}

void process_define_directive(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * define_body_tokens, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map * disabled_tokens){
	struct struct_c_lexer_token_ptr_list processed_tokens;
	struct c_lexer_token * def_token;
	struct c_lexer_token * current_token = (struct c_lexer_token *)0;
	struct c_lexer_token * identifier_token;
	struct macro_definition * new_macro_definition = malloc(sizeof(struct macro_definition));
	unsigned char * macro_identifier;
	const char * va_arg_str = "__VA_ARGS__";
	struct_c_lexer_token_ptr_list_create(&processed_tokens);
	unsigned_char_ptr_to_struct_macro_parameter_ptr_map_create(&new_macro_definition->function_macro_parameters);

	identifier_token = read_until_next_token(state, define_body_tokens, &processed_tokens);
	assert(identifier_token->type == IDENTIFIER); /*  Identifier for the macro */
	if(struct_c_lexer_token_ptr_list_size(define_body_tokens)){
		current_token = struct_c_lexer_token_ptr_list_pop_end(define_body_tokens);
	}
	(void)disabled_macros;
	(void)disabled_tokens;
	if(current_token && current_token->type == OPEN_PAREN_CHAR){
		unsigned parameter_index = 0;
		/*  Function like macro */
		new_macro_definition->type = FUNCTION_MACRO;
		while((current_token = read_until_next_token(state, define_body_tokens, &processed_tokens))->type != CLOSE_PAREN_CHAR){
			switch(current_token->type){
				case ELLIPSIS:{
					/* Case statement fallthrough to identifier */
					unsigned char * parameter_identifier = copy_string((unsigned char *)va_arg_str, get_null_terminator((unsigned char *)va_arg_str));
					struct macro_parameter * new_parameter = malloc(sizeof(struct macro_parameter));
					new_parameter->is_variadic = 1;
					new_parameter->position_index = parameter_index;
					assert(!unsigned_char_ptr_to_struct_macro_parameter_ptr_map_exists(&new_macro_definition->function_macro_parameters, parameter_identifier) && "Duplicate parameter name in macro.");
					unsigned_char_ptr_to_struct_macro_parameter_ptr_map_put(&new_macro_definition->function_macro_parameters, parameter_identifier, new_parameter);
					parameter_index++;
					break;
				}case IDENTIFIER:{
					unsigned char * parameter_identifier = copy_string(current_token->first_byte, current_token->last_byte);
					struct macro_parameter * new_parameter = malloc(sizeof(struct macro_parameter));
					new_parameter->is_variadic = 0;
					new_parameter->position_index = parameter_index;
					assert(!unsigned_char_ptr_to_struct_macro_parameter_ptr_map_exists(&new_macro_definition->function_macro_parameters, parameter_identifier) && "Duplicate parameter name in macro.");
					unsigned_char_ptr_to_struct_macro_parameter_ptr_map_put(&new_macro_definition->function_macro_parameters, parameter_identifier, new_parameter);
					parameter_index++;
					break;
				}case COMMA_CHAR:{
					break;
				}default:{
					assert(0 && "Not expected.");
					break;
				}
			}
		}
		/*  Skip over whitespace */
		current_token = read_until_next_token(state, define_body_tokens, &processed_tokens);
		/*  Put first non-whitespace token back */
		if(current_token){
			struct_c_lexer_token_ptr_list_add_end(define_body_tokens, current_token);
		}
	}else if(!current_token || current_token->type == SPACE){
		/*  Object like macro */
		new_macro_definition->type = OBJECT_MACRO;
	}else{
		printf("type is %s\n", get_c_token_type_names()[current_token->type]);
		assert(0 && "Bad macro definition.");
	}
	/*  Save the macro definition tokens */
	struct_c_lexer_token_ptr_list_create(&new_macro_definition->definition_tokens);
	if(current_token){ /* If we're already at end, its an empty definition */
		do{
			def_token = read_until_next_token(state, define_body_tokens, &new_macro_definition->definition_tokens);
		}while(def_token);
	}

	macro_identifier = copy_string(identifier_token->first_byte, identifier_token->last_byte);
	/*  Save our macro definition */
	unsigned_char_ptr_to_struct_macro_definition_ptr_map_put(macro_map, macro_identifier, new_macro_definition);

	struct_c_lexer_token_ptr_list_destroy(&processed_tokens);
}

void process_include_directive(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * output_tokens, struct struct_c_lexer_token_ptr_list * include_body_tokens, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map * disabled_tokens){
	struct struct_c_lexer_token_ptr_list processed_include_body_tokens;
	struct struct_c_lexer_token_ptr_list working_tokens;
	struct c_lexer_token * current_token;
	const char * stdlib_directory = "libc/";
	unsigned char * current_file = get_current_file(state);
	unsigned char * current_directory = convert_filename_to_directory(copy_string(current_file, get_null_terminator(current_file)));
	struct unsigned_char_list file_to_include;
	unsigned char * final_include_path;
	int preprocess_rtn;
	struct struct_c_lexer_token_ptr_list header_tokens;
	struct_c_lexer_token_ptr_list_create(&header_tokens);
	unsigned_char_list_create(&file_to_include);
	struct_c_lexer_token_ptr_list_create(&processed_include_body_tokens);
	struct_c_lexer_token_ptr_list_create(&working_tokens);
	/*  Need to resolve any macros that might describe the file to include (instead of literal "abc.h") */
	process_tokens(state, &processed_include_body_tokens, include_body_tokens, macro_map, disabled_macros, disabled_tokens, BEGIN_LINE, 0);
	struct_c_lexer_token_ptr_list_reverse(&processed_include_body_tokens);
	current_token = read_until_next_token(state, &processed_include_body_tokens, &working_tokens);
	if(current_token && current_token->type == STRING_LITERAL){
		unsigned char * literal;
		unsigned char * j;
		literal = copy_string(current_token->first_byte, current_token->last_byte);
		removes_quotes_from_string_literal(literal);
		j = current_directory;
		while(*j){
			unsigned_char_list_add_end(&file_to_include, *j);
			j++;
		}

		j = literal;
		while(*j){
			unsigned_char_list_add_end(&file_to_include, *j);
			j++;
		}	
		free(literal);
		current_token = read_until_next_token(state, &processed_include_body_tokens, &working_tokens);
	}else if(current_token && current_token->type == OPEN_ANGLE_BRACKET_CHAR){
		unsigned char * j;
		j = (unsigned char *)stdlib_directory;
		while(*j){
			unsigned_char_list_add_end(&file_to_include, *j);
			j++;
		}
		do{
			current_token = read_until_next_token(state, &processed_include_body_tokens, &working_tokens);
			if(current_token && current_token->type != CLOSE_ANGLE_BRACKET_CHAR){
				for(j = current_token->first_byte; j < (current_token->last_byte + 1); j++){
					unsigned_char_list_add_end(&file_to_include, *j);
				}	
			}	
		}while(current_token && current_token->type != CLOSE_ANGLE_BRACKET_CHAR);	
		if(!current_token){
			assert(0 && "Unfinished include directive.");
		}
		current_token = read_until_next_token(state, &processed_include_body_tokens, &working_tokens);
	}else{
		printf("Type is %s\n", get_c_token_type_names()[current_token->type]);
		assert(0 && "Bad include directive.");
	}

	unsigned_char_list_add_end(&file_to_include, 0);
	final_include_path = unsigned_char_list_data(&file_to_include);

	struct_preprocessor_file_context_ptr_list_add_end(&state->file_contexts, make_preprocessor_file_context(copy_string(final_include_path, get_null_terminator(final_include_path))));
	printf("Begin preprocessing ");
	print_file_stack(state);
	if(!(preprocess_rtn = get_preprocessed_output_from_file(state, final_include_path, &header_tokens, macro_map, disabled_macros, disabled_tokens))){
		struct_c_lexer_token_ptr_list_add_all_end(output_tokens, &header_tokens);
	}else{
		printf("Nothing to output.  Preprocessing failed for %s\n", final_include_path);
		assert(0);
	}
	destroy_preprocessor_file_context(struct_preprocessor_file_context_ptr_list_pop_end(&state->file_contexts));
	
	free(current_directory);
	struct_c_lexer_token_ptr_list_destroy(&working_tokens);
	struct_c_lexer_token_ptr_list_destroy(&processed_include_body_tokens);
	unsigned_char_list_destroy(&file_to_include);
	struct_c_lexer_token_ptr_list_destroy(&header_tokens);
}

void process_endif_directive(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * working_tokens)
{
	struct struct_c_lexer_token_ptr_list processed_tokens;
	struct c_lexer_token * current_token;
	struct_c_lexer_token_ptr_list_create(&processed_tokens);
	current_token = read_until_next_token(state, working_tokens, &processed_tokens);
	if(!current_token || current_token->type == NEWLINE){
		struct if_branch * branch;
		assert(struct_if_branch_ptr_list_size(&state->if_branches));
		branch = struct_if_branch_ptr_list_pop_end(&state->if_branches);
		free(branch);
	}else{
		assert(0 && "Expected newline.");
	}
	struct_c_lexer_token_ptr_list_destroy(&processed_tokens);
}

void process_ifndef_directive(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * working_tokens, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map)
{
	struct struct_c_lexer_token_ptr_list processed_tokens;
	struct if_branch * branch = malloc(sizeof(struct if_branch));
	struct c_lexer_token * current_token;
	struct_c_lexer_token_ptr_list_create(&processed_tokens);
	current_token = read_until_next_token(state, working_tokens, &processed_tokens);
	if(current_token->type == IDENTIFIER){
		unsigned char * identifier_str = copy_string(current_token->first_byte, current_token->last_byte);
		unsigned int macro_defined = unsigned_char_ptr_to_struct_macro_definition_ptr_map_exists(macro_map, identifier_str);
		if(macro_defined){
			branch->active = 0;
		}else{
			branch->active = 1;
		}
		free(identifier_str);
	}else{
		assert(0 && "Expected identifier.");
	}
	current_token = read_until_next_token(state, working_tokens, &processed_tokens);
	assert(!current_token);
	struct_if_branch_ptr_list_add_end(&state->if_branches, branch);
	struct_c_lexer_token_ptr_list_destroy(&processed_tokens);
}

unsigned int always_processed_directive(enum directive_type d){
	return d == ENDIF_DIRECTIVE || d == IFDEF_DIRECTIVE || d == IFNDEF_DIRECTIVE || d == IF_DIRECTIVE;
}

void process_directive(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * output_tokens, struct struct_c_lexer_token_ptr_list * working_tokens, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map * disabled_tokens){
	/*  We're now working with a single line that contains a preprocessor directive.  */
	enum directive_type directive_type;
	struct struct_c_lexer_token_ptr_list processed_tokens;
	struct c_lexer_token * current_token;
	struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_pop_end(working_tokens);
	struct_c_lexer_token_ptr_list_create(&processed_tokens);

	/*  Remove newline from end and count after so it isn't counted twice (for includes) */
	if(tok->type != NEWLINE){
		struct_c_lexer_token_ptr_list_add_end(working_tokens, tok);
	}

	/*  Reverse list of working tokens so we can push and pop */
	struct_c_lexer_token_ptr_list_reverse(working_tokens);

	current_token = read_until_next_token(state, working_tokens, &processed_tokens);
	assert(current_token->type == NUMBER_SIGN_CHAR);
	current_token = read_until_next_token(state, working_tokens, &processed_tokens);
	assert(current_token->type == IDENTIFIER);
	directive_type = get_directive_type(current_token);
	if(always_processed_directive(directive_type) || is_in_active_branch(state)){
		switch(directive_type){
			case DEFINE_DIRECTIVE:{
				process_define_directive(state, working_tokens, macro_map, disabled_macros, disabled_tokens);
				break;
			}case INCLUDE_DIRECTIVE:{
				process_include_directive(state, output_tokens, working_tokens, macro_map, disabled_macros, disabled_tokens);
				break;
			}case PRAGMA_DIRECTIVE:{
				break;
			}case ERROR_DIRECTIVE:{
				break;
			}case IF_DIRECTIVE:{
				break;
			}case IFNDEF_DIRECTIVE:{
				process_ifndef_directive(state, working_tokens, macro_map);
				break;
			}case IFDEF_DIRECTIVE:{
				break;
			}case ENDIF_DIRECTIVE:{
				process_endif_directive(state, working_tokens);
				break;
			}default:{
				assert(0 && "Unexpected case.");
				break;
			}
		}
	}
	if(tok->type == NEWLINE){
		increment_current_line(state);
	}
	struct_c_lexer_token_ptr_list_destroy(&processed_tokens);
	/*  Clear working tokens */
	struct_c_lexer_token_ptr_list_destroy(working_tokens);
	struct_c_lexer_token_ptr_list_create(working_tokens);
}

void split_tokens_into_parameters(struct struct_c_lexer_token_ptr_list * all_parameters, struct struct_struct_c_lexer_token_ptr_list_ptr_list * parameter_lists){
	/* Split up the tokens that make up the argument list into lists for each argument position. */
	unsigned int i;
	unsigned int num_tokens = struct_c_lexer_token_ptr_list_size(all_parameters);
	unsigned int paren_level = 0; /* Argument lists for nested function macros need to be tracked */

	struct struct_c_lexer_token_ptr_list tmp_arg_tokens;
	struct struct_c_lexer_token_ptr_list * current_argument_tokens = malloc(sizeof(struct struct_c_lexer_token_ptr_list));
	/*  Temp list before we do a trim */
	struct_c_lexer_token_ptr_list_create(&tmp_arg_tokens);
	struct_c_lexer_token_ptr_list_create(current_argument_tokens);
	for(i = 0; i < num_tokens; i++){
		struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_get(all_parameters, i);

		if(tok->type == OPEN_PAREN_CHAR){
			paren_level++;
		}

		if(tok->type == CLOSE_PAREN_CHAR){
			assert(paren_level);
			paren_level--;
		}

		if(!(tok->type == COMMA_CHAR && paren_level == 0)){
			struct_c_lexer_token_ptr_list_add_end(&tmp_arg_tokens, tok);
		}

		if(tok->type == COMMA_CHAR && paren_level == 0){
			make_trimmed_arg_list(current_argument_tokens, &tmp_arg_tokens);
			struct_struct_c_lexer_token_ptr_list_ptr_list_add_end(parameter_lists, current_argument_tokens);
			current_argument_tokens = malloc(sizeof(struct struct_c_lexer_token_ptr_list));
			struct_c_lexer_token_ptr_list_create(current_argument_tokens);
			/*  Clear temp tokens */
			struct_c_lexer_token_ptr_list_destroy(&tmp_arg_tokens);
			struct_c_lexer_token_ptr_list_create(&tmp_arg_tokens);
		}
	}

	make_trimmed_arg_list(current_argument_tokens, &tmp_arg_tokens);
	struct_struct_c_lexer_token_ptr_list_ptr_list_add_end(parameter_lists, current_argument_tokens);
	struct_c_lexer_token_ptr_list_destroy(&tmp_arg_tokens);
}

void fully_expand_macros(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * arg_after, struct struct_c_lexer_token_ptr_list * arg_before, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map * disabled_tokens){
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map new_disabled_macros;
	struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map new_disabled_tokens;
	new_disabled_macros = unsigned_char_ptr_to_struct_macro_definition_ptr_map_copy(disabled_macros);
	new_disabled_tokens = struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_copy(disabled_tokens);
	struct_c_lexer_token_ptr_list_reverse(arg_before); /*  So we can pop next token off end */
	process_tokens(state, arg_after, arg_before, macro_map, &new_disabled_macros, disabled_tokens, MACRO_SEARCH, 0);
	unsigned_char_ptr_to_struct_macro_definition_ptr_map_destroy(&new_disabled_macros);
	struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_destroy(&new_disabled_tokens);
}

void perform_argument_prescan_all(struct preprocessor_state * state, struct struct_struct_c_lexer_token_ptr_list_ptr_list * after, struct struct_struct_c_lexer_token_ptr_list_ptr_list * before, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map * disabled_tokens){
	unsigned int i;
	unsigned int num_lists = struct_struct_c_lexer_token_ptr_list_ptr_list_size(before);
	for(i = 0; i < num_lists; i++){
		struct struct_c_lexer_token_ptr_list * arg_after = malloc(sizeof(struct struct_c_lexer_token_ptr_list));
		struct struct_c_lexer_token_ptr_list arg_before_copy = struct_c_lexer_token_ptr_list_copy(struct_struct_c_lexer_token_ptr_list_ptr_list_get(before, i));
		struct_c_lexer_token_ptr_list_create(arg_after);
		fully_expand_macros(state, arg_after, &arg_before_copy, macro_map, disabled_macros, disabled_tokens);
		struct_struct_c_lexer_token_ptr_list_ptr_list_add_end(after, arg_after);
		struct_c_lexer_token_ptr_list_destroy(&arg_before_copy);
	}
}

void add_stringified_character(struct unsigned_char_list * chars, unsigned int * num_chars, unsigned char c){
	*num_chars = *num_chars + 1;
	/* TODO: add more escaped characters for inside strings */
	if(c == '\\' || c == '"'){
		unsigned_char_list_add_end(chars, '\\');
		*num_chars = *num_chars + 1;
	}
	unsigned_char_list_add_end(chars, c);
}

void make_trimmed_arg_list(struct struct_c_lexer_token_ptr_list * trimmed_arg_list, struct struct_c_lexer_token_ptr_list * arg_list){
	unsigned int i;
	unsigned int end_index = 0;
	unsigned int observed_first_non_whitespace = 0;
	/*  Obtain the index of the last non-whitespace token */
	if(struct_c_lexer_token_ptr_list_size(arg_list)){
		for(i = struct_c_lexer_token_ptr_list_size(arg_list) -1; ;i--){
			struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_get(arg_list, i);
			if(!(tok->type == SPACE)){
				end_index = i;
				break;
			}
			if(i == 0){
				break;
			}
		}
	}
	for(i = 0; i <= end_index && i < struct_c_lexer_token_ptr_list_size(arg_list); i++){
		struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_get(arg_list, i);
		if(!(tok->type == SPACE)){
			observed_first_non_whitespace = 1;
		}
		if(observed_first_non_whitespace){
			struct_c_lexer_token_ptr_list_add_end(trimmed_arg_list, tok);
		}
	}
}

struct c_lexer_token * make_stringified_token(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * arg_list){
	/*  Return a STRING_LITERAL token that takes in the text of the tokens of the argument */
	struct c_lexer_token * token = malloc(sizeof(struct c_lexer_token));
	struct unsigned_char_list result_chars;
	unsigned int i;
	unsigned int num_chars = 2; /*  Min of 2 characters because of the quotes */
	unsigned_char_list_create(&result_chars);
	unsigned_char_list_add_end(&result_chars, '"');
	for(i = 0; i < struct_c_lexer_token_ptr_list_size(arg_list); i++){
		struct c_lexer_token * t = struct_c_lexer_token_ptr_list_get(arg_list, i);
		unsigned char * str = t->first_byte;
		do{
			add_stringified_character(&result_chars, &num_chars, *str);
		}while(str++ != t->last_byte);
	}
	unsigned_char_list_add_end(&result_chars, '"');
	token->type = STRING_LITERAL;
	token->first_byte = copy_string((unsigned char *)unsigned_char_list_data(&result_chars), ((unsigned char *)unsigned_char_list_data(&result_chars)) + (num_chars -1));
	token->last_byte = token->first_byte + (num_chars -1);
	unsigned_char_list_destroy(&result_chars);
	struct_c_lexer_token_ptr_list_add_end(&state->created_tokens, token);
	struct_c_lexer_token_ptr_list_destroy(arg_list);
	return token;
}

void evaluate_function_macro_body(struct preprocessor_state * state, struct macro_definition * macro_def, struct struct_c_lexer_token_ptr_list * result, struct struct_struct_c_lexer_token_ptr_list_ptr_list * after_prescan, struct struct_struct_c_lexer_token_ptr_list_ptr_list * before_prescan, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map * disabled_tokens){
	struct struct_c_lexer_token_ptr_list after_arg_substitution;
	struct struct_c_lexer_token_ptr_list definition_tokens = struct_c_lexer_token_ptr_list_copy(&macro_def->definition_tokens);
	struct struct_c_lexer_token_ptr_list working_tokens;
	unsigned int i;
	unsigned int num_definition_tokens = struct_c_lexer_token_ptr_list_size(&macro_def->definition_tokens);
	struct_c_lexer_token_ptr_list_create(&after_arg_substitution);
	struct_c_lexer_token_ptr_list_create(&working_tokens);
	struct_c_lexer_token_ptr_list_reverse(&definition_tokens);
	(void)disabled_macros;
	(void)disabled_tokens;
	(void)macro_map;
	for(i = 0; i < num_definition_tokens; i++){
		struct c_lexer_token * tok = read_until_next_token(state, &definition_tokens, &working_tokens);
		if(!tok){
			/*  Do nothing */
		}else if(tok->type == IDENTIFIER){
			unsigned char * identifier_str = copy_string(tok->first_byte, tok->last_byte);
			struct macro_parameter * potential_param = unsigned_char_ptr_to_struct_macro_parameter_ptr_map_exists(&macro_def->function_macro_parameters,identifier_str) ? unsigned_char_ptr_to_struct_macro_parameter_ptr_map_get(&macro_def->function_macro_parameters,identifier_str) : (struct macro_parameter *)0;
			unsigned int j;
			/* Add whitespace tokens */
			for(j = 0; j < struct_c_lexer_token_ptr_list_size(&working_tokens); j++){
				if(j != struct_c_lexer_token_ptr_list_size(&working_tokens) -1){ /*  Last one isn't whitespace */
					struct_c_lexer_token_ptr_list_add_end(result, struct_c_lexer_token_ptr_list_get(&working_tokens, j));
				}
			}
			struct_c_lexer_token_ptr_list_destroy(&working_tokens);
			struct_c_lexer_token_ptr_list_create(&working_tokens);
			/*  If the function body references a parameter of the function, replace it with the prescanned version of the arg */
			if(potential_param){
				if(potential_param->is_variadic){
					/*  For variadic arguments, take all of the rest of the tokens */
					unsigned int k;
					struct struct_c_lexer_token_ptr_list * arg_list;
					for(k = potential_param->position_index; k < struct_struct_c_lexer_token_ptr_list_ptr_list_size(after_prescan); k++){
						assert(struct_struct_c_lexer_token_ptr_list_ptr_list_size(after_prescan) > k);
						arg_list = struct_struct_c_lexer_token_ptr_list_ptr_list_get(after_prescan, k);
						struct_c_lexer_token_ptr_list_add_all_end(result, arg_list);
						if(k != (struct_struct_c_lexer_token_ptr_list_ptr_list_size(after_prescan) -1)){
							struct_c_lexer_token_ptr_list_add_end(result, state->comma_token);
						}
					}
				}else{
					struct struct_c_lexer_token_ptr_list * arg_list;
					assert(struct_struct_c_lexer_token_ptr_list_ptr_list_size(after_prescan) > potential_param->position_index);
					arg_list = struct_struct_c_lexer_token_ptr_list_ptr_list_get(after_prescan, potential_param->position_index);
					struct_c_lexer_token_ptr_list_add_all_end(result, arg_list);
				}
			}else{
				struct_c_lexer_token_ptr_list_add_end(result, tok);
			}
			free(identifier_str);
		}else if(tok->type == NUMBER_SIGN_CHAR){
			/* Was this the last token in the definition? */
			struct c_lexer_token * next = read_until_next_token(state, &definition_tokens, &working_tokens);
			if(!next){ /* # was last non whitespace token */
				struct_c_lexer_token_ptr_list_add_end(result, tok);
			}else if(next->type == IDENTIFIER){
				unsigned char * identifier_str = copy_string(next->first_byte, next->last_byte);
				struct macro_parameter * potential_param = unsigned_char_ptr_to_struct_macro_parameter_ptr_map_exists(&macro_def->function_macro_parameters,identifier_str) ? unsigned_char_ptr_to_struct_macro_parameter_ptr_map_get(&macro_def->function_macro_parameters,identifier_str) : (struct macro_parameter *)0;
				if(potential_param){
					struct c_lexer_token * stingified_token;
					struct struct_c_lexer_token_ptr_list * arg_list;
					assert(struct_struct_c_lexer_token_ptr_list_ptr_list_size(before_prescan) > potential_param->position_index);
					arg_list = struct_struct_c_lexer_token_ptr_list_ptr_list_get(before_prescan, potential_param->position_index);
					stingified_token = make_stringified_token(state, arg_list);
					struct_c_lexer_token_ptr_list_add_end(result, stingified_token);
				}else{
					assert(0 && "Stringify operator only operates on function macro parameters.");
				}
				free(identifier_str);
			}else{
				struct_c_lexer_token_ptr_list_add_all_end(result, &working_tokens);
				struct_c_lexer_token_ptr_list_add_end(result, next);
			}
		}else{
			unsigned int j;
			/* Add whitespace tokens */
			for(j = 0; j < struct_c_lexer_token_ptr_list_size(&working_tokens); j++){
				if(j != struct_c_lexer_token_ptr_list_size(&working_tokens) -1){ /*  Last one isn't whitespace */
					struct_c_lexer_token_ptr_list_add_end(result, struct_c_lexer_token_ptr_list_get(&working_tokens, j));
				}
			}
			struct_c_lexer_token_ptr_list_destroy(&working_tokens);
			struct_c_lexer_token_ptr_list_create(&working_tokens);
			struct_c_lexer_token_ptr_list_add_end(result, tok);
		}
	}
	/*  Perform another scan after macro arguments have been substituted */
	/*fully_expand_macros(result, &after_arg_substitution, macro_map, disabled_macros, disabled_tokens);*/
	struct_c_lexer_token_ptr_list_destroy(&after_arg_substitution);
	struct_c_lexer_token_ptr_list_destroy(&working_tokens);
	struct_c_lexer_token_ptr_list_destroy(&definition_tokens);
}

unsigned char * make_current_file_line_string(struct preprocessor_state * state){
	unsigned int current_line = get_current_file_line(state);
	sprintf_hook("%i", current_line);
	return copy_null_terminated_string(get_sprintf_buffer());
}

unsigned char * make_current_file_string(struct preprocessor_state * state){
	unsigned char * current_file = get_current_file(state);
	sprintf_hook("\"%s\"", current_file);
	return copy_null_terminated_string(get_sprintf_buffer());
}

void evaluate_special_macro(struct preprocessor_state * state, struct special_macro_definition * special_def, struct struct_c_lexer_token_ptr_list * after_expansion){
	switch(special_def->type){
		case __LINE__MACRO:{
			unsigned char * line = make_current_file_line_string(state);
			unsigned int len = (unsigned int)strlen((char *)line);
			struct c_lexer_token * tok = malloc(sizeof(struct c_lexer_token));
			tok->type = CONSTANT_DECIMAL;
			tok->first_byte = line;
			tok->last_byte = tok->first_byte + (len-1);
			struct_c_lexer_token_ptr_list_add_end(after_expansion, tok);
			struct_c_lexer_token_ptr_list_add_end(&state->created_tokens, tok);
			break;
		}case __FILE__MACRO:{
			unsigned char * file = make_current_file_string(state);
			unsigned int len = (unsigned int)strlen((char *)file);
			struct c_lexer_token * tok = malloc(sizeof(struct c_lexer_token));
			tok->type = STRING_LITERAL;
			tok->first_byte = file;
			tok->last_byte = tok->first_byte + (len-1);
			struct_c_lexer_token_ptr_list_add_end(after_expansion, tok);
			struct_c_lexer_token_ptr_list_add_end(&state->created_tokens, tok);
			break;
		}default:{
			assert(0 && "Unknown special macro.");
		}
	}
}

unsigned int process_identifier_if_macro(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * output_tokens, struct struct_c_lexer_token_ptr_list * input_tokens, struct struct_c_lexer_token_ptr_list * working_tokens, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map * disabled_tokens){
	/*  Does nothing if identifer is not a macro */
	/*  Expands macro and puts token back if it is a macro  */
	/*  Pop the last token we just inspected */
	struct c_lexer_token * identifier_token = struct_c_lexer_token_ptr_list_pop_end(working_tokens);
	struct c_lexer_token * disabled_token = struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_exists(disabled_tokens, identifier_token) ? struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_get(disabled_tokens, identifier_token) : (struct c_lexer_token *)0;
	unsigned char * identifier_str = copy_string(identifier_token->first_byte, identifier_token->last_byte);
	struct macro_definition * macro_def = unsigned_char_ptr_to_struct_macro_definition_ptr_map_exists(macro_map, identifier_str) ? unsigned_char_ptr_to_struct_macro_definition_ptr_map_get(macro_map, identifier_str) : (struct macro_definition *)0;
	struct macro_definition * disabled_def = unsigned_char_ptr_to_struct_macro_definition_ptr_map_exists(disabled_macros, identifier_str) ? unsigned_char_ptr_to_struct_macro_definition_ptr_map_get(disabled_macros, identifier_str) : (struct macro_definition *)0;
	struct special_macro_definition * special_def = unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_exists(&state->special_macros, identifier_str) ? unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_get(&state->special_macros, identifier_str) : (struct special_macro_definition *)0;
	unsigned int rtn;
	(void)output_tokens;
	(void)disabled_token;
	if(disabled_token){
		/*  Disabled, put it back */
		struct_c_lexer_token_ptr_list_add_end(working_tokens, identifier_token);
		rtn = 0;
	}else if(special_def){
		/*  Special macro definition */
		struct struct_c_lexer_token_ptr_list after_expansion;
		struct_c_lexer_token_ptr_list_create(&after_expansion);
		evaluate_special_macro(state, special_def, &after_expansion);
		if(struct_c_lexer_token_ptr_list_size(&after_expansion)){
			unsigned int i;
			for(i = struct_c_lexer_token_ptr_list_size(&after_expansion) -1; ; i--){
				struct_c_lexer_token_ptr_list_add_end(input_tokens, struct_c_lexer_token_ptr_list_get(&after_expansion, i));
				if(i == 0){
					break;
				}
			}
		}
		struct_c_lexer_token_ptr_list_destroy(&after_expansion);
		rtn = 0;
	}else if(macro_def && disabled_def == macro_def){
		struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_put(disabled_tokens, identifier_token, identifier_token);
		/*  Disabled, put it back */
		struct_c_lexer_token_ptr_list_add_end(working_tokens, identifier_token);
		rtn = 0;
	}else if(macro_def){
		unsigned int num_definition_tokens = struct_c_lexer_token_ptr_list_size(&macro_def->definition_tokens);
		unsigned int i;
		if(macro_def->type == OBJECT_MACRO){
			struct struct_c_lexer_token_ptr_list initial_replacement;
			struct struct_c_lexer_token_ptr_list after_expansion;
			struct_c_lexer_token_ptr_list_create(&initial_replacement);
			struct_c_lexer_token_ptr_list_create(&after_expansion);
			for(i = 0; i < num_definition_tokens; i++){
				struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_get(&macro_def->definition_tokens, i);
				struct_c_lexer_token_ptr_list_add_end(&initial_replacement, tok);
			}
			unsigned_char_ptr_to_struct_macro_definition_ptr_map_put(disabled_macros, identifier_str, macro_def);
			fully_expand_macros(state, &after_expansion, &initial_replacement, macro_map, disabled_macros, disabled_tokens);
			unsigned_char_ptr_to_struct_macro_definition_ptr_map_remove(disabled_macros, identifier_str);

			if(struct_c_lexer_token_ptr_list_size(&after_expansion)){
				for(i = struct_c_lexer_token_ptr_list_size(&after_expansion) -1; ; i--){
					struct_c_lexer_token_ptr_list_add_end(input_tokens, struct_c_lexer_token_ptr_list_get(&after_expansion, i));
					if(i == 0){
						break;
					}
				}
			}

			struct_c_lexer_token_ptr_list_destroy(&initial_replacement);
			struct_c_lexer_token_ptr_list_destroy(&after_expansion);
		}else if(macro_def->type == FUNCTION_MACRO){
			struct struct_c_lexer_token_ptr_list tmp_before_open_paren;
			struct struct_c_lexer_token_ptr_list function_macro_result;
			struct struct_struct_c_lexer_token_ptr_list_ptr_list argument_token_collections;
			struct struct_struct_c_lexer_token_ptr_list_ptr_list prescanned_argument_token_collections;
			struct struct_c_lexer_token_ptr_list initial_pass_tokens;
			struct c_lexer_token * open_paren;
			unsigned int k;
			struct_c_lexer_token_ptr_list_create(&tmp_before_open_paren);
			/*  We expect the next non-whitespace character to be a ( */
			open_paren = read_until_next_token(state, input_tokens, &tmp_before_open_paren);
			if(!open_paren || open_paren->type != OPEN_PAREN_CHAR){
				/*  This not an invocation of the function, just a reference to the name */
				/*  whitespace and non paren tokens we just poped off  */
				while(struct_c_lexer_token_ptr_list_size(&tmp_before_open_paren)){
					struct_c_lexer_token_ptr_list_add_end(input_tokens, struct_c_lexer_token_ptr_list_pop_end(&tmp_before_open_paren));
				}
				free(identifier_str);
				struct_c_lexer_token_ptr_list_destroy(&tmp_before_open_paren);
				/*  Put back identifier */
				struct_c_lexer_token_ptr_list_add_end(working_tokens, identifier_token);
				return 0;
			}

			struct_struct_c_lexer_token_ptr_list_ptr_list_create(&argument_token_collections);
			struct_struct_c_lexer_token_ptr_list_ptr_list_create(&prescanned_argument_token_collections);
			struct_c_lexer_token_ptr_list_create(&initial_pass_tokens);
			struct_c_lexer_token_ptr_list_create(&function_macro_result);
			/*  Process tokens to evaluate any macro directives embedded inside the function macro invocation */
			process_tokens(state, &initial_pass_tokens, input_tokens, macro_map, disabled_macros, disabled_tokens, MACRO_SEARCH, 1);
			split_tokens_into_parameters(&initial_pass_tokens, &argument_token_collections);

			perform_argument_prescan_all(state, &prescanned_argument_token_collections, &argument_token_collections, macro_map, disabled_macros, disabled_tokens);
			unsigned_char_ptr_to_struct_macro_definition_ptr_map_put(disabled_macros, identifier_str, macro_def);
			evaluate_function_macro_body(state, macro_def, &function_macro_result, &prescanned_argument_token_collections, &argument_token_collections, macro_map, disabled_macros, disabled_tokens);

			unsigned_char_ptr_to_struct_macro_definition_ptr_map_remove(disabled_macros, identifier_str);

			/*  Clean up argument tokens */
			for(k = 0; k < struct_struct_c_lexer_token_ptr_list_ptr_list_size(&argument_token_collections); k++){
				struct struct_c_lexer_token_ptr_list * l = struct_struct_c_lexer_token_ptr_list_ptr_list_get(&argument_token_collections, k);
				struct_c_lexer_token_ptr_list_destroy(l);
				free(l);
			}
			/*  Clean up prescanned argument tokens */
			for(k = 0; k < struct_struct_c_lexer_token_ptr_list_ptr_list_size(&prescanned_argument_token_collections); k++){
				struct struct_c_lexer_token_ptr_list * l = struct_struct_c_lexer_token_ptr_list_ptr_list_get(&prescanned_argument_token_collections, k);
				struct_c_lexer_token_ptr_list_destroy(l);
				free(l);
			}

			/*  Add the result to our input */
			if(struct_c_lexer_token_ptr_list_size(&function_macro_result)){
				for(k = struct_c_lexer_token_ptr_list_size(&function_macro_result) -1; ; k--){
					struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_get(&function_macro_result, k);
					struct_c_lexer_token_ptr_list_add_end(input_tokens, tok);
					if(k == 0){
						break;
					}
				}
			}

			struct_struct_c_lexer_token_ptr_list_ptr_list_destroy(&argument_token_collections);
			struct_struct_c_lexer_token_ptr_list_ptr_list_destroy(&prescanned_argument_token_collections);
			struct_c_lexer_token_ptr_list_destroy(&initial_pass_tokens);
			struct_c_lexer_token_ptr_list_destroy(&function_macro_result);
			struct_c_lexer_token_ptr_list_destroy(&tmp_before_open_paren);
		}else{
			assert(0 && "Unknown macro type");
		}
		rtn = 1;
	}else{
		/*  Was not a defined macro, put it back */
		struct_c_lexer_token_ptr_list_add_end(working_tokens, identifier_token);
		rtn = 0;
	}
	free(identifier_str);
	return rtn;
}

void process_tokens(struct preprocessor_state * state, struct struct_c_lexer_token_ptr_list * output_tokens, struct struct_c_lexer_token_ptr_list * input_tokens, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map * disabled_tokens, enum search_state starting_state, unsigned int inside_function_macro_invocation){
	struct struct_c_lexer_token_ptr_list working_tokens;
	enum search_state current_state = starting_state;
	struct c_lexer_token * current_token;
	unsigned int function_macro_paren_level = 1; /*  If we're inside a function macro invocation */
	struct_c_lexer_token_ptr_list_create(&working_tokens);
	do {
		/*  Returns 0 when all tokens have been read */
		current_token = read_until_next_token(state, input_tokens, &working_tokens);
		/* current_token is the last token in 'working_tokens' */
		switch(current_state){
			case BEGIN_LINE:{
				if(current_token){
					switch(current_token->type){
						case NUMBER_SIGN_CHAR:{
							current_state = EXPECT_DIRECTIVE;
							break;
						}case NEWLINE:{
							release_working_tokens(state, output_tokens, &working_tokens);
							current_state = BEGIN_LINE;
							increment_current_line(state);
							break;
						}case IDENTIFIER:{
							if(!inside_function_macro_invocation){
								process_identifier_if_macro(state, output_tokens, input_tokens, &working_tokens, macro_map, disabled_macros, disabled_tokens);
							}
							current_state = MACRO_SEARCH;
							release_working_tokens(state, output_tokens, &working_tokens);
							break;
						}case OPEN_PAREN_CHAR:{
							function_macro_paren_level++;
							release_working_tokens(state, output_tokens, &working_tokens);
							current_state = MACRO_SEARCH;
							break;
						}case CLOSE_PAREN_CHAR:{
							assert(function_macro_paren_level);
							function_macro_paren_level--;
							/*  Was this the closing paren of a function macro call? */
							if(function_macro_paren_level == 0 && inside_function_macro_invocation){
								struct_c_lexer_token_ptr_list_destroy(&working_tokens);
								return;
							}else{
								release_working_tokens(state, output_tokens, &working_tokens);
								current_state = MACRO_SEARCH;
							}
							break;
						}default:{
							current_state = MACRO_SEARCH;
							break;
						}
					}
				}else{
					release_working_tokens(state, output_tokens, &working_tokens);
				}
				break;
			}case MACRO_SEARCH:{
				if(current_token){
					switch(current_token->type){
						case IDENTIFIER:{
							if(!inside_function_macro_invocation){
								process_identifier_if_macro(state, output_tokens, input_tokens, &working_tokens, macro_map, disabled_macros, disabled_tokens);
							}
							release_working_tokens(state, output_tokens, &working_tokens);
							current_state = MACRO_SEARCH;
							break;
						}case NEWLINE:{
							release_working_tokens(state, output_tokens, &working_tokens);
							current_state = BEGIN_LINE;
							increment_current_line(state);
							break;
						}case OPEN_PAREN_CHAR:{
							function_macro_paren_level++;
							release_working_tokens(state, output_tokens, &working_tokens);
							break;
						}case CLOSE_PAREN_CHAR:{
							assert(function_macro_paren_level);
							function_macro_paren_level--;
							/*  Was this the closing paren of a function macro call? */
							if(function_macro_paren_level == 0 && inside_function_macro_invocation){
								struct_c_lexer_token_ptr_list_destroy(&working_tokens);
								return;
							}else{
								release_working_tokens(state, output_tokens, &working_tokens);
							}
							break;
						}default:{
							release_working_tokens(state, output_tokens, &working_tokens);
							break;
						}
					}
				}else{
					release_working_tokens(state, output_tokens, &working_tokens);
				}
				break;
			}case EXPECT_DIRECTIVE:{
				if(current_token){
					switch(current_token->type){
						case IDENTIFIER:{
							current_state = COMPLETE_DIRECTIVE;
							break;
						}default:{
							assert(0 && "Not a valid preprocessor directive.");
							break;
						}
					}
				}else{
					release_working_tokens(state, output_tokens, &working_tokens);
				}
				break;
			}case COMPLETE_DIRECTIVE:{
				if(current_token){
					switch(current_token->type){
						case NEWLINE:{
							process_directive(state, output_tokens, &working_tokens, macro_map, disabled_macros, disabled_tokens);
							current_state = BEGIN_LINE;
							/*  Newline count is incremented somewhere in process directive */
							break;
						}default:{
							/*  Keep taking in tokens to get the entire directive */
							break;
						}
					}
				}else{
					process_directive(state, output_tokens, &working_tokens, macro_map, disabled_macros, disabled_tokens);
				}
				break;
			}default:{
				assert(0 && "Unexpected case.");
				break;
			}
		}
	}while(current_token);
	assert(struct_c_lexer_token_ptr_list_size(&working_tokens) == 0);
	struct_c_lexer_token_ptr_list_destroy(&working_tokens);
	/*  If we're not inside a potential macro invocation, define, include etc. and we encounter a newline, then we can consider the current set of working tokens processed */
}

void process_line_continuators(struct unsigned_char_list * in_characters, struct unsigned_char_list * out_characters){
	/*  Process line contination marks where '\' followed by '\n' is replaced with nothing. */
	unsigned int i;
	unsigned int in_size = unsigned_char_list_size(in_characters);
	unsigned char * in_data = unsigned_char_list_data(in_characters);
	if(in_size > 0){
		for(i = 1; i < in_size; i++){
			if(in_data[i -1] == '\\' && in_data[i] == '\n'){
				/*  Skip '\' and loop increment skips newline */
				i++;
				continue;
			}else{
				unsigned_char_list_add_end(out_characters, in_data[i-1]);
			}
		}
		/*  Add the last character only if was not part of a line continuation mark */
		if(i == in_size -1){
			unsigned_char_list_add_end(out_characters, in_data[in_size -1]);
		}
	}
}

int c_lexer_token_cmp(struct c_lexer_token * a, struct c_lexer_token * b){
	return (int)(a - b);
}

struct unsigned_char_list * add_tokenizable_input_buffer(struct preprocessor_state * state){
	struct unsigned_char_list * new_list = malloc(sizeof(struct unsigned_char_list));
	unsigned_char_list_create(new_list);
	struct_unsigned_char_list_ptr_list_add_end(&state->tokenizable_input_buffers, new_list);
	return new_list;
}

struct c_lexer_state * add_c_lexer_state(struct preprocessor_state * state){
	struct c_lexer_state * new_lexer = malloc(sizeof(struct c_lexer_state));
	struct_c_lexer_state_ptr_list_add_end(&state->c_lexer_states, new_lexer);
	return new_lexer;
}

int get_preprocessed_output_from_file(struct preprocessor_state * state, unsigned char * in_file, struct struct_c_lexer_token_ptr_list * output_tokens, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * macro_map, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * disabled_macros, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map  * disabled_tokens){
	struct unsigned_char_list fname;
	struct unsigned_char_list boo;
	struct unsigned_char_list file_input;
	struct unsigned_char_list lexer_output;
	struct unsigned_char_list * tokenizable_input = add_tokenizable_input_buffer(state); /*  Released when preprocessor is destroyed */
	struct c_lexer_state * c_lexer_state = add_c_lexer_state(state);
	struct struct_c_lexer_token_ptr_list input_tokens;
	int rtn = 0;
	unsigned int i;
	unsigned_char_list_create(&fname);
	unsigned_char_list_create(&boo);
	unsigned_char_list_create(&file_input);
	unsigned_char_list_create(&lexer_output);
	c_lexer_state->c.buffered_output = &lexer_output;
	c_lexer_state->c.memory_pooler_collection = state->memory_pooler_collection; 
	/*  Read the file we want to preprocess */
	add_file_to_buffer(&file_input, (char*)in_file);
	process_line_continuators(&file_input, tokenizable_input);

	/*  Using the c lexer for the unpreprocessed input allow us to preprocess the input easier */
	rtn = lex_c(c_lexer_state, in_file, unsigned_char_list_data(tokenizable_input), unsigned_char_list_size(tokenizable_input));
	/*  Reverse the list of tokens so we can use the list's push and pop with good memory performance */
	input_tokens = struct_c_lexer_token_ptr_list_copy(&c_lexer_state->tokens);
	struct_c_lexer_token_ptr_list_reverse(&input_tokens);
	process_tokens(state, output_tokens, &input_tokens, macro_map, disabled_macros, disabled_tokens, BEGIN_LINE, 0);

	{
		unsigned int j;
		unsigned char * k = in_file;
		unsigned int num_output_tokens = struct_c_lexer_token_ptr_list_size(output_tokens);
		struct c_lexer_token ** all_output_tokens = (struct c_lexer_token **)struct_c_lexer_token_ptr_list_data(output_tokens);
		while(*k){unsigned_char_list_add_end(&fname, *k); k++;}
		unsigned_char_list_add_end(&fname, 'x');
		unsigned_char_list_add_end(&fname, 0);
		for(j = 0; j < num_output_tokens; j++){
			struct c_lexer_token * current_token = all_output_tokens[j];
			unsigned char * str = current_token->first_byte;
			do{
				unsigned_char_list_add_end(&boo, *str);
			}while(str++ != current_token->last_byte);
		}
	}

	/*  Typically, there is only output if there is an error.  There could also be debug info output */
	for(i = 0; i < unsigned_char_list_size(&lexer_output); i++){
		printf("%c", unsigned_char_list_get(&lexer_output, i));
	}

	unsigned_char_list_destroy(&fname);
	unsigned_char_list_destroy(&boo);
	unsigned_char_list_destroy(&lexer_output);
	unsigned_char_list_destroy(&file_input);
	struct_c_lexer_token_ptr_list_destroy(&input_tokens);
	return rtn;
}

void free_special_macro_definition_map(struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_map * map){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_keys(map);
	unsigned int size = unsigned_char_ptr_list_size(&keys);
	unsigned int i;
	for(i = 0; i < size; i++){
		struct special_macro_definition * macro_def = unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_get(map, unsigned_char_ptr_list_get(&keys, i));
		free(macro_def);
	}
	/*  Need to do this twice because we're deleting the data under the keys of the map */
	for(i = 0; i < size; i++){
		free(unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_destroy(map);
	unsigned_char_ptr_list_destroy(&keys);
}

void free_macro_definition_map(struct unsigned_char_ptr_to_struct_macro_definition_ptr_map * map){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_macro_definition_ptr_map_keys(map);
	unsigned int size = unsigned_char_ptr_list_size(&keys);
	unsigned int i;
	for(i = 0; i < size; i++){
		struct macro_definition * macro_def = unsigned_char_ptr_to_struct_macro_definition_ptr_map_get(map, unsigned_char_ptr_list_get(&keys, i));
		unsigned int j;
		struct unsigned_char_ptr_list param_keys = unsigned_char_ptr_to_struct_macro_parameter_ptr_map_keys(&macro_def->function_macro_parameters);
		unsigned int param_keys_size = unsigned_char_ptr_list_size(&param_keys);

		for(j = 0; j < param_keys_size; j++){
			free(unsigned_char_ptr_to_struct_macro_parameter_ptr_map_get(&macro_def->function_macro_parameters, unsigned_char_ptr_list_get(&param_keys, j)));
		}

		for(j = 0; j < param_keys_size; j++){
			free(unsigned_char_ptr_list_get(&param_keys, j));
		}
		unsigned_char_ptr_to_struct_macro_parameter_ptr_map_destroy(&macro_def->function_macro_parameters);

		struct_c_lexer_token_ptr_list_destroy(&macro_def->definition_tokens);
		free(macro_def);
		unsigned_char_ptr_list_destroy(&param_keys);
	}
	/*  Need to do this twice because we're deleting the data under the keys of the map */
	for(i = 0; i < size; i++){
		free(unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_to_struct_macro_definition_ptr_map_destroy(map);
	unsigned_char_ptr_list_destroy(&keys);
}

void destroy_preprocessor_state(struct preprocessor_state * state){
	unsigned int i;
	struct_if_branch_ptr_list_destroy(&state->if_branches);

	for(i = 0; i < struct_unsigned_char_list_ptr_list_size(&state->tokenizable_input_buffers); i++){
		struct unsigned_char_list * l = struct_unsigned_char_list_ptr_list_get(&state->tokenizable_input_buffers, i);
		unsigned_char_list_destroy(l);
		free(l);
	}
	struct_unsigned_char_list_ptr_list_destroy(&state->tokenizable_input_buffers);
	while(struct_c_lexer_state_ptr_list_size(&state->c_lexer_states)){
		struct c_lexer_state * s = struct_c_lexer_state_ptr_list_pop_end(&state->c_lexer_states);
		destroy_c_lexer_state(s);
		free(s);
	}
	struct_c_lexer_state_ptr_list_destroy(&state->c_lexer_states);
	free(state->comma_token);

	for(i = 0; i < struct_c_lexer_token_ptr_list_size(&state->created_tokens); i++){
		struct c_lexer_token * t = struct_c_lexer_token_ptr_list_get(&state->created_tokens, i);
		free(t->first_byte);
		free(t);
	}
	struct_c_lexer_token_ptr_list_destroy(&state->created_tokens);
	struct_preprocessor_file_context_ptr_list_destroy(&state->file_contexts);

	free_special_macro_definition_map(&state->special_macros);
	free(state);
	g_format_buffer_release();
}


void print_file_stack(struct preprocessor_state * state){
	unsigned int i;
	for(i = 0; i < struct_preprocessor_file_context_ptr_list_size(&state->file_contexts); i++){
		struct preprocessor_file_context * context = struct_preprocessor_file_context_ptr_list_get(&state->file_contexts, i);
		if(i != struct_preprocessor_file_context_ptr_list_size(&state->file_contexts) -1){
			printf("%s line %d -> ", context->filename, context->current_line);
		}else{
			printf("%s\n", context->filename);
		}
	}
}

unsigned int get_current_file_line(struct preprocessor_state * state){
	assert(struct_preprocessor_file_context_ptr_list_size(&state->file_contexts));
	return struct_preprocessor_file_context_ptr_list_get(&state->file_contexts, struct_preprocessor_file_context_ptr_list_size(&state->file_contexts) -1)->current_line;
}

void increment_current_line(struct preprocessor_state * state){
	struct preprocessor_file_context * con;
	assert(struct_preprocessor_file_context_ptr_list_size(&state->file_contexts));
	con = struct_preprocessor_file_context_ptr_list_get(&state->file_contexts, struct_preprocessor_file_context_ptr_list_size(&state->file_contexts) -1);
	con->current_line = con->current_line + 1;
}

unsigned char * get_current_file(struct preprocessor_state * state){
	assert(struct_preprocessor_file_context_ptr_list_size(&state->file_contexts));
	return struct_preprocessor_file_context_ptr_list_get(&state->file_contexts, struct_preprocessor_file_context_ptr_list_size(&state->file_contexts) -1)->filename;
}

void add_special_macros(struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_map * map){
	const char * line_macro = "__LINE__";
	const char * file_macro = "__FILE__";
	struct special_macro_definition * line_macro_definition = malloc(sizeof(struct special_macro_definition));
	struct special_macro_definition * file_macro_definition = malloc(sizeof(struct special_macro_definition));
	line_macro_definition->type = __LINE__MACRO;
	file_macro_definition->type = __FILE__MACRO;
	unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_put(map, copy_null_terminated_string((unsigned char *)line_macro), line_macro_definition);
	unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_put(map, copy_null_terminated_string((unsigned char *)file_macro), file_macro_definition);
}

struct preprocessor_state * create_preprocessor_state(struct memory_pooler_collection * memory_pooler_collection){
	struct preprocessor_state * state = malloc(sizeof(struct preprocessor_state));
	g_format_buffer_use();
	unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_create(&state->special_macros);
	add_special_macros(&state->special_macros);
	state->comma_token = malloc(sizeof(struct c_lexer_token));
	state->comma_token->type = COMMA_CHAR;
	state->comma_token->first_byte = (unsigned char *)comma;
	state->comma_token->last_byte = (unsigned char *)comma;
	state->memory_pooler_collection = memory_pooler_collection;
	struct_if_branch_ptr_list_create(&state->if_branches);
	struct_unsigned_char_list_ptr_list_create(&state->tokenizable_input_buffers);
	struct_preprocessor_file_context_ptr_list_create(&state->file_contexts);
	struct_c_lexer_state_ptr_list_create(&state->c_lexer_states);
	struct_c_lexer_token_ptr_list_create(&state->created_tokens);
	return state;
}

void destroy_preprocessor_file_context(struct preprocessor_file_context * file_context){
	free(file_context->filename);
	free(file_context);
}

struct preprocessor_file_context * make_preprocessor_file_context(unsigned char * filename){
	struct preprocessor_file_context * file_context = malloc(sizeof(struct preprocessor_file_context));
	file_context->filename = filename;
	file_context->current_line = 1;
	return file_context;
}

int do_preprocess(struct memory_pooler_collection * memory_pooler_collection, unsigned char * in_file, unsigned char * out_file){
	int rtn; 
	struct preprocessor_state * preprocessor_state = create_preprocessor_state(memory_pooler_collection);
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map macro_map;
	struct struct_c_lexer_token_ptr_list output_tokens;
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map disabled_macros;
	struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map disabled_tokens;
	struct_c_lexer_token_ptr_list_create(&output_tokens);
	unsigned_char_ptr_to_struct_macro_definition_ptr_map_create(&disabled_macros);
	struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_create(&disabled_tokens);

	/*  Push the first file we're traversing down into */
	struct_preprocessor_file_context_ptr_list_add_end(&preprocessor_state->file_contexts, make_preprocessor_file_context(copy_string(in_file, get_null_terminator(in_file))));

	printf("Begin preprocessing ");
	print_file_stack(preprocessor_state);
	unsigned_char_ptr_to_struct_macro_definition_ptr_map_create(&macro_map);
	if(!(rtn = get_preprocessed_output_from_file(preprocessor_state, in_file, &output_tokens, &macro_map, &disabled_macros, &disabled_tokens))){
		unsigned int i;
		struct unsigned_char_list file_output;
		unsigned_char_list_create(&file_output);
		for(i = 0; i < struct_c_lexer_token_ptr_list_size(&output_tokens); i++){
			struct c_lexer_token * current_token = struct_c_lexer_token_ptr_list_get(&output_tokens, i);
			unsigned char * j;
			for(j = current_token->first_byte; j < (current_token->last_byte + 1); j++){
				unsigned_char_list_add_end(&file_output, *j);
			}
		}
		output_buffer_to_file(&file_output, (char*)out_file);
		unsigned_char_list_destroy(&file_output);
	}else{
		printf("Nothing to output.  Preprocessing failed for %s\n", in_file);
	}
	destroy_preprocessor_file_context(struct_preprocessor_file_context_ptr_list_pop_end(&preprocessor_state->file_contexts));

	free_macro_definition_map(&macro_map);
	struct_c_lexer_token_ptr_list_destroy(&output_tokens);
	unsigned_char_ptr_to_struct_macro_definition_ptr_map_destroy(&disabled_macros);
	struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_destroy(&disabled_tokens);
	destroy_preprocessor_state(preprocessor_state);
	return rtn;
}
