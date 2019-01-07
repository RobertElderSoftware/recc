/*
    Copyright 2019 Robert Elder Software Inc.
    
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
#include "parser.h"

static const char * node_type_names[93] = {
	"TERMINAL",
	"EPSILON",
	"PRIMARY_EXPRESSION",
	"POSTFIX_EXPRESSION",
	"POSTFIX_EXPRESSION_REST",
	"UNARY_EXPRESSION",
	"UNARY_OPERATOR",
	"CAST_EXPRESSION",
	"MULTIPLICATIVE_EXPRESSION",
	"MULTIPLICATIVE_EXPRESSION_REST",
	"ADDITIVE_EXPRESSION",
	"ADDITIVE_EXPRESSION_REST",
	"SHIFT_EXPRESSION",
	"SHIFT_EXPRESSION_REST",
	"RELATIONAL_EXPRESSION",
	"RELATIONAL_EXPRESSION_REST",
	"EQUALITY_EXPRESSION",
	"EQUALITY_EXPRESSION_REST",
	"AND_EXPRESSION",
	"AND_EXPRESSION_REST",
	"EXCLUSIVE_OR_EXPRESSION",
	"EXCLUSIVE_OR_EXPRESSION_REST",
	"INCLUSIVE_OR_EXPRESSION",
	"INCLUSIVE_OR_EXPRESSION_REST",
	"LOGICAL_AND_EXPRESSION",
	"LOGICAL_AND_EXPRESSION_REST",
	"LOGICAL_OR_EXPRESSION",
	"LOGICAL_OR_EXPRESSION_REST",
	"CONDITIONAL_EXPRESSION",
	"ASSIGNMENT_EXPRESSION",
	"ASSIGNMENT_OPERATOR",
	"EXPRESSION_REST",
	"EXPRESSION",
	"STRING_LITERAL_LIST_REST",
	"STRING_LITERAL_LIST",
	"CONSTANT_EXPRESSION",
	"EXPRESSION_STATEMENT",
	"LABELED_STATEMENT",
	"SELECTION_STATEMENT",
	"ITERATION_STATEMENT",
	"JUMP_STATEMENT",
	"STATEMENT",
	"STATEMENT_LIST",
	"STATEMENT_LIST_REST",
	"COMPOUND_STATEMENT",
	"TYPE_QUALIFIER",
	"TYPE_QUALIFIER_LIST",
	"TYPE_QUALIFIER_LIST_REST",
	"STORAGE_CLASS_SPECIFIER",
	"TYPE_SPECIFIER",
	"ENUM_SPECIFIER",
	"DECLARATION_SPECIFIERS",
	"DECLARATION",
	"DECLARATION_LIST",
	"DECLARATION_LIST_REST",
	"DIRECT_DECLARATOR_REST",
	"DIRECT_DECLARATOR",
	"DIRECT_ABSTRACT_DECLARATOR",
	"DIRECT_ABSTRACT_DECLARATOR_REST",
	"TYPE_NAME",
	"ABSTRACT_DECLARATOR",
	"DECLARATOR",
	"INIT_DECLARATOR",
	"INIT_DECLARATOR_LIST",
	"INIT_DECLARATOR_LIST_REST",
	"INITIALIZER",
	"INITIALIZER_LIST",
	"INITIALIZER_LIST_REST",
	"IDENTIFIER_LIST",
	"IDENTIFIER_LIST_REST",
	"POINTER",
	"STRUCT_OR_UNION",
	"STRUCT_DECLARATOR",
	"STRUCT_DECLARATOR_LIST",
	"STRUCT_DECLARATOR_LIST_REST",
	"STRUCT_DECLARATION",
	"STRUCT_DECLARATION_LIST",
	"STRUCT_DECLARATION_LIST_REST",
	"STRUCT_OR_UNION_SPECIFIER",
	"SPECIFIER_QUALIFIER_LIST",
	"PARAMETER_DECLARATION",
	"ARGUMENT_EXPRESSION_LIST",
	"ARGUMENT_EXPRESSION_LIST_REST",
	"PARAMETER_LIST",
	"PARAMETER_LIST_REST",
	"PARAMETER_TYPE_LIST",
	"ENUMERATOR_LIST",
	"ENUMERATOR_LIST_REST",
	"ENUMERATOR",
	"TRANSLATION_UNIT",
	"TRANSLATION_UNIT_REST",
	"EXTERNAL_DECLARATION",
	"FUNCTION_DEFINITION"
};

struct parser_node * create_parser_node(struct parser_state *, struct parser_node *, struct parser_node *, struct c_lexer_token *, enum node_type);
void parser_progress(const char*, ...);
void print_scope_level(struct parser_state *, struct scope_level *, unsigned int);
void destroy_scope_level(struct scope_level *, unsigned int);
void add_terminal_tokens_to_buffer_h2(struct unsigned_char_list *, struct parser_node *, unsigned int *);
void add_terminal_tokens_to_buffer_h1(struct unsigned_char_list *, struct parser_node *, unsigned int *);
void add_terminal_tokens_to_buffer(struct unsigned_char_list *, struct parser_node *);

void print_generic_declaration(struct memory_pool_collection *, struct unsigned_char_list *, struct parser_node * n, unsigned int);
void print_parser_node_tree(struct unsigned_char_list *, struct parser_node *, struct parser_node *, unsigned int);
struct parser_node * create_abstract_declarator_from_declarator(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_declarator_to_abstract_declarator(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_declarator_to_abstract_declarator_h(struct memory_pool_collection *, struct parser_node *);
unsigned int convert_character_constant(unsigned char *, unsigned char *, unsigned int *, unsigned int *);
void convert_string_literal(unsigned char *, unsigned char *, struct unsigned_char_list *);
unsigned int is_function_k_and_r_c_style(struct parser_node *);
unsigned int is_parameter_type_list_variadic(struct parser_node *);
struct parser_node * add_level_to_pointer(struct memory_pool_collection *, struct parser_node * n);
struct parser_node * create_pointer_node(struct memory_pool_collection *);
struct parser_node * convert_abstract_declarator_to_address_type_h(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_abstract_declarator_to_address_type(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_abstract_declarator_to_pointer_type(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_abstract_declarator_to_array_type(struct memory_pool_collection *, struct parser_node *);
struct parser_node * remove_one_pointer_level(struct memory_pool_collection *, struct parser_node *);
struct c_lexer_token * get_identifier_token_from_declarator(struct parser_node *);
unsigned int convert_integer(unsigned char *, unsigned int, unsigned int);
unsigned int convert_decimal_constant(unsigned char *);
unsigned int convert_hexadecimal_constant(unsigned char *);
unsigned int convert_octal_constant(unsigned char *);
struct scope_level * get_last_function_namespace_scope(struct parser_state *);
unsigned int is_const(struct type_description * t);
struct type_description * ensure_signed(struct memory_pool_collection *, struct type_description * t);
struct type_description * ensure_unsigned(struct memory_pool_collection *, struct type_description * t);
void validate_specifier_token_type(enum c_token_type);
enum node_type get_specifier_parser_node_type(enum c_token_type);
unsigned int get_token_index(struct c_lexer_state *, struct c_lexer_token *, unsigned int *);
unsigned int get_token_line_number(struct c_lexer_state *, struct c_lexer_token *);
unsigned char * get_specifier_string(enum c_token_type);
void pretty_print_context(struct c_lexer_state *, struct c_lexer_token *, struct c_lexer_token *, struct c_lexer_token *, struct c_lexer_token *);
struct parser_node * get_last_terminal_node(struct parser_node *);
struct parser_node * insert_abstract_declarator(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
struct parser_node * add_outer_direct_abstract_declarator_to_inner_as_rest(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
void add_pointer_to_end(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
void destroy_constant_description(struct memory_pool_collection *, struct constant_description *);
struct parser_checkpoint create_parser_checkpoint(struct parser_state *);
void backtrack_parser(struct parser_state *, unsigned int);

void print_number_rep(unsigned int *);
unsigned int get_hex_digit_value(unsigned char);
unsigned int add_with_carry(struct parser_state *, unsigned int *, unsigned int, unsigned int);
unsigned int shift_and_add_in(struct parser_state *, unsigned int *, unsigned int, unsigned int);
unsigned int update_base10_rep(struct parser_state *, unsigned int *, unsigned int *, unsigned int);
unsigned int add_digit_value_to_parts(struct parser_state *, unsigned int *, unsigned int *, unsigned int *, unsigned int, unsigned int, unsigned int);
unsigned int token_radix_parse(struct parser_state *, struct c_lexer_token *, unsigned int *, unsigned int, unsigned int, unsigned int);
struct general_type_id get_general_type_id_for_integer_constant(struct parser_state *, unsigned int *, unsigned int, unsigned int, unsigned int);
struct data_package_id get_data_package_id_for_integer_constant(struct parser_state *, unsigned int *);
struct constant_description_id parse_integer_constant(struct parser_state *, struct c_lexer_token *, unsigned int, unsigned int);
struct specifier_or_qualifier_list_item_id add_to_simple_type_specifier_list(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct simple_type_specifier_id);
unsigned int can_fit_in(unsigned int *, unsigned int *);
void set_first_n_bits(struct parser_state *, unsigned int *, unsigned int);
struct general_type_id make_const_char_ptr_general_type(struct parser_state *);
struct general_type_id make_const_char_general_type(struct parser_state *);
void make_tag_predeclarations(struct parser_state *, struct specifier_or_qualifier_list_item_id);
void make_tag_predeclarations_h(struct parser_state *, struct scoped_tag_specifier_details);
unsigned int specifier_list_is_only_void(struct parser_state *, struct specifier_or_qualifier_list_item_id);
void declare_enum_member(struct parser_state *, struct identifier_id, struct scoped_tag_specifier_id);

struct parser_checkpoint create_parser_checkpoint(struct parser_state * state){
	struct parser_checkpoint c;
	c.parser_operation_checkpoint = struct_parser_operation_list_size(&state->operation_stack);
	c.asm_checkpoint = asm_checkpoint(state->syntax_model);
	return c;
}

unsigned int get_token_line_number(struct c_lexer_state * state, struct c_lexer_token * t){
	/* Number of newlines up to and including the given token */
	unsigned int line = 1;
	unsigned int i;
	for(i = 0; i < struct_c_lexer_token_ptr_list_size(&state->tokens); i++){
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i)->type == NEWLINE){
			line++;
		}
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i)->type == COMMENT){
			line += count_newlines_in_comment(struct_c_lexer_token_ptr_list_get(&state->tokens, i));
		}
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i) == t){
			return line;
		}
	}
	assert(0 && "Should have found token.");
	return 0;
}

unsigned int get_token_index(struct c_lexer_state * state, struct c_lexer_token * t, unsigned int * success){
	unsigned int i;
	for(i = 0; i < struct_c_lexer_token_ptr_list_size(&state->tokens); i++){
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i) == t){
			*success = 1;
			return i;
		}
	}
	return 0; /* Currently occurs due to parser node and lexer token copying. */
}

struct parser_node * get_last_terminal_node(struct parser_node * n){
	if(n){
		struct parser_node * a;
		a = get_last_terminal_node(n->next);
		if(a){
			return a;
		}else{
			if(n->type == TERMINAL){
				return n;
			}
			return get_last_terminal_node(n->first_child);
		}

	}else{
		return (struct parser_node *)0;
	}
}

void print_node_context(struct c_lexer_state * state, struct parser_node * n){
	struct parser_node * first_terminal = n;
	struct parser_node * last_terminal = n ? (n->type == TERMINAL ? n : n->first_child) : (struct parser_node *)0;
	unsigned int lines_context = 3; /*  Print this many lines before and after */
	unsigned int newlines_encountered_before = 0;
	unsigned int newlines_encountered_after = 0;
	unsigned int start_token_index;
	unsigned char * first_byte;
	unsigned char * last_byte;
	struct c_lexer_token * start_context_token;
	struct c_lexer_token * end_context_token = struct_c_lexer_token_ptr_list_get(&state->tokens, struct_c_lexer_token_ptr_list_size(&state->tokens) -1);
	unsigned int i;
	unsigned int success = 0;
	if(!n){
		printf("Was passed null context.\n");
		return;
	}
	(void)lines_context;
	while(first_terminal->type != TERMINAL){
		first_terminal = first_terminal->first_child;
	}
	last_terminal = get_last_terminal_node(last_terminal);
	assert(last_terminal && "Not able to find a terminal node.");
	first_byte = first_terminal->c_lexer_token->first_byte;
	last_byte = last_terminal->c_lexer_token->last_byte;
	assert(first_byte >= state->c.buf && first_byte <= (state->c.buf + state->c.buffer_size) && last_byte >= state->c.buf && last_byte <= (state->c.buf + state->c.buffer_size)); /*  Make sure these are actually inside the buffer.  Some tokens are made up */
	/*  Find the start and end of context tokens */
	start_token_index = get_token_index(state, first_terminal->c_lexer_token, &success);
	if(!success){
		printf("Unable to find location of node in context.\n");
		return;
	}

	/*  This is the first token we're going to print */
	for(i = start_token_index; ; i--){
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i)->type == NEWLINE){
			newlines_encountered_before++;
			if(newlines_encountered_before == lines_context){
				start_context_token = struct_c_lexer_token_ptr_list_get(&state->tokens, i);
				break;
			}
		}
		if(i == 0){
			start_context_token = struct_c_lexer_token_ptr_list_get(&state->tokens, i);
			break;
		}
	}
	/*  This is the last token we're going to print */
	for(i = start_token_index; i < struct_c_lexer_token_ptr_list_size(&state->tokens); i++){
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i)->type == NEWLINE){
			newlines_encountered_after++;
			if(newlines_encountered_after == lines_context){
				end_context_token = struct_c_lexer_token_ptr_list_get(&state->tokens, i);
				break;
			}
		}
	}
	pretty_print_context(state, start_context_token, end_context_token, first_terminal->c_lexer_token, last_terminal->c_lexer_token);
}

void pretty_print_context(struct c_lexer_state * state, struct c_lexer_token * start_context_token, struct c_lexer_token * end_context_token, struct c_lexer_token * start_target, struct c_lexer_token * end_target){
	unsigned int i;
	unsigned int success = 0;
	unsigned int start_line = get_token_line_number(state, start_context_token);
	unsigned int first_token_index = get_token_index(state, start_context_token, &success);
	unsigned int last_token_index = get_token_index(state, end_context_token, &success);
	assert(start_context_token != start_target && end_context_token != end_target && "Haven't accounted for this case.");
	for(i = first_token_index; i <= last_token_index; i++){
		struct c_lexer_token * tok = struct_c_lexer_token_ptr_list_get(&state->tokens, i);
		unsigned char * j;
		if(tok == start_target){
			printf(">>>>");
		}
		for(j = tok->first_byte; j < (tok->last_byte + 1); j++){
			printf("%c", *j);
		}
		if(tok->type == NEWLINE){
			printf("%s:%u:", state->c.filename, start_line);
			start_line++;
		}
		if(tok == end_target){
			printf("<<<<");
		}
	}
}

struct scope_level * get_last_function_namespace_scope(struct parser_state * state){
	(void)state;
	return (struct scope_level *)0;
}

void parser_progress(const char* format, ...){
	va_list arglist;
	va_start( arglist, format );
	/*
	if(1){
		printf(";");
		vprintf( format, arglist );
	}
	*/
	va_end( arglist );
}


void add_terminal_tokens_to_buffer_h2(struct unsigned_char_list * buffer, struct parser_node * n, unsigned int * num_added){
	while(n){
		add_terminal_tokens_to_buffer_h1(buffer, n, num_added);
		if(n->type == TERMINAL){
			if(num_added){
				buffered_puts(buffer, " ");
			}
			add_string_to_buffer(buffer, n->c_lexer_token->first_byte, n->c_lexer_token->last_byte);
			*num_added = *num_added + 1;
			
		}
		n = n->next;
	}
}

void add_terminal_tokens_to_buffer_h1(struct unsigned_char_list * buffer, struct parser_node * n, unsigned int * num_added){
	if(n && n->first_child){
		add_terminal_tokens_to_buffer_h2(buffer, n->first_child, num_added);
	}
}

void add_terminal_tokens_to_buffer(struct unsigned_char_list * buffer, struct parser_node * n){
	unsigned int num = 0;
	add_terminal_tokens_to_buffer_h1(buffer, n, &num);
}

struct parser_node * get_identifier_from_declarator(struct parser_node * n){
	assert(n->type == DECLARATOR);
	while(!(n->type == TERMINAL && n->c_lexer_token->type == IDENTIFIER)){
		if(n->type == POINTER || n->type == TERMINAL){ 
			n = n->next;
		}
		if(n->type == DECLARATOR || n->type == DIRECT_DECLARATOR){
			n = n->first_child;
		}
	}
	return n;
}

struct constant_description * find_constant(struct parser_state * state, unsigned char * str){
	return unsigned_char_ptr_to_struct_constant_description_ptr_map_exists(&state->constant_map, str) ? unsigned_char_ptr_to_struct_constant_description_ptr_map_get(&state->constant_map, str) : (struct constant_description *)0;
}

unsigned int convert_integer(unsigned char * str, unsigned int radix, unsigned int prefix_size){
	unsigned int rtn = 0;
	unsigned int len = (unsigned int)strlen((const char *)str);
	unsigned int i = len;
	unsigned int base = 1;
	while(i > prefix_size){
		if(!(str[i-1] == 'u' || str[i-1] == 'U' || str[i-1] == 'l' || str[i-1] == 'L')){
			unsigned int digit = get_hex_digit_value(str[i-1]);
			rtn += digit * base;
			base = base * radix;
		}
		i--;
	}
	return rtn;
}

unsigned int convert_hexadecimal_constant(unsigned char * str){
	return convert_integer(str, 16, 2);
}

unsigned int convert_decimal_constant(unsigned char * str){
	return convert_integer(str, 10, 0);
}

unsigned int convert_octal_constant(unsigned char * str){
	return convert_integer(str, 8, 0);
}

unsigned int convert_character_constant(unsigned char * str, unsigned char * end, unsigned int * result, unsigned int * done){
	(void)end; /*  Add in error checking in the future... */
	(void)done;
	/*  Return the number of characters consumed from the input */
	if(str[0] == '\\'){
		switch(str[1]){
			case '\\':{
				*result = '\\';
				return 2;
			}case 'n':{
				*result = '\n';
				return 2;
			}case 'r':{
				*result = '\r';
				return 2;
			}case 't':{
				*result = '\t';
				return 2;
			}case '\'':{
				*result = '\'';
				return 2;
			}case '?':{
				*result = '?';
				return 2;
			}case '"':{
				*result = '"';
				return 2;
			}case 'a':{
				*result = '\a';
				return 2;
			}case 'b':{
				*result = '\b';
				return 2;
			}case 'f':{
				*result = '\f';
				return 2;
			}case 'v':{
				*result = '\v';
				return 2;
			}case 'x':{
				struct unsigned_char_list hex_chars;
				unsigned int chars_consumed = 2;
				str += 2; /*  Jump over '\x' */
				unsigned_char_list_create(&hex_chars);
				unsigned_char_list_add_end(&hex_chars, '0');
				unsigned_char_list_add_end(&hex_chars, 'x');
				while(*str && ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f') || (*str >= 'A' && *str <= 'F'))){
					unsigned_char_list_add_end(&hex_chars, *str);
					chars_consumed++;
					str++;
				}
				unsigned_char_list_add_end(&hex_chars, '\0');
				*result = convert_hexadecimal_constant(unsigned_char_list_data(&hex_chars));
				unsigned_char_list_destroy(&hex_chars);
				return chars_consumed;
			}case '0':; case '1':;case '2':;case '3':;case '4':;case '5':;case '6':;case '7':{
				struct unsigned_char_list octal_chars;
				unsigned int chars_consumed = 1;
				str += 1; /*  Jump over '\' */
				unsigned_char_list_create(&octal_chars);
				while(*str && (*str >= '0' && *str <= '7')){
					unsigned_char_list_add_end(&octal_chars, *str);
					chars_consumed++;
					str++;
				}
				unsigned_char_list_add_end(&octal_chars, '\0');
				*result = convert_octal_constant(unsigned_char_list_data(&octal_chars));
				unsigned_char_list_destroy(&octal_chars);
				return chars_consumed;
			}default:{
				printf("Unknown character constant 0x%X, '%c'\n", str[1], str[1]);
				assert(0 && "Unknown character constant.");
				return 0;
			}
		}
	}
	*result = str[0];
	return 1;
}

void convert_string_literal(unsigned char * start, unsigned char * end, struct unsigned_char_list * converted_chars){
	unsigned char * str = start;
	/*  For now, just ignore the encoding specifiers. */
	while(*str == 'u' || *str == 'U' || *str == '8' || *str == 'L'){
		str++;
	}
	str++; /* Jump over opening " */
	/*  This assumes that the STRING_LITERAL is a well formed token as described in the lexer */
	while(*str != '"'){ /* Any unescaped " is an end of string */
		unsigned int c;
		unsigned int done = 0;
		str += convert_character_constant(str, end, &c, &done);
		if(done){
			break;
		}
		unsigned_char_list_add_end(converted_chars, (unsigned char)c);
	}
}



void * push_operation(struct parser_state * state, enum parser_operation_type t, void * data){
	struct parser_operation new_operation;
	new_operation.type = t;
	new_operation.data = t == ADVANCE_PARSER_POSITION ? struct_parser_node_memory_pool_malloc(state->memory_pool_collection) : data;
	struct_parser_operation_list_add_end(&state->operation_stack, new_operation);
	switch(t){
		case ADVANCE_TOKEN_POSITION:{
			/*
			struct unsigned_char_list l;
			struct c_lexer_token * t1 = struct_c_lexer_token_ptr_list_data(&state->c_lexer_state->tokens)[state->tokens_position];
			unsigned_char_list_create(&l);
			add_string_to_buffer(&l, t1->first_byte, t1->last_byte);
			unsigned_char_list_add_end(&l, '\0');
			printf("Accepted %s\n", (char *)unsigned_char_list_data(&l));
			*/
			state->tokens_position = state->tokens_position + 1;
			return (void *)0;
		}case ADVANCE_PARSER_POSITION:{
			*((struct parser_node *)new_operation.data) = *((struct parser_node *)data);
			return new_operation.data;
		}case INCREMENT_NEWLINE_COUNT:{
			state->line_number = state->line_number + 1;
			return (void *)0;
		}case INCREMENT_SCOPE_DEPTH:{
			/*  Scope guid should have been added prior to calling this function. */
			return (void *)0;
		}case DECREMENT_SCOPE_DEPTH:{
			struct scope_guid_id poped = struct_scope_guid_id_list_pop_end(&state->scope_guid_stack);
			struct_scope_guid_id_list_add_end(&state->decremented_scope_guids, poped);
			return (void *)0;
		}default:{
			assert(0 && "This should never happen.\n");
			return (void *)0;
		}
	}
}

void pop_operation(struct parser_state * state){
	struct parser_operation poped_operation;
	if(struct_parser_operation_list_size(&state->operation_stack) == 0){
		assert(0 && "Trying to pop empty operation stack.\n");
		return;
	}
	poped_operation = struct_parser_operation_list_pop_end(&state->operation_stack);
	
	switch(poped_operation.type){
		case ADVANCE_TOKEN_POSITION:{
			unsigned int new_num_tokens = state->tokens_position - 1;
			state->tokens_position = new_num_tokens;
			/*
			{
			struct unsigned_char_list l;
			struct c_lexer_token * t = struct_c_lexer_token_ptr_list_data(&state->c_lexer_state->tokens)[state->tokens_position];
			unsigned_char_list_create(&l);
			add_string_to_buffer(&l, t->first_byte, t->last_byte);
			unsigned_char_list_add_end(&l, '\0');
			printf("Unaccepted %s\n", (char *)unsigned_char_list_data(&l));
			}
			*/
			break;
		}case ADVANCE_PARSER_POSITION:{
			struct_parser_node_memory_pool_free(state->memory_pool_collection, (struct parser_node*)poped_operation.data);
			break;
		}case INCREMENT_NEWLINE_COUNT:{
			state->line_number = state->line_number - 1;
			break;
		}case INCREMENT_SCOPE_DEPTH:{
			/*  Just discard it */
			struct_scope_guid_id_list_pop_end(&state->scope_guid_stack);
			break;
		}case DECREMENT_SCOPE_DEPTH:{
			/* Undoing a decrement just means adding it back */
			struct scope_guid_id poped = struct_scope_guid_id_list_pop_end(&state->decremented_scope_guids);
			struct_scope_guid_id_list_add_end(&state->scope_guid_stack, poped);
			break;
		}default:{
			assert(0 && "This should never happen.\n");
		}
	}
}

void backtrack_parser(struct parser_state * state, unsigned int target){
	while(struct_parser_operation_list_size(&state->operation_stack) != target){
		pop_operation(state);
	}
}

void backtrack(struct parser_state * state, struct parser_checkpoint checkpoint){
	backtrack_parser(state, checkpoint.parser_operation_checkpoint);
	backtrack_asm(state->syntax_model, checkpoint.asm_checkpoint);
}

struct parser_node * create_parser_node(struct parser_state * state, struct parser_node * n, struct parser_node * f, struct c_lexer_token * l, enum node_type t){
	struct parser_node new_node;
	new_node.next = n;
	new_node.first_child = f;
	new_node.c_lexer_token = l;
	new_node.type = t;
	return (struct parser_node *)push_operation(state, ADVANCE_PARSER_POSITION, (void*)&new_node);
}

struct parser_node * p_accept(enum c_token_type t, struct parser_state * state){
	unsigned int num_tokens = struct_c_lexer_token_ptr_list_size(&state->c_lexer_state->tokens);
	struct c_lexer_token ** tokens = struct_c_lexer_token_ptr_list_data(&state->c_lexer_state->tokens);
	while(
		(state->tokens_position < num_tokens) &&
		(
			tokens[state->tokens_position]->type == SPACE ||
			tokens[state->tokens_position]->type == NEWLINE ||
			tokens[state->tokens_position]->type == COMMENT
		)
	){
		if(tokens[state->tokens_position]->type == NEWLINE){
			push_operation(state, INCREMENT_NEWLINE_COUNT, (void*)0);
		}else if(tokens[state->tokens_position]->type == COMMENT){
			unsigned int num_newlines = count_newlines_in_comment(tokens[state->tokens_position]);
			unsigned int i;
			for(i = 0; i < num_newlines; i++){
				push_operation(state, INCREMENT_NEWLINE_COUNT, (void*)0);
			}
		}
		push_operation(state, ADVANCE_TOKEN_POSITION, (void*)tokens[state->tokens_position]);
	}

	if(state->tokens_position < num_tokens){
		if(tokens[state->tokens_position]->type == t){
			struct c_lexer_token * lt;
			lt = tokens[state->tokens_position];
		        push_operation(state, ADVANCE_TOKEN_POSITION, (void*)tokens[state->tokens_position]);
			return create_parser_node(state, (struct parser_node *)0, (struct parser_node *)0, lt, TERMINAL);
		}
	}
	return (struct parser_node *)0;
}

struct parser_node * expect(enum c_token_type t, struct parser_state * state){
	struct parser_node * l = p_accept(t,state);
	if(l){
		return l;
	}
	assert(0 && "Error, expected ....\n");
	return (struct parser_node *)0;
}

struct parser_node * get_any_identifier_node(struct parser_state * state, struct identifier_id * id){
	struct parser_node * n;
	if((n = p_accept(IDENTIFIER, state))){
		*id = make_identifier_identifier(state->syntax_model, n->c_lexer_token);
		return n;
	}else{
		return (struct parser_node *)0;
	}
}

struct parser_node * get_non_typedef_identifier_node(struct parser_state * state, struct scope_guid_id * non_typedef_origin_scope_id, struct identifier_id * id){
	struct parser_node * n;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	if((n = p_accept(IDENTIFIER, state))){
		struct identifier_id i = make_identifier_identifier(state->syntax_model, n->c_lexer_token);
		if(get_scope_of_closest_identifier_declaration(state, non_typedef_origin_scope_id, get_current_scope_guid(state), i, ASM_IDENTIFIER_NAMESPACE_ENTRY_DECLARATION, ASM_IDENTIFIER_NAMESPACE_ENTRY_TYPEDEF)){
			*id = i;
			return n;
		}else{
			backtrack(state, checkpoint);
			parser_progress("Identifier was typedefed or not declared, put it back.\n");
			return (struct parser_node *)0;
		}
	}else{
		return (struct parser_node *)0;
	}
}

struct parser_node * get_typedef_identifier_node(struct parser_state * state, struct scope_guid_id * typedef_origin_scope_id, struct identifier_id * id){
	struct parser_node * n;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	if((n = p_accept(IDENTIFIER, state))){
		struct identifier_id i = make_identifier_identifier(state->syntax_model, n->c_lexer_token);
		if(get_scope_of_closest_identifier_declaration(state, typedef_origin_scope_id, get_current_scope_guid(state), i, ASM_IDENTIFIER_NAMESPACE_ENTRY_TYPEDEF, ASM_IDENTIFIER_NAMESPACE_ENTRY_DECLARATION)){
			*id = i;
			return n;
		}else{
			parser_progress("Identifier not typedefed type or not declared.  Putting back tokens.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		return (struct parser_node *)0;
	}
}

#define MAX_CONSTANT_WORD_MULTIPLE (128 / (sizeof(unsigned int) * 8))

void print_number_rep(unsigned int * n){
	unsigned int i;
	unsigned int saw_non_zero = 0;
	printf("0x");
	for(i = MAX_CONSTANT_WORD_MULTIPLE; i-- > 0;){
		if(n[i]){
			if(saw_non_zero){
				printf("%08X", n[i]);
			}else{
				printf("%X", n[i]);
			}
			saw_non_zero = 1;
		}else if(saw_non_zero){
			printf("%08X", n[i]);
		}
	}
	if(!saw_non_zero){
		printf("%X", n[0]);
	}
}

unsigned int get_hex_digit_value(unsigned char c){
	if(c >= '0' && c <= '9'){
		return c - '0';
	}else if(c >= 'a' && c <= 'f'){
		return (c - 'a') + 10;
	}else if(c >= 'A' && c <= 'F'){
		return (c - 'A') + 10;
	}else{
		assert(0);
		return 0;
	}
}

unsigned int add_with_carry(struct parser_state * state, unsigned int * parts, unsigned int index, unsigned int value_to_add){
	/*printf("Adding %u to index %u\n", value_to_add, index);*/
	if(index >= MAX_CONSTANT_WORD_MULTIPLE){
		if(value_to_add > 0){
			/*printf("Overflow!\n");*/
			add_parser_error(state, PARSER_ERROR_CONSTANT_OVERFLOW);
			return 1;
		}
	}else{
		unsigned int max = 0;
		unsigned int representations_left = (~max) - parts[index];
		/*printf("Checking if overflow will occur: %u representations left, need to add %u.\n", representations_left, value_to_add);*/
		if(representations_left < value_to_add){
			unsigned int remain_after_carry = value_to_add - (representations_left + 1);
			/*printf("Overflow will occur.  Do a carry with %u remaining.\n", remain_after_carry);*/
			parts[index] = remain_after_carry;
			return add_with_carry(state, parts, index + 1, 1);
		}else{
			parts[index] = parts[index] + value_to_add;
		}
	}
	return 0;
}

unsigned int shift_and_add_in(struct parser_state * state, unsigned int * parts, unsigned int digit_value, unsigned int bits_to_shift){
	/*  Use an array of unsigned ints to represent an arbitrarly large number.
	    By figuring out which array element a shifted value would end up in, we can add the current
	    digit to whatever part of the array it belongs to.
	*/
	/*printf("Doing shift and add in with digit value %u and shifting %u bits\n", digit_value, bits_to_shift);*/
	/*printf("Before: ");*/
	/*printf("\n");*/
	unsigned int bits_in_int = (sizeof(unsigned int) * 8);
	unsigned int lower_index = bits_to_shift / bits_in_int;
	unsigned int higher_index = lower_index + 1;
	unsigned int lower_shift = bits_to_shift % bits_in_int;
	unsigned int higher_shift = (bits_in_int - lower_shift) % bits_in_int;
	/*print_number_rep(parts);*/
	/*printf("lower_shift: %u higher_shift: %u\n", lower_shift, higher_shift);*/
	if(add_with_carry(state, parts, lower_index, digit_value << lower_shift)){
		return 1;
	}
	if(higher_shift){
		if(add_with_carry(state, parts, higher_index, digit_value >> higher_shift)){
			return 1;
		}
	}
	/*printf("After: ");*/
	/*print_number_rep(parts);*/
	/*printf("\n");*/
	return 0;
}

unsigned int update_base10_rep(struct parser_state * state, unsigned int * current_base10_rep, unsigned int * tmp, unsigned int position){
	if(position == 0){
		unsigned int i;
		for(i = 0; i < MAX_CONSTANT_WORD_MULTIPLE; i++){
			current_base10_rep[i] = 0;
		}
		current_base10_rep[0] = 1;
	}else{
		unsigned int i;
		/*  Make a copy to begin multiply by 10 */
		for(i = 0; i < MAX_CONSTANT_WORD_MULTIPLE; i++){
			tmp[i] = current_base10_rep[i];
		}
		/*  Reset to we can use add in method */
		for(i = 0; i < MAX_CONSTANT_WORD_MULTIPLE; i++){
			current_base10_rep[i] = 0;
		}

		/*  Multiplication of x by 10 is equal to (x << 3) + (x << 1) */
		for(i = 0; i < MAX_CONSTANT_WORD_MULTIPLE; i++){
			if(shift_and_add_in(state, current_base10_rep, tmp[i], (i * sizeof(unsigned int) * 8) + 3)){
				return 1;
			}
			if(shift_and_add_in(state, current_base10_rep, tmp[i], (i * sizeof(unsigned int) * 8) + 1)){
				return 1;
			}
		}
	}
	/*printf("Updated rep: ");*/
	/*print_number_rep(current_base10_rep);*/
	/*printf("\n");*/
	return 0;
}

unsigned int add_digit_value_to_parts(struct parser_state * state, unsigned int * parts, unsigned int * current_base10_rep, unsigned int * tmp, unsigned int position, unsigned int digit_value, unsigned int radix){
	/*printf("Adding to parts: position: %u digit_value: %u radix: %u.\n", position, digit_value, radix);*/
	switch(radix){
		case 8:{
			if(shift_and_add_in(state, parts, digit_value, 3 * position)){
				return 1;
			}
			break;
		}case 10:{
			unsigned int i;
			/*printf("Begin base 10 update\n");*/
			if(update_base10_rep(state, current_base10_rep, tmp, position)){
				return 1;
			}
			/*printf("End base 10 update\n");*/
			for(i = 0; i < (sizeof(unsigned int) * 8 * MAX_CONSTANT_WORD_MULTIPLE); i++){
				/*  Which part are we looking for a bit in? */
				unsigned int part_index = i / (sizeof(unsigned int) * 8);
				unsigned int bit_state = current_base10_rep[part_index] & (0x1 << (i % (sizeof(unsigned int) * 8)));
				/*printf("Adding in base 10 part for bit %u:  part index is %u, and bit state is %u\n", i, part_index, bit_state);*/
				if(bit_state){
					if(shift_and_add_in(state, parts, digit_value, i)){
						return 1;
					}
				}
			}
			break;
		}case 16:{
			if(shift_and_add_in(state, parts, digit_value, MAX_CONSTANT_WORD_MULTIPLE * position)){
				return 1;
			}
			break;
		}default:{
			assert(0);
		}
	}
	return 0;
}

unsigned int token_radix_parse(struct parser_state * state, struct c_lexer_token * t, unsigned int * parts, unsigned int begin_index, unsigned int end_index, unsigned int radix){
	unsigned int current_base10_rep[MAX_CONSTANT_WORD_MULTIPLE];
	unsigned int tmp[MAX_CONSTANT_WORD_MULTIPLE];
	unsigned int i;
	unsigned int position = 0;
	for(i = end_index + 1; i-- > begin_index;){
		unsigned char c = t->first_byte[i];
		unsigned int digit_value = get_hex_digit_value(c);
		if(add_digit_value_to_parts(state, parts, current_base10_rep, tmp, position, digit_value, radix)){
			return 1;
		}
		position++;
	}
	/*printf("Result: ");
	print_number_rep(parts);
	printf("\n");
	*/
	return 0;	
}

struct specifier_or_qualifier_list_item_id add_to_simple_type_specifier_list(struct parser_state * state, struct specifier_or_qualifier_list_item_id prev, struct simple_type_specifier_id simple){
	struct aggregate_type_specifier_id aggregate_type_specifier_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, simple);
	struct specifier_or_qualifier_id specifier_or_qualifier_id = make_specifier_or_qualifier_aggregate_type_specifier(state->syntax_model, aggregate_type_specifier_id);
	return make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, prev, specifier_or_qualifier_id);
}

void set_first_n_bits(struct parser_state * state, unsigned int * parts, unsigned int n){
	unsigned int i;
	for(i = MAX_CONSTANT_WORD_MULTIPLE; i-- > 0;){
		parts[i] = 0;
	}
	for(i = 0; i < n; i++){
		shift_and_add_in(state, parts, 1, i);
	}
}

unsigned int can_fit_in(unsigned int * number, unsigned int * limit){
	unsigned int i;
	for(i = MAX_CONSTANT_WORD_MULTIPLE; i-- > 0; ){
		if(number[i] > limit[i]){
			return 0;
		}else if (number[i] < limit[i]){
			return 1;
		}else{
			/*  Else, equal, continue comparison */
		}
	}
	return 1;
}

struct general_type_id get_general_type_id_for_integer_constant(struct parser_state * state, unsigned int * parts, unsigned int radix, unsigned int num_l_flags, unsigned int num_u_flags){
	struct specifier_or_qualifier_list_item_id list_start = make_specifier_or_qualifier_list_item_list_start(state->syntax_model);
	struct simple_type_specifier_id int_specifier = make_simple_type_specifier_int(state->syntax_model);
	struct simple_type_specifier_id long_specifier = make_simple_type_specifier_long(state->syntax_model);
	struct simple_type_specifier_id unsigned_specifier = make_simple_type_specifier_unsigned(state->syntax_model);
	struct specifier_or_qualifier_list_item_id final_list;
	unsigned int int_max[MAX_CONSTANT_WORD_MULTIPLE];
	unsigned int uint_max[MAX_CONSTANT_WORD_MULTIPLE];
	unsigned int long_max[MAX_CONSTANT_WORD_MULTIPLE];
	unsigned int ulong_max[MAX_CONSTANT_WORD_MULTIPLE];
	unsigned int long_long_max[MAX_CONSTANT_WORD_MULTIPLE];
	unsigned int ulong_long_max[MAX_CONSTANT_WORD_MULTIPLE];
	unsigned int is_unsiffixed_decimal = num_u_flags == 0 && num_l_flags == 0 && radix == 10;
	set_first_n_bits(state, int_max, 31);
	set_first_n_bits(state, uint_max, 32);
	set_first_n_bits(state, long_max, 63);
	set_first_n_bits(state, ulong_max, 64);
	set_first_n_bits(state, long_long_max, 127);
	set_first_n_bits(state, ulong_long_max, 128);
	if(can_fit_in(parts, int_max) && num_u_flags == 0 && num_l_flags == 0){
		final_list = add_to_simple_type_specifier_list(state, list_start, int_specifier);
	}else if(can_fit_in(parts, uint_max) && num_l_flags == 0 && !is_unsiffixed_decimal){
		final_list = add_to_simple_type_specifier_list(state, list_start, unsigned_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, int_specifier);
	}else if(can_fit_in(parts, long_max) && num_u_flags == 0 && num_l_flags != 2){
		final_list = add_to_simple_type_specifier_list(state, list_start, long_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, int_specifier);
	}else if(can_fit_in(parts, ulong_max) && num_l_flags != 2){
		final_list = add_to_simple_type_specifier_list(state, list_start, unsigned_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, long_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, int_specifier);
	}else if(can_fit_in(parts, long_long_max) && num_u_flags == 0){
		final_list = add_to_simple_type_specifier_list(state, list_start, long_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, long_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, int_specifier);
	}else{
		final_list = add_to_simple_type_specifier_list(state, list_start, unsigned_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, long_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, long_specifier);
		final_list = add_to_simple_type_specifier_list(state, final_list, int_specifier);
	}
	return make_general_type_general_type(state->syntax_model, final_list, make_declarator_part_list_item_list_start(state->syntax_model), make_bitfield_no_bitfield(state->syntax_model));
}

struct data_package_id get_data_package_id_for_integer_constant(struct parser_state * state, unsigned int * parts){
	if(parts[3] || parts[2]){
		struct qword_package_id qword_package_id = make_qword_package_qword(state->syntax_model, parts[3], parts[2], parts[1], parts[0]);
		return make_data_package_qword(state->syntax_model, qword_package_id);
	}else if(parts[1]){
		struct dword_package_id dword_package_id = make_dword_package_dword(state->syntax_model, parts[1], parts[0]);
		return make_data_package_dword(state->syntax_model, dword_package_id);
	}else{
		struct word_package_id word_package_id = make_word_package_word(state->syntax_model, parts[0]);
		return make_data_package_word(state->syntax_model, word_package_id);
	}
}

struct constant_description_id parse_integer_constant(struct parser_state * state, struct c_lexer_token * t, unsigned int radix, unsigned int begin_index){
	unsigned int token_length = 0;
	unsigned char * c = t->first_byte;
	unsigned int i;
	unsigned int num_l_flags = 0;
	unsigned int num_u_flags = 0;
	unsigned int parts[MAX_CONSTANT_WORD_MULTIPLE];
	struct general_type_id general_type_id;
	struct data_package_id data_package_id;
	unsigned int parse_rtn;
	for(i = 0; i < MAX_CONSTANT_WORD_MULTIPLE; i++){
		parts[i] = 0;
	}
	/*  We rely on the lexer to supply us with a valid constant that ends in ([uU]?([lL]|(ll|LL)))|([uU]([lL]|(ll|LL))?)*/
	do{
		if(*c == 'l' || *c == 'L'){
			num_l_flags++;
		}else if(*c == 'u' || *c == 'U'){
			num_u_flags++;
		}
		token_length++;
	} while(c++ != t->last_byte);
	/*printf("len is %u\n", token_length);*/
	/*  Begin parsing from the end just before the u/l flags appear and back until the begin_index  */
	parse_rtn = token_radix_parse(state, t, parts, begin_index, token_length - (num_l_flags + num_u_flags) -1, radix);
	if(parse_rtn){
		struct bitfield_id bitfield_id = make_bitfield_no_bitfield(state->syntax_model);
		struct declarator_part_list_item_id declarator = make_declarator_part_list_item_list_start(state->syntax_model);
		struct specifier_or_qualifier_list_item_id specifiers = make_specifier_or_qualifier_list_item_list_start(state->syntax_model);
		general_type_id = make_general_type_general_type(state->syntax_model, specifiers, declarator, bitfield_id);
		data_package_id = make_data_package_error(state->syntax_model, make_error_node_error(state->syntax_model));
	}else{
		general_type_id = get_general_type_id_for_integer_constant(state, parts, radix, num_l_flags, num_u_flags);
		data_package_id = get_data_package_id_for_integer_constant(state, parts);
	}
	return make_constant_description_constant_description(state->syntax_model, general_type_id, data_package_id);
}

struct general_type_id make_const_char_general_type(struct parser_state * state){
	struct simple_type_qualifier_id const_id = make_simple_type_qualifier_const(state->syntax_model);
	struct specifier_or_qualifier_id wrapped_const = make_specifier_or_qualifier_simple_type_qualifier(state->syntax_model, const_id);
	struct simple_type_specifier_id char_id = make_simple_type_specifier_char(state->syntax_model);
	struct aggregate_type_specifier_id wrapped_char_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, char_id);
	struct specifier_or_qualifier_id wrapped_wrapped_char_id = make_specifier_or_qualifier_aggregate_type_specifier(state->syntax_model, wrapped_char_id);
	struct specifier_or_qualifier_list_item_id first = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), wrapped_const);
	struct specifier_or_qualifier_list_item_id second = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, first, wrapped_wrapped_char_id);

	struct declarator_part_list_item_id declarator_part_list_item_id = make_declarator_part_list_item_list_start(state->syntax_model);
	return make_general_type_general_type(state->syntax_model, second, declarator_part_list_item_id, make_bitfield_no_bitfield(state->syntax_model));
}

struct general_type_id make_const_char_ptr_general_type(struct parser_state * state){
	struct simple_type_qualifier_id const_id = make_simple_type_qualifier_const(state->syntax_model);
	struct specifier_or_qualifier_id wrapped_const = make_specifier_or_qualifier_simple_type_qualifier(state->syntax_model, const_id);
	struct simple_type_specifier_id char_id = make_simple_type_specifier_char(state->syntax_model);
	struct aggregate_type_specifier_id wrapped_char_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, char_id);
	struct specifier_or_qualifier_id wrapped_wrapped_char_id = make_specifier_or_qualifier_aggregate_type_specifier(state->syntax_model, wrapped_char_id);
	struct specifier_or_qualifier_list_item_id first = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), wrapped_const);
	struct specifier_or_qualifier_list_item_id second = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, first, wrapped_wrapped_char_id);

	struct pointer_part_id pointer_part_id = make_pointer_part_pointer(state->syntax_model, make_specifier_or_qualifier_list_item_list_start(state->syntax_model));
	struct declarator_part_id declarator_part_id = make_declarator_part_pointer(state->syntax_model, pointer_part_id);
	struct declarator_part_list_item_id declarator_part_list_item_id = make_declarator_part_list_item_declarator_part(state->syntax_model, make_declarator_part_list_item_list_start(state->syntax_model), declarator_part_id);
	return make_general_type_general_type(state->syntax_model, second, declarator_part_list_item_id, make_bitfield_no_bitfield(state->syntax_model));
}

struct parser_node * string_literal_list_rest(struct parser_state * state, struct unsigned_char_list * chars){
	struct parser_node * n;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	if((n = p_accept(STRING_LITERAL, state))){
		convert_string_literal(n->c_lexer_token->first_byte, n->c_lexer_token->last_byte, chars);
		if((n->next = string_literal_list_rest(state, chars))){
			return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token *)0, STRING_LITERAL_LIST_REST);
		}else{
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * e = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, e, (struct c_lexer_token *)0, STRING_LITERAL_LIST_REST);
	}
}

struct parser_node * string_literal_list(struct parser_state * state, struct primary_expression_id * e){
	struct unsigned_char_list chars;
	struct parser_node * n;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned_char_list_create(&chars);
	if((n = p_accept(STRING_LITERAL, state))){
		convert_string_literal(n->c_lexer_token->first_byte, n->c_lexer_token->last_byte, &chars);
		if((n->next = string_literal_list_rest(state, &chars))){
			unsigned int size_in_bytes;
			unsigned int size_in_words;
			unsigned int * result_chrs;
			struct string_package_id * existing_string_package_id;
			struct string_package_id required_string_package_id;
			struct string_package_details details;
			struct constant_description_id constant_description_id;
			struct general_type_id const_char_ptr = make_const_char_ptr_general_type(state);
			struct data_package_id data_package_id;

			unsigned_char_list_add_end(&chars, '\0'); /*  Null termination. */
			size_in_bytes = unsigned_char_list_size(&chars);
			while((unsigned_char_list_size(&chars) % sizeof(unsigned int)) != 0){
				unsigned_char_list_add_end(&chars, '\0'); /*  Ensure int alignment at end of string */
			}
			size_in_words = size_in_bytes / sizeof(unsigned int);
			/*  Don't destroy char list, return data buffer as raw converted data with padded nulls at end */
			result_chrs = (unsigned int *)(void*)unsigned_char_list_data(&chars);

			details.type = ASM_STRING_PACKAGE_STRING;
			details.byte_length = size_in_bytes;
			details.word_length = size_in_words;
			details.data = result_chrs;
			/*  Check if it already exists so we can free the string. */
			if((existing_string_package_id = struct_string_package_details_to_struct_string_package_id_map_exists(&state->syntax_model->string_packages_map, details))){
				required_string_package_id = *existing_string_package_id;
				free(result_chrs); /* Currently using some magic with relying on the internal implementation of the character list. */
			}else{
				required_string_package_id = make_string_package_string(state->syntax_model, size_in_bytes, size_in_words, result_chrs);
			}
			data_package_id = make_data_package_string(state->syntax_model, required_string_package_id);
			constant_description_id = make_constant_description_constant_description(state->syntax_model, const_char_ptr, data_package_id);
			*e = make_primary_expression_constant_description(state->syntax_model, constant_description_id);
			return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token *)0, STRING_LITERAL_LIST);
		}else{
			unsigned_char_list_destroy(&chars);
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		unsigned_char_list_destroy(&chars);
		return (struct parser_node *)0;
	}
}

struct parser_node * primary_expression(struct parser_state * state, struct primary_expression_id * primary_expression_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct identifier_id identifier_id;
	struct scope_guid_id identifier_origin_scope;
	parser_progress("Attempting to build primary_expression\n");
	if((n1 = get_non_typedef_identifier_node(state, &identifier_origin_scope, &identifier_id))){
		*primary_expression_id = make_primary_expression_identifier(state->syntax_model, identifier_id, identifier_origin_scope);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(CONSTANT_HEXADECIMAL, state))){
		struct constant_description_id id = parse_integer_constant(state, n1->c_lexer_token, 16, 2);
		*primary_expression_id = make_primary_expression_constant_description(state->syntax_model, id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(CONSTANT_OCTAL, state))){
		struct constant_description_id id = parse_integer_constant(state, n1->c_lexer_token, 8, 1);
		*primary_expression_id = make_primary_expression_constant_description(state->syntax_model, id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(CONSTANT_DECIMAL, state))){
		struct constant_description_id id = parse_integer_constant(state, n1->c_lexer_token, 10, 0);
		*primary_expression_id = make_primary_expression_constant_description(state->syntax_model, id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(CONSTANT_EXPONENT, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(CONSTANT_FLOAT_SMALL, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(CONSTANT_FLOAT_LARGE, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(CONSTANT_CHARACTER, state))){
		unsigned int c;
		unsigned int done;
		struct constant_description_id constant_description_id;
		struct general_type_id const_char = make_const_char_general_type(state);
		struct word_package_id word_package_id;
		struct data_package_id data_package_id;
		convert_character_constant(n1->c_lexer_token->first_byte + 1, n1->c_lexer_token->last_byte, &c, &done);
		word_package_id = make_word_package_word(state->syntax_model, c);
		data_package_id = make_data_package_word(state->syntax_model, word_package_id);
		constant_description_id = make_constant_description_constant_description(state->syntax_model, const_char, data_package_id);
		*primary_expression_id = make_primary_expression_constant_description(state->syntax_model, constant_description_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = string_literal_list(state, primary_expression_id))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		struct expression_id e1;
		if((n1->next = expression(state, &e1))) {
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				*primary_expression_id = make_primary_expression_expression(state->syntax_model, e1);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
			}else{
				add_parser_error(state, PARSER_ERROR_PRIMARY_EXPRESSION_MISSING_CLOSE_PAREN);
				return (struct parser_node *)0;
			}
		}else{
			parser_progress("Expected EXPRESSION...backtracking...\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected primary expression\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * make_epsilon(struct parser_state * state){
	parser_progress("Attempting to build epsilon\n");
	return create_parser_node(state, (struct parser_node *)0, (struct parser_node *)0, (struct c_lexer_token *)0, EPSILON);
}

struct parser_node * argument_expression_list_rest(struct parser_state * state, struct any_statement_list_item_id prev, struct any_statement_list_item_id * list){
	struct parser_node * n1;
	parser_progress("Attempting to build argument_expression_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		struct expression_id e1;
		if((n1->next = assignment_expression(state, &e1))){
			struct code_statement_id code_statement_id = make_code_statement_expression_statement(state->syntax_model, e1);
			struct any_statement_id any_statement_id = make_any_statement_code_statement(state->syntax_model, code_statement_id);
			*list = make_any_statement_list_item_any_statement(state->syntax_model, prev, any_statement_id);
			if((n1->next->next = argument_expression_list_rest(state, *list, list))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, ARGUMENT_EXPRESSION_LIST_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_REST_MISSING_ARGUMENT);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_REST_MISSING_ASSIGNMENT_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*list = prev;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token *)0, ARGUMENT_EXPRESSION_LIST_REST);
	}
}

struct parser_node * argument_expression_list(struct parser_state * state, struct any_statement_list_item_id * list){
	struct parser_node * n1;
	struct expression_id e1;
	struct any_statement_list_item_id start = make_any_statement_list_item_list_start(state->syntax_model);
	parser_progress("Attempting to build argument_expression_list\n");
	if((n1 = assignment_expression(state, &e1))){
		struct code_statement_id code_statement_id = make_code_statement_expression_statement(state->syntax_model, e1);
		struct any_statement_id any_statement_id = make_any_statement_code_statement(state->syntax_model, code_statement_id);
		*list = make_any_statement_list_item_any_statement(state->syntax_model, start, any_statement_id);
		if((n1->next = argument_expression_list_rest(state, *list, list))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, ARGUMENT_EXPRESSION_LIST);
		}else{
			add_parser_error(state, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_MISSING_ARGUMENT);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected a assignment_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * postfix_expression_rest(struct parser_state * state, struct postfix_expression_part_list_item_id prev_postfix_expression_part_list_item_id, struct postfix_expression_part_list_item_id * this_postfix_expression_part_list_item_id){
	struct parser_node * n1;
	parser_progress("Attempting to build postfix_expression_rest\n");
	if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, state))){
		struct expression_id e1;
		if((n1->next = expression(state, &e1))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
				*this_postfix_expression_part_list_item_id = make_postfix_expression_part_list_item_postfix_expression_part(state->syntax_model, prev_postfix_expression_part_list_item_id, make_postfix_expression_part_array_dereference(state->syntax_model, e1));
				if((n1->next->next->next = postfix_expression_rest(state, *this_postfix_expression_part_list_item_id, this_postfix_expression_part_list_item_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, POSTFIX_EXPRESSION_REST);
				}else{
					add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_NO_REST);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_EXPRESSION_INCOMPLETE);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_EXPRESSION_MISSING);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		struct any_statement_list_item_id any_statement_list_item_id = make_any_statement_list_item_list_start(state->syntax_model);
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, state))){
			*this_postfix_expression_part_list_item_id = make_postfix_expression_part_list_item_postfix_expression_part(state->syntax_model, prev_postfix_expression_part_list_item_id, make_postfix_expression_part_function_dereference(state->syntax_model, any_statement_list_item_id));
			if((n1->next->next = postfix_expression_rest(state, *this_postfix_expression_part_list_item_id, this_postfix_expression_part_list_item_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, POSTFIX_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_0_ARGS_NO_REST);
				return (struct parser_node *)0;
			}
		}else if((n1->next = argument_expression_list(state, &any_statement_list_item_id))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				*this_postfix_expression_part_list_item_id = make_postfix_expression_part_list_item_postfix_expression_part(state->syntax_model, prev_postfix_expression_part_list_item_id, make_postfix_expression_part_function_dereference(state->syntax_model, any_statement_list_item_id));
				if((n1->next->next->next = postfix_expression_rest(state, *this_postfix_expression_part_list_item_id, this_postfix_expression_part_list_item_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, POSTFIX_EXPRESSION_REST);
				}else{
					add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_SOME_ARGS_NO_REST);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_SOME_ARGS_NO_CLOSE);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_0_ARGS_NO_CLOSE);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DOT_CHAR, state))){
		struct identifier_id identifier_id;
		if((n1->next = get_any_identifier_node(state, &identifier_id))){
			*this_postfix_expression_part_list_item_id = make_postfix_expression_part_list_item_postfix_expression_part(state->syntax_model, prev_postfix_expression_part_list_item_id, make_postfix_expression_part_struct_dot_dereference(state->syntax_model, identifier_id));
			if((n1->next->next = postfix_expression_rest(state, *this_postfix_expression_part_list_item_id, this_postfix_expression_part_list_item_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, POSTFIX_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_DOT_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_DOT_NO_IDENTIFIER);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(PTR_OP, state))){
		struct identifier_id identifier_id;
		if((n1->next = get_any_identifier_node(state, &identifier_id))){
			*this_postfix_expression_part_list_item_id = make_postfix_expression_part_list_item_postfix_expression_part(state->syntax_model, prev_postfix_expression_part_list_item_id, make_postfix_expression_part_struct_arrow_dereference(state->syntax_model, identifier_id));
			if((n1->next->next = postfix_expression_rest(state, *this_postfix_expression_part_list_item_id, this_postfix_expression_part_list_item_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, POSTFIX_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_PTR_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_PTR_NO_IDENTIFIER);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(INC_OP, state))){
		*this_postfix_expression_part_list_item_id = make_postfix_expression_part_list_item_postfix_expression_part(state->syntax_model, prev_postfix_expression_part_list_item_id, make_postfix_expression_part_inc(state->syntax_model));
		if((n1->next = postfix_expression_rest(state, *this_postfix_expression_part_list_item_id, this_postfix_expression_part_list_item_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, POSTFIX_EXPRESSION_REST);
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_INC_NO_REST);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DEC_OP, state))){
		*this_postfix_expression_part_list_item_id = make_postfix_expression_part_list_item_postfix_expression_part(state->syntax_model, prev_postfix_expression_part_list_item_id, make_postfix_expression_part_dec(state->syntax_model));
		if((n1->next = postfix_expression_rest(state, *this_postfix_expression_part_list_item_id, this_postfix_expression_part_list_item_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POSTFIX_EXPRESSION_REST);
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_DEC_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_postfix_expression_part_list_item_id = prev_postfix_expression_part_list_item_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, POSTFIX_EXPRESSION_REST);
	}
}

struct parser_node * postfix_expression(struct parser_state * state, struct postfix_expression_id * postfix_expression_id){
	struct primary_expression_id primary_expression_id;
	struct parser_node * n1;
	parser_progress("Attempting to build postfix_expression\n");
	if((n1 = primary_expression(state, &primary_expression_id))){
		struct postfix_expression_part_list_item_id postfix_expression_part_list_item_id;
		if((n1->next = postfix_expression_rest(state, make_postfix_expression_part_list_item_list_start(state->syntax_model), &postfix_expression_part_list_item_id))){
			*postfix_expression_id = make_postfix_expression_postfix_expression(state->syntax_model, primary_expression_id, postfix_expression_part_list_item_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POSTFIX_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected postfix expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * unary_expression(struct parser_state * state, struct unary_expression_id * unary_expression_id){
	struct parser_node * n1;
	struct postfix_expression_id postfix_expression_id;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build unary_expression\n");
	if((n1 = postfix_expression(state, &postfix_expression_id))){
		*unary_expression_id = make_unary_expression_postfix_expression(state->syntax_model, postfix_expression_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
	}else if((n1 = p_accept(INC_OP, state))){
		struct unary_expression_id sub_id;
		if((n1->next = unary_expression(state, &sub_id))){
			*unary_expression_id = make_unary_expression_increment(state->syntax_model, make_expression_cast_expression(state->syntax_model, make_cast_expression_no_cast(state->syntax_model, sub_id)));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_AFTER_INC);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DEC_OP, state))){
		struct unary_expression_id sub_id;
		if((n1->next = unary_expression(state, &sub_id))){
			*unary_expression_id = make_unary_expression_decrement(state->syntax_model, make_expression_cast_expression(state->syntax_model, make_cast_expression_no_cast(state->syntax_model, sub_id)));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_AFTER_DEC);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(AMPERSAND_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			*unary_expression_id = make_unary_expression_address_of(state->syntax_model, make_expression_cast_expression(state->syntax_model, cast_expression_id));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(MULTIPLY_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			*unary_expression_id = make_unary_expression_point_to(state->syntax_model, make_expression_cast_expression(state->syntax_model, cast_expression_id));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(PLUS_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			*unary_expression_id = make_unary_expression_plus(state->syntax_model, make_expression_cast_expression(state->syntax_model, cast_expression_id));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(MINUS_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			*unary_expression_id = make_unary_expression_minus(state->syntax_model, make_expression_cast_expression(state->syntax_model, cast_expression_id));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(TILDE_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			*unary_expression_id = make_unary_expression_bitwise_not(state->syntax_model, make_expression_cast_expression(state->syntax_model, cast_expression_id));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(EXCLAMATION_MARK_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			*unary_expression_id = make_unary_expression_logical_not(state->syntax_model, make_expression_cast_expression(state->syntax_model, cast_expression_id));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(SIZEOF, state))){
		struct unary_expression_id sub_id;
		if((n1->next = p_accept(OPEN_PAREN_CHAR, state))){
			struct general_type_id general_type_id;
			if((n1->next->next = type_name(state, &general_type_id))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					*unary_expression_id = make_unary_expression_sizeof_type(state->syntax_model, general_type_id);
					return (struct parser_node *)create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
				}else{
					add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_PAREN);
					return (struct parser_node *)0;
				}
			}else if((n1->next->next = unary_expression(state, &sub_id))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					*unary_expression_id = make_unary_expression_sizeof_expression(state->syntax_model, make_expression_cast_expression(state->syntax_model, make_cast_expression_no_cast(state->syntax_model, sub_id)));
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
				}else{
					add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_PAREN);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_TYPE);
				return (struct parser_node *)0;
			}
		}else if((n1->next = unary_expression(state, &sub_id))){
			*unary_expression_id = make_unary_expression_sizeof_expression(state->syntax_model, make_expression_cast_expression(state->syntax_model, make_cast_expression_no_cast(state->syntax_model, sub_id)));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else {
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_BAD_SIZEOF);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected unary expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * cast_expression(struct parser_state * state, struct cast_expression_id * cast_expression_id){
	struct parser_node * n1;
	struct unary_expression_id unary_expression_id;
	parser_progress("Attempting to build cast_expression\n");
	if((n1 = unary_expression(state, &unary_expression_id))){
		*cast_expression_id = make_cast_expression_no_cast(state->syntax_model, unary_expression_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, CAST_EXPRESSION);
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		struct general_type_id general_type_id;
		if((n1->next = type_name(state, &general_type_id))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				struct cast_expression_id sub_id;
				if((n1->next->next->next = cast_expression(state, &sub_id))){
					*cast_expression_id = make_cast_expression_has_cast(state->syntax_model, sub_id, general_type_id);
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, CAST_EXPRESSION);
				}else{
					add_parser_error(state, PARSER_ERROR_CAST_EXPRESSION_MISSING_CAST);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_CAST_EXPRESSION_MISSING_CLOSE_PAREN);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_CAST_EXPRESSION_MISSING_TYPE_NAME);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected unary expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * multiplicative_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build multiplicative_expression_rest\n");
	if((n1 = p_accept(MULTIPLY_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			struct expression_id right_expression = make_expression_cast_expression(state->syntax_model, cast_expression_id);
			struct binary_expression_id binary = make_binary_expression_multiply(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = multiplicative_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, MULTIPLICATIVE_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DIVIDE_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			struct expression_id right_expression = make_expression_cast_expression(state->syntax_model, cast_expression_id);
			struct binary_expression_id binary = make_binary_expression_divide(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = multiplicative_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, MULTIPLICATIVE_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(PERCENT_CHAR, state))){
		struct cast_expression_id cast_expression_id;
		if((n1->next = cast_expression(state, &cast_expression_id))){
			struct expression_id right_expression = make_expression_cast_expression(state->syntax_model, cast_expression_id);
			struct binary_expression_id binary = make_binary_expression_modulus(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = multiplicative_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, MULTIPLICATIVE_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, MULTIPLICATIVE_EXPRESSION_REST);
	}
}

struct parser_node * multiplicative_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct cast_expression_id cast_expression_id;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build multiplicative_expression\n");
	if((n1 = cast_expression(state, &cast_expression_id))){
		struct expression_id left_expression = make_expression_cast_expression(state->syntax_model, cast_expression_id);
		if((n1->next = multiplicative_expression_rest(state, left_expression, result_expression))){
			
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, MULTIPLICATIVE_EXPRESSION);
		}else{
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected cast expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * additive_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(PLUS_CHAR, state))){
		struct expression_id right_expression;
		if((n1->next = multiplicative_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_plus(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = additive_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ADDITIVE_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_ADDITIVE_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_ADDITIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(MINUS_CHAR, state))){
		struct expression_id right_expression;
		if((n1->next = multiplicative_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_minus(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = additive_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ADDITIVE_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_ADDITIVE_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_ADDITIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, ADDITIVE_EXPRESSION_REST);
	}
}

struct parser_node * additive_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build additive_expression\n");
	if((n1 = multiplicative_expression(state, &expression_id))){
		if((n1->next = additive_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ADDITIVE_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_ADDITIVE_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected multiplicative_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * shift_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(LEFT_OP, state))){
		struct expression_id right_expression;
		if((n1->next = additive_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_left(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = shift_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SHIFT_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_SHIFT_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_SHIFT_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(RIGHT_OP, state))){
		struct expression_id right_expression;
		if((n1->next = additive_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_right(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = shift_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SHIFT_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_SHIFT_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_SHIFT_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, SHIFT_EXPRESSION_REST);
	}
}

struct parser_node * shift_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build shift_expression\n");
	if((n1 = additive_expression(state, &expression_id))){
		if((n1->next = shift_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SHIFT_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_SHIFT_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected additive_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * relational_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(OPEN_ANGLE_BRACKET_CHAR, state))){
		struct expression_id right_expression;
		if((n1->next = shift_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_lt(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = relational_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, RELATIONAL_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(CLOSE_ANGLE_BRACKET_CHAR, state))){
		struct expression_id right_expression;
		if((n1->next = shift_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_gt(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = relational_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, RELATIONAL_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(LE_OP, state))){
		struct expression_id right_expression;
		if((n1->next = shift_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_le(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = relational_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, RELATIONAL_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(GE_OP, state))){
		struct expression_id right_expression;
		if((n1->next = shift_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_ge(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = relational_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, RELATIONAL_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, RELATIONAL_EXPRESSION_REST);
	}
}

struct parser_node * relational_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build relational_expression\n");
	if((n1 = shift_expression(state, &expression_id))){
		if((n1->next = relational_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, RELATIONAL_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected shift_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * equality_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(EQ_OP, state))){
		struct expression_id right_expression;
		if((n1->next = relational_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_eq(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = equality_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EQUALITY_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_EQUALITY_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_EQUALITY_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(NE_OP, state))){
		struct expression_id right_expression;
		if((n1->next = relational_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_ne(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = equality_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EQUALITY_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_EQUALITY_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_EQUALITY_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, EQUALITY_EXPRESSION_REST);
	}
}

struct parser_node * equality_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build equality_expression\n");
	if((n1 = relational_expression(state, &expression_id))){
		if((n1->next = equality_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EQUALITY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_EQUALITY_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected relational_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * and_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(AMPERSAND_CHAR, state))){
		struct expression_id right_expression;
		if((n1->next = equality_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_bitwise_and(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = and_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, AND_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_AND_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_AND_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, AND_EXPRESSION_REST);
	}
}

struct parser_node * and_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build and_expression\n");
	if((n1 = equality_expression(state, &expression_id))){
		if((n1->next = and_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, AND_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_AND_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected equality_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * exclusive_or_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(CARET_CHAR, state))){
		struct expression_id right_expression;
		if((n1->next = and_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_exclusive_or(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = exclusive_or_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXCLUSIVE_OR_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, EXCLUSIVE_OR_EXPRESSION_REST);
	}
}

struct parser_node * exclusive_or_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build exclusive_or_expression\n");
	if((n1 = and_expression(state, &expression_id))){
		if((n1->next = exclusive_or_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXCLUSIVE_OR_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected and_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * inclusive_or_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(PIPE_CHAR, state))){
		struct expression_id right_expression;
		if((n1->next = exclusive_or_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_bitwise_or(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = inclusive_or_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INCLUSIVE_OR_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, INCLUSIVE_OR_EXPRESSION_REST);
	}
}

struct parser_node * inclusive_or_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build inclusive_or_expression\n");
	if((n1 = exclusive_or_expression(state, &expression_id))){
		if((n1->next = inclusive_or_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INCLUSIVE_OR_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected exclusive_or_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * logical_and_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(AND_OP, state))){
		struct expression_id right_expression;
		if((n1->next = inclusive_or_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_logical_and(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = logical_and_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LOGICAL_AND_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_LOGICAL_AND_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_LOGICAL_AND_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, LOGICAL_AND_EXPRESSION_REST);
	}
}

struct parser_node * logical_and_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build logical_and_expression\n");
	if((n1 = inclusive_or_expression(state, &expression_id))){
		if((n1->next = logical_and_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LOGICAL_AND_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_LOGICAL_AND_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected inclusive_or_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * logical_or_expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	if((n1 = p_accept(OR_OP, state))){
		struct expression_id right_expression;
		if((n1->next = logical_and_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_logical_or(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = logical_or_expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LOGICAL_OR_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_LOGICAL_OR_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_LOGICAL_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, LOGICAL_OR_EXPRESSION_REST);
	}
}

struct parser_node * logical_or_expression(struct parser_state * state, struct expression_id * result_expression){
	struct parser_node * n1;
	struct expression_id expression_id;
	parser_progress("Attempting to build logical_or_expression\n");
	if((n1 = logical_and_expression(state, &expression_id))){
		if((n1->next = logical_or_expression_rest(state, expression_id, result_expression))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LOGICAL_OR_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_LOGICAL_OR_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected logical_and_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * conditional_expression(struct parser_state * state, struct expression_id * result){
	struct parser_node * n1;
	struct expression_id e1;
	struct expression_id e2;
	struct expression_id e3;
	parser_progress("Attempting to build conditional_expression\n");
	if((n1 = logical_or_expression(state, &e1))){
		if((n1->next = p_accept(QUESTION_MARK_CHAR, state))){
			if((n1->next->next = expression(state, &e2))){
				if((n1->next->next->next = p_accept(COLON_CHAR, state))){
					if((n1->next->next->next->next = conditional_expression(state, &e3))){
						*result = make_expression_ternary_expression(state->syntax_model, make_ternary_expression_ternary_operator(state->syntax_model, e1, e2, e3));
						return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, CONDITIONAL_EXPRESSION);
					}else{
						add_parser_error(state, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_FALSE);
						return (struct parser_node *)0;
					}
				}else{
					add_parser_error(state, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_COLON);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_TRUE);
				return (struct parser_node *)0;
			}
		}else{
			*result = e1;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, CONDITIONAL_EXPRESSION);
		}
	}else{
		parser_progress("Expected logical_or_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * assignment_expression(struct parser_state * state, struct expression_id * result){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct unary_expression_id unary;
	parser_progress("Attempting to build assignment_expression\n");

	if((n1 = unary_expression(state, &unary))){
		struct expression_id e1 = make_expression_cast_expression(state->syntax_model, make_cast_expression_no_cast(state->syntax_model, unary));
		struct expression_id e2;
		if((n1->next = p_accept(EQUALS_CHAR, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(MUL_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_mul_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(DIV_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_div_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(MOD_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_mod_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(ADD_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_add_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(SUB_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_sub_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(LEFT_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_left_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(RIGHT_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_right_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(AND_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_and_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(XOR_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_xor_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else if((n1->next = p_accept(OR_ASSIGN, state))){
			if((n1->next->next = assignment_expression(state, &e2))){	
				*result = make_expression_binary_expression(state->syntax_model, make_binary_expression_or_assign(state->syntax_model, e1, e2));
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(state, checkpoint);
			}
		}else{
			parser_progress("Expected assignment operator, putting tokens back.\n");
			backtrack(state, checkpoint);
		}
	}

	if((n1 = conditional_expression(state, result))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
	}else{
		parser_progress("Expected conditional_expression.\n");
		backtrack(state, checkpoint);
	}
	return (struct parser_node *)0;
}

struct parser_node * expression_rest(struct parser_state * state, struct expression_id left_expression, struct expression_id * this_expression_id){
	struct parser_node * n1;
	parser_progress("Attempting to build expression_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		struct expression_id right_expression;
		if((n1->next = assignment_expression(state, &right_expression))){
			struct binary_expression_id binary = make_binary_expression_comma(state->syntax_model, left_expression, right_expression);
			struct expression_id wrapped_expression = make_expression_binary_expression(state->syntax_model, binary);
			if((n1->next->next = expression_rest(state, wrapped_expression, this_expression_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_EXPRESSION_REST_NO_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_EXPRESSION_REST_NO_ASSIGNMENT_EXPRESSION);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_expression_id = left_expression;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, EXPRESSION_REST);
	}
}

struct parser_node * expression(struct parser_state * state, struct expression_id * result){
	struct parser_node * n1;
	struct expression_id left_expression;
	parser_progress("Attempting to build expression\n");
	if((n1 = assignment_expression(state, &left_expression))){
		if((n1->next = expression_rest(state, left_expression, result))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * constant_expression(struct parser_state * state, struct expression_id * result){
	struct parser_node * n1;
	parser_progress("Attempting to build constant_expression\n");
	if((n1 = conditional_expression(state, result))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, CONSTANT_EXPRESSION);
	}else{
		parser_progress("Expected expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * expression_statement(struct parser_state * state, struct expression_id * expression_id){
	struct parser_node * n1;
	struct parser_node * n2;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build expression_statement\n");

	if((n1 = p_accept(SEMICOLON_CHAR, state))){
		*expression_id = make_expression_nullary_expression(state->syntax_model);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXPRESSION_STATEMENT);
	}else if((n1 = expression(state, expression_id))){
		if((n2 = p_accept(SEMICOLON_CHAR, state))){
			n1->next = n2;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXPRESSION_STATEMENT);
		}else{
			parser_progress("Expected semicolon in expression statement.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected semicolon or expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * statement(struct parser_state * state, unsigned int scope_already_created, struct any_statement_list_item_id prev, struct any_statement_list_item_id * result){
	struct parser_node * n1;
	struct expression_id expression_id;
	struct jump_statement_id jump_statement_id;
	struct labeled_statement_id labeled_statement_id;
	struct code_statement_id code_statement_id;
	struct function_definition_helper h;
	h.for_function_definition = 0;
	parser_progress("Attempting to build statement\n");
	if((n1 = labeled_statement(state, &labeled_statement_id))){
		struct code_statement_id lc = make_code_statement_labeled_statement(state->syntax_model, labeled_statement_id);
		struct any_statement_id any_statement_id = make_any_statement_code_statement(state->syntax_model, lc);
		*result = make_any_statement_list_item_any_statement(state->syntax_model, prev, any_statement_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = compound_statement(state, &h, scope_already_created, result))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = expression_statement(state, &expression_id))){
		struct code_statement_id ce = make_code_statement_expression_statement(state->syntax_model, expression_id);
		struct any_statement_id any_statement_id = make_any_statement_code_statement(state->syntax_model, ce);
		*result = make_any_statement_list_item_any_statement(state->syntax_model, prev, any_statement_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = selection_statement(state, &code_statement_id))){
		struct any_statement_id any_statement_id = make_any_statement_code_statement(state->syntax_model, code_statement_id);
		*result = make_any_statement_list_item_any_statement(state->syntax_model, prev, any_statement_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = iteration_statement(state, &code_statement_id))){
		struct any_statement_id any_statement_id = make_any_statement_code_statement(state->syntax_model, code_statement_id);
		*result = make_any_statement_list_item_any_statement(state->syntax_model, prev, any_statement_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = jump_statement(state, &jump_statement_id))){
		struct code_statement_id cj = make_code_statement_jump_statement(state->syntax_model, jump_statement_id);
		struct any_statement_id any_statement_id = make_any_statement_code_statement(state->syntax_model, cj);
		*result = make_any_statement_list_item_any_statement(state->syntax_model, prev, any_statement_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else{
		parser_progress("Expected statement.\n");
		*result = prev;
		return (struct parser_node *)0;
	}
}

struct parser_node * statement_list_rest(struct parser_state * state, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build statement_list_rest\n");
	if((n1 = statement(state, 0, prev_any_statement_list_item_id, result_statement_list))){
		if((n1->next = statement_list_rest(state, *result_statement_list, result_statement_list))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT_LIST_REST);
		}else{
			parser_progress("Expected statement_list_rest.\n");
			backtrack(state, checkpoint);
			*result_statement_list = prev_any_statement_list_item_id;
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*result_statement_list = prev_any_statement_list_item_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, STATEMENT_LIST_REST);
	}
}

struct parser_node * statement_list(struct parser_state * state, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build statement_list\n");
	if((n1 = statement(state, 0, prev_any_statement_list_item_id, result_statement_list))){
		if((n1->next = statement_list_rest(state, *result_statement_list, result_statement_list))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT_LIST);
		}else{
			parser_progress("Expected statement_list_rest.\n");
			backtrack(state, checkpoint);
			*result_statement_list = prev_any_statement_list_item_id;
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected statement.\n");
		*result_statement_list = prev_any_statement_list_item_id;
		return (struct parser_node *)0;
	}
}

struct parser_node * compound_statement(struct parser_state * state, struct function_definition_helper * h, unsigned int scope_already_created, struct any_statement_list_item_id * result){
	struct parser_node * n1;
	struct any_statement_list_item_id start = make_any_statement_list_item_list_start(state->syntax_model);
	parser_progress("Attempting to build compound_statement\n");
	if((n1 = p_accept(OPEN_BRACE_CHAR, state))){
		if(h->for_function_definition){
			struct scope_guid_details * scope_guids_ptr = (struct scope_guid_details *)struct_scope_guid_details_list_data(&state->syntax_model->scope_guids);
			/*  We're already inside the scope of the function, so go back to the parent scope of the function. */
			struct scope_guid_id file_scope = scope_guids_ptr[get_current_scope_guid(state).id].parent;
			assert(scope_guids_ptr[file_scope.id].type == ASM_SCOPE_GUID_FILE_SCOPE);
			make_identifier_namespace_entry_declaration_with_default_order(state->syntax_model, h->identifier_id, file_scope, h->general_type_id);
		}
		if(!scope_already_created) { increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE); }
		if((n1->next = statement_list(state, start, result))){
			if((n1->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
				if(!scope_already_created) { decrement_scope_depth(state); }
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, COMPOUND_STATEMENT);
			}else{
				add_parser_error(state, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_1);
				return (struct parser_node *)0;
			}
		}else if((n1->next = declaration_list(state, start, result))){
			if((n1->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
				if(!scope_already_created) { decrement_scope_depth(state); }
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, COMPOUND_STATEMENT);
			}else if((n1->next->next = statement_list(state, *result, result))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
					if(!scope_already_created) { decrement_scope_depth(state); }
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, COMPOUND_STATEMENT);
				}else{
					add_parser_error(state, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_2);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_3);
				return (struct parser_node *)0;
			}
		}else if((n1->next = p_accept(CLOSE_BRACE_CHAR, state))){
			if(!scope_already_created) { decrement_scope_depth(state); }
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, COMPOUND_STATEMENT);
		}else{
			add_parser_error(state, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_4);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected compound_statement.\n");
		*result = start;
		return (struct parser_node *)0;
	}
}


struct parser_node * type_qualifier(struct parser_state * state, struct simple_type_qualifier_id * asm_id){
	struct parser_node * n1;
	parser_progress("Attempting to build type_qualifier\n");
	if((n1 = p_accept(CONST, state))){
		*asm_id = make_simple_type_qualifier_const(state->syntax_model);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_QUALIFIER);
	}else if((n1 = p_accept(VOLATILE, state))){
		*asm_id = make_simple_type_qualifier_volatile(state->syntax_model);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_QUALIFIER);
	}else {
		parser_progress("Expected type_qualifier.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * simple_storage_class_specifier(struct parser_state * state, struct simple_storage_class_specifier_id * asm_id){
	struct parser_node * n1;
	parser_progress("Attempting to build simple_storage_class_specifier\n");
	if((n1 = p_accept(TYPEDEF, state))){
		*asm_id = make_simple_storage_class_specifier_typedef(state->syntax_model);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(EXTERN, state))){
		*asm_id = make_simple_storage_class_specifier_extern(state->syntax_model);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(STATIC, state))){
		*asm_id = make_simple_storage_class_specifier_static(state->syntax_model);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(AUTO, state))){
		*asm_id = make_simple_storage_class_specifier_auto(state->syntax_model);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(REGISTER, state))){
		*asm_id = make_simple_storage_class_specifier_register(state->syntax_model);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else {
		parser_progress("Expected type_qualifier.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * enumerator(struct parser_state * state, struct enum_member_id * enum_member_id, struct scoped_tag_specifier_id scoped_tag_specifier_id){
	struct parser_node * n1;
	struct identifier_id identifier_id;
	parser_progress("Attempting to build enumerator\n");
	if((n1 = get_any_identifier_node(state, &identifier_id))){
		if((n1->next = p_accept(EQUALS_CHAR, state))){
			struct expression_id e1;
			if((n1->next->next = constant_expression(state, &e1))){
				declare_enum_member(state, identifier_id, scoped_tag_specifier_id);
				*enum_member_id = make_enum_member_has_expression(state->syntax_model, identifier_id, e1);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUMERATOR);
			}else{
				add_parser_error(state, PARSER_ERROR_ENUMERATOR_MISSING_CONSTANT_EXPRESSION);
				return (struct parser_node *)0;
			}
		}else{
			declare_enum_member(state, identifier_id, scoped_tag_specifier_id);
			*enum_member_id = make_enum_member_no_expression(state->syntax_model, identifier_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUMERATOR);
		}
	}else{
		buffered_puts(state->buffered_output, "Required identifier.  Cannot continue.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * enumerator_list_rest(struct parser_state * state, struct enum_member_list_item_id prev_item, struct enum_member_list_item_id * this_list_id, struct scoped_tag_specifier_id scoped_tag_specifier_id){
	struct parser_node * n1;
	parser_progress("Attempting to build enumerator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		struct enum_member_id enum_member_id;
		if((n1->next = enumerator(state, &enum_member_id, scoped_tag_specifier_id))){
			*this_list_id = make_enum_member_list_item_member(state->syntax_model, prev_item, enum_member_id);
			if((n1->next->next = enumerator_list_rest(state, *this_list_id, this_list_id, scoped_tag_specifier_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUMERATOR_LIST_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_ENUMERATOR_LIST_REST_MISSING_REST);
				*this_list_id = prev_item;
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_ENUMERATOR_LIST_REST_MISSING_ENUMERATOR);
			*this_list_id = prev_item;
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_item;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, ENUMERATOR_LIST_REST);
	}
}

struct parser_node * enumerator_list(struct parser_state * state, struct enum_member_list_item_id prev_item, struct enum_member_list_item_id * this_list_id, struct scoped_tag_specifier_id scoped_tag_specifier_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct enum_member_id enum_member_id;
	parser_progress("Attempting to build enumerator_list\n");
	if((n1 = enumerator(state, &enum_member_id, scoped_tag_specifier_id))){
		*this_list_id = make_enum_member_list_item_member(state->syntax_model, prev_item, enum_member_id);
		if((n1->next = enumerator_list_rest(state, *this_list_id, this_list_id, scoped_tag_specifier_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUMERATOR_LIST);
		}else{
			add_parser_error(state, PARSER_ERROR_ENUMERATOR_LIST_MISSING_REST);
			backtrack(state, checkpoint);
			*this_list_id = prev_item;
			return (struct parser_node *)0;
		}
	}else{
		buffered_puts(state->buffered_output,"Required enumerator.  Cannot continue.\n");
		*this_list_id = prev_item;
		return (struct parser_node *)0;
	}
}

struct scope_level * get_parser_scope_level(struct asm_state * asm_state){
	(void)asm_state;
	return (struct scope_level *)0;
}

void declare_enum_member(struct parser_state * state, struct identifier_id identifier_id, struct scoped_tag_specifier_id scoped_tag_specifier_id){
	struct aggregate_type_specifier_id asm_id = make_aggregate_type_specifier_tag_specifier(state->syntax_model, scoped_tag_specifier_id);
	struct specifier_or_qualifier_list_item_id specifier_list_start = make_specifier_or_qualifier_list_item_list_start(state->syntax_model);
	struct specifier_or_qualifier_id specifier_or_qualifier_id = make_specifier_or_qualifier_aggregate_type_specifier(state->syntax_model, asm_id);
	struct specifier_or_qualifier_list_item_id specifier_list_id = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, specifier_list_start, specifier_or_qualifier_id);
	struct general_type_id the_enum_type = make_general_type_general_type(state->syntax_model, specifier_list_id, make_declarator_part_list_item_list_start(state->syntax_model), make_bitfield_no_bitfield(state->syntax_model));
	make_identifier_namespace_entry_declaration_with_default_order(state->syntax_model, identifier_id, get_current_scope_guid(state), the_enum_type);
}

struct parser_node * enum_specifier(struct parser_state * state, struct scoped_tag_specifier_id * scoped_tag_specifier_id){
	struct parser_node * n1;
	parser_progress("Attempting to build enum_specifier\n");
	if((n1 = p_accept(ENUM, state))){
		struct scope_guid_id current_scope = get_current_scope_guid(state); 
		struct identifier_id identifier_id;
		if((n1->next = p_accept(OPEN_BRACE_CHAR, state))){
			struct unscoped_tag_specifier_id anonymous_unscoped_tag_specifier_id;
			struct anonymous_entry_id anonymous_entry_id = make_anonymous_entry_anonymous_entry(state->syntax_model);
			struct unscoped_enum_specifier_id anonymous_id = make_unscoped_enum_specifier_anonymous(state->syntax_model, anonymous_entry_id);
			struct enum_member_list_item_id enum_member_list_item_id;
			anonymous_unscoped_tag_specifier_id = make_unscoped_tag_specifier_enum(state->syntax_model, anonymous_id);
			*scoped_tag_specifier_id = make_scoped_tag_specifier_scoped_tag_specifier(state->syntax_model, current_scope, anonymous_unscoped_tag_specifier_id);

			if((n1->next->next = enumerator_list(state, make_enum_member_list_item_list_start(state->syntax_model), &enum_member_list_item_id, *scoped_tag_specifier_id))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
					add_anonymous_enum_definition(state, enum_member_list_item_id, current_scope, anonymous_entry_id);
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUM_SPECIFIER);
				}else{
					add_parser_error(state, PARSER_ERROR_ENUM_SPECIFIER_MISSING_CLOSE_BRACE_ANON);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_ENUM_SPECIFIER_MISSING_ENUMERATOR_LIST_ANON);
				return (struct parser_node *)0;
			}
		}else if((n1->next = get_any_identifier_node(state, &identifier_id))){
			struct unscoped_enum_specifier_id non_anonymous_id = make_unscoped_enum_specifier_named(state->syntax_model, identifier_id);
			struct scope_guid_id enum_origin_scope;
			struct scope_guid_id closest_declared_scope;
			struct unscoped_tag_specifier_id named_unscoped_tag_specifier_id;
			struct enum_member_list_item_id enum_member_list_item_id;

			named_unscoped_tag_specifier_id = make_unscoped_tag_specifier_enum(state->syntax_model, non_anonymous_id);

			if(get_scope_of_closest_tag_declaration(state, &closest_declared_scope, current_scope, identifier_id)){
				enum_origin_scope = closest_declared_scope;
			}else{
				/*  If there is no closest one, then this must declare an incomplete type... but enums can't be pre-declared. */
				enum_origin_scope = current_scope;
			}

			*scoped_tag_specifier_id = make_scoped_tag_specifier_scoped_tag_specifier(state->syntax_model, enum_origin_scope, named_unscoped_tag_specifier_id);

			if((n1->next->next = p_accept(OPEN_BRACE_CHAR, state))){
				if((n1->next->next->next = enumerator_list(state, make_enum_member_list_item_list_start(state->syntax_model), &enum_member_list_item_id, *scoped_tag_specifier_id))){
					if((n1->next->next->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
						add_named_enum_definition(state, enum_member_list_item_id, current_scope, identifier_id);
						return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUM_SPECIFIER);
					} else {
						add_parser_error(state, PARSER_ERROR_ENUM_SPECIFIER_MISSING_CLOSE_BRACE_NAMED);
						return (struct parser_node *)0;
					}
				}else{
					add_parser_error(state, PARSER_ERROR_ENUM_SPECIFIER_MISSING_ENUMERATOR_LIST_NAMED);
					return (struct parser_node *)0;
				}
			}else{
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUM_SPECIFIER);
			}
		}else {
			add_parser_error(state, PARSER_ERROR_ENUM_SPECIFIER_MISSING_OPEN_BRACE_OR_IDENTIFIER);
			return (struct parser_node *)0;
		}
	}else {
		parser_progress("Expected ENUM.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * struct_or_union(struct parser_state * state, enum asm_unscoped_tag_specifier_kind * kind){
	struct parser_node * n1;
	parser_progress("Attempting to build struct_or_union\n");
	if((n1 = p_accept(STRUCT, state))){
		*kind = ASM_UNSCOPED_TAG_SPECIFIER_STRUCT;
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_OR_UNION);
	}else if((n1 = p_accept(UNION, state))){
		*kind = ASM_UNSCOPED_TAG_SPECIFIER_UNION;
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_OR_UNION);
	}else {
		parser_progress("Expected struct_or_union.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * type_specifier(struct parser_state * state, struct aggregate_type_specifier_id * asm_id){
	struct parser_node * n1;
	struct scoped_tag_specifier_id temp_id;
	struct identifier_id identifier_id;
	struct scope_guid_id typedef_origin_scope_id;
	parser_progress("Attempting to build type_specifier\n");
	if((n1 = p_accept(VOID, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_void(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(CHAR, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_char(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(SHORT, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_short(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(INT, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_int(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(LONG, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_long(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(FLOAT, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_float(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(DOUBLE, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_double(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(SIGNED, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_signed(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(UNSIGNED, state))){
		*asm_id = make_aggregate_type_specifier_simple_type_specifier(state->syntax_model, make_simple_type_specifier_unsigned(state->syntax_model));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = struct_or_union_specifier(state, &temp_id))){
		*asm_id = make_aggregate_type_specifier_tag_specifier(state->syntax_model, temp_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = enum_specifier(state, &temp_id))){
		*asm_id = make_aggregate_type_specifier_tag_specifier(state->syntax_model, temp_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = get_typedef_identifier_node(state, &typedef_origin_scope_id, &identifier_id))){
		struct scoped_typename_specifier_id id1 = make_scoped_typename_specifier_scoped_typename(state->syntax_model, typedef_origin_scope_id, identifier_id);
		*asm_id = make_aggregate_type_specifier_typename_specifier(state->syntax_model, id1);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else{
		parser_progress("Expected a type_specifier.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * declaration_specifiers(struct parser_state * state, struct specifier_or_qualifier_list_item_id previous_specifier_qualifier_list_id, struct specifier_or_qualifier_list_item_id * this_list_id){
	struct parser_node * n1;
	struct aggregate_type_specifier_id asm_type_specifier_id;
	struct simple_type_qualifier_id asm_type_qualifier_id;
	struct simple_storage_class_specifier_id asm_simple_storage_class_specifier_id;
	parser_progress("Attempting to build declaration_specifiers\n");
	if((n1 = simple_storage_class_specifier(state, &asm_simple_storage_class_specifier_id))){
		struct specifier_or_qualifier_list_item_id new_list_id = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, previous_specifier_qualifier_list_id, make_specifier_or_qualifier_simple_storage_class_specifier(state->syntax_model, asm_simple_storage_class_specifier_id));
		if((n1->next = declaration_specifiers(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}
	}else if((n1 = type_specifier(state, &asm_type_specifier_id))){
		struct specifier_or_qualifier_list_item_id new_list_id = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, previous_specifier_qualifier_list_id, make_specifier_or_qualifier_aggregate_type_specifier(state->syntax_model, asm_type_specifier_id));
		if((n1->next = declaration_specifiers(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}
	}else if((n1 = type_qualifier(state, &asm_type_qualifier_id))){
		struct specifier_or_qualifier_list_item_id new_list_id = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, previous_specifier_qualifier_list_id, make_specifier_or_qualifier_simple_type_qualifier(state->syntax_model, asm_type_qualifier_id));
		if((n1->next = declaration_specifiers(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}
	}else{
		*this_list_id = previous_specifier_qualifier_list_id;
		parser_progress("Expected a declaration_specifiers.\n");
		return (struct parser_node *)0;
	}
}

unsigned int specifier_or_qualifier_list_item_id_contains_typedef(struct parser_state * state, struct specifier_or_qualifier_list_item_id list_id){
	struct specifier_or_qualifier_list_item_details d1 = get_specifier_or_qualifier_list_item_details_by_id(state->syntax_model, list_id);

	switch(d1.type){
		case ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER:{
			struct specifier_or_qualifier_id specifier_id = d1.item_id;
			struct specifier_or_qualifier_list_item_id next = d1.prev_id;
			struct specifier_or_qualifier_details d2 = get_specifier_or_qualifier_details_by_id(state->syntax_model, specifier_id);
			if(d2.type == ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_STORAGE_CLASS_SPECIFIER){
				struct simple_storage_class_specifier_id simple_storage_class_specifier_id;
				struct simple_storage_class_specifier_details d3;
				simple_storage_class_specifier_id = get_simple_storage_class_specifier_id_simple_storage_class_specifier_from_specifier_or_qualifier(d2);
				d3 = get_simple_storage_class_specifier_details_by_id(state->syntax_model, simple_storage_class_specifier_id);
				if(d3.type == ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_TYPEDEF){
					return 1;
				}
			}
			/*  Look for one deeper... */
			return specifier_or_qualifier_list_item_id_contains_typedef(state, next);
		}case ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START:{
			return 0;
		}default:{
			assert(0);
			return 0;
		}
	}
}

struct parser_node * init_declarator_list_rest(struct parser_state * state, struct specifier_or_qualifier_list_item_id specifier_qualifier_list_id, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build init_declarator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		if((n1->next = init_declarator(state, specifier_qualifier_list_id, prev_any_statement_list_item_id, result_statement_list))){
			if((n1->next->next = init_declarator_list_rest(state, specifier_qualifier_list_id, *result_statement_list, result_statement_list))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INIT_DECLARATOR_LIST_REST);
			}else{
				parser_progress("Expected a init_declarator_list_rest.\n");
				backtrack(state, checkpoint);
				*result_statement_list = prev_any_statement_list_item_id;
				return (struct parser_node *)0;
			}
		}else{
			parser_progress("Expected a init_declarator.\n");
			backtrack(state, checkpoint);
			*result_statement_list = prev_any_statement_list_item_id;
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*result_statement_list = prev_any_statement_list_item_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, INIT_DECLARATOR_LIST_REST);
	}
}

struct parser_node * init_declarator_list(struct parser_state * state, struct specifier_or_qualifier_list_item_id specifier_qualifier_list_id, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build init_declarator_list\n");
	if((n1 = init_declarator(state, specifier_qualifier_list_id, prev_any_statement_list_item_id, result_statement_list))){
		if((n1->next = init_declarator_list_rest(state, specifier_qualifier_list_id, *result_statement_list, result_statement_list))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INIT_DECLARATOR_LIST);
		}else{
			parser_progress("Expected a init_declarator_list_rest.\n");
			*result_statement_list = prev_any_statement_list_item_id;
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected an init_declarator .\n");
		*result_statement_list = prev_any_statement_list_item_id;
		return (struct parser_node *)0;
	}
}

void make_tag_predeclarations_h(struct parser_state * state, struct scoped_tag_specifier_details scoped_tag_specifier_details){
	struct unscoped_tag_specifier_id unscoped_tag_specifier_id = scoped_tag_specifier_details.unscoped_tag_specifier_id;
	struct unscoped_tag_specifier_details unscoped_tag_specifier_details = get_unscoped_tag_specifier_details_by_id(state->syntax_model, unscoped_tag_specifier_id);
	switch(unscoped_tag_specifier_details.type){
		case ASM_UNSCOPED_TAG_SPECIFIER_STRUCT:{
			struct unscoped_struct_specifier_id unscoped_struct_specifier_id = get_unscoped_struct_specifier_id_struct_from_unscoped_tag_specifier(unscoped_tag_specifier_details);
			struct unscoped_struct_specifier_details unscoped_struct_specifier_details = get_unscoped_struct_specifier_details_by_id(state->syntax_model, unscoped_struct_specifier_id);
			if(unscoped_struct_specifier_details.type == ASM_UNSCOPED_STRUCT_SPECIFIER_NAMED){
				struct identifier_id identifier_id = get_identifier_id_named_from_unscoped_struct_specifier(unscoped_struct_specifier_details);
				make_named_tag_predeclaration_named_with_default_order(state->syntax_model, get_current_scope_guid(state), identifier_id, ASM_UNSCOPED_TAG_SPECIFIER_STRUCT);
			}
			break;
		}case ASM_UNSCOPED_TAG_SPECIFIER_UNION:{
			struct unscoped_union_specifier_id unscoped_union_specifier_id = get_unscoped_union_specifier_id_union_from_unscoped_tag_specifier(unscoped_tag_specifier_details);
			struct unscoped_union_specifier_details unscoped_union_specifier_details = get_unscoped_union_specifier_details_by_id(state->syntax_model, unscoped_union_specifier_id);
			if(unscoped_union_specifier_details.type == ASM_UNSCOPED_UNION_SPECIFIER_NAMED){
				struct identifier_id identifier_id = get_identifier_id_named_from_unscoped_union_specifier(unscoped_union_specifier_details);
				make_named_tag_predeclaration_named_with_default_order(state->syntax_model, get_current_scope_guid(state), identifier_id, ASM_UNSCOPED_TAG_SPECIFIER_UNION);
			}
			break;
		}case ASM_UNSCOPED_TAG_SPECIFIER_ENUM:{
			struct unscoped_enum_specifier_id unscoped_enum_specifier_id = get_unscoped_enum_specifier_id_enum_from_unscoped_tag_specifier(unscoped_tag_specifier_details);
			struct unscoped_enum_specifier_details unscoped_enum_specifier_details = get_unscoped_enum_specifier_details_by_id(state->syntax_model, unscoped_enum_specifier_id);
			/*  Forward-declaring enums is not allowed, but we'll catch that later in semantic analysis. */
			if(unscoped_enum_specifier_details.type == ASM_UNSCOPED_ENUM_SPECIFIER_NAMED){
				struct identifier_id identifier_id = get_identifier_id_named_from_unscoped_enum_specifier(unscoped_enum_specifier_details);
				make_named_tag_predeclaration_named_with_default_order(state->syntax_model, get_current_scope_guid(state), identifier_id, ASM_UNSCOPED_TAG_SPECIFIER_ENUM);
			}

			break;
		}default:{
			assert(0);
		}
	}
}

void make_tag_predeclarations(struct parser_state * state, struct specifier_or_qualifier_list_item_id specifiers_id){
	/*  Called on a specifier list when we encounter of the syntax like 'struct foo ;' with no declarator after the specifier list. */
	struct specifier_or_qualifier_list_item_details details = get_specifier_or_qualifier_list_item_details_by_id(state->syntax_model, specifiers_id);
	if(details.type == ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER){
		struct specifier_or_qualifier_list_item_id prev_id = get_specifier_or_qualifier_list_item_prev_id_specifier_or_qualifier_from_specifier_or_qualifier_list_item(details);
		struct specifier_or_qualifier_id specifier_id = details.item_id;
		struct specifier_or_qualifier_details specifier_details = get_specifier_or_qualifier_details_by_id(state->syntax_model, specifier_id);
		if(specifier_details.type == ASM_SPECIFIER_OR_QUALIFIER_AGGREGATE_TYPE_SPECIFIER){
			struct aggregate_type_specifier_id aggregate_type_specifier_id = get_aggregate_type_specifier_id_aggregate_type_specifier_from_specifier_or_qualifier(specifier_details); 
			struct aggregate_type_specifier_details aggregate_details = get_aggregate_type_specifier_details_by_id(state->syntax_model, aggregate_type_specifier_id);
			if(aggregate_details.type == ASM_AGGREGATE_TYPE_SPECIFIER_TAG_SPECIFIER){
				struct scoped_tag_specifier_id scoped_tag_specifier_id = get_scoped_tag_specifier_id_tag_specifier_from_aggregate_type_specifier(aggregate_details);
				struct scoped_tag_specifier_details scoped_tag_specifier_details = get_scoped_tag_specifier_details_by_id(state->syntax_model, scoped_tag_specifier_id);
				make_tag_predeclarations_h(state, scoped_tag_specifier_details);
			}
		}
		make_tag_predeclarations(state, prev_id);
	}
}

struct parser_node * declaration(struct parser_state * state, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct specifier_or_qualifier_list_item_id declaration_specifiers_list_id;
	parser_progress("Attempting to build declaration\n");

	if((n1 = declaration_specifiers(state, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), &declaration_specifiers_list_id))){
		if((n1->next = init_declarator_list(state, declaration_specifiers_list_id, prev_any_statement_list_item_id, result_statement_list))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION);
			}else{
				parser_progress("Expected a SEMICOLON_CHAR in declaration.\n");
				backtrack(state, checkpoint);
				*result_statement_list = prev_any_statement_list_item_id;
				return (struct parser_node *)0;
			}
		}else if((n1->next = p_accept(SEMICOLON_CHAR, state))){
			make_general_type_general_type(state->syntax_model, declaration_specifiers_list_id, make_declarator_part_list_item_list_start(state->syntax_model), make_bitfield_no_bitfield(state->syntax_model));
			*result_statement_list = prev_any_statement_list_item_id;
			/*  This is the case when we need to explicitly add a tag pre-declaration for the 'struct foo;' case. 
			    Note that this can cause multiple 'predeclarations' for the tag (one here and one in the struct/union specifier rule), but
			    this shouldn't be a problem in general because the declarations should be compatible.
			*/
			make_tag_predeclarations(state, declaration_specifiers_list_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION);
		}else{
			parser_progress("Expected a SEMICOLON_CHAR.\n");
			backtrack(state, checkpoint);
			*result_statement_list = prev_any_statement_list_item_id;
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected a declaration.\n");
		*result_statement_list = prev_any_statement_list_item_id;
		return (struct parser_node *)0;
	}
}

struct parser_node * declaration_list_rest(struct parser_state * state, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build declaration_list_rest\n");
	if((n1 = declaration(state, prev_any_statement_list_item_id, result_statement_list))){
		if((n1->next = declaration_list_rest(state, *result_statement_list, result_statement_list))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_LIST);
		}else{
			parser_progress("Expected a declaration_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*result_statement_list = prev_any_statement_list_item_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, DECLARATION_LIST_REST);
	}
}

struct parser_node * declaration_list(struct parser_state * state, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build declaration_list\n");
	if((n1 = declaration(state, prev_any_statement_list_item_id, result_statement_list))){
		if((n1->next = declaration_list_rest(state, *result_statement_list, result_statement_list))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_LIST);
		}else{
			parser_progress("Expected a declaration_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected a declaration.\n");
		*result_statement_list = prev_any_statement_list_item_id;
		return (struct parser_node *)0;
	}
}

struct parser_node * identifier_list_rest(struct parser_state * state, struct any_statement_list_item_id prev_list_id, struct any_statement_list_item_id * result_dcl_list){
	struct parser_node * n1;
	struct specifier_or_qualifier_list_item_id p1 = make_specifier_or_qualifier_list_item_list_start(state->syntax_model);
	struct declarator_part_list_item_id p2 = make_declarator_part_list_item_list_start(state->syntax_model);
	struct bitfield_id p3 = make_bitfield_no_bitfield(state->syntax_model);
	struct general_type_id empty_general_type = make_general_type_general_type(state->syntax_model, p1, p2, p3);
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build identifier_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		struct identifier_id identifier_id;
		if((n1->next = get_any_identifier_node(state, &identifier_id))){
			struct simple_declaration_id simple_declaration_id = make_simple_declaration_simple_declaration(state->syntax_model, empty_general_type, identifier_id, make_initializer_no_initializer(state->syntax_model));
			struct declaration_statement_id declaration_statement_id = make_declaration_statement_simple_declaration(state->syntax_model, simple_declaration_id);
			struct any_statement_id any_statement_id = make_any_statement_declaration_statement(state->syntax_model, declaration_statement_id);
			*result_dcl_list = make_any_statement_list_item_any_statement(state->syntax_model, prev_list_id, any_statement_id);
			make_identifier_namespace_entry_declaration_with_default_order(state->syntax_model, identifier_id, get_current_scope_guid(state), empty_general_type);
			if((n1->next->next = identifier_list_rest(state, *result_dcl_list, result_dcl_list))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, IDENTIFIER_LIST_REST);
			}else{
				parser_progress("Expected a IDENTIFIER_LIST_REST.\n");
				backtrack(state, checkpoint);
				return (struct parser_node *)0;
			}
		}else{
			parser_progress("Expected an IDENTIFIER.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, IDENTIFIER_LIST_REST);
	}
}

struct parser_node * identifier_list(struct parser_state * state, struct any_statement_list_item_id * result_dcl_list, unsigned int is_function_definition, unsigned int * parameter_list_count){
	struct parser_node * n1;
	struct identifier_id identifier_id;
	struct specifier_or_qualifier_list_item_id p1 = make_specifier_or_qualifier_list_item_list_start(state->syntax_model);
	struct declarator_part_list_item_id p2 = make_declarator_part_list_item_list_start(state->syntax_model);
	struct bitfield_id p3 = make_bitfield_no_bitfield(state->syntax_model);
	struct general_type_id empty_general_type = make_general_type_general_type(state->syntax_model, p1, p2, p3);
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	if(is_function_definition == 1 && *parameter_list_count == 1){
		increment_scope_depth(state, SCOPE_LEVEL_TYPE_FUNCTION_SCOPE);
	}else{
		increment_scope_depth(state, SCOPE_LEVEL_TYPE_FUNCTION_PROTOTYPE_SCOPE);
	}
	parser_progress("Attempting to build identifier_list\n");
	if((n1 = get_any_identifier_node(state, &identifier_id))){
		struct simple_declaration_id simple_declaration_id = make_simple_declaration_simple_declaration(state->syntax_model, empty_general_type, identifier_id, make_initializer_no_initializer(state->syntax_model));
		struct declaration_statement_id declaration_statement_id = make_declaration_statement_simple_declaration(state->syntax_model, simple_declaration_id);
		struct any_statement_id any_statement_id = make_any_statement_declaration_statement(state->syntax_model, declaration_statement_id);
		*result_dcl_list = make_any_statement_list_item_any_statement(state->syntax_model, make_any_statement_list_item_list_start(state->syntax_model), any_statement_id);
		make_identifier_namespace_entry_declaration_with_default_order(state->syntax_model, identifier_id, get_current_scope_guid(state), empty_general_type);
		if((n1->next = identifier_list_rest(state, *result_dcl_list, result_dcl_list))){
			if(is_function_definition == 0 || *parameter_list_count > 1){
				*parameter_list_count = *parameter_list_count - 1;
				decrement_scope_depth(state);
			}else{
			}
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, IDENTIFIER_LIST);
		}else{
			parser_progress("Expected a IDENTIFIER_LIST.\n");
			backtrack(state, checkpoint);
			*result_dcl_list = make_any_statement_list_item_list_start(state->syntax_model);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected an IDENTIFIER.\n");
		*result_dcl_list = make_any_statement_list_item_list_start(state->syntax_model);
		if(is_function_definition == 0 || *parameter_list_count > 1){ /*  For a declaration, we go back to file scope after the prototype. */
			*parameter_list_count = *parameter_list_count - 1;
			decrement_scope_depth(state);
		}
		return (struct parser_node *)0;
	}
}

struct parser_node * direct_declarator_rest(struct parser_state * state, struct declarator_part_list_item_id previous_declarator_item_list_id, struct declarator_part_list_item_id * this_list_id, struct identifier_from_declarator * identifier, struct any_statement_list_item_id * param_declaration_statements, unsigned int is_function_definition, unsigned int * parameter_list_count){
	struct parser_node * n1;
	parser_progress("Attempting to build direct_declarator_rest\n");
	if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR,state))){
		struct expression_id e1;
		if((n1->next = constant_expression(state, &e1))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR,state))){
				struct declarator_part_list_item_id current_declarator_part_id;
				current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_array(state->syntax_model, make_array_part_expression(state->syntax_model, e1)));
				if((n1->next->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier, param_declaration_statements, is_function_definition, parameter_list_count))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR_REST);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_ARRAY_SIZED);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_AFTER_CONSTANT);
				return (struct parser_node *)0;
			}
		}else if((n1->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR,state))){
			struct declarator_part_list_item_id current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_array(state->syntax_model, make_array_part_flexible(state->syntax_model)));
			if((n1->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier, param_declaration_statements, is_function_definition, parameter_list_count))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_ARRAY_UNSIZED);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_OR_CONSTANT);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		struct parameter_list_id parameter_list_id;
		*parameter_list_count = *parameter_list_count + 1;
		if((n1->next = parameter_type_list(state, &parameter_list_id, param_declaration_statements, is_function_definition, parameter_list_count))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR,state))){
				struct declarator_part_list_item_id current_declarator_part_id;
				struct any_statement_list_item_id next_params_list_dcls;
				current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_function(state->syntax_model, make_function_part_prototype(state->syntax_model, parameter_list_id)));
				if((n1->next->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier, &next_params_list_dcls, is_function_definition, parameter_list_count))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR_REST);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_PARAM_FUNCTION);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_PARAM);
				return (struct parser_node *)0;
			}
		}else if((n1->next = identifier_list(state, param_declaration_statements, is_function_definition, parameter_list_count))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR,state))){
				struct parameter_list_id empty_variadic_parameter_list = make_parameter_list_variadic(state->syntax_model, make_general_type_list_item_list_start(state->syntax_model));
				struct declarator_part_list_item_id current_declarator_part_id;
				struct any_statement_list_item_id next_params_list_dcls;
				current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_function(state->syntax_model, make_function_part_k_and_r_c(state->syntax_model, empty_variadic_parameter_list)));
				if((n1->next->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier, &next_params_list_dcls, is_function_definition, parameter_list_count))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR_REST);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_IDENTIFIER_FUNCTION);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_IDENTIFIER_LIST);
				return (struct parser_node *)0;
			}
		}else if((n1->next = p_accept(CLOSE_PAREN_CHAR,state))){
			struct parameter_list_id empty_variadic_parameter_list = make_parameter_list_variadic(state->syntax_model, make_general_type_list_item_list_start(state->syntax_model));
			struct declarator_part_list_item_id current_declarator_part_id;
			current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_function(state->syntax_model, make_function_part_k_and_r_c(state->syntax_model, empty_variadic_parameter_list)));
			if((n1->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier, param_declaration_statements, is_function_definition, parameter_list_count))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_KR_FUNCTION);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = previous_declarator_item_list_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, DIRECT_DECLARATOR_REST);
	}
}

struct parser_node * type_qualifier_list_rest(struct parser_state * state, struct specifier_or_qualifier_list_item_id previous_specifier_qualifier_list_id, struct specifier_or_qualifier_list_item_id * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct simple_type_qualifier_id asm_type_qualifier_id;
	parser_progress("Attempting to build type_qualifier_list_rest\n");
	if((n1 = type_qualifier(state, &asm_type_qualifier_id))){
		struct specifier_or_qualifier_list_item_id new_list_id = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, previous_specifier_qualifier_list_id, make_specifier_or_qualifier_simple_type_qualifier(state->syntax_model, asm_type_qualifier_id));
		if((n1->next = type_qualifier_list_rest(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_QUALIFIER_LIST_REST);
		}else{
			parser_progress("Expected type_qualifier_list_rest.\n");
			backtrack(state, checkpoint);
			*this_list_id = previous_specifier_qualifier_list_id;
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = previous_specifier_qualifier_list_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, TYPE_QUALIFIER_LIST_REST);
	}
}

struct parser_node * type_qualifier_list(struct parser_state * state, struct specifier_or_qualifier_list_item_id previous_specifier_qualifier_list_id, struct specifier_or_qualifier_list_item_id * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct simple_type_qualifier_id asm_type_qualifier_id;
	parser_progress("Attempting to build type_qualifier_list\n");
	if((n1 = type_qualifier(state, &asm_type_qualifier_id))){
		struct specifier_or_qualifier_list_item_id new_list_id = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, previous_specifier_qualifier_list_id, make_specifier_or_qualifier_simple_type_qualifier(state->syntax_model, asm_type_qualifier_id));
		if((n1->next = type_qualifier_list_rest(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_QUALIFIER_LIST);
		}else{
			parser_progress("Expected type_qualifier_list_rest.\n");
			backtrack(state, checkpoint);
			*this_list_id = previous_specifier_qualifier_list_id;
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected type_qualifier.\n");
		*this_list_id = previous_specifier_qualifier_list_id;
		return (struct parser_node *)0;
	}
}


struct parser_node * pointer(struct parser_state * state, struct declarator_part_list_item_id previous_declarator_item_list_id, struct declarator_part_list_item_id * this_list_id){
	struct parser_node * n1;
	parser_progress("Attempting to build pointer\n");
	if((n1 = p_accept(MULTIPLY_CHAR, state))){
		struct specifier_or_qualifier_list_item_id asm_specifier_qualifier_list_id;
		if((n1->next = type_qualifier_list(state, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), &asm_specifier_qualifier_list_id))){
			struct declarator_part_list_item_id current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_pointer(state->syntax_model, make_pointer_part_pointer(state->syntax_model, asm_specifier_qualifier_list_id)));
			if((n1->next->next = pointer(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POINTER);
			}else{
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POINTER);
			}
		}else {
			struct declarator_part_list_item_id current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_pointer(state->syntax_model, make_pointer_part_pointer(state->syntax_model, make_specifier_or_qualifier_list_item_list_start(state->syntax_model))));
			if ((n1->next = pointer(state, current_declarator_part_id, this_list_id))) {
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POINTER);
			}else{
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POINTER);
			}
		}
	}else{
		parser_progress("Expected *.\n");
		*this_list_id = previous_declarator_item_list_id;
		return (struct parser_node *)0;
	}
}

struct parser_node * direct_declarator(struct parser_state * state, struct declarator_part_list_item_id previous_declarator_item_list_id, struct declarator_part_list_item_id * this_list_id, struct identifier_from_declarator * identifier, struct any_statement_list_item_id * param_declaration_statements, unsigned int is_function_definition, unsigned int * parameter_list_count){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build direct_declarator\n");
	assert(!identifier->initialized);
	if((n1 = get_any_identifier_node(state, &identifier->id))){
		identifier->initialized = 1; /*  id does point to a valid identifier */
		if((n1->next = direct_declarator_rest(state, previous_declarator_item_list_id, this_list_id, identifier, param_declaration_statements, is_function_definition, parameter_list_count))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR);
		}else{
			add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_MISSING_REST_AFTER_IDENTIFIER);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		if((n1->next = declarator(state, previous_declarator_item_list_id, this_list_id, identifier, param_declaration_statements, is_function_definition, parameter_list_count))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				if((n1->next->next->next = direct_declarator_rest(state, *this_list_id, this_list_id, identifier, param_declaration_statements, is_function_definition, parameter_list_count))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_MISSING_REST_AFTER_CLOSE_PAREN);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_MISSING_CLOSE_PAREN);
				return (struct parser_node *)0;
			}
		}else{
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected a pointer or direct declarator.\n");
		return (struct parser_node *)0;
	}
}



struct parser_node * declarator(struct parser_state * state, struct declarator_part_list_item_id previous_declarator_item_list_id, struct declarator_part_list_item_id * this_list_id, struct identifier_from_declarator * identifier, struct any_statement_list_item_id * param_declaration_statements, unsigned int is_function_definition, unsigned int * parameter_list_count){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct declarator_part_list_item_id pointer_declarator_list_id;
	struct declarator_part_list_item_id direct_declarator_list_id;
	parser_progress("Attempting to build declarator\n");
	if((n1 = pointer(state, make_declarator_part_list_item_list_start(state->syntax_model), &pointer_declarator_list_id))){
		if((n1->next = direct_declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &direct_declarator_list_id, identifier, param_declaration_statements, is_function_definition, parameter_list_count))){
			/*  Special case: */
			*this_list_id = add_pointer_and_direct_declarators_to_list(state->syntax_model, previous_declarator_item_list_id, pointer_declarator_list_id, direct_declarator_list_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATOR);
		}else{
			parser_progress("Expected a direct declarator.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else if((n1 = direct_declarator(state, previous_declarator_item_list_id, this_list_id, identifier, param_declaration_statements, is_function_definition, parameter_list_count))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATOR);
	}else{
		parser_progress("Expected a pointer or direct declarator.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * initializer_list_rest(struct parser_state * state, struct initializer_list_item_id prev_member, struct initializer_list_item_id * this_list_id){
	struct parser_node * n1;
	parser_progress("Attempting to build initializer_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR,state))){
		struct initializer_id initializer_id;
		if((n1->next = initializer(state, &initializer_id))){
			*this_list_id = make_initializer_list_item_initializer(state->syntax_model, prev_member, initializer_id);
			if((n1->next->next = initializer_list_rest(state, *this_list_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER_LIST_REST);
			}else{
				assert(0 && "Impossible");
				return (struct parser_node *)0;
			}
		}else{
			*this_list_id = prev_member;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER_LIST_REST);
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_member;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, INITIALIZER_LIST_REST);
	}
}

struct parser_node * initializer_list(struct parser_state * state, struct initializer_id * initializer_id){
	struct parser_node * n1;
	struct initializer_id sub_initializer_id;
	struct initializer_list_item_id initializer_list_id = make_initializer_list_item_list_start(state->syntax_model);
	parser_progress("Attempting to build initializer_list\n");
	if((n1 = initializer(state, &sub_initializer_id))){
		initializer_list_id = make_initializer_list_item_initializer(state->syntax_model, initializer_list_id, sub_initializer_id);
		if((n1->next = initializer_list_rest(state, initializer_list_id, &initializer_list_id))){
			*initializer_id = make_initializer_initializer_list(state->syntax_model, initializer_list_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER_LIST);
		}else{
			assert(0 && "Impossible");
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected initializer.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * initializer(struct parser_state * state, struct initializer_id * initializer_id){
	struct parser_node * n1;
	struct expression_id e1;
	parser_progress("Attempting to build initializer\n");
	if((n1 = assignment_expression(state, &e1))){
		*initializer_id = make_initializer_expression(state->syntax_model, e1);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER);
	}else if((n1 = p_accept(OPEN_BRACE_CHAR, state))){
		if((n1->next = initializer_list(state, initializer_id))){
			if((n1->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER);
			}else{
				add_parser_error(state, PARSER_ERROR_INITIALIZER_MISSING_COMMA_OR_CLOSE_BRACE);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_INITIALIZER_MISSING_INITIALIZER_LIST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected OPEN_BRACE_CHAR or assignment expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * init_declarator(struct parser_state * state, struct specifier_or_qualifier_list_item_id specifier_qualifier_list_id, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct any_statement_list_item_id param_declaration_statements;
	struct identifier_from_declarator identifier_id;
	struct declarator_part_list_item_id declarator_part_list_item_id;
	struct initializer_id initializer_id;
	unsigned int is_function_definition = 0;
	unsigned int parameter_list_count = 0;
	identifier_id.initialized = 0; /*  No id obtained yet. */
	parser_progress("Attempting to build init_declarator\n");
	if((n1 = declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &declarator_part_list_item_id, &identifier_id, &param_declaration_statements, is_function_definition, &parameter_list_count))){
		struct simple_declaration_id simple_declaration_id;
		struct declaration_statement_id declaration_statement_id;
		struct any_statement_id any_statement_id;
		struct any_statement_list_item_id new_any_statement_list_item_id;
		struct general_type_id general_type_id = make_general_type_general_type(state->syntax_model, specifier_qualifier_list_id, declarator_part_list_item_id, make_bitfield_no_bitfield(state->syntax_model));
		assert(identifier_id.initialized);
		/*  Typedefed types won't be added as an 'any_statement' to the ast model. */
		if(specifier_or_qualifier_list_item_id_contains_typedef(state, specifier_qualifier_list_id)){
			make_identifier_namespace_entry_typedef_with_default_order(state->syntax_model, identifier_id.id, get_current_scope_guid(state), general_type_id);
		}else{
			make_identifier_namespace_entry_declaration_with_default_order(state->syntax_model, identifier_id.id, get_current_scope_guid(state), general_type_id);
		}
		if((n1->next = p_accept(EQUALS_CHAR, state))){
			if((n1->next->next = initializer(state, &initializer_id))){

				simple_declaration_id = make_simple_declaration_simple_declaration(state->syntax_model, general_type_id, identifier_id.id, initializer_id);
				declaration_statement_id = make_declaration_statement_simple_declaration(state->syntax_model, simple_declaration_id);
				any_statement_id = make_any_statement_declaration_statement(state->syntax_model, declaration_statement_id);
				new_any_statement_list_item_id = make_any_statement_list_item_any_statement(state->syntax_model, prev_any_statement_list_item_id, any_statement_id);
				if(specifier_or_qualifier_list_item_id_contains_typedef(state, specifier_qualifier_list_id)){
					*result_statement_list = prev_any_statement_list_item_id;
				}else{
					*result_statement_list = new_any_statement_list_item_id;
				}
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INIT_DECLARATOR);
			}else{
				add_parser_error(state, PARSER_ERROR_INIT_DECLARATOR_MISSING_INITIALIZER);
				return (struct parser_node *)0;
			}
		}else{
			initializer_id = make_initializer_no_initializer(state->syntax_model);

			simple_declaration_id = make_simple_declaration_simple_declaration(state->syntax_model, general_type_id, identifier_id.id, initializer_id);
			declaration_statement_id = make_declaration_statement_simple_declaration(state->syntax_model, simple_declaration_id);
			any_statement_id = make_any_statement_declaration_statement(state->syntax_model, declaration_statement_id);
			new_any_statement_list_item_id = make_any_statement_list_item_any_statement(state->syntax_model, prev_any_statement_list_item_id, any_statement_id);
			if(specifier_or_qualifier_list_item_id_contains_typedef(state, specifier_qualifier_list_id)){
				*result_statement_list = prev_any_statement_list_item_id;
			}else{
				*result_statement_list = new_any_statement_list_item_id;
			}
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INIT_DECLARATOR);
		}

	}else{
		parser_progress("Expected declarator in init declarator.\n");
		*result_statement_list = prev_any_statement_list_item_id;
		return (struct parser_node *)0;
	}
}

struct parser_node * struct_declarator(struct parser_state * state, struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_item_id, struct struct_or_union_member_id * struct_or_union_member_id){
	struct parser_node * n1;
	struct identifier_from_declarator identifier_id;
	struct bitfield_id bitfield_id;
	struct declarator_part_list_item_id result_declarator_list;
	struct general_type_id general_type_id;
	struct any_statement_list_item_id param_declaration_statements;
	unsigned int is_function_definition = 0;
	unsigned int parameter_list_count = 0;
	identifier_id.initialized = 0; /*  No id obtained yet. */
	parser_progress("Attempting to build struct_declarator\n");
	if((n1 = declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &result_declarator_list, &identifier_id, &param_declaration_statements, is_function_definition, &parameter_list_count))){
		if((n1->next = p_accept(COLON_CHAR, state))){
			struct expression_id e1;
			if((n1->next->next = constant_expression(state, &e1))){
				bitfield_id = make_bitfield_constant_bitfield(state->syntax_model, e1);
				general_type_id = make_general_type_general_type(state->syntax_model, specifier_or_qualifier_list_item_id, result_declarator_list, bitfield_id);
				*struct_or_union_member_id = make_struct_or_union_member_member(state->syntax_model, general_type_id, identifier_id.id);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR);
			}else{
				add_parser_error(state, PARSER_ERROR_STRUCT_DECLARATOR_MISSING_CONSTANT_AFTER_DECLARATOR);
				return (struct parser_node *)0;
			}
		}else{
			bitfield_id = make_bitfield_no_bitfield(state->syntax_model);
			general_type_id = make_general_type_general_type(state->syntax_model, specifier_or_qualifier_list_item_id, result_declarator_list, bitfield_id);
			*struct_or_union_member_id = make_struct_or_union_member_member(state->syntax_model, general_type_id, identifier_id.id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR);
		}
	}else if((n1 = p_accept(COLON_CHAR, state))){
		struct expression_id e1;
		if((n1->next = constant_expression(state, &e1))){
			bitfield_id = make_bitfield_constant_bitfield(state->syntax_model, e1);
			general_type_id = make_general_type_general_type(state->syntax_model, specifier_or_qualifier_list_item_id, make_declarator_part_list_item_list_start(state->syntax_model), bitfield_id);
			*struct_or_union_member_id = make_struct_or_union_member_padding(state->syntax_model, general_type_id);

			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR);
		}else{
			add_parser_error(state, PARSER_ERROR_STRUCT_DECLARATOR_MISSING_CONSTANT_NO_DECLARATOR);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected COLON_CHAR or declarator.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * struct_declarator_list_rest(struct parser_state * state, struct specifier_or_qualifier_list_item_id specifier_qualifier_list_id, struct struct_or_union_member_list_item_id prev_member, struct struct_or_union_member_list_item_id * this_list_id){
	struct parser_node * n1;
	parser_progress("Attempting to build struct_declarator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		struct struct_or_union_member_id struct_or_union_member_id;
		if((n1->next = struct_declarator(state, specifier_qualifier_list_id, &struct_or_union_member_id))){
			*this_list_id = make_struct_or_union_member_list_item_member(state->syntax_model, prev_member, struct_or_union_member_id);
			if((n1->next->next = struct_declarator_list_rest(state, specifier_qualifier_list_id, *this_list_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR_LIST_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_STRUCT_DECLARATOR_LIST_REST_MISSING_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_STRUCT_DECLARATOR_LIST_REST_MISSING_STRUCT_DECLARATOR);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_member;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, STRUCT_DECLARATOR_LIST_REST);
	}
}


struct parser_node * struct_declarator_list(struct parser_state * state, struct specifier_or_qualifier_list_item_id specifier_qualifier_list_id, struct struct_or_union_member_list_item_id prev_member, struct struct_or_union_member_list_item_id * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct struct_or_union_member_id struct_or_union_member_id;
	parser_progress("Attempting to build struct_declarator_list\n");
	if((n1 = struct_declarator(state, specifier_qualifier_list_id, &struct_or_union_member_id))){
		*this_list_id = make_struct_or_union_member_list_item_member(state->syntax_model, prev_member, struct_or_union_member_id);
		if((n1->next = struct_declarator_list_rest(state, specifier_qualifier_list_id, *this_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR_LIST);
		}else{
			parser_progress("Expected struct_declarator_list_rest.\n");
			backtrack(state, checkpoint);
			*this_list_id = prev_member;
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected struct_declarator.\n");
		*this_list_id = prev_member;
		return (struct parser_node *)0;
	}
}

struct parser_node * specifier_qualifier_list(struct parser_state * state, struct specifier_or_qualifier_list_item_id previous_specifier_qualifier_list_id, struct specifier_or_qualifier_list_item_id * this_list_id){
	struct parser_node * n1;
	struct aggregate_type_specifier_id asm_type_specifier_id;
	struct simple_type_qualifier_id asm_type_qualifier_id;
	parser_progress("Attempting to build specifier_qualifier_list\n");
	if((n1 = type_specifier(state, &asm_type_specifier_id))){
		struct specifier_or_qualifier_list_item_id new_list_id = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, previous_specifier_qualifier_list_id, make_specifier_or_qualifier_aggregate_type_specifier(state->syntax_model, asm_type_specifier_id));
		if((n1->next = specifier_qualifier_list(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SPECIFIER_QUALIFIER_LIST);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SPECIFIER_QUALIFIER_LIST);
		}
	}else if((n1 = type_qualifier(state, &asm_type_qualifier_id))){
		struct specifier_or_qualifier_list_item_id new_list_id = make_specifier_or_qualifier_list_item_specifier_or_qualifier(state->syntax_model, previous_specifier_qualifier_list_id, make_specifier_or_qualifier_simple_type_qualifier(state->syntax_model, asm_type_qualifier_id));
		if((n1->next = specifier_qualifier_list(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SPECIFIER_QUALIFIER_LIST);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SPECIFIER_QUALIFIER_LIST);
		}
	}else{
		parser_progress("Expected type specifier or type qualifier.\n");
		*this_list_id = previous_specifier_qualifier_list_id;
		return (struct parser_node *)0;
	}
}

struct parser_node * struct_declaration(struct parser_state * state, struct struct_or_union_member_list_item_id prev_member, struct struct_or_union_member_list_item_id * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_item_id;
	parser_progress("Attempting to build struct_declaration\n");
	if((n1 = specifier_qualifier_list(state, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), &specifier_or_qualifier_list_item_id))){
		if((n1->next = struct_declarator_list(state, specifier_or_qualifier_list_item_id, prev_member, this_list_id))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR,state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATION);
			}else{
				parser_progress("Expected SEMICOLON_CHAR.\n");
				backtrack(state, checkpoint);
				*this_list_id = prev_member;
				return (struct parser_node *)0;
			}
		}else if((n1->next = p_accept(SEMICOLON_CHAR, state))){
			/*  C11 style ones where inner members are accessible by outer structure when inner members are in anonymous struct/union.  */
			struct bitfield_id bitfield_id = make_bitfield_no_bitfield(state->syntax_model);
			struct general_type_id general_type_id = make_general_type_general_type(state->syntax_model, specifier_or_qualifier_list_item_id, make_declarator_part_list_item_list_start(state->syntax_model), bitfield_id);
			struct struct_or_union_member_id struct_or_union_member_id = make_struct_or_union_member_c11_anonymous_member(state->syntax_model, general_type_id);
			*this_list_id = make_struct_or_union_member_list_item_member(state->syntax_model, prev_member, struct_or_union_member_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATION);
		}else{
			parser_progress("Expected SEMICOLON_CHAR.\n");
			backtrack(state, checkpoint);
			*this_list_id = prev_member;
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected specifier_qualifier_list.\n");
		*this_list_id = prev_member;
		return (struct parser_node *)0;
	}
}

struct parser_node * struct_declaration_list_rest(struct parser_state * state, struct struct_or_union_member_list_item_id prev_member, struct struct_or_union_member_list_item_id * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build struct_declaration_list_rest\n");
	if((n1 = struct_declaration(state, prev_member, this_list_id))){
		if((n1->next = struct_declaration_list_rest(state, *this_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATION_LIST_REST);
		}else{
			parser_progress("Expected struct_declaration_list_rest.\n");
			backtrack(state, checkpoint);
			*this_list_id = prev_member;
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_member;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, STRUCT_DECLARATION_LIST_REST);
	}
}


struct parser_node * struct_declaration_list(struct parser_state * state, struct struct_or_union_member_list_item_id prev_member, struct struct_or_union_member_list_item_id * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build struct_declaration_list\n");
	if((n1 = struct_declaration(state, prev_member, this_list_id))){
		if((n1->next = struct_declaration_list_rest(state, *this_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATION_LIST);
		}else{
			parser_progress("Expected struct_declaration_list_rest.\n");
			backtrack(state, checkpoint);
			*this_list_id = prev_member;
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected struct_declaration.\n");
		*this_list_id = prev_member;
		return (struct parser_node *)0;
	}
}

void print_parser_error_description(struct unsigned_char_list * list, struct parser_state * state, enum parser_error_type t){
	(void)state;
	switch(t){
		case PARSER_ERROR_INCOMPLETE_PARSE:{
			buffered_printf(list, "Unable to parse remaining tokens.\n");
			break;
		}case PARSER_ERROR_PARSING_FAILED:{
			buffered_printf(list, "Unable to build parser tree.\n");
			break;
		}case PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_BRACE:{
			buffered_printf(list, "Missing closing brace on struct or union definition.\n");
			break;
		}case PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_NO_DECLARATIONS:{
			buffered_printf(list, "This struct or union definition does not contain any declarations.\n");
			break;
		}case PARSER_ERROR_PRIMARY_EXPRESSION_MISSING_CLOSE_PAREN:{
			buffered_printf(list, "Unterminated parenthesis in primary expression.\n");
			break;
		}case PARSER_ERROR_INIT_DECLARATOR_MISSING_INITIALIZER:{
			buffered_printf(list, "Expected an initializer, but didn't find one.\n");
			break;
		}case PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_MISSING_ARGUMENT:{
			buffered_printf(list, "Expected an argument, but didn't find one.\n");
			break;
		}case PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_REST_MISSING_ARGUMENT:{
			buffered_printf(list, "Expected another argument, but didn't find one.\n");
			break;
		}case PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_REST_MISSING_ASSIGNMENT_EXPRESSION:{
			buffered_printf(list, "Unable to build argument list because of previous errors.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_NO_REST:{
			buffered_printf(list, "Error build rest of postfix expression after array.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_EXPRESSION_INCOMPLETE:{
			buffered_printf(list, "Missing closing square bracket on array dereference.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_EXPRESSION_MISSING:{
			buffered_printf(list, "Missing expression and closing brace after open square bracket.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_0_ARGS_NO_REST:{
			buffered_printf(list, "Error building rest of postfix expression: expected arguments.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_SOME_ARGS_NO_REST:{
			buffered_printf(list, "Error building rest of postfix expression in function arguments.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_SOME_ARGS_NO_CLOSE:{
			buffered_printf(list, "Missing parenthesis for function call\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_0_ARGS_NO_CLOSE:{
			buffered_printf(list, "Missing parenthesis for function call.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_DOT_NO_REST:{
			buffered_printf(list, "Error building rest of postfix expression after dot operator.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_DOT_NO_IDENTIFIER:{
			buffered_printf(list, "Missing identifier after dot operator.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_PTR_NO_REST:{
			buffered_printf(list, "Error building rest of postfix expression after structure member dereference operator.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_PTR_NO_IDENTIFIER:{
			buffered_printf(list, "Missing identifier after structure member dereference operator.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_INC_NO_REST:{
			buffered_printf(list, "Error building rest of postfix expression after increment operator.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_REST_DEC_NO_REST:{
			buffered_printf(list, "Error building rest of postfix expression after decrement operator.\n");
			break;
		}case PARSER_ERROR_POSTFIX_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error building rest of postfix expression.\n");
			break;
		}case PARSER_ERROR_UNARY_EXPRESSION_AFTER_INC:{
			buffered_printf(list, "Syntax error after increment operator.\n");
			break;
		}case PARSER_ERROR_UNARY_EXPRESSION_AFTER_DEC:{
			buffered_printf(list, "Syntax error after decrement operator.\n");
			break;
		}case PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST:{
			buffered_printf(list, "Missing cast expression in sizeof operation.\n");
			break;
		}case PARSER_ERROR_UNARY_EXPRESSION_MISSING_PAREN:{
			buffered_printf(list, "Missing closing parenthesis in sizeof operation.\n");
			break;
		}case PARSER_ERROR_UNARY_EXPRESSION_MISSING_TYPE:{
			buffered_printf(list, "Missing type in sizeof operation.\n");
			break;
		}case PARSER_ERROR_UNARY_EXPRESSION_BAD_SIZEOF:{
			buffered_printf(list, "Syntax error in sizeof operation.\n");
			break;
		}case PARSER_ERROR_CAST_EXPRESSION_MISSING_CAST:{
			buffered_printf(list, "Missing an expression to cast.\n");
			break;
		}case PARSER_ERROR_CAST_EXPRESSION_MISSING_CLOSE_PAREN:{
			buffered_printf(list, "Missing close paren in cast expression.\n");
			break;
		}case PARSER_ERROR_CAST_EXPRESSION_MISSING_TYPE_NAME:{
			buffered_printf(list, "Missing type name in cast expression.\n");
			break;
		}case PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing cast expression for multiplicative expression.\n");
			break;
		}case PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of multiplicative expression.\n");
			break;
		}case PARSER_ERROR_ADDITIVE_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for additive expression.\n");
			break;
		}case PARSER_ERROR_ADDITIVE_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of additive expression.\n");
			break;
		}case PARSER_ERROR_ADDITIVE_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of additive expression.\n");
			break;
		}case PARSER_ERROR_SHIFT_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for shift expression.\n");
			break;
		}case PARSER_ERROR_SHIFT_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of shift expression.\n");
			break;
		}case PARSER_ERROR_SHIFT_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of shift expression.\n");
			break;
		}case PARSER_ERROR_RELATIONAL_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for relational expression.\n");
			break;
		}case PARSER_ERROR_RELATIONAL_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of relational expression.\n");
			break;
		}case PARSER_ERROR_RELATIONAL_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of relational expression.\n");
			break;
		}case PARSER_ERROR_EQUALITY_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for equality expression.\n");
			break;
		}case PARSER_ERROR_EQUALITY_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of equality expression.\n");
			break;
		}case PARSER_ERROR_EQUALITY_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of equality expression.\n");
			break;
		}case PARSER_ERROR_AND_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for and expression.\n");
			break;
		}case PARSER_ERROR_AND_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of and expression.\n");
			break;
		}case PARSER_ERROR_AND_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of and expression.\n");
			break;
		}case PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for exclusive or expression.\n");
			break;
		}case PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of exclusive or expression.\n");
			break;
		}case PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of exclusive or expression.\n");
			break;
		}case PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for inclusive or expression.\n");
			break;
		}case PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of inclusive or expression.\n");
			break;
		}case PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of inclusive or expression.\n");
			break;
		}case PARSER_ERROR_LOGICAL_AND_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for logical and expression.\n");
			break;
		}case PARSER_ERROR_LOGICAL_AND_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of logical and expression.\n");
			break;
		}case PARSER_ERROR_LOGICAL_AND_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of logical and expression.\n");
			break;
		}case PARSER_ERROR_LOGICAL_OR_EXPRESSION_REST_EXPECTED_EXPRESSION:{
			buffered_printf(list, "Missing expression for logical or expression.\n");
			break;
		}case PARSER_ERROR_LOGICAL_OR_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error in rest of logical or expression.\n");
			break;
		}case PARSER_ERROR_LOGICAL_OR_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error in rest of logical or expression.\n");
			break;
		}case PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_1:{
			buffered_printf(list, "Missing a closing brace to end compound statement(1).\n");
			break;
		}case PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_2:{
			buffered_printf(list, "Missing a closing brace to end compound statement(2).\n");
			break;
		}case PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_3:{
			buffered_printf(list, "Missing a closing brace to end compound statement(3).\n");
			break;
		}case PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_4:{
			buffered_printf(list, "Missing a closing brace to end compound statement(4).\n");
			break;
		}case PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_FALSE:{
			buffered_printf(list, "Missing expression for false case in conditional expression.\n");
			break;
		}case PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_COLON:{
			buffered_printf(list, "Missing colon in conditional expression.\n");
			break;
		}case PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_TRUE:{
			buffered_printf(list, "Missing expression for true case in conditional expression.\n");
			break;
		}case PARSER_ERROR_EXPRESSION_REST_NO_REST:{
			buffered_printf(list, "Error building expression.\n");
			break;
		}case PARSER_ERROR_EXPRESSION_REST_NO_ASSIGNMENT_EXPRESSION:{
			buffered_printf(list, "Error building expression.\n");
			break;
		}case PARSER_ERROR_EXPRESSION_NO_REST:{
			buffered_printf(list, "Error building expression.\n");
			break;
		}case PARSER_ERROR_ENUM_SPECIFIER_MISSING_OPEN_BRACE_OR_IDENTIFIER:{
			buffered_printf(list, "Error building enum specifier:  Expected an open brace or identifier.\n");
			break;
		}case PARSER_ERROR_ENUM_SPECIFIER_MISSING_ENUMERATOR_LIST_NAMED:{
			buffered_printf(list, "Error building named enum specifier:  Expected an enumerator list.\n");
			break;
		}case PARSER_ERROR_ENUM_SPECIFIER_MISSING_CLOSE_BRACE_NAMED:{
			buffered_printf(list, "Error building named enum specifier:  Expected closing brace.\n");
			break;
		}case PARSER_ERROR_ENUM_SPECIFIER_MISSING_ENUMERATOR_LIST_ANON:{
			buffered_printf(list, "Error building anonymous enum specifier:  Expected enumerator list.\n");
			break;
		}case PARSER_ERROR_ENUM_SPECIFIER_MISSING_CLOSE_BRACE_ANON:{
			buffered_printf(list, "Error building anonymous enum specifier:  Expected close brace.\n");
			break;
		}case PARSER_ERROR_ENUMERATOR_LIST_MISSING_REST:{
			buffered_printf(list, "Error building rest of enumerator list.\n");
			break;
		}case PARSER_ERROR_ENUMERATOR_LIST_REST_MISSING_ENUMERATOR:{
			buffered_printf(list, "Error building enumerator list rest:  Expected enumerator.\n");
			break;
		}case PARSER_ERROR_ENUMERATOR_LIST_REST_MISSING_REST:{
			buffered_printf(list, "Error building rest of enumerator list rest.\n");
			break;
		}case PARSER_ERROR_ENUMERATOR_MISSING_CONSTANT_EXPRESSION:{
			buffered_printf(list, "Error building enumerator:  Expected constant expression.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_ARRAY_SIZED:{
			buffered_printf(list, "Error building rest of direct_declarator_rest.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_AFTER_CONSTANT:{
			buffered_printf(list, "Error building direct declarator rest: Missing close bracket.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_ARRAY_UNSIZED:{
			buffered_printf(list, "Error building rest of direct_declarator_rest.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_OR_CONSTANT:{
			buffered_printf(list, "Error building direct declarator rest: Expected close bracket or constant.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_PARAM_FUNCTION:{
			buffered_printf(list, "Error building rest of direct_declarator_rest.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_PARAM:{
			buffered_printf(list, "Error building direct declarator rest:  Missing close paren.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_IDENTIFIER_FUNCTION:{
			buffered_printf(list, "Error building rest of direct_declarator_rest.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_IDENTIFIER_LIST:{
			buffered_printf(list, "Error building direct declarator rest: missing close paren.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_KR_FUNCTION:{
			buffered_printf(list, "Error building rest of direct_declarator_rest.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN:{
			buffered_printf(list, "Error building direct declarator rest: Unmatched open paren.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_MISSING_REST_AFTER_IDENTIFIER:{
			buffered_printf(list, "Error building direct declarator: Error after identifier.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_MISSING_REST_AFTER_CLOSE_PAREN:{
			buffered_printf(list, "Error building direct declarator: Error after close paren.\n");
			break;
		}case PARSER_ERROR_DIRECT_DECLARATOR_MISSING_CLOSE_PAREN:{
			buffered_printf(list, "Error building direct declarator: Missing close paren.\n");
			break;
		}case PARSER_ERROR_INITIALIZER_MISSING_COMMA_OR_CLOSE_BRACE:{
			buffered_printf(list, "Error building initializer:  Expected comma or close brace.\n");
			break;
		}case PARSER_ERROR_INITIALIZER_MISSING_INITIALIZER_LIST:{
			buffered_printf(list, "Error building initializer:  Missing initializer list.\n");
			break;
		}case PARSER_ERROR_STRUCT_DECLARATOR_MISSING_CONSTANT_AFTER_DECLARATOR:{
			buffered_printf(list, "Error building struct declarator bitfield:  Missing constant value after declarator.\n");
			break;
		}case PARSER_ERROR_STRUCT_DECLARATOR_MISSING_CONSTANT_NO_DECLARATOR:{
			buffered_printf(list, "Error building struct declarator bitfield:  Missing constant value.\n");
			break;
		}case PARSER_ERROR_STRUCT_DECLARATOR_LIST_REST_MISSING_REST:{
			buffered_printf(list, "Error building struct declarator list.\n");
			break;
		}case PARSER_ERROR_STRUCT_DECLARATOR_LIST_REST_MISSING_STRUCT_DECLARATOR:{
			buffered_printf(list, "Error building struct declarator list:  Missing struct declarator.\n");
			break;
		}case PARSER_ERROR_PARAMETER_TYPE_LIST_MISSING_ELLIPSIS:{
			buffered_printf(list, "Error building parameter type list:  Expected ellipsis.\n");
			break;
		}case PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_STRUCT_DECLARATION_LIST:{
			buffered_printf(list, "Error building struct or union specifier:  Missing declaration list.\n");
			break;
		}case PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_CLOSE_BRACE:{
			buffered_printf(list, "Error building struct or union specifier:  Missing close brace.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_NO_PARAM:{
			buffered_printf(list, "Error building direct abstract declarator rest after close parameter list.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_WITH_PARAM:{
			buffered_printf(list, "Error building direct abstract declarator rest after close parameter list.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_PARAMS:{
			buffered_printf(list, "Error building direct abstract declarator rest missing close paren.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_NO_SIZE:{
			buffered_printf(list, "Error building direct abstract declarator rest after close square bracket.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_SIZED:{
			buffered_printf(list, "Error building direct abstract declarator rest after close square bracket.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_AFTER_SIZED:{
			buffered_printf(list, "Error building direct abstract declarator rest missing close square bracket.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_OR_CONSTANT:{
			buffered_printf(list, "Error building direct abstract declarator rest missing close square bracket or constant.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_NO_PARAM:{
			buffered_printf(list, "Error building direct abstract declarator after close parameter list.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_WITH_PARAM:{
			buffered_printf(list, "Error building direct abstract declarator after close parameter list.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_PAREN_AFTER_PARAMS:{
			buffered_printf(list, "Error building direct abstract declarator missing close paren.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_ABSTRACT_DECLARATOR:{
			buffered_printf(list, "Error building direct abstract declarator.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_PAREN_AFTER_ABSTRACT_DECLARATOR:{
			buffered_printf(list, "Error building direct abstract declarator missing close paren.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_NO_SIZE:{
			buffered_printf(list, "Error building direct abstract declarator after array.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_SIZED:{
			buffered_printf(list, "Error building direct abstract declarator after array size.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_BRACKET_AFTER_SIZED:{
			buffered_printf(list, "Error building direct abstract declarator missing close square bracket.\n");
			break;
		}case PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_BRACKET_OR_CONSTANT:{
			buffered_printf(list, "Error building direct abstract declarator missing close square bracket or constant.\n");
			break;
		}case PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_LABEL:{
			buffered_printf(list, "Missing statement after label.\n");
			break;
		}case PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_CASE:{
			buffered_printf(list, "Missing statement after case.\n");
			break;
		}case PARSER_ERROR_LABELED_STATEMENT_MISSING_COLON_AFTER_CASE:{
			buffered_printf(list, "Missing colon after case.\n");
			break;
		}case PARSER_ERROR_LABELED_STATEMENT_MISSING_CONSTANT_AFTER_CASE:{
			buffered_printf(list, "Missing constant after case.\n");
			break;
		}case PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_DEFAULT:{
			buffered_printf(list, "Missing statement after default.\n");
			break;
		}case PARSER_ERROR_LABELED_STATEMENT_MISSING_COLON_AFTER_DEFAULT:{
			buffered_printf(list, "Missing colon after default.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_ELSE:{
			buffered_printf(list, "Missing statement after else.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_IF:{
			buffered_printf(list, "Missing statement after if.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_IF:{
			buffered_printf(list, "Missing close paren after if.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_EXPRESSION_AFTER_IF:{
			buffered_printf(list, "Missing expression after if.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_OPEN_PAREN_AFTER_IF:{
			buffered_printf(list, "Missing open paren after if.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_SWITCH:{
			buffered_printf(list, "Missing statement after switch.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_SWITCH:{
			buffered_printf(list, "Missing close paren after switch.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_EXPRESSION_AFTER_SWITCH:{
			buffered_printf(list, "Missing expression after switch.\n");
			break;
		}case PARSER_ERROR_SELECTION_STATEMENT_MISSING_OPEN_PAREN_AFTER_SWITCH:{
			buffered_printf(list, "Missing open paren after switch.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_WHILE_CLOSE_PAREN:{
			buffered_printf(list, "Missing state for while loop.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_WHILE:{
			buffered_printf(list, "Missing close paren after while statement.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_AFTER_WHILE:{
			buffered_printf(list, "Missing expression for while condition.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_WHILE:{
			buffered_printf(list, "Missing open paren after while.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_SEMICOLON_AFTER_DO_WHILE:{
			buffered_printf(list, "Missing semicolon after do while.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_DO_WHILE:{
			buffered_printf(list, "Missing close paren after do while.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_AFTER_DO_WHILE:{
			buffered_printf(list, "Missing expression after do while.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_DO_WHILE:{
			buffered_printf(list, "Missing open paren after do while statement.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_WHILE_AFTER_DO_WHILE:{
			buffered_printf(list, "Missing while after do statement.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_DO_WHILE:{
			buffered_printf(list, "Missing statement for do while.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_SMALL_FOR:{
			buffered_printf(list, "Missing statement after for.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_BIG_FOR:{
			buffered_printf(list, "Missing statement after for.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_FOR:{
			buffered_printf(list, "Missing close paren after for.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_OR_CLOSE_PAREN_AFTER_FOR:{
			buffered_printf(list, "Missing expression or close paren after for.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_STATEMENT_2_AFTER_FOR:{
			buffered_printf(list, "Missing second expression statment in for loop.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_STATEMENT_1_AFTER_FOR:{
			buffered_printf(list, "Missing first expression statment in for loop.\n");
			break;
		}case PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_FOR:{
			buffered_printf(list, "Missing open paren for loop.\n");
			break;
		}case PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_GOTO:{
			buffered_printf(list, "Missing semicolon after goto statement.\n");
			break;
		}case PARSER_ERROR_JUMP_STATEMENT_MISSING_IDENTIFIER_AFTER_GOTO:{
			buffered_printf(list, "Missing identifier after goto statement.\n");
			break;
		}case PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_CONTINUE:{
			buffered_printf(list, "Missing semicolon after continue.\n");
			break;
		}case PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_BREAK:{
			buffered_printf(list, "Missing semicolon after break.\n");
			break;
		}case PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_RETURN:{
			buffered_printf(list, "Missing semicolon after return.\n");
			break;
		}case PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AND_EXPRESSION_AFTER_RETURN:{
			buffered_printf(list, "Missing expression or semicolon after return.\n");
			break;
		}case PARSER_ERROR_CONSTANT_OVERFLOW:{
			buffered_printf(list, "Constant is too large and results in overflow.\n");
			break;
		}default:{
			assert(0 && "Unknown error.");
		}
	}
}

void print_parser_error(struct unsigned_char_list * list, struct parser_state * state, struct parser_error e){
	unsigned int starting_token_position;
	struct c_lexer_token ** tokens = struct_c_lexer_token_ptr_list_data(&state->c_lexer_state->tokens);
	if(e.tokens_position){
		/*  Find the first token that has a newline in it */
		starting_token_position = (e.tokens_position -1);
		while(
			(starting_token_position > 0) &&
			tokens[starting_token_position]->type != NEWLINE &&
			(!
				(
					(tokens[starting_token_position]->type == COMMENT) &&
					count_newlines_in_comment(tokens[starting_token_position])
				)
			)
		){
			starting_token_position--;
		}
	}else{
		starting_token_position = e.tokens_position;
	}

	print_parser_error_description(list, state, e.type);
	buffered_printf(list, "Error in file '%s' on line %u:\n", e.filename, e.line_number);
	/*  Print out the relevant tokens */
	do{
		unsigned char * c = tokens[starting_token_position]->first_byte;
		/*  TODO:  Don't print all lines in a comment token. */
		do{
			buffered_printf(list, "%c", *c);
		}while (c++ != tokens[starting_token_position]->last_byte);
		starting_token_position++;
		if(starting_token_position == e.tokens_position){
			buffered_printf(list, ">>>>ERROR<<<<");
		}
	}while(
		/*  Keep printing until we encounter a newline or the end of the tokens. */
		(starting_token_position < struct_c_lexer_token_ptr_list_size(&state->c_lexer_state->tokens)) &&
		tokens[starting_token_position]->type != NEWLINE &&
		(!
			(
				(tokens[starting_token_position]->type == COMMENT) &&
				count_newlines_in_comment(tokens[starting_token_position])
			)
		)
	);
	buffered_printf(list, "\n");
}

void add_parser_error(struct parser_state * state, enum parser_error_type t){
	struct parser_error e;
	e.type = t;
	e.line_number = state->line_number;
	e.filename = state->c_lexer_state->c.filename;
	e.tokens_position = state->tokens_position;
	struct_parser_error_list_add_end(&state->parser_errors, e);
}

unsigned int get_scope_of_closest_identifier_declaration(struct parser_state * state, struct scope_guid_id * result, struct scope_guid_id starting_scope, struct identifier_id identifier_id, enum asm_identifier_namespace_entry_kind desired_kind, enum asm_identifier_namespace_entry_kind undesired_kind){
	struct scope_guid_id current_scope = starting_scope;
	struct scope_guid_details * scope_guids_ptr = (struct scope_guid_details *)struct_scope_guid_details_list_data(&state->syntax_model->scope_guids);
	while(1){
		struct identifier_namespace_entry_details typedef_entry_details;
		typedef_entry_details.type = desired_kind;
		typedef_entry_details.scope_guid_id = current_scope;
		typedef_entry_details.identifier_id = identifier_id;
		typedef_entry_details.order_id = 0; /*  Just look for the first one. */
		if(
			struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map_exists(
				&state->syntax_model->identifier_namespace_entrys_map,
				typedef_entry_details
			)
		){
			*result = current_scope;
			return 1; /*  Found */
		}else{
			struct identifier_namespace_entry_details declaration_entry_details;
			declaration_entry_details.type = undesired_kind;
			declaration_entry_details.scope_guid_id = current_scope;
			declaration_entry_details.identifier_id = identifier_id;
			declaration_entry_details.order_id = 0; /*  Just look for the first one. */
			if(
				struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map_exists(
					&state->syntax_model->identifier_namespace_entrys_map,
					declaration_entry_details
				)
			){
				return 0; /*  Not possible because of identifier of other type (typename/identifier) in current scope. */
			}
		}
		if(scope_guids_ptr[current_scope.id].type == ASM_SCOPE_GUID_FILE_SCOPE){
			return 0; /*  Not found */
		}else{
			current_scope = scope_guids_ptr[current_scope.id].parent;
		}
	}
}

unsigned int get_scope_of_closest_tag_declaration(struct parser_state * state, struct scope_guid_id * result, struct scope_guid_id starting_scope, struct identifier_id identifier_id){
	struct scope_guid_id current_scope = starting_scope;
	struct scope_guid_details * scope_guids_ptr = (struct scope_guid_details *)struct_scope_guid_details_list_data(&state->syntax_model->scope_guids);
	while(1){
		struct named_tag_definition_details named_tag_definition_details;
		struct named_tag_predeclaration_details named_tag_predeclaration_details;
		named_tag_definition_details.type = ASM_NAMED_TAG_DEFINITION_NAMED;
		named_tag_definition_details.scope_guid_id = current_scope;
		named_tag_definition_details.identifier_id = identifier_id;
		named_tag_definition_details.order_id = 0; /*  Use the first one. */
		named_tag_predeclaration_details.type = ASM_NAMED_TAG_PREDECLARATION_NAMED;
		named_tag_predeclaration_details.scope_guid_id = current_scope;
		named_tag_predeclaration_details.identifier_id = identifier_id;
		named_tag_predeclaration_details.order_id = 0; /*  Use the first one. */
		if(
			struct_named_tag_definition_details_to_struct_named_tag_definition_id_map_exists(
				&state->syntax_model->named_tag_definitions_map,
				named_tag_definition_details
			) ||
			struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map_exists(
				&state->syntax_model->named_tag_predeclarations_map,
				named_tag_predeclaration_details
			)
		){
			*result = current_scope;
			return 1;
		}
		if(scope_guids_ptr[current_scope.id].type == ASM_SCOPE_GUID_FILE_SCOPE){
			return 0;
		}else{
			current_scope = scope_guids_ptr[current_scope.id].parent;
		}
	}
}

void add_named_enum_definition(struct parser_state * state, struct enum_member_list_item_id enum_member_list_item_id, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id){
	struct tag_definition_id id1 = make_tag_definition_enum(state->syntax_model, enum_member_list_item_id);
	make_named_tag_definition_named_with_default_order(state->syntax_model, id1, scope_guid_id, identifier_id);
}

void add_named_struct_or_union_definition(struct parser_state * state, struct struct_or_union_member_list_item_id struct_or_union_member_list_item_id, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id, enum asm_unscoped_tag_specifier_kind kind){
	struct tag_definition_id id1;
	if(kind == ASM_UNSCOPED_TAG_SPECIFIER_STRUCT){
		id1 = make_tag_definition_struct(state->syntax_model, struct_or_union_member_list_item_id);
	}else{
		id1 = make_tag_definition_union(state->syntax_model, struct_or_union_member_list_item_id);
	}
	
	make_named_tag_definition_named_with_default_order(state->syntax_model, id1, scope_guid_id, identifier_id);
}

void add_anonymous_struct_or_union_definition(struct parser_state * state, struct struct_or_union_member_list_item_id struct_or_union_member_list_item_id, struct scope_guid_id scope_guid_id, struct anonymous_entry_id anonymous_entry_id, enum asm_unscoped_tag_specifier_kind kind){
	struct tag_definition_id id1;
	if(kind == ASM_UNSCOPED_TAG_SPECIFIER_STRUCT){
		id1 = make_tag_definition_struct(state->syntax_model, struct_or_union_member_list_item_id);
	}else{
		id1 = make_tag_definition_union(state->syntax_model, struct_or_union_member_list_item_id);
	}
	
	make_anonymous_tag_definition_anonymous(state->syntax_model, id1, scope_guid_id, anonymous_entry_id);
}

void add_anonymous_enum_definition(struct parser_state * state, struct enum_member_list_item_id enum_member_list_item_id, struct scope_guid_id scope_guid_id, struct anonymous_entry_id anonymous_entry_id){
	struct tag_definition_id id1 = make_tag_definition_enum(state->syntax_model, enum_member_list_item_id);
	make_anonymous_tag_definition_anonymous(state->syntax_model, id1, scope_guid_id, anonymous_entry_id);
}

struct parser_node * struct_or_union_specifier(struct parser_state * state, struct scoped_tag_specifier_id * scoped_tag_specifier_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	enum asm_unscoped_tag_specifier_kind kind;
	parser_progress("Attempting to build struct_or_union_specifier\n");
	if((n1 = struct_or_union(state, &kind))){
		struct scope_guid_id current_scope = get_current_scope_guid(state); 
		struct identifier_id identifier_id;
		if((n1->next = get_any_identifier_node(state, &identifier_id))){
			struct scope_guid_id struct_or_union_origin_scope;
			struct scope_guid_id closest_declared_scope;
			struct unscoped_tag_specifier_id named_unscoped_tag_specifier_id;
			(void)struct_or_union_origin_scope;
			if(kind == ASM_UNSCOPED_TAG_SPECIFIER_STRUCT){
				struct unscoped_struct_specifier_id non_anonymous_id = make_unscoped_struct_specifier_named(state->syntax_model, identifier_id);
				named_unscoped_tag_specifier_id = make_unscoped_tag_specifier_struct(state->syntax_model, non_anonymous_id);
			}else{
				struct unscoped_union_specifier_id non_anonymous_id = make_unscoped_union_specifier_named(state->syntax_model, identifier_id);
				named_unscoped_tag_specifier_id = make_unscoped_tag_specifier_union(state->syntax_model, non_anonymous_id);
			}

			if(get_scope_of_closest_tag_declaration(state, &closest_declared_scope, current_scope, identifier_id)){
				struct_or_union_origin_scope = closest_declared_scope;
			}else{
				/*  If there is no closest one, then this must declare an incomplete struct or union type. */
				make_named_tag_predeclaration_named_with_default_order(state->syntax_model, current_scope, identifier_id, kind);
				struct_or_union_origin_scope = current_scope;
			}
			*scoped_tag_specifier_id = make_scoped_tag_specifier_scoped_tag_specifier(state->syntax_model, struct_or_union_origin_scope, named_unscoped_tag_specifier_id);
			if((n1->next->next = p_accept(OPEN_BRACE_CHAR,state))){
				struct struct_or_union_member_list_item_id struct_or_union_member_list_item_id;

				if((n1->next->next->next = struct_declaration_list(state, make_struct_or_union_member_list_item_list_start(state->syntax_model), &struct_or_union_member_list_item_id))){
					if((n1->next->next->next->next = p_accept(CLOSE_BRACE_CHAR,state))){
						/*  If this is a struct or union definition, it wont complete the incomplete type from the outer scope */
						*scoped_tag_specifier_id = make_scoped_tag_specifier_scoped_tag_specifier(state->syntax_model, current_scope, named_unscoped_tag_specifier_id);
						add_named_struct_or_union_definition(state, struct_or_union_member_list_item_id, current_scope, identifier_id, kind);
						return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_OR_UNION_SPECIFIER);
					}else{
						add_parser_error(state, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_BRACE);
						return (struct parser_node *)0;
					}
				}else{
					add_parser_error(state, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_NO_DECLARATIONS);
					return (struct parser_node *)0;
				}
			}else{
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_OR_UNION_SPECIFIER);
			}
		}else if((n1->next = p_accept(OPEN_BRACE_CHAR, state))){
			struct struct_or_union_member_list_item_id struct_or_union_member_list_item_id;
			struct unscoped_tag_specifier_id anonymous_unscoped_tag_specifier_id;
			struct anonymous_entry_id anonymous_entry_id = make_anonymous_entry_anonymous_entry(state->syntax_model); 
			if(kind == ASM_UNSCOPED_TAG_SPECIFIER_STRUCT){
				struct unscoped_struct_specifier_id anonymous_id = make_unscoped_struct_specifier_anonymous(state->syntax_model, anonymous_entry_id);
			 	anonymous_unscoped_tag_specifier_id = make_unscoped_tag_specifier_struct(state->syntax_model, anonymous_id);
			}else{
				struct unscoped_union_specifier_id anonymous_id = make_unscoped_union_specifier_anonymous(state->syntax_model, anonymous_entry_id);
			 	anonymous_unscoped_tag_specifier_id = make_unscoped_tag_specifier_union(state->syntax_model, anonymous_id);
			}
			*scoped_tag_specifier_id = make_scoped_tag_specifier_scoped_tag_specifier(state->syntax_model, current_scope, anonymous_unscoped_tag_specifier_id);


			if((n1->next->next = struct_declaration_list(state, make_struct_or_union_member_list_item_list_start(state->syntax_model), &struct_or_union_member_list_item_id))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR,state))){
					add_anonymous_struct_or_union_definition(state, struct_or_union_member_list_item_id, current_scope, anonymous_entry_id, kind);
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_OR_UNION_SPECIFIER);
				}else{
					add_parser_error(state, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_CLOSE_BRACE);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_STRUCT_DECLARATION_LIST);
				return (struct parser_node *)0;
			}
		}else{
			parser_progress("Expected IDENTIFIER or open brace char.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected struct_declaration.\n");
		return (struct parser_node *)0;
	}
}

unsigned int specifier_list_is_only_void(struct parser_state * state, struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_item_id){
	/*  TODO:  Pretty sure this function can be moved later into the code generator when function artuments are evaluated.  */
	struct specifier_or_qualifier_list_item_details specifier_or_qualifier_list_item_details = get_specifier_or_qualifier_list_item_details_by_id(state->syntax_model, specifier_or_qualifier_list_item_id);
	if(specifier_or_qualifier_list_item_details.type == ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER){
		struct specifier_or_qualifier_id specifier_or_qualifier_id = specifier_or_qualifier_list_item_details.item_id;
		struct specifier_or_qualifier_details specifier_or_qualifier_details = get_specifier_or_qualifier_details_by_id(state->syntax_model, specifier_or_qualifier_id);
		if(specifier_or_qualifier_details.type == ASM_SPECIFIER_OR_QUALIFIER_AGGREGATE_TYPE_SPECIFIER){
			struct aggregate_type_specifier_id aggregate_type_specifier_id = get_aggregate_type_specifier_id_aggregate_type_specifier_from_specifier_or_qualifier(specifier_or_qualifier_details);
			struct aggregate_type_specifier_details aggregate_type_specifier_details = get_aggregate_type_specifier_details_by_id(state->syntax_model, aggregate_type_specifier_id);
			if(aggregate_type_specifier_details.type == ASM_AGGREGATE_TYPE_SPECIFIER_SIMPLE_TYPE_SPECIFIER){
				struct simple_type_specifier_id simple_type_specifier_id = get_simple_type_specifier_id_simple_type_specifier_from_aggregate_type_specifier(aggregate_type_specifier_details);
				struct simple_type_specifier_details simple_type_specifier_details = get_simple_type_specifier_details_by_id(state->syntax_model, simple_type_specifier_id);
				if(simple_type_specifier_details.type == ASM_SIMPLE_TYPE_SPECIFIER_VOID){
					return 1;
				}else{
					return 0;
				}
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}

struct parser_node * parameter_declaration(struct parser_state * state, struct general_type_id * result_general_type_id, struct simple_declaration_id * result_simple_declaration_id, unsigned int is_function_definition, unsigned int * parameter_list_count, unsigned int * contains_declaration){
	struct parser_node * n1;
	struct specifier_or_qualifier_list_item_id declaration_specifiers_list_id;
	struct bitfield_id bitfield_id = make_bitfield_no_bitfield(state->syntax_model);
	struct initializer_id initializer_id = make_initializer_no_initializer(state->syntax_model);
	parser_progress("Attempting to build parameter_declaration\n");
	if((n1 = declaration_specifiers(state, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), &declaration_specifiers_list_id))){
		struct declarator_part_list_item_id declarator_list_item_id;
		struct identifier_from_declarator identifier_id;
		struct any_statement_list_item_id discarded_param_declaration_statements;
		identifier_id.initialized = 0; /*  No id obtained yet. */
		if((n1->next = declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &declarator_list_item_id, &identifier_id, &discarded_param_declaration_statements, is_function_definition, parameter_list_count))){
			*result_general_type_id = make_general_type_general_type(state->syntax_model, declaration_specifiers_list_id, declarator_list_item_id, bitfield_id);
			assert(identifier_id.initialized);
			*result_simple_declaration_id = make_simple_declaration_simple_declaration(state->syntax_model, *result_general_type_id, identifier_id.id, initializer_id);
			make_identifier_namespace_entry_declaration_with_default_order(state->syntax_model, identifier_id.id, get_current_scope_guid(state), *result_general_type_id);

			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_DECLARATION);
		}else if((n1->next = abstract_declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &declarator_list_item_id))){
			*result_general_type_id = make_general_type_general_type(state->syntax_model, declaration_specifiers_list_id, declarator_list_item_id, bitfield_id);
			assert(is_function_definition == 0 || *parameter_list_count > 1); /*  Abstract declarators should not be used for function parameters. */
			*contains_declaration = 0;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_DECLARATION);
		}else{
			*result_general_type_id = make_general_type_general_type(state->syntax_model, declaration_specifiers_list_id, make_declarator_part_list_item_list_start(state->syntax_model), bitfield_id);
			*contains_declaration = 0;
			if(specifier_list_is_only_void(state, declaration_specifiers_list_id)){
				/*  If it is only a void specifier, then that's ok for function definitions because void is not a parameter. */
			}else{
				assert(is_function_definition == 0 || *parameter_list_count > 1); /*  No identifier if no declarator, so what's the parameter name? */
			}

			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_DECLARATION);
		}
	}else{
		parser_progress("Expected parameter_declaration.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * parameter_list_rest(struct parser_state * state, struct general_type_list_item_id prev_general_type_list_item_id, struct general_type_list_item_id * result_general_type_list, struct any_statement_list_item_id prev_declarations, struct any_statement_list_item_id * parameter_declarations, unsigned int is_function_definition, unsigned int *parameter_list_count){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int contains_declaration = 1; /*  Things like 'void' or unnamed parameters won't have an identifier, so flag this */
	parser_progress("Attempting to build parameter_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		struct general_type_id general_type_id;
		struct simple_declaration_id simple_declaration_id;
		if((n1->next = parameter_declaration(state, &general_type_id, &simple_declaration_id, is_function_definition, parameter_list_count, &contains_declaration))){
			struct general_type_list_item_id general_type_list_item_id = make_general_type_list_item_general_type(state->syntax_model, prev_general_type_list_item_id, general_type_id);
			if(is_function_definition && contains_declaration){
				struct declaration_statement_id declaration_statement_id = make_declaration_statement_simple_declaration(state->syntax_model, simple_declaration_id);
				struct any_statement_id any_statement_id = make_any_statement_declaration_statement(state->syntax_model, declaration_statement_id);
				*parameter_declarations = make_any_statement_list_item_any_statement(state->syntax_model, prev_declarations, any_statement_id);
			}else{
				*parameter_declarations = prev_declarations;
			}
			if((n1->next->next = parameter_list_rest(state, general_type_list_item_id, result_general_type_list, *parameter_declarations, parameter_declarations, is_function_definition, parameter_list_count))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_LIST_REST);
			}else{
				assert(0 && "Not possible\n");
				return (struct parser_node *)0;
			}
		}else{
			/* Re-wind back over the ',' that we accepted, so it can be accepted by a different rule */
			backtrack(state, checkpoint);
			n1 = make_epsilon(state);
			*result_general_type_list = prev_general_type_list_item_id;
			*parameter_declarations = prev_declarations;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_LIST_REST);
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*result_general_type_list = prev_general_type_list_item_id;
		*parameter_declarations = prev_declarations;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, PARAMETER_LIST_REST);
	}
}

struct parser_node * parameter_list(struct parser_state * state, struct general_type_list_item_id * this_list_id, struct any_statement_list_item_id * parameter_declarations, unsigned int is_function_definition, unsigned int *parameter_list_count){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct general_type_id general_type_id;
	struct simple_declaration_id simple_declaration_id;
	unsigned int contains_declaration = 1; /*  Things like 'void' or unnamed parameters won't have an identifier, so flag this */
	parser_progress("Attempting to build parameter_list\n");
	if((n1 = parameter_declaration(state, &general_type_id, &simple_declaration_id, is_function_definition, parameter_list_count, &contains_declaration))){
		struct general_type_list_item_id general_type_list_item_id = make_general_type_list_item_list_start(state->syntax_model);
		general_type_list_item_id = make_general_type_list_item_general_type(state->syntax_model, general_type_list_item_id, general_type_id);
		if(is_function_definition && contains_declaration){
			struct declaration_statement_id declaration_statement_id = make_declaration_statement_simple_declaration(state->syntax_model, simple_declaration_id);
			struct any_statement_id any_statement_id = make_any_statement_declaration_statement(state->syntax_model, declaration_statement_id);
			*parameter_declarations = make_any_statement_list_item_any_statement(state->syntax_model, make_any_statement_list_item_list_start(state->syntax_model), any_statement_id);
		}else{
			*parameter_declarations = make_any_statement_list_item_list_start(state->syntax_model);
		}
		if((n1->next = parameter_list_rest(state, general_type_list_item_id, this_list_id, *parameter_declarations, parameter_declarations, is_function_definition, parameter_list_count))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_LIST);
		}else{
			parser_progress("Expected parameter_list_rest.\n");
			backtrack(state, checkpoint);
			*parameter_declarations = make_any_statement_list_item_list_start(state->syntax_model);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected parameter_declaration.\n");
		*parameter_declarations = make_any_statement_list_item_list_start(state->syntax_model);
		return (struct parser_node *)0;
	}
}

struct parser_node * parameter_type_list(struct parser_state * state, struct parameter_list_id *parameter_list_id, struct any_statement_list_item_id * parameter_declarations, unsigned int is_function_definition, unsigned int *parameter_list_count){
	struct parser_node * n1;
	struct general_type_list_item_id general_types;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	/*  Function declarations and function definitions both start a new scope in the parameter list. */
	if(is_function_definition == 1 && *parameter_list_count == 1){
		increment_scope_depth(state, SCOPE_LEVEL_TYPE_FUNCTION_SCOPE);
	}else{
		increment_scope_depth(state, SCOPE_LEVEL_TYPE_FUNCTION_PROTOTYPE_SCOPE);
	}
	parser_progress("Attempting to build parameter_type_list\n");
	if((n1 = parameter_list(state, &general_types, parameter_declarations, is_function_definition, parameter_list_count))){
		if((n1->next = p_accept(COMMA_CHAR, state))){
			if((n1->next->next = p_accept(ELLIPSIS, state))){
				*parameter_list_id = make_parameter_list_variadic(state->syntax_model, general_types);
				if(is_function_definition == 0 || *parameter_list_count > 1){ /*  For a declaration, we go back to file scope after the prototype. */
					*parameter_list_count = *parameter_list_count - 1;
					decrement_scope_depth(state);
				}
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_TYPE_LIST);
			}else{
				add_parser_error(state, PARSER_ERROR_PARAMETER_TYPE_LIST_MISSING_ELLIPSIS);
				*parameter_declarations = make_any_statement_list_item_list_start(state->syntax_model);
				backtrack(state, checkpoint);
				return (struct parser_node *)0;
			}
		}else{
			*parameter_list_id = make_parameter_list_non_variadic(state->syntax_model, general_types);
			if(is_function_definition == 0 || *parameter_list_count > 1){ /*  For a declaration, we go back to file scope after the prototype. */
				*parameter_list_count = *parameter_list_count - 1;
				decrement_scope_depth(state);
			}
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_TYPE_LIST);
		}
	}else{
		parser_progress("Expected parameter_list.\n");
		*parameter_declarations = make_any_statement_list_item_list_start(state->syntax_model);
		backtrack(state, checkpoint);
		return (struct parser_node *)0;
	}
}



struct parser_node * direct_abstract_declarator_rest(struct parser_state * state, struct declarator_part_list_item_id previous_declarator_item_list_id, struct declarator_part_list_item_id * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct any_statement_list_item_id parameter_list_declaration_statements;
	parser_progress("Attempting to build direct_abstract_declarator_rest\n");
	if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		struct parameter_list_id parameter_list_id;
		unsigned int is_function_definition = 0;
		unsigned int parameter_list_count = 0;
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, state))){
			struct declarator_part_list_item_id current_declarator_part_id;
			struct parameter_list_id empty_variadic_parameter_list = make_parameter_list_variadic(state->syntax_model, make_general_type_list_item_list_start(state->syntax_model));
			current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_function(state->syntax_model, make_function_part_k_and_r_c(state->syntax_model, empty_variadic_parameter_list)));
			if((n1->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_NO_PARAM);
				return (struct parser_node *)0;
			}
		}else if((n1->next = parameter_type_list(state, &parameter_list_id, &parameter_list_declaration_statements, is_function_definition, &parameter_list_count))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				struct declarator_part_list_item_id current_declarator_part_id;
				current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_function(state->syntax_model, make_function_part_prototype(state->syntax_model, parameter_list_id)));
				if((n1->next->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_WITH_PARAM);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_PARAMS);
				return (struct parser_node *)0;
			}
		}else{
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, state))){
		struct expression_id e1;
		if((n1->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
			struct declarator_part_list_item_id current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_array(state->syntax_model, make_array_part_flexible(state->syntax_model)));
			if((n1->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_NO_SIZE);
				return (struct parser_node *)0;
			}
		}else if((n1->next = constant_expression(state, &e1))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
				struct declarator_part_list_item_id current_declarator_part_id;
				current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_array(state->syntax_model, make_array_part_expression(state->syntax_model, e1)));
				if((n1->next->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_SIZED);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_AFTER_SIZED);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_OR_CONSTANT);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = previous_declarator_item_list_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
	}
}


struct parser_node * direct_abstract_declarator(struct parser_state * state, struct declarator_part_list_item_id previous_declarator_item_list_id, struct declarator_part_list_item_id * this_list_id){
	struct parser_node * n1;
	struct any_statement_list_item_id parameter_list_declaration_statements;
	unsigned int is_function_definition = 0;
	unsigned int parameter_list_count = 0;
	parser_progress("Attempting to build direct_abstract_declarator\n");
	if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		struct parameter_list_id parameter_list_id;
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, state))){
			struct declarator_part_list_item_id current_declarator_part_id;
			struct parameter_list_id empty_variadic_parameter_list = make_parameter_list_variadic(state->syntax_model, make_general_type_list_item_list_start(state->syntax_model));
			current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_function(state->syntax_model, make_function_part_k_and_r_c(state->syntax_model, empty_variadic_parameter_list)));
			if((n1->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_NO_PARAM);
				return (struct parser_node *)0;
			}
		}else if((n1->next = parameter_type_list(state, &parameter_list_id, &parameter_list_declaration_statements, is_function_definition, &parameter_list_count))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				struct declarator_part_list_item_id current_declarator_part_id;
				current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_function(state->syntax_model, make_function_part_prototype(state->syntax_model, parameter_list_id)));
				if((n1->next->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_WITH_PARAM);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_PAREN_AFTER_PARAMS);
				return (struct parser_node *)0;
			}
		}else if((n1->next = abstract_declarator(state, previous_declarator_item_list_id, this_list_id))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				if((n1->next->next->next = direct_abstract_declarator_rest(state, *this_list_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_ABSTRACT_DECLARATOR);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_PAREN_AFTER_ABSTRACT_DECLARATOR);
				return (struct parser_node *)0;
			}
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
		}
	}else if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, state))){
		struct expression_id e1;
		if((n1->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
			struct declarator_part_list_item_id current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_array(state->syntax_model, make_array_part_flexible(state->syntax_model)));
			if((n1->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_NO_SIZE);
				return (struct parser_node *)0;
			}
		}else if((n1->next = constant_expression(state, &e1))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
				struct declarator_part_list_item_id current_declarator_part_id;
				current_declarator_part_id = make_declarator_part_list_item_declarator_part(state->syntax_model, previous_declarator_item_list_id, make_declarator_part_array(state->syntax_model, make_array_part_expression(state->syntax_model, e1)));
				if((n1->next->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_SIZED);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_BRACKET_AFTER_SIZED);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_BRACKET_OR_CONSTANT);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected OPEN_SQUARE_BRACKET_CHAR or OPEN_PAREN_CHAR.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * type_name(struct parser_state * state, struct general_type_id * general_type_id){
	struct parser_node * n1;
	struct specifier_or_qualifier_list_item_id asm_specifier_qualifier_list_id;
	struct declarator_part_list_item_id declarator_item_list_id;
	parser_progress("Attempting to build type_name\n");
	if((n1 = specifier_qualifier_list(state, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), &asm_specifier_qualifier_list_id))){
		if((n1->next = abstract_declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &declarator_item_list_id))){
			*general_type_id = make_general_type_general_type(state->syntax_model, asm_specifier_qualifier_list_id, declarator_item_list_id, make_bitfield_no_bitfield(state->syntax_model));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_NAME);
		}else{
			*general_type_id = make_general_type_general_type(state->syntax_model, asm_specifier_qualifier_list_id, make_declarator_part_list_item_list_start(state->syntax_model), make_bitfield_no_bitfield(state->syntax_model));
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_NAME);
		}
	}else{
		parser_progress("Expected specifier_qualifier_list.\n");
		return (struct parser_node *)0;
	}
}


struct parser_node * abstract_declarator(struct parser_state * state, struct declarator_part_list_item_id previous_declarator_item_list_id, struct declarator_part_list_item_id * this_list_id){
	struct parser_node * n1;
	struct declarator_part_list_item_id pointer_declarator_list_id;
	struct declarator_part_list_item_id direct_declarator_list_id;
	parser_progress("Attempting to build abstract_declarator\n");
	if((n1 = pointer(state, make_declarator_part_list_item_list_start(state->syntax_model), &pointer_declarator_list_id))){
		if((n1->next = direct_abstract_declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &direct_declarator_list_id))){
			/*  Special case: */
			*this_list_id = add_pointer_and_direct_declarators_to_list(state->syntax_model, previous_declarator_item_list_id, pointer_declarator_list_id, direct_declarator_list_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ABSTRACT_DECLARATOR);
		}else{
			add_declarators_to_list(state->syntax_model, &previous_declarator_item_list_id, pointer_declarator_list_id, 1);
			*this_list_id = previous_declarator_item_list_id;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ABSTRACT_DECLARATOR);
		}
	}else if((n1 = direct_abstract_declarator(state, previous_declarator_item_list_id, this_list_id))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ABSTRACT_DECLARATOR);
	}else{
		parser_progress("Expected abstract_declarator.\n");
		return (struct parser_node *)0;
	}
}

void increment_scope_depth(struct parser_state * state, enum scope_level_type t){
	struct scope_guid_id scope_guid_id;
	switch(t){
		case SCOPE_LEVEL_TYPE_FUNCTION_SCOPE:{
			scope_guid_id = make_scope_guid_function_scope(state->syntax_model, get_current_scope_guid(state));
			break;
		}case SCOPE_LEVEL_TYPE_FUNCTION_PROTOTYPE_SCOPE:{
			scope_guid_id = make_scope_guid_function_prototype_scope(state->syntax_model, get_current_scope_guid(state));
			break;
		}case SCOPE_LEVEL_TYPE_FILE_SCOPE:{
			scope_guid_id = make_scope_guid_file_scope(state->syntax_model);
			break;
		}case SCOPE_LEVEL_TYPE_BLOCK_SCOPE:{
			scope_guid_id = make_scope_guid_block_scope(state->syntax_model, get_current_scope_guid(state));
			break;
		}default:{
			scope_guid_id.id = 0;
			assert(0);
		}
	}
	struct_scope_guid_id_list_add_end(&state->scope_guid_stack, scope_guid_id);
	push_operation(state, INCREMENT_SCOPE_DEPTH, (void*)0);
}

void decrement_scope_depth(struct parser_state * state){
	push_operation(state, DECREMENT_SCOPE_DEPTH, (void*)0);
}

struct parser_node * labeled_statement(struct parser_state * state, struct labeled_statement_id * labeled_statement_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct identifier_id identifier_id;
	parser_progress("Attempting to build labeled_statement\n");
	if((n1 = get_any_identifier_node(state, &identifier_id))){
		if((n1->next = p_accept(COLON_CHAR, state))){
			struct any_statement_list_item_id list;
			increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
			if((n1->next->next = statement(state, 0, make_any_statement_list_item_list_start(state->syntax_model), &list))){
				struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, list, get_current_scope_guid(state));
				*labeled_statement_id = make_labeled_statement_goto_label(state->syntax_model, scoped_statement_id, identifier_id);
				decrement_scope_depth(state);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LABELED_STATEMENT);
			}else{
				add_parser_error(state, PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_LABEL);
				return (struct parser_node *)0;
			}
		}else{
			parser_progress("Assuming not a label, resetting position.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(CASE,state))){
		struct expression_id e1;
		if((n1->next = constant_expression(state, &e1))){
			if((n1->next->next = p_accept(COLON_CHAR, state))){
				struct any_statement_list_item_id list;
				increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
				if((n1->next->next->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &list))){
					struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, list, get_current_scope_guid(state));
					*labeled_statement_id = make_labeled_statement_case_label(state->syntax_model, scoped_statement_id, e1);
					decrement_scope_depth(state);
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LABELED_STATEMENT);
				}else{
					add_parser_error(state, PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_CASE);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_LABELED_STATEMENT_MISSING_COLON_AFTER_CASE);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_LABELED_STATEMENT_MISSING_CONSTANT_AFTER_CASE);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DEFAULT,state))){
		if((n1->next = p_accept(COLON_CHAR, state))){
			struct any_statement_list_item_id list;
			increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
			if((n1->next->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &list))){
				struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, list, get_current_scope_guid(state));
				*labeled_statement_id = make_labeled_statement_default_label(state->syntax_model, scoped_statement_id);
				decrement_scope_depth(state);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LABELED_STATEMENT);
			}else{
				add_parser_error(state, PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_DEFAULT);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_LABELED_STATEMENT_MISSING_COLON_AFTER_DEFAULT);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected labeled_statement.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * selection_statement(struct parser_state * state, struct code_statement_id * code_statement_id){
	struct parser_node * n1;
	(void)code_statement_id;
	parser_progress("Attempting to build selection_statement\n");
	if((n1 = p_accept(IF,state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, state))){
			struct expression_id e1;
			if((n1->next->next = expression(state, &e1))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					struct any_statement_list_item_id l1;
					increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
					if((n1->next->next->next->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &l1))){
						struct scoped_statement_id scoped_statement_id_1 = make_scoped_statement_scoped_statement(state->syntax_model, l1, get_current_scope_guid(state));
						decrement_scope_depth(state);
						if((n1->next->next->next->next->next = p_accept(ELSE, state))){
							struct any_statement_list_item_id l2;
							increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
							if((n1->next->next->next->next->next->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &l2))){
								struct scoped_statement_id scoped_statement_id_2 = make_scoped_statement_scoped_statement(state->syntax_model, l2, get_current_scope_guid(state));
								struct if_statement_id if_statement_id = make_if_statement_if_else(state->syntax_model, scoped_statement_id_1, e1, scoped_statement_id_2);
								*code_statement_id = make_code_statement_if_statement(state->syntax_model, if_statement_id);
								decrement_scope_depth(state);
								return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SELECTION_STATEMENT);
							}else{
								add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_ELSE);
								return (struct parser_node *)0;
							}
						}else{

							struct if_statement_id if_statement_id = make_if_statement_just_if(state->syntax_model, scoped_statement_id_1, e1);
							*code_statement_id = make_code_statement_if_statement(state->syntax_model, if_statement_id);
							return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SELECTION_STATEMENT);
						}
					}else{
						add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_IF);
						return (struct parser_node *)0;
					}
				}else{
					add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_IF);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_EXPRESSION_AFTER_IF);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_OPEN_PAREN_AFTER_IF);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(SWITCH, state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, state))){
			struct expression_id e1;
			if((n1->next->next = expression(state, &e1))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					struct any_statement_list_item_id l1;
					increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
					if((n1->next->next->next->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &l1))){
						struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, l1, get_current_scope_guid(state));
						struct switch_statement_id switch_statement_id = make_switch_statement_switch(state->syntax_model, e1, scoped_statement_id);
						*code_statement_id = make_code_statement_switch_statement(state->syntax_model, switch_statement_id);
						decrement_scope_depth(state);
						return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SELECTION_STATEMENT);
					}else{
						add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_SWITCH);
						return (struct parser_node *)0;
					}
				}else{
					add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_SWITCH);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_EXPRESSION_AFTER_SWITCH);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_SELECTION_STATEMENT_MISSING_OPEN_PAREN_AFTER_SWITCH);
			return (struct parser_node *)0;
		}
	}else {
		parser_progress("Expected IF or SWITCH.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * iteration_statement(struct parser_state * state, struct code_statement_id * code_statement_id){
	struct parser_node * n1;
	parser_progress("Attempting to build iteration_statement\n");
	if((n1 = p_accept(WHILE,state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, state))){
			struct expression_id e1;
			if((n1->next->next = expression(state, &e1))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					struct any_statement_list_item_id l1;
					increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
					if((n1->next->next->next->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &l1))){
						struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, l1, get_current_scope_guid(state));
						struct while_statement_id while_statement_id = make_while_statement_while(state->syntax_model, e1, scoped_statement_id);
						*code_statement_id = make_code_statement_while_statement(state->syntax_model, while_statement_id);
						decrement_scope_depth(state);
						return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ITERATION_STATEMENT);
					}else{
						add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_WHILE_CLOSE_PAREN);
						return (struct parser_node *)0;
					}
				}else{
					add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_WHILE);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_AFTER_WHILE);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_WHILE);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DO,state))){
		struct any_statement_list_item_id l1;
		increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
		if((n1->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &l1))){
			struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, l1, get_current_scope_guid(state));
			decrement_scope_depth(state);
			if((n1->next->next = p_accept(WHILE, state))){
				if((n1->next->next->next = p_accept(OPEN_PAREN_CHAR, state))){
					struct expression_id e1;
					if((n1->next->next->next->next = expression(state, &e1))){
						if((n1->next->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
							if((n1->next->next->next->next->next->next = p_accept(SEMICOLON_CHAR, state))){
								struct while_statement_id while_statement_id  = make_while_statement_do_while(state->syntax_model, e1, scoped_statement_id);
								*code_statement_id = make_code_statement_while_statement(state->syntax_model, while_statement_id);
								return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ITERATION_STATEMENT);
							}else{
								add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_SEMICOLON_AFTER_DO_WHILE);
								return (struct parser_node *)0;
							}
						}else{
							add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_DO_WHILE);
							return (struct parser_node *)0;
						}
					}else{
						add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_AFTER_DO_WHILE);
						return (struct parser_node *)0;
					}
				}else{
					add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_DO_WHILE);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_WHILE_AFTER_DO_WHILE);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_DO_WHILE);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(FOR,state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR,state))){
			struct expression_id e1;
			if((n1->next->next = expression_statement(state, &e1))){
				struct expression_id e2;
				if((n1->next->next->next = expression_statement(state, &e2))){
					struct code_statement_id c1 = make_code_statement_expression_statement(state->syntax_model, e1);
					struct any_statement_id a1 = make_any_statement_code_statement(state->syntax_model, c1);
					struct code_statement_id c2 = make_code_statement_expression_statement(state->syntax_model, e2);
					struct any_statement_id a2 = make_any_statement_code_statement(state->syntax_model, c2);
					struct expression_id e3;
					if((n1->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
						struct any_statement_list_item_id l1;
						increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
						if((n1->next->next->next->next->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &l1))){
							struct expression_id en3  = make_expression_nullary_expression(state->syntax_model);
							struct code_statement_id c3 = make_code_statement_expression_statement(state->syntax_model, en3);
							struct any_statement_id a3 = make_any_statement_code_statement(state->syntax_model, c3);
							struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, l1, get_current_scope_guid(state));
							struct for_statement_id for_statement_id = make_for_statement_for(state->syntax_model, a1, a2, a3, scoped_statement_id);
							*code_statement_id = make_code_statement_for_statement(state->syntax_model, for_statement_id);
							decrement_scope_depth(state);
							return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ITERATION_STATEMENT);
						}else{
							add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_SMALL_FOR);
							return (struct parser_node *)0;
						}
					}else if((n1->next->next->next->next = expression(state, &e3))){
						struct code_statement_id c3 = make_code_statement_expression_statement(state->syntax_model, e3);
						struct any_statement_id a3 = make_any_statement_code_statement(state->syntax_model, c3);
						if((n1->next->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
							struct any_statement_list_item_id l1;
							increment_scope_depth(state, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
							if((n1->next->next->next->next->next->next = statement(state, 1, make_any_statement_list_item_list_start(state->syntax_model), &l1))){
								struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, l1, get_current_scope_guid(state));
								struct for_statement_id for_statement_id = make_for_statement_for(state->syntax_model, a1, a2, a3, scoped_statement_id);
								*code_statement_id = make_code_statement_for_statement(state->syntax_model, for_statement_id);
								decrement_scope_depth(state);
								return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ITERATION_STATEMENT);
							}else{
								add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_BIG_FOR);
								return (struct parser_node *)0;
							}
						}else{
							add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_FOR);
							return (struct parser_node *)0;
						}
					}else{
						add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_OR_CLOSE_PAREN_AFTER_FOR);
						return (struct parser_node *)0;
					}
				}else{
					add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_STATEMENT_2_AFTER_FOR);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_STATEMENT_1_AFTER_FOR);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_FOR);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected WHILE DO or FOR.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * jump_statement(struct parser_state * state, struct jump_statement_id *jump_statement_id){
	struct parser_node * n1;
	struct identifier_id identifier_id;
	parser_progress("Attempting to build jump_statement\n");
	if((n1 = p_accept(GOTO,state))){
		if((n1->next = get_any_identifier_node(state, &identifier_id))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, state))){
				*jump_statement_id = make_jump_statement_goto(state->syntax_model, identifier_id);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
			}else{
				add_parser_error(state, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_GOTO);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_JUMP_STATEMENT_MISSING_IDENTIFIER_AFTER_GOTO);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(CONTINUE,state))){
		if((n1->next= p_accept(SEMICOLON_CHAR, state))){
			*jump_statement_id = make_jump_statement_continue(state->syntax_model);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
		}else{
			add_parser_error(state, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_CONTINUE);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(BREAK,state))){
		if((n1->next= p_accept(SEMICOLON_CHAR, state))){
			*jump_statement_id = make_jump_statement_break(state->syntax_model);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
		}else{
			add_parser_error(state, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_BREAK);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(RETURN,state))){
		struct expression_id e1;
		if((n1->next = p_accept(SEMICOLON_CHAR, state))){
			*jump_statement_id = make_jump_statement_return_no_expression(state->syntax_model);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
		}else if((n1->next = expression(state, &e1))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, state))){
				*jump_statement_id = make_jump_statement_return_expression(state->syntax_model, e1);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
			}else{
				add_parser_error(state, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_RETURN);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AND_EXPRESSION_AFTER_RETURN);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected jump_statement.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * function_definition(struct parser_state * state, struct function_definition_id * function_definition_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	struct specifier_or_qualifier_list_item_id declaration_specifiers_list_id;
	struct declarator_part_list_item_id declarator_item_list_id;
	struct identifier_from_declarator identifier_id;
	struct any_statement_list_item_id body_statements;
	struct any_statement_list_item_id param_declaration_statements;
	unsigned int is_function_definition = 1;
	unsigned int parameter_list_count = 0;
	struct function_definition_helper h;
	h.for_function_definition = 1;
	identifier_id.initialized = 0; /*  No id obtained yet. */
	parser_progress("Attempting to build function_definition\n");
	if((n1 = declaration_specifiers(state, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), &declaration_specifiers_list_id))){
		if((n1->next = declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &declarator_item_list_id, &identifier_id, &param_declaration_statements, is_function_definition, &parameter_list_count))){
			struct any_statement_list_item_id k_r_declarations;
			struct general_type_id general_type_id = make_general_type_general_type(state->syntax_model, declaration_specifiers_list_id, declarator_item_list_id, make_bitfield_no_bitfield(state->syntax_model));
			assert(identifier_id.initialized);
			h.identifier_id = identifier_id.id;
			h.general_type_id = general_type_id;
			if((n1->next->next = declaration_list(state, make_any_statement_list_item_list_start(state->syntax_model), &k_r_declarations))){
				if((n1->next->next->next = compound_statement(state, &h, 1, &body_statements))){
					struct scoped_statement_id scoped_body_statements = make_scoped_statement_scoped_statement(state->syntax_model, body_statements, get_current_scope_guid(state));

					*function_definition_id = make_function_definition_function_definition(state->syntax_model, general_type_id, k_r_declarations, param_declaration_statements, scoped_body_statements, identifier_id.id);
					decrement_scope_depth(state);
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, FUNCTION_DEFINITION);
				}else{
					parser_progress("Expected compound_statement.\n");
					backtrack(state, checkpoint);
					return (struct parser_node *)0;
				}
			}else if((n1->next->next = compound_statement(state, &h, 1, &body_statements))){
				struct scoped_statement_id scoped_body_statements = make_scoped_statement_scoped_statement(state->syntax_model, body_statements, get_current_scope_guid(state));
				*function_definition_id = make_function_definition_function_definition(state->syntax_model, general_type_id, make_any_statement_list_item_list_start(state->syntax_model), param_declaration_statements, scoped_body_statements, identifier_id.id);
				decrement_scope_depth(state);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, FUNCTION_DEFINITION);
			}else{
				parser_progress("Expected compound_statement or declaration_list for function definition.\n");
				backtrack(state, checkpoint);
				return (struct parser_node *)0;
			}
		}else{
			backtrack(state, checkpoint);
			buffered_printf(state->buffered_output,"Unable to complete function definition, putting back tokens line %d.\n", state->line_number);
			return (struct parser_node *)0;
		}
	}else if((n1 = declarator(state, make_declarator_part_list_item_list_start(state->syntax_model), &declarator_item_list_id, &identifier_id, &param_declaration_statements, is_function_definition, &parameter_list_count))){
		struct any_statement_list_item_id k_r_declarations;
		struct general_type_id general_type_id = make_general_type_general_type(state->syntax_model, make_specifier_or_qualifier_list_item_list_start(state->syntax_model), declarator_item_list_id, make_bitfield_no_bitfield(state->syntax_model));
		assert(identifier_id.initialized);
		h.identifier_id = identifier_id.id;
		h.general_type_id = general_type_id;
		if((n1->next = declaration_list(state, make_any_statement_list_item_list_start(state->syntax_model), &k_r_declarations))){
			if((n1->next->next = compound_statement(state, &h, 1, &body_statements))){
				struct scoped_statement_id scoped_body_statements = make_scoped_statement_scoped_statement(state->syntax_model, body_statements, get_current_scope_guid(state));
				*function_definition_id = make_function_definition_function_definition(state->syntax_model, general_type_id, k_r_declarations, param_declaration_statements, scoped_body_statements, identifier_id.id);
				decrement_scope_depth(state);
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, FUNCTION_DEFINITION);
			}else{
				parser_progress("Expected compound_statement.\n");
				backtrack(state, checkpoint);
				return (struct parser_node *)0;
			}
		}else if((n1->next = compound_statement(state, &h, 1, &body_statements))){
			struct scoped_statement_id scoped_body_statements = make_scoped_statement_scoped_statement(state->syntax_model, body_statements, get_current_scope_guid(state));
			*function_definition_id = make_function_definition_function_definition(state->syntax_model, general_type_id, make_any_statement_list_item_list_start(state->syntax_model), param_declaration_statements, scoped_body_statements, identifier_id.id);
			decrement_scope_depth(state);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, FUNCTION_DEFINITION);
		}else{
			backtrack(state, checkpoint);
			buffered_printf(state->buffered_output,"Unable to complete function definition, putting back tokens line %d..\n", state->line_number);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected declarator or declaration_specifiers for function definition.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * external_declaration(struct parser_state * state, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct function_definition_id function_definition_id;
	parser_progress("Attempting to build external_declaration\n");
	if((n1 = declaration(state, prev_any_statement_list_item_id, result_statement_list))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXTERNAL_DECLARATION);
	}else if((n1 = function_definition(state, &function_definition_id))){
		struct declaration_statement_id declaration_statement_id = make_declaration_statement_function_definition(state->syntax_model, function_definition_id);
		struct any_statement_id any_statement_id = make_any_statement_declaration_statement(state->syntax_model, declaration_statement_id);
		*result_statement_list = make_any_statement_list_item_any_statement(state->syntax_model, prev_any_statement_list_item_id, any_statement_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXTERNAL_DECLARATION);
	}else{
		parser_progress("Expected external declaration.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * translation_unit_rest(struct parser_state * state, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build translation_unit_rest\n");
	if((n1 = external_declaration(state, prev_any_statement_list_item_id, result_statement_list))){
		if((n1->next = translation_unit_rest(state, *result_statement_list, result_statement_list))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TRANSLATION_UNIT_REST);
		}else{
			parser_progress("Expected translation_unit_rest.\n");
			*result_statement_list = prev_any_statement_list_item_id;
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*result_statement_list = prev_any_statement_list_item_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, TRANSLATION_UNIT_REST);
	}
}

struct scope_guid_id get_current_scope_guid(struct parser_state * state){
	unsigned int size = struct_scope_guid_id_list_size(&state->scope_guid_stack);
	assert(size);
	return struct_scope_guid_id_list_get(&state->scope_guid_stack, size -1);
}

struct any_statement_list_item_id add_scoped_any_statement(struct parser_state * state, struct any_statement_list_item_id prev_any_statement_list_item_id, struct any_statement_list_item_id scoped_statement_list){
	struct scope_guid_id current_scope = get_current_scope_guid(state);
	struct scoped_statement_id scoped_statement_id = make_scoped_statement_scoped_statement(state->syntax_model, scoped_statement_list, current_scope);
	struct any_statement_id new_statement = make_any_statement_scoped_statement(state->syntax_model, scoped_statement_id);
	return make_any_statement_list_item_any_statement(state->syntax_model, prev_any_statement_list_item_id, new_statement);
}

struct parser_node * translation_unit(struct parser_state * state, struct any_statement_list_item_id * result_statement_list){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	*result_statement_list = make_any_statement_list_item_list_start(state->syntax_model);

	parser_progress("Attempting to build translation_unit\n");
	if((n1 = external_declaration(state, *result_statement_list, result_statement_list))){
		if((n1->next = translation_unit_rest(state, *result_statement_list, result_statement_list))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TRANSLATION_UNIT);
		}else{
			parser_progress("Expected translation_unit_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected external_declaration.\n");
		return (struct parser_node *)0;
	}
}

void print_parser_node_tree(struct unsigned_char_list * buffer, struct parser_node * parent, struct parser_node * n, unsigned int level){
	/*  This print function is useful for debugging how the parser constructs the parser tree.  */
	if(n){
		char * parent_str;
		if(parent){
			parent_str = (char *)get_node_type_names()[parent->type];
		}else{
			parent_str = (char *)"Root";
		}
		if(n->type == TERMINAL){
			buffered_printf(buffer, ";Tree level %d, %s -> %s %s\n", level, parent_str, get_node_type_names()[n->type], get_c_token_type_names()[n->c_lexer_token->type]);
		}else{
			buffered_printf(buffer, ";Tree level %d, %s -> %s\n", level, parent_str, get_node_type_names()[n->type]);
		}
		print_parser_node_tree(buffer, parent, n->next, level);
		print_parser_node_tree(buffer, n, n->first_child, level + 1);
	}
}

void print_scope_level(struct parser_state * state, struct scope_level * scope, unsigned int level){
	unsigned int j;
	(void)scope;

	buffered_puts(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_puts(state->buffered_output,"-");
	buffered_puts(state->buffered_output,">BEGIN scope details\n");
	buffered_puts(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_puts(state->buffered_output,"-");
	buffered_puts(state->buffered_output,">Identifiers:\n");
	buffered_puts(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_puts(state->buffered_output,"-");

	buffered_puts(state->buffered_output,">Tags:\n");
		
	buffered_puts(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_puts(state->buffered_output,"-");
	buffered_puts(state->buffered_output,">END scope details\n");
}



enum node_type get_specifier_parser_node_type(enum c_token_type t){
	switch(t){
		case VOID:{ return TYPE_SPECIFIER; }
		case CHAR:{ return TYPE_SPECIFIER; }
		case SHORT:{ return TYPE_SPECIFIER; }
		case INT:{ return TYPE_SPECIFIER; }
		case LONG:{ return TYPE_SPECIFIER; }
		case FLOAT:{ return TYPE_SPECIFIER; }
		case DOUBLE:{ return TYPE_SPECIFIER; }
		case SIGNED:{ return TYPE_SPECIFIER; }
		case UNSIGNED:{ return TYPE_SPECIFIER; }
		case TYPEDEF:{ return STORAGE_CLASS_SPECIFIER; }
		case EXTERN:{ return STORAGE_CLASS_SPECIFIER; }
		case STATIC:{ return STORAGE_CLASS_SPECIFIER; }
		case AUTO:{ return STORAGE_CLASS_SPECIFIER; }
		case CONST:{ return TYPE_QUALIFIER; }
		case VOLATILE:{ return TYPE_QUALIFIER; }
		default:{ assert(0 && "Not possible."); return (enum node_type)0; }
	}
}

unsigned int parse(struct parser_state * state){
	unsigned int print_scope_active = 0;
	struct any_statement_list_item_id result_list;
	struct any_statement_list_item_id scoped_list;

	increment_scope_depth(state, SCOPE_LEVEL_TYPE_FILE_SCOPE);
	state->top_node = translation_unit(state, &scoped_list);
	result_list = add_scoped_any_statement(state, make_any_statement_list_item_list_start(state->syntax_model), scoped_list);
	(void)result_list;
	decrement_scope_depth(state);

	if(state->tokens_position != struct_c_lexer_token_ptr_list_size(&state->c_lexer_state->tokens)){
		add_parser_error(state, PARSER_ERROR_INCOMPLETE_PARSE);
	}

	if(!state->top_node){
		add_parser_error(state, PARSER_ERROR_PARSING_FAILED);
	}

	if(print_scope_active){
	}
	return 0;
}

void create_parser_state(struct parser_state * state, struct memory_pool_collection * memory_pool_collection, struct c_lexer_state * c_lexer_state, struct unsigned_char_list * buffered_output, unsigned char * buffer, struct asm_state * asm_state){
	state->syntax_model = asm_state;
	state->buffered_output = buffered_output;
	state->memory_pool_collection = memory_pool_collection;
	state->c_lexer_state = c_lexer_state;
	state->line_number = 1;
	state->tokens_position = 0;
	struct_parser_operation_list_create(&state->operation_stack);
	state->buff = buffer;
	unsigned_char_ptr_to_struct_constant_description_ptr_map_create(&state->constant_map, struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_compare);
	struct_parser_error_list_create(&state->parser_errors);
	struct_scope_guid_id_list_create(&state->scope_guid_stack);
	struct_scope_guid_id_list_create(&state->decremented_scope_guids); /*  Possibly get rid of this in future: used for unwinding parser */

	state->next_constant_description_id = 0;
}

void destroy_parser_state(struct parser_state * state){
	/* Undo every operation that was used to build up the entire parser state */
	backtrack_parser(state, 0);
	struct_parser_operation_list_destroy(&state->operation_stack);
	unsigned_char_ptr_to_struct_constant_description_ptr_map_destroy(&state->constant_map);
	struct_parser_error_list_destroy(&state->parser_errors);
	assert(!struct_scope_guid_id_list_size(&state->scope_guid_stack));
	struct_scope_guid_id_list_destroy(&state->scope_guid_stack);
	struct_scope_guid_id_list_destroy(&state->decremented_scope_guids);
}

const char ** get_node_type_names(void){
	return (const char **)node_type_names;
}

unsigned int get_ceil_modulo(unsigned int n, unsigned int modulo){
	unsigned int quotient = n / modulo;
	unsigned int remainder = n - (quotient * modulo);
	return modulo * (quotient + (remainder ? 1 : 0));
}
