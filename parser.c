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
#include "parser.h"

const char * g_extern_string = "extern";
const char * g_static_string = "static";
const char * g_auto_string = "auto";
const char * g_typedef_string = "typedef";
const char * g_volatile_string = "volatile";
const char * g_void_string = "void";
const char * g_char_string = "char";
const char * g_short_string = "short";
const char * g_int_string = "int";
const char * g_long_string = "long";
const char * g_float_string = "float";
const char * g_double_string = "double";
const char * g_register_string = "register";
const char * g_const_string = "const";
const char * g_signed_string = "signed";
const char * g_unsigned_string = "unsigned";
const char * g_ptr_string = "*";
const char * g_open_paren_string = "(";
const char * g_close_paren_string = ")";

struct memory_pooler * g_parser_node_pool;
struct memory_pooler * g_c_lexer_token_pool;

const char * node_type_names[] = {
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
struct parser_node * expression(struct parser_state *);
struct parser_node * expression_rest(struct parser_state *);
struct parser_node * declarator(struct parser_state *);
struct parser_node * initializer(struct parser_state *);
struct parser_node * struct_or_union_specifier(struct parser_state *);
struct parser_node * type_name(struct parser_state *);
struct parser_node * abstract_declarator(struct parser_state *);
struct parser_node * init_declarator(struct parser_state *);
struct parser_node * parameter_type_list(struct parser_state *);
struct parser_node * labeled_statement(struct parser_state *);
struct parser_node * compound_statement(struct parser_state *, struct parser_node *);
struct parser_node * expression_statement(struct parser_state *);
struct parser_node * selection_statement(struct parser_state *);
struct parser_node * iteration_statement(struct parser_state *);
struct parser_node * jump_statement(struct parser_state *);
struct parser_node * assignment_expression(struct parser_state *);
struct parser_node * cast_expression(struct parser_state *);
struct parser_node * p_accept(enum c_token_type, struct parser_state *);
struct parser_node * expect(enum c_token_type, struct parser_state *);
struct parser_node * primary_expression(struct parser_state *);
struct parser_node * make_epsilon(struct parser_state *);
struct parser_node * argument_expression_list_rest(struct parser_state *);
struct parser_node * argument_expression_list(struct parser_state *);
struct parser_node * postfix_expression_rest(struct parser_state *);
struct parser_node * postfix_expression(struct parser_state *);
struct parser_node * unary_operator(struct parser_state *);
struct parser_node * unary_expression(struct parser_state *);
struct parser_node * multiplicative_expression_rest(struct parser_state *);
struct parser_node * multiplicative_expression(struct parser_state *);
struct parser_node * additive_expression_rest(struct parser_state *);
struct parser_node * additive_expression(struct parser_state *);
struct parser_node * shift_expression_rest(struct parser_state *);
struct parser_node * shift_expression(struct parser_state *);
struct parser_node * relational_expression_rest(struct parser_state *);
struct parser_node * relational_expression(struct parser_state *);
struct parser_node * equality_expression_rest(struct parser_state *);
struct parser_node * equality_expression(struct parser_state *);
struct parser_node * and_expression_rest(struct parser_state *);
struct parser_node * and_expression(struct parser_state *);
struct parser_node * exclusive_or_expression_rest(struct parser_state *);
struct parser_node * exclusive_or_expression(struct parser_state *);
struct parser_node * inclusive_or_expression_rest(struct parser_state *);
struct parser_node * inclusive_or_expression(struct parser_state *);
struct parser_node * logical_and_expression_rest(struct parser_state *);
struct parser_node * logical_and_expression(struct parser_state *);
struct parser_node * logical_or_expression_rest(struct parser_state *);
struct parser_node * logical_or_expression(struct parser_state *);
struct parser_node * conditional_expression(struct parser_state *);
struct parser_node * assignment_operator(struct parser_state *);
struct parser_node * constant_expression(struct parser_state *);
struct parser_node * expression_statement(struct parser_state *);
struct parser_node * statement(struct parser_state *);
struct parser_node * statement_list_rest(struct parser_state *);
struct parser_node * statement_list(struct parser_state *);
struct parser_node * type_qualifier(struct parser_state *);
struct parser_node * storage_class_specifier(struct parser_state *);
struct parser_node * enumerator(struct parser_state *);
struct parser_node * enumerator_list_rest(struct parser_state *);
struct parser_node * enumerator_list(struct parser_state *);
struct parser_node * enum_specifier(struct parser_state *);
struct parser_node * struct_or_union(struct parser_state *);
struct parser_node * type_specifier(struct parser_state *);
struct parser_node * declaration_specifiers(struct parser_state *);
struct parser_node * init_declarator_list_rest(struct parser_state *);
struct parser_node * init_declarator_list(struct parser_state *);
struct parser_node * declaration(struct parser_state *);
struct parser_node * declaration_list_rest(struct parser_state *, struct namespace_object *, struct first_and_last_namespace_object *);
struct parser_node * declaration_list(struct parser_state *, struct namespace_object *);
struct parser_node * identifier_list_rest(struct parser_state *);
struct parser_node * identifier_list(struct parser_state *);
struct parser_node * direct_declarator_rest(struct parser_state *);
struct parser_node * type_qualifier_list_rest(struct parser_state *);
struct parser_node * type_qualifier_list(struct parser_state *);
struct parser_node * pointer(struct parser_state *);
struct parser_node * direct_declarator(struct parser_state *);
struct parser_node * initializer_list_rest(struct parser_state *);
struct parser_node * initializer_list(struct parser_state *);
struct parser_node * struct_declarator(struct parser_state *);
struct parser_node * struct_declarator_list_rest(struct parser_state *);
struct parser_node * struct_declarator_list(struct parser_state *);
struct parser_node * specifier_qualifier_list(struct parser_state *);
struct parser_node * struct_declaration(struct parser_state *);
struct parser_node * struct_declaration_list_rest(struct parser_state *);
struct parser_node * struct_declaration_list(struct parser_state *);
struct parser_node * struct_or_union_specifier(struct parser_state *);
struct parser_node * parameter_declaration(struct parser_state *);
struct parser_node * parameter_list_rest(struct parser_state *);
struct parser_node * parameter_list(struct parser_state *);
struct parser_node * direct_abstract_declarator_rest(struct parser_state *);
struct parser_node * direct_abstract_declarator(struct parser_state *);
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
void manage_new_scope(struct scope_level *, unsigned int, enum add_or_remove);
void destroy_empty_scope(struct scope_level *);
void remove_scope(struct scope_level *);
void add_scope(struct scope_level *);
void print_scope_level(struct parser_state *, struct scope_level *, unsigned int);
void destroy_scope_level(struct scope_level *, unsigned int);
struct scope_level * create_empty_scope(struct scope_level *);
void add_terminal_tokens_to_buffer_h2(struct unsigned_char_list *, struct parser_node *, unsigned int *);
void add_terminal_tokens_to_buffer_h1(struct unsigned_char_list *, struct parser_node *, unsigned int *);
void add_terminal_tokens_to_buffer(struct unsigned_char_list *, struct parser_node *);
void descend_scope(struct scope_level **, unsigned int);
void manage_constant(struct parser_state *, struct parser_node *, enum add_or_remove);

void add_normalized_specifier_to_list(struct parser_node *, struct struct_normalized_specifier_ptr_list *);
void add_normalized_declarator_to_list(struct parser_node *, struct struct_normalized_declarator_ptr_list *);
struct struct_normalized_specifier_ptr_list * get_normalized_specifier_list(struct parser_node *);
struct struct_normalized_declarator_ptr_list * get_normalized_declarator_list(struct parser_node *);
void print_generic_declaration(struct unsigned_char_list *, struct parser_node * n, unsigned int);
void print_parser_node_tree(struct unsigned_char_list *, struct parser_node *, struct parser_node *, unsigned int);
struct first_and_last_namespace_object manage_generic_declaration(struct parser_state *, struct parser_node * n, struct normalized_declaration_set *, unsigned int, enum object_location, struct namespace_object *, struct struct_namespace_object_ptr_list *);
int is_anonymous_struct_or_union_or_enum(struct parser_node *);
int is_struct_or_union_or_enum_definition(struct parser_node *);
int is_bitfield(struct parser_node *);
struct parser_node * get_function_definition(struct normalized_declaration_set *);
struct parser_node * create_abstract_declarator_from_declarator(struct parser_node *);
struct parser_node * convert_declarator_to_abstract_declarator(struct parser_node *);
struct parser_node * convert_declarator_to_abstract_declarator_h(struct parser_node *);
unsigned char * convert_character_constant(unsigned char *, unsigned char *);
unsigned char * convert_string_literal(unsigned char *);
int is_function_variadic(struct parser_node *);
int is_function_k_and_r_c_style(struct parser_node *);
int is_parameter_type_list_variadic(struct parser_node *);
struct parser_node * add_level_to_pointer(struct parser_node * n);
struct parser_node * create_pointer_node(void);
struct parser_node * convert_abstract_declarator_to_address_type_h(struct parser_node *);
struct parser_node * convert_abstract_declarator_to_address_type(struct parser_node *);
struct parser_node * convert_abstract_declarator_to_pointer_type(struct parser_node *);
struct parser_node * convert_abstract_declarator_to_array_type(struct parser_node *);
struct parser_node * remove_one_pointer_level(struct parser_node *);
struct c_lexer_token * get_identifier_token_from_declarator(struct parser_node *);
struct parser_node * get_parameter_type_list_from_abstract_declarator(struct parser_node *);
struct parser_node * get_nth_parameter_declaration_from_parameter_type_list(struct parser_node *, unsigned int);
struct first_and_last_namespace_object manage_declaration_elements(struct parser_state *, struct struct_normalized_declaration_element_ptr_list *, enum object_location, struct namespace_object *, struct struct_namespace_object_ptr_list *);
struct namespace_object * manage_declaration_element(struct parser_state *, struct normalized_declaration_element *, enum scope_type, enum object_location, struct struct_namespace_object_ptr_list *);
void print_normalized_specifier(struct unsigned_char_list *, struct struct_normalized_specifier_ptr_list *);
struct namespace_modification * create_namespace_modification(struct scope_level *, struct struct_namespace_object_ptr_list *, struct normalized_declaration_element *, enum object_location, struct namespace_object *);
struct namespace_object * do_namespace_modification(struct namespace_modification *);
void undo_namespace_modification(struct namespace_modification *);
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
unsigned int is_const(struct type_description * t);
struct type_description * ensure_signed(struct type_description * t);
struct type_description * ensure_unsigned(struct type_description * t);
void validate_specifier_token_type(enum c_token_type);
enum normalized_specifier_type get_normalized_specifier_type(enum c_token_type);
enum node_type get_specifier_parser_node_type(enum c_token_type);
unsigned int get_token_index(struct c_lexer_state *, struct c_lexer_token *);
unsigned int get_token_line_number(struct c_lexer_state *, struct c_lexer_token *);
unsigned char * get_specifier_string(enum c_token_type);
void pretty_print_context(struct c_lexer_state *, struct c_lexer_token *, struct c_lexer_token *, struct c_lexer_token *, struct c_lexer_token *);
struct parser_node * get_last_terminal_node(struct parser_node *);
void declare_enum_constants(struct parser_state *, struct parser_node *, struct normalized_declaration_set *);

unsigned int get_token_line_number(struct c_lexer_state * state, struct c_lexer_token * t){
	/* Number of newlines up to and including the given token */
	unsigned int line = 1;
	unsigned int i;
	for(i = 0; i < struct_c_lexer_token_ptr_list_size(&state->tokens); i++){
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i)->type == NEWLINE){
			line++;
		}
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i) == t){
			return line;
		}
	}
	assert("Should have found token.");
	return 0;
}

unsigned int get_token_index(struct c_lexer_state * state, struct c_lexer_token * t){
	unsigned int i;
	for(i = 0; i < struct_c_lexer_token_ptr_list_size(&state->tokens); i++){
		if(struct_c_lexer_token_ptr_list_get(&state->tokens, i) == t){
			return i;
		}
	}
	assert("Should have found token.");
	return 0;
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
		return 0;
	}
}

void print_node_context(struct c_lexer_state * state, struct parser_node * n){
	struct parser_node * first_terminal = n;
	struct parser_node * last_terminal = n->type == TERMINAL ? n : n->first_child;
	unsigned int lines_context = 3; /*  Print this many lines before and after */
	unsigned int newlines_encountered_before = 0;
	unsigned int newlines_encountered_after = 0;
	unsigned int start_token_index;
	unsigned char * first_byte;
	unsigned char * last_byte;
	struct c_lexer_token * start_context_token;
	struct c_lexer_token * end_context_token = struct_c_lexer_token_ptr_list_get(&state->tokens, struct_c_lexer_token_ptr_list_size(&state->tokens) -1);
	unsigned int i;
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
	start_token_index = get_token_index(state, first_terminal->c_lexer_token);

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
	unsigned int start_line = get_token_line_number(state, start_context_token);
	unsigned int first_token_index = get_token_index(state, start_context_token);
	unsigned int last_token_index = get_token_index(state, end_context_token);
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
	assert(state->top_scope->scopes && "There should be at least one sub scope at this point.");
	return state->top_scope->scopes[state->top_scope->num_sub_scopes - 1];
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


int is_type_description_a_function(struct type_description * t){
	struct parser_node * temp = create_abstract_declarator_from_normalized_declarator(t->declarator);
	int rtn = is_function(temp);
	destroy_parser_node_tree_and_c_lexer_tokens(temp);
	return rtn;
}

int is_type_description_a_function_pointer(struct type_description * t){
	struct parser_node * temp = create_abstract_declarator_from_normalized_declarator(t->declarator);
	int rtn;
	while(is_pointer(temp)){
		temp = convert_abstract_declarator_to_pointer_type(temp);
	}
	rtn = is_function(temp);
	destroy_parser_node_tree_and_c_lexer_tokens(temp);
	return rtn;
}

struct type_description * create_type_description_from_type_name(struct parser_node * n){
	struct type_description * rtn = malloc(sizeof(struct type_description));
	struct normalized_declarator * normalized_declarator = malloc(sizeof(struct normalized_declarator));
	assert(n->type == TYPE_NAME);
	rtn->specifiers = get_normalized_specifier_list(n->first_child);
	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	if(n->first_child->next && n->first_child->next->type == ABSTRACT_DECLARATOR){
		normalized_declarator->declarator = copy_parser_node_children_only(n->first_child->next);
	}else{
		normalized_declarator->declarator = 0;
	}
	rtn->declarator = normalized_declarator;
	return rtn;
}

struct type_description * get_current_function_return_type_description(struct type_description * t){
	/*  Should be a plain function type with no levels of indirection */
	struct type_description * return_type_description = 0;
	struct normalized_declarator * normalized_declarator_return = malloc(sizeof(struct normalized_declarator));

	struct parser_node * abstract = create_abstract_declarator_from_normalized_declarator(t->declarator);
	struct parser_node * fcn_type = convert_abstract_declarator_to_function_type(abstract);
	normalized_declarator_return->declarator = fcn_type;
	normalized_declarator_return->type = NORMALIZED_ABSTRACT_DECLARATOR;
	return_type_description = create_type_description_from_normalized_declarator_and_specifiers(normalized_declarator_return, t->specifiers);
	destroy_parser_node_tree_and_c_lexer_tokens(normalized_declarator_return->declarator);
	free(normalized_declarator_return);
	return_type_description->source_scope_level = t->source_scope_level;
	return return_type_description;
}

struct type_description * create_dereferenced_array_type_description_from_type_description(struct type_description * a){
	/* Take a type description as input and return a type description of the pointer type (do the [] operator on an array or pointer) */
	struct type_description * rtn;
	struct normalized_declarator * normalized_declarator = malloc(sizeof(struct normalized_declarator));
	struct parser_node * temp = 0;
	temp = create_abstract_declarator_from_normalized_declarator(a->declarator);
	if(is_array(temp)){
		normalized_declarator->declarator = convert_abstract_declarator_to_array_type(temp);
	}else{
		/*  If the abstract declarator type is not an array, assume it is a pointer we're using [] to dereference */
		normalized_declarator->declarator = convert_abstract_declarator_to_pointer_type(temp);
	}
	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	rtn = create_type_description_from_normalized_declarator_and_specifiers(normalized_declarator, a->specifiers);
	destroy_parser_node_tree_and_c_lexer_tokens(normalized_declarator->declarator);
	free(normalized_declarator);
	rtn->source_scope_level = a->source_scope_level;
	return rtn;
}

struct type_description * create_dereferenced_pointer_type_description_from_type_description(struct type_description * a){
	/* Take a type description as input and return a type description of the pointer type (do the * (value pointed to by) operator on a pointer) */
	struct type_description * rtn;
	struct normalized_declarator * normalized_declarator = malloc(sizeof(struct normalized_declarator));
	struct parser_node * temp = 0;
	temp = create_abstract_declarator_from_normalized_declarator(a->declarator);
	normalized_declarator->declarator = convert_abstract_declarator_to_pointer_type(temp);
	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	rtn = create_type_description_from_normalized_declarator_and_specifiers(normalized_declarator, a->specifiers);
	destroy_parser_node_tree_and_c_lexer_tokens(normalized_declarator->declarator);
	free(normalized_declarator);
	rtn->source_scope_level = a->source_scope_level;
	return rtn;
}

void convert_to_untypedefed_type_description(struct type_description * t){
	/*  Will resolve any typedefed types int the top level specifiers (but not in deeper places like function parameters.) */
	unsigned int num_specifiers = struct_normalized_specifier_ptr_list_size(t->specifiers);
	unsigned int i;
	for(i = 0; i < num_specifiers; i++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(t->specifiers, i);
		if(normalized_specifier->type == NORMALIZED_TYPE_SPECIFIER && normalized_specifier->specifier->first_child->type == TERMINAL){
			struct parser_node * child = normalized_specifier->specifier->first_child;
			if(child->c_lexer_token->type == IDENTIFIER){
				unsigned char * ident = copy_string(child->c_lexer_token->first_byte, child->c_lexer_token->last_byte);
				struct namespace_object * obj = get_namespace_object_from_closest_namespace(ident, IDENTIFIER_NAMESPACE, t->source_scope_level, 0);
				struct normalized_declaration_element * element;
				struct type_description * typedefed_type;
				element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, 0);
				typedefed_type = create_type_description_from_normalized_declaration_element(element);
				if(obj && count_specifiers(typedefed_type, TYPEDEF)){
					unsigned int num_original_specifiers;
					unsigned int j;
					remove_specifier(t, 0, IDENTIFIER);
					remove_specifier(typedefed_type, 0, TYPEDEF);
					num_original_specifiers = struct_normalized_specifier_ptr_list_size(typedefed_type->specifiers);
					for(j = 0; j < num_original_specifiers; j++){
						struct normalized_specifier * src_normalized_specifier = struct_normalized_specifier_ptr_list_get(typedefed_type->specifiers, j);
						struct normalized_specifier * dst_normalized_specifier = malloc(sizeof(struct normalized_specifier));
						struct parser_node * specifiers_copy = copy_parser_node_children_only(src_normalized_specifier->specifier);
						dst_normalized_specifier->specifier = specifiers_copy;
						dst_normalized_specifier->type = src_normalized_specifier->type;
						struct_normalized_specifier_ptr_list_add(t->specifiers, dst_normalized_specifier);
					}
					destroy_type_description(typedefed_type);
				}else{
					assert(0 && "Unable to resolve typedef.");
				}
				free(ident);
			}
		}/* else, no typedefs to resolve */
	}
}

struct type_description * create_address_type_description_from_type_description(struct type_description * a){
	/* Take a type description as input and return a type description of the address type do the & operator */
	struct type_description * rtn;
	struct normalized_declarator * normalized_declarator = malloc(sizeof(struct normalized_declarator));
	struct parser_node * temp = 0;
	temp = create_abstract_declarator_from_normalized_declarator(a->declarator);
	normalized_declarator->declarator = convert_abstract_declarator_to_address_type(temp);
	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	rtn = create_type_description_from_normalized_declarator_and_specifiers(normalized_declarator, a->specifiers);
	destroy_parser_node_tree_and_c_lexer_tokens(normalized_declarator->declarator);
	free(normalized_declarator);
	rtn->source_scope_level = a->source_scope_level;
	return rtn;
}

int type_description_cmp(struct type_description * a, struct type_description * b){
	/*  Compares the 'type' and ignores storage class specifiers and type qualifiers */

	enum type_class a_class = determine_type_class(a);
	enum type_class b_class = determine_type_class(b);

	if(a_class != b_class){
		return 1;
	}else{
		struct parser_node * abstract_declarator_a = create_abstract_declarator_from_normalized_declarator(a->declarator);
		struct parser_node * abstract_declarator_b = create_abstract_declarator_from_normalized_declarator(b->declarator);
		while(abstract_declarator_a || abstract_declarator_b){

			if(is_pointer(abstract_declarator_a) || is_pointer(abstract_declarator_b)){
				if(is_pointer(abstract_declarator_a) && is_pointer(abstract_declarator_b)){
					abstract_declarator_a = convert_abstract_declarator_to_pointer_type(abstract_declarator_a);
					abstract_declarator_b = convert_abstract_declarator_to_pointer_type(abstract_declarator_b);
				}else{
					destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_a);
					destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_b);
					return 1;
				}
			}

			if(is_function(abstract_declarator_a) || is_function(abstract_declarator_b)){
				if(is_function(abstract_declarator_a) && is_function(abstract_declarator_b)){
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
										
										struct normalized_declaration_set * declaration_set_a = create_normalized_declaration_set_from_parser_node(parameter_declaration_a, 0);
										struct normalized_declaration_set * declaration_set_b = create_normalized_declaration_set_from_parser_node(parameter_declaration_b, 0);
										int rtn;

										struct type_description * param_type_a = malloc(sizeof(struct type_description));
										struct type_description * param_type_b = malloc(sizeof(struct type_description));
										param_type_a->specifiers = declaration_set_a->normalized_specifiers;
										param_type_b->specifiers = declaration_set_b->normalized_specifiers;
										assert(struct_normalized_declarator_ptr_list_size(declaration_set_a->normalized_declarators) <= 1);
										assert(struct_normalized_declarator_ptr_list_size(declaration_set_b->normalized_declarators) <= 1);
										if(struct_normalized_declarator_ptr_list_size(declaration_set_a->normalized_declarators)){
											param_type_a->declarator = struct_normalized_declarator_ptr_list_get(declaration_set_a->normalized_declarators, 0);
										}else{
											param_type_a->declarator = 0;
										}
										if(struct_normalized_declarator_ptr_list_size(declaration_set_b->normalized_declarators)){
											param_type_b->declarator = struct_normalized_declarator_ptr_list_get(declaration_set_b->normalized_declarators, 0);
										}else{
											param_type_b->declarator = 0;
										}
										rtn = type_description_cmp(param_type_a, param_type_b);
										destroy_normalized_declaration_element_list(create_normalized_declaration_element_list(declaration_set_a));
										destroy_normalized_declaration_element_list(create_normalized_declaration_element_list(declaration_set_b));
										free(param_type_a);
										free(param_type_b);
										arg_index++;
										if(rtn){
											return rtn;
										}
									}else{
										destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_a);
										destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_b);
										return 1;
									}
								}
							}while(parameter_declaration_a || parameter_declaration_b);
						}else{
							destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_a);
							destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_b);
							return 1;
						}
					}
					abstract_declarator_a = convert_abstract_declarator_to_function_type(abstract_declarator_a);
					abstract_declarator_b = convert_abstract_declarator_to_function_type(abstract_declarator_b);
				}else{
					destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_a);
					destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_b);
					return 1;
				}
			}

			if(is_array(abstract_declarator_a) || is_array(abstract_declarator_b)){
				if(is_array(abstract_declarator_a) && is_array(abstract_declarator_b)){
					abstract_declarator_a = convert_abstract_declarator_to_array_type(abstract_declarator_a);
					abstract_declarator_b = convert_abstract_declarator_to_array_type(abstract_declarator_b);
				}else{
					destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_a);
					destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator_b);
					return 1;
				}
			}
		}
	}
	return 0;
}

struct type_description * create_type_description_from_normalized_declarator_and_specifiers(struct normalized_declarator * normalized_declarator, struct struct_normalized_specifier_ptr_list * specifiers){
	/* Take a normalized declarator and specifiers, change any declarator it has into an abstract one, and copy all parser nodes */
	/* Make copies of everything so we can manage memory more consistently */
	struct struct_normalized_specifier_ptr_list * normalized_specifiers_copy = malloc(sizeof(struct struct_normalized_specifier_ptr_list));
	struct normalized_declarator * normalized_declarator_copy = malloc(sizeof(struct normalized_declarator));
	struct type_description * new_description = malloc(sizeof(struct type_description));
	unsigned int num_specifiers = struct_normalized_specifier_ptr_list_size(specifiers);
	unsigned int i;
	normalized_declarator_copy->declarator = create_abstract_declarator_from_normalized_declarator(normalized_declarator);
	normalized_declarator_copy->type = NORMALIZED_ABSTRACT_DECLARATOR;
	new_description->declarator = normalized_declarator_copy;
	new_description->specifiers = normalized_specifiers_copy;
	struct_normalized_specifier_ptr_list_create(normalized_specifiers_copy);
	for(i = 0; i < num_specifiers; i++){
		struct normalized_specifier * src_normalized_specifier = struct_normalized_specifier_ptr_list_get(specifiers, i);
		struct normalized_specifier * dst_normalized_specifier = malloc(sizeof(struct normalized_specifier));
		struct parser_node * specifiers_copy = copy_parser_node_children_only(src_normalized_specifier->specifier);
		dst_normalized_specifier->specifier = specifiers_copy;
		dst_normalized_specifier->type = src_normalized_specifier->type;
		struct_normalized_specifier_ptr_list_add(normalized_specifiers_copy, dst_normalized_specifier);
	}
	return new_description;
}

struct type_description * create_type_description_from_normalized_declaration_element(struct normalized_declaration_element * element){
	if(element->normalized_declarator && element->normalized_declarator->type == NORMALIZED_ENUMERATOR){
		/*  For an enumerator, the individual enumerator identifier doesn't have specifiers on it, they're on the parent node */
		struct normalized_declaration_set * declaration_set = create_normalized_declaration_set_from_parser_node(element->normalized_declaration_set->parent_normalized_declaration_set->set, 0);
		struct type_description * type = create_type_description_from_normalized_declarator_and_specifiers(element->normalized_declarator, declaration_set->normalized_specifiers);
		destroy_normalized_declaration_element_list(create_normalized_declaration_element_list(declaration_set));
		return type;
	}else{
		return create_type_description_from_normalized_declarator_and_specifiers(element->normalized_declarator, element->normalized_specifiers);
	}
}

void destroy_type_description(struct type_description * description){
	if(description->specifiers){
		unsigned int num_specifiers = struct_normalized_specifier_ptr_list_size(description->specifiers);
		unsigned int i;
		for(i = 0; i < num_specifiers; i++){
			struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(description->specifiers, i);
			destroy_parser_node_tree_and_c_lexer_tokens(normalized_specifier->specifier);
			free(normalized_specifier);
		}
		struct_normalized_specifier_ptr_list_destroy(description->specifiers);
		free(description->specifiers);
	}
	if(description->declarator){
		destroy_parser_node_tree_and_c_lexer_tokens(description->declarator->declarator);
		free(description->declarator);
	}
	free(description);
}


unsigned int get_enum_value(struct normalized_declaration_element * element){
	unsigned int rtn = 0; /* Enum starts at 0 */
	struct parser_node * n;
	unsigned char * label = copy_string(element->normalized_declarator->declarator->first_child->c_lexer_token->first_byte, element->normalized_declarator->declarator->first_child->c_lexer_token->last_byte);
	assert(element->normalized_declaration_set->set->type == ENUMERATOR_LIST);
	n = element->normalized_declaration_set->set->first_child;
	while(n->type == ENUMERATOR){
		unsigned char * cmp = copy_string(n->first_child->c_lexer_token->first_byte, n->first_child->c_lexer_token->last_byte);
		if(!strcmp((const char *)cmp, (const char *)label)){
			free(label);
			free(cmp);
			return rtn;
		}
		free(cmp);
		n = n->next->first_child;
		if(n->next)
			n = n->next;
		rtn++;
	}
	assert(0 && "enum value not found.");
	return 0;
}


int is_pointer(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_pointer(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_pointer(n->next);
		}else{
			return 1;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->next->type == ABSTRACT_DECLARATOR){
			return is_pointer(n->first_child->next);
		}else{
			return 0;
		}
	}else{
		assert(0);
	}
}

int is_function(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_function(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_function(n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return is_function(n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return 1;
			}else if(n->first_child->next->type == PARAMETER_TYPE_LIST){
				return 1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
	}
}


struct parser_node * get_nth_parameter_declaration_from_parameter_type_list(struct parser_node * n, unsigned int nth){
	/*  Will return the nth (0-based) parameter_declaration from the parameter_type_list */
	switch(n->type){
		case PARAMETER_TYPE_LIST:{
			return get_nth_parameter_declaration_from_parameter_type_list(n->first_child, nth);
		}case PARAMETER_LIST:{
			if(nth == 0){
				return n->first_child;
			}else{
				return get_nth_parameter_declaration_from_parameter_type_list(n->first_child->next, nth - 1);
			}
		}case PARAMETER_LIST_REST:{
			if(nth == 0){
				if(n->first_child->type == EPSILON){
					return 0;
				}else{
					return n->first_child->next;
				}
			}else{
				if(n->first_child->type == EPSILON){
					return 0;
				}else{
					return get_nth_parameter_declaration_from_parameter_type_list(n->first_child->next->next, nth - 1);
				}
			}
		}default:{
			assert(0);
		}
	}
}

struct parser_node * get_parameter_type_list_from_abstract_declarator(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return get_parameter_type_list_from_abstract_declarator(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return get_parameter_type_list_from_abstract_declarator(n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return get_parameter_type_list_from_abstract_declarator(n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return 0;
			}else if(n->first_child->next->type == PARAMETER_TYPE_LIST){
				return n->first_child->next;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
	}
}

struct parser_node * get_constant_expression_from_abstract_declarator(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return get_constant_expression_from_abstract_declarator(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return get_constant_expression_from_abstract_declarator(n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_SQUARE_BRACKET_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return get_constant_expression_from_abstract_declarator(n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return 0;
			}else if(n->first_child->next->type == CONSTANT_EXPRESSION){
				return n->first_child->next;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
	}
}

int is_parameter_type_list_variadic(struct parser_node * n){
	assert(n->type == PARAMETER_TYPE_LIST);
	return n->first_child->next && n->first_child->next->next && n->first_child->next->next->type == TERMINAL && n->first_child->next->next->c_lexer_token->type == ELLIPSIS;
}

int is_function_variadic(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_function_variadic(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_function_variadic(n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return is_function_variadic(n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return 0;
			}else if(n->first_child->next->type == PARAMETER_TYPE_LIST){
				return is_parameter_type_list_variadic(n->first_child->next);
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
	}
}

int is_function_k_and_r_c_style(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_function_k_and_r_c_style(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_function_k_and_r_c_style(n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return is_function_k_and_r_c_style(n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return 1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
	}
}

int is_array(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_array(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_array(n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return is_array(n->first_child->next);
			}else{
				return 0;
			}
		}else if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_SQUARE_BRACKET_CHAR){
			if(n->first_child->next->type == TERMINAL){
				return 1;
			}else if(n->first_child->next->type == CONSTANT_EXPRESSION){
				return 1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
	}
}

struct parser_node * copy_parser_node_tree_and_c_lexer_tokens(struct parser_node * n){
	/*  Create a copy of every node we can reach from n */
	if(n){
		struct parser_node * new_node = memory_pooler_malloc(g_parser_node_pool);
		new_node->type = n->type;
		new_node->first_child = copy_parser_node_tree_and_c_lexer_tokens(n->first_child);
		new_node->next = copy_parser_node_tree_and_c_lexer_tokens(n->next);
		if(n->c_lexer_token){
			new_node->c_lexer_token = memory_pooler_malloc(g_c_lexer_token_pool);
			new_node->c_lexer_token->type = n->c_lexer_token->type;
			new_node->c_lexer_token->first_byte = n->c_lexer_token->first_byte;
			new_node->c_lexer_token->last_byte = n->c_lexer_token->last_byte;
		}else{
			new_node->c_lexer_token = 0;
		}
		return new_node;
	}else{
		return 0;
	}
}

struct parser_node * copy_parser_node_children_only(struct parser_node * n){
	/*  Create a copy of only the children of n.  Often, we don't want to copy the node's sibilings */
	if(n){
		struct parser_node * new_node = memory_pooler_malloc(g_parser_node_pool);
		new_node->type = n->type;
		new_node->first_child = copy_parser_node_tree_and_c_lexer_tokens(n->first_child);
		new_node->next = 0;
		if(n->c_lexer_token){
			new_node->c_lexer_token = memory_pooler_malloc(g_c_lexer_token_pool);
			new_node->c_lexer_token->type = n->c_lexer_token->type;
			new_node->c_lexer_token->first_byte = n->c_lexer_token->first_byte;
			new_node->c_lexer_token->last_byte = n->c_lexer_token->last_byte;
		}else{
			new_node->c_lexer_token = 0;
		}
		return new_node;
	}else{
		return 0;
	}
}

struct parser_node * destroy_parser_node_tree_and_c_lexer_tokens(struct parser_node * n){
	if(n){
		if(n->c_lexer_token){
			memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
		}
		destroy_parser_node_tree_and_c_lexer_tokens(n->first_child);
		destroy_parser_node_tree_and_c_lexer_tokens(n->next);
		memory_pooler_free(g_parser_node_pool, n);
	}
	return 0;
}

struct parser_node * remove_one_pointer_level(struct parser_node * n){
	assert(n->type == POINTER);
	if(n->first_child->next && n->first_child->next->type == POINTER){
		n->first_child->next = remove_one_pointer_level(n->first_child->next);
		return n;
	}else if(n->first_child->next && n->first_child->next->next && n->first_child->next->next->type == POINTER){
		n->first_child->next->next = remove_one_pointer_level(n->first_child->next->next);
		return n;
	}else{
		/*  This will delete the pointer and possible type qualifier list */
		destroy_parser_node_tree_and_c_lexer_tokens(n);
		return 0;
	}
}

struct parser_node * create_pointer_node(void){
	struct c_lexer_token * ptr_token = memory_pooler_malloc(g_c_lexer_token_pool);
	struct parser_node * terminal = memory_pooler_malloc(g_parser_node_pool);
	struct parser_node * pointer = memory_pooler_malloc(g_parser_node_pool);
	ptr_token->type = MULTIPLY_CHAR;
	ptr_token->first_byte = (unsigned char *)g_ptr_string;
	ptr_token->last_byte = (unsigned char *)(g_ptr_string + (strlen(g_ptr_string) -1));
	terminal->c_lexer_token = ptr_token;
	terminal->first_child = 0;
	terminal->next = 0;
	terminal->type = TERMINAL;
	pointer->first_child = terminal;
	pointer->c_lexer_token = 0;
	pointer->next = 0;
	pointer->type = POINTER;
	return pointer;
}

struct parser_node * add_level_to_pointer(struct parser_node * n){
	/* Takes an existing pointer and adds a pointer onto it */
	struct parser_node * p = create_pointer_node();
	p->next = n;
	return p;
}

struct parser_node * convert_abstract_declarator_to_address_type_h(struct parser_node * n){
	/*
	The purpose of this function is to take an abstract declarator as input and change it
	to be the address type version of itself (like with the & operator)
	Input1:  int (*(*(void))(void))(int i);
	Output1: int (*(*(*)(void))(void))(int i);
	Input2:  int
	Output2: int *
	*/
	switch (n->type){
		case POINTER:{
			if(n->next){
				n->next = convert_abstract_declarator_to_address_type_h(n->next);
				return n;
			}else{
				return add_level_to_pointer(n);
			}
		}case ABSTRACT_DECLARATOR:{
			/*  The current node should not change */
			n->first_child = convert_abstract_declarator_to_address_type_h(n->first_child);
			return n;
		}case DIRECT_ABSTRACT_DECLARATOR:{
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				/*  Don't need an assignment, abstract declarator node should not change */
				convert_abstract_declarator_to_address_type_h(n->first_child->next);
				return n;
			}else{
				struct parser_node * pointer = create_pointer_node();
				struct parser_node * abstract_declarator = memory_pooler_malloc(g_parser_node_pool);
				struct parser_node * current_first_child = n->first_child;
				struct parser_node * open_paren = memory_pooler_malloc(g_parser_node_pool);
				struct parser_node * close_paren = memory_pooler_malloc(g_parser_node_pool);
				struct parser_node * direct_abstract_declarator_rest = memory_pooler_malloc(g_parser_node_pool);
				struct c_lexer_token * open_paren_token = memory_pooler_malloc(g_c_lexer_token_pool);
				struct c_lexer_token * close_paren_token = memory_pooler_malloc(g_c_lexer_token_pool);
				n->first_child = open_paren;
				/* Set up the '(' token */
				open_paren_token->type = OPEN_PAREN_CHAR;
				open_paren_token->first_byte = (unsigned char *)g_open_paren_string;
				open_paren_token->last_byte = (unsigned char *)(g_open_paren_string + (strlen(g_open_paren_string) -1));
				/* Set up the TERMINAL for '(' token */
				open_paren->c_lexer_token = open_paren_token;
				open_paren->next = abstract_declarator;
				open_paren->first_child = 0;
				open_paren->type = TERMINAL;
				/* Set up the abstract declarator */
				abstract_declarator->c_lexer_token = 0;
				abstract_declarator->next = close_paren;
				abstract_declarator->first_child = pointer;
				abstract_declarator->type = ABSTRACT_DECLARATOR;
				/* Set up the ')' token */
				close_paren_token->type = CLOSE_PAREN_CHAR;
				close_paren_token->first_byte = (unsigned char *)g_close_paren_string;
				close_paren_token->last_byte = (unsigned char *)(g_close_paren_string + (strlen(g_close_paren_string) -1));
				/* Set up the TERMINAL for ')' token */
				close_paren->c_lexer_token = close_paren_token;
				close_paren->next = direct_abstract_declarator_rest;
				close_paren->first_child = 0;
				close_paren->type = TERMINAL;
				/* Move what was here into the new direct_abstract_declarator_rest */
				direct_abstract_declarator_rest->c_lexer_token = 0;
				direct_abstract_declarator_rest->next = 0;
				direct_abstract_declarator_rest->first_child = current_first_child;
				direct_abstract_declarator_rest->type = DIRECT_ABSTRACT_DECLARATOR_REST;
				return n;
			}
		}default:{
			assert(0 && "Unknown conversion case for address type.");
			return 0;
		}
	}
}

struct parser_node * convert_abstract_declarator_to_address_type(struct parser_node * n){
	assert(!n || n->type == ABSTRACT_DECLARATOR);
	if(!n){
		struct parser_node * abstract_declarator = memory_pooler_malloc(g_parser_node_pool);
		abstract_declarator->first_child = create_pointer_node();
		abstract_declarator->c_lexer_token = 0;
		abstract_declarator->next = 0;
		abstract_declarator->type = ABSTRACT_DECLARATOR;
		return abstract_declarator;
	}
	n->first_child = convert_abstract_declarator_to_address_type_h(n->first_child);
	return n;
}

struct parser_node * convert_abstract_declarator_to_pointer_type(struct parser_node * n){
	if(n == 0){
		return 0;
	}

	switch(n->type){
		case DIRECT_ABSTRACT_DECLARATOR:{
			n->first_child = convert_abstract_declarator_to_pointer_type(n->first_child);
			n->next = 0;
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				memory_pooler_free(g_parser_node_pool, n);
				return 0;
			}
		}case ABSTRACT_DECLARATOR:{
			assert(!(n->next) && "This function only handles the top-most case of abstract_declarator directly.");
			if(n->first_child->next){
				n->first_child->next = convert_abstract_declarator_to_pointer_type(n->first_child->next);
				return n;
			}
			n->first_child = convert_abstract_declarator_to_pointer_type(n->first_child);
			if(n->first_child){
				return n;
			}else{
				memory_pooler_free(g_parser_node_pool, n);
				return 0;
			}
		}case POINTER:{
			if(n->next){
				n->next = convert_abstract_declarator_to_pointer_type(n->next);
				return n;
			}else{
				return remove_one_pointer_level(n);
			}
		}case EPSILON:{
			return n;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case OPEN_PAREN_CHAR:{
					if(n->next->type == ABSTRACT_DECLARATOR){
						n->next->first_child = convert_abstract_declarator_to_pointer_type(n->next->first_child);
						if(n->next->first_child){
							/*  abstract_declarator still exists */
							return n;
						}else{
							/*  In this case, we're going to delete the ( abs ) nodes 
							  Determine if the _rest is an epsilon or not */
							if(n->next->next->next->first_child->type == EPSILON){
								/* There is nothing left */
								destroy_parser_node_tree_and_c_lexer_tokens(n->next->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next);
								memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n);
								return 0;
							}else{
								/*  The first_child of the _rest becomes the first_child of the parent */
								struct parser_node * rtn = n->next->next->next->first_child;
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next);
								memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n);
								return rtn;
							}
						}
					}else{
						assert(0 && "Must not have been a pointer type.");
						return 0;
					}
				}default:{
					assert(0);
				}
			}
		}default:{
			assert(0);
		}
	}
}


struct parser_node * convert_abstract_declarator_to_array_type(struct parser_node * n){
	if(n == 0){
		return 0;
	}

	switch(n->type){
		case DIRECT_ABSTRACT_DECLARATOR:{
			n->first_child = convert_abstract_declarator_to_array_type(n->first_child);
			n->next = 0;
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				memory_pooler_free(g_parser_node_pool, n);
				return 0;
			}
		}case ABSTRACT_DECLARATOR:{
			assert(!(n->next) && "This function only handles the top-most case of abstract_declarator directly.");
			if(n->first_child->next){
				n->first_child->next = convert_abstract_declarator_to_array_type(n->first_child->next);
				return n;
			}
			n->first_child = convert_abstract_declarator_to_array_type(n->first_child);
			if(n->first_child){
				return n;
			}else{
				memory_pooler_free(g_parser_node_pool, n);
				return 0;
			}
		}case POINTER:{
			n->next = convert_abstract_declarator_to_array_type(n->next);
			return n;
		}case EPSILON:{
			return n;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case OPEN_PAREN_CHAR:{
					if(n->next->type == ABSTRACT_DECLARATOR){
						n->next->first_child = convert_abstract_declarator_to_array_type(n->next->first_child);
						if(n->next->first_child){
							/*  abstract_declarator still exists */
							return n;
						}else{
							/*  In this case, we're going to delete the ( abs ) nodes 
							  Determine if the _rest is an epsilon or not */
							if(n->next->next->next->first_child->type == EPSILON){
								/* There is nothing left */
								destroy_parser_node_tree_and_c_lexer_tokens(n->next->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next);
								memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n);
								return 0;
							}else{
								/*  The first_child of the _rest becomes the first_child of the parent */
								struct parser_node * rtn = n->next->next->next->first_child;
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next);
								memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n);
								return rtn;
							}
						}
					}else{
						assert(0);
						return 0;
					}
				}case OPEN_SQUARE_BRACKET_CHAR:{
					if(n->next->type == TERMINAL && n->next->c_lexer_token->type == CLOSE_SQUARE_BRACKET_CHAR){
						if(n->next->next->first_child->type == EPSILON){
							/*  This also destroys the _rest and epsilon */
							destroy_parser_node_tree_and_c_lexer_tokens(n->next);
							memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n);
							return 0;
						}else{
							struct parser_node * rtn = n->next->next->first_child;
							memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next->next);
							memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next);
							memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n);
							return rtn;
						}
					}else if(n->next->type == CONSTANT_EXPRESSION){
						if(n->next->next->next->first_child->type == EPSILON){
							/*  This also destroys the _rest and epsilon */
							destroy_parser_node_tree_and_c_lexer_tokens(n->next);
							memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n);
							return 0;
						}else{
							struct parser_node * rtn = n->next->next->next->first_child;
							memory_pooler_free(g_c_lexer_token_pool, n->next->next->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next->next->next);
							memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next->next);
							/* Destroy the constant expression */
							destroy_parser_node_tree_and_c_lexer_tokens(n->next->first_child);
							memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next);
							memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n);
							return rtn;
						}
					}else{
						assert(0 && "Must not have been an array type.");
						return 0;
					}
				}default:{
					assert(0);
				}
			}
		}default:{
			assert(0);
		}
	}
}


struct parser_node * convert_abstract_declarator_to_function_type(struct parser_node * n){
	if(n == 0){
		return 0;
	}

	switch(n->type){
		case DIRECT_ABSTRACT_DECLARATOR:{
			n->first_child = convert_abstract_declarator_to_function_type(n->first_child);
			n->next = 0;
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				memory_pooler_free(g_parser_node_pool, n);
				return 0;
			}
		}case ABSTRACT_DECLARATOR:{
			assert(!(n->next) && "This function only handles the top-most case of abstract_declarator directly.");
			if(n->first_child->next){
				n->first_child->next = convert_abstract_declarator_to_function_type(n->first_child->next);
				return n;
			}
			n->first_child = convert_abstract_declarator_to_function_type(n->first_child);
			if(n->first_child){
				return n;
			}else{
				memory_pooler_free(g_parser_node_pool, n);
				return 0;
			}
		}case POINTER:{
			n->next = convert_abstract_declarator_to_function_type(n->next);
			return n;
		}case EPSILON:{
			return n;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case OPEN_PAREN_CHAR:{
					if(n->next->type == ABSTRACT_DECLARATOR){
						n->next->first_child = convert_abstract_declarator_to_function_type(n->next->first_child);
						if(n->next->first_child){
							/*  abstract_declarator still exists */
							return n;
						}else{
							/*  In this case, we're going to delete the ( abs ) nodes 
							  Determine if the _rest is an epsilon or not */
							if(n->next->next->next->first_child->type == EPSILON){
								/* There is nothing left */
								destroy_parser_node_tree_and_c_lexer_tokens(n->next->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next);
								memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n);
								return 0;
							}else{
								/*  The first_child of the _rest becomes the first_child of the parent */
								struct parser_node * rtn = n->next->next->next->first_child;
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next->next);
								memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n->next);
								memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
								memory_pooler_free(g_parser_node_pool, n);
								return rtn;
							}
						}
					}else if(n->next->type == TERMINAL && n->next->c_lexer_token->type == CLOSE_PAREN_CHAR){
						if(n->next->next->first_child->type == EPSILON){
							/*  This also destroys the _rest and epsilon */
							destroy_parser_node_tree_and_c_lexer_tokens(n->next);
							memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n);
							return 0;
						}else{
							struct parser_node * rtn = n->next->next->first_child;
							memory_pooler_free(g_parser_node_pool, n->next->next);
							memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next);
							memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n);
							return rtn;
						}
					}else if(n->next->type == PARAMETER_TYPE_LIST){
						if(n->next->next->next->first_child->type == EPSILON){
							/*  This also destroys the _rest and epsilon */
							destroy_parser_node_tree_and_c_lexer_tokens(n->next);
							memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n);
							return 0;
						}else{
							struct parser_node * rtn = n->next->next->next->first_child;
							memory_pooler_free(g_c_lexer_token_pool, n->next->next->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next->next->next);
							memory_pooler_free(g_c_lexer_token_pool, n->next->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next->next);
							memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n->next);
							memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
							memory_pooler_free(g_parser_node_pool, n);
							return rtn;
						}
					}else{
						assert(0 && "Must not have been a function type.");
						return 0;
					}
				}default:{
					assert(0);
				}
			}
		}default:{
			assert(0);
		}
	}
}

struct c_lexer_token * get_identifier_token_from_declarator(struct parser_node * n){
	/*  This function should be called externally with non-abstract declarators */
	if(n == 0){
		return 0;
	}

	switch(n->type){
		case DIRECT_DECLARATOR:{
			return get_identifier_token_from_declarator(n->first_child);
		}case DECLARATOR:{
			return get_identifier_token_from_declarator(n->first_child);
		}case POINTER:{
			return get_identifier_token_from_declarator(n->next);
		}case DIRECT_DECLARATOR_REST:{
			return get_identifier_token_from_declarator(n->first_child);
		}case CONSTANT_EXPRESSION:{
			return get_identifier_token_from_declarator(n->next);
		}case PARAMETER_TYPE_LIST:{
			return get_identifier_token_from_declarator(n->next);
		}case IDENTIFIER_LIST:{
			return get_identifier_token_from_declarator(n->next);
		}case EPSILON:{
			return 0;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case IDENTIFIER:{
					return n->c_lexer_token;
				}case OPEN_PAREN_CHAR:{
					if(n->next->type == DECLARATOR){
						/*  There are 2 possible locations for the identifier at this point */
						struct c_lexer_token * n1 = get_identifier_token_from_declarator(n->next);
						struct c_lexer_token * n2 = get_identifier_token_from_declarator(n->next->next);
						return n1 ? n1 : n2;
					}else{
						return get_identifier_token_from_declarator(n->next);
					}
				}case CLOSE_PAREN_CHAR:{
					return get_identifier_token_from_declarator(n->next);
				}case OPEN_SQUARE_BRACKET_CHAR:{
					return get_identifier_token_from_declarator(n->next);
				}case CLOSE_SQUARE_BRACKET_CHAR:{
					return get_identifier_token_from_declarator(n->next);
				}default:{
					assert(0);
				}
			}
		}default:{
			assert(0);
		}
	}
}


struct parser_node * convert_declarator_to_abstract_declarator(struct parser_node * n){
	struct parser_node * rtn;
	if(!(!n || n->type == DECLARATOR)){
		assert(!n || n->type == DECLARATOR);
	}
	rtn = convert_declarator_to_abstract_declarator_h(n);
	assert(!rtn || rtn->type == ABSTRACT_DECLARATOR);
	return rtn;
}


struct parser_node * convert_declarator_to_abstract_declarator_h(struct parser_node * n){
	if(n == 0){
		return 0;
	}
	switch(n->type){
		case ABSTRACT_DECLARATOR:{
			return n;
		}case DIRECT_DECLARATOR:{
			/* its abstract now */
			n->type = DIRECT_ABSTRACT_DECLARATOR;
			n->first_child = convert_declarator_to_abstract_declarator_h(n->first_child);
			n->next = convert_declarator_to_abstract_declarator_h(n->next);
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				struct parser_node * rtn = n->next;
				memory_pooler_free(g_parser_node_pool, n);
				return rtn;
			}
		}case DECLARATOR:{
			n->type = ABSTRACT_DECLARATOR;
			n->first_child = convert_declarator_to_abstract_declarator_h(n->first_child);
			n->next = convert_declarator_to_abstract_declarator_h(n->next);
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				struct parser_node * rtn = n->next;
				memory_pooler_free(g_parser_node_pool, n);
				return rtn;
			}
		}case POINTER:{
			n->next = convert_declarator_to_abstract_declarator_h(n->next);
			return n;
		}case DIRECT_DECLARATOR_REST:{
			n->type = DIRECT_ABSTRACT_DECLARATOR_REST;
			n->first_child = convert_declarator_to_abstract_declarator_h(n->first_child);
			return n;
		}case CONSTANT_EXPRESSION:{
			n->next = convert_declarator_to_abstract_declarator_h(n->next);
			return n;
		}case PARAMETER_TYPE_LIST:{
			n->next = convert_declarator_to_abstract_declarator_h(n->next);
			return n;
		}case IDENTIFIER_LIST:{
			/* identifier lists don't exist in abstract declarators */
			struct parser_node * next = n->next;
			n->next = 0;
			destroy_parser_node_tree_and_c_lexer_tokens(n);
			return convert_declarator_to_abstract_declarator_h(next);
		}case EPSILON:{
			return n;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case IDENTIFIER:{
					if(n->next->first_child->type == EPSILON){
						/*  There is a _rest that just has an epsilon node.  Delete them. */
						destroy_parser_node_tree_and_c_lexer_tokens(n->next);
						memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
						memory_pooler_free(g_parser_node_pool, n);
						return 0;
					}else{
						/*  The first child is no longer the identifier, it is the expansion of the _rest */
						struct parser_node * rtn = convert_declarator_to_abstract_declarator_h(n->next->first_child);
						memory_pooler_free(g_c_lexer_token_pool, n->next->c_lexer_token);
						memory_pooler_free(g_parser_node_pool, n->next);
						memory_pooler_free(g_c_lexer_token_pool, n->c_lexer_token);
						memory_pooler_free(g_parser_node_pool, n);
						return rtn;
					}
				}case OPEN_PAREN_CHAR:{
					/*  Just keep moving along */
					n->next = convert_declarator_to_abstract_declarator_h(n->next);
					return n;
				}case CLOSE_PAREN_CHAR:{
					/*  Just keep moving along */
					n->next = convert_declarator_to_abstract_declarator_h(n->next);
					return n;
				}case OPEN_SQUARE_BRACKET_CHAR:{
					/*  Just keep moving along */
					n->next = convert_declarator_to_abstract_declarator_h(n->next);
					return n;
				}case CLOSE_SQUARE_BRACKET_CHAR:{
					/*  Just keep moving along */
					n->next = convert_declarator_to_abstract_declarator_h(n->next);
					return n;
				}default:{
					assert(0);
				}
			}
		}default:{
			assert(0);
		}
	}
}

struct parser_node * create_abstract_declarator_from_declarator(struct parser_node * n){
	assert(n->type == DECLARATOR);
	return convert_declarator_to_abstract_declarator(copy_parser_node_children_only(n));
}


struct parser_node * create_abstract_declarator_from_normalized_declarator(struct normalized_declarator * declarator){
	/*  Abstract declarators are just like regular declarators, but without an identifier.
	An example of this is what comes after the declaration specifiers when stating parameter types in a function prorotype:
	int foo(int, int (*)(int), double (*a)[3], int [3]);
	In this example the abstract declarators are:
	Arg1: [no abstract declarator]
	Arg2: (*)(int)
	Arg3: (*a)[3]
	Arg4: [3]
	
	*/
	if(!(declarator)){
		return 0;
	}

	switch(declarator->type){
		case NORMALIZED_DECLARATOR:{
			struct parser_node * rtn;
			assert(declarator->declarator->type == DECLARATOR);
			rtn = create_abstract_declarator_from_declarator(declarator->declarator);
			assert(!rtn || rtn->type == ABSTRACT_DECLARATOR);
			return rtn;
		}case NORMALIZED_ABSTRACT_DECLARATOR:{
			struct parser_node * copy;
			assert(!declarator || !declarator->declarator || declarator->declarator->type == ABSTRACT_DECLARATOR);
			copy = copy_parser_node_children_only(declarator->declarator);
			assert(!copy || copy->type == ABSTRACT_DECLARATOR);
			return copy;
		}case NORMALIZED_STRUCT_DECLARATOR:{
			if(declarator->declarator->first_child->type == DECLARATOR){
				struct parser_node * rtn;
				assert(declarator->declarator->first_child->type == DECLARATOR);
				rtn = create_abstract_declarator_from_declarator(declarator->declarator->first_child);
				assert(!rtn || rtn->type == ABSTRACT_DECLARATOR);
				return rtn;
			}else{
				return 0;
			}
		}case NORMALIZED_INIT_DECLARATOR:{
			struct parser_node * rtn;
			assert(declarator->declarator->first_child->type == DECLARATOR);
			rtn =  create_abstract_declarator_from_declarator(declarator->declarator->first_child);
			assert(!rtn || rtn->type == ABSTRACT_DECLARATOR);
			return rtn;
		}case NORMALIZED_ENUMERATOR:{
			return 0;
		}default:{
			assert(0);
		}
	}
	return 0;
}

struct c_lexer_token * get_identifier_token_from_normalized_declarator(struct normalized_declarator * declarator){
	if(!(declarator)){
		return 0;
	}

	switch(declarator->type){
		case NORMALIZED_DECLARATOR:{
			return get_identifier_token_from_declarator(declarator->declarator);
		}case NORMALIZED_ABSTRACT_DECLARATOR:{
			return 0;
		}case NORMALIZED_STRUCT_DECLARATOR:{
			if(declarator->declarator->first_child->type == DECLARATOR){
				return get_identifier_token_from_declarator(declarator->declarator->first_child);
			}else{
				return 0;
			}
		}case NORMALIZED_INIT_DECLARATOR:{
			return get_identifier_token_from_declarator(declarator->declarator->first_child);
		}case NORMALIZED_ENUMERATOR:{
			return declarator->declarator->first_child->c_lexer_token;
		}default:{
			assert(0);
		}
	}
	return 0;
}

struct parser_node * get_function_definition(struct normalized_declaration_set * declaration_set){
	return declaration_set->type == NORMALIZED_FUNCTION_DEFINITION ? declaration_set->set : 0;
}

int is_bitfield(struct parser_node * n){
	return n && n->type == STRUCT_DECLARATOR && (
		n->first_child->type == TERMINAL ||
		(n->first_child->next && n->first_child->next->type == TERMINAL)
	);
}

int is_struct(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n->first_child->first_child && n->first_child->first_child->c_lexer_token->type == STRUCT;
}

int is_union(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n->first_child->first_child && n->first_child->first_child->c_lexer_token->type == UNION;
}

unsigned int contains_struct_or_union_or_enum_definition(struct namespace_object * obj){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	unsigned int i;
	for(i = 0; i < num_elements; i++){
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, i);
		struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(element->normalized_specifiers);
		if(struct_or_union_or_enum_specifier){
			int its_a_struct = is_struct(struct_or_union_or_enum_specifier);
			int its_a_union = is_union(struct_or_union_or_enum_specifier);
			int its_a_enum = is_enum(struct_or_union_or_enum_specifier);
			if(its_a_enum){
				if(get_enumerator_list(struct_or_union_or_enum_specifier)){
					return 1;
				}
			}

			if(its_a_struct || its_a_union){
				if(get_struct_declaration_list(struct_or_union_or_enum_specifier)){
					return 1;
				}
			}
		}
	}
	return 0;
}

int is_enum(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type;
}

int is_anonymous_struct_or_union_or_enum(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n->first_child->next->c_lexer_token->type != IDENTIFIER;
}

int is_struct_or_union_or_enum_definition(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return (
		n->first_child->next->c_lexer_token->type == OPEN_BRACE_CHAR || (
			n->first_child->next->next && 
			n->first_child->next->next->c_lexer_token->type == OPEN_BRACE_CHAR
		)
	);
}

struct parser_node * get_struct_declaration_list(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER);
	if(n->first_child->next->next){
		if(n->first_child->next->next->type == STRUCT_DECLARATION_LIST){
			return n->first_child->next->next;
		}else if(n->first_child->next->next->next){
			if(n->first_child->next->next->next->type == STRUCT_DECLARATION_LIST){
				return n->first_child->next->next->next;
			}
		}
	}
	return 0;
}

struct parser_node * get_enumerator_list(struct parser_node * n){
	assert(n->type == ENUM_SPECIFIER);
	if(n->first_child->next->next){
		if(n->first_child->next->next->type == ENUMERATOR_LIST){
			return n->first_child->next->next;
		}else if(n->first_child->next->next->next){
			if(n->first_child->next->next->next->type == ENUMERATOR_LIST){
				return n->first_child->next->next->next;
			}
		}
	}
	return 0;
}

struct c_lexer_token * get_struct_or_union_or_enum_tag_token(struct parser_node * n){
	assert(!n || n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n && n->first_child->next->c_lexer_token->type == IDENTIFIER ? n->first_child->next->c_lexer_token : 0;
}

struct parser_node * get_struct_or_union_or_enum_specifier(struct struct_normalized_specifier_ptr_list * list){
	unsigned int len = struct_normalized_specifier_ptr_list_size(list);
	unsigned int i;
	/* TODO:  This will only return the *first* struct or union specifier.  Others are ignored. */
	for(i = 0; i < len; i++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(list, i);
		if(normalized_specifier->type == NORMALIZED_TYPE_SPECIFIER){
			if(
				normalized_specifier->specifier->first_child->type == STRUCT_OR_UNION_SPECIFIER ||
				normalized_specifier->specifier->first_child->type == ENUM_SPECIFIER
			){
				return normalized_specifier->specifier->first_child;
			}
		}
	}
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

void print_normalized_declaration_element(struct unsigned_char_list * buffer, struct normalized_declaration_element * element, unsigned int depth){
	unsigned int i;
	if(depth == 0){
		buffered_printf(buffer, ";===============================\n");
	}
	buffered_printf(buffer, ";");
	for(i = 0; i < (2*depth);i++)
		buffered_printf(buffer, "-");
	buffered_printf(buffer, ">");

	print_normalized_declaration_declarator_and_specifiers(buffer, element->normalized_declarator, element->normalized_specifiers, get_function_definition(element->normalized_declaration_set), depth);
}

void print_normalized_declaration_declarator_and_specifiers(struct unsigned_char_list * buffer, struct normalized_declarator * normalized_declarator, struct struct_normalized_specifier_ptr_list * normalized_specifiers, struct parser_node * function_definition, unsigned int depth){
	struct parser_node * struct_or_union_or_enum_specifier;
	unsigned int i;
	buffered_printf(buffer,";Specifiers: ");
	print_normalized_specifier(buffer, normalized_specifiers);
	buffered_printf(buffer," ");

	if(normalized_declarator){
		struct c_lexer_token * identifier_token = get_identifier_token_from_normalized_declarator(normalized_declarator);
		if(identifier_token){
			buffered_printf(buffer, "Declarator identifier is '");
			add_string_to_buffer(buffer, identifier_token->first_byte, identifier_token->last_byte);
			buffered_printf(buffer, "'");
		}
		buffered_printf(buffer, "  The full declarator is: ");

		add_terminal_tokens_to_buffer(buffer, normalized_declarator->declarator);
	}
	if(function_definition){
		buffered_printf(buffer, "{...}");
	}
	buffered_printf(buffer, "\n");

	if(normalized_declarator && is_bitfield(normalized_declarator->declarator)){
		buffered_printf(buffer,";");
		for(i = 0; i < (2*depth);i++)
			buffered_printf(buffer,"-");
		buffered_printf(buffer, ">This is a bitfield.\n");
	}

	struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(normalized_specifiers);
	if(struct_or_union_or_enum_specifier){
		struct c_lexer_token * token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
		struct parser_node * declaration_list = 0;
		struct parser_node * enumerator_list = 0;
		int its_a_struct = is_struct(struct_or_union_or_enum_specifier);
		int its_a_union = is_union(struct_or_union_or_enum_specifier);
		int its_a_enum = is_enum(struct_or_union_or_enum_specifier);
		buffered_printf(buffer,";");
		if(its_a_enum){
			enumerator_list = get_enumerator_list(struct_or_union_or_enum_specifier);
		}

		if(its_a_struct || its_a_union){
			declaration_list = get_struct_declaration_list(struct_or_union_or_enum_specifier);
		}

		for(i = 0; i < (2*depth);i++)
			buffered_printf(buffer,"-");
		buffered_printf(buffer,";>This is ");
		if(token){
			buffered_printf(buffer,"a ");
		}else{
			buffered_printf(buffer,"an anonymous ");
		}
		if(its_a_struct){
			buffered_printf(buffer,"struct");
		}else if(its_a_union){
			buffered_printf(buffer,"union");
		}else if(its_a_enum){
			buffered_printf(buffer,"enum");
		}else{
			assert(0);
		}
		buffered_printf(buffer," declaration");
		if(token){
			buffered_printf(buffer," with tag '");
			add_string_to_buffer(buffer, token->first_byte, token->last_byte);
			buffered_printf(buffer,"'.");
		}else{
			buffered_printf(buffer,".");
		}
		buffered_printf(buffer,"\n");

		buffered_printf(buffer,";");
		if(declaration_list){
			/*  Print out the struct declarations  */
			/*
			struct parser_node * node = declaration_list->first_child;
			for(i = 0; i < (2*depth);i++)
				buffered_printf(buffer,"-");
			buffered_printf(buffer,">It is also a definition that defines:\n");
			while(node->type != EPSILON){
				print_generic_declaration(buffer, node, depth + 1);
				node = node->next->first_child;
			}
			*/
		}
		if(enumerator_list){
			for(i = 0; i < (2*depth);i++)
				buffered_printf(buffer,"-");
			buffered_printf(buffer,">It is also a definition that defines:\n");
			buffered_printf(buffer,";");
			for(i = 0; i < (2*depth);i++)
				buffered_printf(buffer,"-");
			print_generic_declaration(buffer, enumerator_list, depth + 1);
		}
	}
	if(normalized_declarator){
		struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(normalized_declarator);
		struct parser_node * address_declarator = convert_abstract_declarator_to_address_type(copy_parser_node_children_only(abstract_declarator));
		buffered_printf(buffer,";");

		for(i = 0; i < (2*depth);i++)
			buffered_printf(buffer,"-");
		buffered_printf(buffer,">Here is the abstract declarator: ");
		add_terminal_tokens_to_buffer(buffer, abstract_declarator);
		buffered_printf(buffer,"\n;Here is the address type: ");
		add_terminal_tokens_to_buffer(buffer, address_declarator);

		destroy_parser_node_tree_and_c_lexer_tokens(address_declarator);

		while(abstract_declarator){
			if(is_pointer(abstract_declarator)){
				abstract_declarator = convert_abstract_declarator_to_pointer_type(abstract_declarator);
				buffered_printf(buffer," it is a pointer of type '");
				add_terminal_tokens_to_buffer(buffer, abstract_declarator);
				buffered_printf(buffer,"'");
			}

			if(is_function(abstract_declarator)){
				struct parser_node * parameter_type_list;
				struct parser_node * parameter_declaration;
				unsigned int arg_index = 0;
				parameter_type_list = get_parameter_type_list_from_abstract_declarator(abstract_declarator);
				if(parameter_type_list){
					while((parameter_declaration = get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list, arg_index))){
						buffered_printf(buffer,";Here is param number %d\n", arg_index);
						buffered_printf(buffer,";");
						print_generic_declaration(buffer, parameter_declaration, depth);
						arg_index++;
					}
				}else{
					buffered_printf(buffer,"No parameter list\n");
					buffered_printf(buffer,";");
				}
				buffered_printf(buffer,";");
				abstract_declarator = convert_abstract_declarator_to_function_type(abstract_declarator);
				buffered_printf(buffer," it is a function ");
				if(is_function_variadic(abstract_declarator)){
					buffered_printf(buffer," that is variadic ");
				}
				if(is_function_k_and_r_c_style(abstract_declarator)){
					buffered_printf(buffer," that is K&R C style ");
				}
				buffered_printf(buffer," its function type is '");
				add_terminal_tokens_to_buffer(buffer, abstract_declarator);
				buffered_printf(buffer,"'\n");
				buffered_printf(buffer,";");

			}

			if(is_array(abstract_declarator)){
				abstract_declarator = convert_abstract_declarator_to_array_type(abstract_declarator);
				buffered_printf(buffer," it is an array ");
				buffered_printf(buffer," its array type is '");
				add_terminal_tokens_to_buffer(buffer, abstract_declarator);
				buffered_printf(buffer,"'");
			}
		}

		destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);
		buffered_printf(buffer,"\n");
	}
}

void print_generic_declaration(struct unsigned_char_list * buffer, struct parser_node * n, unsigned int depth){
	struct normalized_declaration_set * declaration_set = create_normalized_declaration_set_from_parser_node(n, 0);
	struct struct_normalized_declaration_element_ptr_list * element_list = create_normalized_declaration_element_list(declaration_set);
	unsigned int i;

	for(i = 0; i < struct_normalized_declaration_element_ptr_list_size(element_list); i++){
		print_normalized_declaration_element(buffer, struct_normalized_declaration_element_ptr_list_get(element_list, i), depth);
	}
	destroy_normalized_declaration_element_list(element_list);
}

struct first_and_last_namespace_object manage_generic_declaration(struct parser_state * state, struct parser_node * n, struct normalized_declaration_set * parent_set, unsigned int depth, enum object_location object_location, struct namespace_object * previous, struct struct_namespace_object_ptr_list * forced_namespace){
	struct normalized_declaration_set * declaration_set = create_normalized_declaration_set_from_parser_node(n, parent_set);
	struct struct_normalized_declaration_element_ptr_list * element_list = create_normalized_declaration_element_list(declaration_set);
	(void)depth;
	push_operation(state, SAVE_DECLARATION_ELEMENTS, element_list); /*  We can only destroy this information after we're done with the parser state because we will reference it every time we interact with variables */
	return manage_declaration_elements(state, element_list, object_location, previous, forced_namespace);
}

struct struct_normalized_declaration_element_ptr_list * create_normalized_declaration_element_list(struct normalized_declaration_set * declaration_set){
	struct struct_normalized_declaration_element_ptr_list * list = malloc(sizeof(struct struct_normalized_declaration_element_ptr_list));
	unsigned int num_declarators = struct_normalized_declarator_ptr_list_size(declaration_set->normalized_declarators);
	unsigned int i;
	struct_normalized_declaration_element_ptr_list_create(list);

	if(num_declarators == 0){
		/*  If there are no declarators, we still want the declaration specifiers because they can describe structs */
		struct normalized_declaration_element * new_element = malloc(sizeof(struct normalized_declaration_element));
		new_element->normalized_specifiers = malloc(sizeof(struct struct_normalized_specifier_ptr_list));
		*(new_element->normalized_specifiers) = struct_normalized_specifier_ptr_list_copy(declaration_set->normalized_specifiers);
		new_element->normalized_declarator = 0;
		new_element->normalized_declaration_set = declaration_set;
		struct_normalized_declaration_element_ptr_list_add(list, new_element);
	}else{
		for(i = 0; i < num_declarators; i++){
			struct normalized_declaration_element * new_element = malloc(sizeof(struct normalized_declaration_element));
			new_element->normalized_specifiers = malloc(sizeof(struct struct_normalized_specifier_ptr_list));
			*(new_element->normalized_specifiers) = struct_normalized_specifier_ptr_list_copy(declaration_set->normalized_specifiers);
			new_element->normalized_declarator = struct_normalized_declarator_ptr_list_get(declaration_set->normalized_declarators,i);
			new_element->normalized_declaration_set = declaration_set;
			struct_normalized_declaration_element_ptr_list_add(list, new_element);
		}
	}

	return list;
}

void destroy_normalized_declaration_element_list(struct struct_normalized_declaration_element_ptr_list * list){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(list);
	unsigned int num_declarators;
	unsigned int num_specifiers;
	unsigned int i;
	struct normalized_declaration_set * declaration_set;

	assert(num_elements);
	/*  Because all declaration elements in a list should share the same declaration set, we can destroy the declaration set after we destroy the list elements */
	declaration_set = struct_normalized_declaration_element_ptr_list_get(list, 0)->normalized_declaration_set;

	/*  In the element list, we just have copies of the pointers from the normalized specifiers and declaratiors on the declaration set. */
	for(i = 0; i < num_elements; i++){
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(list, i);
		struct_normalized_specifier_ptr_list_destroy(element->normalized_specifiers);
		free(element->normalized_specifiers);
		free(element);
	}
	struct_normalized_declaration_element_ptr_list_destroy(list);
	free(list);

	/*  Now destroy the copies of the parser nodes and lexer tokens, normalized specifier and declarator structs...  */
	num_specifiers = struct_normalized_specifier_ptr_list_size(declaration_set->normalized_specifiers);
	for(i = 0; i < num_specifiers; i++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(declaration_set->normalized_specifiers, i);
		if(normalized_specifier){
			destroy_parser_node_tree_and_c_lexer_tokens(normalized_specifier->specifier);
		}
		free(normalized_specifier);
	}
	num_declarators = struct_normalized_declarator_ptr_list_size(declaration_set->normalized_declarators);
	for(i = 0; i < num_declarators; i++){
		struct normalized_declarator * normalized_declarator = struct_normalized_declarator_ptr_list_get(declaration_set->normalized_declarators, i);
		if(normalized_declarator){
			destroy_parser_node_tree_and_c_lexer_tokens(normalized_declarator->declarator);
		}
		free(normalized_declarator);
	}
	/* Free memory used by the lists */
	struct_normalized_specifier_ptr_list_destroy(declaration_set->normalized_specifiers);
	struct_normalized_declarator_ptr_list_destroy(declaration_set->normalized_declarators);
	/* Free the list structs */
	free(declaration_set->normalized_specifiers);
	free(declaration_set->normalized_declarators);
	/* Free the normalized declaration set */
	free(declaration_set);

}

struct normalized_declaration_set * create_normalized_declaration_set_from_parser_node(struct parser_node * n, struct normalized_declaration_set * parent_set){
	/*  This function gives us a single list of specifiers that modify each element of a list of declarators */
	struct parser_node * specifier_list = 0;
	struct normalized_declaration_set * declaration_set = malloc(sizeof(struct normalized_declaration_set));

	if(n->type == EXTERNAL_DECLARATION){
		declaration_set->set = n->first_child;
	}else{
		declaration_set->set = n;
	}
	declaration_set->parent_normalized_declaration_set = parent_set;

	switch(declaration_set->set->type){
		case DECLARATION:{
			declaration_set->type = NORMALIZED_DECLARATION;
			break;
		}case FUNCTION_DEFINITION:{
			declaration_set->type = NORMALIZED_FUNCTION_DEFINITION;
			break;
		}case PARAMETER_DECLARATION:{
			declaration_set->type = NORMALIZED_PARAMETER_DECLARATION;
			break;
		}case STRUCT_DECLARATION:{
			declaration_set->type = NORMALIZED_STRUCT_DECLARATION;
			break;
		}case ENUMERATOR_LIST:{
			declaration_set->type = NORMALIZED_ENUMERATOR_LIST;
			break;
		}default:{
			assert(0 && "Unknown node type.");
		}
	}

	if(declaration_set->set->first_child->type == DECLARATION_SPECIFIERS || declaration_set->set->first_child->type == SPECIFIER_QUALIFIER_LIST){
		specifier_list = declaration_set->set->first_child;
	}

	declaration_set->normalized_specifiers = get_normalized_specifier_list(specifier_list);
	declaration_set->normalized_declarators = get_normalized_declarator_list(declaration_set->set);

	return declaration_set;
}

void add_normalized_specifier_to_list(struct parser_node * n, struct struct_normalized_specifier_ptr_list * list){
	struct normalized_specifier * new_p;
	struct parser_node * first;
	struct parser_node * second;
	if(!n){
		return;
	}

	first = n->first_child;
	second = first ? first->next : 0;

	assert(
		n->type == DECLARATION_SPECIFIERS ||
		n->type == SPECIFIER_QUALIFIER_LIST ||
		n->type == TYPE_QUALIFIER_LIST ||
		n->type == TYPE_QUALIFIER_LIST_REST
	);

	new_p = malloc(sizeof(struct normalized_specifier));
	new_p->specifier = copy_parser_node_children_only(first);

	switch(first->type){
		case STORAGE_CLASS_SPECIFIER:{
			new_p->type = NORMALIZED_STORAGE_CLASS_SPECIFIER;
			break;
		}case TYPE_SPECIFIER:{
			new_p->type = NORMALIZED_TYPE_SPECIFIER;
			break;
		}case TYPE_QUALIFIER:{
			new_p->type = NORMALIZED_TYPE_QUALIFIER;
			break;
		}default:{
			assert(0 && "Unknown normalized specifier type.");
		}
	}

	struct_normalized_specifier_ptr_list_add(list, new_p);

	if( /*  Use recursion to add the rest of the specifiers to the list, if there are more */
		second && (
			second->type == DECLARATION_SPECIFIERS ||
			second->type == SPECIFIER_QUALIFIER_LIST || (
				second->type == TYPE_QUALIFIER_LIST_REST &&
				second->first_child->type != EPSILON
			)
		)
	){
		add_normalized_specifier_to_list(second, list);
	}
}



struct struct_normalized_specifier_ptr_list * get_normalized_specifier_list(struct parser_node * n){
	/*
	This function exists to abstract away the differences in iterating over declaration_specifiers,
	specifier_qualifier_lists, and type_qualifier_lists

	declaration_specifiers
		: storage_class_specifier
		| storage_class_specifier declaration_specifiers
		| type_specifier
		| type_specifier declaration_specifiers
		| type_qualifier
		| type_qualifier declaration_specifiers
		;
	specifier_qualifier_list
		: type_specifier specifier_qualifier_list
		| type_specifier
		| type_qualifier specifier_qualifier_list
		| type_qualifier
		;
	type_qualifier_list
		: type_qualifier type_qualifier_list_rest
		;

	type_qualifier_list_rest
		| type_qualifier type_qualifier_list_rest
		| epsilon
		;
	These all get flattened out to a list of [ storage_class_specifier | type_specifier | type_qualifier ]
	*/
	struct struct_normalized_specifier_ptr_list * rtn = malloc(sizeof(struct struct_normalized_specifier_ptr_list));
	struct_normalized_specifier_ptr_list_create(rtn);
	add_normalized_specifier_to_list(n, rtn);
	return rtn;
}

void add_normalized_declarator_to_list(struct parser_node * n, struct struct_normalized_declarator_ptr_list * list){
	struct normalized_declarator * new_p;

	if(!n || n->first_child->type == EPSILON){
		return;
	}

	new_p = malloc(sizeof(struct normalized_declarator));

	switch(n->type){
		case ENUMERATOR_LIST:{
			new_p->type = NORMALIZED_ENUMERATOR;
			new_p->declarator = copy_parser_node_children_only(n->first_child);
			struct_normalized_declarator_ptr_list_add(list, new_p);
			add_normalized_declarator_to_list(n->first_child->next, list);
			break;
		}case ENUMERATOR_LIST_REST:{
			new_p->type = NORMALIZED_ENUMERATOR;
			new_p->declarator = copy_parser_node_children_only(n->first_child->next);
			struct_normalized_declarator_ptr_list_add(list, new_p);
			add_normalized_declarator_to_list(n->first_child->next->next, list);
			break;
		}case INIT_DECLARATOR_LIST:{
			new_p->type = NORMALIZED_INIT_DECLARATOR;
			new_p->declarator = copy_parser_node_children_only(n->first_child);
			struct_normalized_declarator_ptr_list_add(list, new_p);
			add_normalized_declarator_to_list(n->first_child->next, list);
			break;
		}case INIT_DECLARATOR_LIST_REST:{
			new_p->type = NORMALIZED_INIT_DECLARATOR;
			new_p->declarator = copy_parser_node_children_only(n->first_child->next);
			struct_normalized_declarator_ptr_list_add(list, new_p);
			add_normalized_declarator_to_list(n->first_child->next->next, list);
			break;
		}case STRUCT_DECLARATOR_LIST:{
			new_p->type = NORMALIZED_STRUCT_DECLARATOR;
			new_p->declarator = copy_parser_node_children_only(n->first_child);
			struct_normalized_declarator_ptr_list_add(list, new_p);
			add_normalized_declarator_to_list(n->first_child->next, list);
			break;
		}case STRUCT_DECLARATOR_LIST_REST:{
			new_p->type = NORMALIZED_STRUCT_DECLARATOR;
			new_p->declarator = copy_parser_node_children_only(n->first_child->next);
			struct_normalized_declarator_ptr_list_add(list, new_p);
			add_normalized_declarator_to_list(n->first_child->next->next, list);
			break;
		}case PARAMETER_DECLARATION:{
			if(n->first_child->next){
				switch(n->first_child->next->type){
					case DECLARATOR:{
						new_p->type = NORMALIZED_DECLARATOR;
						break;
					}case ABSTRACT_DECLARATOR:{
						new_p->type = NORMALIZED_ABSTRACT_DECLARATOR;
						break;
					}default:{
						assert(0 && "Unknown declarator.");
					}
				}
				new_p->declarator = copy_parser_node_children_only(n->first_child->next);
				assert(new_p->declarator->type == DECLARATOR || new_p->declarator->type == ABSTRACT_DECLARATOR);
				struct_normalized_declarator_ptr_list_add(list, new_p);
			}else{
				free(new_p); /*  No declarator */
			}
			break;
		}case FUNCTION_DEFINITION:{
			new_p->type = NORMALIZED_DECLARATOR;
			if(n->first_child->type == DECLARATOR){
				new_p->declarator = copy_parser_node_children_only(n->first_child);
			}else if(n->first_child->next->type == DECLARATOR){
				new_p->declarator = copy_parser_node_children_only(n->first_child->next);
			}else{
				assert(0 && "Unknown function definition.");
			}
			assert(new_p->declarator->type == DECLARATOR);
			struct_normalized_declarator_ptr_list_add(list, new_p);
			break;
		}case DECLARATION:{
			free(new_p); /*  The declarator (if there is one) is deeper in the recursion */
			if(n->first_child->next && n->first_child->next->type == INIT_DECLARATOR_LIST){
				add_normalized_declarator_to_list(n->first_child->next, list);
			}
			break;
		}case STRUCT_DECLARATION:{
			free(new_p); /*  The declarator (if there is one) is deeper in the recursion */
			if(n->first_child->next && n->first_child->next->type == STRUCT_DECLARATOR_LIST){
				add_normalized_declarator_to_list(n->first_child->next, list);
			}
			break;
		}default:{
			assert(0 && "Unknown normalized declarator type.");
		}
	}
}

struct struct_normalized_declarator_ptr_list * get_normalized_declarator_list(struct parser_node * n){
	/*
	This function exists to abstract away the differences in iterating over init_declarator_lists,
	struct_declaration_lists

	declaration
		: declaration_specifiers ';'
		| declaration_specifiers init_declarator_list ';'
		;
	init_declarator_list
		: init_declarator init_declarator_list_rest
		;
	init_declarator_list_rest
		: ',' init_declarator init_declarator_list_rest
		| epsilon
		;
	struct_declarator_list
		: struct_declarator struct_declarator_list_rest
		;
	struct_declarator_list_rest
		| ',' struct_declarator struct_declarator_list_rest
		| epsilon
		;
	parameter_declaration
		: declaration_specifiers declarator
		| declaration_specifiers abstract_declarator
		| declaration_specifiers
		;
	function_definition
		: declaration_specifiers declarator declaration_list compound_statement
		| declaration_specifiers declarator compound_statement
		| declarator declaration_list compound_statement
		| declarator compound_statement
		;

	These all get flattened out to a list of [ declarator | abstract_declarator | struct_declarator | ... ]
	*/
	struct struct_normalized_declarator_ptr_list * rtn = malloc(sizeof(struct struct_normalized_declarator_ptr_list));
	struct_normalized_declarator_ptr_list_create(rtn);
	add_normalized_declarator_to_list(n, rtn);
	return rtn;
}

void stack_operation(const char* format, ...){
	va_list arglist;
	va_start( arglist, format );
	/*
	if(SHOW_STACK_PROGRESS){
		vprintf( format, arglist );
	}
	*/
	va_end( arglist );
}

void parser_progress(const char* format, ...){
	va_list arglist;
	va_start( arglist, format );
	/*
	if(SHOW_PARSER_PROGRESS){
		printf(";");
		vprintf( format, arglist );
	}
	*/
	va_end( arglist );
}



struct scope_level * create_empty_scope(struct scope_level * parent){
	struct scope_level * s = malloc(sizeof(struct scope_level));
	s->num_sub_scopes = 0;
	s->scopes = 0;
	s->current_function = 0;
	s->first_local_object = 0;
	s->parent_scope = parent;
	struct_namespace_object_ptr_list_create(&s->tag_namespace);
	struct_namespace_object_ptr_list_create(&s->label_namespace);
	struct_namespace_object_ptr_list_create(&s->identifier_namespace);
	unsigned_char_ptr_list_create(&s->evaluate_labels);
	unsigned_char_ptr_list_create(&s->end_labels);
	return s;
}

void destroy_empty_scope(struct scope_level * s){
	unsigned_char_ptr_list_destroy(&s->evaluate_labels);
	unsigned_char_ptr_list_destroy(&s->end_labels);
	struct_namespace_object_ptr_list_destroy(&s->tag_namespace);
	struct_namespace_object_ptr_list_destroy(&s->label_namespace);
	struct_namespace_object_ptr_list_destroy(&s->identifier_namespace);
	free(s);
}

void add_scope(struct scope_level * scope){
	unsigned int old_num_scopes = scope->num_sub_scopes;
	unsigned int new_num_scopes = old_num_scopes + 1;
	scope->scopes = realloc(scope->scopes, sizeof(struct scope_level *) * new_num_scopes);
	scope->scopes[old_num_scopes] = create_empty_scope(scope);
	scope->num_sub_scopes = new_num_scopes;
}

void remove_scope(struct scope_level * scope){
	unsigned int old_num_scopes = scope->num_sub_scopes;
	unsigned int new_num_scopes = old_num_scopes - 1;
	assert(scope->num_sub_scopes);
	destroy_empty_scope(scope->scopes[new_num_scopes]);
	scope->scopes = realloc(scope->scopes, sizeof(struct scope_level *) * new_num_scopes);
	scope->num_sub_scopes = new_num_scopes;
}

void manage_new_scope(struct scope_level * scope, unsigned int levels_to_go, enum add_or_remove add_or_remove){
	if(levels_to_go){
		manage_new_scope((scope->scopes)[scope->num_sub_scopes - 1], levels_to_go - 1, add_or_remove);
	}else{
		if(add_or_remove == ADD){
			add_scope(scope);
		}else if(add_or_remove == REMOVE){
			remove_scope(scope);
		}else{
			assert(0);
		}
	}
}

void add_terminal_tokens_to_buffer_h2(struct unsigned_char_list * buffer, struct parser_node * n, unsigned int * num_added){
	while(n){
		add_terminal_tokens_to_buffer_h1(buffer, n, num_added);
		if(n->type == TERMINAL){
			if(num_added){
				buffered_printf(buffer, " ");
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

struct namespace_object * get_namespace_object_from_scope_namespace_using_string(enum scope_type scope_type, struct struct_namespace_object_ptr_list * name, unsigned char * ident){
	unsigned int i;
	for(i = 0; i < struct_namespace_object_ptr_list_size(name); i++){
		struct namespace_object * namespace_object = struct_namespace_object_ptr_list_get(name, i);
		/*  They should all have the same label, so just choose the first one. */
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&namespace_object->elements, 0);
		struct c_lexer_token * compare_token;
		unsigned char * compare_string;
		switch(scope_type){
			case IDENTIFIER_NAMESPACE:{
				compare_token = get_identifier_token_from_normalized_declarator(element->normalized_declarator);
				break;
			}case TAG_NAMESPACE:{
				compare_token = get_struct_or_union_or_enum_tag_token(get_struct_or_union_or_enum_specifier(element->normalized_specifiers));
				break;
			}default:{
				assert(0 && "Not implemented.");
			}
		}
		assert(compare_token);
		compare_string = copy_string(compare_token->first_byte, compare_token->last_byte);
		if(!strcmp((const char *)ident, (const char *)compare_string)){
			free(compare_string);
			return namespace_object;
		}
		free(compare_string);
	}
	return 0;
}

struct namespace_object * get_namespace_object_from_closest_namespace(unsigned char * ident, enum scope_type scope_type, struct scope_level * start_scope, unsigned int require_definition){
	struct scope_level * current_scope = start_scope;
	while(current_scope){
		struct namespace_object * current_obj;
		struct struct_namespace_object_ptr_list * scope_namespace;
		switch (scope_type){
			case IDENTIFIER_NAMESPACE:{
				scope_namespace = &current_scope->identifier_namespace;
				break;
			}case TAG_NAMESPACE:{
				scope_namespace = &current_scope->tag_namespace;
				break;
			}default:{
				assert(0 && "Not covered.");
			}
		}
		current_obj = get_namespace_object_from_scope_namespace_using_string(scope_type, scope_namespace, ident);
		/* Sometimes we need to make sure that we get definition, not just a declaration  */
		if(current_obj && (!require_definition || contains_struct_or_union_or_enum_definition(current_obj))){
			return current_obj;
		}
		current_scope = current_scope->parent_scope;
	}
	return 0;
}

void descend_scope(struct scope_level ** scope, unsigned int levels_to_go){
	if(levels_to_go){
		/* Descend the most recently created scope */
		*scope = (*scope)->scopes[(*scope)->num_sub_scopes - 1];
		descend_scope(scope, levels_to_go - 1);
	}
}

struct namespace_modification * create_namespace_modification(struct scope_level * scope, struct struct_namespace_object_ptr_list * name, struct normalized_declaration_element * element, enum object_location object_location, struct namespace_object * obj){
	struct namespace_modification * modification = malloc(sizeof(struct namespace_modification));
	modification->new_element = element;
	modification->object = obj;
	modification->object_location = object_location;
	modification->scope_namespace = name;
	modification->scope_level = scope;
	return modification;
}

struct namespace_object * do_namespace_modification(struct namespace_modification * modification){
	struct struct_namespace_object_ptr_list * name = modification->scope_namespace;
	struct namespace_object * parent_obj = modification->object;
	if(!parent_obj){
		parent_obj = malloc(sizeof(struct namespace_object));
		parent_obj->object_location = modification->object_location;
		parent_obj->scope_level = modification->scope_level;
		parent_obj->offset = 0;
		parent_obj->first_load = 1;
		parent_obj->previous = 0;
		parent_obj->next = 0;
		parent_obj->children = malloc(sizeof(struct struct_namespace_object_ptr_list));
		struct_namespace_object_ptr_list_create(parent_obj->children);
		struct_namespace_object_ptr_list_add(name, parent_obj);
		struct_normalized_declaration_element_ptr_list_create(&parent_obj->elements);
	}
	modification->object = parent_obj;
	/*  Remember which object we added it to, so we can remove that element when we destroy the state */
	struct_normalized_declaration_element_ptr_list_add(&parent_obj->elements, modification->new_element);

	return parent_obj;
}

void undo_namespace_modification(struct namespace_modification * modification){
	struct struct_namespace_object_ptr_list * name = modification->scope_namespace;
	struct struct_normalized_declaration_element_ptr_list * elements = &modification->object->elements;
	assert(struct_normalized_declaration_element_ptr_list_size(elements));
	/*  Always remove one element */
	struct_normalized_declaration_element_ptr_list_pop(elements);
	if(struct_normalized_declaration_element_ptr_list_size(elements) == 0){
		/*  If that was the last element for that namespace object, delete the namespace object */
		struct namespace_object * o = struct_namespace_object_ptr_list_pop(name);
		assert(o == modification->object); /*  The function should work as long as this assumption is correct. */
		struct_normalized_declaration_element_ptr_list_destroy(elements);
		struct_namespace_object_ptr_list_destroy(o->children);
		free(o->children);
		free(o);
	}
	free(modification);
}

void declare_enum_constants(struct parser_state * state, struct parser_node * n, struct normalized_declaration_set * set){
	if(n && n->type == ENUM_SPECIFIER){
		struct parser_node * enumerator_list = get_enumerator_list(n);
		if(enumerator_list){
			manage_generic_declaration(state, enumerator_list, set, 0, ENUM_IDENTIFIER, 0, 0);
		}
	}
}

struct namespace_object * manage_declaration_element(struct parser_state * state, struct normalized_declaration_element * element, enum scope_type scope_type, enum object_location object_location, struct struct_namespace_object_ptr_list * forced_namespace){
	struct scope_level * scope;
	struct struct_namespace_object_ptr_list * name;
	struct namespace_modification * modification;
	struct namespace_object * obj;
	struct namespace_object * o;
	struct c_lexer_token * identifier_token;
	unsigned char * identifier_string;
	scope = state->top_scope;
	descend_scope(&scope, state->current_scope_depth);

	switch(scope_type){
		case IDENTIFIER_NAMESPACE:{
			identifier_token = get_identifier_token_from_normalized_declarator(element->normalized_declarator);
			assert(identifier_token);
			/*  For structure elements, declare the identifiers inside the namespace of the right tag */
			name = forced_namespace ? forced_namespace : &scope->identifier_namespace;
			break;
		}case TAG_NAMESPACE:{
			struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(element->normalized_specifiers);
			declare_enum_constants(state, struct_or_union_or_enum_specifier, element->normalized_declaration_set);
			identifier_token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
			assert(identifier_token);
			name = &scope->tag_namespace;
			break;
		}case LABEL_NAMESPACE:{
			identifier_token = 0;
			name = &scope->label_namespace;
			break;
		}default:{
			assert(0);
			identifier_token = 0;
		}
	}

	/*  Check if there was already something that was declared with this identifier */
	identifier_string = copy_string(identifier_token->first_byte, identifier_token->last_byte);
	obj = get_namespace_object_from_scope_namespace_using_string(scope_type, name, identifier_string);
	free(identifier_string);

	modification = create_namespace_modification(scope, name, element, object_location, obj);
	modification->scope_type = scope_type;
	o = do_namespace_modification(modification);
	push_operation(state, NAMESPACE_MODIFICATION, modification);
	return o;
}

struct first_and_last_namespace_object manage_declaration_elements(struct parser_state * state, struct struct_normalized_declaration_element_ptr_list * elements, enum object_location object_location, struct namespace_object * previous_obj, struct struct_namespace_object_ptr_list * forced_namespace){
	/*  Forced namespace is used when we're declaring identifiers that are struct members.  These identifiers are forced
            into the namespace of the children of the namespace object of the given struct (and not the current scope).
        */
	unsigned int i;
	struct first_and_last_namespace_object fl;
	fl.first = 0;
	fl.last = 0;
	for(i = 0; i < struct_normalized_declaration_element_ptr_list_size(elements); i++){
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(elements, i);
		struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(element->normalized_specifiers);
		if(get_identifier_token_from_normalized_declarator(element->normalized_declarator)){
			/*  We can only add something to the scope if it has an identifier */
			struct namespace_object * next = manage_declaration_element(state, element, IDENTIFIER_NAMESPACE, object_location, forced_namespace);
			if(previous_obj){
				struct namespace_object_change * change_next = malloc(sizeof(struct namespace_object_change));
				change_next->target = previous_obj;
				change_next->old_obj = previous_obj->next;
				change_next->new_obj = next;
				push_operation(state, SET_NEXT_NAMESPACE_OBJECT, change_next);
			}
			if(next){
				struct namespace_object_change * change_previous = malloc(sizeof(struct namespace_object_change));
				change_previous->target = next;
				change_previous->old_obj = next->previous;
				change_previous->new_obj = previous_obj;
				push_operation(state, SET_PREVIOUS_NAMESPACE_OBJECT, change_previous);
				previous_obj = next;
				fl.last = next;
			}
			if(fl.first == 0){
				fl.first = next;
			}
		}
		/*  The same declaration element could be declaraing a structure, union or enum too */
		if(get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier)){
			int its_a_union;
			int its_a_struct;
			struct namespace_object * new_obj = manage_declaration_element(state, element, TAG_NAMESPACE, object_location, 0);
			its_a_union = is_union(struct_or_union_or_enum_specifier);
			its_a_struct = is_struct(struct_or_union_or_enum_specifier);
			/*  Declare all structure members */
			/*  Make sure we don't declare these things twice if the struct declaration also declares an identifier */
			if(its_a_struct || its_a_union){
				struct parser_node * declaration_list = get_struct_declaration_list(struct_or_union_or_enum_specifier);
				struct parser_node * current_declaration;
				if(declaration_list){
					current_declaration = declaration_list->first_child;
					do{
						manage_generic_declaration(state, current_declaration, element->normalized_declaration_set, 0, LOCATION_STRUCT, 0, new_obj->children);
						current_declaration = current_declaration->next->first_child;
					}while(current_declaration->type != EPSILON);
				}
			}
		}
	}
	return fl;
}

struct constant_description * find_constant(struct parser_state * state, unsigned char * str){
	unsigned int i;
	/*  Search for the first constant with an identical literal string representation */
	for(i = 0; i < state->num_constants; i++){
		if(!(strcmp((const char*)state->constants[i]->str, (const char*)str))){
			return state->constants[i];
		}
	}
	return 0;
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
		unsigned int digit = get_hex_digit_value(str[i-1]);
		rtn += digit * base;
		base = base * 16;
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
		unsigned int digit = (str[i-1] - '0');
		rtn += digit * base;
		base = base * 10;
		i--;
	}
	return rtn;
}

unsigned char * convert_character_constant(unsigned char * str, unsigned char * result){
	/*  Return the address of the start of the next character, and put the current char in result */
	if(str[0] == '\\'){
		switch(str[1]){
			/*  TODO:  This does not cover all character escape codes. */
			case '\\':{
				*result = '\\';
				return &str[2];
			}case 'n':{
				*result = '\n';
				return &str[2];
			}case 't':{
				*result = '\t';
				return &str[2];
			}case '\'':{
				*result = '\'';
				return &str[2];
			}case '0':{
				*result = '\0';
				return &str[2];
			}default:{
				assert(0 && "Unknown character constant.");
				return 0;
			}
		}
	}
	*result = str[0];
	return &str[1];
}


unsigned char * convert_string_literal(unsigned char * str){
	unsigned int max_len = (unsigned int)strlen((char *)str) + 1;
	unsigned char * max_buffer = calloc(max_len, 1); /* Always larger than or equal to the necessary size */
	unsigned char * next_char = str;
	unsigned int chars_converted = 0;
	unsigned char * last_quote = &str[max_len -2]; /* The last double quote */
	while(last_quote != next_char){
		next_char = convert_character_constant(next_char, &max_buffer[chars_converted]);
		chars_converted++;
	}
	max_buffer[chars_converted] = 0; /* Null termination */
	return max_buffer;
}

void manage_constant(struct parser_state * state, struct parser_node * n, enum add_or_remove add_or_remove){
	if(add_or_remove == ADD){
		/*  TODO:  don't create multiple constant entries for the same constant */
		/*  TODO:  don't assume everything is 4 bytes in size */
		state->constants = realloc(state->constants, sizeof(struct constant_description *) * (state->num_constants + 1));
		state->constants[state->num_constants] = malloc(sizeof(struct constant_description));
		/*  Copy the literal data that represents the constant so we can look for it */
		state->constants[state->num_constants]->str = copy_string(n->c_lexer_token->first_byte, n->c_lexer_token->last_byte);
		switch(n->c_lexer_token->type){
			case CONSTANT_HEX:{
				state->constants[state->num_constants]->native_data = calloc(4,1);
				*(state->constants[state->num_constants]->native_data) = convert_hexadecimal_constant(state->constants[state->num_constants]->str);
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
				state->constants[state->num_constants]->native_data = calloc(4,1);
				*(state->constants[state->num_constants]->native_data) = convert_decimal_constant(state->constants[state->num_constants]->str);
				break;
			}case CONSTANT_CHARACTER:{
				state->constants[state->num_constants]->native_data = calloc(4,1);
				convert_character_constant(&state->constants[state->num_constants]->str[1], (unsigned char *)(&state->constants[state->num_constants]->native_data[0]));
				((unsigned char *)state->constants[state->num_constants]->native_data)[1] = 0;
				((unsigned char *)state->constants[state->num_constants]->native_data)[2] = 0;
				((unsigned char *)state->constants[state->num_constants]->native_data)[3] = 0;
				break;
			}case STRING_LITERAL:{
				unsigned char * raw_string_data = convert_string_literal(&state->constants[state->num_constants]->str[1]); /* offset 1 to jump over the first quote */
				/* TODO:  This will not work properly for character constants with multiple embeded string literals because of the strlen */
				state->constants[state->num_constants]->native_data = calloc(sizeof(unsigned int),1);
				*(state->constants[state->num_constants]->native_data) = char_ptr_list_size(&state->string_literals); /* This is the nth string literal */
				char_ptr_list_add(&state->string_literals, (char *)raw_string_data);
				break;
			}default:{
				assert(0 && "Unknown conversion.");
			}
		}
		state->num_constants = state->num_constants + 1;
	}else if(add_or_remove == REMOVE){
		assert(state->num_constants);
		free(state->constants[state->num_constants -1]->native_data);
		free(state->constants[state->num_constants -1]->str);
		free(state->constants[state->num_constants -1]);
		state->constants = realloc(state->constants, sizeof(struct constant_description *) * (state->num_constants - 1));
		state->num_constants = state->num_constants - 1;
		switch(n->c_lexer_token->type){
			case CONSTANT_HEX:{
				break;
			}case CONSTANT_EXPONENT:{
				break;
			}case CONSTANT_FLOAT_SMALL:{
				break;
			}case CONSTANT_FLOAT_LARGE:{
				break;
			}case CONSTANT_DECIMAL:{
				break;
			}case CONSTANT_CHARACTER:{
				break;
			}case STRING_LITERAL:{
				/*  If backtacking occurs, we need to get rid of the last string literal pointer we added */
				unsigned int size = char_ptr_list_size(&state->string_literals);
				char * c = char_ptr_list_get(&state->string_literals, size - 1);
				char_ptr_list_remove_all(&state->string_literals, c);
				free(c);
				break;
			}default:{
				assert(0 && "Unknown option.");
			}
		}
	}else{
		assert(0 && "Unknown option.");
	}
}
	

void * push_operation(struct parser_state * parser_state, enum parser_operation_type t, void * data){
	struct parser_operation new_operation;
	new_operation.type = t;
	new_operation.data = t == ADVANCE_PARSER_POSITION ? memory_pooler_malloc(g_parser_node_pool) : data;
	struct_parser_operation_stack_push(&parser_state->operation_stack, new_operation);
	switch(t){
		case ADVANCE_TOKEN_POSITION:{
			parser_state->tokens_position = parser_state->tokens_position + 1;
			return 0;
		}case ADVANCE_PARSER_POSITION:{
			*((struct parser_node *)new_operation.data) = *((struct parser_node *)data);
			return new_operation.data;
		}case INCREMENT_SCOPE_DEPTH:{
			manage_new_scope(parser_state->top_scope, parser_state->current_scope_depth, ADD);
			parser_state->current_scope_depth = parser_state->current_scope_depth + 1;
			return 0;
		}case DECREMENT_SCOPE_DEPTH:{
			assert(parser_state->current_scope_depth > 0);
			parser_state->current_scope_depth = parser_state->current_scope_depth - 1;
			return 0;
		}case PROCESS_CONSTANT:{
			manage_constant(parser_state, data, ADD);
			return 0;
		}case SAVE_DECLARATION_ELEMENTS:{
			return 0;
		}case NAMESPACE_MODIFICATION:{
			return 0;
		}case CREATE_PARSER_NODE_TREE:{
			return 0;
		}case INCREMENT_NEWLINE_COUNT:{
			parser_state->line_number = parser_state->line_number + 1;
			return 0;
		}case SET_NEXT_NAMESPACE_OBJECT:{
			set_next_namespace_object((struct namespace_object_change *)data);
			return 0;
		}case SET_PREVIOUS_NAMESPACE_OBJECT:{
			set_previous_namespace_object((struct namespace_object_change *)data);
			return 0;
		}case SET_CURRENT_FUNCTION:{
			set_current_function((struct current_function_change *)data);
			return 0;
		}default:{
			assert(0 && "This should never happen.\n");
			return 0;
		}
	}
}

void pop_operation(struct parser_state * parser_state){
	struct parser_operation poped_operation;
	if(struct_parser_operation_stack_size(&parser_state->operation_stack) == 0){
		assert(0 && "Trying to pop empty operation stack.\n");
		return;
	}
	poped_operation = struct_parser_operation_stack_pop(&parser_state->operation_stack);
	
	switch(poped_operation.type){
		case ADVANCE_TOKEN_POSITION:{
			parser_state->tokens_position = parser_state->tokens_position - 1;
			break;
		}case ADVANCE_PARSER_POSITION:{
			memory_pooler_free(g_parser_node_pool, poped_operation.data);
			break;
		}case INCREMENT_SCOPE_DEPTH:{
			parser_state->current_scope_depth = parser_state->current_scope_depth - 1;
			manage_new_scope(parser_state->top_scope, parser_state->current_scope_depth, REMOVE);
			break;
		}case DECREMENT_SCOPE_DEPTH:{
			parser_state->current_scope_depth = parser_state->current_scope_depth + 1;
			break;
		}case PROCESS_CONSTANT:{
			manage_constant(parser_state, poped_operation.data, REMOVE);
			break;
		}case SAVE_DECLARATION_ELEMENTS:{
			destroy_normalized_declaration_element_list((struct struct_normalized_declaration_element_ptr_list *)(poped_operation.data));
			break;
		}case NAMESPACE_MODIFICATION:{
			undo_namespace_modification((struct namespace_modification *)(poped_operation.data));
			break;
		}case CREATE_PARSER_NODE_TREE:{
			destroy_parser_node_tree_and_c_lexer_tokens((struct parser_node *)(poped_operation.data));
			break;
		}case INCREMENT_NEWLINE_COUNT:{
			parser_state->line_number = parser_state->line_number - 1;
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

void backtrack(struct parser_state * parser_state, unsigned int target){
	while(struct_parser_operation_stack_size(&parser_state->operation_stack) != target){
		pop_operation(parser_state);
	}
}

struct parser_node * create_parser_node(struct parser_state * parser_state, struct parser_node * n, struct parser_node * f, struct c_lexer_token * l, enum node_type t){
	struct parser_node new_node;
	new_node.next = n;
	new_node.first_child = f;
	new_node.c_lexer_token = l;
	new_node.type = t;
	return push_operation(parser_state, ADVANCE_PARSER_POSITION, &new_node);
}

struct parser_node * p_accept(enum c_token_type t, struct parser_state * parser_state){
	unsigned int num_tokens = struct_c_lexer_token_ptr_list_size(&parser_state->c_lexer_state->tokens);
	struct c_lexer_token ** tokens = struct_c_lexer_token_ptr_list_data(&parser_state->c_lexer_state->tokens);
	while(
		(parser_state->tokens_position < num_tokens) &&
		(
			tokens[parser_state->tokens_position]->type == SPACE ||
			tokens[parser_state->tokens_position]->type == NEWLINE ||
			tokens[parser_state->tokens_position]->type == COMMENT
		)
	){
		if(tokens[parser_state->tokens_position]->type == NEWLINE){
			push_operation(parser_state, INCREMENT_NEWLINE_COUNT, 0);
		}
		push_operation(parser_state, ADVANCE_TOKEN_POSITION, tokens[parser_state->tokens_position]);
	}

	if(parser_state->tokens_position < num_tokens){
		if(tokens[parser_state->tokens_position]->type == t){
			struct c_lexer_token * lt;
			lt = tokens[parser_state->tokens_position];
		        push_operation(parser_state, ADVANCE_TOKEN_POSITION, tokens[parser_state->tokens_position]);
			return create_parser_node(parser_state, 0, 0, lt, TERMINAL);
		}
	}
	return 0;
}

struct parser_node * expect(enum c_token_type t, struct parser_state * parser_state){
	struct parser_node * l = p_accept(t,parser_state);
	if(l){
		return l;
	}
	assert(0 && "Error, expected ....\n");
	return 0;
}

struct parser_node * primary_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build primary_expression\n");
	if((n1 = p_accept(IDENTIFIER, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, PRIMARY_EXPRESSION);
	}else if(
		(n1 = p_accept(CONSTANT_HEX, parser_state)) ||
		(n1 = p_accept(CONSTANT_EXPONENT, parser_state)) ||
		(n1 = p_accept(CONSTANT_FLOAT_SMALL, parser_state)) ||
		(n1 = p_accept(CONSTANT_FLOAT_LARGE, parser_state)) ||
		(n1 = p_accept(CONSTANT_DECIMAL, parser_state)) ||
		(n1 = p_accept(CONSTANT_CHARACTER, parser_state))
	){
		push_operation(parser_state, PROCESS_CONSTANT, n1);
		return create_parser_node(parser_state, 0, n1, 0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(STRING_LITERAL, parser_state))){
		push_operation(parser_state, PROCESS_CONSTANT, n1);
		return create_parser_node(parser_state, 0, n1, 0, PRIMARY_EXPRESSION);
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, parser_state))){
		if((n1->next = expression(parser_state))) {
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, PRIMARY_EXPRESSION);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR\n");
				return 0;
			}
		}else{
			parser_progress("Expected EXPRESSION...backtracking...\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected primary expression\n");
		return 0;
	}
}

struct parser_node * make_epsilon(struct parser_state * parser_state){
	parser_progress("Attempting to build epsilon\n");
	return create_parser_node(parser_state, 0, 0, 0, EPSILON);
}



struct parser_node * argument_expression_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build argument_expression_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, parser_state))){
		if((n1->next = assignment_expression(parser_state))){
			if((n1->next->next = argument_expression_list_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, ARGUMENT_EXPRESSION_LIST_REST);
			}else{
				assert(0 && "Expected a ARGUMENT_EXPRESSION_LIST_REST.\n");
				return 0;
			}
		}else{
			assert(0 && "Expected a assignment_expression.\n");
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, ARGUMENT_EXPRESSION_LIST_REST);
	}
}

struct parser_node * argument_expression_list(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build argument_expression_list\n");
	if((n1 = assignment_expression(parser_state))){
		if((n1->next = argument_expression_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, ARGUMENT_EXPRESSION_LIST);
		}else{
			assert(0 && "Expected a ARGUMENT_EXPRESSION_LIST.\n");
			return 0;
		}
	}else{
		parser_progress("Expected a assignment_expression.\n");
		return 0;
	}
}

struct parser_node * postfix_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build postfix_expression_rest\n");
	if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, parser_state))){
		if((n1->next = expression(parser_state))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, parser_state))){
				if((n1->next->next->next = postfix_expression_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, POSTFIX_EXPRESSION_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected postfix_expression_rest\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_SQUARE_BRACKET_CHAR\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression\n");
			return 0;
		}
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, parser_state))){
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
			if((n1->next->next = postfix_expression_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, POSTFIX_EXPRESSION_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected postfix_expression_rest.\n");
				return 0;
			}
		}else if((n1->next = argument_expression_list(parser_state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
				if((n1->next->next->next = postfix_expression_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, POSTFIX_EXPRESSION_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected postfix_expression_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR or argument_expression_list.\n");
			return 0;
		}
	}else if((n1 = p_accept(DOT_CHAR, parser_state))){
		if((n1->next = p_accept(IDENTIFIER, parser_state))){
			if((n1->next->next = postfix_expression_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, POSTFIX_EXPRESSION_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected postfix_expression_rest.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected IDENTIFIER.\n");
			return 0;
		}
	}else if((n1 = p_accept(PTR_OP, parser_state))){
		if((n1->next = p_accept(IDENTIFIER, parser_state))){
			if((n1->next->next = postfix_expression_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, POSTFIX_EXPRESSION_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected postfix_expression_rest.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected IDENTIFIER.\n");
			return 0;
		}
	}else if((n1 = p_accept(INC_OP, parser_state))){
		if((n1->next = postfix_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, POSTFIX_EXPRESSION_REST);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected postfix_expression_rest.\n");
			return 0;
		}
	}else if((n1 = p_accept(DEC_OP, parser_state))){
		if((n1->next = postfix_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, POSTFIX_EXPRESSION_REST);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected postfix_expression_rest.\n");
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, POSTFIX_EXPRESSION_REST);
	}
}

struct parser_node * postfix_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build postfix_expression\n");
	if((n1 = primary_expression(parser_state))){
		if((n1->next = postfix_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, POSTFIX_EXPRESSION);
		}else{
			assert(0 && "Expected postfix_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected postfix expression.\n");
		return 0;
	}
}


struct parser_node * unary_operator(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build unary_operator\n");
	if((n1 = p_accept(AMPERSAND_CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, UNARY_OPERATOR);
	}else if((n1 = p_accept(MULTIPLY_CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, UNARY_OPERATOR);
	}else if((n1 = p_accept(PLUS_CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, UNARY_OPERATOR);
	}else if((n1 = p_accept(MINUS_CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, UNARY_OPERATOR);
	}else if((n1 = p_accept(TILDE_CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, UNARY_OPERATOR);
	}else if((n1 = p_accept(EXCLAMATION_MARK_CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, UNARY_OPERATOR);
	}else{
		parser_progress("Expected unary operator.\n");
		return 0;
	}
}

struct parser_node * unary_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build unary_expression\n");
	if((n1 = postfix_expression(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, UNARY_EXPRESSION);
	}else if((n1 = p_accept(INC_OP, parser_state))){
		if((n1->next = unary_expression(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, UNARY_EXPRESSION);
		}else{
			assert(0 && "Expected unary expression.\n");
			return 0;
		}
	}else if((n1 = p_accept(DEC_OP, parser_state))){
		if((n1->next = unary_expression(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, UNARY_EXPRESSION);
		}else{
			assert(0 && "Expected unary expression.\n");
			return 0;
		}
	}else if((n1 = unary_operator(parser_state))){
		if((n1->next = cast_expression(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, UNARY_EXPRESSION);
		}else{
			assert(0 && "Expected cast_expression.\n");
			return 0;
		}
	}else if((n1 = p_accept(SIZEOF, parser_state))){
		if((n1->next = unary_expression(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, UNARY_EXPRESSION);
		}else if((n1->next = p_accept(OPEN_PAREN_CHAR, parser_state))){
			if((n1->next->next = type_name(parser_state))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, UNARY_EXPRESSION);
				}else{
					assert(0 && "Expected CLOSE_PAREN_CHAR.\n");
					return 0;
				}
			}else{
				assert(0 && "Expected IDENTIFIER.\n");
				return 0;
			}
		}else{
			assert(0 && "Expected OPEN_PAREN_CHAR or uary expression.\n");
			return 0;
		}
	}else{
		parser_progress("Expected unary expression.\n");
		return 0;
	}
}

struct parser_node * cast_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build cast_expression\n");
	if((n1 = unary_expression(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, CAST_EXPRESSION);
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, parser_state))){
		if((n1->next = type_name(parser_state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
				if((n1->next->next->next = cast_expression(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, CAST_EXPRESSION);
				}else{
					assert(0 && "Fatal. Expected cast_expression\n");
				}
			}else{
				assert(0 && "Fatal. Expected CLOSE_PAREN_CHAR\n");
			}
		}else{
			assert(0 && "Fatal. Expected type_name\n");
		}
	}else{
		parser_progress("Expected unary expression.\n");
		return 0;
	}
	return 0;
}

struct parser_node * multiplicative_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build multiplicative_expression_rest\n");
	if(!((n1 = p_accept(MULTIPLY_CHAR, parser_state)) || (n1 = p_accept(DIVIDE_CHAR, parser_state)) || (n1 = p_accept(PERCENT_CHAR, parser_state)))){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, MULTIPLICATIVE_EXPRESSION_REST);
	}
	if(!(n1->next = cast_expression(parser_state))){
		assert(0 && "Expected cast expression.\n");
		return 0;
	}
	if(!(n1->next->next = multiplicative_expression_rest(parser_state))){
		assert(0 && "Expected multiplicative_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, MULTIPLICATIVE_EXPRESSION_REST);
}

struct parser_node * multiplicative_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build multiplicative_expression\n");
	if((n1 = cast_expression(parser_state))){
		if((n1->next = multiplicative_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, MULTIPLICATIVE_EXPRESSION);
		}else{
			assert(0 && "Expected multiplicative_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected cast expression.\n");
		return 0;
	}
}

struct parser_node * additive_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build additive_expression_rest\n");
	if(!((n1 = p_accept(PLUS_CHAR, parser_state)) || (n1 = p_accept(MINUS_CHAR, parser_state)))){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, ADDITIVE_EXPRESSION_REST);
	}
	if(!(n1->next = multiplicative_expression(parser_state))){
		assert(0 && "Expected multiplicative_expression.\n");
		return 0;
	}
	if(!(n1->next->next = additive_expression_rest(parser_state))){
		assert(0 && "Expected additive_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, ADDITIVE_EXPRESSION_REST);
}

struct parser_node * additive_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build additive_expression\n");
	if((n1 = multiplicative_expression(parser_state))){
		if((n1->next = additive_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, ADDITIVE_EXPRESSION);
		}else{
			assert(0 && "Expected additive_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected multiplicative_expression.\n");
		return 0;
	}
}

struct parser_node * shift_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build shift_expression_rest\n");
	if(!((n1 = p_accept(LEFT_OP, parser_state)) || (n1 = p_accept(RIGHT_OP, parser_state)))){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, SHIFT_EXPRESSION_REST);
	}
	if(!(n1->next = additive_expression(parser_state))){
		assert(0 && "Expected additive_expression.\n");
		return 0;
	}
	if(!(n1->next->next = shift_expression_rest(parser_state))){
		assert(0 && "Expected shift_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, SHIFT_EXPRESSION_REST);
}

struct parser_node * shift_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build shift_expression\n");
	if((n1 = additive_expression(parser_state))){
		if((n1->next = shift_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, SHIFT_EXPRESSION);
		}else{
			assert(0 && "Expected shift_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected additive_expression.\n");
		return 0;
	}
}

struct parser_node * relational_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build relational_expression_rest\n");
	if(!(
		(n1 = p_accept(OPEN_ANGLE_BRACKET_CHAR, parser_state)) ||
		(n1 = p_accept(CLOSE_ANGLE_BRACKET_CHAR, parser_state)) ||
		(n1 = p_accept(LE_OP, parser_state)) ||
		(n1 = p_accept(GE_OP, parser_state))
	)){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, RELATIONAL_EXPRESSION_REST);
	}
	if(!(n1->next = shift_expression(parser_state))){
		assert(0 && "Expected shift_expression.\n");
		return 0;
	}
	if(!(n1->next->next = relational_expression_rest(parser_state))){
		assert(0 && "Expected relational_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, RELATIONAL_EXPRESSION_REST);
}

struct parser_node * relational_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build relational_expression\n");
	if((n1 = shift_expression(parser_state))){
		if((n1->next = relational_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, RELATIONAL_EXPRESSION);
		}else{
			assert(0 && "Expected relational_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected shift_expression.\n");
		return 0;
	}
}

struct parser_node * equality_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build equality_expression_rest\n");
	if(!(
		(n1 = p_accept(EQ_OP, parser_state)) ||
		(n1 = p_accept(NE_OP, parser_state))
	)){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, EQUALITY_EXPRESSION_REST);
	}
	if(!(n1->next = relational_expression(parser_state))){
		assert(0 && "Expected relational_expression.\n");
		return 0;
	}
	if(!(n1->next->next = equality_expression_rest(parser_state))){
		assert(0 && "Expected equality_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, EQUALITY_EXPRESSION_REST);
}

struct parser_node * equality_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build equality_expression\n");
	if((n1 = relational_expression(parser_state))){
		if((n1->next = equality_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, EQUALITY_EXPRESSION);
		}else{
			assert(0 && "Expected equality_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected relational_expression.\n");
		return 0;
	}
}

struct parser_node * and_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build and_expression_rest\n");
	if(!(
		(n1 = p_accept(AMPERSAND_CHAR, parser_state))
	)){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, AND_EXPRESSION_REST);
	}
	if(!(n1->next = equality_expression(parser_state))){
		assert(0 && "Expected equality_expression.\n");
		return 0;
	}
	if(!(n1->next->next = and_expression_rest(parser_state))){
		assert(0 && "Expected and_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, AND_EXPRESSION_REST);
}

struct parser_node * and_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build and_expression\n");
	if((n1 = equality_expression(parser_state))){
		if((n1->next = and_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, AND_EXPRESSION);
		}else{
			assert(0 && "Expected and_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected equality_expression.\n");
		return 0;
	}
}

struct parser_node * exclusive_or_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build exclusive_or_expression_rest\n");
	if(!(
		(n1 = p_accept(CARET_CHAR, parser_state))
	)){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, EXCLUSIVE_OR_EXPRESSION_REST);
	}
	if(!(n1->next = and_expression(parser_state))){
		assert(0 && "Expected and_expression.\n");
		return 0;
	}
	if(!(n1->next->next = exclusive_or_expression_rest(parser_state))){
		assert(0 && "Expected exclusive_or_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, EXCLUSIVE_OR_EXPRESSION_REST);
}

struct parser_node * exclusive_or_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build exclusive_or_expression\n");
	if((n1 = and_expression(parser_state))){
		if((n1->next = exclusive_or_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, EXCLUSIVE_OR_EXPRESSION);
		}else{
			assert(0 && "Expected exclusive_or_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected and_expression.\n");
		return 0;
	}
}

struct parser_node * inclusive_or_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build inclusive_or_expression_rest\n");
	if(!(
		(n1 = p_accept(PIPE_CHAR, parser_state))
	)){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, INCLUSIVE_OR_EXPRESSION_REST);
	}
	if(!(n1->next = exclusive_or_expression(parser_state))){
		assert(0 && "Expected exclusive_or_expression.\n");
		return 0;
	}
	if(!(n1->next->next = inclusive_or_expression_rest(parser_state))){
		assert(0 && "Expected inclusive_or_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, INCLUSIVE_OR_EXPRESSION_REST);
}

struct parser_node * inclusive_or_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build inclusive_or_expression\n");
	if((n1 = exclusive_or_expression(parser_state))){
		if((n1->next = inclusive_or_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, INCLUSIVE_OR_EXPRESSION);
		}else{
			assert(0 && "Expected inclusive_or_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected exclusive_or_expression.\n");
		return 0;
	}
}

struct parser_node * logical_and_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build logical_and_expression_rest\n");
	if(!(
		(n1 = p_accept(AND_OP, parser_state))
	)){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, LOGICAL_AND_EXPRESSION_REST);
	}
	if(!(n1->next = inclusive_or_expression(parser_state))){
		assert(0 && "Expected inclusive_or_expression.\n");
		return 0;
	}
	if(!(n1->next->next = logical_and_expression_rest(parser_state))){
		assert(0 && "Expected logical_and_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, LOGICAL_AND_EXPRESSION_REST);
}

struct parser_node * logical_and_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build logical_and_expression\n");
	if((n1 = inclusive_or_expression(parser_state))){
		if((n1->next = logical_and_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, LOGICAL_AND_EXPRESSION);
		}else{
			assert(0 && "Expected logical_and_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected inclusive_or_expression.\n");
		return 0;
	}
}

struct parser_node * logical_or_expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build logical_or_expression_rest\n");
	if(!(
		(n1 = p_accept(OR_OP, parser_state))
	)){
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, LOGICAL_OR_EXPRESSION_REST);
	}
	if(!(n1->next = logical_and_expression(parser_state))){
		assert(0 && "Expected logical_and_expression.\n");
		return 0;
	}
	if(!(n1->next->next = logical_or_expression_rest(parser_state))){
		assert(0 && "Expected logical_or_expression_rest.\n");
		return 0;
	}
	return create_parser_node(parser_state, 0, n1, 0, LOGICAL_OR_EXPRESSION_REST);
}

struct parser_node * logical_or_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build logical_or_expression\n");
	if((n1 = logical_and_expression(parser_state))){
		if((n1->next = logical_or_expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, LOGICAL_OR_EXPRESSION);
		}else{
			assert(0 && "Expected logical_or_expression_rest\n");
			return 0;
		}
	}else{
		parser_progress("Expected logical_and_expression.\n");
		return 0;
	}
}

struct parser_node * conditional_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build conditional_expression\n");
	if((n1 = logical_or_expression(parser_state))){
		if((n1->next = p_accept(QUESTION_MARK_CHAR, parser_state))){
			if((n1->next->next = expression(parser_state))){
				if((n1->next->next->next = p_accept(COLON_CHAR, parser_state))){
					if((n1->next->next->next->next = conditional_expression(parser_state))){
						return create_parser_node(parser_state, 0, n1, 0, CONDITIONAL_EXPRESSION);
					}else{
						assert(0 && "FATAL! Expected conditional_expression.\n");
					}
				}else{
					assert(0 && "FATAL! Expected COLON_CHAR.\n");
				}
			}else{
				assert(0 && "FATAL! Expected expression.\n");
			}
		}else{
			return create_parser_node(parser_state, 0, n1, 0, CONDITIONAL_EXPRESSION);
		}
	}else{
		parser_progress("Expected logical_or_expression.\n");
		return 0;
	}
	return 0;
}

struct parser_node * assignment_operator(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build assignment_operator\n");
	if((n1 = p_accept(EQUALS_CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(MUL_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(DIV_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(MOD_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(ADD_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(SUB_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(LEFT_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(RIGHT_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(AND_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(XOR_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else if((n1 = p_accept(OR_ASSIGN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_OPERATOR);
	}else{
		parser_progress("Expected assignment operator.\n");
		return 0;
	}
}

struct parser_node * assignment_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build assignment_expression\n");

	if((n1 = unary_expression(parser_state))){
		if((n1->next = assignment_operator(parser_state))){	
			if((n1->next->next = assignment_expression(parser_state))){	
				return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_EXPRESSION);
			}else{
				parser_progress("Expected assignment expression, putting tokens back.\n");
				backtrack(parser_state, checkpoint);
			}
		}else{
			parser_progress("Expected assignment operator, putting tokens back.\n");
			backtrack(parser_state, checkpoint);
		}
	}

	if((n1 = conditional_expression(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ASSIGNMENT_EXPRESSION);
	}else{
		parser_progress("Expected conditional_expression.\n");
		backtrack(parser_state, checkpoint);
	}
	return 0;
}

struct parser_node * expression_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build expression_rest\n");
	if((n1 = p_accept(COMMA_CHAR, parser_state))){
		if((n1->next = assignment_expression(parser_state))){
			if((n1->next->next = expression_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, EXPRESSION_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression_rest.\n");
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected assignment_expression.\n");
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, EXPRESSION_REST);
	}
}

struct parser_node * expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build expression\n");
	if((n1 = assignment_expression(parser_state))){
		if((n1->next = expression_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, EXPRESSION);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression_rest.\n");
		}
	}else{
		parser_progress("Expected expression.\n");
		return 0;
	}
}

struct parser_node * constant_expression(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build constant_expression\n");
	if((n1 = conditional_expression(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, CONSTANT_EXPRESSION);
	}else{
		parser_progress("Expected expression.\n");
		return 0;
	}
}

struct parser_node * expression_statement(struct parser_state * parser_state){
	struct parser_node * n1;
	struct parser_node * n2;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build expression_statement\n");

	if((n1 = p_accept(SEMICOLON_CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, EXPRESSION_STATEMENT);
	}else if((n1 = expression(parser_state))){
		if((n2 = p_accept(SEMICOLON_CHAR, parser_state))){
			n1->next = n2;
			return create_parser_node(parser_state, 0, n1, 0, EXPRESSION_STATEMENT);
		}else{
			parser_progress("Expected semicolon in expression statement.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected semicolon or expression.\n");
		return 0;
	}
}

struct parser_node * statement(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build statement\n");
	if((n1 = labeled_statement(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STATEMENT);
	}else if((n1 = compound_statement(parser_state, 0))){
		return create_parser_node(parser_state, 0, n1, 0, STATEMENT);
	}else if((n1 = expression_statement(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STATEMENT);
	}else if((n1 = selection_statement(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STATEMENT);
	}else if((n1 = iteration_statement(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STATEMENT);
	}else if((n1 = jump_statement(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STATEMENT);
	}else{
		parser_progress("Expected statement.\n");
		return 0;
	}
}

struct parser_node * statement_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build statement_list_rest\n");
	if((n1 = statement(parser_state))){
		if((n1->next = statement_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, STATEMENT_LIST_REST);
		}else{
			parser_progress("Expected statement_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, STATEMENT_LIST_REST);
	}
}

struct parser_node * statement_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build statement_list\n");
	if((n1 = statement(parser_state))){
		if((n1->next = statement_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, STATEMENT_LIST);
		}else{
			parser_progress("Expected statement_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected statement.\n");
		return 0;
	}
}

struct parser_node * compound_statement(struct parser_state * parser_state, struct parser_node * possible_declarator){
	struct parser_node * n1;
	parser_progress("Attempting to build compound_statement\n");
	if((n1 = p_accept(OPEN_BRACE_CHAR, parser_state))){
		push_operation(parser_state, INCREMENT_SCOPE_DEPTH, 0);
		if(possible_declarator){
			if(possible_declarator->type == DECLARATION_LIST){
				/*  For K&R C style function */
			}else if(possible_declarator->type == DECLARATOR){
				/* If possible_declarator is set, it will be a declarator or declaration_list from a function.  Declare the params in this scope. */
				struct parser_node * copy = copy_parser_node_children_only(possible_declarator);
				struct parser_node * abstract;
				struct parser_node * parameter_type_list;
				assert(copy->type == DECLARATOR);
				abstract = convert_declarator_to_abstract_declarator(copy);
				parameter_type_list = get_parameter_type_list_from_abstract_declarator(abstract);
				if(parameter_type_list){
					struct parser_node * parameter_declaration = 0;
					unsigned int param_index = 0;
					struct namespace_object * prev_obj = 0;
					while((parameter_declaration = get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list, param_index))){
						struct first_and_last_namespace_object fl;
						fl = manage_generic_declaration(parser_state, parameter_declaration, 0, 0, PARAMETER, prev_obj, 0);
						prev_obj = fl.last;
						param_index++;
					}
				}
				/*  We have to delete this after we're done using the parser state  */
				push_operation(parser_state, CREATE_PARSER_NODE_TREE, abstract);
			}else{
				assert(0 && "Not expected");
			}
		}
		if((n1->next = statement_list(parser_state))){
			if((n1->next->next = p_accept(CLOSE_BRACE_CHAR, parser_state))){
				push_operation(parser_state, DECREMENT_SCOPE_DEPTH, 0);
				return create_parser_node(parser_state, 0, n1, 0, COMPOUND_STATEMENT);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_BRACE_CHAR.\n");
				return 0;
			}
		}else if((n1->next = declaration_list(parser_state, 0))){
			if((n1->next->next = p_accept(CLOSE_BRACE_CHAR, parser_state))){
				push_operation(parser_state, DECREMENT_SCOPE_DEPTH, 0);
				return create_parser_node(parser_state, 0, n1, 0, COMPOUND_STATEMENT);
			}else if((n1->next->next = statement_list(parser_state))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR, parser_state))){
					push_operation(parser_state, DECREMENT_SCOPE_DEPTH, 0);
					return create_parser_node(parser_state, 0, n1, 0, COMPOUND_STATEMENT);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_BRACE_CHAR.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement list or }.\n");
				return 0;
			}
		}else if((n1->next = p_accept(CLOSE_BRACE_CHAR, parser_state))){
			push_operation(parser_state, DECREMENT_SCOPE_DEPTH, 0);
			return create_parser_node(parser_state, 0, n1, 0, COMPOUND_STATEMENT);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement_list or CLOSE_BRACE_CHAR.\n");
			return 0;
		}
	}else{
		parser_progress("Expected compound_statement.\n");
		return 0;
	}
}


struct parser_node * type_qualifier(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build type_qualifier\n");
	if((n1 = p_accept(CONST, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_QUALIFIER);
	}else if((n1 = p_accept(VOLATILE, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_QUALIFIER);
	}else {
		parser_progress("Expected type_qualifier.\n");
		return 0;
	}
}

struct parser_node * storage_class_specifier(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build storage_class_specifier\n");
	if((n1 = p_accept(TYPEDEF, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(EXTERN, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(STATIC, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(AUTO, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STORAGE_CLASS_SPECIFIER);
	}else if((n1 = p_accept(REGISTER, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STORAGE_CLASS_SPECIFIER);
	}else {
		parser_progress("Expected type_qualifier.\n");
		return 0;
	}
}

struct parser_node * enumerator(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build enumerator\n");
	if((n1 = p_accept(IDENTIFIER, parser_state))){
		if((n1->next = p_accept(EQUALS_CHAR, parser_state))){
			if((n1->next->next = constant_expression(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, ENUMERATOR);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Required identifier.  Cannot continue.\n");
				return 0;
			}
		}else{
			return create_parser_node(parser_state, 0, n1, 0, ENUMERATOR);
		}
	}else{
		buffered_printf(parser_state->buffered_output, "Required identifier.  Cannot continue.\n");
		return 0;
	}
}

struct parser_node * enumerator_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build enumerator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, parser_state))){
		if((n1->next = enumerator(parser_state))){
			if((n1->next->next = enumerator_list_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, ENUMERATOR_LIST_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Required enumerator list rest.  Cannot continue.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Required enumerator.  Cannot continue.\n");
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, ENUMERATOR_LIST_REST);
	}
}

struct parser_node * enumerator_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build enumerator_list\n");
	if((n1 = enumerator(parser_state))){
		if((n1->next = enumerator_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, ENUMERATOR_LIST);
		}else{
			buffered_printf(parser_state->buffered_output,"Required enumerator list rest.  Cannot continue.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		buffered_printf(parser_state->buffered_output,"Required enumerator.  Cannot continue.\n");
		return 0;
	}
}

struct parser_node * enum_specifier(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build enum_specifier\n");
	if((n1 = p_accept(ENUM, parser_state))){
		if((n1->next = p_accept(OPEN_BRACE_CHAR, parser_state))){
			if((n1->next->next = enumerator_list(parser_state))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR, parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, ENUM_SPECIFIER);
				} else {
					assert(0 && "FATAL_COMPILE_FAILURE!!! Required CLOSE_BRACE_CHAR.  Cannot continue.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Required ENUMERATOR_LIST.  Cannot continue.\n");
				return 0;
			}
		}else if((n1->next = p_accept(IDENTIFIER, parser_state))){
			if((n1->next->next = p_accept(OPEN_BRACE_CHAR, parser_state))){
				if((n1->next->next->next = enumerator_list(parser_state))){
					if((n1->next->next->next->next = p_accept(CLOSE_BRACE_CHAR, parser_state))){
						return create_parser_node(parser_state, 0, n1, 0, ENUM_SPECIFIER);
					} else {
						assert(0 && "FATAL_COMPILE_FAILURE!!! Required CLOSE_BRACE_CHAR.  Cannot continue.\n");
						return 0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Required ENUMERATOR_LIST.  Cannot continue.\n");
					return 0;
				}
			}else{
				return create_parser_node(parser_state, 0, n1, 0, ENUM_SPECIFIER);
			}
		}else {
			assert(0 && "FATAL_COMPILE_FAILURE!!! Required OPEN_BRACE_CHAR or IDENTIFIER cannot continue.\n");
			return 0;
		}
	}else {
		parser_progress("Expected ENUM.\n");
		return 0;
	}
}

struct parser_node * struct_or_union(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build struct_or_union\n");
	if((n1 = p_accept(STRUCT, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STRUCT_OR_UNION);
	}else if((n1 = p_accept(UNION, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, STRUCT_OR_UNION);
	}else {
		parser_progress("Expected struct_or_union.\n");
		return 0;
	}
}

struct parser_node * type_specifier(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build type_specifier\n");
	if((n1 = p_accept(VOID, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(CHAR, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(SHORT, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(INT, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(LONG, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(FLOAT, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(DOUBLE, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(SIGNED, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(UNSIGNED, parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = struct_or_union_specifier(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = enum_specifier(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
	}else if((n1 = p_accept(IDENTIFIER, parser_state))){
		/*  This identifier can only be treated as a type if it has been declared with a typedef.  */
		unsigned char * ident = copy_string(n1->c_lexer_token->first_byte, n1->c_lexer_token->last_byte);
		struct namespace_object * obj;
		struct scope_level * scope;
		struct normalized_declaration_element * element;
		struct type_description * type_description = 0;
		scope = parser_state->top_scope;
		descend_scope(&scope, parser_state->current_scope_depth);
		obj = get_namespace_object_from_closest_namespace(ident, IDENTIFIER_NAMESPACE, scope, 0);
		if(obj){
			element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, 0);
			type_description = create_type_description_from_normalized_declaration_element(element);
		}
		if(obj && count_specifiers(type_description, TYPEDEF)){
			destroy_type_description(type_description);
			free(ident);
			return create_parser_node(parser_state, 0, n1, 0, TYPE_SPECIFIER);
		}else{
			if(type_description){
				destroy_type_description(type_description);
			}
			backtrack(parser_state, checkpoint);
			free(ident);
			parser_progress("Identifier not typedefed type.  Putting back tokens.\n");
			return 0;
		}
	}else{
		parser_progress("Expected a type_specifier.\n");
		return 0;
	}
}

struct parser_node * declaration_specifiers(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build declaration_specifiers\n");
	if((n1 = storage_class_specifier(parser_state))){
		if((n1->next = declaration_specifiers(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION_SPECIFIERS);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION_SPECIFIERS);
		}
	}else if((n1 = type_specifier(parser_state))){
		if((n1->next = declaration_specifiers(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION_SPECIFIERS);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION_SPECIFIERS);
		}
	}else if((n1 = type_qualifier(parser_state))){
		if((n1->next = declaration_specifiers(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION_SPECIFIERS);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION_SPECIFIERS);
		}
	}else{
		parser_progress("Expected a declaration_specifiers.\n");
		return 0;
	}
}

struct parser_node * init_declarator_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build init_declarator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, parser_state))){
		if((n1->next = init_declarator(parser_state))){
			if((n1->next->next = init_declarator_list_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, INIT_DECLARATOR_LIST_REST);
			}else{
				parser_progress("Expected a init_declarator_list_rest.\n");
				backtrack(parser_state, checkpoint);
				return 0;
			}
		}else{
			parser_progress("Expected a init_declarator.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, INIT_DECLARATOR_LIST_REST);
	}
}

struct parser_node * init_declarator_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build init_declarator_list\n");
	if((n1 = init_declarator(parser_state))){
		if((n1->next = init_declarator_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, INIT_DECLARATOR_LIST);
		}else{
			parser_progress("Expected a init_declarator_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected an init_declarator .\n");
		return 0;
	}
}

struct parser_node * declaration(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build declaration\n");
	if((n1 = declaration_specifiers(parser_state))){
		if((n1->next = init_declarator_list(parser_state))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, DECLARATION);
			}else{
				parser_progress("Expected a SEMICOLON_CHAR in declaration.\n");
				backtrack(parser_state, checkpoint);
				return 0;
			}
		}else if((n1->next = p_accept(SEMICOLON_CHAR, parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION);
		}else{
			parser_progress("Expected a SEMICOLON_CHAR.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected a declaration.\n");
		return 0;
	}
}

struct parser_node * declaration_list_rest(struct parser_state * parser_state, struct namespace_object * previous_object, struct first_and_last_namespace_object * fl_all){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build declaration_list_rest\n");
	if((n1 = declaration(parser_state))){
		struct first_and_last_namespace_object fl_rest; /*  First and last object of all later declarations */
		struct first_and_last_namespace_object fl_current = manage_generic_declaration(parser_state, n1, 0, 0, LOCAL, previous_object, 0); /*  First and last object of current declaration */
		fl_rest.first = 0;
		fl_rest.last = 0;
		if((n1->next = declaration_list_rest(parser_state, fl_current.last, &fl_rest))){
			if(fl_current.last && fl_rest.last){
				/* The point of this exercise it to be able to point to the next one */
				struct namespace_object_change * c = malloc(sizeof(struct namespace_object_change));
				c->target = fl_current.last;
				c->old_obj = fl_current.last->next;
				c->new_obj = fl_rest.first;
				push_operation(parser_state, SET_NEXT_NAMESPACE_OBJECT, c);
				fl_current.last = fl_rest.last; 
			}
			fl_all->first = fl_current.first ? fl_current.first : fl_rest.first;
			fl_all->last = fl_rest.last ? fl_rest.last : fl_current.last;
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION_LIST_REST);
		}else{
			parser_progress("Expected a declaration_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, DECLARATION_LIST_REST);
	}
}

struct parser_node * declaration_list(struct parser_state * parser_state, struct namespace_object * previous_object){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build declaration_list\n");
	if((n1 = declaration(parser_state))){
		struct scope_level * scope;
		struct first_and_last_namespace_object fl_rest; /*  First and last object of all later declarations */
		struct first_and_last_namespace_object fl_current = manage_generic_declaration(parser_state, n1, 0, 0, LOCAL, previous_object, 0); /*  First and last object of current declaration */
		fl_rest.first = 0;
		fl_rest.last = 0;
		scope = parser_state->top_scope;
		descend_scope(&scope, parser_state->current_scope_depth);
		if((n1->next = declaration_list_rest(parser_state, fl_current.last, &fl_rest))){
			scope->first_local_object = fl_current.first ? fl_current.first : fl_rest.first; /*  This block scope will need to keep track of the first local it has */
			if(fl_current.last && fl_rest.last){
				/* The point of this exercise it to be able to point to the next one */
				struct namespace_object_change * c = malloc(sizeof(struct namespace_object_change));
				c->target = fl_current.last;
				c->old_obj = fl_current.last->next;
				c->new_obj = fl_rest.first;
				push_operation(parser_state, SET_NEXT_NAMESPACE_OBJECT, c);
				fl_current.last = fl_rest.last; 
			}
			return create_parser_node(parser_state, 0, n1, 0, DECLARATION_LIST);
		}else{
			parser_progress("Expected a declaration_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected a declaration.\n");
		return 0;
	}
}

struct parser_node * identifier_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build identifier_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, parser_state))){
		if((n1->next = p_accept(IDENTIFIER, parser_state))){
			if((n1->next->next = identifier_list_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, IDENTIFIER_LIST_REST);
			}else{
				parser_progress("Expected a IDENTIFIER_LIST_REST.\n");
				backtrack(parser_state, checkpoint);
				return 0;
			}
		}else{
			parser_progress("Expected an IDENTIFIER.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, IDENTIFIER_LIST_REST);
	}
}

struct parser_node * identifier_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build identifier_list\n");
	if((n1 = p_accept(IDENTIFIER, parser_state))){
		if((n1->next = identifier_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, IDENTIFIER_LIST);
		}else{
			parser_progress("Expected a IDENTIFIER_LIST.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected an IDENTIFIER.\n");
		return 0;
	}
}

struct parser_node * direct_declarator_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build direct_declarator_rest\n");
	if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR,parser_state))){
		if((n1->next = constant_expression(parser_state))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR,parser_state))){
				if((n1->next->next->next = direct_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_DECLARATOR_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_declarator_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_SQUARE_BRACKET_CHAR.\n");
				return 0;
			}
		}else if((n1->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR,parser_state))){
			if((n1->next->next = direct_declarator_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, DIRECT_DECLARATOR_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_declarator_rest.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected a constant expression or a CLOSE_SQUARE_BRACKET_CHAR.\n");
			return 0;
		}
	}else if((n1 = p_accept(OPEN_PAREN_CHAR,parser_state))){
		if((n1->next = parameter_type_list(parser_state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR,parser_state))){
				if((n1->next->next->next = direct_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_DECLARATOR_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_declarator_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected a CLOSE_PAREN.\n");
				return 0;
			}
		}else if((n1->next = identifier_list(parser_state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR,parser_state))){
				if((n1->next->next->next = direct_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_DECLARATOR_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_declarator_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected a CLOSE_PAREN.\n");
				return 0;
			}
		}else if((n1->next = p_accept(CLOSE_PAREN_CHAR,parser_state))){
			if((n1->next->next = direct_declarator_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, DIRECT_DECLARATOR_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_declarator_rest.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected a CLOSE_PAREN.\n");
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, DIRECT_DECLARATOR_REST);
	}
}

struct parser_node * type_qualifier_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build type_qualifier_list_rest\n");
	if((n1 = type_qualifier(parser_state))){
		if((n1->next = type_qualifier_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, TYPE_QUALIFIER_LIST_REST);
		}else{
			parser_progress("Expected type_qualifier_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, TYPE_QUALIFIER_LIST_REST);
	}
}

struct parser_node * type_qualifier_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build type_qualifier_list\n");
	if((n1 = type_qualifier(parser_state))){
		if((n1->next = type_qualifier_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, TYPE_QUALIFIER_LIST);
		}else{
			parser_progress("Expected type_qualifier_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected type_qualifier.\n");
		return 0;
	}
}


struct parser_node * pointer(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build pointer\n");
	if((n1 = p_accept(MULTIPLY_CHAR, parser_state))){
		if((n1->next = type_qualifier_list(parser_state))){
			if((n1->next->next = pointer(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, POINTER);
			}else{
				return create_parser_node(parser_state, 0, n1, 0, POINTER);
			}
		}else if ((n1->next = pointer(parser_state))) {
			return create_parser_node(parser_state, 0, n1, 0, POINTER);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, POINTER);
		}
	}else{
		parser_progress("Expected *.\n");
		return 0;
	}
}

struct parser_node * direct_declarator(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build direct_declarator\n");
	if((n1 = p_accept(IDENTIFIER, parser_state))){
		if((n1->next = direct_declarator_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, DIRECT_DECLARATOR);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected a direct declarator rest.\n");
			return 0;
		}
	}else if((n1 = p_accept(OPEN_PAREN_CHAR, parser_state))){
		if((n1->next = declarator(parser_state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
				if((n1->next->next->next = direct_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_DECLARATOR);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct declarator rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
				return 0;
			}
		}else{
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected a pointer or direct declarator.\n");
		return 0;
	}
}



struct parser_node * declarator(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build declarator\n");
	if((n1 = pointer(parser_state))){
		if((n1->next = direct_declarator(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, DECLARATOR);
		}else{
			parser_progress("Expected a direct declarator.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else if((n1 = direct_declarator(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, DECLARATOR);
	}else{
		parser_progress("Expected a pointer or direct declarator.\n");
		return 0;
	}
}

struct parser_node * initializer_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build initializer_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR,parser_state))){
		if((n1->next = initializer(parser_state))){
			if((n1->next->next = initializer_list_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, INITIALIZER_LIST_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected initializer_list_rest.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected initializer.\n");
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, INITIALIZER_LIST_REST);
	}
}

struct parser_node * initializer_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build initializer_list\n");
	if((n1 = initializer(parser_state))){
		if((n1->next = initializer_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, INITIALIZER_LIST);
		}else{
			parser_progress("Expected initializer_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected initializer.\n");
		return 0;
	}
}

struct parser_node * initializer(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build initializer\n");
	if((n1 = assignment_expression(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, INITIALIZER);
	}else if((n1 = p_accept(OPEN_BRACE_CHAR, parser_state))){
		if((n1->next = initializer_list(parser_state))){
			if((n1->next->next = p_accept(CLOSE_BRACE_CHAR, parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, INITIALIZER);
			}else if((n1->next->next = p_accept(COMMA_CHAR, parser_state))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR, parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, INITIALIZER);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected }.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected , or }.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected initializer_list.\n");
			return 0;
		}
	}else{
		parser_progress("Expected OPEN_BRACE_CHAR or assignment expression.\n");
		return 0;
	}
}

struct parser_node * init_declarator(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build init_declarator\n");
	if((n1 = declarator(parser_state))){
		if((n1->next = p_accept(EQUALS_CHAR, parser_state))){
			if((n1->next->next = initializer(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, INIT_DECLARATOR);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected initializer.\n");
				return 0;
			}
		}else{
			return create_parser_node(parser_state, 0, n1, 0, INIT_DECLARATOR);
		}
	}else{
		parser_progress("Expected declarator in init declarator.\n");
		return 0;
	}
}

struct parser_node * struct_declarator(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build struct_declarator\n");
	if((n1 = declarator(parser_state))){
		if((n1->next = p_accept(COLON_CHAR, parser_state))){
			if((n1->next->next = constant_expression(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, STRUCT_DECLARATOR);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected constant_expression.\n");
				return 0;
			}
		}else{
			return create_parser_node(parser_state, 0, n1, 0, STRUCT_DECLARATOR);
		}
	}else if((n1 = p_accept(COLON_CHAR, parser_state))){
		if((n1->next = constant_expression(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, STRUCT_DECLARATOR);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected constant_expression.\n");
			return 0;
		}
	}else{
		parser_progress("Expected COLON_CHAR or declarator.\n");
		return 0;
	}
}

struct parser_node * struct_declarator_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build struct_declarator_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, parser_state))){
		if((n1->next = struct_declarator(parser_state))){
			if((n1->next->next = struct_declarator_list_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, STRUCT_DECLARATOR_LIST_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected struct_declarator_list_rest.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected struct_declarator.\n");
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, STRUCT_DECLARATOR_LIST_REST);
	}
}


struct parser_node * struct_declarator_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build struct_declarator_list\n");
	if((n1 = struct_declarator(parser_state))){
		if((n1->next = struct_declarator_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, STRUCT_DECLARATOR_LIST);
		}else{
			parser_progress("Expected struct_declarator_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected struct_declarator.\n");
		return 0;
	}
}

struct parser_node * specifier_qualifier_list(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build specifier_qualifier_list\n");
	if((n1 = type_specifier(parser_state))){
		if((n1->next = specifier_qualifier_list(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, SPECIFIER_QUALIFIER_LIST);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, SPECIFIER_QUALIFIER_LIST);
		}
	}else if((n1 = type_qualifier(parser_state))){
		if((n1->next = specifier_qualifier_list(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, SPECIFIER_QUALIFIER_LIST);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, SPECIFIER_QUALIFIER_LIST);
		}
	}else{
		parser_progress("Expected type specifier or type qualifier.\n");
		return 0;
	}
}

struct parser_node * struct_declaration(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build struct_declaration\n");
	if((n1 = specifier_qualifier_list(parser_state))){
		if((n1->next = struct_declarator_list(parser_state))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR,parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, STRUCT_DECLARATION);
			}else{
				parser_progress("Expected SEMICOLON_CHAR.\n");
				backtrack(parser_state, checkpoint);
				return 0;
			}
		}else{
			parser_progress("Expected struct_declarator_list.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected specifier_qualifier_list.\n");
		return 0;
	}
}

struct parser_node * struct_declaration_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build struct_declaration_list_rest\n");
	if((n1 = struct_declaration(parser_state))){
		if((n1->next = struct_declaration_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, STRUCT_DECLARATION_LIST_REST);
		}else{
			parser_progress("Expected struct_declaration_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, STRUCT_DECLARATION_LIST_REST);
	}
}


struct parser_node * struct_declaration_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build struct_declaration_list\n");
	if((n1 = struct_declaration(parser_state))){
		if((n1->next = struct_declaration_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, STRUCT_DECLARATION_LIST);
		}else{
			parser_progress("Expected struct_declaration_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected struct_declaration.\n");
		return 0;
	}
}

struct parser_node * struct_or_union_specifier(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build struct_or_union_specifier\n");
	if((n1 = struct_or_union(parser_state))){
		if((n1->next = p_accept(IDENTIFIER,parser_state))){
			if((n1->next->next = p_accept(OPEN_BRACE_CHAR,parser_state))){
				if((n1->next->next->next = struct_declaration_list(parser_state))){
					if((n1->next->next->next->next = p_accept(CLOSE_BRACE_CHAR,parser_state))){
						return create_parser_node(parser_state, 0, n1, 0, STRUCT_OR_UNION_SPECIFIER);
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_BRACE_CHAR.\n");
						return 0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected struct_declaration_list.\n");
					return 0;
				}
			}else{
				return create_parser_node(parser_state, 0, n1, 0, STRUCT_OR_UNION_SPECIFIER);
			}
		}else if((n1->next = p_accept(OPEN_BRACE_CHAR,parser_state))){
			if((n1->next->next = struct_declaration_list(parser_state))){
				if((n1->next->next->next = p_accept(CLOSE_BRACE_CHAR,parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, STRUCT_OR_UNION_SPECIFIER);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_BRACE_CHAR.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected struct_declaration_list.\n");
				return 0;
			}
		}else{
			parser_progress("Expected IDENTIFIER or open brace char.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected struct_declaration.\n");
		return 0;
	}
}

struct parser_node * parameter_declaration(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build parameter_declaration\n");
	if((n1 = declaration_specifiers(parser_state))){
		if((n1->next = declarator(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, PARAMETER_DECLARATION);
		}else if((n1->next = abstract_declarator(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, PARAMETER_DECLARATION);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, PARAMETER_DECLARATION);
		}
	}else{
		parser_progress("Expected parameter_declaration.\n");
		return 0;
	}
}

struct parser_node * parameter_list_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build parameter_list_rest\n");
	if((n1 = p_accept(COMMA_CHAR, parser_state))){
		if((n1->next = parameter_declaration(parser_state))){
			if((n1->next->next = parameter_list_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, PARAMETER_LIST_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected parameter_list_rest, (in parameter_list_rest).\n");
				return 0;
			}
		}else{
			/* Re-wind back over the ',' that we accepted */
			backtrack(parser_state, checkpoint);
			n1 = make_epsilon(parser_state);
			return create_parser_node(parser_state, 0, n1, 0, PARAMETER_LIST_REST);
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, PARAMETER_LIST_REST);
	}
}

struct parser_node * parameter_list(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build parameter_list\n");
	if((n1 = parameter_declaration(parser_state))){
		if((n1->next = parameter_list_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, PARAMETER_LIST);
		}else{
			parser_progress("Expected parameter_list_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected parameter_declaration.\n");
		return 0;
	}
}

struct parser_node * parameter_type_list(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build parameter_type_list\n");
	if((n1 = parameter_list(parser_state))){
		if((n1->next = p_accept(COMMA_CHAR, parser_state))){
			if((n1->next->next = p_accept(ELLIPSIS, parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, PARAMETER_TYPE_LIST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected parameter_list_rest.\n");
				return 0;
			}
		}else{
			return create_parser_node(parser_state, 0, n1, 0, PARAMETER_TYPE_LIST);
		}
	}else{
		parser_progress("Expected parameter_list.\n");
		return 0;
	}
}



struct parser_node * direct_abstract_declarator_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build direct_abstract_declarator_rest\n");
	if((n1 = p_accept(OPEN_PAREN_CHAR, parser_state))){
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
			if((n1->next->next = direct_abstract_declarator_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
				return 0;
			}
		}else if((n1->next = parameter_type_list(parser_state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
				if((n1->next->next->next = direct_abstract_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
				return 0;
			}
		}else{
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, parser_state))){
		if((n1->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, parser_state))){
			if((n1->next->next = direct_abstract_declarator_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR_REST);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
				return 0;
			}
		}else if((n1->next = constant_expression(parser_state))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, parser_state))){
				if((n1->next->next->next = direct_abstract_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR_REST);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_SQUARE_BRACKET_CHAR.\n");
				return 0;
			}
		}else{
			return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR_REST);
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, DIRECT_ABSTRACT_DECLARATOR_REST);
	}
}


struct parser_node * direct_abstract_declarator(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build direct_abstract_declarator\n");
	if((n1 = p_accept(OPEN_PAREN_CHAR, parser_state))){
		if((n1->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
			if((n1->next->next = direct_abstract_declarator_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
				return 0;
			}
		}else if((n1->next = parameter_type_list(parser_state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
				if((n1->next->next->next = direct_abstract_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
				return 0;
			}
		}else if((n1->next = abstract_declarator(parser_state))){
			if((n1->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
				if((n1->next->next->next = direct_abstract_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
				return 0;
			}
		}else{
			return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR);
		}
	}else if((n1 = p_accept(OPEN_SQUARE_BRACKET_CHAR, parser_state))){
		if((n1->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, parser_state))){
			if((n1->next->next = direct_abstract_declarator_rest(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
				return 0;
			}
		}else if((n1->next = constant_expression(parser_state))){
			if((n1->next->next = p_accept(CLOSE_SQUARE_BRACKET_CHAR, parser_state))){
				if((n1->next->next->next = direct_abstract_declarator_rest(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected direct_abstract_declarator_rest.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_SQUARE_BRACKET_CHAR.\n");
				return 0;
			}
		}else{
			return create_parser_node(parser_state, 0, n1, 0, DIRECT_ABSTRACT_DECLARATOR);
		}
	}else{
		parser_progress("Expected OPEN_SQUARE_BRACKET_CHAR or OPEN_PAREN_CHAR.\n");
		return 0;
	}
}

struct parser_node * type_name(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build type_name\n");
	if((n1 = specifier_qualifier_list(parser_state))){
		if((n1->next = abstract_declarator(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, TYPE_NAME);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, TYPE_NAME);
		}
	}else{
		parser_progress("Expected specifier_qualifier_list.\n");
		return 0;
	}
}


struct parser_node * abstract_declarator(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build abstract_declarator\n");
	if((n1 = pointer(parser_state))){
		if((n1->next = direct_abstract_declarator(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, ABSTRACT_DECLARATOR);
		}else{
			return create_parser_node(parser_state, 0, n1, 0, ABSTRACT_DECLARATOR);
		}
	}else if((n1 = direct_abstract_declarator(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, ABSTRACT_DECLARATOR);
	}else{
		parser_progress("Expected abstract_declarator.\n");
		return 0;
	}
}

struct parser_node * labeled_statement(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build labeled_statement\n");
	if((n1 = p_accept(IDENTIFIER,parser_state))){
		if((n1->next = p_accept(COLON_CHAR, parser_state))){
			if((n1->next->next = statement(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, LABELED_STATEMENT);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
				return 0;
			}
		}else{
			parser_progress("Assuming not a label, resetting position.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else if((n1 = p_accept(CASE,parser_state))){
		if((n1->next = constant_expression(parser_state))){
			if((n1->next->next = p_accept(COLON_CHAR, parser_state))){
				if((n1->next->next->next = statement(parser_state))){
					return create_parser_node(parser_state, 0, n1, 0, LABELED_STATEMENT);
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected COLON_CHAR.\n");
			return 0;
		}
	}else if((n1 = p_accept(DEFAULT,parser_state))){
		if((n1->next = p_accept(COLON_CHAR, parser_state))){
			if((n1->next->next = statement(parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, LABELED_STATEMENT);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected COLON_CHAR.\n");
			return 0;
		}
	}else{
		parser_progress("Expected labeled_statement.\n");
		return 0;
	}
}

struct parser_node * selection_statement(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build selection_statement\n");
	if((n1 = p_accept(IF,parser_state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, parser_state))){
			if((n1->next->next = expression(parser_state))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
					if((n1->next->next->next->next = statement(parser_state))){
						if((n1->next->next->next->next->next = p_accept(ELSE, parser_state))){
							if((n1->next->next->next->next->next->next = statement(parser_state))){
								return create_parser_node(parser_state, 0, n1, 0, SELECTION_STATEMENT);
							}else{
								assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement after else.\n");
								return 0;
							}
						}else{
							return create_parser_node(parser_state, 0, n1, 0, SELECTION_STATEMENT);
						}
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement after if.\n");
						return 0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR.\n");
			return 0;
		}
	}else if((n1 = p_accept(SWITCH, parser_state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, parser_state))){
			if((n1->next->next = expression(parser_state))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
					if((n1->next->next->next->next = statement(parser_state))){
						return create_parser_node(parser_state, 0, n1, 0, SELECTION_STATEMENT);
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
						return 0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR.\n");
			return 0;
		}
	}else {
		parser_progress("Expected IF or SWITCH.\n");
		return 0;
	}
}

struct parser_node * iteration_statement(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build iteration_statement\n");
	if((n1 = p_accept(WHILE,parser_state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR, parser_state))){
			if((n1->next->next = expression(parser_state))){
				if((n1->next->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
					if((n1->next->next->next->next = statement(parser_state))){
						return create_parser_node(parser_state, 0, n1, 0, ITERATION_STATEMENT);
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
						return 0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR for while.\n");
			return 0;
		}
	}else if((n1 = p_accept(DO,parser_state))){
		if((n1->next = statement(parser_state))){
			if((n1->next->next = p_accept(WHILE, parser_state))){
				if((n1->next->next->next = p_accept(OPEN_PAREN_CHAR, parser_state))){
					if((n1->next->next->next->next = expression(parser_state))){
						if((n1->next->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
							if((n1->next->next->next->next->next->next = p_accept(SEMICOLON_CHAR, parser_state))){
								return create_parser_node(parser_state, 0, n1, 0, ITERATION_STATEMENT);
							}else{
								assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
								return 0;
							}
						}else{
							assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
							return 0;
						}
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression.\n");
						return 0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected WHILE.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
			return 0;
		}
	}else if((n1 = p_accept(FOR,parser_state))){
		if((n1->next = p_accept(OPEN_PAREN_CHAR,parser_state))){
			if((n1->next->next = expression_statement(parser_state))){
				if((n1->next->next->next = expression_statement(parser_state))){
					if((n1->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
						if((n1->next->next->next->next->next = statement(parser_state))){
							return create_parser_node(parser_state, 0, n1, 0, ITERATION_STATEMENT);
						}else{
							assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression or statement.\n");
							return 0;
						}
					}else if((n1->next->next->next->next = expression(parser_state))){
						if((n1->next->next->next->next->next = p_accept(CLOSE_PAREN_CHAR, parser_state))){
							if((n1->next->next->next->next->next->next = statement(parser_state))){
								return create_parser_node(parser_state, 0, n1, 0, ITERATION_STATEMENT);
							}else{
								assert(0 && "FATAL_COMPILE_FAILURE!!! Expected statement.\n");
								return 0;
							}
						}else{
							assert(0 && "FATAL_COMPILE_FAILURE!!! Expected CLOSE_PAREN_CHAR.\n");
							return 0;
						}
					}else{
						assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression or CLOSE_PAREN_CHAR.\n");
						return 0;
					}
				}else{
					assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression_statement.\n");
					return 0;
				}
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected expression_statement.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected OPEN_PAREN_CHAR.\n");
			return 0;
		}
	}else{
		parser_progress("Expected WHILE DO or FOR.\n");
		return 0;
	}
}

struct parser_node * jump_statement(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build jump_statement\n");
	if((n1 = p_accept(GOTO,parser_state))){
		if((n1->next = p_accept(IDENTIFIER, parser_state))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, JUMP_STATEMENT);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected IDENTIFIER.\n");
			return 0;
		}
	}else if((n1 = p_accept(CONTINUE,parser_state))){
		if((n1->next= p_accept(SEMICOLON_CHAR, parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, JUMP_STATEMENT);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
			return 0;
		}
	}else if((n1 = p_accept(BREAK,parser_state))){
		if((n1->next= p_accept(SEMICOLON_CHAR, parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, JUMP_STATEMENT);
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
			return 0;
		}
	}else if((n1 = p_accept(RETURN,parser_state))){
		if((n1->next = p_accept(SEMICOLON_CHAR, parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, JUMP_STATEMENT);
		}else if((n1->next = expression(parser_state))){
			if((n1->next->next = p_accept(SEMICOLON_CHAR, parser_state))){
				return create_parser_node(parser_state, 0, n1, 0, JUMP_STATEMENT);
			}else{
				assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR.\n");
				return 0;
			}
		}else{
			assert(0 && "FATAL_COMPILE_FAILURE!!! Expected SEMICOLON_CHAR or expression.\n");
			return 0;
		}
	}else{
		parser_progress("Expected jump_statement.\n");
		return 0;
	}
}

struct parser_node * function_definition(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build function_definition\n");
	if((n1 = declaration_specifiers(parser_state))){
		if((n1->next = declarator(parser_state))){
			if((n1->next->next = declaration_list(parser_state, 0))){
				if((n1->next->next->next = compound_statement(parser_state, n1->next->next))){
					return create_parser_node(parser_state, 0, n1, 0, FUNCTION_DEFINITION);
				}else{
					parser_progress("Expected compound_statement.\n");
					backtrack(parser_state, checkpoint);
					return 0;
				}
			}else if((n1->next->next = compound_statement(parser_state, n1->next))){
				return create_parser_node(parser_state, 0, n1, 0, FUNCTION_DEFINITION);
			}else{
				parser_progress("Expected compound_statement or declaration_list for function definition.\n");
				backtrack(parser_state, checkpoint);
				return 0;
			}
		}else{
			backtrack(parser_state, checkpoint);
			buffered_printf(parser_state->buffered_output,"Unable to complete function definition, putting back tokens line %d.\n", parser_state->line_number);
			return 0;
		}
	}else if((n1 = declarator(parser_state))){
		if((n1->next = declaration_list(parser_state, 0))){
			if((n1->next->next = compound_statement(parser_state, n1->next))){
				return create_parser_node(parser_state, 0, n1, 0, FUNCTION_DEFINITION);
			}else{
				parser_progress("Expected compound_statement.\n");
				backtrack(parser_state, checkpoint);
				return 0;
			}
		}else if((n1->next = compound_statement(parser_state, n1))){
			return create_parser_node(parser_state, 0, n1, 0, FUNCTION_DEFINITION);
		}else{
			backtrack(parser_state, checkpoint);
			buffered_printf(parser_state->buffered_output,"Unable to complete function definition, putting back tokens line %d..\n", parser_state->line_number);
			return 0;
		}
	}else{
		parser_progress("Expected declarator or declaration_specifiers for function definition.\n");
		return 0;
	}
}

struct parser_node * external_declaration(struct parser_state * parser_state){
	struct parser_node * n1;
	parser_progress("Attempting to build external_declaration\n");
	if((n1 = declaration(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, EXTERNAL_DECLARATION);
	}else if((n1 = function_definition(parser_state))){
		return create_parser_node(parser_state, 0, n1, 0, EXTERNAL_DECLARATION);
	}else{
		parser_progress("Expected external declaration.\n");
		return 0;
	}
}

struct normalized_declaration_element * get_most_recently_added_declaration_element_from_object(struct namespace_object * o){
	return struct_normalized_declaration_element_ptr_list_get(&o->elements, struct_normalized_declaration_element_ptr_list_size(&o->elements) -1);
}

struct parser_node * translation_unit_rest(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build translation_unit_rest\n");
	if((n1 = external_declaration(parser_state))){
		struct first_and_last_namespace_object fl = manage_generic_declaration(parser_state, n1, 0, 0, GLOBAL, 0, 0);
		if(fl.first && get_most_recently_added_declaration_element_from_object(fl.first)->normalized_declaration_set->set->type == FUNCTION_DEFINITION){
			struct scope_level * scope = get_last_function_namespace_scope(parser_state);
			struct current_function_change * change = malloc(sizeof(struct current_function_change));
			change->target = scope;
			change->old_obj = scope->current_function;
			change->new_obj = fl.first;
			push_operation(parser_state, SET_CURRENT_FUNCTION, change);
		}
		if((n1->next = translation_unit_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, TRANSLATION_UNIT_REST);
		}else{
			parser_progress("Expected translation_unit_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		struct parser_node * n = make_epsilon(parser_state);
		return create_parser_node(parser_state, 0, n, 0, TRANSLATION_UNIT_REST);
	}
}

struct parser_node * translation_unit(struct parser_state * parser_state){
	struct parser_node * n1;
	unsigned int checkpoint = struct_parser_operation_stack_size(&parser_state->operation_stack);
	parser_progress("Attempting to build translation_unit\n");
	if((n1 = external_declaration(parser_state))){
		struct first_and_last_namespace_object fl = manage_generic_declaration(parser_state, n1, 0, 0, GLOBAL, 0, 0);
		if(fl.first && get_most_recently_added_declaration_element_from_object(fl.first)->normalized_declaration_set->set->type == FUNCTION_DEFINITION){
			struct scope_level * scope = get_last_function_namespace_scope(parser_state);
			struct current_function_change * change = malloc(sizeof(struct current_function_change));
			change->target = scope;
			change->old_obj = scope->current_function;
			change->new_obj = fl.first;
			push_operation(parser_state, SET_CURRENT_FUNCTION, change);
		}
		if((n1->next = translation_unit_rest(parser_state))){
			return create_parser_node(parser_state, 0, n1, 0, TRANSLATION_UNIT);
		}else{
			parser_progress("Expected translation_unit_rest.\n");
			backtrack(parser_state, checkpoint);
			return 0;
		}
	}else{
		parser_progress("Expected external declaration.\n");
		return 0;
	}
}

void print_parser_node_tree(struct unsigned_char_list * buffer, struct parser_node * parent, struct parser_node * n, unsigned int level){
	/*  This print function is useful for debugging how the parser constructs the parser tree.  */
	if(n){
		char * parent_str;
		if(parent){
			parent_str = (char *)get_node_type_names()[parent->type];
		}else{
			parent_str = "Root";
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

	buffered_printf(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_printf(state->buffered_output,"-");
	buffered_printf(state->buffered_output,">BEGIN scope details\n");
	buffered_printf(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_printf(state->buffered_output,"-");
	buffered_printf(state->buffered_output,">Identifiers:\n");
	for(i = 0; i < struct_namespace_object_ptr_list_size(&scope->identifier_namespace); i++){
		struct namespace_object * namespace_object = struct_namespace_object_ptr_list_get(&scope->identifier_namespace, i);
		unsigned int k;
		for(k = 0; k < struct_normalized_declaration_element_ptr_list_size(&namespace_object->elements); k++){
			struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&namespace_object->elements, k);
			print_normalized_declaration_element(state->buffered_output, element, 0);
			buffered_printf(state->buffered_output,"\n");
		}
	}
	buffered_printf(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_printf(state->buffered_output,"-");

	buffered_printf(state->buffered_output,">Tags:\n");
	for(i = 0; i < struct_namespace_object_ptr_list_size(&scope->tag_namespace); i++){
		struct namespace_object * namespace_object = struct_namespace_object_ptr_list_get(&scope->tag_namespace, i);
		unsigned int k;
		for(k = 0; k < struct_normalized_declaration_element_ptr_list_size(&namespace_object->elements); k++){
			struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&namespace_object->elements, k);
			print_normalized_declaration_element(state->buffered_output, element, 0);
			buffered_printf(state->buffered_output,"\n");
		}
	}
		
	for(i = 0; i < scope->num_sub_scopes; i++){
		buffered_printf(state->buffered_output,";");
		for(j = 0; j < level; j++)
			buffered_printf(state->buffered_output,"-");
		buffered_printf(state->buffered_output,">%dth sub-scope of level %d:\n", i, level);
		print_scope_level(state, scope->scopes[i], level + 1);
	}
	buffered_printf(state->buffered_output,";");
	for(j = 0; j < level; j++)
		buffered_printf(state->buffered_output,"-");
	buffered_printf(state->buffered_output,">END scope details\n");
}

void destroy_scope_level(struct scope_level * scope, unsigned int level){
	unsigned int i;
	for(i = 0; i < scope->num_sub_scopes; i++){
		destroy_scope_level(scope->scopes[i], level + 1);
		free(scope->scopes[i]);
		free(scope->scopes);
	}
}

unsigned char * get_specifier_string(enum c_token_type t){
	switch(t){
		case VOID:{ return (unsigned char *)g_void_string; }
		case CHAR:{ return (unsigned char *)g_char_string; }
		case SHORT:{ return (unsigned char *)g_short_string; }
		case INT:{ return (unsigned char *)g_int_string; }
		case LONG:{ return (unsigned char *)g_long_string; }
		case FLOAT:{ return (unsigned char *)g_float_string; }
		case DOUBLE:{ return (unsigned char *)g_double_string; }
		case SIGNED:{ return (unsigned char *)g_signed_string; }
		case UNSIGNED:{ return (unsigned char *)g_unsigned_string; }
		case TYPEDEF:{ return (unsigned char *)g_typedef_string; }
		case EXTERN:{ return (unsigned char *)g_extern_string; }
		case STATIC:{ return (unsigned char *)g_static_string; }
		case AUTO:{ return (unsigned char *)g_auto_string; }
		case CONST:{ return (unsigned char *)g_const_string; }
		case VOLATILE:{ return (unsigned char *)g_volatile_string; }
		default:{ assert(0 && "Not possible."); return 0; }
	}
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
		default:{ assert(0 && "Not possible."); return 0; }
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
		default:{ assert(0 && "Not possible."); return 0; }
	}
}

struct type_description * create_empty_type_description(void){
	struct type_description * new_type = malloc(sizeof(struct type_description));
	struct struct_normalized_specifier_ptr_list * specifiers = malloc(sizeof(struct struct_normalized_specifier_ptr_list));
	struct_normalized_specifier_ptr_list_create(specifiers);
	new_type->specifiers = specifiers;
	new_type->declarator = 0;
	return new_type;
}

struct type_description * copy_type_description(struct type_description * type){
	return create_type_description_from_normalized_declarator_and_specifiers(type->declarator, type->specifiers);
}

int parse(struct c_lexer_state * c_lexer_state, struct parser_state * parser_state, unsigned char * buffer){
	struct type_description * const_char;
	g_parser_node_pool = memory_pooler_collection_get_pool(parser_state->memory_pooler_collection, sizeof(struct parser_node));
	g_c_lexer_token_pool = memory_pooler_collection_get_pool(parser_state->memory_pooler_collection, sizeof(struct c_lexer_token));
	parser_state->c_lexer_state = c_lexer_state;
	parser_state->line_number = 0;
	parser_state->tokens_position = 0;
	parser_state->current_scope_depth = 0;
	parser_state->top_scope = create_empty_scope(0);
	parser_state->constants = 0;
	parser_state->num_constants = 0;
	const_char = add_specifier(add_specifier(create_empty_type_description(), CONST), CHAR);

	parser_state->unsigned_int_description = add_specifier(add_specifier(create_empty_type_description(), UNSIGNED), INT);
	parser_state->const_char_ptr_description = create_address_type_description_from_type_description(const_char);
	parser_state->const_char_description = copy_type_description(const_char);
	struct_parser_operation_stack_create(&parser_state->operation_stack);
	g_format_buffer_use();

	parser_state->buff = buffer;
	char_ptr_list_create(&parser_state->string_literals);

	parser_state->top_node = translation_unit(parser_state);
	assert(parser_state->top_node && "Parsing FAILED.\n");
	print_scope_level(parser_state, parser_state->top_scope, 0);
	destroy_type_description(const_char);
	return 0;
}

void destroy_parser_state(struct parser_state * parser_state){
	unsigned int i;
	/* Undo every operation that was used to build up the entire parser state */
	backtrack(parser_state, 0);
	for(i = 0; i < char_ptr_list_size(&parser_state->string_literals); i++){
		free(char_ptr_list_get(&parser_state->string_literals, i));
	}
	destroy_empty_scope(parser_state->top_scope);
	char_ptr_list_destroy(&parser_state->string_literals);
	struct_parser_operation_stack_destroy(&parser_state->operation_stack);
	destroy_type_description(parser_state->unsigned_int_description);
	destroy_type_description(parser_state->const_char_description);
	destroy_type_description(parser_state->const_char_ptr_description);
	g_format_buffer_release();
}

const char ** get_node_type_names(void){
	return node_type_names;
}

unsigned int is_const(struct type_description * t){
	return !(count_specifiers(t, CONST));
}

unsigned int is_signed(struct type_description * t){
	return !(count_specifiers(t, UNSIGNED));
}

struct type_description * ensure_signed(struct type_description * t){
	/*  Make sure that this type has the specifiers require for it to be signed.
	Note that 'char' signedness is compiler defined.  We will define it to be signed.
	This function assumes it is passed an integer based type (char, short, int, long,
	long long) with a valid list of specifiers.  Undefined behaviour if passed floats etc.
	*/
	while(count_specifiers(t, UNSIGNED)){
		remove_specifier(t, 0, UNSIGNED);
	}
	return t;
}

struct type_description * ensure_unsigned(struct type_description * t){
	/*  Make sure that this type has the specifiers require for it to be unsigned.
	Note that 'char' signedness is compiler defined.  We will define it to be signed.
	This function assumes it is passed an integer based type (char, short, int, long,
	long long) with a valid list of specifiers.  Undefined behaviour if passed floats etc.
	*/
	while(count_specifiers(t, SIGNED)){
		remove_specifier(t, 0, SIGNED);
	}
	return add_specifier(t, UNSIGNED);
}

struct type_description * add_specifier(struct type_description * description, enum c_token_type t){
	unsigned char * string = get_specifier_string(t);
	struct c_lexer_token * new_token = memory_pooler_malloc(g_c_lexer_token_pool);
	struct parser_node * terminal = memory_pooler_malloc(g_parser_node_pool);
	struct parser_node * specifier = memory_pooler_malloc(g_parser_node_pool);
	struct normalized_specifier * normalized_specifier = malloc(sizeof(struct normalized_specifier));

	new_token->first_byte = string;
	new_token->last_byte = string + (strlen((char*)string) -1);
	new_token->type = t;

	terminal->c_lexer_token = new_token;
	terminal->first_child = 0;
	terminal->next = 0;
	terminal->type = TERMINAL;

	specifier->first_child = terminal;
	specifier->c_lexer_token = 0;
	specifier->next = 0;
	specifier->type = get_specifier_parser_node_type(t);

	normalized_specifier->specifier = specifier;
	normalized_specifier->type = get_normalized_specifier_type(t);
	struct_normalized_specifier_ptr_list_add(description->specifiers, normalized_specifier);
	return description;
}

void remove_specifier(struct type_description * description, unsigned int n, enum c_token_type t){
	/*  Remove the nth (zero based) normalized specifier that has type t */
	unsigned int count = 0;
	unsigned int k;
	validate_specifier_token_type(t);
	for(k = 0; k < struct_normalized_specifier_ptr_list_size(description->specifiers); k++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(description->specifiers, k);
		if(normalized_specifier->specifier->first_child->type == TERMINAL && normalized_specifier->specifier->first_child->c_lexer_token->type == t){
			if(count == n){
				normalized_specifier->specifier->next = 0;
				destroy_parser_node_tree_and_c_lexer_tokens(normalized_specifier->specifier);
				free(normalized_specifier);
				struct_normalized_specifier_ptr_list_remove_all(description->specifiers, normalized_specifier);
				return;
			}
			count++;
		}
	}
	assert(0 && "Did not find specifier.");
}

void remove_enum(struct type_description * description){
	unsigned int k;
	for(k = 0; k < struct_normalized_specifier_ptr_list_size(description->specifiers); k++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(description->specifiers, k);
		if(normalized_specifier->specifier->type == TYPE_SPECIFIER && normalized_specifier->specifier->first_child->type == ENUM_SPECIFIER){
			normalized_specifier->specifier->next = 0;
			destroy_parser_node_tree_and_c_lexer_tokens(normalized_specifier->specifier);
			free(normalized_specifier);
			struct_normalized_specifier_ptr_list_remove_all(description->specifiers, normalized_specifier);
			return;
		}
	}
	assert(0 && "Did not find enum specifier.");
}

void validate_specifier_token_type(enum c_token_type t){
	assert(t == TYPEDEF || t == VOID || t == CHAR || t == SHORT || t == INT || t == LONG || t == FLOAT || t == DOUBLE || t == SIGNED || t == UNSIGNED || t == IDENTIFIER);
}

unsigned int count_specifiers(struct type_description * description, enum c_token_type t){
	unsigned int rtn = 0;
	unsigned int k;
	for(k = 0; k < struct_normalized_specifier_ptr_list_size(description->specifiers); k++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(description->specifiers, k);
		if(normalized_specifier->specifier->first_child->type == TERMINAL && normalized_specifier->specifier->first_child->c_lexer_token->type == t){
			rtn++;
		}
	}
	return rtn;
}

enum type_class determine_type_class(struct type_description * t){
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
	struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(t->specifiers);
	struct parser_node * temp = create_abstract_declarator_from_normalized_declarator(t->declarator);
	if(is_pointer(temp)){
		destroy_parser_node_tree_and_c_lexer_tokens(temp);
		return TYPE_CLASS_POINTER;
	}
	if(is_array(temp)){
		destroy_parser_node_tree_and_c_lexer_tokens(temp);
		return TYPE_CLASS_ARRAY;
	}
	if(is_function(temp)){
		destroy_parser_node_tree_and_c_lexer_tokens(temp);
		return TYPE_CLASS_FUNCTION;
	}
	destroy_parser_node_tree_and_c_lexer_tokens(temp);
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
		assert(0 && "Unrecognized primative.");
	}
}



unsigned int get_ceil_modulo(unsigned int n, unsigned int modulo){
	unsigned int quotient = n / modulo;
	unsigned int remainder = n - (quotient * modulo);
	return modulo * (quotient + (remainder ? 1 : 0));
}

unsigned int arithmetic_type_size(struct type_description * t, enum type_size_class tsc){
	enum type_class type_class = determine_type_class(t);
	unsigned int address_bits = 32;
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
			print_normalized_declaration_declarator_and_specifiers(&l, t->declarator, t->specifiers, 0, 0);
			unsigned_char_list_add(&l, 0);
			printf("%s\n", unsigned_char_list_data(&l));
			assert(0 && "Unknown arithmetic type.");
		}
	}

	switch (tsc){
		case MINIMAL_BITS:{
			return data_bits;
		}case MINIMAL_BYTES:{
			return data_bits / 8;
		}case FORCED_WORD_BITS:{
			return get_ceil_modulo(data_bits, 32);
		}case FORCED_WORD_BYTES:{
			return get_ceil_modulo(data_bits, 32) / 8;
		}case DATA_AND_PTR_BITS:{
			return get_ceil_modulo(data_bits, 32) + address_bits;
		}case DATA_AND_PTR_BYTES:{
			return (get_ceil_modulo(data_bits, 32) + address_bits) / 8;
		}default:{
			assert(0 && "Unknown type class size.");
			return 0;
		}
	}
}

unsigned int void_type_size(enum type_size_class tsc){
	unsigned int address_bits = 32;
	unsigned int data_bits = 32;
	/*  Void isn't supposed to have a size, but for now it gets returned from void 
            functions as if were and int.  This makes certain things easier.  */

	switch (tsc){
		case MINIMAL_BITS:{
			return data_bits;
		}case MINIMAL_BYTES:{
			return data_bits / 8;
		}case FORCED_WORD_BITS:{
			return get_ceil_modulo(data_bits, 32);
		}case FORCED_WORD_BYTES:{
			return get_ceil_modulo(data_bits, 32) / 8;
		}case DATA_AND_PTR_BITS:{
			return get_ceil_modulo(data_bits, 32) + address_bits;
		}case DATA_AND_PTR_BYTES:{
			return (get_ceil_modulo(data_bits, 32) + address_bits) / 8;
		}default:{
			assert(0 && "Unknown type class size.");
			return 0;
		}
	}
}

unsigned int enum_type_size(struct type_description * t, enum type_size_class tsc){
	(void)t;
	switch (tsc){
		case MINIMAL_BITS:{
			return 32;
		}case MINIMAL_BYTES:{
			return 4;
		}case FORCED_WORD_BITS:{
			return 32;
		}case FORCED_WORD_BYTES:{
			return 4;
		}case DATA_AND_PTR_BITS:{
			return 64;
		}case DATA_AND_PTR_BYTES:{
			return 8;
		}default:{
			assert(0 && "Unknown type class size.");
			return 0;
		}
	}
}

unsigned int pointer_type_size(struct type_description * t, enum type_size_class tsc){
	(void)t;
	switch (tsc){
		case MINIMAL_BITS:{
			return 32;
		}case MINIMAL_BYTES:{
			return 4;
		}case FORCED_WORD_BITS:{
			return 32;
		}case FORCED_WORD_BYTES:{
			return 4;
		}case DATA_AND_PTR_BITS:{
			return 64;
		}case DATA_AND_PTR_BYTES:{
			return 8;
		}default:{
			assert(0 && "Unknown type class size.");
			return 0;
		}
	}
}

unsigned int is_integral_type(struct type_description * t){
	enum type_class c = determine_type_class(t);
	return (
		c == TYPE_CLASS_CHAR ||
		c == TYPE_CLASS_SHORT ||
		c == TYPE_CLASS_INT ||
		c == TYPE_CLASS_LONG ||
		c == TYPE_CLASS_LONG_LONG ||
		c == TYPE_CLASS_ENUM
	);
}

unsigned int is_floating_type(struct type_description * t){
	enum type_class c = determine_type_class(t);
	return (
		c == TYPE_CLASS_DOUBLE ||
		c == TYPE_CLASS_LONG_DOUBLE ||
		c == TYPE_CLASS_ENUM
	);
}

unsigned int is_arithmetic_type(struct type_description * t){
	return is_floating_type(t) || is_integral_type(t);
}

unsigned int is_scalar_type(struct type_description * t){
	enum type_class c = determine_type_class(t);
	return is_arithmetic_type(t) || c == TYPE_CLASS_POINTER;
}
