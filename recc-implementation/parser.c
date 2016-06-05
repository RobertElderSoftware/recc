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
#include "parser.h"

static const char * node_type_names[91] = {
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
struct parser_node * type_name(struct parser_state *);
struct parser_node * parameter_type_list(struct parser_state *);
struct parser_node * labeled_statement(struct parser_state *);
struct parser_node * expression_statement(struct parser_state *);
struct parser_node * selection_statement(struct parser_state *);
struct parser_node * iteration_statement(struct parser_state *);
struct parser_node * jump_statement(struct parser_state *);
struct parser_node * assignment_expression(struct parser_state *);
struct parser_node * p_accept(enum c_token_type, struct parser_state *);
struct parser_node * expect(enum c_token_type, struct parser_state *);
struct parser_node * make_epsilon(struct parser_state *);
struct parser_node * unary_operator(struct parser_state *);
struct parser_node * assignment_operator(struct parser_state *);
struct parser_node * constant_expression(struct parser_state *);
struct parser_node * expression_statement(struct parser_state *);
struct parser_node * statement(struct parser_state *, unsigned int);
struct parser_node * statement_list_rest(struct parser_state *);
struct parser_node * statement_list(struct parser_state *);
struct parser_node * identifier_list_rest(struct parser_state *);
struct parser_node * identifier_list(struct parser_state *);
struct parser_node * labeled_statement(struct parser_state *);
struct parser_node * selection_statement(struct parser_state *);
struct parser_node * iteration_statement(struct parser_state *);
struct parser_node * jump_statement(struct parser_state *);
struct parser_node * function_definition(struct parser_state *);
struct parser_node * external_declaration(struct parser_state *);
struct parser_node * translation_unit_rest(struct parser_state *);
struct parser_node * translation_unit(struct parser_state *);

void stack_operation(const char*, ...);
void parser_progress(const char*, ...);
void print_scope_level(struct parser_state *, struct scope_level *, unsigned int);
void destroy_scope_level(struct scope_level *, unsigned int);
void add_terminal_tokens_to_buffer_h2(struct unsigned_char_list *, struct parser_node *, unsigned int *);
void add_terminal_tokens_to_buffer_h1(struct unsigned_char_list *, struct parser_node *, unsigned int *);
void add_terminal_tokens_to_buffer(struct unsigned_char_list *, struct parser_node *);
void manage_constant(struct parser_state *, struct parser_node *, enum add_or_remove);

void add_normalized_specifier_to_list(struct memory_pool_collection *, struct parser_node *, struct struct_normalized_specifier_ptr_list *);
void add_normalized_declarator_to_list(struct memory_pool_collection *, struct parser_node *, struct struct_normalized_declarator_ptr_list *);
struct struct_normalized_specifier_ptr_list * get_normalized_specifier_list(struct memory_pool_collection *, struct parser_node *);
struct struct_normalized_declarator_ptr_list * get_normalized_declarator_list(struct memory_pool_collection *, struct parser_node *);
void print_generic_declaration(struct memory_pool_collection *, struct unsigned_char_list *, struct parser_node * n, unsigned int);
void print_parser_node_tree(struct unsigned_char_list *, struct parser_node *, struct parser_node *, unsigned int);
struct first_and_last_namespace_object manage_generic_declaration(struct parser_state *, struct parser_node * n, struct normalized_declaration_set *, unsigned int, enum object_location, struct namespace_object *, struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *);
struct parser_node * get_function_definition(struct normalized_declaration_set *);
struct parser_node * create_abstract_declarator_from_declarator(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_declarator_to_abstract_declarator(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_declarator_to_abstract_declarator_h(struct memory_pool_collection *, struct parser_node *);
unsigned int convert_character_constant(unsigned char *, unsigned int *);
unsigned int * convert_string_literal(unsigned char *, unsigned int *);
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
struct first_and_last_namespace_object manage_declaration_elements(struct parser_state *, struct struct_normalized_declaration_element_ptr_list *, enum object_location, struct namespace_object *, struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *);
struct namespace_object * manage_declaration_element(struct parser_state *, struct normalized_declaration_element *, enum scope_type, enum object_location, struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *);
void print_normalized_specifier(struct unsigned_char_list *, struct struct_normalized_specifier_ptr_list *);
struct namespace_modification * create_namespace_modification(struct scope_level *, struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *, struct normalized_declaration_element *, enum object_location, struct namespace_object *, unsigned char *);
struct namespace_object * do_namespace_modification(struct namespace_modification *);
void undo_namespace_modification(struct memory_pool_collection *, struct namespace_modification *);
unsigned int convert_hexadecimal_constant(unsigned char *);
unsigned int get_hex_digit_value(unsigned char);
void set_next_namespace_object(struct namespace_object_change *);
void unset_next_namespace_object(struct namespace_object_change *);
void set_previous_namespace_object(struct namespace_object_change *);
void unset_previous_namespace_object(struct namespace_object_change *);
void set_current_function(struct current_function_change *);
void unset_current_function(struct current_function_change *);
struct scope_level * get_last_function_namespace_scope(struct parser_state *);
struct normalized_declaration_element * get_most_recently_added_declaration_element_from_object(struct namespace_object *);
unsigned int is_const(struct type_description_reference t);
struct type_description_reference ensure_signed(struct memory_pool_collection *, struct type_description_reference t);
struct type_description_reference ensure_unsigned(struct memory_pool_collection *, struct type_description_reference t);
void validate_specifier_token_type(enum c_token_type);
enum normalized_specifier_type get_normalized_specifier_type(enum c_token_type);
enum node_type get_specifier_parser_node_type(enum c_token_type);
unsigned int get_token_index(struct c_lexer_state *, struct c_lexer_token *, unsigned int *);
unsigned int get_token_line_number(struct c_lexer_state *, struct c_lexer_token *);
unsigned char * get_specifier_string(enum c_token_type);
void pretty_print_context(struct c_lexer_state *, struct c_lexer_token *, struct c_lexer_token *, struct c_lexer_token *, struct c_lexer_token *);
struct parser_node * get_last_terminal_node(struct parser_node *);
void declare_enum_constants(struct parser_state *, struct parser_node *, struct normalized_declaration_set *);
struct parser_node * insert_abstract_declarator(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
struct parser_node * add_outer_direct_abstract_declarator_to_inner_as_rest(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
void add_pointer_to_end(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
void destroy_constant_description(struct memory_pool_collection *, struct constant_description *);
int normalized_specifier_ptr_cmp(struct normalized_specifier *, struct normalized_specifier *);
struct parser_checkpoint create_parser_checkpoint(struct parser_state *);
void backtrack_parser(struct parser_state *, unsigned int);

struct parser_checkpoint create_parser_checkpoint(struct parser_state * state){
	struct parser_checkpoint c;
	c.parser_operation_checkpoint = struct_parser_operation_list_size(&state->operation_stack);
	c.type_engine_checkpoint = type_engine_checkpoint(state->type_engine);
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

void print_error_with_type(struct c_lexer_state * state, struct type_description_reference t1, struct parser_node * context, const char * msg){
	struct unsigned_char_list l;
	unsigned_char_list_create(&l);
	buffered_printf(&l,"\n%s\n", msg);
	if(t1.t){
		buffered_puts(&l,"Type #1:\n");
		print_normalized_declaration_declarator_and_specifiers(state->c.memory_pool_collection, &l, t1.t->declarator, t1.t->specifiers, (struct parser_node *)0, 0);
	}
	unsigned_char_list_add_end(&l, 0);
	printf("%s\n", unsigned_char_list_data(&l));
	print_node_context(state, context);
	unsigned_char_list_destroy(&l);
}

void print_error_with_types(struct c_lexer_state * state, struct type_description_reference t1, struct type_description_reference t2, struct parser_node * context, const char * msg){
	struct unsigned_char_list l;
	unsigned_char_list_create(&l);
	buffered_printf(&l,"\n%s\n", msg);
	buffered_puts(&l,"Type #1:\n");
	print_normalized_declaration_declarator_and_specifiers(state->c.memory_pool_collection, &l, t1.t->declarator, t1.t->specifiers, (struct parser_node *)0, 0);
	buffered_puts(&l,"Type #2:\n");
	print_normalized_declaration_declarator_and_specifiers(state->c.memory_pool_collection, &l, t2.t->declarator, t2.t->specifiers, (struct parser_node *)0, 0);
	unsigned_char_list_add_end(&l, 0);
	printf("%s\n", unsigned_char_list_data(&l));
	print_node_context(state, context);
	unsigned_char_list_destroy(&l);
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
	unsigned int num_scopes = struct_scope_level_ptr_list_size(&state->type_engine->top_scope->scopes);
	assert(num_scopes && "There should be at least one sub scope at this point.");
	return struct_scope_level_ptr_list_get(&state->type_engine->top_scope->scopes, num_scopes -1);
}

void set_next_namespace_object(struct namespace_object_change * c){
	c->target->next = c->new_obj;
}

void unset_next_namespace_object(struct namespace_object_change * c){
	c->target->next = c->old_obj;
}

void set_previous_namespace_object(struct namespace_object_change * c){
	c->target->previous = c->new_obj;
}

void unset_previous_namespace_object(struct namespace_object_change * c){
	c->target->previous = c->old_obj;
}

void set_current_function(struct current_function_change * c){
	c->target->current_function = c->new_obj;
}

void unset_current_function(struct current_function_change * c){
	c->target->current_function = c->old_obj;
}


unsigned int is_type_description_a_function(struct memory_pool_collection * m, struct type_description_reference t){
	struct parser_node * temp = create_abstract_declarator_from_normalized_declarator(m, t.t->declarator);
	unsigned int rtn = is_function(m, temp);
	destroy_parser_node_tree_and_c_lexer_tokens(m, temp);
	return rtn;
}

unsigned int is_type_description_a_function_pointer(struct memory_pool_collection * m, struct type_description_reference t){
	struct parser_node * temp = create_abstract_declarator_from_normalized_declarator(m, t.t->declarator);
	unsigned int rtn;
	while(is_pointer(m, temp)){
		temp = convert_abstract_declarator_to_pointer_type(m, temp);
	}
	rtn = is_function(m, temp);
	destroy_parser_node_tree_and_c_lexer_tokens(m, temp);
	return rtn;
}

struct type_description_reference create_type_description_from_type_name(struct memory_pool_collection * m, struct parser_state * state, struct parser_node * n){
	struct type_description_reference rtn;
	struct normalized_declarator * normalized_declarator = (struct normalized_declarator *)malloc(sizeof(struct normalized_declarator));
	struct scope_level * scope = get_parser_scope_level(state->type_engine);
	rtn.t = struct_type_description_memory_pool_malloc(m->struct_type_description_pool);
	assert(n->type == TYPE_NAME);
	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	if(n->first_child->next && n->first_child->next->type == ABSTRACT_DECLARATOR){
		normalized_declarator->declarator = copy_parser_node_children_only(state->memory_pool_collection, n->first_child->next);
	}else{
		normalized_declarator->declarator = (struct parser_node *)0;
	}
	rtn.t->specifiers = get_normalized_specifier_list(m, n->first_child);
	rtn.t->declarator = normalized_declarator;
	rtn.t->source_scope_level = scope;
	rtn.t->context = n;
	rtn.t->value_type = WORD_ALIGNED_RVALUE;
	rtn.t->source_element = (struct normalized_declaration_element *)0;
	return rtn;
}

struct type_description_reference get_current_function_return_type_description(struct memory_pool_collection * m, struct type_description_reference t){
	/*  Should be a plain function type with no levels of indirection */
	struct type_description_reference return_type_description;
	struct normalized_declarator * normalized_declarator_return = (struct normalized_declarator *)malloc(sizeof(struct normalized_declarator));

	struct parser_node * abstract = create_abstract_declarator_from_normalized_declarator(m, t.t->declarator);
	struct parser_node * fcn_type = convert_abstract_declarator_to_function_type(m, abstract);
	normalized_declarator_return->declarator = fcn_type;
	normalized_declarator_return->type = NORMALIZED_ABSTRACT_DECLARATOR;
	return_type_description = create_type_description_from_normalized_declarator_and_specifiers(m, normalized_declarator_return, t.t->specifiers, t.t->context, t.t->source_scope_level, t.t->value_type, t.t->source_element);
	destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_declarator_return->declarator);
	free(normalized_declarator_return);
	return return_type_description;
}

struct type_description_reference create_dereferenced_array_type_description_from_type_description(struct memory_pool_collection * m, struct type_description_reference a){
	/* Take a type description as input and return a type description of the pointer type (do the [] operator on an array or pointer) */
	struct type_description_reference rtn;
	struct normalized_declarator * normalized_declarator = (struct normalized_declarator *)malloc(sizeof(struct normalized_declarator));
	struct parser_node * temp = (struct parser_node *)0;
	temp = create_abstract_declarator_from_normalized_declarator(m, a.t->declarator);
	if(is_array(m, temp)){
		normalized_declarator->declarator = convert_abstract_declarator_to_array_type(m, temp);
	}else if(is_pointer(m, temp)){
		normalized_declarator->declarator = convert_abstract_declarator_to_pointer_type(m, temp);
	}else{
		struct unsigned_char_list l;
		unsigned_char_list_create(&l);
		buffered_puts(&l, "Type is:\n");
		print_normalized_declaration_declarator_and_specifiers(m, &l, a.t->declarator, a.t->specifiers, (struct parser_node *)0, 0);
		unsigned_char_list_add_end(&l, 0);
		printf("%s\n", unsigned_char_list_data(&l));
		assert(0 && "Trying to perform [] operator on something that is not an array or pointer.");
	}
	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	rtn = create_type_description_from_normalized_declarator_and_specifiers(m, normalized_declarator, a.t->specifiers, a.t->context, a.t->source_scope_level, a.t->value_type, a.t->source_element);
	destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_declarator->declarator);
	free(normalized_declarator);
	return rtn;
}

struct type_description_reference create_dereferenced_pointer_type_description_from_type_description(struct memory_pool_collection * m, struct type_description_reference a){
	/* Take a type description as input and return a type description of the pointer type (do the * (value pointed to by) operator on a pointer) */
	struct type_description_reference rtn;
	struct normalized_declarator * normalized_declarator = (struct normalized_declarator *)malloc(sizeof(struct normalized_declarator));
	struct parser_node * temp = (struct parser_node *)0;
	temp = create_abstract_declarator_from_normalized_declarator(m, a.t->declarator);
	normalized_declarator->declarator = convert_abstract_declarator_to_pointer_type(m, temp);
	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	rtn = create_type_description_from_normalized_declarator_and_specifiers(m, normalized_declarator, a.t->specifiers, a.t->context, a.t->source_scope_level, a.t->value_type, a.t->source_element);
	destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_declarator->declarator);
	free(normalized_declarator);
	return rtn;
}


struct type_description_reference create_address_type_description_from_type_description(struct memory_pool_collection * m, struct type_description_reference a){
	/* Take a type description as input and return a type description of the address type (do the & operator) */
	struct type_description_reference rtn;
	struct normalized_declarator * normalized_declarator = (struct normalized_declarator *)malloc(sizeof(struct normalized_declarator));
	struct parser_node * temp = (struct parser_node *)0;
	temp = create_abstract_declarator_from_normalized_declarator(m, a.t->declarator);
	normalized_declarator->declarator = convert_abstract_declarator_to_address_type(m, temp);
	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	rtn = create_type_description_from_normalized_declarator_and_specifiers(m, normalized_declarator, a.t->specifiers, a.t->context, a.t->source_scope_level, a.t->value_type, a.t->source_element);
	destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_declarator->declarator);
	free(normalized_declarator);
	return rtn;
}

unsigned int type_description_cmp(struct memory_pool_collection * m, struct type_description_reference a, struct type_description_reference b){
	/*  Compares the 'type' and ignores storage class specifiers and type qualifiers */
	/*  TODO:  Refactor with options that can selectively ignore comparison of type qualifiers, specifiers etc. */

	enum type_class a_class = determine_type_class(m, a);
	enum type_class b_class = determine_type_class(m, b);

	if(a_class != b_class){
		return 1;
	}else{
		struct parser_node * abstract_declarator_a = create_abstract_declarator_from_normalized_declarator(m, a.t->declarator);
		struct parser_node * abstract_declarator_b = create_abstract_declarator_from_normalized_declarator(m, b.t->declarator);
		while(abstract_declarator_a || abstract_declarator_b){

			if(is_pointer(m, abstract_declarator_a) || is_pointer(m, abstract_declarator_b)){
				if(is_pointer(m, abstract_declarator_a) && is_pointer(m, abstract_declarator_b)){
					abstract_declarator_a = convert_abstract_declarator_to_pointer_type(m, abstract_declarator_a);
					abstract_declarator_b = convert_abstract_declarator_to_pointer_type(m, abstract_declarator_b);
				}else{
					destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_a);
					destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_b);
					return 1;
				}
			}

			if(is_function(m, abstract_declarator_a) || is_function(m, abstract_declarator_b)){
				if(is_function(m, abstract_declarator_a) && is_function(m, abstract_declarator_b)){
					struct parser_node * parameter_type_list_a;
					struct parser_node * parameter_type_list_b;
					unsigned int arg_index = 0;
					parameter_type_list_a = get_parameter_type_list_from_abstract_declarator(abstract_declarator_a);
					parameter_type_list_b = get_parameter_type_list_from_abstract_declarator(abstract_declarator_b);
					if(parameter_type_list_a || parameter_type_list_a){
						if(parameter_type_list_a && parameter_type_list_a){
							struct parser_node * parameter_declaration_a;
							struct parser_node * parameter_declaration_b;
							do{
								parameter_declaration_a = get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list_a, arg_index);
								parameter_declaration_b = get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list_b, arg_index);
								if(parameter_declaration_a || parameter_declaration_b){
									if(parameter_declaration_a && parameter_declaration_b){
										
										struct normalized_declaration_set * declaration_set_a = create_normalized_declaration_set_from_parser_node(m, parameter_declaration_a, (struct normalized_declaration_set *)0);
										struct normalized_declaration_set * declaration_set_b = create_normalized_declaration_set_from_parser_node(m, parameter_declaration_b, (struct normalized_declaration_set *)0);
										unsigned int rtn;

										struct type_description_reference param_type_a;
										struct type_description_reference param_type_b;
										param_type_a.t = struct_type_description_memory_pool_malloc(m->struct_type_description_pool);
										param_type_b.t = struct_type_description_memory_pool_malloc(m->struct_type_description_pool);
										param_type_a.t->specifiers = declaration_set_a->normalized_specifiers;
										param_type_b.t->specifiers = declaration_set_b->normalized_specifiers;
										assert(struct_normalized_declarator_ptr_list_size(declaration_set_a->normalized_declarators) <= 1);
										assert(struct_normalized_declarator_ptr_list_size(declaration_set_b->normalized_declarators) <= 1);
										if(struct_normalized_declarator_ptr_list_size(declaration_set_a->normalized_declarators)){
											param_type_a.t->declarator = struct_normalized_declarator_ptr_list_get(declaration_set_a->normalized_declarators, 0);
										}else{
											param_type_a.t->declarator = (struct normalized_declarator *)0;
										}
										if(struct_normalized_declarator_ptr_list_size(declaration_set_b->normalized_declarators)){
											param_type_b.t->declarator = struct_normalized_declarator_ptr_list_get(declaration_set_b->normalized_declarators, 0);
										}else{
											param_type_b.t->declarator = (struct normalized_declarator *)0;
										}
										rtn = type_description_cmp(m, param_type_a, param_type_b);
										destroy_normalized_declaration_element_list(m, create_normalized_declaration_element_list(declaration_set_a));
										destroy_normalized_declaration_element_list(m, create_normalized_declaration_element_list(declaration_set_b));
										struct_type_description_memory_pool_free(m->struct_type_description_pool, param_type_a.t);
										struct_type_description_memory_pool_free(m->struct_type_description_pool, param_type_b.t);
										arg_index++;
										if(rtn){
											return rtn;
										}
									}else{
										destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_a);
										destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_b);
										return 1;
									}
								}
							}while(parameter_declaration_a || parameter_declaration_b);
						}else{
							destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_a);
							destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_b);
							return 1;
						}
					}
					abstract_declarator_a = convert_abstract_declarator_to_function_type(m, abstract_declarator_a);
					abstract_declarator_b = convert_abstract_declarator_to_function_type(m, abstract_declarator_b);
				}else{
					destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_a);
					destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_b);
					return 1;
				}
			}

			if(is_array(m, abstract_declarator_a) || is_array(m, abstract_declarator_b)){
				if(is_array(m, abstract_declarator_a) && is_array(m, abstract_declarator_b)){
					abstract_declarator_a = convert_abstract_declarator_to_array_type(m, abstract_declarator_a);
					abstract_declarator_b = convert_abstract_declarator_to_array_type(m, abstract_declarator_b);
				}else{
					destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_a);
					destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator_b);
					return 1;
				}
			}
		}
	}
	return 0;
}


unsigned int get_enum_value(struct memory_pool_collection * m, struct normalized_declaration_element * element){
	unsigned int rtn = 0; /* Enum starts at 0 */
	struct parser_node * n;
	unsigned char * label = copy_string(element->normalized_declarator->declarator->first_child->c_lexer_token->first_byte, element->normalized_declarator->declarator->first_child->c_lexer_token->last_byte, m);
	(void)m;
	assert(element->normalized_declaration_set->set->type == ENUMERATOR_LIST);
	n = element->normalized_declaration_set->set->first_child;
	while(n->type == ENUMERATOR){
		unsigned char * cmp = copy_string(n->first_child->c_lexer_token->first_byte, n->first_child->c_lexer_token->last_byte, m);
		if(!strcmp((const char *)cmp, (const char *)label)){
			heap_memory_pool_free(m->heap_pool, label);
			heap_memory_pool_free(m->heap_pool, cmp);
			return rtn;
		}
		heap_memory_pool_free(m->heap_pool, cmp);
		n = n->next->first_child;
		if(n->next)
			n = n->next;
		rtn++;
	}
	assert(0 && "enum value not found.");
	return 0;
}
void print_normalized_specifier(struct unsigned_char_list * buffer, struct struct_normalized_specifier_ptr_list * specifier_list){
	unsigned int num_specifiers = struct_normalized_specifier_ptr_list_size(specifier_list);
	unsigned int i;
	for(i = 0; i < num_specifiers; i++){
		struct normalized_specifier * specifier = struct_normalized_specifier_ptr_list_get(specifier_list, i);
		add_terminal_tokens_to_buffer(buffer, specifier->specifier);
	}
}

void print_normalized_declaration_element(struct memory_pool_collection * m, struct unsigned_char_list * buffer, struct normalized_declaration_element * element, unsigned int depth){
	unsigned int i;
	if(depth == 0){
		buffered_puts(buffer, ";===============================\n");
	}
	buffered_puts(buffer, ";");
	for(i = 0; i < (2*depth);i++)
		buffered_puts(buffer, "-");
	buffered_puts(buffer, ">");

	print_normalized_declaration_declarator_and_specifiers(m, buffer, element->normalized_declarator, element->normalized_specifiers, get_function_definition(element->normalized_declaration_set), depth);
}

void print_normalized_declaration_declarator_and_specifiers(struct memory_pool_collection * m, struct unsigned_char_list * buffer, struct normalized_declarator * normalized_declarator, struct struct_normalized_specifier_ptr_list * normalized_specifiers, struct parser_node * function_definition, unsigned int depth){
	struct parser_node * struct_or_union_or_enum_specifier;
	unsigned int i;
	buffered_puts(buffer,";Specifiers: ");
	print_normalized_specifier(buffer, normalized_specifiers);
	buffered_puts(buffer," ");

	if(normalized_declarator){
		struct c_lexer_token * identifier_token = get_identifier_token_from_normalized_declarator(normalized_declarator);
		if(identifier_token){
			buffered_puts(buffer, "Declarator identifier is '");
			add_string_to_buffer(buffer, identifier_token->first_byte, identifier_token->last_byte);
			buffered_puts(buffer, "'");
		}
		buffered_puts(buffer, "  The full declarator is: ");

		add_terminal_tokens_to_buffer(buffer, normalized_declarator->declarator);
	}
	if(function_definition){
		buffered_puts(buffer, "{...}");
	}
	buffered_puts(buffer, "\n");

	if(normalized_declarator && is_bitfield(normalized_declarator->declarator)){
		buffered_puts(buffer,";");
		for(i = 0; i < (2*depth);i++)
			buffered_puts(buffer,"-");
		buffered_puts(buffer, ">This is a bitfield.\n");
	}

	struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(normalized_specifiers);
	if(struct_or_union_or_enum_specifier){
		struct c_lexer_token * token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
		struct parser_node * declaration_list = (struct parser_node *)0;
		struct parser_node * enumerator_list = (struct parser_node *)0;
		unsigned int its_a_struct = is_struct(struct_or_union_or_enum_specifier);
		unsigned int its_a_union = is_union(struct_or_union_or_enum_specifier);
		unsigned int its_a_enum = is_enum(struct_or_union_or_enum_specifier);
		buffered_puts(buffer,";");
		if(its_a_enum){
			enumerator_list = get_enumerator_list(struct_or_union_or_enum_specifier);
		}

		if(its_a_struct || its_a_union){
			declaration_list = get_struct_declaration_list(struct_or_union_or_enum_specifier);
		}

		for(i = 0; i < (2*depth);i++)
			buffered_puts(buffer,"-");
		buffered_puts(buffer,";>This is ");
		if(token){
			buffered_puts(buffer,"a ");
		}else{
			buffered_puts(buffer,"an anonymous ");
		}
		if(its_a_struct){
			buffered_puts(buffer,"struct");
		}else if(its_a_union){
			buffered_puts(buffer,"union");
		}else if(its_a_enum){
			buffered_puts(buffer,"enum");
		}else{
			assert(0);
		}
		buffered_puts(buffer," declaration");
		if(token){
			buffered_puts(buffer," with tag '");
			add_string_to_buffer(buffer, token->first_byte, token->last_byte);
			buffered_puts(buffer,"'.");
		}else{
			buffered_puts(buffer,".");
		}
		buffered_puts(buffer,"\n");

		buffered_puts(buffer,";");
		if(declaration_list){
			/*  Print out the struct declarations  */
			/*
			struct parser_node * node = declaration_list->first_child;
			for(i = 0; i < (2*depth);i++)
				buffered_puts(buffer,"-");
			buffered_puts(buffer,">It is also a definition that defines:\n");
			while(node->type != EPSILON){
				print_generic_declaration(buffer, node, depth + 1);
				node = node->next->first_child;
			}
			*/
		}
		if(enumerator_list){
			for(i = 0; i < (2*depth);i++)
				buffered_puts(buffer,"-");
			buffered_puts(buffer,">It is also a definition that defines:\n");
			buffered_puts(buffer,";");
			for(i = 0; i < (2*depth);i++)
				buffered_puts(buffer,"-");
			print_generic_declaration(m, buffer, enumerator_list, depth + 1);
		}
	}
	if(normalized_declarator){
		struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(m, normalized_declarator);
		struct parser_node * address_declarator = convert_abstract_declarator_to_address_type(m, copy_parser_node_children_only(m, abstract_declarator));
		buffered_puts(buffer,";");

		for(i = 0; i < (2*depth);i++)
			buffered_puts(buffer,"-");
		buffered_puts(buffer,">Here is the abstract declarator: ");
		add_terminal_tokens_to_buffer(buffer, abstract_declarator);
		buffered_puts(buffer,"\n;Here is the address type: ");
		add_terminal_tokens_to_buffer(buffer, address_declarator);

		destroy_parser_node_tree_and_c_lexer_tokens(m, address_declarator);

		while(abstract_declarator){
			if(is_pointer(m, abstract_declarator)){
				abstract_declarator = convert_abstract_declarator_to_pointer_type(m, abstract_declarator);
				buffered_puts(buffer," it is a pointer of type '");
				add_terminal_tokens_to_buffer(buffer, abstract_declarator);
				buffered_puts(buffer,"'");
			}

			if(is_function(m, abstract_declarator)){
				struct parser_node * parameter_type_list;
				struct parser_node * parameter_declaration;
				unsigned int arg_index = 0;
				parameter_type_list = get_parameter_type_list_from_abstract_declarator(abstract_declarator);
				if(parameter_type_list){
					while((parameter_declaration = get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list, arg_index))){
						buffered_printf(buffer,";Here is param number %d\n", arg_index);
						buffered_puts(buffer,";");
						print_generic_declaration(m, buffer, parameter_declaration, depth);
						arg_index++;
					}
				}else{
					buffered_puts(buffer,"No parameter list\n");
					buffered_puts(buffer,";");
				}
				buffered_puts(buffer,";");
				abstract_declarator = convert_abstract_declarator_to_function_type(m, abstract_declarator);
				buffered_puts(buffer," it is a function ");
				if(is_function_variadic(abstract_declarator)){
					buffered_puts(buffer," that is variadic ");
				}
				if(is_function_k_and_r_c_style(abstract_declarator)){
					buffered_puts(buffer," that is K&R C style ");
				}
				buffered_puts(buffer," its function type is '");
				add_terminal_tokens_to_buffer(buffer, abstract_declarator);
				buffered_puts(buffer,"'\n");
				buffered_puts(buffer,";");

			}

			if(is_array(m, abstract_declarator)){
				abstract_declarator = convert_abstract_declarator_to_array_type(m, abstract_declarator);
				buffered_puts(buffer," it is an array ");
				buffered_puts(buffer," its array type is '");
				add_terminal_tokens_to_buffer(buffer, abstract_declarator);
				buffered_puts(buffer,"'");
			}
		}

		destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator);
		buffered_puts(buffer,"\n");
	}
}

void print_generic_declaration(struct memory_pool_collection * m, struct unsigned_char_list * buffer, struct parser_node * n, unsigned int depth){
	struct normalized_declaration_set * declaration_set = create_normalized_declaration_set_from_parser_node(m, n, (struct normalized_declaration_set *)0);
	struct struct_normalized_declaration_element_ptr_list * element_list = create_normalized_declaration_element_list(declaration_set);
	unsigned int i;

	for(i = 0; i < struct_normalized_declaration_element_ptr_list_size(element_list); i++){
		print_normalized_declaration_element(m, buffer, struct_normalized_declaration_element_ptr_list_get(element_list, i), depth);
	}
	destroy_normalized_declaration_element_list(m, element_list);
}

struct first_and_last_namespace_object manage_generic_declaration(struct parser_state * state, struct parser_node * n, struct normalized_declaration_set * parent_set, unsigned int depth, enum object_location object_location, struct namespace_object * previous, struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * forced_namespace){
	struct normalized_declaration_set * declaration_set = create_normalized_declaration_set_from_parser_node(state->memory_pool_collection, n, parent_set);
	struct struct_normalized_declaration_element_ptr_list * element_list = create_normalized_declaration_element_list(declaration_set);
	(void)depth;
	push_operation(state, SAVE_DECLARATION_ELEMENTS, (void*)element_list); /*  We can only destroy this information after we're done with the parser state because we will reference it every time we interact with variables */
	return manage_declaration_elements(state, element_list, object_location, previous, forced_namespace);
}
void stack_operation(const char* format, ...){
	va_list arglist;
	va_start( arglist, format );
	/*
	if(1){
		vprintf( format, arglist );
	}
	*/
	va_end( arglist );
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


struct namespace_modification * create_namespace_modification(struct scope_level * scope, struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * name, struct normalized_declaration_element * element, enum object_location object_location, struct namespace_object * obj, unsigned char * identifier_str){
	struct namespace_modification * modification = (struct namespace_modification *)malloc(sizeof(struct namespace_modification));
	modification->new_element = element;
	modification->object = obj;
	modification->object_location = object_location;
	modification->scope_namespace = name;
	modification->scope_level = scope;
	modification->identifier_str = identifier_str;
	return modification;
}

struct namespace_object * do_namespace_modification(struct namespace_modification * modification){
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * name = modification->scope_namespace;
	struct namespace_object * parent_obj = modification->object;
	if(!parent_obj){
		parent_obj = (struct namespace_object *)malloc(sizeof(struct namespace_object));
		parent_obj->object_location = modification->object_location;
		parent_obj->scope_level = modification->scope_level;
		parent_obj->offset = 0;
		parent_obj->first_load = 1;
		parent_obj->previous = (struct namespace_object *)0;
		parent_obj->next = (struct namespace_object *)0;
		parent_obj->children = (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)malloc(sizeof(struct unsigned_char_ptr_to_struct_namespace_object_ptr_map));
		unsigned_char_ptr_to_struct_namespace_object_ptr_map_create(parent_obj->children, unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_compare);
		unsigned_char_ptr_to_struct_namespace_object_ptr_map_put(name, modification->identifier_str, parent_obj);
		struct_normalized_declaration_element_ptr_list_create(&parent_obj->elements);
	}
	modification->object = parent_obj;
	/*  Remember which object we added it to, so we can remove that element when we destroy the state */
	struct_normalized_declaration_element_ptr_list_add_end(&parent_obj->elements, modification->new_element);

	return parent_obj;
}

void undo_namespace_modification(struct memory_pool_collection * m, struct namespace_modification * modification){
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * name = modification->scope_namespace;
	struct struct_normalized_declaration_element_ptr_list * elements = &modification->object->elements;
	assert(struct_normalized_declaration_element_ptr_list_size(elements));
	/*  Always remove one element */
	struct_normalized_declaration_element_ptr_list_pop_end(elements);
	if(struct_normalized_declaration_element_ptr_list_size(elements) == 0){
		/*  If that was the last element for that namespace object, delete the namespace object */
		if(unsigned_char_ptr_to_struct_namespace_object_ptr_map_exists(name, modification->identifier_str)){
			struct namespace_object * o = unsigned_char_ptr_to_struct_namespace_object_ptr_map_get(name, modification->identifier_str);
			assert(o == modification->object); /*  The function should work as long as this assumption is correct. */
			struct_normalized_declaration_element_ptr_list_destroy(elements);
			unsigned_char_ptr_to_struct_namespace_object_ptr_map_destroy(o->children);
			free(o->children);
			free(o);
			unsigned_char_ptr_to_struct_namespace_object_ptr_map_remove(name, modification->identifier_str);
		}else{
			assert(0 && "Shouldn't happen.");
		}
	}
	heap_memory_pool_free(m->heap_pool, modification->identifier_str);
	free(modification);
}

void declare_enum_constants(struct parser_state * state, struct parser_node * n, struct normalized_declaration_set * set){
	if(n && n->type == ENUM_SPECIFIER){
		struct parser_node * enumerator_list = get_enumerator_list(n);
		if(enumerator_list){
			manage_generic_declaration(state, enumerator_list, set, 0, ENUM_IDENTIFIER, (struct namespace_object *)0, (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)0);
		}
	}
}

unsigned char * make_up_identifier(struct normalized_declaration_element * element, struct memory_pool_collection * m){
	/*  Required for anonymous struct/union/enum that don't have a tag but need to exit in the namespace.  */
	/*  Make up an identifier that is unique to the declaration (similar anonymous structs not compatible)  */
	return create_formatted_string(m, 20, "_%panonymous", (void *)element);
}

struct namespace_object * manage_declaration_element(struct parser_state * state, struct normalized_declaration_element * element, enum scope_type scope_type, enum object_location object_location, struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * forced_namespace){
	struct scope_level * scope = get_parser_scope_level(state->type_engine);
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * name;
	struct namespace_modification * modification;
	struct namespace_object * obj;
	struct namespace_object * o;
	unsigned char * identifier_string;

	switch(scope_type){
		case IDENTIFIER_NAMESPACE:{
			struct c_lexer_token * identifier_token = get_identifier_token_from_normalized_declarator(element->normalized_declarator);
			identifier_string = copy_string(identifier_token->first_byte, identifier_token->last_byte, state->memory_pool_collection);
			/*  For structure elements, declare the identifiers inside the namespace of the right tag */
			name = forced_namespace ? forced_namespace : &scope->identifier_namespace;
			break;
		}case TAG_NAMESPACE:{
			struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(element->normalized_specifiers);
			struct c_lexer_token * identifier_token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
			identifier_string = identifier_token ? copy_string(identifier_token->first_byte, identifier_token->last_byte, state->memory_pool_collection) : make_up_identifier(element, state->memory_pool_collection); /*  In case it is anonymous */
			name = &scope->tag_namespace;
			break;
		}case LABEL_NAMESPACE:{
			identifier_string = (unsigned char *)0;
			name = &scope->label_namespace;
			break;
		}default:{
			assert(0);
			identifier_string = (unsigned char *)0;
		}
	}

	/*  Check if there was already something that was declared with this identifier */
	obj = get_namespace_object_from_scope_namespace_using_string(name, identifier_string);

	modification = create_namespace_modification(scope, name, element, object_location, obj, identifier_string);
	modification->scope_type = scope_type;
	o = do_namespace_modification(modification);
	push_operation(state, NAMESPACE_MODIFICATION, (void*)modification);
	return o;
}

struct first_and_last_namespace_object manage_declaration_elements(struct parser_state * state, struct struct_normalized_declaration_element_ptr_list * elements, enum object_location object_location, struct namespace_object * previous_obj, struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * forced_namespace){
	/*  Forced namespace is used when we're declaring identifiers that are struct members.  These identifiers are forced
            into the namespace of the children of the namespace object of the given struct (and not the current scope).
        */
	unsigned int i;
	struct first_and_last_namespace_object fl;
	fl.first = (struct namespace_object *)0;
	fl.last = (struct namespace_object *)0;
	for(i = 0; i < struct_normalized_declaration_element_ptr_list_size(elements); i++){
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(elements, i);
		struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(element->normalized_specifiers);
		if(get_identifier_token_from_normalized_declarator(element->normalized_declarator)){
			/*  We can only add something to the scope if it has an identifier */
			struct namespace_object * next = manage_declaration_element(state, element, IDENTIFIER_NAMESPACE, object_location, forced_namespace);
			if(previous_obj){
				struct namespace_object_change * change_next = (struct namespace_object_change *)malloc(sizeof(struct namespace_object_change));
				change_next->target = previous_obj;
				change_next->old_obj = previous_obj->next;
				change_next->new_obj = next;
				push_operation(state, SET_NEXT_NAMESPACE_OBJECT, (void*)change_next);
			}
			if(next){
				struct namespace_object_change * change_previous = (struct namespace_object_change *)malloc(sizeof(struct namespace_object_change));
				change_previous->target = next;
				change_previous->old_obj = next->previous;
				change_previous->new_obj = previous_obj;
				push_operation(state, SET_PREVIOUS_NAMESPACE_OBJECT, (void*)change_previous);
				previous_obj = next;
				fl.last = next;
			}
			if(fl.first == (struct namespace_object *)0){
				fl.first = next;
			}
		}

		/*  The same declaration element could be declaraing a structure, union or enum too */
		if(struct_or_union_or_enum_specifier){
			struct namespace_object * new_obj = manage_declaration_element(state, element, TAG_NAMESPACE, object_location, (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)0);
			unsigned int its_a_union = is_union(struct_or_union_or_enum_specifier);
			unsigned int its_a_struct = is_struct(struct_or_union_or_enum_specifier);
			unsigned int its_a_enum = is_enum(struct_or_union_or_enum_specifier);
			/*  Declare all structure members in the structure's children namespace */
			/*  Make sure we don't declare these things twice if the struct declaration also declares an identifier */
			if(its_a_struct || its_a_union){
				struct parser_node * declaration_list = get_struct_declaration_list(struct_or_union_or_enum_specifier);
				struct parser_node * current_declaration;
				if(declaration_list){
					struct first_and_last_namespace_object all_members_fl;
					all_members_fl.first = (struct namespace_object *)0;
					all_members_fl.last = (struct namespace_object *)0;
					current_declaration = declaration_list->first_child;
					do{
						struct first_and_last_namespace_object current_members_fl = manage_generic_declaration(state, current_declaration, element->normalized_declaration_set, 0, LOCATION_STRUCT, (struct namespace_object *)0, new_obj->children);
						/*  Connect the members together */
						if(all_members_fl.last){
							if(current_members_fl.first){
								struct namespace_object_change * change_next = (struct namespace_object_change *)malloc(sizeof(struct namespace_object_change));
								struct namespace_object_change * change_previous = (struct namespace_object_change *)malloc(sizeof(struct namespace_object_change));
								change_next->target = all_members_fl.last;
								change_next->old_obj = all_members_fl.last->next;
								change_next->new_obj = current_members_fl.first;
								push_operation(state, SET_NEXT_NAMESPACE_OBJECT, (void*)change_next);

								change_previous->target = current_members_fl.first;
								change_previous->old_obj = current_members_fl.first->previous;
								change_previous->new_obj = all_members_fl.last;
								push_operation(state, SET_PREVIOUS_NAMESPACE_OBJECT, (void*)change_previous);
								all_members_fl.last = current_members_fl.last;
							}
						}else{
							all_members_fl = current_members_fl;
						}
						current_declaration = current_declaration->next->first_child;
					}while(current_declaration->type != EPSILON);
				}
			}
			if(its_a_enum){
				declare_enum_constants(state, struct_or_union_or_enum_specifier, element->normalized_declaration_set);
			}
		}
	}
	return fl;
}

struct constant_description * find_constant(struct parser_state * state, unsigned char * str){
	return unsigned_char_ptr_to_struct_constant_description_ptr_map_exists(&state->constant_map, str) ? unsigned_char_ptr_to_struct_constant_description_ptr_map_get(&state->constant_map, str) : (struct constant_description *)0;
}

unsigned int get_hex_digit_value(unsigned char c){
	if(c >= '0' && c <= '9'){
		return c - '0';
	}else if(c >= 'a' && c <= 'f'){
		return (c - 'A') + 10;
	}else if(c >= 'A' && c <= 'F'){
		return (c - 'A') + 10;
	}else{
		assert(0);
		return 0;
	}
}

unsigned int convert_hexadecimal_constant(unsigned char * str){
	unsigned int rtn = 0;
	unsigned int len = (unsigned int)strlen((const char *)str);
	unsigned int i = len;
	unsigned int base = 1;
	while(i > 2){
		if(!(str[i-1] == 'u' || str[i-1] == 'U' || str[i-1] == 'l' || str[i-1] == 'L')){
			unsigned int digit = get_hex_digit_value(str[i-1]);
			rtn += digit * base;
			base = base * 16;
		}
		i--;
	}
	return rtn;
}

unsigned int convert_decimal_constant(unsigned char * str){
	unsigned int rtn = 0;
	unsigned int len = (unsigned int)strlen((const char *)str);
	unsigned int i = len;
	unsigned int base = 1;
	while(i){
		if(!(str[i-1] == 'u' || str[i-1] == 'U' || str[i-1] == 'l' || str[i-1] == 'L')){
			unsigned int digit = (str[i-1] - '0');
			rtn += digit * base;
			base = base * 10;
		}
		i--;
	}
	return rtn;
}

unsigned int convert_character_constant(unsigned char * str, unsigned int * result){
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
			}case '"':{
				*result = '"';
				return 2;
			}case 'x':{
				struct unsigned_char_list hex_chars;
				unsigned int chars_consumed = 2;
				str += 2;
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
			}case '0':{
				*result = '\0';
				return 2;
			}default:{
				assert(0 && "Unknown character constant.");
				return 0;
			}
		}
	}
	*result = str[0];
	return 1;
}

unsigned int * convert_string_literal(unsigned char * str, unsigned int *size_in_bytes){
	struct unsigned_char_list converted_chars;
	unsigned_char_list_create(&converted_chars);
	while(*str){
		unsigned char * begin;
		str++; /* Jump over opening " */
		/*  This assumes that the STRING_LITERAL is a well formed token as described in the lexer */
		while(*str != '"'){ /* Any unescaped " is an end of string */
			unsigned int c;
			str += convert_character_constant(str, &c);
			assert(*str);
			unsigned_char_list_add_end(&converted_chars, (unsigned char)c);
		}
		str++; /*  Consume the closing '"' */
		do{
			begin = str; /* For detecting consumption of whitespace and comments */
			/*  Consume newlines and whitespace */
			while(*str == '\n' || *str == ' ' || *str == '\t'){ /* Skip spaces and newlines */
				str++;
			}
			/*  Consume comments */
			if(str[0] && str[1] && (str[0] == '/' && str[1] == '*')){
				str += 2; /* skip '/''*' */
				while(!(str[0] == '*' && str[1] == '/')){
					str++;
				}
				str += 2; /* skip '*''/' */
			}
		} while(begin != str);
	}
	unsigned_char_list_add_end(&converted_chars, '\0'); /*  Null termination. */
	while((unsigned_char_list_size(&converted_chars) % sizeof(unsigned int)) != 0){
		unsigned_char_list_add_end(&converted_chars, '\0'); /*  Ensure int alignment at end of string */
	}
	*size_in_bytes = unsigned_char_list_size(&converted_chars);
	/*  Don't destroy char list, return data buffer as raw converted data with padded nulls at end */
	return (unsigned int *)(void*)unsigned_char_list_data(&converted_chars);
}



struct type_description_reference get_type_description_from_suffix(struct memory_pool_collection * m, unsigned char * str){
	unsigned int num_u = 0;
	unsigned int num_l = 0;
	while(*str){
		switch(*str){
			case 'u':{
				num_u++;
				break;
			}case 'U':{
				num_u++;
				break;
			}case 'l':{
				num_l++;
				break;
			}case 'L':{
				num_l++;
				break;
			}default:{
			}
		}
		str++;
	}

	if(num_u == 0 && num_l == 0){
		return add_specifier(m, add_specifier(m, create_empty_type_description(m), UNSIGNED), INT);
	}else if(num_u == 1 && num_l == 0){
		return add_specifier(m, add_specifier(m, create_empty_type_description(m), UNSIGNED), INT);
	}else{
		printf("num u: %d, num l: %d\n", num_u, num_l);
		assert(0 && "Not implemented.");
	}
}

void manage_constant(struct parser_state * state, struct parser_node * n, enum add_or_remove add_or_remove){
	unsigned char * constant_string_representation = copy_string(n->c_lexer_token->first_byte, n->c_lexer_token->last_byte, state->memory_pool_collection);
	if(add_or_remove == ADD){
		struct constant_description * previous_description = unsigned_char_ptr_to_struct_constant_description_ptr_map_exists(&state->constant_map, constant_string_representation) ? unsigned_char_ptr_to_struct_constant_description_ptr_map_get(&state->constant_map, constant_string_representation) : (struct constant_description *)0;
		struct constant_description * new_description = (struct constant_description *)0;
		if(previous_description){
			previous_description->num_references = previous_description->num_references + 1;
			heap_memory_pool_free(state->memory_pool_collection->heap_pool, constant_string_representation);
			return;
		}/* else, constant does not already exist so create it */
		new_description = (struct constant_description *)malloc(sizeof(struct constant_description));
		new_description->num_references = 1;
		new_description->type = n->c_lexer_token->type;
		new_description->str = constant_string_representation;
		switch(n->c_lexer_token->type){
			case CONSTANT_HEX:{
				new_description->native_data = (unsigned int *)malloc(sizeof(unsigned int));
				*(new_description->native_data) = convert_hexadecimal_constant(new_description->str);
				new_description->type_description = get_type_description_from_suffix(state->memory_pool_collection, new_description->str);
				new_description->type_description.t->value_type = WORD_ALIGNED_RVALUE;
				new_description->type_description.t->source_scope_level = (struct scope_level*)0;
				break;
			}case CONSTANT_EXPONENT:{
				assert(0 && "constant exponent conversion not implemented.");
				break;
			}case CONSTANT_FLOAT_SMALL:{
				assert(0 && "constant float small conversion not implemented.");
				break;
			}case CONSTANT_FLOAT_LARGE:{
				assert(0 && "constant float large conversion not implemented.");
				break;
			}case CONSTANT_DECIMAL:{
				unsigned int value = convert_decimal_constant(new_description->str);
				new_description->native_data = (unsigned int *)malloc(sizeof(unsigned int));
				*(new_description->native_data) = value;
				/*  Assumes 32 bit integers */
				if(value & 0x80000000){
					/*  Cannot fit in signed integer */
					new_description->type_description = add_specifier(state->memory_pool_collection, add_specifier(state->memory_pool_collection, create_empty_type_description(state->memory_pool_collection), UNSIGNED), INT);
				}else{
					new_description->type_description = add_specifier(state->memory_pool_collection, create_empty_type_description(state->memory_pool_collection), INT);
				}
				new_description->type_description.t->value_type = WORD_ALIGNED_RVALUE;
				new_description->type_description.t->source_scope_level = (struct scope_level*)0;
				break;
			}case CONSTANT_CHARACTER:{
				unsigned int c;
				new_description->native_data = (unsigned int *)malloc(sizeof(unsigned int));
				*((unsigned int *)new_description->native_data) = 0;
				convert_character_constant(&new_description->str[1], &c);
				*((unsigned char *)&new_description->native_data[0]) = (unsigned char)c;
				new_description->type_description = add_specifier(state->memory_pool_collection, add_specifier(state->memory_pool_collection, create_empty_type_description(state->memory_pool_collection), CONST), CHAR);
				new_description->type_description.t->value_type = WORD_ALIGNED_RVALUE;
				new_description->type_description.t->source_scope_level = (struct scope_level*)0;
				break;
			}case STRING_LITERAL:{
				struct type_description_reference type_description;
				new_description->native_data = convert_string_literal(new_description->str, &new_description->size_in_bytes);
				type_description = add_specifier(state->memory_pool_collection, add_specifier(state->memory_pool_collection, create_empty_type_description(state->memory_pool_collection), CONST), CHAR);
				type_description.t->declarator = make_array_brackets(state->memory_pool_collection);
				type_description.t->value_type = LVALUE;
				type_description.t->source_scope_level = (struct scope_level*)0;
				new_description->type_description = type_description;
				break;
			}default:{
				assert(0 && "Unknown conversion.");
			}
		}
		unsigned_char_ptr_to_struct_constant_description_ptr_map_put(&state->constant_map, constant_string_representation, new_description);
	}else if(add_or_remove == REMOVE){
		struct constant_description * previous_description = unsigned_char_ptr_to_struct_constant_description_ptr_map_get(&state->constant_map, constant_string_representation);
		if(previous_description->num_references > 1){
			previous_description->num_references = previous_description->num_references - 1;
		}else{
			unsigned_char_ptr_to_struct_constant_description_ptr_map_remove(&state->constant_map, constant_string_representation);
			destroy_constant_description(state->memory_pool_collection, previous_description);
		}
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, constant_string_representation); /* The second copy we used to search with */
	}else{
		assert(0 && "Unknown option.");
	}
}

void destroy_constant_description(struct memory_pool_collection * m, struct constant_description * description){
	destroy_type_description(m, description->type_description);
	free(description->native_data);
	heap_memory_pool_free(m->heap_pool, description->str);
	free(description);
}

void * push_operation(struct parser_state * state, enum parser_operation_type t, void * data){
	struct parser_operation new_operation;
	new_operation.type = t;
	new_operation.data = t == ADVANCE_PARSER_POSITION ? struct_parser_node_memory_pool_malloc(state->memory_pool_collection->struct_parser_node_pool) : data;
	struct_parser_operation_list_add_end(&state->operation_stack, new_operation);
	switch(t){
		case ADVANCE_TOKEN_POSITION:{
			struct c_lexer_token ** tokens = struct_c_lexer_token_ptr_list_data(&state->c_lexer_state->tokens);
			unsigned char * c = tokens[state->tokens_position]->first_byte;
			stack_operation("Accepting token '");
			do{
				stack_operation("%c", *c);
			}while (c++ != tokens[state->tokens_position]->last_byte);
			stack_operation("'\n");
			state->tokens_position = state->tokens_position + 1;
			return (void *)0;
		}case ADVANCE_PARSER_POSITION:{
			*((struct parser_node *)new_operation.data) = *((struct parser_node *)data);
			return new_operation.data;
		}case PROCESS_CONSTANT:{
			manage_constant(state, (struct parser_node *)data, ADD);
			return (void *)0;
		}case SAVE_DECLARATION_ELEMENTS:{
			return (void *)0;
		}case NAMESPACE_MODIFICATION:{
			return (void *)0;
		}case CREATE_PARSER_NODE_TREE:{
			return (void *)0;
		}case INCREMENT_NEWLINE_COUNT:{
			state->line_number = state->line_number + 1;
			return (void *)0;
		}case SET_NEXT_NAMESPACE_OBJECT:{
			set_next_namespace_object((struct namespace_object_change *)data);
			return (void *)0;
		}case SET_PREVIOUS_NAMESPACE_OBJECT:{
			set_previous_namespace_object((struct namespace_object_change *)data);
			return (void *)0;
		}case SET_CURRENT_FUNCTION:{
			set_current_function((struct current_function_change *)data);
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
			struct c_lexer_token ** tokens = struct_c_lexer_token_ptr_list_data(&state->c_lexer_state->tokens);
			unsigned int new_num_tokens = state->tokens_position - 1;
			unsigned char * c = tokens[new_num_tokens]->first_byte;
			state->tokens_position = new_num_tokens;
			stack_operation("Put back token '");
			do{
				stack_operation("%c", *c);
			}while (c++ != tokens[new_num_tokens]->last_byte);
			stack_operation("'\n");
			break;
		}case ADVANCE_PARSER_POSITION:{
			struct_parser_node_memory_pool_free(state->memory_pool_collection->struct_parser_node_pool, (struct parser_node*)poped_operation.data);
			break;
		}case PROCESS_CONSTANT:{
			manage_constant(state, (struct parser_node *)poped_operation.data, REMOVE);
			break;
		}case SAVE_DECLARATION_ELEMENTS:{
			destroy_normalized_declaration_element_list(state->memory_pool_collection, (struct struct_normalized_declaration_element_ptr_list *)(poped_operation.data));
			break;
		}case NAMESPACE_MODIFICATION:{
			undo_namespace_modification(state->memory_pool_collection, (struct namespace_modification *)(poped_operation.data));
			break;
		}case CREATE_PARSER_NODE_TREE:{
			destroy_parser_node_tree_and_c_lexer_tokens(state->memory_pool_collection, (struct parser_node *)(poped_operation.data));
			break;
		}case INCREMENT_NEWLINE_COUNT:{
			state->line_number = state->line_number - 1;
			break;
		}case SET_NEXT_NAMESPACE_OBJECT:{
			struct namespace_object_change * c = (struct namespace_object_change *)(poped_operation.data);
			unset_next_namespace_object(c);
			free(c);
			break;
		}case SET_PREVIOUS_NAMESPACE_OBJECT:{
			struct namespace_object_change * c = (struct namespace_object_change *)(poped_operation.data);
			unset_previous_namespace_object(c);
			free(c);
			break;
		}case SET_CURRENT_FUNCTION:{
			struct current_function_change * c = (struct current_function_change *)(poped_operation.data);
			set_current_function(c);
			free(c);
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
	backtrack_type_engine(state->type_engine, checkpoint.type_engine_checkpoint);
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

struct parser_node * get_identifier_node(struct parser_state * state, unsigned int * id){
	struct parser_node * n;
	if((n = p_accept(IDENTIFIER, state))){
		*id = get_type_engine_id_for_identifier(state->type_engine, n->c_lexer_token);
		return n;
	}else{
		return (struct parser_node *)0;
	}
}

struct parser_node * primary_expression(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build primary_expression\n");
	if((n1 = p_accept(IDENTIFIER, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if(
		(n1 = p_accept(CONSTANT_HEX, state)) ||
		(n1 = p_accept(CONSTANT_EXPONENT, state)) ||
		(n1 = p_accept(CONSTANT_FLOAT_SMALL, state)) ||
		(n1 = p_accept(CONSTANT_FLOAT_LARGE, state)) ||
		(n1 = p_accept(CONSTANT_DECIMAL, state)) ||
		(n1 = p_accept(CONSTANT_CHARACTER, state))
	){
		push_operation(state, PROCESS_CONSTANT, (void*)n1);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(STRING_LITERAL, state))){
		push_operation(state, PROCESS_CONSTANT, (void*)n1);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		if((n1->next = expression(state))) {
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
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



struct parser_node * argument_expression_list_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build argument_expression_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		if((n1->next = assignment_expression(state))){
			if((n1->next->next = argument_expression_list_rest(state))){
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
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token *)0, ARGUMENT_EXPRESSION_LIST_REST);
	}
}

struct parser_node * argument_expression_list(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build argument_expression_list\n");
	if((n1 = assignment_expression(state))){
		if((n1->next = argument_expression_list_rest(state))){
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

struct parser_node * postfix_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build postfix_expression_rest\n");
	if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, state))){
		if((n1->next = expression(state))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
				if((n1->next->next->next = postfix_expression_rest(state))){
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
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, state))){
			if((n1->next->next = postfix_expression_rest(state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, POSTFIX_EXPRESSION_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_0_ARGS_NO_REST);
				return (struct parser_node *)0;
			}
		}else if((n1->next = argument_expression_list(state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				if((n1->next->next->next = postfix_expression_rest(state))){
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
		if((n1->next = p_accept(IDENTIFIER, state))){
			if((n1->next->next = postfix_expression_rest(state))){
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
		if((n1->next = p_accept(IDENTIFIER, state))){
			if((n1->next->next = postfix_expression_rest(state))){
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
		if((n1->next = postfix_expression_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token *)0, POSTFIX_EXPRESSION_REST);
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_INC_NO_REST);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DEC_OP, state))){
		if((n1->next = postfix_expression_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POSTFIX_EXPRESSION_REST);
		}else{
			add_parser_error(state, PARSER_ERROR_POSTFIX_EXPRESSION_REST_DEC_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, POSTFIX_EXPRESSION_REST);
	}
}

struct parser_node * postfix_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build postfix_expression\n");
	if((n1 = primary_expression(state))){
		if((n1->next = postfix_expression_rest(state))){
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


struct parser_node * unary_operator(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build unary_operator\n");
	if((n1 = p_accept(AMPERSAND_CHAR, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_OPERATOR);
	}else if((n1 = p_accept(MULTIPLY_CHAR, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_OPERATOR);
	}else if((n1 = p_accept(PLUS_CHAR, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_OPERATOR);
	}else if((n1 = p_accept(MINUS_CHAR, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_OPERATOR);
	}else if((n1 = p_accept(TILDE_CHAR, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_OPERATOR);
	}else if((n1 = p_accept(EXCLAMATION_MARK_CHAR, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_OPERATOR);
	}else{
		parser_progress("Expected unary operator.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * unary_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build unary_expression\n");
	if((n1 = postfix_expression(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
	}else if((n1 = p_accept(INC_OP, state))){
		if((n1->next = unary_expression(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_AFTER_INC);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DEC_OP, state))){
		if((n1->next = unary_expression(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_AFTER_DEC);
			return (struct parser_node *)0;
		}
	}else if((n1 = unary_operator(state))){
		if((n1->next = cast_expression(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(SIZEOF, state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, state))){
			if((n1->next->next = type_name(state))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					return (struct parser_node *)create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, UNARY_EXPRESSION);
				}else{
					add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_PAREN);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_UNARY_EXPRESSION_MISSING_TYPE);
				return (struct parser_node *)0;
			}
		}else if((n1->next = unary_expression(state))){
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

struct parser_node * cast_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build cast_expression\n");
	if((n1 = unary_expression(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, CAST_EXPRESSION);
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		if((n1->next = type_name(state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				if((n1->next->next->next = cast_expression(state))){
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

struct parser_node * multiplicative_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build multiplicative_expression_rest\n");
	if(!((n1 = p_accept(MULTIPLY_CHAR, state)) || (n1 = p_accept(DIVIDE_CHAR, state)) || (n1 = p_accept(PERCENT_CHAR, state)))){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, MULTIPLICATIVE_EXPRESSION_REST);
	}
	if(!(n1->next = cast_expression(state))){
		add_parser_error(state, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = multiplicative_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, MULTIPLICATIVE_EXPRESSION_REST);
}

struct parser_node * multiplicative_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build multiplicative_expression\n");
	if((n1 = cast_expression(state))){
		if((n1->next = multiplicative_expression_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, MULTIPLICATIVE_EXPRESSION);
		}else{
			add_parser_error(state, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_NO_REST);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected cast expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * additive_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build additive_expression_rest\n");
	if(!((n1 = p_accept(PLUS_CHAR, state)) || (n1 = p_accept(MINUS_CHAR, state)))){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, ADDITIVE_EXPRESSION_REST);
	}
	if(!(n1->next = multiplicative_expression(state))){
		add_parser_error(state, PARSER_ERROR_ADDITIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = additive_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_ADDITIVE_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ADDITIVE_EXPRESSION_REST);
}

struct parser_node * additive_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build additive_expression\n");
	if((n1 = multiplicative_expression(state))){
		if((n1->next = additive_expression_rest(state))){
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

struct parser_node * shift_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build shift_expression_rest\n");
	if(!((n1 = p_accept(LEFT_OP, state)) || (n1 = p_accept(RIGHT_OP, state)))){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, SHIFT_EXPRESSION_REST);
	}
	if(!(n1->next = additive_expression(state))){
		add_parser_error(state, PARSER_ERROR_SHIFT_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = shift_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_SHIFT_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SHIFT_EXPRESSION_REST);
}

struct parser_node * shift_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build shift_expression\n");
	if((n1 = additive_expression(state))){
		if((n1->next = shift_expression_rest(state))){
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

struct parser_node * relational_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build relational_expression_rest\n");
	if(!(
		(n1 = p_accept(OPEN_ANGLE_BRACKET_CHAR, state)) ||
		(n1 = p_accept(CLOSE_ANGLE_BRACKET_CHAR, state)) ||
		(n1 = p_accept(LE_OP, state)) ||
		(n1 = p_accept(GE_OP, state))
	)){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, RELATIONAL_EXPRESSION_REST);
	}
	if(!(n1->next = shift_expression(state))){
		add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = relational_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, RELATIONAL_EXPRESSION_REST);
}

struct parser_node * relational_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build relational_expression\n");
	if((n1 = shift_expression(state))){
		if((n1->next = relational_expression_rest(state))){
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

struct parser_node * equality_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build equality_expression_rest\n");
	if(!(
		(n1 = p_accept(EQ_OP, state)) ||
		(n1 = p_accept(NE_OP, state))
	)){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, EQUALITY_EXPRESSION_REST);
	}
	if(!(n1->next = relational_expression(state))){
		add_parser_error(state, PARSER_ERROR_EQUALITY_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = equality_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_EQUALITY_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EQUALITY_EXPRESSION_REST);
}

struct parser_node * equality_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build equality_expression\n");
	if((n1 = relational_expression(state))){
		if((n1->next = equality_expression_rest(state))){
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

struct parser_node * and_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build and_expression_rest\n");
	if(!(
		(n1 = p_accept(AMPERSAND_CHAR, state))
	)){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, AND_EXPRESSION_REST);
	}
	if(!(n1->next = equality_expression(state))){
		add_parser_error(state, PARSER_ERROR_AND_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = and_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_AND_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, AND_EXPRESSION_REST);
}

struct parser_node * and_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build and_expression\n");
	if((n1 = equality_expression(state))){
		if((n1->next = and_expression_rest(state))){
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

struct parser_node * exclusive_or_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build exclusive_or_expression_rest\n");
	if(!(
		(n1 = p_accept(CARET_CHAR, state))
	)){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, EXCLUSIVE_OR_EXPRESSION_REST);
	}
	if(!(n1->next = and_expression(state))){
		add_parser_error(state, PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = exclusive_or_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXCLUSIVE_OR_EXPRESSION_REST);
}

struct parser_node * exclusive_or_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build exclusive_or_expression\n");
	if((n1 = and_expression(state))){
		if((n1->next = exclusive_or_expression_rest(state))){
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

struct parser_node * inclusive_or_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build inclusive_or_expression_rest\n");
	if(!(
		(n1 = p_accept(PIPE_CHAR, state))
	)){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, INCLUSIVE_OR_EXPRESSION_REST);
	}
	if(!(n1->next = exclusive_or_expression(state))){
		add_parser_error(state, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = inclusive_or_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INCLUSIVE_OR_EXPRESSION_REST);
}

struct parser_node * inclusive_or_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build inclusive_or_expression\n");
	if((n1 = exclusive_or_expression(state))){
		if((n1->next = inclusive_or_expression_rest(state))){
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

struct parser_node * logical_and_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build logical_and_expression_rest\n");
	if(!(
		(n1 = p_accept(AND_OP, state))
	)){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, LOGICAL_AND_EXPRESSION_REST);
	}
	if(!(n1->next = inclusive_or_expression(state))){
		add_parser_error(state, PARSER_ERROR_LOGICAL_AND_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = logical_and_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_LOGICAL_AND_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LOGICAL_AND_EXPRESSION_REST);
}

struct parser_node * logical_and_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build logical_and_expression\n");
	if((n1 = inclusive_or_expression(state))){
		if((n1->next = logical_and_expression_rest(state))){
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

struct parser_node * logical_or_expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build logical_or_expression_rest\n");
	if(!(
		(n1 = p_accept(OR_OP, state))
	)){
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, LOGICAL_OR_EXPRESSION_REST);
	}
	if(!(n1->next = logical_and_expression(state))){
		add_parser_error(state, PARSER_ERROR_LOGICAL_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
		return (struct parser_node *)0;
	}
	if(!(n1->next->next = logical_or_expression_rest(state))){
		add_parser_error(state, PARSER_ERROR_LOGICAL_OR_EXPRESSION_REST_NO_REST);
		return (struct parser_node *)0;
	}
	return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LOGICAL_OR_EXPRESSION_REST);
}

struct parser_node * logical_or_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build logical_or_expression\n");
	if((n1 = logical_and_expression(state))){
		if((n1->next = logical_or_expression_rest(state))){
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

struct parser_node * conditional_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build conditional_expression\n");
	if((n1 = logical_or_expression(state))){
		if((n1->next = p_accept(QUESTION_MARK_CHAR, state))){
			if((n1->next->next = expression(state))){
				if((n1->next->next->next = p_accept(COLON_CHAR, state))){
					if((n1->next->next->next->next = conditional_expression(state))){
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
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, CONDITIONAL_EXPRESSION);
		}
	}else{
		parser_progress("Expected logical_or_expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * assignment_operator(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build assignment_operator\n");
	if((n1 = p_accept(EQUALS_CHAR, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(MUL_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(DIV_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(MOD_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(ADD_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(SUB_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(LEFT_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(RIGHT_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(AND_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(XOR_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(OR_ASSIGN, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_OPERATOR);
	}else{
		parser_progress("Expected assignment operator.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * assignment_expression(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build assignment_expression\n");

	if((n1 = unary_expression(state))){
		if((n1->next = assignment_operator(state))){	
			if((n1->next->next = assignment_expression(state))){	
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

	if((n1 = conditional_expression(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ASSIGNMENT_EXPRESSION);
	}else{
		parser_progress("Expected conditional_expression.\n");
		backtrack(state, checkpoint);
	}
	return (struct parser_node *)0;
}

struct parser_node * expression_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build expression_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		if((n1->next = assignment_expression(state))){
			if((n1->next->next = expression_rest(state))){
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
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, EXPRESSION_REST);
	}
}

struct parser_node * expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build expression\n");
	if((n1 = assignment_expression(state))){
		if((n1->next = expression_rest(state))){
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

struct parser_node * constant_expression(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build constant_expression\n");
	if((n1 = conditional_expression(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, CONSTANT_EXPRESSION);
	}else{
		parser_progress("Expected expression.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * expression_statement(struct parser_state * state){
	struct parser_node * n1;
	struct parser_node * n2;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build expression_statement\n");

	if((n1 = p_accept(SEMICOLON_CHAR, state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXPRESSION_STATEMENT);
	}else if((n1 = expression(state))){
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

struct parser_node * statement(struct parser_state * state, unsigned int scope_already_created){
	struct parser_node * n1;
	parser_progress("Attempting to build statement\n");
	if((n1 = labeled_statement(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = compound_statement(state, (struct parser_node *)0, scope_already_created))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = expression_statement(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = selection_statement(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = iteration_statement(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else if((n1 = jump_statement(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT);
	}else{
		parser_progress("Expected statement.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * statement_list_rest(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build statement_list_rest\n");
	if((n1 = statement(state, 0))){
		if((n1->next = statement_list_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT_LIST_REST);
		}else{
			parser_progress("Expected statement_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, STATEMENT_LIST_REST);
	}
}

struct parser_node * statement_list(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build statement_list\n");
	if((n1 = statement(state, 0))){
		if((n1->next = statement_list_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STATEMENT_LIST);
		}else{
			parser_progress("Expected statement_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected statement.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * compound_statement(struct parser_state * state, struct parser_node * possible_declarator, unsigned int scope_already_created){
	struct parser_node * n1;
	parser_progress("Attempting to build compound_statement\n");
	if((n1 = p_accept(OPEN_BRACE_CHAR, state))){
		unsigned int general_type_list_id;
		struct scope_level * possible_new_scope_level = (struct scope_level *)0;
		enum scope_level_type scope_type = (possible_declarator && possible_declarator->type == DECLARATOR) ? SCOPE_LEVEL_TYPE_FUNCTION_SCOPE : SCOPE_LEVEL_TYPE_BLOCK_SCOPE;
		if(!scope_already_created) { possible_new_scope_level = increment_scope_depth(state->type_engine, scope_type); }
		if(possible_declarator){
			if(possible_declarator->type == DECLARATION_LIST){
				/*  For K&R C style function */
			}else if(possible_declarator->type == DECLARATOR){
				/* If possible_declarator is set, it will be a declarator or declaration_list from a function.  Declare the params in this scope. */
				struct parser_node * copy = copy_parser_node_children_only(state->memory_pool_collection, possible_declarator);
				struct parser_node * abstract;
				struct parser_node * parameter_type_list;
				assert(copy->type == DECLARATOR);
				abstract = convert_declarator_to_abstract_declarator(state->memory_pool_collection, copy);
				parameter_type_list = get_parameter_type_list_from_abstract_declarator(abstract);
				if(parameter_type_list){
					struct parser_node * parameter_declaration = (struct parser_node *)0;
					unsigned int param_index = 0;
					struct namespace_object * prev_obj = (struct namespace_object *)0;
					while((parameter_declaration = get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list, param_index))){
						struct first_and_last_namespace_object fl;
						fl = manage_generic_declaration(state, parameter_declaration, (struct normalized_declaration_set *)0, 0, PARAMETER, prev_obj, (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)0);
						prev_obj = fl.last;
						param_index++;
					}
				}
				/*  We have to delete this after we're done using the parser state  */
				push_operation(state, CREATE_PARSER_NODE_TREE, (void*)abstract);
			}else{
				assert(0 && "Not expected");
			}
		}
		if((n1->next = statement_list(state))){
			if((n1->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
				if(!scope_already_created) { decrement_scope_depth(state->type_engine); }
				return associate_parser_node_with_scope_level(state->type_engine, possible_new_scope_level, create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, COMPOUND_STATEMENT));
			}else{
				add_parser_error(state, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_1);
				return (struct parser_node *)0;
			}
		}else if((n1->next = declaration_list(state, (struct namespace_object *)0, get_type_engine_id_for_general_type_list_begin(state->type_engine), &general_type_list_id))){
			if((n1->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
				if(!scope_already_created) { decrement_scope_depth(state->type_engine); }
				return associate_parser_node_with_scope_level(state->type_engine, possible_new_scope_level, create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, COMPOUND_STATEMENT));
			}else if((n1->next->next = statement_list(state))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
					if(!scope_already_created) { decrement_scope_depth(state->type_engine); }
					return associate_parser_node_with_scope_level(state->type_engine, possible_new_scope_level, create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, COMPOUND_STATEMENT));
				}else{
					add_parser_error(state, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_2);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_3);
				return (struct parser_node *)0;
			}
		}else if((n1->next = p_accept(CLOSE_BRACE_CHAR, state))){
			if(!scope_already_created) { decrement_scope_depth(state->type_engine); }
			return associate_parser_node_with_scope_level(state->type_engine, possible_new_scope_level, create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, COMPOUND_STATEMENT));
		}else{
			add_parser_error(state, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_4);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected compound_statement.\n");
		return (struct parser_node *)0;
	}
}


struct parser_node * type_qualifier(struct parser_state * state, unsigned int * type_engine_id){
	struct parser_node * n1;
	parser_progress("Attempting to build type_qualifier\n");
	if((n1 = p_accept(CONST, state))){
		*type_engine_id = get_type_engine_id_for_simple_qualifier(state->type_engine, CONST);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_QUALIFIER);
	}else if((n1 = p_accept(VOLATILE, state))){
		*type_engine_id = get_type_engine_id_for_simple_qualifier(state->type_engine, VOLATILE);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_QUALIFIER);
	}else {
		parser_progress("Expected type_qualifier.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * storage_class_specifier(struct parser_state * state, unsigned int * type_engine_id){
	struct parser_node * n1;
	parser_progress("Attempting to build storage_class_specifier\n");
	if((n1 = p_accept(TYPEDEF, state))){
		*type_engine_id = get_type_engine_id_for_storage_class_specifier(state->type_engine, TYPEDEF);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(EXTERN, state))){
		*type_engine_id = get_type_engine_id_for_storage_class_specifier(state->type_engine, EXTERN);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(STATIC, state))){
		*type_engine_id = get_type_engine_id_for_storage_class_specifier(state->type_engine, STATIC);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(AUTO, state))){
		*type_engine_id = get_type_engine_id_for_storage_class_specifier(state->type_engine, AUTO);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(REGISTER, state))){
		*type_engine_id = get_type_engine_id_for_storage_class_specifier(state->type_engine, REGISTER);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STORAGE_CLASS_SPECIFIER);
	}else {
		parser_progress("Expected type_qualifier.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * enumerator(struct parser_state * state){
	struct parser_node * n1;
	unsigned int identifier_id;
	parser_progress("Attempting to build enumerator\n");
	if((n1 = get_identifier_node(state, &identifier_id))){
		if((n1->next = p_accept(EQUALS_CHAR, state))){
			if((n1->next->next = constant_expression(state))){
				unsigned int constant_expression_id = get_type_engine_id_for_constant_parser_node(state->type_engine, n1->next->next);
				(void)constant_expression_id;
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUMERATOR);
			}else{
				add_parser_error(state, PARSER_ERROR_ENUMERATOR_MISSING_CONSTANT_EXPRESSION);
				return (struct parser_node *)0;
			}
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUMERATOR);
		}
	}else{
		buffered_puts(state->buffered_output, "Required identifier.  Cannot continue.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * enumerator_list_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build enumerator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		if((n1->next = enumerator(state))){
			if((n1->next->next = enumerator_list_rest(state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUMERATOR_LIST_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_ENUMERATOR_LIST_REST_MISSING_REST);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_ENUMERATOR_LIST_REST_MISSING_ENUMERATOR);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, ENUMERATOR_LIST_REST);
	}
}

struct parser_node * enumerator_list(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build enumerator_list\n");
	if((n1 = enumerator(state))){
		if((n1->next = enumerator_list_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUMERATOR_LIST);
		}else{
			add_parser_error(state, PARSER_ERROR_ENUMERATOR_LIST_MISSING_REST);
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		buffered_puts(state->buffered_output,"Required enumerator.  Cannot continue.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * enum_specifier(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build enum_specifier\n");
	if((n1 = p_accept(ENUM, state))){
		unsigned int identifier_id;
		if((n1->next = p_accept(OPEN_BRACE_CHAR, state))){
			if((n1->next->next = enumerator_list(state))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ENUM_SPECIFIER);
				} else {
					add_parser_error(state, PARSER_ERROR_ENUM_SPECIFIER_MISSING_CLOSE_BRACE_ANON);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_ENUM_SPECIFIER_MISSING_ENUMERATOR_LIST_ANON);
				return (struct parser_node *)0;
			}
		}else if((n1->next = get_identifier_node(state, &identifier_id))){
			if((n1->next->next = p_accept(OPEN_BRACE_CHAR, state))){
				if((n1->next->next->next = enumerator_list(state))){
					if((n1->next->next->next->next = p_accept(CLOSE_BRACE_CHAR, state))){
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

struct parser_node * struct_or_union(struct parser_state * state, enum type_engine_struct_or_union_specifier_kind * kind){
	struct parser_node * n1;
	parser_progress("Attempting to build struct_or_union\n");
	if((n1 = p_accept(STRUCT, state))){
		*kind = TYPE_ENGINE_STRUCT_SPECIFIER;
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_OR_UNION);
	}else if((n1 = p_accept(UNION, state))){
		*kind = TYPE_ENGINE_UNION_SPECIFIER;
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_OR_UNION);
	}else {
		parser_progress("Expected struct_or_union.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * type_specifier(struct parser_state * state, unsigned int * type_engine_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int temp_id;
	unsigned int identifier_id;
	parser_progress("Attempting to build type_specifier\n");
	if((n1 = p_accept(VOID, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, VOID));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(CHAR, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, CHAR));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(SHORT, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, SHORT));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(INT, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, INT));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(LONG, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, LONG));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(FLOAT, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, FLOAT));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(DOUBLE, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, DOUBLE));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(SIGNED, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, SIGNED));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(UNSIGNED, state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_SIMPLE_SPECIFIER, get_type_engine_id_for_simple_specifier(state->type_engine, UNSIGNED));
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = struct_or_union_specifier(state, &temp_id, consume_next_anonymous_tag_id_in_current_parser_scope(state->type_engine)))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_STRUCT_OR_UNION_SPECIFIER, temp_id);
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = enum_specifier(state))){
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_ENUM_SPECIFIER, 0); /* TODO:  Not really 0 */
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
	}else if((n1 = get_identifier_node(state, &identifier_id))){
		/*  This identifier can only be treated as a type if it has been declared with a typedef.  */
		unsigned char * ident = copy_string(n1->c_lexer_token->first_byte, n1->c_lexer_token->last_byte, state->memory_pool_collection);
		struct namespace_object * obj;
		struct scope_level * scope = get_parser_scope_level(state->type_engine);
		struct normalized_declaration_element * element;
		struct type_description_reference type_description;
		*type_engine_id = get_type_engine_id_for_all_specifier(state->type_engine, TYPE_ENGINE_TYPENAME_SPECIFIER, 0); /* TODO:  Not really 0 */
		type_description.t = (struct type_description *)0;
		obj = get_namespace_object_from_closest_namespace(ident, IDENTIFIER_NAMESPACE, scope, 0, state->memory_pool_collection);
		if(obj){
			element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, 0);
			type_description = create_type_description_from_normalized_declaration_element(state->memory_pool_collection, element, n1, scope, LVALUE);
		}
		if(obj && count_specifiers(type_description, TYPEDEF)){
			destroy_type_description(state->memory_pool_collection, type_description);
			heap_memory_pool_free(state->memory_pool_collection->heap_pool, ident);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_SPECIFIER);
		}else{
			if(type_description.t){
				destroy_type_description(state->memory_pool_collection, type_description);
			}
			backtrack(state, checkpoint);
			heap_memory_pool_free(state->memory_pool_collection->heap_pool, ident);
			parser_progress("Identifier not typedefed type.  Putting back tokens.\n");
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected a type_specifier.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * declaration_specifiers(struct parser_state * state, unsigned int previous_specifier_qualifier_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	unsigned int type_engine_type_specifier_id;
	unsigned int type_engine_type_qualifier_id;
	unsigned int type_engine_storage_class_specifier_id;
	parser_progress("Attempting to build declaration_specifiers\n");
	if((n1 = storage_class_specifier(state, &type_engine_storage_class_specifier_id))){
		unsigned int new_list_id = get_type_engine_specifier_qualifier_list_id(state->type_engine, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, previous_specifier_qualifier_list_id, get_type_engine_id_for_specifier_or_qualifiers(state->type_engine, TYPE_ENGINE_STORAGE_CLASS_SPECIFIER, type_engine_storage_class_specifier_id));
		if((n1->next = declaration_specifiers(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}
	}else if((n1 = type_specifier(state, &type_engine_type_specifier_id))){
		unsigned int new_list_id = get_type_engine_specifier_qualifier_list_id(state->type_engine, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, previous_specifier_qualifier_list_id, get_type_engine_id_for_specifier_or_qualifiers(state->type_engine, TYPE_ENGINE_TYPE_SPECIFIER, type_engine_type_specifier_id));
		if((n1->next = declaration_specifiers(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_SPECIFIERS);
		}
	}else if((n1 = type_qualifier(state, &type_engine_type_qualifier_id))){
		unsigned int new_list_id = get_type_engine_specifier_qualifier_list_id(state->type_engine, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, previous_specifier_qualifier_list_id, get_type_engine_id_for_specifier_or_qualifiers(state->type_engine, TYPE_ENGINE_SIMPLE_QUALIFIER, type_engine_type_qualifier_id));
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

struct parser_node * init_declarator_list_rest(struct parser_state * state, unsigned int specifier_qualifier_list_id, unsigned int prev_general_type_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build init_declarator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		unsigned int declarator_id;
		struct identifier_from_declarator identifier_id;
		identifier_id.initialized = 0; /*  No id obtained yet. */
		if((n1->next = init_declarator(state, &declarator_id, &identifier_id))){
			unsigned int general_type_id = get_type_engine_id_for_general_type(state->type_engine, specifier_qualifier_list_id, declarator_id);
			unsigned int general_type_list_item_id = get_type_engine_general_type_list_item_id(state->type_engine, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_general_type_list_id, general_type_id);
			if((n1->next->next = init_declarator_list_rest(state, specifier_qualifier_list_id, general_type_list_item_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INIT_DECLARATOR_LIST_REST);
			}else{
				parser_progress("Expected a init_declarator_list_rest.\n");
				backtrack(state, checkpoint);
				return (struct parser_node *)0;
			}
		}else{
			parser_progress("Expected a init_declarator.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_general_type_list_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, INIT_DECLARATOR_LIST_REST);
	}
}

struct parser_node * init_declarator_list(struct parser_state * state, unsigned int specifier_qualifier_list_id, unsigned int prev_general_type_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int declarator_id;
	struct identifier_from_declarator identifier_id;
	identifier_id.initialized = 0; /*  No id obtained yet. */
	parser_progress("Attempting to build init_declarator_list\n");
	if((n1 = init_declarator(state, &declarator_id, &identifier_id))){
		unsigned int general_type_id = get_type_engine_id_for_general_type(state->type_engine, specifier_qualifier_list_id, declarator_id);
		unsigned int general_type_list_item_id = get_type_engine_general_type_list_item_id(state->type_engine, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_general_type_list_id, general_type_id);
		if((n1->next = init_declarator_list_rest(state, specifier_qualifier_list_id, general_type_list_item_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INIT_DECLARATOR_LIST);
		}else{
			parser_progress("Expected a init_declarator_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected an init_declarator .\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * declaration(struct parser_state * state, unsigned int prev_general_type_list_item_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int declaration_specifiers_list_id;
	parser_progress("Attempting to build declaration\n");

	if((n1 = declaration_specifiers(state, get_type_engine_id_for_specifier_or_qualifier_list_begin(state->type_engine), &declaration_specifiers_list_id))){
		if((n1->next = init_declarator_list(state, declaration_specifiers_list_id, prev_general_type_list_item_id, this_list_id))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION);
			}else{
				parser_progress("Expected a SEMICOLON_CHAR in declaration.\n");
				backtrack(state, checkpoint);
				return (struct parser_node *)0;
			}
		}else if((n1->next = p_accept(SEMICOLON_CHAR, state))){
			unsigned int general_type_id = get_type_engine_id_for_general_type(state->type_engine, declaration_specifiers_list_id, get_type_engine_id_for_declarator_part_list_begin(state->type_engine));
			*this_list_id = get_type_engine_general_type_list_item_id(state->type_engine, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_general_type_list_item_id, general_type_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION);
		}else{
			parser_progress("Expected a SEMICOLON_CHAR.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected a declaration.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * declaration_list_rest(struct parser_state * state, struct namespace_object * previous_object, struct first_and_last_namespace_object * fl_all, unsigned int prev_general_type_list_item_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build declaration_list_rest\n");
	if((n1 = declaration(state, prev_general_type_list_item_id, this_list_id))){
		struct first_and_last_namespace_object fl_rest; /*  First and last object of all later declarations */
		struct first_and_last_namespace_object fl_current = manage_generic_declaration(state, n1, (struct normalized_declaration_set*)0, 0, LOCAL, previous_object, (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)0); /*  First and last object of current declaration */
		fl_rest.first = (struct namespace_object *)0;
		fl_rest.last = (struct namespace_object *)0;
		if((n1->next = declaration_list_rest(state, fl_current.last, &fl_rest, *this_list_id, this_list_id))){
			if(fl_current.last && fl_rest.last){
				/* The point of this exercise it to be able to point to the next one */
				struct namespace_object_change * c = (struct namespace_object_change *)malloc(sizeof(struct namespace_object_change));
				c->target = fl_current.last;
				c->old_obj = fl_current.last->next;
				c->new_obj = fl_rest.first;
				push_operation(state, SET_NEXT_NAMESPACE_OBJECT, (void*)c);
				fl_current.last = fl_rest.last; 
			}
			fl_all->first = fl_current.first ? fl_current.first : fl_rest.first;
			fl_all->last = fl_rest.last ? fl_rest.last : fl_current.last;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_LIST_REST);
		}else{
			parser_progress("Expected a declaration_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_general_type_list_item_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, DECLARATION_LIST_REST);
	}
}

struct parser_node * declaration_list(struct parser_state * state, struct namespace_object * previous_object, unsigned int prev_general_type_list_item_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build declaration_list\n");
	if((n1 = declaration(state, prev_general_type_list_item_id, this_list_id))){
		struct scope_level * scope = get_parser_scope_level(state->type_engine);
		struct first_and_last_namespace_object fl_rest; /*  First and last object of all later declarations */
		struct first_and_last_namespace_object fl_current = manage_generic_declaration(state, n1, (struct normalized_declaration_set*)0, 0, LOCAL, previous_object, (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)0); /*  First and last object of current declaration */
		fl_rest.first = (struct namespace_object *)0;
		fl_rest.last = (struct namespace_object *)0;
		if((n1->next = declaration_list_rest(state, fl_current.last, &fl_rest, *this_list_id, this_list_id))){
			scope->first_local_object = fl_current.first ? fl_current.first : fl_rest.first; /*  This block scope will need to keep track of the first local it has */
			if(fl_current.last && fl_rest.last){
				/* The point of this exercise it to be able to point to the next one */
				struct namespace_object_change * c = (struct namespace_object_change *)malloc(sizeof(struct namespace_object_change));
				c->target = fl_current.last;
				c->old_obj = fl_current.last->next;
				c->new_obj = fl_rest.first;
				push_operation(state, SET_NEXT_NAMESPACE_OBJECT, (void*)c);
				fl_current.last = fl_rest.last; 
			}
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATION_LIST);
		}else{
			parser_progress("Expected a declaration_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected a declaration.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * identifier_list_rest(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build identifier_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		if((n1->next = p_accept(IDENTIFIER, state))){
			if((n1->next->next = identifier_list_rest(state))){
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

struct parser_node * identifier_list(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build identifier_list\n");
	if((n1 = p_accept(IDENTIFIER, state))){
		if((n1->next = identifier_list_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, IDENTIFIER_LIST);
		}else{
			parser_progress("Expected a IDENTIFIER_LIST.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected an IDENTIFIER.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * direct_declarator_rest(struct parser_state * state, unsigned int previous_declarator_item_list_id, unsigned int * this_list_id, struct identifier_from_declarator * identifier){
	struct parser_node * n1;
	parser_progress("Attempting to build direct_declarator_rest\n");
	if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR,state))){
		if((n1->next = constant_expression(state))){
			unsigned int constant_expression_id = get_type_engine_id_for_constant_parser_node(state->type_engine, n1->next);
			(void)constant_expression_id;
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR,state))){
				unsigned int current_declarator_part_id 	= get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_ARRAY_DECLARATOR_PART, get_type_engine_id_for_array_part(state->type_engine, TYPE_ENGINE_CONSTANT_EXPRESSION_ARRAY, 0 /*  TODO:  Need to actually determine constant value ids in the future. */)));
				if((n1->next->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier))){
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
			unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_ARRAY_DECLARATOR_PART, get_type_engine_id_for_array_part(state->type_engine, TYPE_ENGINE_FLEXIBLE_ARRAY, 0)));
			if((n1->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR_REST);
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_ARRAY_UNSIZED);
				return (struct parser_node *)0;
			}
		}else{
			add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_OR_CONSTANT);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(OPEN_PAREN_CHAR,state))){
		if((n1->next = parameter_type_list(state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR,state))){
				unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_FUNCTION_DECLARATOR_PART, get_type_engine_id_for_function_part(state->type_engine, TYPE_ENGINE_FUNCTION_PROTOTYPE, 0/* TODO: Parameter list */)));
				if((n1->next->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR_REST);
				}else{
					add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_PARAM_FUNCTION);
					return (struct parser_node *)0;
				}
			}else{
				add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_PARAM);
				return (struct parser_node *)0;
			}
		}else if((n1->next = identifier_list(state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR,state))){
				unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_FUNCTION_DECLARATOR_PART, get_type_engine_id_for_function_part(state->type_engine, TYPE_ENGINE_FUNCTION_K_AND_R_C, 0)));
				if((n1->next->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier))){
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
			unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_FUNCTION_DECLARATOR_PART, get_type_engine_id_for_function_part(state->type_engine, TYPE_ENGINE_FUNCTION_K_AND_R_C, 0)));
			if((n1->next->next = direct_declarator_rest(state, current_declarator_part_id, this_list_id, identifier))){
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

struct parser_node * type_qualifier_list_rest(struct parser_state * state, unsigned int previous_specifier_qualifier_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int type_engine_type_qualifier_id;
	parser_progress("Attempting to build type_qualifier_list_rest\n");
	if((n1 = type_qualifier(state, &type_engine_type_qualifier_id))){
		unsigned int new_list_id = get_type_engine_specifier_qualifier_list_id(state->type_engine, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, previous_specifier_qualifier_list_id, get_type_engine_id_for_specifier_or_qualifiers(state->type_engine, TYPE_ENGINE_SIMPLE_QUALIFIER, type_engine_type_qualifier_id));
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

struct parser_node * type_qualifier_list(struct parser_state * state, unsigned int previous_specifier_qualifier_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int type_engine_type_qualifier_id;
	parser_progress("Attempting to build type_qualifier_list\n");
	if((n1 = type_qualifier(state, &type_engine_type_qualifier_id))){
		unsigned int new_list_id = get_type_engine_specifier_qualifier_list_id(state->type_engine, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, previous_specifier_qualifier_list_id, get_type_engine_id_for_specifier_or_qualifiers(state->type_engine, TYPE_ENGINE_SIMPLE_QUALIFIER, type_engine_type_qualifier_id));
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


struct parser_node * pointer(struct parser_state * state, unsigned int previous_declarator_item_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	parser_progress("Attempting to build pointer\n");
	if((n1 = p_accept(MULTIPLY_CHAR, state))){
		unsigned int type_engine_specifier_qualifier_list_id;
		if((n1->next = type_qualifier_list(state, get_type_engine_id_for_specifier_or_qualifier_list_begin(state->type_engine), &type_engine_specifier_qualifier_list_id))){
			unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_POINTER_DECLARATOR_PART, get_type_engine_id_for_pointer_part(state->type_engine, TYPE_ENGINE_QUALIFIED_POINTER, type_engine_specifier_qualifier_list_id)));
			if((n1->next->next = pointer(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POINTER);
			}else{
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, POINTER);
			}
		}else {
			unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_POINTER_DECLARATOR_PART, get_type_engine_id_for_pointer_part(state->type_engine, TYPE_ENGINE_UNQUALIFIED_POINTER, 0)));
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

struct parser_node * direct_declarator(struct parser_state * state, unsigned int previous_declarator_item_list_id, unsigned int * this_list_id, struct identifier_from_declarator * identifier){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build direct_declarator\n");
	assert(!identifier->initialized);
	if((n1 = get_identifier_node(state, &identifier->id))){
		identifier->initialized = 1; /*  id does point to a valid identifier */
		if((n1->next = direct_declarator_rest(state, previous_declarator_item_list_id, this_list_id, identifier))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_DECLARATOR);
		}else{
			add_parser_error(state, PARSER_ERROR_DIRECT_DECLARATOR_MISSING_REST_AFTER_IDENTIFIER);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		if((n1->next = declarator(state, previous_declarator_item_list_id, this_list_id, identifier))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				if((n1->next->next->next = direct_declarator_rest(state, *this_list_id, this_list_id, identifier))){
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



struct parser_node * declarator(struct parser_state * state, unsigned int previous_declarator_item_list_id, unsigned int * this_list_id, struct identifier_from_declarator * identifier){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int pointer_declarator_list_id;
	unsigned int direct_declarator_list_id;
	parser_progress("Attempting to build declarator\n");
	if((n1 = pointer(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &pointer_declarator_list_id))){
		if((n1->next = direct_declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &direct_declarator_list_id, identifier))){
			/*  Special case: */
			*this_list_id = add_pointer_and_direct_declarators_to_list(state->type_engine, previous_declarator_item_list_id, pointer_declarator_list_id, direct_declarator_list_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATOR);
		}else{
			parser_progress("Expected a direct declarator.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else if((n1 = direct_declarator(state, previous_declarator_item_list_id, this_list_id, identifier))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DECLARATOR);
	}else{
		parser_progress("Expected a pointer or direct declarator.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * initializer_list_rest(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build initializer_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR,state))){
		if((n1->next = initializer(state))){
			if((n1->next->next = initializer_list_rest(state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER_LIST_REST);
			}else{
				assert(0 && "Impossible");
			}
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER_LIST_REST);
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, INITIALIZER_LIST_REST);
	}
}

struct parser_node * initializer_list(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build initializer_list\n");
	if((n1 = initializer(state))){
		if((n1->next = initializer_list_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER_LIST);
		}else{
			assert(0 && "Impossible");
		}
	}else{
		parser_progress("Expected initializer.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * initializer(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build initializer\n");
	if((n1 = assignment_expression(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INITIALIZER);
	}else if((n1 = p_accept(OPEN_BRACE_CHAR, state))){
		if((n1->next = initializer_list(state))){
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

struct parser_node * init_declarator(struct parser_state * state, unsigned int * bitfield_or_declarator_id, struct identifier_from_declarator * identifier){
	struct parser_node * n1;
	unsigned int declarator_item_list_id;
	parser_progress("Attempting to build init_declarator\n");
	if((n1 = declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &declarator_item_list_id, identifier))){
		*bitfield_or_declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_ONLY_DECLARATOR, declarator_item_list_id);
		if((n1->next = p_accept(EQUALS_CHAR, state))){
			if((n1->next->next = initializer(state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INIT_DECLARATOR);
			}else{
				add_parser_error(state, PARSER_ERROR_INIT_DECLARATOR_MISSING_INITIALIZER);
				return (struct parser_node *)0;
			}
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, INIT_DECLARATOR);
		}
	}else{
		parser_progress("Expected declarator in init declarator.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * struct_declarator(struct parser_state * state, unsigned int * bitfield_or_declarator_id, struct identifier_from_declarator * identifier){
	struct parser_node * n1;
	unsigned int declarator_item_list_id;
	parser_progress("Attempting to build struct_declarator\n");
	if((n1 = declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &declarator_item_list_id, identifier))){
		if((n1->next = p_accept(COLON_CHAR, state))){
			if((n1->next->next = constant_expression(state))){
				unsigned int constant_expression_id = get_type_engine_id_for_constant_parser_node(state->type_engine, n1->next->next);
				*bitfield_or_declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_BITFIELD_AND_DECLARATOR, 0/* TODO: id for declarator and constant expression */);
				(void)constant_expression_id;
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected constant_expression.\n");
				return (struct parser_node *)0;
			}
		}else{
			*bitfield_or_declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_ONLY_DECLARATOR, declarator_item_list_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR);
		}
	}else if((n1 = p_accept(COLON_CHAR, state))){
		if((n1->next = constant_expression(state))){
			unsigned int constant_expression_id = get_type_engine_id_for_constant_parser_node(state->type_engine, n1->next);
			(void)constant_expression_id;
			*bitfield_or_declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_ONLY_BITFIELD, 0/* TODO: id for bitfield */);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected constant_expression.\n");
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected COLON_CHAR or declarator.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * struct_declarator_list_rest(struct parser_state * state, unsigned int specifier_qualifier_list_id, unsigned int prev_general_type_list_id, unsigned int * this_list_id, struct declaration_namespace * dn){
	struct parser_node * n1;
	parser_progress("Attempting to build struct_declarator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		unsigned int declarator_id;
		struct identifier_from_declarator identifier_id;
		identifier_id.initialized = 0; /*  No id obtained yet. */
		if((n1->next = struct_declarator(state, &declarator_id, &identifier_id))){
			unsigned int general_type_id = get_type_engine_id_for_general_type(state->type_engine, specifier_qualifier_list_id, declarator_id);
			unsigned int general_type_list_item_id = get_type_engine_general_type_list_item_id(state->type_engine, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_general_type_list_id, general_type_id);

			add_ordered_general_type_to_declaration_namespace(state->type_engine, dn, general_type_id);
			assert(identifier_id.initialized);
			add_identifier_id_to_declaration_namespace(state->type_engine, dn, general_type_id, identifier_id.id);

			if((n1->next->next = struct_declarator_list_rest(state, specifier_qualifier_list_id, general_type_list_item_id, this_list_id, dn))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR_LIST_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected struct_declarator_list_rest.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected struct_declarator.\n");
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_general_type_list_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, STRUCT_DECLARATOR_LIST_REST);
	}
}


struct parser_node * struct_declarator_list(struct parser_state * state, unsigned int specifier_qualifier_list_id, unsigned int prev_general_type_list_id, unsigned int * this_list_id, struct declaration_namespace * dn){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int declarator_id;
	struct identifier_from_declarator identifier_id;
	identifier_id.initialized = 0; /*  No id obtained yet. */
	parser_progress("Attempting to build struct_declarator_list\n");
	if((n1 = struct_declarator(state, &declarator_id, &identifier_id))){
		unsigned int general_type_id = get_type_engine_id_for_general_type(state->type_engine, specifier_qualifier_list_id, declarator_id);
		unsigned int general_type_list_item_id = get_type_engine_general_type_list_item_id(state->type_engine, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_general_type_list_id, general_type_id);
		if((n1->next = struct_declarator_list_rest(state, specifier_qualifier_list_id, general_type_list_item_id, this_list_id, dn))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATOR_LIST);
		}else{
			parser_progress("Expected struct_declarator_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected struct_declarator.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * specifier_qualifier_list(struct parser_state * state, unsigned int previous_specifier_qualifier_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	unsigned int type_engine_type_specifier_id;
	unsigned int type_engine_type_qualifier_id;
	parser_progress("Attempting to build specifier_qualifier_list\n");
	if((n1 = type_specifier(state, &type_engine_type_specifier_id))){
		unsigned int new_list_id = get_type_engine_specifier_qualifier_list_id(state->type_engine, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, previous_specifier_qualifier_list_id, get_type_engine_id_for_specifier_or_qualifiers(state->type_engine, TYPE_ENGINE_TYPE_SPECIFIER, type_engine_type_specifier_id));
		if((n1->next = specifier_qualifier_list(state, new_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SPECIFIER_QUALIFIER_LIST);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SPECIFIER_QUALIFIER_LIST);
		}
	}else if((n1 = type_qualifier(state, &type_engine_type_qualifier_id))){
		unsigned int new_list_id = get_type_engine_specifier_qualifier_list_id(state->type_engine, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, previous_specifier_qualifier_list_id, get_type_engine_id_for_specifier_or_qualifiers(state->type_engine, TYPE_ENGINE_SIMPLE_QUALIFIER, type_engine_type_qualifier_id));
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

struct parser_node * struct_declaration(struct parser_state * state, unsigned int prev_generic_type_list_item_id, unsigned int * this_list_id, struct declaration_namespace * dn){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int type_engine_specifier_qualifier_list_id;
	parser_progress("Attempting to build struct_declaration\n");
	if((n1 = specifier_qualifier_list(state, get_type_engine_id_for_specifier_or_qualifier_list_begin(state->type_engine), &type_engine_specifier_qualifier_list_id))){
		if((n1->next = struct_declarator_list(state, type_engine_specifier_qualifier_list_id, prev_generic_type_list_item_id, this_list_id, dn))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR,state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATION);
			}else{
				parser_progress("Expected SEMICOLON_CHAR.\n");
				backtrack(state, checkpoint);
				return (struct parser_node *)0;
			}
		}else{
			/*  TODO:  I'm pretty sure there is a case missing here with un-named structure members. */
			parser_progress("Expected struct_declarator_list.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected specifier_qualifier_list.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * struct_declaration_list_rest(struct parser_state * state, unsigned int prev_generic_type_list_item_id, unsigned int * this_list_id, struct declaration_namespace * dn){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build struct_declaration_list_rest\n");
	if((n1 = struct_declaration(state, prev_generic_type_list_item_id, this_list_id, dn))){
		if((n1->next = struct_declaration_list_rest(state, *this_list_id, this_list_id, dn))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATION_LIST_REST);
		}else{
			parser_progress("Expected struct_declaration_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_generic_type_list_item_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, STRUCT_DECLARATION_LIST_REST);
	}
}


struct parser_node * struct_declaration_list(struct parser_state * state, unsigned int prev_generic_type_list_item_id, unsigned int * this_list_id, struct declaration_namespace * dn){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build struct_declaration_list\n");
	if((n1 = struct_declaration(state, prev_generic_type_list_item_id, this_list_id, dn))){
		if((n1->next = struct_declaration_list_rest(state, *this_list_id, this_list_id, dn))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_DECLARATION_LIST);
		}else{
			parser_progress("Expected struct_declaration_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected struct_declaration.\n");
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
		}case PARSER_ERROR_STRUCT_OR_UNION_REDEFINITION:{
			buffered_printf(list, "Duplicate struct or union definition.\n");
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
		}case PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_NO_REST:{
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
			buffered_printf(list, "Missing a closing brace to end compound statement.\n");
			break;
		}case PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_2:{
			buffered_printf(list, "Missing a closing brace to end compound statement.\n");
			break;
		}case PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_3:{
			buffered_printf(list, "Missing a closing brace to end compound statement.\n");
			break;
		}case PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_4:{
			buffered_printf(list, "Missing a closing brace to end compound statement.\n");
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

struct parser_node * struct_or_union_specifier(struct parser_state * state, unsigned int * scoped_struct_or_union_specifier_id, unsigned int possible_anonymous_tag_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	enum type_engine_struct_or_union_specifier_kind kind;
	struct scope_level * current_scope = get_parser_scope_level(state->type_engine); 
	parser_progress("Attempting to build struct_or_union_specifier\n");
	if((n1 = struct_or_union(state, &kind))){
		unsigned int identifier_id;
		if((n1->next = get_identifier_node(state, &identifier_id))){
			struct scope_level * struct_or_union_origin_scope;
			struct scope_level * cloest_declared_scope;
			unsigned int non_anonymous_id;
			unsigned int struct_or_union_non_anonymous_id;
			unsigned int scope_id;
			if(kind == TYPE_ENGINE_STRUCT_SPECIFIER){
				non_anonymous_id = get_type_engine_id_for_struct_specifier(state->type_engine, TYPE_ENGINE_NAMED_STRUCT, identifier_id);
			}else{
				non_anonymous_id = get_type_engine_id_for_union_specifier(state->type_engine, TYPE_ENGINE_NAMED_UNION, identifier_id);
			}
			/*  Non-scoped id that identifies all of 'struct foo' in any scope as the same thing.  */
			struct_or_union_non_anonymous_id = get_type_engine_id_for_struct_or_union_specifier(state->type_engine, kind, non_anonymous_id);
			cloest_declared_scope = get_scope_of_closest_struct_or_union_tag_declaration(state->type_engine, current_scope, struct_or_union_non_anonymous_id);
			if(!cloest_declared_scope){
				/*  If there is no closest one, then this must declare an incomplete struct or union type. */
				add_named_struct_or_union_declaration(state->type_engine, identifier_id, struct_or_union_non_anonymous_id, current_scope);
			}
			struct_or_union_origin_scope = cloest_declared_scope ? cloest_declared_scope : current_scope;
			scope_id = get_type_engine_id_for_scope_level(state->type_engine, struct_or_union_origin_scope);
			*scoped_struct_or_union_specifier_id = get_type_engine_id_for_scoped_struct_or_union_specifier(state->type_engine, scope_id, struct_or_union_non_anonymous_id);
			if((n1->next->next = p_accept(OPEN_BRACE_CHAR,state))){
				unsigned int generic_type_list_id;
				struct declaration_namespace * dn = create_declaration_namespace(state->type_engine);

				if((n1->next->next->next = struct_declaration_list(state, get_type_engine_id_for_general_type_list_begin(state->type_engine), &generic_type_list_id, dn))){
					if((n1->next->next->next->next = p_accept(CLOSE_BRACE_CHAR,state))){
						if(struct_or_union_origin_scope != current_scope){
							/*  If this is a struct or union definition, it wont complete the incomplete type from the outer scope */
							scope_id = get_type_engine_id_for_scope_level(state->type_engine, current_scope);
							*scoped_struct_or_union_specifier_id = get_type_engine_id_for_scoped_struct_or_union_specifier(state->type_engine, scope_id, struct_or_union_non_anonymous_id);
						}
						if(add_struct_or_union_definition(state->type_engine, dn, struct_or_union_non_anonymous_id, current_scope)){
							add_parser_error(state, PARSER_ERROR_STRUCT_OR_UNION_REDEFINITION);
						}
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
		}else if((n1->next = p_accept(OPEN_BRACE_CHAR,state))){
			struct declaration_namespace * dn = create_declaration_namespace(state->type_engine);
			unsigned int generic_type_list_id;
			unsigned int anonymous_id;
			unsigned int struct_or_union_anonymous_id;
			unsigned int anonymous_declaration_id = possible_anonymous_tag_id; /* Unique id for each anonymous tag in a scope. */
			unsigned int scope_id = get_type_engine_id_for_scope_level(state->type_engine, current_scope);
			if(kind == TYPE_ENGINE_STRUCT_SPECIFIER){
				anonymous_id = get_type_engine_id_for_struct_specifier(state->type_engine, TYPE_ENGINE_ANONYMOUS_STRUCT, anonymous_declaration_id);
			}else{
				anonymous_id = get_type_engine_id_for_union_specifier(state->type_engine, TYPE_ENGINE_ANONYMOUS_UNION, anonymous_declaration_id);
			}
			 struct_or_union_anonymous_id = get_type_engine_id_for_struct_or_union_specifier(state->type_engine, kind, anonymous_id);
			*scoped_struct_or_union_specifier_id = get_type_engine_id_for_scoped_struct_or_union_specifier(state->type_engine, scope_id, struct_or_union_anonymous_id);

			if((n1->next->next = struct_declaration_list(state, get_type_engine_id_for_general_type_list_begin(state->type_engine), &generic_type_list_id, dn))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR,state))){
					if(add_struct_or_union_definition(state->type_engine, dn, struct_or_union_anonymous_id, current_scope)){
						add_parser_error(state, PARSER_ERROR_STRUCT_OR_UNION_REDEFINITION);
					}
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, STRUCT_OR_UNION_SPECIFIER);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_BRACE_CHAR.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected struct_declaration_list.\n");
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

struct parser_node * parameter_declaration(struct parser_state * state, unsigned int prev_general_type_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	unsigned int declaration_specifiers_list_id;
	parser_progress("Attempting to build parameter_declaration\n");
	if((n1 = declaration_specifiers(state, get_type_engine_id_for_specifier_or_qualifier_list_begin(state->type_engine), &declaration_specifiers_list_id))){
		unsigned int declarator_list_item_id;
		struct identifier_from_declarator identifier_id;
		identifier_id.initialized = 0; /*  No id obtained yet. */
		if((n1->next = declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &declarator_list_item_id, &identifier_id))){
			unsigned int bitfield_or_declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_ONLY_DECLARATOR, declarator_list_item_id);
			unsigned int general_type_id = get_type_engine_id_for_general_type(state->type_engine, declaration_specifiers_list_id, bitfield_or_declarator_id);

			*this_list_id = get_type_engine_general_type_list_item_id(state->type_engine, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_general_type_list_id, general_type_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_DECLARATION);
		}else if((n1->next = abstract_declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &declarator_list_item_id))){
			unsigned int bitfield_or_declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_ONLY_DECLARATOR, declarator_list_item_id);
			unsigned int general_type_id = get_type_engine_id_for_general_type(state->type_engine, declaration_specifiers_list_id, bitfield_or_declarator_id);
			*this_list_id = get_type_engine_general_type_list_item_id(state->type_engine, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_general_type_list_id, general_type_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_DECLARATION);
		}else{
			unsigned int bitfield_or_declarator_id;
			unsigned int general_type_id;
			declarator_list_item_id = get_type_engine_id_for_declarator_part_list_begin(state->type_engine);
			bitfield_or_declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_ONLY_DECLARATOR, declarator_list_item_id);
			general_type_id = get_type_engine_id_for_general_type(state->type_engine, declaration_specifiers_list_id, bitfield_or_declarator_id);

			*this_list_id = get_type_engine_general_type_list_item_id(state->type_engine, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_general_type_list_id, general_type_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_DECLARATION);
		}
	}else{
		parser_progress("Expected parameter_declaration.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * parameter_list_rest(struct parser_state * state, unsigned int prev_generic_type_list_item_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build parameter_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, state))){
		if((n1->next = parameter_declaration(state, prev_generic_type_list_item_id, this_list_id))){
			if((n1->next->next = parameter_list_rest(state, *this_list_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_LIST_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected parameter_list_rest, (in parameter_list_rest).\n");
				return (struct parser_node *)0;
			}
		}else{
			/* Re-wind back over the ',' that we accepted */
			backtrack(state, checkpoint);
			n1 = make_epsilon(state);
			*this_list_id = prev_generic_type_list_item_id;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_LIST_REST);
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = prev_generic_type_list_item_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, PARAMETER_LIST_REST);
	}
}

struct parser_node * parameter_list(struct parser_state * state, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build parameter_list\n");
	if((n1 = parameter_declaration(state, get_type_engine_id_for_general_type_list_begin(state->type_engine), this_list_id))){
		if((n1->next = parameter_list_rest(state, *this_list_id, this_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_LIST);
		}else{
			parser_progress("Expected parameter_list_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected parameter_declaration.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * parameter_type_list(struct parser_state * state){
	struct parser_node * n1;
	unsigned int parameter_list_id;
	unsigned int parameter_type_list_id;
	parser_progress("Attempting to build parameter_type_list\n");
	if((n1 = parameter_list(state, &parameter_list_id))){
		if((n1->next = p_accept(COMMA_CHAR, state))){
			if((n1->next->next = p_accept(ELLIPSIS, state))){
				parameter_type_list_id = get_type_engine_id_for_parameter_list(state->type_engine, TYPE_ENGINE_PARAMETER_LIST_VARIADIC, parameter_list_id);
				(void)parameter_type_list_id;
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_TYPE_LIST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected parameter_list_rest.\n");
				return (struct parser_node *)0;
			}
		}else{
			parameter_type_list_id = get_type_engine_id_for_parameter_list(state->type_engine, TYPE_ENGINE_PARAMETER_LIST_NON_VARIADIC, parameter_list_id);
			(void)parameter_type_list_id;
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, PARAMETER_TYPE_LIST);
		}
	}else{
		parser_progress("Expected parameter_list.\n");
		return (struct parser_node *)0;
	}
}



struct parser_node * direct_abstract_declarator_rest(struct parser_state * state, unsigned int previous_declarator_item_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build direct_abstract_declarator_rest\n");
	if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, state))){
			unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_FUNCTION_DECLARATOR_PART, get_type_engine_id_for_function_part(state->type_engine, TYPE_ENGINE_FUNCTION_K_AND_R_C, 0)));
			if((n1->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
				return (struct parser_node *)0;
			}
		}else if((n1->next = parameter_type_list(state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_FUNCTION_DECLARATOR_PART, get_type_engine_id_for_function_part(state->type_engine, TYPE_ENGINE_FUNCTION_PROTOTYPE, 0/* TODO: Parameter list */)));
				if((n1->next->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
				return (struct parser_node *)0;
			}
		}else{
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, state))){
		if((n1->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
			unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_ARRAY_DECLARATOR_PART, get_type_engine_id_for_array_part(state->type_engine, TYPE_ENGINE_FLEXIBLE_ARRAY, 0)));
			if((n1->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
				return (struct parser_node *)0;
			}
		}else if((n1->next = constant_expression(state))){
			unsigned int constant_expression_id = get_type_engine_id_for_constant_parser_node(state->type_engine, n1->next);
			(void)constant_expression_id;
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
				unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_ARRAY_DECLARATOR_PART, get_type_engine_id_for_array_part(state->type_engine, TYPE_ENGINE_CONSTANT_EXPRESSION_ARRAY, 0 /*  TODO:  Need to actually determine constant value ids in the future. */)));
				if((n1->next->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_SQUARE_BRACKET_CHAR.\n");
				return (struct parser_node *)0;
			}
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		*this_list_id = previous_declarator_item_list_id;
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR_REST);
	}
}


struct parser_node * direct_abstract_declarator(struct parser_state * state, unsigned int previous_declarator_item_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	parser_progress("Attempting to build direct_abstract_declarator\n");
	if((n1 = p_accept(OPEN_PAREN_CHAR, state))){
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, state))){
			unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_FUNCTION_DECLARATOR_PART, get_type_engine_id_for_function_part(state->type_engine, TYPE_ENGINE_FUNCTION_K_AND_R_C, 0)));
			if((n1->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
				return (struct parser_node *)0;
			}
		}else if((n1->next = parameter_type_list(state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_FUNCTION_DECLARATOR_PART, get_type_engine_id_for_function_part(state->type_engine, TYPE_ENGINE_FUNCTION_PROTOTYPE, 0/* TODO: Parameter list */)));
				if((n1->next->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
				return (struct parser_node *)0;
			}
		}else if((n1->next = abstract_declarator(state, previous_declarator_item_list_id, this_list_id))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
				if((n1->next->next->next = direct_abstract_declarator_rest(state, *this_list_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
				return (struct parser_node *)0;
			}
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
		}
	}else if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, state))){
		if((n1->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
			unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_ARRAY_DECLARATOR_PART, get_type_engine_id_for_array_part(state->type_engine, TYPE_ENGINE_FLEXIBLE_ARRAY, 0)));
			if((n1->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
				return (struct parser_node *)0;
			}
		}else if((n1->next = constant_expression(state))){
			unsigned int constant_expression_id = get_type_engine_id_for_constant_parser_node(state->type_engine, n1->next);
			(void)constant_expression_id;
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, state))){
				unsigned int current_declarator_part_id = get_type_engine_declarator_part_list_item_id(state->type_engine, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, previous_declarator_item_list_id, get_type_engine_id_for_declarator_part(state->type_engine, TYPE_ENGINE_ARRAY_DECLARATOR_PART, get_type_engine_id_for_array_part(state->type_engine, TYPE_ENGINE_CONSTANT_EXPRESSION_ARRAY, 0 /*  TODO:  Need to actually determine constant value ids in the future. */)));
				if((n1->next->next->next = direct_abstract_declarator_rest(state, current_declarator_part_id, this_list_id))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_SQUARE_BRACKET_CHAR.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_SQUARE_BRACKET_CHAR.\n");
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected OPEN_SQUARE_BRACKET_CHAR or OPEN_PAREN_CHAR.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * type_name(struct parser_state * state){
	struct parser_node * n1;
	unsigned int type_engine_specifier_qualifier_list_id;
	unsigned int declarator_item_list_id;
	parser_progress("Attempting to build type_name\n");
	if((n1 = specifier_qualifier_list(state, get_type_engine_id_for_specifier_or_qualifier_list_begin(state->type_engine), &type_engine_specifier_qualifier_list_id))){
		if((n1->next = abstract_declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &declarator_item_list_id))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_NAME);
		}else{
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TYPE_NAME);
		}
	}else{
		parser_progress("Expected specifier_qualifier_list.\n");
		return (struct parser_node *)0;
	}
}


struct parser_node * abstract_declarator(struct parser_state * state, unsigned int previous_declarator_item_list_id, unsigned int * this_list_id){
	struct parser_node * n1;
	unsigned int pointer_declarator_list_id;
	unsigned int direct_declarator_list_id;
	parser_progress("Attempting to build abstract_declarator\n");
	if((n1 = pointer(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &pointer_declarator_list_id))){
		if((n1->next = direct_abstract_declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &direct_declarator_list_id))){
			/*  Special case: */
			*this_list_id = add_pointer_and_direct_declarators_to_list(state->type_engine, previous_declarator_item_list_id, pointer_declarator_list_id, direct_declarator_list_id);
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ABSTRACT_DECLARATOR);
		}else{
			add_declarators_to_list(state->type_engine, &previous_declarator_item_list_id, pointer_declarator_list_id, 1);
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

struct parser_node * labeled_statement(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int identifier_id;
	parser_progress("Attempting to build labeled_statement\n");
	if((n1 = get_identifier_node(state, &identifier_id))){
		if((n1->next = p_accept(COLON_CHAR, state))){
			if((n1->next->next = statement(state, 0))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LABELED_STATEMENT);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
				return (struct parser_node *)0;
			}
		}else{
			parser_progress("Assuming not a label, resetting position.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(CASE,state))){
		if((n1->next = constant_expression(state))){
			if((n1->next->next = p_accept(COLON_CHAR, state))){
				struct scope_level * new_scope_level = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
				if((n1->next->next->next = statement(state, 1))){
					decrement_scope_depth(state->type_engine);
					return associate_parser_node_with_scope_level(state->type_engine, new_scope_level, create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LABELED_STATEMENT));
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected COLON_CHAR.\n");
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DEFAULT,state))){
		if((n1->next = p_accept(COLON_CHAR, state))){
			struct scope_level * new_scope_level = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
			if((n1->next->next = statement(state, 1))){
				decrement_scope_depth(state->type_engine);
				return associate_parser_node_with_scope_level(state->type_engine, new_scope_level, create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, LABELED_STATEMENT));
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected COLON_CHAR.\n");
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected labeled_statement.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * selection_statement(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build selection_statement\n");
	if((n1 = p_accept(IF,state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, state))){
			if((n1->next->next = expression(state))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					struct scope_level * new_scope_level1 = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
					if((n1->next->next->next->next = statement(state, 1))){
						decrement_scope_depth(state->type_engine);
						associate_parser_node_with_scope_level(state->type_engine, new_scope_level1, n1->next->next->next->next);
						if((n1->next->next->next->next->next = p_accept(ELSE, state))){
							struct scope_level * new_scope_level2 = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
							if((n1->next->next->next->next->next->next = statement(state, 1))){
								decrement_scope_depth(state->type_engine);
								associate_parser_node_with_scope_level(state->type_engine, new_scope_level2, n1->next->next->next->next->next->next);
								return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SELECTION_STATEMENT);
							}else{
								assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement after else.\n");
								return (struct parser_node *)0;
							}
						}else{
							return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SELECTION_STATEMENT);
						}
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement after if.\n");
						return (struct parser_node *)0;
					}
				}else{
					printf("Parser could not continue on line %d of file %s\n", state->line_number, state->c_lexer_state->c.filename);
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR.\n");
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(SWITCH, state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, state))){
			if((n1->next->next = expression(state))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					struct scope_level * new_scope_level = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
					if((n1->next->next->next->next = statement(state, 1))){
						decrement_scope_depth(state->type_engine);
						associate_parser_node_with_scope_level(state->type_engine, new_scope_level, n1->next->next->next->next);
						return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, SELECTION_STATEMENT);
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
						return (struct parser_node *)0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR.\n");
			return (struct parser_node *)0;
		}
	}else {
		parser_progress("Expected IF or SWITCH.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * iteration_statement(struct parser_state * state){
	struct parser_node * n1;
	parser_progress("Attempting to build iteration_statement\n");
	if((n1 = p_accept(WHILE,state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, state))){
			if((n1->next->next = expression(state))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
					struct scope_level * new_scope_level = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
					if((n1->next->next->next->next = statement(state, 1))){
						decrement_scope_depth(state->type_engine);
						associate_parser_node_with_scope_level(state->type_engine, new_scope_level, n1->next->next->next->next);
						return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ITERATION_STATEMENT);
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
						return (struct parser_node *)0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR for while.\n");
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(DO,state))){
		struct scope_level * new_scope_level = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
		if((n1->next = statement(state, 1))){
			decrement_scope_depth(state->type_engine);
			if((n1->next->next = p_accept(WHILE, state))){
				if((n1->next->next->next = p_accept(OPEN_PAREN_CHAR, state))){
					if((n1->next->next->next->next = expression(state))){
						if((n1->next->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
							if((n1->next->next->next->next->next->next = p_accept(SEMICOLON_CHAR, state))){
								associate_parser_node_with_scope_level(state->type_engine, new_scope_level, n1->next);
								return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ITERATION_STATEMENT);
							}else{
								assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
								return (struct parser_node *)0;
							}
						}else{
							assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
							return (struct parser_node *)0;
						}
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression.\n");
						return (struct parser_node *)0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected WHILE.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(FOR,state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR,state))){
			if((n1->next->next = expression_statement(state))){
				if((n1->next->next->next = expression_statement(state))){
					if((n1->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
						struct scope_level * new_scope_level = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
						if((n1->next->next->next->next->next = statement(state, 1))){
							decrement_scope_depth(state->type_engine);
							associate_parser_node_with_scope_level(state->type_engine, new_scope_level, n1->next->next->next->next->next);
							return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ITERATION_STATEMENT);
						}else{
							assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression or statement.\n");
							return (struct parser_node *)0;
						}
					}else if((n1->next->next->next->next = expression(state))){
						if((n1->next->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, state))){
							struct scope_level * new_scope_level = increment_scope_depth(state->type_engine, SCOPE_LEVEL_TYPE_BLOCK_SCOPE);
							if((n1->next->next->next->next->next->next = statement(state, 1))){
								decrement_scope_depth(state->type_engine);
								associate_parser_node_with_scope_level(state->type_engine, new_scope_level, n1->next->next->next->next->next->next);
								return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, ITERATION_STATEMENT);
							}else{
								assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
								return (struct parser_node *)0;
							}
						}else{
							assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
							return (struct parser_node *)0;
						}
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression or CLOSE_PAREN_CHAR.\n");
						return (struct parser_node *)0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression_statement.\n");
					return (struct parser_node *)0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression_statement.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR.\n");
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected WHILE DO or FOR.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * jump_statement(struct parser_state * state){
	struct parser_node * n1;
	unsigned int identifier_id;
	parser_progress("Attempting to build jump_statement\n");
	if((n1 = p_accept(GOTO,state))){
		if((n1->next = get_identifier_node(state, &identifier_id))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected IDENTIFIER.\n");
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(CONTINUE,state))){
		if((n1->next= p_accept(SEMICOLON_CHAR, state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(BREAK,state))){
		if((n1->next= p_accept(SEMICOLON_CHAR, state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
			return (struct parser_node *)0;
		}
	}else if((n1 = p_accept(RETURN,state))){
		if((n1->next = p_accept(SEMICOLON_CHAR, state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
		}else if((n1->next = expression(state))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, state))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, JUMP_STATEMENT);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
				return (struct parser_node *)0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR or expression.\n");
			return (struct parser_node *)0;
		}
	}else{
		parser_progress("Expected jump_statement.\n");
		return (struct parser_node *)0;
	}
}

struct parser_node * function_definition(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	unsigned int declaration_specifiers_list_id;
	unsigned int declarator_item_list_id;
	struct identifier_from_declarator identifier_id;
	identifier_id.initialized = 0; /*  No id obtained yet. */
	parser_progress("Attempting to build function_definition\n");
	if((n1 = declaration_specifiers(state, get_type_engine_id_for_specifier_or_qualifier_list_begin(state->type_engine), &declaration_specifiers_list_id))){
		if((n1->next = declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &declarator_item_list_id, &identifier_id))){
			unsigned int declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_ONLY_DECLARATOR, declarator_item_list_id);
			unsigned int general_type_list_id;
			(void)declarator_id;
			if((n1->next->next = declaration_list(state, (struct namespace_object *)0, get_type_engine_id_for_general_type_list_begin(state->type_engine), &general_type_list_id))){
				if((n1->next->next->next = compound_statement(state, n1->next->next, 0))){
					return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, FUNCTION_DEFINITION);
				}else{
					parser_progress("Expected compound_statement.\n");
					backtrack(state, checkpoint);
					return (struct parser_node *)0;
				}
			}else if((n1->next->next = compound_statement(state, n1->next, 0))){
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
	}else if((n1 = declarator(state, get_type_engine_id_for_declarator_part_list_begin(state->type_engine), &declarator_item_list_id, &identifier_id))){
		unsigned int general_type_list_id;
		unsigned int declarator_id = get_type_engine_id_for_bitfield_or_declarator(state->type_engine, TYPE_ENGINE_ONLY_DECLARATOR, declarator_item_list_id);
		(void)declarator_id;
		if((n1->next = declaration_list(state, (struct namespace_object *)0, get_type_engine_id_for_general_type_list_begin(state->type_engine), &general_type_list_id))){
			if((n1->next->next = compound_statement(state, n1->next, 0))){
				return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, FUNCTION_DEFINITION);
			}else{
				parser_progress("Expected compound_statement.\n");
				backtrack(state, checkpoint);
				return (struct parser_node *)0;
			}
		}else if((n1->next = compound_statement(state, n1, 0))){
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

struct parser_node * external_declaration(struct parser_state * state){
	struct parser_node * n1;
	unsigned int general_type_list_id;
	parser_progress("Attempting to build external_declaration\n");
	if((n1 = declaration(state, get_type_engine_id_for_general_type_list_begin(state->type_engine), &general_type_list_id))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXTERNAL_DECLARATION);
	}else if((n1 = function_definition(state))){
		return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, EXTERNAL_DECLARATION);
	}else{
		parser_progress("Expected external declaration.\n");
		return (struct parser_node *)0;
	}
}

struct normalized_declaration_element * get_most_recently_added_declaration_element_from_object(struct namespace_object * o){
	return struct_normalized_declaration_element_ptr_list_get(&o->elements, struct_normalized_declaration_element_ptr_list_size(&o->elements) -1);
}

struct parser_node * translation_unit_rest(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build translation_unit_rest\n");
	if((n1 = external_declaration(state))){
		struct first_and_last_namespace_object fl = manage_generic_declaration(state, n1, (struct normalized_declaration_set *)0, 0, GLOBAL, (struct namespace_object *)0, (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)0);
		if(fl.first && get_most_recently_added_declaration_element_from_object(fl.first)->normalized_declaration_set->set->type == FUNCTION_DEFINITION){
			struct scope_level * scope = get_last_function_namespace_scope(state);
			struct current_function_change * change = (struct current_function_change *)malloc(sizeof(struct current_function_change));
			change->target = scope;
			change->old_obj = scope->current_function;
			change->new_obj = fl.first;
			push_operation(state, SET_CURRENT_FUNCTION, (void*)change);
		}
		if((n1->next = translation_unit_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TRANSLATION_UNIT_REST);
		}else{
			parser_progress("Expected translation_unit_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		struct parser_node * n = make_epsilon(state);
		return create_parser_node(state, (struct parser_node *)0, n, (struct c_lexer_token*)0, TRANSLATION_UNIT_REST);
	}
}

struct parser_node * translation_unit(struct parser_state * state){
	struct parser_node * n1;
	struct parser_checkpoint checkpoint = create_parser_checkpoint(state);
	parser_progress("Attempting to build translation_unit\n");
	if((n1 = external_declaration(state))){
		struct first_and_last_namespace_object fl = manage_generic_declaration(state, n1, (struct normalized_declaration_set *)0, 0, GLOBAL, (struct namespace_object *)0, (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)0);
		if(fl.first && get_most_recently_added_declaration_element_from_object(fl.first)->normalized_declaration_set->set->type == FUNCTION_DEFINITION){
			struct scope_level * scope = get_last_function_namespace_scope(state);
			struct current_function_change * change = (struct current_function_change *)malloc(sizeof(struct current_function_change));
			change->target = scope;
			change->old_obj = scope->current_function;
			change->new_obj = fl.first;
			push_operation(state, SET_CURRENT_FUNCTION, (void*)change);
		}
		if((n1->next = translation_unit_rest(state))){
			return create_parser_node(state, (struct parser_node *)0, n1, (struct c_lexer_token*)0, TRANSLATION_UNIT);
		}else{
			parser_progress("Expected translation_unit_rest.\n");
			backtrack(state, checkpoint);
			return (struct parser_node *)0;
		}
	}else{
		printf("Parser could not continue on line %d of file %s\n", state->line_number, state->c_lexer_state->c.filename);
		assert(0 && "Expected external declaration.\n");
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
	unsigned int i;
	unsigned int j;

	buffered_puts(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_puts(state->buffered_output,"-");
	buffered_puts(state->buffered_output,">BEGIN scope details\n");
	buffered_puts(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_puts(state->buffered_output,"-");
	buffered_puts(state->buffered_output,">Identifiers:\n");
	/*
	for(i = 0; i < struct_namespace_object_ptr_list_size(&scope->identifier_namespace); i++){
		struct namespace_object * namespace_object = struct_namespace_object_ptr_list_get(&scope->identifier_namespace, i);
		unsigned int k;
		for(k = 0; k < struct_normalized_declaration_element_ptr_list_size(&namespace_object->elements); k++){
			struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&namespace_object->elements, k);
			print_normalized_declaration_element(state->memory_pool_collection, state->buffered_output, element, 0);
			buffered_puts(state->buffered_output,"\n");
		}
	}
	*/
	buffered_puts(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_puts(state->buffered_output,"-");

	buffered_puts(state->buffered_output,">Tags:\n");
	/*
	for(i = 0; i < struct_namespace_object_ptr_list_size(&scope->tag_namespace); i++){
		struct namespace_object * namespace_object = struct_namespace_object_ptr_list_get(&scope->tag_namespace, i);
		unsigned int k;
		for(k = 0; k < struct_normalized_declaration_element_ptr_list_size(&namespace_object->elements); k++){
			struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&namespace_object->elements, k);
			print_normalized_declaration_element(state->memory_pool_collection, state->buffered_output, element, 0);
			buffered_puts(state->buffered_output,"\n");
		}
	}
	*/
		
	for(i = 0; i < struct_scope_level_ptr_list_size(&scope->scopes); i++){
		buffered_puts(state->buffered_output,";");
		for(j = 0; j < level; j++)
			buffered_puts(state->buffered_output,"-");
		buffered_printf(state->buffered_output,">%dth sub-scope of level %d:\n", i, level);
		print_scope_level(state, struct_scope_level_ptr_list_get(&scope->scopes, i), level + 1);
	}
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

enum normalized_specifier_type get_normalized_specifier_type(enum c_token_type t){
	switch(t){
		case VOID:{ return NORMALIZED_TYPE_SPECIFIER; }
		case CHAR:{ return NORMALIZED_TYPE_SPECIFIER; }
		case SHORT:{ return NORMALIZED_TYPE_SPECIFIER; }
		case INT:{ return NORMALIZED_TYPE_SPECIFIER; }
		case LONG:{ return NORMALIZED_TYPE_SPECIFIER; }
		case FLOAT:{ return NORMALIZED_TYPE_SPECIFIER; }
		case DOUBLE:{ return NORMALIZED_TYPE_SPECIFIER; }
		case SIGNED:{ return NORMALIZED_TYPE_SPECIFIER; }
		case UNSIGNED:{ return NORMALIZED_TYPE_SPECIFIER; }
		case TYPEDEF:{ return NORMALIZED_STORAGE_CLASS_SPECIFIER; }
		case EXTERN:{ return NORMALIZED_STORAGE_CLASS_SPECIFIER; }
		case STATIC:{ return NORMALIZED_STORAGE_CLASS_SPECIFIER; }
		case AUTO:{ return NORMALIZED_STORAGE_CLASS_SPECIFIER; }
		case CONST:{ return NORMALIZED_TYPE_QUALIFIER; }
		case VOLATILE:{ return NORMALIZED_TYPE_QUALIFIER; }
		default:{ assert(0 && "Not possible."); return (enum normalized_specifier_type)0; }
	}
}

struct type_description_reference create_empty_type_description(struct memory_pool_collection * m){
	struct type_description_reference new_type;
	struct struct_normalized_specifier_ptr_list * specifiers = (struct struct_normalized_specifier_ptr_list *)malloc(sizeof(struct struct_normalized_specifier_ptr_list));
	new_type.t = struct_type_description_memory_pool_malloc(m->struct_type_description_pool);
	struct_normalized_specifier_ptr_list_create(specifiers);
	new_type.t->specifiers = specifiers;
	new_type.t->declarator = (struct normalized_declarator *)0;
	return new_type;
}

struct type_description_reference copy_type_description(struct memory_pool_collection * m, struct type_description_reference type){
	return create_type_description_from_normalized_declarator_and_specifiers(m, type.t->declarator, type.t->specifiers, type.t->context, type.t->source_scope_level, type.t->value_type, type.t->source_element);
}

unsigned int parse(struct parser_state * state){
	unsigned int print_scope_active = 0;
	state->top_node = translation_unit(state);

	if(state->tokens_position != struct_c_lexer_token_ptr_list_size(&state->c_lexer_state->tokens)){
		add_parser_error(state, PARSER_ERROR_INCOMPLETE_PARSE);
	}

	if(!state->top_node){
		add_parser_error(state, PARSER_ERROR_PARSING_FAILED);
	}

	if(print_scope_active){
		print_scope_level(state, state->type_engine->top_scope, 0);
	}
	return 0;
}

void create_parser_state(struct parser_state * state, struct memory_pool_collection * memory_pool_collection, struct c_lexer_state * c_lexer_state, struct unsigned_char_list * buffered_output, unsigned char * buffer, struct type_engine_state * type_engine){
	state->type_engine = type_engine;
	state->buffered_output = buffered_output;
	state->memory_pool_collection = memory_pool_collection;
	state->c_lexer_state = c_lexer_state;
	state->line_number = 1;
	state->tokens_position = 0;
	struct_parser_operation_list_create(&state->operation_stack);
	state->buff = buffer;
	unsigned_char_ptr_to_struct_constant_description_ptr_map_create(&state->constant_map, unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_compare);
	struct_parser_error_list_create(&state->parser_errors);
}

void destroy_parser_state(struct parser_state * state){
	/* Undo every operation that was used to build up the entire parser state */
	backtrack_parser(state, 0);
	struct_parser_operation_list_destroy(&state->operation_stack);
	unsigned_char_ptr_to_struct_constant_description_ptr_map_destroy(&state->constant_map);
	struct_parser_error_list_destroy(&state->parser_errors);
}

const char ** get_node_type_names(void){
	return (const char **)node_type_names;
}

unsigned int is_const(struct type_description_reference t){
	return !(count_specifiers(t, CONST));
}

unsigned int is_signed(struct type_description_reference t){
	return !(count_specifiers(t, UNSIGNED));
}

struct type_description_reference ensure_signed(struct memory_pool_collection * m, struct type_description_reference t){
	/*  Make sure that this type has the specifiers require for it to be signed.
	Note that 'char' signedness is compiler defined.  We will define it to be signed.
	This function assumes it is passed an integer based type (char, short, int, long,
	long long) with a valid list of specifiers.  Undefined behaviour if passed floats etc.
	*/
	while(count_specifiers(t, UNSIGNED)){
		remove_specifier(m, t, 0, UNSIGNED);
	}
	return t;
}

struct type_description_reference ensure_unsigned(struct memory_pool_collection * m, struct type_description_reference t){
	/*  Make sure that this type has the specifiers require for it to be unsigned.
	Note that 'char' signedness is compiler defined.  We will define it to be signed.
	This function assumes it is passed an integer based type (char, short, int, long,
	long long) with a valid list of specifiers.  Undefined behaviour if passed floats etc.
	*/
	while(count_specifiers(t, SIGNED)){
		remove_specifier(m, t, 0, SIGNED);
	}
	return add_specifier(m, t, UNSIGNED);
}

struct type_description_reference add_specifier(struct memory_pool_collection * m, struct type_description_reference description, enum c_token_type t){
	unsigned char * string = get_specifier_string(t);
	struct c_lexer_token * new_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
	struct parser_node * terminal = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct parser_node * specifier = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct normalized_specifier * normalized_specifier = (struct normalized_specifier *)malloc(sizeof(struct normalized_specifier));

	new_token->first_byte = string;
	new_token->last_byte = string + (strlen((char*)string) -1);
	new_token->type = t;

	terminal->c_lexer_token = new_token;
	terminal->first_child = (struct parser_node *)0;
	terminal->next = (struct parser_node *)0;
	terminal->type = TERMINAL;

	specifier->first_child = terminal;
	specifier->c_lexer_token = (struct c_lexer_token *)0;
	specifier->next = (struct parser_node *)0;
	specifier->type = get_specifier_parser_node_type(t);

	normalized_specifier->specifier = specifier;
	normalized_specifier->type = get_normalized_specifier_type(t);
	struct_normalized_specifier_ptr_list_add_end(description.t->specifiers, normalized_specifier);
	return description;
}

void remove_enum(struct memory_pool_collection * m, struct type_description_reference description){
	unsigned int k;
	for(k = 0; k < struct_normalized_specifier_ptr_list_size(description.t->specifiers); k++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(description.t->specifiers, k);
		if(normalized_specifier->specifier->type == TYPE_SPECIFIER && normalized_specifier->specifier->first_child->type == ENUM_SPECIFIER){
			normalized_specifier->specifier->next = (struct parser_node *)0;
			destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_specifier->specifier);
			free(normalized_specifier);
			struct_normalized_specifier_ptr_list_remove_all(description.t->specifiers, normalized_specifier, normalized_specifier_ptr_cmp);
			return;
		}
	}
	assert(0 && "Did not find enum specifier.");
}

enum type_class determine_type_class(struct memory_pool_collection * m, struct type_description_reference t){
	/*  Be as specific as possible about the 'type' of something without differentiating between
	    different user defined enums, unions or structs */
	unsigned int num_char = count_specifiers(t, CHAR);
	unsigned int num_short = count_specifiers(t, SHORT);
	unsigned int num_int = count_specifiers(t, INT);
	unsigned int num_long = count_specifiers(t, LONG);
	unsigned int num_float = count_specifiers(t, FLOAT);
	unsigned int num_double = count_specifiers(t, DOUBLE);
	unsigned int num_void = count_specifiers(t, VOID);
	unsigned int num_unsigned = count_specifiers(t, UNSIGNED);
	struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(t.t->specifiers);
	struct parser_node * temp = create_abstract_declarator_from_normalized_declarator(m, t.t->declarator);
	if(is_pointer(m, temp)){
		destroy_parser_node_tree_and_c_lexer_tokens(m, temp);
		return TYPE_CLASS_POINTER;
	}
	if(is_array(m, temp)){
		destroy_parser_node_tree_and_c_lexer_tokens(m, temp);
		return TYPE_CLASS_ARRAY;
	}
	if(is_function(m, temp)){
		destroy_parser_node_tree_and_c_lexer_tokens(m, temp);
		return TYPE_CLASS_FUNCTION;
	}
	destroy_parser_node_tree_and_c_lexer_tokens(m, temp);
	if(struct_or_union_or_enum_specifier){
		if(is_struct(struct_or_union_or_enum_specifier)){
			return TYPE_CLASS_STRUCT;
		}else if(is_union(struct_or_union_or_enum_specifier)){
			return TYPE_CLASS_UNION;
		}else if(is_enum(struct_or_union_or_enum_specifier)){
			return TYPE_CLASS_ENUM;
		}else{
			assert(0 && "Not expected.");
		}
	}

	if(!num_char && !num_short && !num_int && !num_long && !num_float && !num_double && num_void == 1){
		return TYPE_CLASS_VOID;
	}else if(num_char == 1 && !num_short && !num_int && !num_long && !num_float && !num_double && !num_void){
		return TYPE_CLASS_CHAR;
	}else if(!num_char && num_short == 1 && !num_int && !num_long && !num_float && !num_double && !num_void){
		return TYPE_CLASS_SHORT;
	}else if(!num_char && num_short == 1 && num_int == 1 && !num_long && !num_float && !num_double && !num_void){
		return TYPE_CLASS_SHORT;
	}else if(!num_char && !num_short && num_int == 1 && !num_long && !num_float && !num_double && !num_void){
		return TYPE_CLASS_INT;
	}else if(!num_char && !num_short && !num_int && num_long == 1 && !num_float && !num_double && !num_void){
		return TYPE_CLASS_LONG;
	}else if(!num_char && !num_short && num_int == 1 && num_long == 1 && !num_float && !num_double && !num_void){
		return TYPE_CLASS_LONG;
	}else if(!num_char && !num_short && !num_int && num_long == 2 && !num_float && !num_double && !num_void){
		return TYPE_CLASS_LONG_LONG;
	}else if(!num_char && !num_short && num_int == 1 && num_long == 2 && !num_float && !num_double && !num_void){
		return TYPE_CLASS_LONG_LONG;
	}else if(!num_char && !num_short && !num_int && !num_long && num_float == 1 && !num_double && !num_void){
		return TYPE_CLASS_FLOAT;
	}else if(!num_char && !num_short && !num_int && !num_long && !num_float && num_double == 1 && !num_void){
		return TYPE_CLASS_DOUBLE;
	}else if(!num_char && !num_short && !num_int && num_long == 1 && !num_float && num_double == 1 && !num_void){
		return TYPE_CLASS_LONG_DOUBLE;
	}else if(num_unsigned == 1 && !num_char && !num_short && !num_int && !num_long && !num_float && !num_double && !num_void){
		return TYPE_CLASS_INT; /*  This case is sneaky */
	}else{
		struct unsigned_char_list l;
		unsigned_char_list_create(&l);
		buffered_puts(&l,"Type:\n");
		print_normalized_declaration_declarator_and_specifiers(m, &l, t.t->declarator, t.t->specifiers, (struct parser_node *)0, 0);
		unsigned_char_list_add_end(&l, 0);
		printf("%s\n", unsigned_char_list_data(&l));
		assert(0 && "Unrecognized primative.");
	}
}



unsigned int get_ceil_modulo(unsigned int n, unsigned int modulo){
	unsigned int quotient = n / modulo;
	unsigned int remainder = n - (quotient * modulo);
	return modulo * (quotient + (remainder ? 1 : 0));
}

unsigned int arithmetic_type_size(struct memory_pool_collection * m, struct type_description_reference t, enum value_type tsc){
	enum type_class type_class = determine_type_class(m, t);
	unsigned int data_bits;

	switch(type_class){
		case TYPE_CLASS_CHAR:{
			data_bits = 8;
			break;
		}case TYPE_CLASS_SHORT:{
			data_bits = 16;
			break;
		}case TYPE_CLASS_INT:{
			data_bits = 32;
			break;
		}case TYPE_CLASS_LONG:{
			data_bits = 64;
			break;
		}case TYPE_CLASS_LONG_LONG:{
			data_bits = 64;
			break;
		}case TYPE_CLASS_FLOAT:{
			data_bits = 32;
			break;
		}case TYPE_CLASS_DOUBLE:{
			data_bits = 64;
			break;
		}case TYPE_CLASS_LONG_DOUBLE:{
			data_bits = 128;
			break;
		}default:{
			struct unsigned_char_list l;
			unsigned_char_list_create(&l);
			print_normalized_declaration_declarator_and_specifiers(m, &l, t.t->declarator, t.t->specifiers, (struct parser_node *)0, 0);
			unsigned_char_list_add_end(&l, 0);
			printf("%s\n", unsigned_char_list_data(&l));
			assert(0 && "Unknown arithmetic type.");
		}
	}

	switch (tsc){
		case WORD_ALIGNED_RVALUE:{
			return get_ceil_modulo(data_bits, 32) / 8;
		}case MINIMAL_RVALUE:{
			return data_bits / 8;
		}case LVALUE:{
			return 4;
		}default:{
			assert(0 && "Unknown type class size.");
			return 0;
		}
	}
}

unsigned int void_type_size(struct memory_pool_collection * m, enum value_type tsc){
	/*  Void isn't supposed to have a size, but for now it gets returned from void 
            functions as if were and int.  This makes certain things easier.  */
	(void)m;
	switch (tsc){
		case WORD_ALIGNED_RVALUE:{
			return 4;
		}case MINIMAL_RVALUE:{
			return 4;
		}case LVALUE:{
			return 4;
		}default:{
			assert(0 && "Unknown type class size.");
			return 0;
		}
	}
}

unsigned int enum_type_size(struct memory_pool_collection * m, struct type_description_reference t, enum value_type tsc){
	(void)m;
	(void)t;
	switch (tsc){
		case WORD_ALIGNED_RVALUE:{
			return 4;
		}case MINIMAL_RVALUE:{
			return 4;
		}case LVALUE:{
			return 4;
		}default:{
			assert(0 && "Unknown type class size.");
			return 0;
		}
	}
}

unsigned int pointer_type_size(struct memory_pool_collection * m, struct type_description_reference t, enum value_type tsc){
	(void)t;
	(void)m;
	switch (tsc){
		case WORD_ALIGNED_RVALUE:{
			return 4;
		}case MINIMAL_RVALUE:{
			return 4;
		}case LVALUE:{
			return 4;
		}default:{
			assert(0 && "Unknown type class size.");
			return 0;
		}
	}
}

unsigned int is_integral_type(struct memory_pool_collection * m, struct type_description_reference t){
	enum type_class c = determine_type_class(m, t);
	return (
		c == TYPE_CLASS_CHAR ||
		c == TYPE_CLASS_SHORT ||
		c == TYPE_CLASS_INT ||
		c == TYPE_CLASS_LONG ||
		c == TYPE_CLASS_LONG_LONG ||
		c == TYPE_CLASS_ENUM
	);
}

unsigned int is_floating_type(struct memory_pool_collection * m, struct type_description_reference t){
	enum type_class c = determine_type_class(m, t);
	return (
		c == TYPE_CLASS_DOUBLE ||
		c == TYPE_CLASS_LONG_DOUBLE ||
		c == TYPE_CLASS_ENUM
	);
}

unsigned int is_arithmetic_type(struct memory_pool_collection * m, struct type_description_reference t){
	return is_floating_type(m, t) || is_integral_type(m, t);
}

unsigned int is_scalar_type(struct memory_pool_collection * m, struct type_description_reference t){
	enum type_class c = determine_type_class(m, t);
	return is_arithmetic_type(m, t) || c == TYPE_CLASS_POINTER;
}
