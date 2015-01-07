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

#include "code_generator.h"

void g_expression(struct parser_node *, struct code_gen_state *);
void g_expression_rest(struct parser_node *, struct code_gen_state *);
void g_declarator(struct parser_node *, struct code_gen_state *);
void g_initializer(struct parser_node *, struct code_gen_state *);
void g_struct_or_union_specifier(struct parser_node *, struct code_gen_state *);
void g_abstract_declarator(struct parser_node *, struct code_gen_state *);
void g_declaration_list(struct parser_node *, struct code_gen_state *);
void g_init_declarator(struct parser_node *, struct code_gen_state *);
void g_parameter_type_list(struct parser_node *, struct code_gen_state *);
void g_labeled_statement(struct parser_node *, struct code_gen_state *);
void g_compound_statement(struct parser_node *, struct code_gen_state *, struct parser_node *, unsigned char *, unsigned char *);
void g_expression_statement(struct parser_node *, struct code_gen_state *);
void g_selection_statement(struct parser_node *, struct code_gen_state *);
void g_iteration_statement(struct parser_node *, struct code_gen_state *);
void g_jump_statement(struct parser_node *, struct code_gen_state *);
void g_assignment_expression(struct parser_node *, struct code_gen_state *);
void g_cast_expression(struct parser_node *, struct code_gen_state *);
void g_primary_expression(struct parser_node *, struct code_gen_state *);
void g_epsilon(void);
unsigned int g_argument_expression_list_rest(struct parser_node *, struct code_gen_state *);
unsigned int g_argument_expression_list(struct parser_node *, struct code_gen_state *);
void g_postfix_expression_rest(struct parser_node *, struct code_gen_state *);
void g_postfix_expression(struct parser_node *, struct code_gen_state *);
void g_unary_operator(struct parser_node *, struct code_gen_state *);
void g_unary_expression(struct parser_node *, struct code_gen_state *);
void g_multiplicative_expression_rest(struct parser_node *, struct code_gen_state *);
void g_multiplicative_expression(struct parser_node *, struct code_gen_state *);
void g_additive_expression_rest(struct parser_node *, struct code_gen_state *);
void g_additive_expression(struct parser_node *, struct code_gen_state *);
void g_shift_expression_rest(struct parser_node *, struct code_gen_state *);
void g_shift_expression(struct parser_node *, struct code_gen_state *);
void g_relational_expression_rest(struct parser_node *, struct code_gen_state *);
void g_relational_expression(struct parser_node *, struct code_gen_state *);
void g_equality_expression_rest(struct parser_node *, struct code_gen_state *);
void g_equality_expression(struct parser_node *, struct code_gen_state *);
void g_and_expression_rest(struct parser_node *, struct code_gen_state *);
void g_and_expression(struct parser_node *, struct code_gen_state *);
void g_exclusive_or_expression_rest(struct parser_node *, struct code_gen_state *);
void g_exclusive_or_expression(struct parser_node *, struct code_gen_state *);
void g_inclusive_or_expression_rest(struct parser_node *, struct code_gen_state *);
void g_inclusive_or_expression(struct parser_node *, struct code_gen_state *);
void g_logical_and_expression_rest(struct parser_node *, struct code_gen_state *);
void g_logical_and_expression(struct parser_node *, struct code_gen_state *);
void g_logical_or_expression_rest(struct parser_node *, struct code_gen_state *);
void g_logical_or_expression(struct parser_node *, struct code_gen_state *);
void g_conditional_expression(struct parser_node *, struct code_gen_state *);
void g_assignment_operator(struct parser_node *, struct code_gen_state *);
void g_constant_expression(struct parser_node *, struct code_gen_state *);
void g_statement(struct parser_node *, struct code_gen_state *, unsigned char *, unsigned char *);
void g_statement_list_rest(struct parser_node *, struct code_gen_state *);
void g_statement_list(struct parser_node *, struct code_gen_state *);
void g_type_qualifier(struct parser_node *, struct code_gen_state *);
void g_storage_class_specifier(struct parser_node *, struct code_gen_state *);
void g_enumerator(struct parser_node *, struct code_gen_state *);
void g_enumerator_list_rest(struct parser_node *, struct code_gen_state *);
void g_enumerator_list(struct parser_node *, struct code_gen_state *);
void g_enum_specifier(struct parser_node *, struct code_gen_state *);
void g_struct_or_union(struct parser_node *, struct code_gen_state *);
void g_type_specifier(struct parser_node *, struct code_gen_state *);
void g_declaration_specifiers(struct parser_node *, struct code_gen_state *);
void g_init_declarator_list_rest(struct parser_node *, struct code_gen_state *);
void g_init_declarator_list(struct parser_node *, struct code_gen_state *);
void g_declaration(struct parser_node *, struct code_gen_state *);
void g_declaration_list_rest(struct parser_node *, struct code_gen_state *);
void g_declaration_list(struct parser_node *, struct code_gen_state *);
void g_identifier_list_rest(struct parser_node *, struct code_gen_state *);
void g_identifier_list(struct parser_node *, struct code_gen_state *);
void g_direct_declarator_rest(struct parser_node *, struct code_gen_state *);
void g_type_qualifier_list_rest(struct parser_node *, struct code_gen_state *);
void g_type_qualifier_list(struct parser_node *, struct code_gen_state *);
void g_pointer(struct parser_node *, struct code_gen_state *);
void g_direct_declarator(struct parser_node *, struct code_gen_state *);
void g_initializer_list_rest(struct parser_node *, struct code_gen_state *);
void g_initializer_list(struct parser_node *, struct code_gen_state *);
void g_struct_declarator(struct parser_node *, struct code_gen_state *);
void g_struct_declarator_list_rest(struct parser_node *, struct code_gen_state *);
void g_struct_declarator_list(struct parser_node *, struct code_gen_state *);
void g_specifier_qualifier_list(struct parser_node *, struct code_gen_state *);
void g_struct_declaration(struct parser_node *, struct code_gen_state *);
void g_struct_declaration_list_rest(struct parser_node *, struct code_gen_state *);
void g_struct_declaration_list(struct parser_node *, struct code_gen_state *);
void g_struct_or_union_specifier(struct parser_node *, struct code_gen_state *);
void g_parameter_declaration(struct parser_node *, struct code_gen_state *);
void g_parameter_list_rest(struct parser_node *, struct code_gen_state *);
void g_parameter_list(struct parser_node *, struct code_gen_state *);
void g_direct_abstract_declarator_rest(struct parser_node *, struct code_gen_state *);
void g_direct_abstract_declarator(struct parser_node *, struct code_gen_state *);
void g_function_definition(struct parser_node *, struct code_gen_state *);
void g_external_declaration(struct parser_node *, struct code_gen_state *);
void g_translation_unit_rest(struct parser_node *, struct code_gen_state *);
void g_translation_unit(struct parser_node *, struct code_gen_state *);
struct parser_node * first_child(struct parser_node *);
struct parser_node * second_child(struct parser_node *);
struct parser_node * third_child(struct parser_node *);
struct parser_node * fourth_child(struct parser_node *);
struct parser_node * fifth_child(struct parser_node *);
struct parser_node * sixth_child(struct parser_node *);
struct parser_node * seventh_child(struct parser_node *);
struct parser_node * is_first_child_type(struct parser_node *, enum node_type);
struct parser_node * is_second_child_type(struct parser_node *, enum node_type);
struct parser_node * is_third_child_type(struct parser_node *, enum node_type);
struct parser_node * is_fourth_child_type(struct parser_node *, enum node_type);
struct parser_node * is_fifth_child_type(struct parser_node *, enum node_type);
struct parser_node * is_sixth_child_type(struct parser_node *, enum node_type);
struct parser_node * is_seventh_child_type(struct parser_node *, enum node_type);
int check_one_child(struct parser_node *, enum node_type);
int check_two_children(struct parser_node *, enum node_type, enum node_type);
int check_three_children(struct parser_node *, enum node_type, enum node_type, enum node_type);
int check_four_children(struct parser_node *, enum node_type, enum node_type, enum node_type, enum node_type);
int check_five_children(struct parser_node *, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type);
int check_six_children(struct parser_node *, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type);
int check_seven_children(struct parser_node *, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type);
int is_terminal_c_token_type(struct parser_node *, enum c_token_type);

void function_call(struct parser_node *, struct parser_node *, struct code_gen_state *, struct parser_node *);
unsigned int get_word_size(void);
void go_down_scope(struct code_gen_state *);
void go_up_scope(struct code_gen_state *);
void create_default_return_value(struct code_gen_state *, struct parser_node *, struct parser_node *);
void do_default_function_return(struct code_gen_state *, struct parser_node *, struct parser_node *);
void return_from_function(struct code_gen_state *, struct parser_node *);
void load_identifier(struct code_gen_state *, unsigned char * identifier, struct parser_node *);
void push_type(struct code_gen_state *, struct type_description *, struct parser_node *);
struct type_description * pop_type(struct code_gen_state *, struct type_description *, struct parser_node *);
struct type_description * pop_type_without_type_check(struct code_gen_state *, struct parser_node *);
void backtrack_type_stack(struct code_gen_state *, unsigned int, struct parser_node *);
void do_assignment(struct code_gen_state *, struct parser_node *);

unsigned int get_parameter_offset(struct code_gen_state *, struct namespace_object *);
unsigned int get_local_offset_h1(struct code_gen_state *, struct namespace_object *, unsigned int);
unsigned int get_local_offset(struct code_gen_state *, struct namespace_object *);
unsigned int get_namespace_object_size(struct code_gen_state *, struct namespace_object *, unsigned int);
unsigned int get_normalized_declaration_element_size(struct code_gen_state *, struct normalized_declaration_element *, unsigned int, struct scope_level *);
void consume_scalar_type(struct code_gen_state *, struct parser_node *);
struct type_description * usual_arithmetic_conversion(struct code_gen_state *, struct parser_node *);
struct type_description * perform_pointer_conversion(struct code_gen_state *, struct parser_node *);

void require_external_symbol(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *, unsigned char * );
void implement_external_symbol(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *, unsigned char * );
void require_internal_symbol(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *, unsigned char * );
void implement_internal_symbol(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *, unsigned char * );
struct linker_symbol * make_linker_symbol(unsigned int, unsigned int, unsigned int, unsigned int);
void pop(struct code_gen_state *, const char *);
void push(struct code_gen_state *, const char *);
void do_signed_operation_function_call(struct code_gen_state *, const char *);
void manage_local_space(struct code_gen_state *, struct namespace_object *, unsigned);
struct scope_level * get_current_scope_level(struct code_gen_state *);
void clear_locals_from_scope(struct code_gen_state *, struct scope_level *);
void find_child_case_labels(struct code_gen_state *, struct parser_node *, struct switch_frame *);
void find_child_case_labels_h(struct code_gen_state *, struct parser_node *, struct switch_frame *);
void do_dot_operator(struct code_gen_state *, unsigned char *, struct parser_node *);
void do_ptr_operator(struct code_gen_state *, unsigned char *, struct parser_node *);
unsigned int calculate_type_stack_size(struct code_gen_state *);
void delete_top_type(struct code_gen_state *);
void copy_words(struct code_gen_state *, const char *, const char *, const char *, unsigned int);
struct type_description * manage_assignment_type_change(struct code_gen_state *, struct parser_node *);
void traverse_type(struct code_gen_state *, struct type_description *, const char *, const char *, const char *, struct scope_level *, enum traversal_type, unsigned int);
void handle_pointer_on_traversal(struct code_gen_state *, const char *, const char *, const char *, enum traversal_type);
void move_pointers_down(struct code_gen_state *, const char *, const char *);

unsigned int evaluate_constant_expression_h2(struct code_gen_state * code_gen_state, struct parser_node * n, unsigned int * rtn){
	if(!n) return 0;
	(void)code_gen_state;
	if(n->type == TERMINAL && n->c_lexer_token->type == CONSTANT_DECIMAL){
		unsigned char * str;
		unsigned int value;
		str = copy_string(n->c_lexer_token->first_byte, n->c_lexer_token->last_byte);
		value = convert_decimal_constant(str);
		free(str);
		*rtn = value;
		return 1;
	}else if(n->type == TERMINAL && n->c_lexer_token->type == IDENTIFIER){
		unsigned char * identifier = copy_string(n->c_lexer_token->first_byte, n->c_lexer_token->last_byte);
		struct namespace_object * obj;
		struct normalized_declaration_element * element;
		obj = get_namespace_object_from_closest_namespace(identifier, IDENTIFIER_NAMESPACE, get_current_scope_level(code_gen_state), 0);
		assert(obj);
		element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, 0);
		if(element->normalized_declarator && element->normalized_declarator->type == NORMALIZED_ENUMERATOR){
			*rtn = get_enum_value(element);
		}else{
			printf("Attempting to determine compile-time constant value of non enum identifier '%s'.\n", identifier);
			assert(0 && "Attempting to determine compile-time constant value of non enum identifier.");
		}
		free(identifier);
		return 1;
	}else{
		return 0;
	}
}

unsigned int evaluate_constant_expression_h1(struct code_gen_state * code_gen_state, struct parser_node * n, unsigned int * return_value){
	unsigned int success1 = 0;
	unsigned int success2 = 0;
	unsigned int success3 = 0;
	success1 = evaluate_constant_expression_h2(code_gen_state, n, return_value);
	if(n){
		success2 = evaluate_constant_expression_h1(code_gen_state, n->next, return_value);
		success3 = evaluate_constant_expression_h1(code_gen_state, n->first_child, return_value);
	}
	if((success1 + success2 + success3) > 1){
		assert(0 && "Compound constant expressions are not implemented.\n");
	}else{
		return (success1 + success2 + success3);
	}
}

unsigned int evaluate_constant_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	unsigned int return_value;
	assert(n && (n->type == CONSTANT_EXPRESSION || n->type == INITIALIZER));
	if(evaluate_constant_expression_h1(code_gen_state, n->first_child, &return_value)){
		return return_value;
	}else{
		assert(0 && "Unsupported constant expression.");
	}
}

void perform_integer_promotion(struct type_description * t){
	enum type_class c = determine_type_class(t);
	switch(c){
		case TYPE_CLASS_CHAR:{
			remove_specifier(t, 0, CHAR);
			add_specifier(t, INT);
			break;
		}case TYPE_CLASS_SHORT:{
			remove_specifier(t, 0, SHORT);
			if(!count_specifiers(t,INT)){
				add_specifier(t, INT);
			}
			break;
		}case TYPE_CLASS_ENUM:{
			remove_enum(t);
			add_specifier(t, INT);
			break;
		}default:{
			assert(0 && "Shound not call with this type");
			break;
		}
	}
}

struct type_description * usual_arithmetic_conversion(struct code_gen_state * code_gen_state, struct parser_node * context){
	/* Implements ISO/IEC 9899 6.3.1.8 Usual arithmetic conversions */
	struct parser_state * parser_state = code_gen_state->parser_state;
	struct type_description * t1 = pop_type_without_type_check(code_gen_state, context);
	struct type_description * t2 = pop_type_without_type_check(code_gen_state, context);

	enum type_class c1 = determine_type_class(t1);
	enum type_class c2 = determine_type_class(t2);

	if(c1 == TYPE_CLASS_CHAR || c1 == TYPE_CLASS_SHORT || c1 == TYPE_CLASS_ENUM){
		perform_integer_promotion(t1);
	}

	if(c2 == TYPE_CLASS_CHAR || c2 == TYPE_CLASS_SHORT || c2 == TYPE_CLASS_ENUM){
		perform_integer_promotion(t2);
	}

	/*  These may have changed after the integer type promotion */
	c1 = determine_type_class(t1);
	c2 = determine_type_class(t2);

	if(is_signed(t1) == is_signed(t2) && c1 == c2){
		destroy_type_description(t1);
		return t2;
	}

	if((is_signed(t1) && !is_signed(t2)) || (is_signed(t2) && !is_signed(t1))){
		if(c1 == TYPE_CLASS_INT && c2 == TYPE_CLASS_INT){
			destroy_type_description(t1);
			destroy_type_description(t2);
			return add_specifier(add_specifier(create_empty_type_description(), UNSIGNED), INT);
		}else{
			assert(0);
			return 0;
		}
	}else{
		struct unsigned_char_list l;
		unsigned_char_list_create(&l);
		buffered_printf(&l,"\nHere are the two types we're doing usual arithmetic conversion on:\n");
		buffered_printf(&l,"Type #1:\n");
		print_normalized_declaration_declarator_and_specifiers(&l, t1->declarator, t1->specifiers, 0, 0);
		buffered_printf(&l,"Type #2:\n");
		print_normalized_declaration_declarator_and_specifiers(&l, t2->declarator, t2->specifiers, 0, 0);
		unsigned_char_list_add(&l, 0);
		printf("%s\n", unsigned_char_list_data(&l));
		print_node_context(parser_state->c_lexer_state, context);
		assert(0 && "This case is not supported.\n");
	}
}

void push_type(struct code_gen_state * code_gen_state, struct type_description * type, struct parser_node * context){
	(void)context;
	assert(type);
	struct_type_description_ptr_list_add(&code_gen_state->type_stack, type);
}

struct type_description * pop_type_without_type_check(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * found_type = struct_type_description_ptr_list_pop(&code_gen_state->type_stack);
	(void)context;
	return found_type;
}

struct type_description * pop_type(struct code_gen_state * code_gen_state, struct type_description * required_type, struct parser_node * context){
	struct type_description * found_type = struct_type_description_ptr_list_pop(&code_gen_state->type_stack);
	unsigned int compare_successful = 0;
	assert(required_type);
	assert(found_type);
	compare_successful = !type_description_cmp(required_type, found_type);
	if(!compare_successful){
		struct unsigned_char_list list;
		unsigned_char_list_create(&list);
		buffered_printf(&list,";Type missmatch:\n");
		buffered_printf(&list,";Compare required type A:\n");
		print_normalized_declaration_declarator_and_specifiers(&list, required_type->declarator, required_type->specifiers, 0, 0);
		buffered_printf(&list,";With observed type B:\n");
		print_normalized_declaration_declarator_and_specifiers(&list, found_type->declarator, found_type->specifiers, 0, 0);
		buffered_printf(&list,";Type compare failed\n");
		unsigned_char_list_add(&list, 0);
		printf("%s", (unsigned char *)unsigned_char_list_data(&list));
		unsigned_char_list_destroy(&list);
		print_node_context(code_gen_state->parser_state->c_lexer_state, context);
		assert(0 && "Compare failed.");
	}
	return found_type;
}

void backtrack_type_stack(struct code_gen_state * code_gen_state, unsigned int target, struct parser_node * context){
	while(struct_type_description_ptr_list_size(&code_gen_state->type_stack) != target){
		destroy_type_description(pop_type_without_type_check(code_gen_state, context));
	}
}



unsigned int get_word_size(void){
	/* Right now we're using 32 bit */
	return 4;
}

struct parser_node * first_child(struct parser_node * p){
	return p->first_child;
}

struct parser_node * second_child(struct parser_node * p){
	return p->first_child->next;
}

struct parser_node * third_child(struct parser_node * p){
	return p->first_child->next->next;
}

struct parser_node * fourth_child(struct parser_node * p){
	return p->first_child->next->next->next;
}

struct parser_node * fifth_child(struct parser_node * p){
	return p->first_child->next->next->next->next;
}

struct parser_node * sixth_child(struct parser_node * p){
	return p->first_child->next->next->next->next->next;
}

struct parser_node * seventh_child(struct parser_node * p){
	return p->first_child->next->next->next->next->next->next;
}

struct parser_node * is_first_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		first_child(p)->type == t
	) ? first_child(p) : 0;
}

struct parser_node * is_second_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		second_child(p)->type == t
	) ? second_child(p) : 0;
}

struct parser_node * is_third_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		third_child(p) &&
		third_child(p)->type == t
	) ? third_child(p) : 0;
}

struct parser_node * is_fourth_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		third_child(p) &&
		fourth_child(p) &&
		fourth_child(p)->type == t
	) ? fourth_child(p) : 0;
}

struct parser_node * is_fifth_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		third_child(p) &&
		fourth_child(p) &&
		fifth_child(p) &&
		fifth_child(p)->type == t
	) ? fifth_child(p) : 0;
}

struct parser_node * is_sixth_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		third_child(p) &&
		fourth_child(p) &&
		fifth_child(p) &&
		sixth_child(p) &&
		sixth_child(p)->type == t
	) ? sixth_child(p) : 0;
}

struct parser_node * is_seventh_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		third_child(p) &&
		fourth_child(p) &&
		fifth_child(p) &&
		sixth_child(p) &&
		seventh_child(p) &&
		seventh_child(p)->type == t
	) ? seventh_child(p) : 0;
}

int check_one_child(struct parser_node * p, enum node_type t1){
	return (is_first_child_type(p, t1) && 1);
}

int check_two_children(struct parser_node * p, enum node_type t1, enum node_type t2){
	return (
		is_first_child_type(p, t1) &&
		is_second_child_type(p, t2)
	);
}

int check_three_children(struct parser_node * p, enum node_type t1, enum node_type t2, enum node_type t3){
	return (
		is_first_child_type(p, t1) &&
		is_second_child_type(p, t2) &&
		is_third_child_type(p, t3)
	);
}

int check_four_children(struct parser_node * p, enum node_type t1, enum node_type t2, enum node_type t3, enum node_type t4){
	return (
		is_first_child_type(p, t1) &&
		is_second_child_type(p, t2) &&
		is_third_child_type(p, t3) &&
		is_fourth_child_type(p, t4)
	);
}

int check_five_children(struct parser_node * p, enum node_type t1, enum node_type t2, enum node_type t3, enum node_type t4, enum node_type t5){
	return (
		is_first_child_type(p, t1) &&
		is_second_child_type(p, t2) &&
		is_third_child_type(p, t3) &&
		is_fourth_child_type(p, t4) &&
		is_fifth_child_type(p, t5)
	);
}

int check_six_children(struct parser_node * p, enum node_type t1, enum node_type t2, enum node_type t3, enum node_type t4, enum node_type t5, enum node_type t6){
	return (
		is_first_child_type(p, t1) &&
		is_second_child_type(p, t2) &&
		is_third_child_type(p, t3) &&
		is_fourth_child_type(p, t4) &&
		is_fifth_child_type(p, t5) &&
		is_sixth_child_type(p, t6)
	);
}

int check_seven_children(struct parser_node * p, enum node_type t1, enum node_type t2, enum node_type t3, enum node_type t4, enum node_type t5, enum node_type t6, enum node_type t7){
	return (
		is_first_child_type(p, t1) &&
		is_second_child_type(p, t2) &&
		is_third_child_type(p, t3) &&
		is_fourth_child_type(p, t4) &&
		is_fifth_child_type(p, t5) &&
		is_sixth_child_type(p, t6) &&
		is_seventh_child_type(p, t7)
	);
}

int is_terminal_c_token_type(struct parser_node * p, enum c_token_type t){
	return p->c_lexer_token && p->c_lexer_token->type == t;
}

unsigned int struct_type_size(struct code_gen_state * code_gen_state, struct type_description * t, enum type_size_class tsc, struct scope_level * source_scope_level){
	struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(t->specifiers);
	struct c_lexer_token * token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
	unsigned char * tag_identifier = copy_string(token->first_byte, token->last_byte);
	unsigned int num_children;
	struct namespace_object * obj;
	struct normalized_declaration_element * element;
	unsigned int i;
	unsigned int total_size = 0;

	obj = get_namespace_object_from_closest_namespace(tag_identifier, TAG_NAMESPACE, source_scope_level, 1);
	if(!obj){
		printf("Unknown structure: %s in file %s.\n", tag_identifier, code_gen_state->parser_state->c_lexer_state->c.filename);
		assert(obj && "Unknown identifier.");
	}

	num_children = struct_namespace_object_ptr_list_size(obj->children);
	assert(num_children);

	for(i = num_children - 1; ; i--){
		struct namespace_object * child_obj = struct_namespace_object_ptr_list_get(obj->children, i);
		unsigned int size;
		struct type_description * member_description;
		assert(struct_normalized_declaration_element_ptr_list_size(&child_obj->elements));
		element = struct_normalized_declaration_element_ptr_list_get(&child_obj->elements, 0);
		member_description = create_type_description_from_normalized_declaration_element(element);
		size = type_size(code_gen_state, member_description, tsc, 0, obj->scope_level);
		total_size += size;
		destroy_type_description(member_description);
		if(i == 0){
			break;
		}
	}
	free(tag_identifier);
	return total_size;
}

void handle_pointer_on_traversal(struct code_gen_state * code_gen_state, const char * src, const char * dst, const char * tmp1, enum traversal_type traversal_type){
	(void)src;
	(void)dst;
	(void)tmp1;
	switch(traversal_type){
		case ASSIGN_COPY:{
			/* Do nothing, leave the pointer alone */
			buffered_printf(code_gen_state->buffered_output, ";                 Leave pointer alone\n");
			break;
		}case NEW_COPY:{
			buffered_printf(code_gen_state->buffered_output, "sto %s %s;        New item, point to self\n", dst, dst);
			break;
		}case SETUP_GLOBAL_SPACE:{
			unsigned char * name;
			sprintf_hook("global_var_ptr_%d", code_gen_state->global_var_ptr_index);
			name = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			buffered_printf(code_gen_state->buffered_output, "%s:\n", name);
			buffered_printf(code_gen_state->buffered_output, "dw %s; Ptr to item.\n", name);
			code_gen_state->global_var_ptr_index = code_gen_state->global_var_ptr_index + 1;
			require_internal_symbol(&code_gen_state->symbols, name);
			implement_internal_symbol(&code_gen_state->symbols, name);
			free(name);
			break;
		}case REF_COPY:{
			buffered_printf(code_gen_state->buffered_output, "loa %s %s;\n", tmp1, src);
			buffered_printf(code_gen_state->buffered_output, "sto %s %s;\n", dst, tmp1);
			break;
		}case CONVERT_TO_VALUE:{
			buffered_printf(code_gen_state->buffered_output, "sto %s %s;        Convert to by value for function call.\n", dst, dst);
			break;
		}default:{
			assert(0 && "Case not expected.");
			break;
		}
	}
}

void move_pointers_down(struct code_gen_state * code_gen_state, const char * ra, const char * rb){
	buffered_printf(code_gen_state->buffered_output, "sub %s %s WR;\n", ra, ra);
	buffered_printf(code_gen_state->buffered_output, "sub %s %s WR;\n", rb, rb);
}

void traverse_type(struct code_gen_state * code_gen_state, struct type_description * t, const char * src, const char * dst, const char * tmp1, struct scope_level * source_scope_level, enum traversal_type traversal_type, unsigned int arrays_as_pointers){
	/*  src should point to where the bits come from, and dst should point to where they will be copied. tmp1 is for temporary store of word as it is copied */
	enum type_class c = determine_type_class(t);
	if(c == TYPE_CLASS_POINTER){
		if(traversal_type == SETUP_GLOBAL_SPACE){
			handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
			buffered_printf(code_gen_state->buffered_output, "dw 0x0;\n");
		}else{
			if(traversal_type != CONVERT_TO_VALUE){
				copy_words(code_gen_state, src, dst, tmp1, pointer_type_size(t, FORCED_WORD_BYTES));
			}
			move_pointers_down(code_gen_state, dst, src);
			handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
		}
		return;
	}

	if(c == TYPE_CLASS_ARRAY){
		struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(t->declarator);
		struct parser_node * constant_expression = get_constant_expression_from_abstract_declarator(abstract_declarator);
		if(constant_expression){
			unsigned int size;
			struct type_description * arr_type = create_dereferenced_array_type_description_from_type_description(t);
			unsigned element_size = type_size(code_gen_state, arr_type, FORCED_WORD_BYTES, 0, source_scope_level);
			enum type_class element_type = determine_type_class(arr_type);
			unsigned int i;
			assert(constant_expression->type == CONSTANT_EXPRESSION);
			size = evaluate_constant_expression(code_gen_state, constant_expression);
			destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);

			if(arrays_as_pointers){
				/* Reduce the entire array to the pointer to the first element */
				if(traversal_type == CONVERT_TO_VALUE){
					move_pointers_down(code_gen_state, dst, src);
					handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
				}else{
					buffered_printf(code_gen_state->buffered_output, "ll %s 0x%X;\n", tmp1, 4);
					buffered_printf(code_gen_state->buffered_output, "sub %s %s %s;\n", src, src, tmp1);
					buffered_printf(code_gen_state->buffered_output, "loa %s %s; Copy ptr\n", tmp1, src);
					buffered_printf(code_gen_state->buffered_output, "sto %s %s; Store as val\n", dst, tmp1);
					buffered_printf(code_gen_state->buffered_output, "sub %s %s WR;\n", dst, dst);
					buffered_printf(code_gen_state->buffered_output, "sto %s %s; Store as ptr\n", dst, tmp1);
				}
			}else{
				for(i = 0; i < size; i++){
					/*  If the item type is an atomic type, recurse otherwise copy */
					if(element_type == TYPE_CLASS_ARRAY || element_type == TYPE_CLASS_STRUCT){
						traverse_type(code_gen_state, arr_type, src, dst, tmp1, source_scope_level, traversal_type, arrays_as_pointers);
					}else{
						if(traversal_type == SETUP_GLOBAL_SPACE){
							handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
							buffered_printf(code_gen_state->buffered_output, "dw 0x0;\n");
						}else{
							if(traversal_type != CONVERT_TO_VALUE){
								copy_words(code_gen_state, src, dst, tmp1, element_size);
							}
							move_pointers_down(code_gen_state, dst, src);
							handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
						}
					}
					if(i != (size -1)){
						if(traversal_type != SETUP_GLOBAL_SPACE){
							move_pointers_down(code_gen_state, dst, src);
						}
					}
				}
			}

			destroy_type_description(arr_type);
			return;
		}else{
			destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);
			assert(0 && "not implemented.");
			return;
		}
	}

	if(c == TYPE_CLASS_FUNCTION){
		assert(0 && "Not implemented.");
		return;
	}

	if(c == TYPE_CLASS_STRUCT){
		struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(t->specifiers);
		struct c_lexer_token * token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
		unsigned char * tag_identifier = copy_string(token->first_byte, token->last_byte);
		unsigned int num_children;
		struct namespace_object * obj;
		struct normalized_declaration_element * element;
		unsigned int i;
		obj = get_namespace_object_from_closest_namespace(tag_identifier, TAG_NAMESPACE, source_scope_level, 1);
		if(!obj){
			printf("Unknown structure: %s in file %s.\n", tag_identifier, code_gen_state->parser_state->c_lexer_state->c.filename);
			assert(obj && "Unknown identifier.");
		}

		num_children = struct_namespace_object_ptr_list_size(obj->children);
		assert(num_children);

		for(i = 0; i < num_children; i++){
			struct namespace_object * child_obj = struct_namespace_object_ptr_list_get(obj->children, i);
			struct type_description * member_description;
			assert(struct_normalized_declaration_element_ptr_list_size(&child_obj->elements));
			element = struct_normalized_declaration_element_ptr_list_get(&child_obj->elements, 0);
			member_description = create_type_description_from_normalized_declaration_element(element);

			/* Traverse structure member, doing deep copy of arrays if necessary */
			buffered_printf(code_gen_state->buffered_output, ";                 Begin copy of struct member %d of %s\n", i, tag_identifier);
			traverse_type(code_gen_state, member_description, src, dst, tmp1, source_scope_level, traversal_type, 0);
			buffered_printf(code_gen_state->buffered_output, ";                 Finish copy of struct member %d of %s\n", i, tag_identifier);
			if(i != (num_children -1)){
				if(traversal_type != SETUP_GLOBAL_SPACE){
					move_pointers_down(code_gen_state, dst, src);
				}
			}

			destroy_type_description(member_description);
		}
		free(tag_identifier);
		return;
	}else if(c == TYPE_CLASS_UNION){
		assert(0);
		return;
	}else if(c == TYPE_CLASS_ENUM){
		if(traversal_type == SETUP_GLOBAL_SPACE){
			handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
			buffered_printf(code_gen_state->buffered_output, "dw 0x0;\n");
		}else{
			if(traversal_type != CONVERT_TO_VALUE){
				copy_words(code_gen_state, src, dst, tmp1, pointer_type_size(t, FORCED_WORD_BYTES));
			}
			move_pointers_down(code_gen_state, dst, src);
			handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
		}
		return;
	}else if(c == TYPE_CLASS_VOID){
		if(traversal_type == SETUP_GLOBAL_SPACE){
			handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
			buffered_printf(code_gen_state->buffered_output, "dw 0x0;\n");
		}else{
			if(traversal_type != CONVERT_TO_VALUE){
				copy_words(code_gen_state, src, dst, tmp1, pointer_type_size(t, FORCED_WORD_BYTES));
			}
			move_pointers_down(code_gen_state, dst, src);
			handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
		}
		return;
	}else{
		if(traversal_type == SETUP_GLOBAL_SPACE){
			handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
			buffered_printf(code_gen_state->buffered_output, "dw 0x0;\n");
		}else{
			if(traversal_type != CONVERT_TO_VALUE){
				copy_words(code_gen_state, src, dst, tmp1, pointer_type_size(t, FORCED_WORD_BYTES));
			}
			move_pointers_down(code_gen_state, dst, src);
			handle_pointer_on_traversal(code_gen_state, src, dst, tmp1, traversal_type);
		}
		return;
	}
}

unsigned int type_size(struct code_gen_state * code_gen_state, struct type_description * t, enum type_size_class tsc, unsigned int force_arity_1, struct scope_level * source_scope_level){
	enum type_class c = determine_type_class(t);
	if(c == TYPE_CLASS_POINTER){
		return pointer_type_size(t, tsc);
	}

	if(c == TYPE_CLASS_ARRAY){
		struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(t->declarator);
		struct parser_node * constant_expression = get_constant_expression_from_abstract_declarator(abstract_declarator);
		if(constant_expression){
			unsigned int size;
			struct type_description * arr_type = create_dereferenced_array_type_description_from_type_description(t);
			unsigned element_size = type_size(code_gen_state, arr_type, tsc, 0, source_scope_level);
			assert(constant_expression->type == CONSTANT_EXPRESSION);
			size = evaluate_constant_expression(code_gen_state, constant_expression);
			destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);
			if(force_arity_1){
				struct type_description * ptr_type = create_address_type_description_from_type_description(arr_type);
				unsigned int rtn = pointer_type_size(ptr_type, tsc);
				destroy_type_description(arr_type);
				destroy_type_description(ptr_type);
				return rtn;
			}
			destroy_type_description(arr_type);
			return element_size * size;
		}else{
			destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);
			return 8; /* TODO: 8 is wrong. This case is when an array was declared as [] without an expression */
		}
	}

	if(c == TYPE_CLASS_FUNCTION){
		assert(0 && "Not implemented.");
		return 0;
	}

	if(c == TYPE_CLASS_STRUCT){
		return struct_type_size(code_gen_state, t, tsc, source_scope_level);
	}else if(c == TYPE_CLASS_UNION){
		assert(0);
		return 8; /* Works sometimes */
	}else if(c == TYPE_CLASS_ENUM){
		return enum_type_size(t, tsc);
	}else if(c == TYPE_CLASS_VOID){
		return void_type_size(tsc);
	}else{
		return arithmetic_type_size(t, tsc);
	}
}

unsigned int get_normalized_declaration_element_size(struct code_gen_state * code_gen_state, struct normalized_declaration_element * element, unsigned int force_arity_1, struct scope_level * source_scope_level){
	unsigned int rtn;
	struct type_description * type_description = create_type_description_from_normalized_declaration_element(element);
	type_description->source_scope_level = source_scope_level;
	convert_to_untypedefed_type_description(type_description);
	rtn = type_size(code_gen_state, type_description, DATA_AND_PTR_BYTES, force_arity_1, source_scope_level);
	destroy_type_description(type_description);
	return rtn;
}

unsigned int get_namespace_object_size(struct code_gen_state * code_gen_state, struct namespace_object * obj, unsigned int force_arity_1){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, num_elements - 1);
	return get_normalized_declaration_element_size(code_gen_state, element, force_arity_1, obj->scope_level);
}

unsigned int get_parameter_offset(struct code_gen_state * code_gen_state, struct namespace_object * obj){
	if(obj){
		return get_namespace_object_size(code_gen_state, obj, 0) + get_parameter_offset(code_gen_state, obj->previous);
	}else{
		return 0;
	}
}

unsigned int get_local_offset(struct code_gen_state * code_gen_state, struct namespace_object * obj){
	/*  There can be multiple 'frames' of locals set up inside a function from ifs and fors.  Referencing
	variables in outer scopes means we need to add up the size of these frames to get the correct offset
	of the outer variable.
        */
        unsigned int rtn = 0;
        struct scope_level * current_scope = get_current_scope_level(code_gen_state); /* Start adding at the current scope */
        struct scope_level * variable_scope = obj->scope_level;  /*  And go all the way up to the variable we're loading */
        struct scope_level * in_progress = current_scope;
        while(1){
		/*  For the scope of the target variable only, look at the space of variables after the variable, not ones before */
		struct namespace_object * start_obj = (in_progress == variable_scope) ? obj: in_progress->first_local_object;
		if(start_obj){
			unsigned int a;
			a = get_local_offset_h1(code_gen_state, start_obj, 1);
			buffered_printf(code_gen_state->buffered_output, ";Total offset for scope was %d\n", a);
			rtn += a;
		}else{
			buffered_printf(code_gen_state->buffered_output,";Nothing to add for this scope\n");
		}
		if(in_progress == variable_scope){
			break;
		}
		in_progress = in_progress->parent_scope;
        }
        return rtn;
}

unsigned int get_local_offset_h1(struct code_gen_state * code_gen_state, struct namespace_object * obj, unsigned int force_arity_1){
	/* Add up the sizes of all initialized locals that occur after the referenced symbol */
	if(obj && !obj->first_load){
		unsigned int a;
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, 0);
		struct c_lexer_token * token = get_identifier_token_from_normalized_declarator(element->normalized_declarator);
		unsigned char * identifier;
		identifier = copy_string(token->first_byte, token->last_byte);
		a = get_namespace_object_size(code_gen_state, obj, force_arity_1);
		buffered_printf(code_gen_state->buffered_output, ";Add offset of %d due to local %s\n", a, identifier);
		free(identifier);
		return a + get_local_offset_h1(code_gen_state, obj->next, 0);
	}else{
		return 0;
	}
}

unsigned int calculate_type_stack_size(struct code_gen_state * code_gen_state){
	unsigned int num_types = struct_type_description_ptr_list_size(&code_gen_state->type_stack);
	unsigned int i;
	unsigned total_size = 0;
	for(i = 0; i < num_types; i++){
		struct type_description * t = struct_type_description_ptr_list_get(&code_gen_state->type_stack, i);
		unsigned int current_type_size = type_size(code_gen_state, t, DATA_AND_PTR_BYTES, 1, get_current_scope_level(code_gen_state));
		total_size += current_type_size;
		buffered_printf(code_gen_state->buffered_output, "; Temps type size: %X\n", current_type_size);
	}
	return total_size;
}

void delete_top_type(struct code_gen_state * code_gen_state){
	struct type_description * t;
	unsigned int size;
	unsigned int num_words;
	unsigned int i;
	assert(struct_type_description_ptr_list_size(&code_gen_state->type_stack));
	t = struct_type_description_ptr_list_pop(&code_gen_state->type_stack);
	size = type_size(code_gen_state, t, DATA_AND_PTR_BYTES, 1, get_current_scope_level(code_gen_state));
	num_words = size / 4;
	assert(size - (4*num_words) == 0);
	for(i = 0; i < num_words; i++){
		buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Deleting top type word %d.\n", i);
	}
	destroy_type_description(t);
}

void copy_words(struct code_gen_state * code_gen_state, const char * src_addr, const char * dst_addr, const char * tmp, unsigned int bytes_to_copy){
	/*  src_addr is a string that represents the register containing the address of bytes to copy */
	/*  Copy starts at high address and goes down for stack and src_addr  */
	unsigned int words_to_copy = bytes_to_copy / 4;
	unsigned int i;
	assert((bytes_to_copy - (words_to_copy * 4)) == 0);
	for(i = 0; i < (words_to_copy -1); i++){
		buffered_printf(code_gen_state->buffered_output,"loa %s %s;\n", tmp, src_addr);
		buffered_printf(code_gen_state->buffered_output,"sto %s %s;\n", dst_addr, tmp);
		buffered_printf(code_gen_state->buffered_output,"sub %s %s WR;\n", dst_addr, dst_addr);
		buffered_printf(code_gen_state->buffered_output,"sub %s %s WR;\n", src_addr, src_addr);
	}
	buffered_printf(code_gen_state->buffered_output,"loa %s %s;\n", tmp, src_addr);
	buffered_printf(code_gen_state->buffered_output,"sto %s %s;\n", dst_addr, tmp);
}

void load_identifier(struct code_gen_state * code_gen_state, unsigned char * identifier, struct parser_node * context){
	/*  Obtained a stack where the first element is the closest scope */
	struct parser_state * parser_state = code_gen_state->parser_state;
	struct namespace_object * obj;
	struct parser_node * abstract_declarator;
	struct type_description * type_description;
	unsigned int num_elements;
	struct normalized_declaration_element * element;
	obj = get_namespace_object_from_closest_namespace(identifier, IDENTIFIER_NAMESPACE, get_current_scope_level(code_gen_state), 0);
	if(!obj){
		printf("Unknown identifier: %s in file %s.\n", identifier, parser_state->c_lexer_state->c.filename);
		assert(obj && "Unknown identifier.");
	}
	num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, num_elements - 1);
	buffered_printf(code_gen_state->buffered_output,"; Loading identifier %s ", identifier);
	buffered_printf(code_gen_state->buffered_output,"from location %d\n", obj->object_location);

	type_description = create_type_description_from_normalized_declaration_element(element);
	type_description->source_scope_level = obj->scope_level;
	convert_to_untypedefed_type_description(type_description);

	abstract_declarator = create_abstract_declarator_from_normalized_declarator(element->normalized_declarator);
	switch(obj->object_location){
		case GLOBAL:{
			if(is_function(abstract_declarator)){
				struct type_description * ptr = create_address_type_description_from_type_description(type_description);
				buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Now pointing to variable value\n");
				buffered_printf(code_gen_state->buffered_output,"ll r1 %s;  Load the variable %s\n", identifier, identifier); 
				buffered_printf(code_gen_state->buffered_output,"sto SP r1;  Store it on the stack.\n");
				buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Now pointing to variabl ptr\n");
				buffered_printf(code_gen_state->buffered_output,"sto SP SP;  Point to itself.\n");
				push_type(code_gen_state, ptr, context);
				destroy_type_description(type_description);
				require_external_symbol(&code_gen_state->symbols, identifier);
			}else{
				unsigned char * name;

				unsigned int g_size = type_size(code_gen_state, type_description, DATA_AND_PTR_BYTES, 1, type_description->source_scope_level);
				sprintf_hook("globalvar_%s", identifier);
				name = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));

				buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Now pointing to value\n");
				buffered_printf(code_gen_state->buffered_output,"ll r1 %s;  Load the global var addr %s\n", name, name); 
				buffered_printf(code_gen_state->buffered_output,"ll r2 0x%x;  Offset to last word value.\n", g_size -4); 
				buffered_printf(code_gen_state->buffered_output,"add r1 r1 r2;  addr of last word value.\n"); 

				traverse_type(code_gen_state, type_description, "r1", "SP", "r2", type_description->source_scope_level, REF_COPY, 1);

				push_type(code_gen_state, type_description, context);
				require_external_symbol(&code_gen_state->symbols, name);
				free(name);
			}
			break;
		}case PARAMETER:{
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; This is the number of bytes up from the FP where the value is stored\n", (get_parameter_offset(code_gen_state, obj->previous) + 4));
			buffered_printf(code_gen_state->buffered_output,"sub r1 FP r1; r1 now stores the addr of the value of the argument.\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Get stack ready\n");
			traverse_type(code_gen_state, type_description, "r1", "SP", "r2", type_description->source_scope_level, REF_COPY, 1);
			push_type(code_gen_state, type_description, context);
			break;
		}case LOCAL:{
			unsigned int temps_size = calculate_type_stack_size(code_gen_state);
			unsigned int current_obj_size;
			unsigned int offset;
			(void)current_obj_size;
			/*  The first time we reference the local is when we're setting up the locals on the stack, so
			the offset from the current SP will be less  */
			if(obj->first_load){
				manage_local_space(code_gen_state, obj, 1);
				obj->first_load = 0;
			}
			offset = get_local_offset(code_gen_state, obj);
			current_obj_size = get_namespace_object_size(code_gen_state, obj, 1);
			/* - 4 because we start at the obj value */
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; Offset due to locals (%d) - 4 + %d for temps\n", (offset - 4) + temps_size, offset, temps_size);
			buffered_printf(code_gen_state->buffered_output,"add r1 SP r1; r1 now stores the addr of the value of the local.\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Go up to where new value gets copied.\n");
			traverse_type(code_gen_state, type_description, "r1", "SP", "r2", type_description->source_scope_level, REF_COPY, 1);
			push_type(code_gen_state, type_description, context);
			break;
		}case ENUM_IDENTIFIER:{
			unsigned int enum_value = get_enum_value(element);
			buffered_printf(code_gen_state->buffered_output,";Load our enum value\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Point to new value\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; load enum integer value\n", enum_value);
			buffered_printf(code_gen_state->buffered_output,"sto SP r1; Store variable ptr in value too\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Point to address\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP; Point to iteself\n");
			push_type(code_gen_state, type_description, context);
			break;
		}case LOCATION_STRUCT:{
			destroy_type_description(type_description);
			break;
		}default:{
			assert(0 && "Unknown object location.");
		}

	}
	destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);
}

void g_primary_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	struct parser_state * parser_state = code_gen_state->parser_state;
	if(check_three_children(p, TERMINAL, EXPRESSION, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_PAREN_CHAR)
		){
			g_expression(second_child(p), code_gen_state);
		}else{
			buffered_printf(code_gen_state->buffered_output,"Did not find parens: %d, %d.\n",first_child(p)->c_lexer_token->type,third_child(p)->c_lexer_token->type);
		}
	}else if(check_one_child(p,TERMINAL)){
		if(is_terminal_c_token_type(first_child(p), CONSTANT_CHARACTER)){
			unsigned char * constant = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte);
			struct constant_description * description = find_constant(parser_state, constant);
			assert(description);
			buffered_printf(code_gen_state->buffered_output,";Code to load %s with data %c\n", constant, ((unsigned char *)description->native_data)[0]);
			buffered_printf(code_gen_state->buffered_output,"; onto top of stack\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", ((unsigned char *)description->native_data)[0]);
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP;\n");
			push_type(code_gen_state, copy_type_description(parser_state->unsigned_int_description), p);
			free(constant);
		}else if(is_terminal_c_token_type(first_child(p), CONSTANT_DECIMAL) || is_terminal_c_token_type(first_child(p), CONSTANT_HEX)){
			unsigned char * constant = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte);
			struct constant_description * description = find_constant(parser_state, constant);
			assert(description);
			buffered_printf(code_gen_state->buffered_output,";Code to load %s with data %d\n", constant, *(description->native_data));
			buffered_printf(code_gen_state->buffered_output,"; onto top of stack\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			
			if(LITERAL22BITMASK < *(description->native_data)){
				/*  Value must be loaded in 2 parts with shift */
				buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;  Load upper 10 bits\n", ((*(description->native_data)) & (~LITERAL22BITMASK)) >> 22);
				buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;  Number of bits to shift them\n", 22);
				buffered_printf(code_gen_state->buffered_output,"shl r1 r2;  r1 contains upper 10 bits of result\n");
				buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;  Load lower 22 bits\n", (*(description->native_data)) & LITERAL22BITMASK);
				buffered_printf(code_gen_state->buffered_output,"or r1 r1 r2;  Put the pieces together\n");
			}else{
				/*  If value can be loaded directly */
				buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", *(description->native_data));
			}

			buffered_printf(code_gen_state->buffered_output,"sto SP r1;\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP;\n");
			push_type(code_gen_state, copy_type_description(parser_state->unsigned_int_description), p);
			free(constant);
		}else if(is_terminal_c_token_type(first_child(p), IDENTIFIER)){
			unsigned char * identifier = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte);
			load_identifier(code_gen_state, identifier, p);
			free(identifier);
		}else if(is_terminal_c_token_type(first_child(p), STRING_LITERAL)){
			unsigned char * constant = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte);
			struct constant_description * description = find_constant(parser_state, constant);
			unsigned char * string_literal_identifier_str;
			sprintf_hook("stringliteral%d", *(description->native_data));
			string_literal_identifier_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			assert(description);
			buffered_printf(code_gen_state->buffered_output,"; onto top of stack\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 %s;\n", string_literal_identifier_str);
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP;\n");
			push_type(code_gen_state, copy_type_description(parser_state->const_char_ptr_description), p);
			require_internal_symbol(&code_gen_state->symbols, string_literal_identifier_str);
			free(string_literal_identifier_str);
			free(constant);
		}else{
			assert(0 && "Unknown terminal in primary expression\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported primary expression.\n");
	}
}

void g_epsilon(void){
}

unsigned int g_argument_expression_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, ASSIGNMENT_EXPRESSION, ARGUMENT_EXPRESSION_LIST_REST)){
		if(is_terminal_c_token_type(first_child(p),COMMA_CHAR)){
			struct type_description * arg_type;
			unsigned int arg_type_size;
			g_assignment_expression(second_child(p), code_gen_state);
			arg_type = pop_type_without_type_check(code_gen_state, second_child(p));
			arg_type_size = type_size(code_gen_state, arg_type, DATA_AND_PTR_BYTES, 1, arg_type->source_scope_level);
			buffered_printf(code_gen_state->buffered_output,"add r2 SP ZR; \n"); /* src not actually used */
			buffered_printf(code_gen_state->buffered_output,"add r3 SP ZR; \n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", arg_type_size -4);
			buffered_printf(code_gen_state->buffered_output,"add r3 r3 r1;  Set up ptr so we can traverse the item.\n");
			/*  This makes all the members of the parameter by value */
			traverse_type(code_gen_state, arg_type, "r2", "r3", "r5", arg_type->source_scope_level, CONVERT_TO_VALUE, 1);
			push_type(code_gen_state, arg_type, second_child(p));
			return arg_type_size + g_argument_expression_list_rest(third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected COMMA_CHAR.\n");
		}
	}else if(check_one_child(p, EPSILON)){
		return 0;
	}else{
		assert(0 &&"Expected init_declarator_list_rest.\n");
		return 0;
	}
}

unsigned int g_argument_expression_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, ASSIGNMENT_EXPRESSION, ARGUMENT_EXPRESSION_LIST_REST)){
		struct type_description * arg_type;
		unsigned int arg_type_size;
		g_assignment_expression(first_child(p), code_gen_state);
		arg_type = pop_type_without_type_check(code_gen_state, first_child(p));
        	arg_type_size = type_size(code_gen_state, arg_type, DATA_AND_PTR_BYTES, 1, arg_type->source_scope_level);
		buffered_printf(code_gen_state->buffered_output,"add r2 SP ZR; \n"); /* src not actually used */
		buffered_printf(code_gen_state->buffered_output,"add r3 SP ZR; \n");
		buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", arg_type_size -4);
		buffered_printf(code_gen_state->buffered_output,"add r3 r3 r1;  Set up ptr so we can traverse the item.\n");
		/*  This makes all the members of the parameter by value */
		traverse_type(code_gen_state, arg_type, "r2", "r3", "r5", arg_type->source_scope_level, CONVERT_TO_VALUE, 1);
		push_type(code_gen_state, arg_type, first_child(p));
		return arg_type_size + g_argument_expression_list_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected init_declarator_list.\n");
		return 0;
	}
}


void function_call(struct parser_node * argument_expression_list, struct parser_node * postfix_experssion_rest, struct code_gen_state * code_gen_state, struct parser_node * context){
	struct parser_state * parser_state = code_gen_state->parser_state;
        unsigned int bytes_return_value;
        unsigned int bytes_arguments = 0;
        unsigned int type_stack_size_before;
        /*  Grab the function type from the top of the stack */
        struct type_description * fcn_ptr_type = pop_type_without_type_check(code_gen_state, context);
        struct type_description * fcn_type = create_dereferenced_pointer_type_description_from_type_description(fcn_ptr_type);
	struct type_description * return_type_description = get_current_function_return_type_description(fcn_type);

	push_type(code_gen_state, return_type_description, context);
        bytes_return_value = type_size(code_gen_state, return_type_description, DATA_AND_PTR_BYTES, 0, return_type_description->source_scope_level);
	/*  -8 because the function address that was loaded gets overwritten with the return value. */
        buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;  Number of extra bytes for rtn val\n", bytes_return_value - 8);
        buffered_printf(code_gen_state->buffered_output,"sub SP SP r2; SP points to top of rtn value space\n");
        buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Inc another 4 bytes for the return address.\n");
        buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Push another 4 bytes for the frame pointer.\n");
        buffered_printf(code_gen_state->buffered_output,"sto SP FP;  Put the frame pointer on the stack\n");
	push_type(code_gen_state, copy_type_description(parser_state->unsigned_int_description), context); /*  We pushed the rtnaddr and fp, which is an int in size = 4 byte + 4 byte = 8 = int */
	type_stack_size_before = struct_type_description_ptr_list_size(&code_gen_state->type_stack);
	if(argument_expression_list){
		bytes_arguments = g_argument_expression_list(argument_expression_list, code_gen_state);
	}
        buffered_printf(code_gen_state->buffered_output,"add FP SP ZR;  Copy the stack pointer into the frame pointer.\n");
        buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;  But need to add the bytes arguments\n", bytes_arguments);
        buffered_printf(code_gen_state->buffered_output,"add FP FP r1;  because we just incremented the SP by that much.\n");
        buffered_printf(code_gen_state->buffered_output,";  Right now FP should equal (SP + bytes arguments = %d)\n", bytes_arguments);
        buffered_printf(code_gen_state->buffered_output,";  SP points to top argument or FP on stack if no args.\n");
        buffered_printf(code_gen_state->buffered_output,";  Now get the function address from stack and store it in r2\n");
        buffered_printf(code_gen_state->buffered_output,";  Need to go back over the arguments, rtn value, rtn address, fp\n");
        buffered_printf(code_gen_state->buffered_output,";  to get the branch addr of the fcn to call.  This is located\n");
        buffered_printf(code_gen_state->buffered_output,";  just before the return value.\n");
        /*  The address of the function pointer is stored in the area where the return value will be */
        buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", bytes_arguments + (bytes_return_value - 4) + (2 * get_word_size()));
        buffered_printf(code_gen_state->buffered_output,"add r5 SP r1;  r5 now points to the function address.\n");
        buffered_printf(code_gen_state->buffered_output,"loa r2 r5;  Function address is now stored in r2.\n");
        buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;  This is the number of bytes up the stack for the rtn addr\n", (bytes_return_value - 4) + get_word_size()); /* -8 see above comment about return overwriting function pointer */
        buffered_printf(code_gen_state->buffered_output,"sub r5 r5 r1;  r5 now points to the return address.\n");
        /*  Now calculate the address the function will have to return to */
        buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; 4 * Number of instructions we need to jump to be after branch\n", 4 * 5);
        buffered_printf(code_gen_state->buffered_output,"add r1 PC r1;  Add the current program counter to this value.\n");
        buffered_printf(code_gen_state->buffered_output,"sto r5 r1;  Store the return address in the stack.\n");
        /*  Now calculate how much we need to add to PC to branch correctly */
        buffered_printf(code_gen_state->buffered_output,"sub r2 r2 PC;  Determine the difference between the PC and function pointer.\n");
        buffered_printf(code_gen_state->buffered_output,"ll r3 0x%X;  The PC changed while we were determining the branch addr, by this fixed amount.\n", 4 * 3);
        buffered_printf(code_gen_state->buffered_output,"sub r2 r2 r3; r2 is truely the value we're going to have to add to PC to do the branch in the next instruction\n");
        buffered_printf(code_gen_state->buffered_output,"add PC PC r2;  Branch to function.\n");
        buffered_printf(code_gen_state->buffered_output,";  SP should be pointing to rtn value from fcn now .\n");
        buffered_printf(code_gen_state->buffered_output,";  SP = SP at start of fcn call - sizeofrtn.\n");

	g_postfix_expression_rest(postfix_experssion_rest, code_gen_state);
	/*  Pop each of the arguments loaded in the argument_expression_list */
	backtrack_type_stack(code_gen_state, type_stack_size_before, context);
	buffered_printf(code_gen_state->buffered_output,";  Finished poping arguments.\n");
	/*  One more pop for fp and rtnaddr */
	destroy_type_description(pop_type(code_gen_state, parser_state->unsigned_int_description, context));
	destroy_type_description(fcn_ptr_type);
	destroy_type_description(fcn_type);
}


void do_ptr_operator(struct code_gen_state * code_gen_state, unsigned char * target_member_identifier, struct parser_node * context){
	struct parser_node * struct_or_union_or_enum_specifier;
	struct c_lexer_token * struct_tag_token;
	unsigned char * struct_tag_identifier;
	unsigned int num_children;
	struct namespace_object * obj;
	struct normalized_declaration_element * element;
	unsigned int i;
	unsigned int bytes_before = 0;
	unsigned int bytes_target_member = 0;
	unsigned int bytes_after = 0;
	struct type_description * t = pop_type_without_type_check(code_gen_state, context);
	struct type_description * dereferenced_type;
	unsigned int member_found = 0;
	unsigned int words_after;
	unsigned int words_member;
	unsigned int words_before;
	struct parser_node * abstract_declarator = 0;
	struct parser_node * temp = create_abstract_declarator_from_normalized_declarator(t->declarator);
	(void)words_before;
	(void)words_after;
	(void)words_member;

	if(!is_pointer(temp)){
		assert(0 && "Trying to do ptr op on something that isn't a pointer.");
	}

	destroy_parser_node_tree_and_c_lexer_tokens(temp);
	dereferenced_type = create_dereferenced_pointer_type_description_from_type_description(t);
	dereferenced_type->source_scope_level = t->source_scope_level;
	destroy_type_description(t);

	struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(dereferenced_type->specifiers);
	struct_tag_token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
	struct_tag_identifier = copy_string(struct_tag_token->first_byte, struct_tag_token->last_byte);

	obj = get_namespace_object_from_closest_namespace(struct_tag_identifier, TAG_NAMESPACE, dereferenced_type->source_scope_level, 1);
	if(!obj){
		printf("Unknown structure: %s in file %s.\n", struct_tag_identifier, code_gen_state->parser_state->c_lexer_state->c.filename);
		assert(obj && "Unknown identifier.");
	}
	/*printf("Ptr op: structure %s found\n", struct_tag_identifier);*/

	num_children = struct_namespace_object_ptr_list_size(obj->children);
	assert(num_children);

	for(i = 0; i < num_children; i++){
		struct namespace_object * child_obj = struct_namespace_object_ptr_list_get(obj->children, i);
		unsigned int size;
		struct type_description * member_description;
		struct c_lexer_token * member_identifier_token;
		unsigned char * member_name;
		assert(struct_normalized_declaration_element_ptr_list_size(&child_obj->elements));
		element = struct_normalized_declaration_element_ptr_list_get(&child_obj->elements, 0);
		member_identifier_token = get_identifier_token_from_normalized_declarator(element->normalized_declarator);
		member_description = create_type_description_from_normalized_declaration_element(element);
		member_description->source_scope_level = obj->scope_level;
		member_name = copy_string(member_identifier_token->first_byte, member_identifier_token->last_byte);
		size = type_size(code_gen_state, member_description, DATA_AND_PTR_BYTES, 0, obj->scope_level);
		/*printf("Member %s has size %d\n", member_name, size);*/
		if(!unsigned_strcmp(member_name, target_member_identifier)){
			push_type(code_gen_state, member_description, context);
			member_found = 1;
			bytes_target_member += size;
			abstract_declarator = create_abstract_declarator_from_normalized_declarator(member_description->declarator);
		}else{
			if(member_found){
				bytes_after += size;
			}else{
				bytes_before += size;
			}
			destroy_type_description(member_description);
		}
		free(member_name);
	}
	if(!member_found){
		printf("Member %s of struct %s not found, in file %s.\n", target_member_identifier, struct_tag_identifier, code_gen_state->parser_state->c_lexer_state->c.filename);
		assert(0 && "Member of struct not found.");
	}
	words_member = (bytes_target_member / 4);
	words_before = (bytes_before / 4);
	if(is_array(abstract_declarator)){
		buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Go down to value(addr)\n");

		buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Load ptr\n");
		buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; Jump over the arr\n", words_before * 4);
		buffered_printf(code_gen_state->buffered_output,"add r2 r1 r2;\n");
		buffered_printf(code_gen_state->buffered_output,"loa r2 r2; Grab the first arr element addr\n");

		buffered_printf(code_gen_state->buffered_output,"sto SP r2; Write address\n");
		buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
		buffered_printf(code_gen_state->buffered_output,"sto SP r2; For pointer too\n");
	}else{
		buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Go down to value(addr)\n");
		buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Load ptr\n");
		buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; Offset in struct to member\n", ((words_before + words_member) - 1) * 4);
		buffered_printf(code_gen_state->buffered_output,"add r1 r2 r1; The address of the struct member\n");
		copy_words(code_gen_state, "r1", "SP", "r2", bytes_target_member);
	}
	/* Copy the member onto the stack */
	/*printf("Found member %s.  It is size %d, has %d bytes before and %d after.\n", target_member_identifier, bytes_target_member, bytes_before, bytes_after);*/
	destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);
	free(struct_tag_identifier);
	destroy_type_description(dereferenced_type);
	/*printf("done ptr op, offset was %d\n", ((words_before + words_member) -1) * 4);*/
}

void do_dot_operator(struct code_gen_state * code_gen_state, unsigned char * target_member_identifier, struct parser_node * context){
	struct parser_node * struct_or_union_or_enum_specifier;
	struct c_lexer_token * struct_tag_token;
	unsigned char * struct_tag_identifier;
	unsigned int num_children;
	struct namespace_object * obj;
	struct normalized_declaration_element * element;
	unsigned int i;
	unsigned int bytes_before = 0;
	unsigned int bytes_target_member = 0;
	unsigned int bytes_after = 0;
	struct type_description * t = pop_type_without_type_check(code_gen_state, context);
	unsigned int member_found = 0;
	unsigned int words_after;
	unsigned int words_member;
	unsigned int words_before;
	struct parser_node * abstract_declarator = 0;

	struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(t->specifiers);
	struct_tag_token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
	if(!struct_tag_token){
		struct unsigned_char_list l;
		unsigned_char_list_create(&l);
		buffered_printf(&l,"\nBad structure type:\n");
		print_normalized_declaration_declarator_and_specifiers(&l, t->declarator, t->specifiers, 0, 0);
		unsigned_char_list_add(&l, 0);
		printf("%s\n", unsigned_char_list_data(&l));
		print_node_context(code_gen_state->parser_state->c_lexer_state, context);
		assert(0);
	}
	struct_tag_identifier = copy_string(struct_tag_token->first_byte, struct_tag_token->last_byte);

	obj = get_namespace_object_from_closest_namespace(struct_tag_identifier, TAG_NAMESPACE, t->source_scope_level, 1);
	if(!obj){
		printf("Unknown structure: %s in file %s.\n", struct_tag_identifier, code_gen_state->parser_state->c_lexer_state->c.filename);
		assert(obj && "Unknown identifier.");
	}
	/*printf("Dot op: Got structure: %s \n", struct_tag_identifier);*/

	num_children = struct_namespace_object_ptr_list_size(obj->children);
	assert(num_children);

	for(i = 0; i < num_children; i++){
		struct namespace_object * child_obj = struct_namespace_object_ptr_list_get(obj->children, i);
		unsigned int size;
		struct type_description * member_description;
		struct c_lexer_token * member_identifier_token;
		unsigned char * member_name;
		assert(struct_normalized_declaration_element_ptr_list_size(&child_obj->elements));
		element = struct_normalized_declaration_element_ptr_list_get(&child_obj->elements, 0);
		member_identifier_token = get_identifier_token_from_normalized_declarator(element->normalized_declarator);
		member_description = create_type_description_from_normalized_declaration_element(element);
		member_description->source_scope_level = obj->scope_level;
		member_name = copy_string(member_identifier_token->first_byte, member_identifier_token->last_byte);
		size = type_size(code_gen_state, member_description, DATA_AND_PTR_BYTES, 0, obj->scope_level);
		/*printf("Member %s has size %d\n", member_name, size);*/
		if(!unsigned_strcmp(member_name, target_member_identifier)){
			push_type(code_gen_state, member_description, context);
			member_found = 1;
			bytes_target_member += size;
			abstract_declarator = create_abstract_declarator_from_normalized_declarator(member_description->declarator);
		}else{
			if(member_found){
				bytes_after += size;
			}else{
				bytes_before += size;
			}
			destroy_type_description(member_description);
		}
		free(member_name);
	}
	if(!member_found){
		assert(0 && "Member of struct not found.");
	}
	words_after = (bytes_after / 4);
	words_member = (bytes_target_member / 4);
	words_before = (bytes_before / 4);
	assert((bytes_after - (words_after * 4)) == 0);
	for(i = 0; i < words_before; i++){
		buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Ignore this part of struct\n");
	}
	/*  If there were bytes before this memeber, we need to get rid of them and move the member down to that space */
	if(bytes_after){
		/*  If we're accessing an array member of a struct, we need to turn the array into a pointer */
		if(is_array(abstract_declarator)){
			assert(words_member > 0);
			/* Go down to where we can grab the pointer to the first array element */
			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Grab the first arr element addr\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; Jump over the after members and arr\n", (words_member + words_after -1)  * 4);
			buffered_printf(code_gen_state->buffered_output,"add SP SP r1;\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r2; Write address\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r2; For pointer too\n");
		}else{
			/*  Start copying from here */
			/*printf("Move source down %d\n", (words_member -1) * 4);*/
			buffered_printf(code_gen_state->buffered_output,"add r3 SP ZR;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", (words_member -1) * 4);
			buffered_printf(code_gen_state->buffered_output,"add r1 r3 r1;\n");
			/*  To here */
			/*printf("Move dst down %d\n", ((words_after + words_member) -1) * 4);*/
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;\n", ((words_after + words_member) -1) * 4);
			buffered_printf(code_gen_state->buffered_output,"add r3 r3 r2;\n");
			/*  Copy the member down on the stack */
			copy_words(code_gen_state, "r1", "r3", "r2", bytes_target_member);
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x%X;\n", words_after * 4);
			buffered_printf(code_gen_state->buffered_output,"add SP SP r3;\n");
		}
	}else{
		if(is_array(abstract_declarator)){
			/* Go down to where we can grab the pointer to the first array element */
			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Grab the first arr element addr\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; Jump over the arr\n", (words_member -1)  * 4);
			buffered_printf(code_gen_state->buffered_output,"add SP SP r1;\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r2; Write address\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r2; For pointer too\n");
		}
	}
	/*printf("Found member %s.  It is size %d, has %d bytes before and %d after.\n", target_member_identifier, bytes_target_member, bytes_before, bytes_after);*/
	destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);
	free(struct_tag_identifier);
	destroy_type_description(t);
}

void g_postfix_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, EXPRESSION, TERMINAL, POSTFIX_EXPRESSION_REST)){
		if(
			is_terminal_c_token_type(first_child(p), OPEN_SQUARE_BRACKET_CHAR) &&
			is_terminal_c_token_type(third_child(p), CLOSE_SQUARE_BRACKET_CHAR)
		){
			struct type_description * array_type;
			struct type_description * dereferenced_array_type;
			unsigned int element_size;
			unsigned int its_an_array;
			struct parser_node * abstract_declarator;
			g_expression(second_child(p), code_gen_state);
			/*  The array index */
			consume_scalar_type(code_gen_state, p);
			array_type = pop_type_without_type_check(code_gen_state, p);
			dereferenced_array_type = create_dereferenced_array_type_description_from_type_description(array_type);
			push_type(code_gen_state, dereferenced_array_type, p);
			destroy_type_description(array_type);

			element_size = type_size(code_gen_state, dereferenced_array_type, DATA_AND_PTR_BYTES, 0, dereferenced_array_type->source_scope_level);
			/*  Determine if the result will be an array (multi-dimensional array case) */
			abstract_declarator = create_abstract_declarator_from_normalized_declarator(dereferenced_array_type->declarator);
			its_an_array = (unsigned int)is_array(abstract_declarator);
			destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);

			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Go down to value of expr\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP; Load the index value\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Go down to addr of address\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Go down to value of address\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Load the address of the first arr element\n");
			buffered_printf(code_gen_state->buffered_output,"ll r5 0x%X;\n", element_size);
			buffered_printf(code_gen_state->buffered_output,"mul r3 r1 r5; compute offset in array \n");
			buffered_printf(code_gen_state->buffered_output,"add r1 r2 r3; Actual address of item \n");
			if(its_an_array){
				buffered_printf(code_gen_state->buffered_output,"sto SP r1; Write address\n");
				buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Go to addr\n");
				buffered_printf(code_gen_state->buffered_output,"sto SP r1; Write address\n");
			}else{
				/* We want the pointer at the base of where this item starts, so we can copy it */
				buffered_printf(code_gen_state->buffered_output,"ll r5 0x%X;\n", element_size - 4); 
				buffered_printf(code_gen_state->buffered_output,"add r1 r1 r5; Address of base of item\n");
				copy_words(code_gen_state, "r1", "SP", "r2", element_size);
			}
			g_postfix_expression_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected [] in postfix expression rest.\n");
		}
	}else if(check_four_children(p, TERMINAL, ARGUMENT_EXPRESSION_LIST, TERMINAL, POSTFIX_EXPRESSION_REST)){
		if(
			is_terminal_c_token_type(first_child(p), OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(third_child(p), CLOSE_PAREN_CHAR)
		){
			function_call(second_child(p), fourth_child(p), code_gen_state, p);
		}else{
			assert(0 &&"Expected () in postfix expression rest.\n");
		}
	}else if(check_three_children(p, TERMINAL, TERMINAL, POSTFIX_EXPRESSION_REST)){
		if(
			is_terminal_c_token_type(first_child(p), OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(second_child(p), CLOSE_PAREN_CHAR)
		){
			function_call(0, third_child(p), code_gen_state, p);
		}else if(
			is_terminal_c_token_type(first_child(p), DOT_CHAR) &&
			is_terminal_c_token_type(second_child(p), IDENTIFIER)
		){
			unsigned char * identifier = copy_string(second_child(p)->c_lexer_token->first_byte, second_child(p)->c_lexer_token->last_byte);
			do_dot_operator(code_gen_state, identifier, second_child(p));
			g_postfix_expression_rest(third_child(p), code_gen_state);
			free(identifier);
		}else if(
			is_terminal_c_token_type(first_child(p), PTR_OP) &&
			is_terminal_c_token_type(second_child(p), IDENTIFIER)
		){
			unsigned char * identifier = copy_string(second_child(p)->c_lexer_token->first_byte, second_child(p)->c_lexer_token->last_byte);
			do_ptr_operator(code_gen_state, identifier, second_child(p));
			g_postfix_expression_rest(third_child(p), code_gen_state);
			free(identifier);
		}else{
			assert(0 &&"Expected () in postfix expression rest.\n");
		}
	}else if(check_two_children(p, TERMINAL, POSTFIX_EXPRESSION_REST)){
		if(is_terminal_c_token_type(first_child(p), INC_OP)){
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x1; Value to inc by\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Copy ptr to value\n");
			buffered_printf(code_gen_state->buffered_output,"add r3 r2 WR; Ptr to origin of value\n");
			buffered_printf(code_gen_state->buffered_output,"loa r5 r3; Load the original value\n");
			buffered_printf(code_gen_state->buffered_output,"add r6 r5 r1; Increment that val by 1\n");
			buffered_printf(code_gen_state->buffered_output,"sto r3 r6; Store the original value at origin\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Point to result val\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r5; Result is value before increment\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Point back to address\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP; Point to itself\n");
			g_postfix_expression_rest(second_child(p), code_gen_state);
		}else if(is_terminal_c_token_type(first_child(p), DEC_OP)){
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x1; Value to dec by\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Copy ptr to value\n");
			buffered_printf(code_gen_state->buffered_output,"add r3 r2 WR; Ptr to origin of value\n");
			buffered_printf(code_gen_state->buffered_output,"loa r5 r3; Load the original value\n");
			buffered_printf(code_gen_state->buffered_output,"sub r6 r5 r1; Decrement that val by 1\n");
			buffered_printf(code_gen_state->buffered_output,"sto r3 r6; Store the original value at origin\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Point to result val\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r5; Result is value before increment\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Point back to address\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP; Point to itself\n");
			g_postfix_expression_rest(second_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected () in postfix expression rest.\n");
		}
	}else if(check_one_child(p, EPSILON)){
		g_epsilon();
	}else{
		assert(0 &&"Expected postfix_expression_rest.\n");
	}
}

void g_postfix_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, PRIMARY_EXPRESSION, POSTFIX_EXPRESSION_REST)){
		g_primary_expression(first_child(p), code_gen_state);
		g_postfix_expression_rest(second_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported postfix expression.\n");
	}
}

void g_unary_operator(struct parser_node * p, struct code_gen_state * code_gen_state){
	struct parser_state * parser_state = code_gen_state->parser_state;
	if(check_one_child(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p), AMPERSAND_CHAR)){
			unsigned int size;
			unsigned int num_words;
			unsigned int i;
			struct type_description * found;
			struct type_description * address_type;
			found = pop_type_without_type_check(code_gen_state, p); /*  Pop the base type */
			buffered_printf(code_gen_state->buffered_output,"; Perform & operator\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;  Copy address\n");
			size = type_size(code_gen_state, found, DATA_AND_PTR_BYTES, 0, found->source_scope_level);
			num_words = size / 4;
			assert(size - (4*num_words) == 0);
			for(i = 0; i < num_words; i++){
				buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Remove type for & %d.\n", i);
			}
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Move to value\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1; Addr becomes value\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Up to address\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP; Point to itself\n");

        		address_type = create_address_type_description_from_type_description(found);
			address_type->source_scope_level = found->source_scope_level;
			push_type(code_gen_state, address_type, p); /* Put back the pointer type */
			destroy_type_description(found);
		}else if(is_terminal_c_token_type(first_child(p), MULTIPLY_CHAR)){
			struct type_description * found_type = pop_type_without_type_check(code_gen_state, p);
			struct type_description * dereferenced_type = create_dereferenced_pointer_type_description_from_type_description(found_type);
			unsigned int item_size = type_size(code_gen_state, dereferenced_type, DATA_AND_PTR_BYTES, 0, dereferenced_type->source_scope_level);

			buffered_printf(code_gen_state->buffered_output,";                 Pointer Dereference\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;     Point at value now\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;        Copy value, which is an address\n");
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;        Load the size of the item\n", item_size - 4);
			buffered_printf(code_gen_state->buffered_output,"add r2 r1 r2;     r2 Now holds the address of the last word of the item we're pointing to\n");
			traverse_type(code_gen_state, dereferenced_type, "r2", "SP", "r5", dereferenced_type->source_scope_level, REF_COPY, 0);

			destroy_type_description(found_type);
			push_type(code_gen_state, dereferenced_type, p);
		}else if(is_terminal_c_token_type(first_child(p), PLUS_CHAR)){
			buffered_printf(code_gen_state->buffered_output,";unary plus operator (Do Nothing).\n");
			destroy_type_description(pop_type(code_gen_state, parser_state->unsigned_int_description, p)); /*  Pop the base type */
			push_type(code_gen_state, copy_type_description(parser_state->unsigned_int_description), p); /* Put back the type */
		}else if(is_terminal_c_token_type(first_child(p), MINUS_CHAR)){
			struct type_description * type;
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;  Point at value now\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;  Copy value\n");
			buffered_printf(code_gen_state->buffered_output,"not r1 r1;  Ones complement\n");
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x1;\n");
			buffered_printf(code_gen_state->buffered_output,"add r1 r1 r2; Twos complement\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;  Save\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Go back to addr\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP;  ptr to self\n");
			type = pop_type_without_type_check(code_gen_state, p); /*  Pop the base type */
			push_type(code_gen_state, type, p); /* Put back the type */
		}else if(is_terminal_c_token_type(first_child(p), TILDE_CHAR)){
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;  Point at value now\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;  Copy value\n");
			buffered_printf(code_gen_state->buffered_output,"not r1 r1;  logical not\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;  Save value\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Point at value now\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP;  ptr to self\n");
			destroy_type_description(pop_type(code_gen_state, parser_state->unsigned_int_description, p)); /*  Pop the base type */
			push_type(code_gen_state, copy_type_description(parser_state->unsigned_int_description), p); /* Put back the type */
		}else if(is_terminal_c_token_type(first_child(p), EXCLAMATION_MARK_CHAR)){
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;  Point at value now\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;  Copy value\n");
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x1; Assume false and invert to 1\n");
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR 1; If it is zero\n");
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x0; It was not 0, inverse is 0\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r2;  Save value\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Point at value now\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP;  ptr to self\n");
			consume_scalar_type(code_gen_state, p);
			push_type(code_gen_state, copy_type_description(parser_state->unsigned_int_description), p); /* Put back the type */
		}else{
			assert(0 && ";Unsupported unary operator unknown terminal.\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported unary operator.\n");
	}

}
void g_unary_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, TERMINAL, TYPE_NAME, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p), SIZEOF) &&
			is_terminal_c_token_type(second_child(p), OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(fourth_child(p), CLOSE_PAREN_CHAR)
		){
			buffered_printf(code_gen_state->buffered_output,"sizeof type.\n");
		}else{
			assert(0 &&"Expected sizeof type.\n");
		}
	}else if(check_two_children(p, UNARY_OPERATOR, CAST_EXPRESSION)){
		g_cast_expression(second_child(p), code_gen_state);
		g_unary_operator(first_child(p), code_gen_state);
	}else if(check_two_children(p, TERMINAL, UNARY_EXPRESSION)){
		if(is_terminal_c_token_type(first_child(p), INC_OP)){
			buffered_printf(code_gen_state->buffered_output,"Do INC_OP.\n");
			g_unary_expression(second_child(p), code_gen_state);
		}else if(is_terminal_c_token_type(first_child(p), DEC_OP)){
			g_unary_expression(second_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x1; Value to dec by\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Copy ptr to value\n");
			buffered_printf(code_gen_state->buffered_output,"add r3 r2 WR; Ptr to origin of value\n");
			buffered_printf(code_gen_state->buffered_output,"loa r5 r3; Load the original value\n");
			buffered_printf(code_gen_state->buffered_output,"sub r6 r5 r1; Decrement that val by 1\n");
			buffered_printf(code_gen_state->buffered_output,"sto r3 r6; Store the original value at origin\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Point to result val\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r6; Result is value after increment\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Point back to address\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP SP; Point to itself\n");
		}else if(is_terminal_c_token_type(first_child(p), SIZEOF)){
			buffered_printf(code_gen_state->buffered_output,"sizeof unary expr.\n");
			g_unary_expression(second_child(p), code_gen_state);
		}else{
			buffered_printf(code_gen_state->buffered_output,"expected unary expr.\n");
		}
	}else if(check_one_child(p, POSTFIX_EXPRESSION)){
		g_postfix_expression(first_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported unary expression.\n");
	}
}

void g_cast_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_first_child_type(p, UNARY_EXPRESSION)){
		g_unary_expression(first_child(p), code_gen_state);
	}else if(check_four_children(p, TERMINAL, TYPE_NAME, TERMINAL, CAST_EXPRESSION)){
		if(
			is_terminal_c_token_type(first_child(p), OPEN_PAREN_CHAR) ||
			is_terminal_c_token_type(third_child(p), CLOSE_PAREN_CHAR)
		){
			struct type_description * new_type = create_type_description_from_type_name(second_child(p));
			g_cast_expression(fourth_child(p), code_gen_state);
			destroy_type_description(pop_type_without_type_check(code_gen_state, p));
			buffered_printf(code_gen_state->buffered_output,";  Performing an explicit type cast.\n");
			push_type(code_gen_state, new_type, p);
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported cast expression.\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported cast expression.\n");
	}
}

void pop(struct code_gen_state * code_gen_state, const char * r){
	buffered_printf(code_gen_state->buffered_output, "add SP SP WR;\n"); /* Pop addr */
	buffered_printf(code_gen_state->buffered_output, "loa %s SP;\n", r); /* Load the value into register */
	buffered_printf(code_gen_state->buffered_output, "add SP SP WR;\n"); /* Pop value */
}

void push(struct code_gen_state * code_gen_state, const char * r){
	buffered_printf(code_gen_state->buffered_output, "sub SP SP WR;\n"); /*  Point to value  */
	buffered_printf(code_gen_state->buffered_output, "sto SP %s;\n", r); /*  Assign value  */
	buffered_printf(code_gen_state->buffered_output, "sub SP SP WR;\n"); /*  Point to address  */
	buffered_printf(code_gen_state->buffered_output, "sto SP SP;\n");    /*  Point to address  */
}

void do_signed_operation_function_call(struct code_gen_state * code_gen_state, const char * function_name){
	push(code_gen_state, "ZR"); /*  Space for the integer return value */
	/* Start setting up a stack frame */
	buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Inc another 4 bytes for the return address.\n");
	buffered_printf(code_gen_state->buffered_output,"ll r3 0x3C;  Constant offset from current PC value to return.\n");
	buffered_printf(code_gen_state->buffered_output,"add r3 r3 PC;.\n");
	buffered_printf(code_gen_state->buffered_output,"sto SP r3;  Store the return address.\n");
	buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Push another 4 bytes for the frame pointer.\n");
	buffered_printf(code_gen_state->buffered_output,"sto SP FP;  Put the frame pointer on the stack\n");
	/* Load arguments */
	push(code_gen_state, "r1");
	push(code_gen_state, "r2");

	buffered_printf(code_gen_state->buffered_output,"add FP SP ZR;  Copy the stack pointer into the frame pointer.\n");
	buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;  But need to add the bytes arguments\n", 16);
	buffered_printf(code_gen_state->buffered_output,"add FP FP r1;  because we just incremented the SP by that much.\n");
	buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;\n", function_name);
	require_external_symbol(&code_gen_state->symbols, (unsigned char *)function_name);
}

void g_multiplicative_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, CAST_EXPRESSION, MULTIPLICATIVE_EXPRESSION_REST)){
		unsigned int iz_signed;
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		iz_signed = is_signed(t);

		pop(code_gen_state, "r2"); /* Arg 2 */
		pop(code_gen_state, "r1"); /* Arg 1 */

		if(is_terminal_c_token_type(first_child(p), MULTIPLY_CHAR)){
			buffered_printf(code_gen_state->buffered_output,"mul r1 r1 r2;\n");
			push(code_gen_state, "r1");
		}else if(is_terminal_c_token_type(first_child(p), DIVIDE_CHAR)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_division");
			}else{
				buffered_printf(code_gen_state->buffered_output,"div r1 r2 r1 r2;\n");
				push(code_gen_state, "r1");
			}
		}else if(is_terminal_c_token_type(first_child(p), PERCENT_CHAR)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_modulo");
			}else{
				buffered_printf(code_gen_state->buffered_output,"div r2 r1 r1 r2;\n");
				push(code_gen_state, "r1");
			}
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in multiplicative_expression_rest.\n");
		}

		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), CAST_EXPRESSION)){
			g_cast_expression(second_child(third_child(p)), code_gen_state);
			g_multiplicative_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported multiplicative_expression_rest.\n");
	}
}

void g_multiplicative_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, CAST_EXPRESSION, MULTIPLICATIVE_EXPRESSION_REST)){
		g_cast_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), CAST_EXPRESSION)){
			g_cast_expression(second_child(second_child(p)), code_gen_state);
			g_multiplicative_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported multiplicative_expression.\n");
	}
}


struct type_description * perform_pointer_conversion(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * t1 = pop_type_without_type_check(code_gen_state, context);
	struct type_description * t2 = pop_type_without_type_check(code_gen_state, context);

	enum type_class c1 = determine_type_class(t1);
	enum type_class c2 = determine_type_class(t2);
	unsigned int is_integral_type1 = is_integral_type(t1);
	unsigned int is_integral_type2 = is_integral_type(t2);

	if(c1 == TYPE_CLASS_POINTER && is_integral_type2){
		struct type_description * dereferenced = create_dereferenced_pointer_type_description_from_type_description(t1);
		unsigned int size = type_size(code_gen_state, dereferenced, DATA_AND_PTR_BYTES, 0, get_current_scope_level(code_gen_state));
		pop(code_gen_state, "r1");
		pop(code_gen_state, "r2");
		buffered_printf(code_gen_state->buffered_output,"ll r3 0x%X; Load the arr element size\n", size);
		buffered_printf(code_gen_state->buffered_output,"mul r5 r2 r3; Multiply by amount to add or sub\n");
		push(code_gen_state, "r5");
		push(code_gen_state, "r1");
		destroy_type_description(dereferenced);
		destroy_type_description(t2);
		return t1;
	}else if(c2 == TYPE_CLASS_POINTER && is_integral_type1){
		struct type_description * dereferenced = create_dereferenced_pointer_type_description_from_type_description(t2);
		unsigned int size = type_size(code_gen_state, dereferenced, DATA_AND_PTR_BYTES, 0, get_current_scope_level(code_gen_state));
		pop(code_gen_state, "r1");
		buffered_printf(code_gen_state->buffered_output,"ll r3 0x%X; Load the arr element size\n", size);
		buffered_printf(code_gen_state->buffered_output,"mul r5 r1 r3; Multiply by amount to add or sub\n");
		push(code_gen_state, "r5");
		destroy_type_description(dereferenced);
		destroy_type_description(t1);
		return t2;
	}else if(c1 == TYPE_CLASS_POINTER && c2 == TYPE_CLASS_POINTER){
		destroy_type_description(t1);
		return t2;
	}
	/*  Neither one is a pointer */
	push_type(code_gen_state, t2, context);
	push_type(code_gen_state, t1, context);
	return 0;
}

void g_additive_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, MULTIPLICATIVE_EXPRESSION, ADDITIVE_EXPRESSION_REST)){
		struct type_description * t = perform_pointer_conversion(code_gen_state, p);
		if(!t){
			t = usual_arithmetic_conversion(code_gen_state, p);
		}
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p), PLUS_CHAR)){
			buffered_printf(code_gen_state->buffered_output,"add r1 r1 r2;\n"); /* Addition */
		}else if(is_terminal_c_token_type(first_child(p), MINUS_CHAR)){
			buffered_printf(code_gen_state->buffered_output,"sub r1 r1 r2;\n"); /* Subtraction */
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in multiplicative_expression_rest.\n");
		}
		push(code_gen_state, "r1");
		push_type(code_gen_state, t, p);
		if(is_second_child_type(third_child(p), MULTIPLICATIVE_EXPRESSION)){
			g_multiplicative_expression(second_child(third_child(p)), code_gen_state);
			g_additive_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported additive_expression_rest.\n");
	}
}

void g_additive_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, MULTIPLICATIVE_EXPRESSION, ADDITIVE_EXPRESSION_REST)){
		g_multiplicative_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), MULTIPLICATIVE_EXPRESSION)){
			g_multiplicative_expression(second_child(second_child(p)), code_gen_state);
			g_additive_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported multiplicative_expression.\n");
	}
}

void g_shift_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, ADDITIVE_EXPRESSION, SHIFT_EXPRESSION_REST)){
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p), LEFT_OP)){
			buffered_printf(code_gen_state->buffered_output,"shl r1 r2;\n");
		}else if(is_terminal_c_token_type(first_child(p), RIGHT_OP)){
			buffered_printf(code_gen_state->buffered_output,"shr r1 r2;\n");
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in shift_expression_rest.\n");
		}
		push(code_gen_state, "r1");
		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), ADDITIVE_EXPRESSION)){
			g_additive_expression(second_child(third_child(p)), code_gen_state);
			g_shift_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported shift_expression_rest.\n");
	}
}

void g_shift_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, ADDITIVE_EXPRESSION, SHIFT_EXPRESSION_REST)){
		g_additive_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), ADDITIVE_EXPRESSION)){
			g_additive_expression(second_child(second_child(p)), code_gen_state);
			g_shift_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported shift_expression.\n");
	}
}

void g_relational_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, SHIFT_EXPRESSION, RELATIONAL_EXPRESSION_REST)){
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		unsigned int iz_signed = is_signed(t);
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p),OPEN_ANGLE_BRACKET_CHAR)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_less_than");
			}else{
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x1; Assume r1 < r2\n");
				buffered_printf(code_gen_state->buffered_output,"blt r1 r2 1;\n");
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n");
				push(code_gen_state, "r3");
			}
		}else if(is_terminal_c_token_type(first_child(p),CLOSE_ANGLE_BRACKET_CHAR)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_greater_than");
			}else{
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x0; Assume r1 > r2 is not true\n");
				buffered_printf(code_gen_state->buffered_output,"blt r1 r2 2;\n");
				buffered_printf(code_gen_state->buffered_output,"beq r1 r2 1;\n");
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n");
				push(code_gen_state, "r3");
			}
		}else if(is_terminal_c_token_type(first_child(p),LE_OP)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_less_than_or_equal_to");
			}else{
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x1; Assume r1 <= r2 is true\n");
				buffered_printf(code_gen_state->buffered_output,"blt r1 r2 2;\n");
				buffered_printf(code_gen_state->buffered_output,"beq r1 r2 1;\n");
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n");
				push(code_gen_state, "r3");
			}
		}else if(is_terminal_c_token_type(first_child(p),GE_OP)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_greater_than_or_equal_to");
			}else{
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x0; Assume r1 >= r2 is not true\n");
				buffered_printf(code_gen_state->buffered_output,"blt r1 r2 1;\n");
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n");
				push(code_gen_state, "r3");
			}
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in shift_expression_rest.\n");
		}
		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), SHIFT_EXPRESSION)){
			g_shift_expression(second_child(third_child(p)), code_gen_state);
			g_relational_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported relational_expression_rest.\n");
	}
}

void g_relational_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, SHIFT_EXPRESSION, RELATIONAL_EXPRESSION_REST)){
		g_shift_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), SHIFT_EXPRESSION)){
			g_shift_expression(second_child(second_child(p)), code_gen_state);
			g_relational_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported relational_expression.\n");
	}
}

void g_equality_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, RELATIONAL_EXPRESSION, EQUALITY_EXPRESSION_REST)){
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p),EQ_OP)){
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x1; Assume r1 == r2 is true\n");
			buffered_printf(code_gen_state->buffered_output,"beq r1 r2 1;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n");
		}else if(is_terminal_c_token_type(first_child(p),NE_OP)){
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x0; Assume r1 != r2 is true\n");
			buffered_printf(code_gen_state->buffered_output,"beq r1 r2 1;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n");
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in equality_expression_rest.\n");
		}
		push(code_gen_state, "r3");
		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), RELATIONAL_EXPRESSION)){
			g_relational_expression(second_child(third_child(p)), code_gen_state);
			g_equality_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported equality_expression_rest.\n");
	}
}

void g_equality_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, RELATIONAL_EXPRESSION, EQUALITY_EXPRESSION_REST)){
		g_relational_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), RELATIONAL_EXPRESSION)){
			g_relational_expression(second_child(second_child(p)), code_gen_state);
			g_equality_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported equality_expression.\n");
	}
}

void g_and_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, EQUALITY_EXPRESSION, AND_EXPRESSION_REST)){
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p),AMPERSAND_CHAR)){
			buffered_printf(code_gen_state->buffered_output,"and r1 r1 r2;\n"); /* Bitwise and */
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in and_expression_rest.\n");
		}
		push(code_gen_state, "r1");
		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), EQUALITY_EXPRESSION)){
			g_equality_expression(second_child(third_child(p)), code_gen_state);
			g_and_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported and_expression_rest.\n");
	}
}

void g_and_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, EQUALITY_EXPRESSION, AND_EXPRESSION_REST)){
		g_equality_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), EQUALITY_EXPRESSION)){
			g_equality_expression(second_child(second_child(p)), code_gen_state);
			g_and_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported and_expression.\n");
	}
}

void g_exclusive_or_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, AND_EXPRESSION, EXCLUSIVE_OR_EXPRESSION_REST)){
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p),CARET_CHAR)){
			buffered_printf(code_gen_state->buffered_output,"or r3 r1 r2;   Bitwise or\n");
			buffered_printf(code_gen_state->buffered_output,"and r5 r1 r2;  Bitwise and\n");
			buffered_printf(code_gen_state->buffered_output,"not r5 r5;     Not both\n");
			buffered_printf(code_gen_state->buffered_output,"and r1 r3 r5;  One of them, and not both\n");
		}else{
			assert(0 && "Unknown operator in exclusive_or_expression_rest.\n");
		}
		push(code_gen_state, "r1");
		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), AND_EXPRESSION)){
			g_and_expression(second_child(third_child(p)), code_gen_state);
			g_exclusive_or_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported exclusive_or_expression_rest.\n");
	}
}

void g_exclusive_or_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, AND_EXPRESSION, EXCLUSIVE_OR_EXPRESSION_REST)){
		g_and_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), AND_EXPRESSION)){
			g_and_expression(second_child(second_child(p)), code_gen_state);
			g_exclusive_or_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported exclusive_or_expression.\n");
	}
}

void g_inclusive_or_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, EXCLUSIVE_OR_EXPRESSION, INCLUSIVE_OR_EXPRESSION_REST)){
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p),PIPE_CHAR)){
			buffered_printf(code_gen_state->buffered_output,"or r1 r1 r2;\n"); /* Bitwise or */
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in inclusive_or_expression_rest.\n");
		}
		push(code_gen_state, "r1");
		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), EXCLUSIVE_OR_EXPRESSION)){
			g_exclusive_or_expression(second_child(third_child(p)), code_gen_state);
			g_inclusive_or_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported inclusive_or_expression_rest.\n");
	}
}

void g_inclusive_or_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, EXCLUSIVE_OR_EXPRESSION, INCLUSIVE_OR_EXPRESSION_REST)){
		g_exclusive_or_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), EXCLUSIVE_OR_EXPRESSION)){
			g_exclusive_or_expression(second_child(second_child(p)), code_gen_state);
			g_inclusive_or_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported inclusive_or_expression.\n");
	}
}

void g_logical_and_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, INCLUSIVE_OR_EXPRESSION, LOGICAL_AND_EXPRESSION_REST)){
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p),AND_OP)){
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n"); /* Assume false */
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR 2;\n");
			buffered_printf(code_gen_state->buffered_output,"beq r2 ZR 1;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n"); /* Didn't branch, both must have been true */
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in logical_and_expression_rest.\n");
		}
		push(code_gen_state, "r3");
		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), INCLUSIVE_OR_EXPRESSION)){
			g_inclusive_or_expression(second_child(third_child(p)), code_gen_state);
			g_logical_and_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported logical_and_expression_rest.\n");
	}
}

void g_logical_and_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, INCLUSIVE_OR_EXPRESSION, LOGICAL_AND_EXPRESSION_REST)){
		g_inclusive_or_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), INCLUSIVE_OR_EXPRESSION)){
			g_inclusive_or_expression(second_child(second_child(p)), code_gen_state);
			g_logical_and_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported logical_and_expression.\n");
	}
}

void g_logical_or_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, LOGICAL_AND_EXPRESSION, LOGICAL_OR_EXPRESSION_REST)){
		struct type_description * t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2");
		pop(code_gen_state, "r1");
		if(is_terminal_c_token_type(first_child(p),OR_OP)){
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n"); /* Assume false */
			buffered_printf(code_gen_state->buffered_output,"or r1 r1 r2;\n"); /* Put all the one bits in one variable */
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR 1;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n");
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in logical_or_expression_rest.\n");
		}
		push(code_gen_state, "r3");
		push_type(code_gen_state, t, p);

		if(is_second_child_type(third_child(p), LOGICAL_AND_EXPRESSION)){
			g_logical_and_expression(second_child(third_child(p)), code_gen_state);
			g_logical_or_expression_rest(third_child(p), code_gen_state);
		}
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported logical_or_expression_rest.\n");
	}
}

void g_logical_or_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, LOGICAL_AND_EXPRESSION, LOGICAL_OR_EXPRESSION_REST)){
		g_logical_and_expression(first_child(p), code_gen_state);
		if(is_second_child_type(second_child(p), LOGICAL_AND_EXPRESSION)){
			g_logical_and_expression(second_child(second_child(p)), code_gen_state);
			g_logical_or_expression_rest(second_child(p), code_gen_state);
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported logical_or_expression.\n");
	}
}

void g_conditional_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_five_children(p, LOGICAL_OR_EXPRESSION, TERMINAL, EXPRESSION, TERMINAL, CONDITIONAL_EXPRESSION)){
		if(
			is_terminal_c_token_type(second_child(p), QUESTION_MARK_CHAR) &&
			is_terminal_c_token_type(fourth_child(p), COLON_CHAR)
		){
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * false_condition_str;
			unsigned char * after_condition_str;
			struct type_description * t1;
			struct type_description * t2;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			sprintf_hook("falsecondition%d", cond_index);
			false_condition_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			sprintf_hook("aftercondition%d", cond_index);
			after_condition_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			g_logical_or_expression(first_child(p), code_gen_state);
			/*  Pop the item that was loaded in the conditional */
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;\n"); /* Pop addr  */
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;\n"); /* Pop value */
			consume_scalar_type(code_gen_state, first_child(p));
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s;\n", false_condition_str);
			g_expression(third_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;\n", after_condition_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", false_condition_str);
			g_conditional_expression(fifth_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", after_condition_str);
			require_internal_symbol(&code_gen_state->symbols, false_condition_str);
			implement_internal_symbol(&code_gen_state->symbols, false_condition_str);
			require_internal_symbol(&code_gen_state->symbols, after_condition_str);
			implement_internal_symbol(&code_gen_state->symbols, after_condition_str);
			free(after_condition_str);
			free(false_condition_str);
			t1 = pop_type_without_type_check(code_gen_state, p);
			t2 = pop_type_without_type_check(code_gen_state, p);
			if(!type_description_cmp(t1, t2)){
				push_type(code_gen_state, t1, p);
        			destroy_type_description(t2);
			}else{
				assert(0 && "Type missmatch in conditional expression.");
			}
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported conditional expression.\n");
		}
	}else if(is_first_child_type(p, LOGICAL_OR_EXPRESSION)){
		g_logical_or_expression(first_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported conditional expression.\n");
	}
}

void g_assignment_operator(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_one_child(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p),EQUALS_CHAR)){
		}else if(is_terminal_c_token_type(first_child(p),MUL_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"code for *=.\n");
		}else if(is_terminal_c_token_type(first_child(p),DIV_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"add r5 SP ZR; Copy of stack pointer\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 r5; value being subtracted\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 r5; value being subtracted from\n");
			buffered_printf(code_gen_state->buffered_output,"div r1 r2 r2 r1;\n");
			buffered_printf(code_gen_state->buffered_output,"sub r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sub r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sto r5 r1; Set this as value to assign\n");
		}else if(is_terminal_c_token_type(first_child(p),MOD_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"code for %%=.\n");
		}else if(is_terminal_c_token_type(first_child(p),ADD_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"add r5 SP ZR; Copy of stack pointer\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 r5; value being added\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 r5; value being added\n");
			buffered_printf(code_gen_state->buffered_output,"add r3 r1 r2; New value we're going to assign \n");
			buffered_printf(code_gen_state->buffered_output,"sub r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sub r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sto r5 r3; Set this as value to assign\n");
		}else if(is_terminal_c_token_type(first_child(p),SUB_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"add r5 SP ZR; Copy of stack pointer\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 r5; value being subtracted\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"add r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 r5; value being subtracted from\n");
			buffered_printf(code_gen_state->buffered_output,"sub r3 r2 r1; New value we're going to assign \n");
			buffered_printf(code_gen_state->buffered_output,"sub r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sub r5 r5 WR;\n");
			buffered_printf(code_gen_state->buffered_output,"sto r5 r3; Set this as value to assign\n");
		}else if(is_terminal_c_token_type(first_child(p),LEFT_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"code for <<=.\n");
		}else if(is_terminal_c_token_type(first_child(p),RIGHT_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"code for >>=.\n");
		}else if(is_terminal_c_token_type(first_child(p),AND_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"code for &=.\n");
		}else if(is_terminal_c_token_type(first_child(p),XOR_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"code for ^=.\n");
		}else if(is_terminal_c_token_type(first_child(p),OR_ASSIGN)){
			buffered_printf(code_gen_state->buffered_output,"code for |=.\n");
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported assignment.\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported assignment.\n");
	}
}


void g_assignment_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, UNARY_EXPRESSION, ASSIGNMENT_OPERATOR, ASSIGNMENT_EXPRESSION)){
		g_unary_expression(first_child(p), code_gen_state);      /*  This loads what we're going to assign to */
		g_assignment_expression(third_child(p), code_gen_state); /*  Compute the value of what we want to assign */
		g_assignment_operator(second_child(p), code_gen_state);  /*  Compute the result of any assignment operator */
		do_assignment(code_gen_state, p);
	}else if(is_first_child_type(p, CONDITIONAL_EXPRESSION)){
		g_conditional_expression(first_child(p), code_gen_state);
	}else{
		assert(0 && "Unsupported assignment_expression.\n");
	}
}

void g_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, ASSIGNMENT_EXPRESSION, EXPRESSION_REST)){
		if(is_terminal_c_token_type(first_child(p),COMMA_CHAR)){
			g_expression_rest(third_child(p), code_gen_state);
			g_assignment_expression(second_child(p), code_gen_state);
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported expression rest.\n");
		}
	}else if(is_first_child_type(p, EPSILON)){
		/*  Do nothing */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported expression rest.\n");
	}
}

void g_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, ASSIGNMENT_EXPRESSION, EXPRESSION_REST)){
		g_expression_rest(second_child(p), code_gen_state);
		g_assignment_expression(first_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported expression.\n");
	}
}

void g_constant_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_first_child_type(p, CONDITIONAL_EXPRESSION)){
		g_conditional_expression(first_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported constant_expression.\n");
	}
}

void g_expression_statement(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_first_child_type(p, TERMINAL)){
		if(!(is_terminal_c_token_type(first_child(p),SEMICOLON_CHAR))){
			buffered_printf(code_gen_state->buffered_output,"Unknown terminal for expression statement.\n");
		}
	}else if(check_two_children(p, EXPRESSION, TERMINAL)){
		if(is_terminal_c_token_type(second_child(p),SEMICOLON_CHAR)){
			g_expression(first_child(p), code_gen_state);
			/*  Each expression statement will put the type it represents on the stack */
			/*  as a final result.  We need to pop this to keep the stack aligned. */
			delete_top_type(code_gen_state);
        	}else{
			assert("Semicolon not found.\n");
        	}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported expression_statement.\n");
	}
}

void g_statement(struct parser_node * p, struct code_gen_state * code_gen_state, unsigned char * evaluate_label, unsigned char * end_label){
	if(is_first_child_type(p, LABELED_STATEMENT)){
		g_labeled_statement(first_child(p), code_gen_state);
	}else if(is_first_child_type(p, COMPOUND_STATEMENT)){
		g_compound_statement(first_child(p), code_gen_state, 0, evaluate_label, end_label);
	}else if(is_first_child_type(p, EXPRESSION_STATEMENT)){
		g_expression_statement(first_child(p), code_gen_state);
	}else if(is_first_child_type(p, SELECTION_STATEMENT)){
		g_selection_statement(first_child(p), code_gen_state);
	}else if(is_first_child_type(p, ITERATION_STATEMENT)){
		g_iteration_statement(first_child(p), code_gen_state);
	}else if(is_first_child_type(p, JUMP_STATEMENT)){
		g_jump_statement(first_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported statement type is: %d\n",first_child(p)->type);
	}
}

void g_statement_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_first_child_type(p, EPSILON)){
		/*  Do nothing */
	}else if(check_two_children(p, STATEMENT, STATEMENT_LIST_REST)){
		buffered_printf(code_gen_state->buffered_output,";--- Start statement---.\n");
		g_statement(first_child(p), code_gen_state, 0, 0);
		buffered_printf(code_gen_state->buffered_output,";--- End statement---.\n");
		g_statement_list_rest(second_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported statement list rest.\n");
	}
}

void g_statement_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, STATEMENT, STATEMENT_LIST_REST)){
		buffered_printf(code_gen_state->buffered_output,";--- Start statement---.\n");
		g_statement(first_child(p), code_gen_state, 0, 0);
		buffered_printf(code_gen_state->buffered_output,";--- End statement---.\n");
		g_statement_list_rest(second_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported statement list.\n");
	}
}

void go_down_scope(struct code_gen_state * code_gen_state){
	struct parser_state * parser_state = code_gen_state->parser_state;
	/*  We'll now be descending into the next scope */
	if(unsigned_int_list_size(&code_gen_state->scope_index_list) == 0){
		/* Will be 0 if this scope was actually a struct/enum/union defintion */
		code_gen_state->current_function = parser_state->top_scope->scopes[code_gen_state->next_scope_index]->current_function;
	}
	unsigned_int_list_add(&code_gen_state->scope_index_list, code_gen_state->next_scope_index);
	code_gen_state->next_scope_index = 0;

}

struct scope_level * get_current_scope_level(struct code_gen_state * code_gen_state){
	unsigned int i;
	struct scope_level * current_scope = code_gen_state->parser_state->top_scope;
	for(i = 0; i < unsigned_int_list_size(&code_gen_state->scope_index_list); i++){
		/*  Traverse down to the current one */
		current_scope = current_scope->scopes[unsigned_int_list_get(&code_gen_state->scope_index_list, i)];
	}
	return current_scope;
}

void clear_locals_from_scope(struct code_gen_state * code_gen_state, struct scope_level * l){
	struct namespace_object * obj;
	if(l->first_local_object){
		/*  Free the space on the stack that was taken up by the locals in this scope */
		obj = l->first_local_object;
		while(obj){
			manage_local_space(code_gen_state, obj, 0);
			obj = obj->next;
		}
	}
}

void go_up_scope(struct code_gen_state * code_gen_state){
	clear_locals_from_scope(code_gen_state, get_current_scope_level(code_gen_state));
	code_gen_state->next_scope_index = 1 + unsigned_int_list_pop(&code_gen_state->scope_index_list);
	if(unsigned_int_list_size(&code_gen_state->scope_index_list) == 0){
		code_gen_state->current_function = 0; /*  We're not currently inside any function */
	}
}

void create_default_return_value(struct code_gen_state * code_gen_state, struct parser_node * possible_function, struct parser_node * context){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&code_gen_state->current_function->elements);
	struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&code_gen_state->current_function->elements, num_elements - 1);
	struct type_description * t = create_type_description_from_normalized_declaration_element(element);
	struct type_description * return_type_description = get_current_function_return_type_description(t);
	unsigned int rtn_val_size = type_size(code_gen_state, return_type_description, DATA_AND_PTR_BYTES, 1, get_current_scope_level(code_gen_state));
	unsigned int num_words;
	unsigned int i;
        (void)possible_function;
	num_words = rtn_val_size / 4;
	for(i = 0; i < num_words; i++){
		buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Creating default rtn value, word %d.\n", i);
	}
        destroy_type_description(t);

	push_type(code_gen_state, copy_type_description(return_type_description), context);
	destroy_type_description(return_type_description);
}

void return_from_function(struct code_gen_state * code_gen_state, struct parser_node * context){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&code_gen_state->current_function->elements);
	struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&code_gen_state->current_function->elements, num_elements - 1);
	struct type_description * t = create_type_description_from_normalized_declaration_element(element);
	struct type_description * return_type_description = get_current_function_return_type_description(t);
	unsigned int rtn_val_size = type_size(code_gen_state, return_type_description, DATA_AND_PTR_BYTES, 1, get_current_scope_level(code_gen_state));
        destroy_type_description(t);
        /*  SP should now be pointing to the top of the return value */
        buffered_printf(code_gen_state->buffered_output,"add r1 FP ZR;  r1 points to FP.\n");
        buffered_printf(code_gen_state->buffered_output,"add r1 r1 WR;  r1 points to return addr \n");
        buffered_printf(code_gen_state->buffered_output,"add r1 r1 WR;  r1 points to top of rtn space \n");
        buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;    Distance to last word of rtn space\n", rtn_val_size - 4);
        buffered_printf(code_gen_state->buffered_output,"add r1 r2 r1;  Source\n");
        buffered_printf(code_gen_state->buffered_output,"add r5 r2 SP;  Destination\n");
	traverse_type(code_gen_state, return_type_description, "r5", "r1", "r2", get_current_scope_level(code_gen_state), NEW_COPY, 0);
	buffered_printf(code_gen_state->buffered_output,";  Put the SP at the FP to get over the arguments and top rtn value \n");
	buffered_printf(code_gen_state->buffered_output,"add SP FP ZR;\n");
	buffered_printf(code_gen_state->buffered_output,"loa FP FP;  load the previous frame pointer \n");
	buffered_printf(code_gen_state->buffered_output,";  Jump over the frame pointer to get to the return address\n");
	buffered_printf(code_gen_state->buffered_output,"add SP SP WR;\n");
	buffered_printf(code_gen_state->buffered_output,";  Load the return address\n");
	buffered_printf(code_gen_state->buffered_output,"loa r1 SP;\n");
	buffered_printf(code_gen_state->buffered_output,";  Point the stack pointer to the top of return value\n");
	buffered_printf(code_gen_state->buffered_output,"add SP SP WR;\n");
	buffered_printf(code_gen_state->buffered_output,";  Jump back to the place where the function was called\n");
	buffered_printf(code_gen_state->buffered_output,"add PC ZR r1;\n");

	destroy_type_description(pop_type(code_gen_state, return_type_description, context));
	destroy_type_description(return_type_description);
	/*  There should never be anything on the type stack after we've poped returned the final value */
	assert(struct_type_description_ptr_list_size(&code_gen_state->type_stack) == 0);
}

void do_default_function_return(struct code_gen_state * code_gen_state, struct parser_node * possible_function, struct parser_node * context){
	if(!possible_function){
		return;
	}
	create_default_return_value(code_gen_state, possible_function, context);
	return_from_function(code_gen_state, possible_function);
}

void g_compound_statement(struct parser_node * p, struct code_gen_state * code_gen_state, struct parser_node * possible_function, unsigned char * evaluate_label, unsigned char * end_label){
	if(check_four_children(p, TERMINAL, DECLARATION_LIST, STATEMENT_LIST, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_BRACE_CHAR)
		){
			struct scope_level * scope;
			go_down_scope(code_gen_state);
			scope = get_current_scope_level(code_gen_state);
			if(evaluate_label){
				unsigned_char_ptr_list_add(&scope->evaluate_labels, evaluate_label);
			}
			if(end_label){
				unsigned_char_ptr_list_add(&scope->end_labels, end_label);
			}
			g_declaration_list(second_child(p), code_gen_state);
			g_statement_list(third_child(p), code_gen_state);
			do_default_function_return(code_gen_state, possible_function, fourth_child(p));
			if(evaluate_label){
				unsigned_char_ptr_list_pop(&scope->evaluate_labels);
			}
			if(end_label){
				unsigned_char_ptr_list_pop(&scope->end_labels);
			}
			go_up_scope(code_gen_state);
		}else{
			assert(0 && "Unknown terminals in compound statement.\n");
		}
	}else if(check_three_children(p, TERMINAL, DECLARATION_LIST, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_BRACE_CHAR)
		){
			struct scope_level * scope;
			go_down_scope(code_gen_state);
			scope = get_current_scope_level(code_gen_state);
			if(evaluate_label){
				unsigned_char_ptr_list_add(&scope->evaluate_labels, evaluate_label);
			}
			if(end_label){
				unsigned_char_ptr_list_add(&scope->end_labels, end_label);
			}
			g_declaration_list(second_child(p), code_gen_state);
			do_default_function_return(code_gen_state, possible_function, third_child(p));
			if(evaluate_label){
				unsigned_char_ptr_list_pop(&scope->evaluate_labels);
			}
			if(end_label){
				unsigned_char_ptr_list_pop(&scope->end_labels);
			}
			go_up_scope(code_gen_state);
		}else{
			assert(0 && "Unknown terminals in compound statement.\n");
		}
	}else if(check_three_children(p, TERMINAL, STATEMENT_LIST, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_BRACE_CHAR)
		){
			struct scope_level * scope;
			go_down_scope(code_gen_state);
			scope = get_current_scope_level(code_gen_state);
			if(evaluate_label){
				unsigned_char_ptr_list_add(&scope->evaluate_labels, evaluate_label);
			}
			if(end_label){
				unsigned_char_ptr_list_add(&scope->end_labels, end_label);
			}
			g_statement_list(second_child(p), code_gen_state);
			do_default_function_return(code_gen_state, possible_function, third_child(p));
			if(evaluate_label){
				unsigned_char_ptr_list_pop(&scope->evaluate_labels);
			}
			if(end_label){
				unsigned_char_ptr_list_pop(&scope->end_labels);
			}
			go_up_scope(code_gen_state);
		}else{
			assert(0 && "Unknown terminals in compound statement.\n");
		}
	}else if(check_two_children(p, TERMINAL, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(second_child(p),CLOSE_BRACE_CHAR)
		){
			struct scope_level * scope;
			go_down_scope(code_gen_state);
			scope = get_current_scope_level(code_gen_state);
			if(evaluate_label){
				unsigned_char_ptr_list_add(&scope->evaluate_labels, evaluate_label);
			}
			if(end_label){
				unsigned_char_ptr_list_add(&scope->end_labels, end_label);
			}
			do_default_function_return(code_gen_state, possible_function, second_child(p));
			if(evaluate_label){
				unsigned_char_ptr_list_pop(&scope->evaluate_labels);
			}
			if(end_label){
				unsigned_char_ptr_list_pop(&scope->end_labels);
			}
			go_up_scope(code_gen_state);
		}else{
			assert(0 && "Unknown terminals in compound statement.\n");
		}
		/*  Do nothing */
	}else{
		assert(0 && "Unsupported compound_statement.\n");
	}
}

void g_type_qualifier(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_first_child_type(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p),CONST)){
		}else if(is_terminal_c_token_type(first_child(p),VOLATILE)){
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported type_qualifier.\n");
		}
	}
}

void g_storage_class_specifier(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_first_child_type(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p),TYPEDEF)){
		}else if(is_terminal_c_token_type(first_child(p),EXTERN)){
		}else if(is_terminal_c_token_type(first_child(p),STATIC)){
		}else if(is_terminal_c_token_type(first_child(p),AUTO)){
		}else if(is_terminal_c_token_type(first_child(p),REGISTER)){
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported storage_class_specifier.\n");
		}
	}else{
		assert(0 &&"Expected terminal.\n");
	}
}

void g_enumerator(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, TERMINAL, CONSTANT_EXPRESSION)){
		if(
			is_terminal_c_token_type(first_child(p),IDENTIFIER) && 
			is_terminal_c_token_type(second_child(p), EQUALS_CHAR)
		){
		}else{
			buffered_printf(code_gen_state->buffered_output,";Unsupported enumerator.\n");
		}
	}else if(is_first_child_type(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p),IDENTIFIER)){
		}else{
			buffered_printf(code_gen_state->buffered_output,";Unsupported enumerator.\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported enumerator.\n");
	}
}

void g_enumerator_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, ENUMERATOR, ENUMERATOR_LIST_REST)){
		if(is_terminal_c_token_type(first_child(p),COMMA_CHAR)){
			g_enumerator(second_child(p), code_gen_state);
			g_enumerator_list_rest(third_child(p), code_gen_state);
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported enumerator_list_rest.\n");
		}
	}else if(is_first_child_type(p, EPSILON)){
		/*  Do nothing */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported enumerator_list_rest.\n");
	}
}

void g_enumerator_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, ENUMERATOR, ENUMERATOR_LIST_REST)){
		g_enumerator(first_child(p), code_gen_state);
		g_enumerator_list_rest(second_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported enumerator_list.\n");
	}
}

void g_enum_specifier(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, TERMINAL, ENUMERATOR_LIST, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),ENUM) && 
			is_terminal_c_token_type(second_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_BRACE_CHAR)
		){
			g_enumerator_list(third_child(p), code_gen_state);
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported enum_specifier.\n");
		}
	}else if(check_five_children(p, TERMINAL, TERMINAL, TERMINAL, ENUMERATOR_LIST, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),ENUM) &&
			is_terminal_c_token_type(second_child(p),IDENTIFIER) &&
			is_terminal_c_token_type(third_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(fifth_child(p),CLOSE_BRACE_CHAR)
		){
			g_enumerator_list(fourth_child(p), code_gen_state);
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported enum_specifier.\n");
		}
	}else if(check_two_children(p, TERMINAL, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),ENUM) &&
			is_terminal_c_token_type(second_child(p),IDENTIFIER)
		){
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported enum_specifier.\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported enum_specifier.\n");
	}
}

void g_struct_or_union(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_first_child_type(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p),STRUCT)){
		}else if(is_terminal_c_token_type(first_child(p),UNION)){
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported storage_class_specifier.\n");
		}
	}else{
		assert(0 &&"Expected terminal.\n");
	}
}

void g_type_specifier(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_first_child_type(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p),VOID)){
		}else if(is_terminal_c_token_type(first_child(p),CHAR)){
		}else if(is_terminal_c_token_type(first_child(p),SHORT)){
		}else if(is_terminal_c_token_type(first_child(p),INT)){
		}else if(is_terminal_c_token_type(first_child(p),LONG)){
		}else if(is_terminal_c_token_type(first_child(p),FLOAT)){
		}else if(is_terminal_c_token_type(first_child(p),DOUBLE)){
		}else if(is_terminal_c_token_type(first_child(p),SIGNED)){
		}else if(is_terminal_c_token_type(first_child(p),UNSIGNED)){
		}else if(is_terminal_c_token_type(first_child(p),IDENTIFIER)){
		}else{
			assert(0 && "Unsupported storage_class_specifier.\n");
		}
	}else if(is_first_child_type(p, ENUM_SPECIFIER)){
		g_enum_specifier(first_child(p), code_gen_state);
	}else if(is_first_child_type(p, STRUCT_OR_UNION_SPECIFIER)){
		g_struct_or_union_specifier(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected terminal or enum_specifier.\n");
	}
}

void g_declaration_specifiers(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, STORAGE_CLASS_SPECIFIER, DECLARATION_SPECIFIERS)){
		g_storage_class_specifier(first_child(p), code_gen_state);
		g_declaration_specifiers(second_child(p), code_gen_state);
	}else if(check_two_children(p, TYPE_SPECIFIER, DECLARATION_SPECIFIERS)){
		g_type_specifier(first_child(p), code_gen_state);
		g_declaration_specifiers(second_child(p), code_gen_state);
	}else if(check_two_children(p, TYPE_QUALIFIER, DECLARATION_SPECIFIERS)){
		g_type_qualifier(first_child(p), code_gen_state);
		g_declaration_specifiers(second_child(p), code_gen_state);
	}else if(check_one_child(p, STORAGE_CLASS_SPECIFIER)){
		g_storage_class_specifier(first_child(p), code_gen_state);
	}else if(check_one_child(p, TYPE_SPECIFIER)){
		g_type_specifier(first_child(p), code_gen_state);
	}else if(check_one_child(p, TYPE_QUALIFIER)){
		g_type_qualifier(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected terminal.\n");
	}
}

void g_init_declarator_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, INIT_DECLARATOR, INIT_DECLARATOR_LIST_REST)){
		if(is_terminal_c_token_type(first_child(p),COMMA_CHAR)){
			g_init_declarator(second_child(p), code_gen_state);
			g_init_declarator_list_rest(third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected COMMA_CHAR.\n");
		}
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected init_declarator_list_rest.\n");
	}
}

void g_init_declarator_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, INIT_DECLARATOR,INIT_DECLARATOR_LIST_REST)){
		g_init_declarator(first_child(p), code_gen_state);
		g_init_declarator_list_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected init_declarator_list.\n");
	}
}

void g_declaration(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, DECLARATION_SPECIFIERS,INIT_DECLARATOR_LIST, TERMINAL)){
		if(is_terminal_c_token_type(third_child(p),SEMICOLON_CHAR)){
			g_declaration_specifiers(first_child(p), code_gen_state);
			g_init_declarator_list(second_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected terminal.\n");
		}
	}else if(check_two_children(p, DECLARATION_SPECIFIERS,TERMINAL)){
		if(is_terminal_c_token_type(second_child(p),SEMICOLON_CHAR)){
			g_declaration_specifiers(first_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected terminal.\n");
		}
	}else{
		assert(0 &&"Expected DECLARATION.\n");
	}
}

void g_declaration_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, DECLARATION, DECLARATION_LIST_REST)){
		g_declaration(first_child(p), code_gen_state);
		g_declaration_list_rest(second_child(p), code_gen_state);
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected DECLARATION.\n");
	}
}

void g_declaration_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, DECLARATION, DECLARATION_LIST_REST)){
		g_declaration(first_child(p), code_gen_state);
		g_declaration_list_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected DECLARATION.\n");
	}
}

void g_identifier_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, TERMINAL, IDENTIFIER_LIST_REST)){
		if(
			is_terminal_c_token_type(first_child(p),COMMA_CHAR) &&
			is_terminal_c_token_type(second_child(p),IDENTIFIER)
		){
			g_identifier_list_rest(third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected COMMA_CHAR.\n");
		}
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected identifier_list_rest.\n");
	}
}

void g_identifier_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, TERMINAL, IDENTIFIER_LIST_REST)){
		if(is_terminal_c_token_type(first_child(p),IDENTIFIER)){
			g_identifier_list_rest(second_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected COMMA_CHAR.\n");
		}
	}else{
		assert(0 &&"Expected identifier_list.\n");
	}
}

void g_direct_declarator_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, CONSTANT_EXPRESSION, TERMINAL, DIRECT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_SQUARE_BRACKET_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_SQUARE_BRACKET_CHAR)
		){
			/*  Not evaluating expression here, because I don't think its necessary and it loads a type we don't want */
			g_direct_declarator_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected direct_declarator rest.\n");
		}
	}else if(check_four_children(p, TERMINAL, PARAMETER_TYPE_LIST, TERMINAL, DIRECT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_PAREN_CHAR)
		){
			g_parameter_type_list(second_child(p), code_gen_state);
			g_direct_declarator_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected direct_declarator rest.\n");
		}
	}else if(check_four_children(p, TERMINAL, IDENTIFIER_LIST, TERMINAL, DIRECT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_PAREN_CHAR)
		){
			g_identifier_list(second_child(p), code_gen_state);
			g_direct_declarator_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected direct_declarator rest.\n");
		}
	}else if(check_three_children(p, TERMINAL, TERMINAL, DIRECT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_SQUARE_BRACKET_CHAR) &&
			is_terminal_c_token_type(second_child(p),CLOSE_SQUARE_BRACKET_CHAR)
		){
			g_direct_declarator_rest(third_child(p), code_gen_state);
		} else if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(second_child(p),CLOSE_PAREN_CHAR)
		){
			g_direct_declarator_rest(third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected direct_declarator rest.\n");
		}
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected direct_declarator rest.\n");
	}
}

void g_type_qualifier_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, TYPE_QUALIFIER, TYPE_QUALIFIER_LIST_REST)){
		g_type_qualifier(first_child(p), code_gen_state);
		g_type_qualifier_list_rest(second_child(p), code_gen_state);
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected type_qualifier_list_rest.\n");
	}
}

void g_type_qualifier_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, TYPE_QUALIFIER, TYPE_QUALIFIER_LIST_REST)){
		g_type_qualifier(first_child(p), code_gen_state);
		g_type_qualifier_list_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected g_type_qualifier_list.\n");
	}
}

void g_pointer(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(is_terminal_c_token_type(first_child(p),MULTIPLY_CHAR)) {
		if(check_three_children(p, TERMINAL, TYPE_QUALIFIER_LIST, POINTER)){
			g_type_qualifier_list(second_child(p), code_gen_state);
			g_pointer(third_child(p), code_gen_state);
		}else if(check_two_children(p, TERMINAL, TYPE_QUALIFIER_LIST)){
			g_type_qualifier_list(second_child(p), code_gen_state);
		}else if(check_two_children(p, TERMINAL, POINTER)){
			g_pointer(second_child(p), code_gen_state);
		}else if(check_one_child(p, TERMINAL)){
		}else{
			buffered_printf(code_gen_state->buffered_output,"not possible.\n");
		}
	}else{
		assert(0 &&"Expected terminal.\n");
	}
}

void manage_local_space(struct code_gen_state * code_gen_state, struct namespace_object * obj, unsigned is_initializing){
	/*  Manages the local space for a namespace object, either by adding or removing the spaces that it will occupy */
	unsigned int arity = 1;
	unsigned int i;
	const char * instruction = is_initializing ? "sub" : "add";
	unsigned int obj_size = get_namespace_object_size(code_gen_state, obj, 0);
	arity = obj_size / 8;
	assert((obj_size - (arity * 8)) == 0);
	for(i = 0; i < arity; i++){
		buffered_printf(code_gen_state->buffered_output,"%s SP SP WR; Room for local val\n", instruction);
		buffered_printf(code_gen_state->buffered_output,"%s SP SP WR; Room for local ptr\n", instruction); 
		if(is_initializing){
			buffered_printf(code_gen_state->buffered_output,"sto SP SP; Make it point to itself.\n"); 
		}
	}
}

void g_direct_declarator(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, DECLARATOR, TERMINAL, DIRECT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_PAREN_CHAR)
		){
			g_declarator(second_child(p), code_gen_state);
			g_direct_declarator_rest(fourth_child(p), code_gen_state);
		}
	}else if(check_two_children(p, TERMINAL, DIRECT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),IDENTIFIER)
		){
			unsigned char * identifier = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte);
			load_identifier(code_gen_state, identifier, p);
			free(identifier);
			g_direct_declarator_rest(second_child(p), code_gen_state);
		}
	}else{
		assert(0 &&"Expected terminal in direct_declarator.\n");
	}
}


void g_declarator(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, POINTER, DIRECT_DECLARATOR)){
		g_pointer(first_child(p), code_gen_state);
		g_direct_declarator(second_child(p), code_gen_state);
	}else if(check_one_child(p, DIRECT_DECLARATOR)){
		g_direct_declarator(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected pointer or direct_declarator.\n");
	}
}

void g_initializer_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, INITIALIZER, INITIALIZER_LIST_REST)){
		g_initializer(second_child(p), code_gen_state);
		g_initializer_list_rest(third_child(p), code_gen_state);
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected initializer.\n");
	}
}

void g_initializer_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, INITIALIZER, INITIALIZER_LIST_REST)){
		g_initializer(first_child(p), code_gen_state);
		g_initializer_list_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected initializer.\n");
	}
}

void g_initializer(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, INITIALIZER_LIST, TERMINAL, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(third_child(p),COMMA_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_BRACE_CHAR)
		){
			g_initializer_list(second_child(p), code_gen_state);
		}
	}else if(check_three_children(p, TERMINAL, INITIALIZER_LIST, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_BRACE_CHAR)
		){
			g_initializer_list(second_child(p), code_gen_state);
		}
	}else if(check_one_child(p, ASSIGNMENT_EXPRESSION)){
		g_assignment_expression(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected OPEN_BRACE_CHAR or assignment expression.\n");
	}
}

void consume_scalar_type(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * observed_type = pop_type_without_type_check(code_gen_state, context);
	if(!is_scalar_type(observed_type)){
		struct unsigned_char_list l;
		unsigned_char_list_create(&l);
		buffered_printf(&l,"\nThis is not a scalar type:\n");
		print_normalized_declaration_declarator_and_specifiers(&l, observed_type->declarator, observed_type->specifiers, 0, 0);
		unsigned_char_list_add(&l, 0);
		printf("%s\n", unsigned_char_list_data(&l));
		print_node_context(code_gen_state->parser_state->c_lexer_state, context);
		assert(0 && "This is not a scalar type.\n");
	}
	destroy_type_description(observed_type);
}

struct type_description * manage_assignment_type_change(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * t1 = pop_type_without_type_check(code_gen_state, context);
	struct type_description * t2 = pop_type_without_type_check(code_gen_state, context);
	unsigned int compare_successful = !type_description_cmp(t1, t2);
	if(!compare_successful){
		struct unsigned_char_list list;
		unsigned_char_list_create(&list);
		buffered_printf(&list,";Type missmatch:\n");
		buffered_printf(&list,";Trying to assign type:\n");
		print_normalized_declaration_declarator_and_specifiers(&list, t1->declarator, t1->specifiers, 0, 0);
		buffered_printf(&list,";With observed type B:\n");
		print_normalized_declaration_declarator_and_specifiers(&list, t2->declarator, t2->specifiers, 0, 0);
		buffered_printf(&list,";Type compare failed\n");
		unsigned_char_list_add(&list, 0);
		printf("%s", (unsigned char *)unsigned_char_list_data(&list));
		unsigned_char_list_destroy(&list);
		print_node_context(code_gen_state->parser_state->c_lexer_state, context);
		assert(0 && "Compare failed.");
		return 0;
	}else{
		push_type(code_gen_state, t2, context);
		return t1;
	}
}

void do_assignment(struct code_gen_state * code_gen_state, struct parser_node * context){
        /*  Stack top is thing we assign to, stack top -1 is thing to assign  */
        struct type_description * assign_type = manage_assignment_type_change(code_gen_state, context);
	unsigned int size = type_size(code_gen_state, assign_type, DATA_AND_PTR_BYTES, 0, assign_type->source_scope_level);
        buffered_printf(code_gen_state->buffered_output,";                 Begin Assignment\n");
        buffered_printf(code_gen_state->buffered_output,"ll r6 0x%X;        Bytes to last word of value to assign\n", size - 4);
        buffered_printf(code_gen_state->buffered_output,"add r7 SP r6;\n");
        buffered_printf(code_gen_state->buffered_output,"add r1 r7 ZR;     We want to start the copy from addr in r1\n");
        buffered_printf(code_gen_state->buffered_output,"add r7 r7 WR;     Go down to get ptr to item\n");
        buffered_printf(code_gen_state->buffered_output,";                 Now r7 Points to a the pointer that will point to the location\n");
        buffered_printf(code_gen_state->buffered_output,";                 of the item we want to assign the value to\n");
        buffered_printf(code_gen_state->buffered_output,"loa r2 r7;\n");
        buffered_printf(code_gen_state->buffered_output,"add r7 r7 r6;     Point stack to value\n");
        buffered_printf(code_gen_state->buffered_output,"add r2 r2 r6;     r2 points the the value's pointer, move to its last word value\n");
	traverse_type(code_gen_state, assign_type, "r1", "r2", "r5", assign_type->source_scope_level, ASSIGN_COPY, 0);
        buffered_printf(code_gen_state->buffered_output,"add r1 r1 r6;     Move src pointer back down to do copy again\n");
	traverse_type(code_gen_state, assign_type, "r1", "r7", "r5", assign_type->source_scope_level, NEW_COPY, 0);
        buffered_printf(code_gen_state->buffered_output,"ll r6 0x%X;        It is now safe to move SP down\n", size);
        buffered_printf(code_gen_state->buffered_output,"add SP SP r6;     Now only one item\n");
        buffered_printf(code_gen_state->buffered_output,";                 End Assignment\n");

        destroy_type_description(assign_type);
}

void g_init_declarator(struct parser_node * p, struct code_gen_state * code_gen_state){
	struct parser_node * identifier = get_identifier_from_declarator(first_child(p));
	unsigned char * identifier_str = copy_string(identifier->c_lexer_token->first_byte, identifier->c_lexer_token->last_byte);
	struct namespace_object * obj;
	unsigned int num_elements;
	struct normalized_declaration_element * element;
	struct type_description * type_description;
	unsigned int is_global;
	unsigned int is_extern;
	struct parser_node * abstract_declarator;
	obj = get_namespace_object_from_closest_namespace(identifier_str, IDENTIFIER_NAMESPACE, get_current_scope_level(code_gen_state), 0);
	num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, num_elements - 1);
	type_description = create_type_description_from_normalized_declaration_element(element);
	type_description->source_scope_level = obj->scope_level;
	convert_to_untypedefed_type_description(type_description);
	is_global = obj->scope_level == code_gen_state->parser_state->top_scope;

	abstract_declarator = create_abstract_declarator_from_normalized_declarator(element->normalized_declarator);

	is_extern = count_specifiers(type_description, EXTERN) ? 1 : 0;

	if(!is_function(abstract_declarator)){
		if(check_three_children(p, DECLARATOR, TERMINAL, INITIALIZER)){
			if(is_terminal_c_token_type(second_child(p),EQUALS_CHAR)){
				if(is_global){
					if(!is_extern){
						unsigned char * name;
						unsigned int expression_value = evaluate_constant_expression(code_gen_state, third_child(p));
						sprintf_hook("globalvar_%s", identifier_str);
						name = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
						buffered_printf(code_gen_state->buffered_output,"%s:\n", name);
						buffered_printf(code_gen_state->buffered_output,"dw %s;\n", name);
						buffered_printf(code_gen_state->buffered_output,"dw 0x%X;\n", expression_value);
						implement_external_symbol(&code_gen_state->symbols, name);
						free(name);
					}
				}else{
					g_declarator(first_child(p), code_gen_state);
					g_initializer(third_child(p), code_gen_state);
					buffered_printf(code_gen_state->buffered_output,";  Performing = initializer declaration.\n");
					do_assignment(code_gen_state, p);
					/* Remove the result of the declarator */
					delete_top_type(code_gen_state);
				}
			}else{
				assert(0 && "Unknown token.");
			}
		}else if(check_one_child(p, DECLARATOR)){
			if(is_global){
				if(!is_extern){
					unsigned char * name;
					sprintf_hook("globalvar_%s", identifier_str);
					name = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
					buffered_printf(code_gen_state->buffered_output,"%s:\n", name);

					/*  The register specified here don't actually do anything. */
					traverse_type(code_gen_state, type_description, "SP", "SP", "SP", get_current_scope_level(code_gen_state), SETUP_GLOBAL_SPACE, 0);

					implement_external_symbol(&code_gen_state->symbols, name);
					free(name);

				}
			}else{
				g_declarator(first_child(p), code_gen_state);
				/* Remove the result of the declarator */
				delete_top_type(code_gen_state);
			}
		}else{
			assert(0 &&"Expected direct_declarator.\n");
		}
	}

	destroy_parser_node_tree_and_c_lexer_tokens(abstract_declarator);
	destroy_type_description(type_description);
	free(identifier_str);
}

void g_struct_declarator(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, DECLARATOR, TERMINAL, CONSTANT_EXPRESSION)){
		if(is_terminal_c_token_type(second_child(p),COLON_CHAR)){
			g_declarator(first_child(p), code_gen_state);
			/*g_constant_expression(third_child(p), code_gen_state);*/
		}
	}else if(check_two_children(p, TERMINAL, CONSTANT_EXPRESSION)){
		if(is_terminal_c_token_type(first_child(p),COLON_CHAR)){
			/*g_constant_expression(second_child(p), code_gen_state);*/
			buffered_printf(code_gen_state->buffered_output,";padding : bitfield thing.\n");
		}
	}else if(check_one_child(p, DECLARATOR)){
		g_declarator(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected declarator or :.\n");
	}
}

void g_struct_declarator_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, STRUCT_DECLARATOR, STRUCT_DECLARATOR_LIST_REST)){
		if(is_terminal_c_token_type(first_child(p),COMMA_CHAR)){
			g_struct_declarator(second_child(p), code_gen_state);
			g_struct_declarator_list_rest(third_child(p), code_gen_state);
		}
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected struct_declarator\n");
	}
}

void g_struct_declarator_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, STRUCT_DECLARATOR, STRUCT_DECLARATOR_LIST_REST)){
		g_struct_declarator(first_child(p), code_gen_state);
		g_struct_declarator_list_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected struct_declarator\n");
	}
}

void g_specifier_qualifier_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, TYPE_SPECIFIER, SPECIFIER_QUALIFIER_LIST)){
		g_type_specifier(first_child(p), code_gen_state);
		g_specifier_qualifier_list(second_child(p), code_gen_state);
	}else if(check_two_children(p, TYPE_QUALIFIER, SPECIFIER_QUALIFIER_LIST)){
		g_type_qualifier(first_child(p), code_gen_state);
		g_specifier_qualifier_list(second_child(p), code_gen_state);
	}else if(check_one_child(p, TYPE_SPECIFIER)){
		g_type_specifier(first_child(p), code_gen_state);
	}else if(check_one_child(p, TYPE_SPECIFIER)){
		g_type_qualifier(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected specifier_qualifier_list\n");
	}
}

void g_struct_declaration(struct parser_node * p, struct code_gen_state * code_gen_state){
	(void)code_gen_state;
	if(check_three_children(p, SPECIFIER_QUALIFIER_LIST, STRUCT_DECLARATOR_LIST, TERMINAL)){
		if(is_terminal_c_token_type(third_child(p),SEMICOLON_CHAR)){
			/*g_specifier_qualifier_list(first_child(p), code_gen_state);*/
			/*g_struct_declarator_list(second_child(p), code_gen_state);*/
		}
	}else{
		assert(0 &&"Expected struct_declaration\n");
	}
}

void g_struct_declaration_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, STRUCT_DECLARATION, STRUCT_DECLARATION_LIST_REST)){
		g_struct_declaration(first_child(p), code_gen_state);
		g_struct_declaration_list_rest(second_child(p), code_gen_state);
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected struct_declaration_list_rest\n");
	}
}

void g_struct_declaration_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, STRUCT_DECLARATION, STRUCT_DECLARATION_LIST_REST)){
		g_struct_declaration(first_child(p), code_gen_state);
		g_struct_declaration_list_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected struct_declaration_list\n");
	}
}

void g_struct_or_union_specifier(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_five_children(p, STRUCT_OR_UNION, TERMINAL, TERMINAL, STRUCT_DECLARATION_LIST, TERMINAL)){
		if(
			is_terminal_c_token_type(second_child(p),IDENTIFIER) &&
			is_terminal_c_token_type(third_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(fifth_child(p),CLOSE_BRACE_CHAR)
		){
			g_struct_or_union(first_child(p), code_gen_state);
			g_struct_declaration_list(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected struct_or_union_specifier\n");
		}
	}else if(check_four_children(p, STRUCT_OR_UNION, TERMINAL, STRUCT_DECLARATION_LIST, TERMINAL)){
		if(
			is_terminal_c_token_type(second_child(p),OPEN_BRACE_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_BRACE_CHAR)
		){
			g_struct_or_union(first_child(p), code_gen_state);
			g_struct_declaration_list(third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected struct_or_union_specifier\n");
		}
	}else if(check_two_children(p, STRUCT_OR_UNION, TERMINAL)){
		if(
			is_terminal_c_token_type(second_child(p),IDENTIFIER)
		){
			g_struct_or_union(first_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected struct_or_union_specifier\n");
		}
	}else{
		assert(0 &&"Expected struct_or_union_specifier\n");
	}
}

void g_parameter_declaration(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, DECLARATION_SPECIFIERS, ABSTRACT_DECLARATOR)){
		g_declaration_specifiers(first_child(p), code_gen_state);
		g_abstract_declarator(second_child(p), code_gen_state);
	}else if(check_two_children(p, DECLARATION_SPECIFIERS, DECLARATOR)){
		g_declaration_specifiers(first_child(p), code_gen_state);
		/*g_declarator(second_child(p), code_gen_state);*/
	}else if(check_one_child(p, DECLARATION_SPECIFIERS)){
		g_declaration_specifiers(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected parameter declaration.\n");
	}
}

void g_parameter_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, PARAMETER_DECLARATION, PARAMETER_LIST_REST)){
		if(is_terminal_c_token_type(first_child(p),COMMA_CHAR)){
			g_parameter_declaration(second_child(p), code_gen_state);
			g_parameter_list_rest(third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected comma for terminal.\n");
		}
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected parameter parameter_list.\n");
	}
}

void g_parameter_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, PARAMETER_DECLARATION, PARAMETER_LIST_REST)){
		g_parameter_declaration(first_child(p), code_gen_state);
		g_parameter_list_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected parameter parameter_list.\n");
	}
}

void g_parameter_type_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, PARAMETER_LIST, TERMINAL, TERMINAL)){
		if(
			is_terminal_c_token_type(second_child(p),COMMA_CHAR) &&
			is_terminal_c_token_type(third_child(p),ELLIPSIS)
		){
			g_parameter_list(first_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected ,....\n");
		}
	}else if(check_one_child(p, PARAMETER_LIST)){
		g_parameter_list(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected parameter parameter_list.\n");
	}
}

void g_direct_abstract_declarator_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, CONSTANT_EXPRESSION, TERMINAL, DIRECT_ABSTRACT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_SQUARE_BRACKET_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_SQUARE_BRACKET_CHAR)
		){
			g_constant_expression(second_child(p), code_gen_state);
			g_direct_abstract_declarator_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected []");
		}
	}else if(check_four_children(p, TERMINAL, PARAMETER_TYPE_LIST, TERMINAL, DIRECT_ABSTRACT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_PAREN_CHAR)
		){
			g_parameter_type_list(second_child(p), code_gen_state);
			g_direct_abstract_declarator_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected ()");
		}
	}else if(check_three_children(p, TERMINAL, TERMINAL, DIRECT_ABSTRACT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_SQUARE_BRACKET_CHAR) &&
			is_terminal_c_token_type(second_child(p),CLOSE_SQUARE_BRACKET_CHAR)
		){
			g_direct_abstract_declarator_rest(third_child(p), code_gen_state);
		}else if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(second_child(p),CLOSE_PAREN_CHAR)
		){
			g_direct_abstract_declarator_rest(third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected [] or ()");
		}
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected direct_abstract_declarator_rest.\n");
	}
}

void g_direct_abstract_declarator(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, CONSTANT_EXPRESSION, TERMINAL, DIRECT_ABSTRACT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_SQUARE_BRACKET_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_SQUARE_BRACKET_CHAR)
		){
			g_constant_expression(second_child(p), code_gen_state);
			g_direct_abstract_declarator_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected []");
		}
	}else if(check_four_children(p, TERMINAL, PARAMETER_TYPE_LIST, TERMINAL, DIRECT_ABSTRACT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_PAREN_CHAR)
		){
			g_parameter_type_list(second_child(p), code_gen_state);
			g_direct_abstract_declarator_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected ()");
		}
	}else if(check_four_children(p, TERMINAL, ABSTRACT_DECLARATOR, TERMINAL, DIRECT_ABSTRACT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(third_child(p),CLOSE_PAREN_CHAR)
		){
			g_abstract_declarator(second_child(p), code_gen_state);
			g_direct_abstract_declarator_rest(fourth_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected ()");
		}
	}else if(check_three_children(p, TERMINAL, TERMINAL, DIRECT_ABSTRACT_DECLARATOR_REST)){
		if(
			is_terminal_c_token_type(first_child(p),OPEN_SQUARE_BRACKET_CHAR) &&
			is_terminal_c_token_type(second_child(p),CLOSE_SQUARE_BRACKET_CHAR)
		){
			g_direct_abstract_declarator_rest(third_child(p), code_gen_state);
		}else if(
			is_terminal_c_token_type(first_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(second_child(p),CLOSE_PAREN_CHAR)
		){
			g_direct_abstract_declarator_rest(third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected [] or ()");
		}
	}else{
		assert(0 &&"Expected direct_abstract_declarator.\n");
	}
}

void g_abstract_declarator(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, POINTER, DIRECT_ABSTRACT_DECLARATOR)){
		g_pointer(first_child(p), code_gen_state);
		g_direct_abstract_declarator(second_child(p), code_gen_state);
	}else if(check_one_child(p, POINTER)){
		g_pointer(first_child(p), code_gen_state);
	}else if(check_one_child(p, DIRECT_ABSTRACT_DECLARATOR)){
		g_direct_abstract_declarator(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected abstract_declarator.\n");
	}
}

void g_labeled_statement(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, CONSTANT_EXPRESSION, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),CASE) &&
			is_terminal_c_token_type(third_child(p),COLON_CHAR)
		){
			struct switch_frame * frame;
			unsigned switch_index = 0;
			unsigned switch_value;
			unsigned i;
			unsigned char * case_str;
			assert(struct_switch_frame_ptr_list_size(&code_gen_state->switch_frames));
			frame = struct_switch_frame_ptr_list_get(&code_gen_state->switch_frames, struct_switch_frame_ptr_list_size(&code_gen_state->switch_frames)-1);
			assert(second_child(p)->type == CONSTANT_EXPRESSION);
			switch_value = evaluate_constant_expression(code_gen_state, second_child(p));
			/*  Determine which label this from the perspective if the outer switch statement. */
			for(i = 0; i < unsigned_int_list_size(&frame->values); i++){
				if(unsigned_int_list_get(&frame->values, i) == switch_value){
					switch_index = i;
					break;
				}
				assert(i != unsigned_int_list_size(&frame->values) -1); /* Not found. */
			}

			sprintf_hook("select%dcase%d", frame->condition_index, switch_index);
			case_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			buffered_printf(code_gen_state->buffered_output,"%s:\n", case_str);
			implement_internal_symbol(&code_gen_state->symbols, case_str);
			free(case_str);
			g_statement(fourth_child(p), code_gen_state, 0, 0);
		}else{
			assert(0 &&"Expected case and colon.\n");
		}
	}else if(check_three_children(p, TERMINAL, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),IDENTIFIER) &&
			is_terminal_c_token_type(second_child(p),COLON_CHAR)
		){
			unsigned char * default_str;
			default_str = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", default_str);
			implement_internal_symbol(&code_gen_state->symbols, default_str);
			g_statement(third_child(p), code_gen_state, 0, 0);
			free(default_str);
		}else if(
			is_terminal_c_token_type(first_child(p),DEFAULT) &&
			is_terminal_c_token_type(second_child(p),COLON_CHAR)
		){
			struct switch_frame * frame;
			unsigned char * default_str;
			assert(struct_switch_frame_ptr_list_size(&code_gen_state->switch_frames));
			frame = struct_switch_frame_ptr_list_get(&code_gen_state->switch_frames, struct_switch_frame_ptr_list_size(&code_gen_state->switch_frames)-1);
			sprintf_hook("select%ddefault", frame->condition_index);
			default_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			buffered_printf(code_gen_state->buffered_output,"%s:\n", default_str);
			implement_internal_symbol(&code_gen_state->symbols, default_str);
			free(default_str);
			g_statement(third_child(p), code_gen_state, 0, 0);
		}else{
			assert(0 &&"Expected identifier and colon.\n");
		}
	}else{
		assert(0 &&"Expected labeled_statement.\n");
	}
}


void find_child_case_labels_h(struct code_gen_state * code_gen_state, struct parser_node * p, struct switch_frame * frame){
	struct parser_node * n = p;
	if(n){
		/*  Follow any statements, except switches because labels apply to the closest parent switch */
		if(n->type == SELECTION_STATEMENT && first_child(n)->c_lexer_token->type == IF){
			find_child_case_labels_h(code_gen_state, fifth_child(n), frame);
			if(p->first_child->next->next->next->next->next){
				find_child_case_labels_h(code_gen_state, seventh_child(n), frame);
			}
		}else if(n->type == COMPOUND_STATEMENT){
			find_child_case_labels_h(code_gen_state, second_child(n), frame);
			if(p->first_child->next){
				find_child_case_labels_h(code_gen_state, third_child(n), frame);
			}
		}else if(n->type == ITERATION_STATEMENT){
			find_child_case_labels_h(code_gen_state, second_child(n), frame);
			find_child_case_labels_h(code_gen_state, fifth_child(n), frame);
			if(p->first_child->next->next->next->next){
				find_child_case_labels_h(code_gen_state, sixth_child(n), frame);
			}
			if(p->first_child->next->next->next->next->next){
				find_child_case_labels_h(code_gen_state, seventh_child(n), frame);
			}
		}else if(n->type == STATEMENT){
			find_child_case_labels_h(code_gen_state, first_child(n), frame);
		}else if(n->type == STATEMENT_LIST){
			find_child_case_labels_h(code_gen_state, first_child(n), frame);
			find_child_case_labels_h(code_gen_state, second_child(n), frame);
		}else if(n->type == STATEMENT_LIST_REST){
			find_child_case_labels_h(code_gen_state, first_child(n), frame);
			find_child_case_labels_h(code_gen_state, second_child(n), frame);
		}else if(n->type == LABELED_STATEMENT){
			if(n->first_child->c_lexer_token->type == CASE){
				unsigned int size;
				assert(n->first_child->next->type == CONSTANT_EXPRESSION);
				size = evaluate_constant_expression(code_gen_state, n->first_child->next);
				unsigned_int_list_add(&frame->values, size);
				find_child_case_labels_h(code_gen_state, fourth_child(n), frame);
			}else if(n->first_child->c_lexer_token->type == DEFAULT){
				find_child_case_labels_h(code_gen_state, third_child(n), frame);
				frame->has_default = 1;
			}
		}
	}
}

void find_child_case_labels(struct code_gen_state * code_gen_state, struct parser_node * p, struct switch_frame * frame){
	assert(p->type == STATEMENT);
	frame->has_default = 0;
	unsigned_int_list_create(&frame->values);
	find_child_case_labels_h(code_gen_state, p->first_child, frame);
}

void g_selection_statement(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_seven_children(p, TERMINAL, TERMINAL, EXPRESSION, TERMINAL, STATEMENT, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),IF) &&
			is_terminal_c_token_type(second_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_PAREN_CHAR) &&
			is_terminal_c_token_type(sixth_child(p),ELSE)
		){
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * false_condition_str;
			unsigned char * after_condition_str;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			sprintf_hook("falsecondition%d", cond_index);
			false_condition_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			sprintf_hook("aftercondition%d", cond_index);
			after_condition_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			g_expression(third_child(p), code_gen_state);
			/*  Pop the item that was loaded in the conditional */
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;\n"); /* Pop addr  */
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;\n"); /* Pop value */
			consume_scalar_type(code_gen_state, third_child(p));
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s;\n", false_condition_str);
			g_statement(fifth_child(p), code_gen_state, 0, 0);
			buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;\n", after_condition_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", false_condition_str);
			g_statement(seventh_child(p), code_gen_state, 0, 0);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", after_condition_str);
			require_internal_symbol(&code_gen_state->symbols, false_condition_str);
			implement_internal_symbol(&code_gen_state->symbols, false_condition_str);
			require_internal_symbol(&code_gen_state->symbols, after_condition_str);
			implement_internal_symbol(&code_gen_state->symbols, after_condition_str);
			free(after_condition_str);
			free(false_condition_str);
		}else{
			assert(0 &&"Expected case and colon.\n");
		}
	}else if(check_five_children(p, TERMINAL, TERMINAL, EXPRESSION, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),IF) &&
			is_terminal_c_token_type(second_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_PAREN_CHAR) 
		){
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * after_condition_str;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			sprintf_hook("aftercondition%d", cond_index);
			after_condition_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			g_expression(third_child(p), code_gen_state);
			/*  Pop the item that was loaded in the conditional */
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;\n"); /* Pop addr  */
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;\n"); /* Pop value */
			consume_scalar_type(code_gen_state, third_child(p));
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s;\n", after_condition_str);
			g_statement(fifth_child(p), code_gen_state, 0, 0);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", after_condition_str);
			require_internal_symbol(&code_gen_state->symbols, after_condition_str);
			implement_internal_symbol(&code_gen_state->symbols, after_condition_str);
			free(after_condition_str);
		}else if(
			is_terminal_c_token_type(first_child(p),SWITCH) &&
			is_terminal_c_token_type(second_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_PAREN_CHAR) 
		){
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * after_condition_str;
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(fifth_child(p))->type == COMPOUND_STATEMENT;
			struct switch_frame * frame = malloc(sizeof(struct switch_frame));
			unsigned int num_cases;
			unsigned int i;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			frame->condition_index = code_gen_state->condition_index;
			sprintf_hook("aftercondition%d", cond_index);
			after_condition_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			g_expression(third_child(p), code_gen_state);

			pop(code_gen_state, "r1"); /* This is the value we need to compare against in the jump table */
			consume_scalar_type(code_gen_state, third_child(p));

			find_child_case_labels(code_gen_state, fifth_child(p), frame);
			num_cases = unsigned_int_list_size(&frame->values);
			for(i = 0; i < num_cases; i++){
				unsigned int value = unsigned_int_list_get(&frame->values, i);
				unsigned char * case_str;
				sprintf_hook("select%dcase%d", frame->condition_index, i);
				case_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
				buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;\n", value);
				buffered_printf(code_gen_state->buffered_output,"beq r1 r2 %s;\n", case_str);
				require_internal_symbol(&code_gen_state->symbols, case_str);
				free(case_str);
			}

			if(frame->has_default){
				unsigned char * default_str;
				sprintf_hook("select%ddefault", frame->condition_index);
				default_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
				buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;\n", default_str);
				require_internal_symbol(&code_gen_state->symbols, default_str);
				free(default_str);
			}else{
				/*  There is no case in this switch, jump directly to the end */
				buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;\n", after_condition_str);
			}

			struct_switch_frame_ptr_list_add(&code_gen_state->switch_frames, frame);
			if(uses_compound_statement){
				g_statement(fifth_child(p), code_gen_state, 0, after_condition_str);
			}else{
				unsigned_char_ptr_list_add(&scope->end_labels, after_condition_str);
				g_statement(fifth_child(p), code_gen_state, 0, 0);
			}
			unsigned_int_list_destroy(&frame->values);
			free(struct_switch_frame_ptr_list_pop(&code_gen_state->switch_frames));

			buffered_printf(code_gen_state->buffered_output,"%s:\n", after_condition_str);
			require_internal_symbol(&code_gen_state->symbols, after_condition_str);
			implement_internal_symbol(&code_gen_state->symbols, after_condition_str);
			if(uses_compound_statement){
				free(after_condition_str);
			}else{
				free(unsigned_char_ptr_list_pop(&scope->end_labels));
			}
		}else{
			assert(0 &&"Expected if or switch.\n");
		}

	}else{
		assert(0 &&"Expected selection statement.\n");
	}
}

void g_iteration_statement(struct parser_node * p, struct code_gen_state * code_gen_state){
	struct parser_state * parser_state = code_gen_state->parser_state;
	if(check_seven_children(p, TERMINAL, STATEMENT, TERMINAL, TERMINAL, EXPRESSION, TERMINAL, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),DO) &&
			is_terminal_c_token_type(third_child(p),WHILE) &&
			is_terminal_c_token_type(fourth_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(sixth_child(p),CLOSE_PAREN_CHAR) &&
			is_terminal_c_token_type(seventh_child(p),SEMICOLON_CHAR)
		){

			unsigned char * startwhile_str;
			unsigned char * evaluatewhile_str;
			unsigned char * endwhile_str;
			unsigned int cond_index = code_gen_state->condition_index;
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(second_child(p))->type == COMPOUND_STATEMENT;
			(void)scope;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			sprintf_hook("endwhile%d", cond_index);
			endwhile_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			sprintf_hook("evaluatewhile%d", cond_index);
			evaluatewhile_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			sprintf_hook("startwhile%d", cond_index);
			startwhile_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));

			buffered_printf(code_gen_state->buffered_output,"%s:\n", startwhile_str);
			if(uses_compound_statement){
				g_statement(second_child(p), code_gen_state, evaluatewhile_str, endwhile_str);
			}else{
				unsigned_char_ptr_list_add(&scope->evaluate_labels, evaluatewhile_str);
				unsigned_char_ptr_list_add(&scope->end_labels, endwhile_str);
				g_statement(second_child(p), code_gen_state, 0, 0);
			}
			buffered_printf(code_gen_state->buffered_output,"%s:\n", evaluatewhile_str);
			g_expression(fifth_child(p), code_gen_state);
			consume_scalar_type(code_gen_state, third_child(p));
			pop(code_gen_state, "r1");
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s; If not true, skip to end\n", endwhile_str);
			buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s; Test condition again\n", startwhile_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", endwhile_str);
			require_internal_symbol(&code_gen_state->symbols, evaluatewhile_str);
			require_internal_symbol(&code_gen_state->symbols, startwhile_str);
			require_internal_symbol(&code_gen_state->symbols, endwhile_str);
			implement_internal_symbol(&code_gen_state->symbols, evaluatewhile_str);
			implement_internal_symbol(&code_gen_state->symbols, startwhile_str);
			implement_internal_symbol(&code_gen_state->symbols, endwhile_str);
			free(startwhile_str);
			if(uses_compound_statement){
				free(evaluatewhile_str);
				free(endwhile_str);
			}else{
				free(unsigned_char_ptr_list_pop(&scope->evaluate_labels));
				free(unsigned_char_ptr_list_pop(&scope->end_labels));
			}
		}else{
			assert(0 &&"Expected do while.\n");
		}
	}else if(check_seven_children(p, TERMINAL, TERMINAL, EXPRESSION_STATEMENT, EXPRESSION_STATEMENT, EXPRESSION, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),FOR) &&
			is_terminal_c_token_type(second_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(sixth_child(p),CLOSE_PAREN_CHAR)
		){
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * startfor_str;
			unsigned char * endfor_str;
			unsigned char * evaluatefor_str;
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(seventh_child(p))->type == COMPOUND_STATEMENT;
			(void)scope;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			sprintf_hook("startfor%d", cond_index);
			startfor_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			sprintf_hook("endfor%d", cond_index);
			endfor_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			sprintf_hook("evaluatefor%d", cond_index);
			evaluatefor_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));

			g_expression_statement(third_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", startfor_str);
			/*  Need to keep the value of the expression so we can inspect it */
			if(fourth_child(p)->first_child->type == EXPRESSION){
				g_expression(fourth_child(p)->first_child, code_gen_state);
				buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Go down to value loaded\n");
				buffered_printf(code_gen_state->buffered_output,"loa r1 SP; Load value\n");
				buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Pop value\n");
				destroy_type_description(pop_type_without_type_check(code_gen_state, p));

				buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s; If not true, skip to end\n", endfor_str);
			}
			if(uses_compound_statement){
				g_statement(seventh_child(p), code_gen_state, evaluatefor_str, endfor_str);
			}else{
				unsigned_char_ptr_list_add(&scope->evaluate_labels, evaluatefor_str);
				unsigned_char_ptr_list_add(&scope->end_labels, endfor_str);
				g_statement(seventh_child(p), code_gen_state, 0, 0);
			}
			buffered_printf(code_gen_state->buffered_output,"%s:\n", evaluatefor_str);
			g_expression(fifth_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Pop value of statement\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Pop value of statement\n");
			destroy_type_description(pop_type(code_gen_state, parser_state->unsigned_int_description, p));
			buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s; Test condition again\n", startfor_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", endfor_str);
			require_internal_symbol(&code_gen_state->symbols, startfor_str);
			require_internal_symbol(&code_gen_state->symbols, endfor_str);
			require_internal_symbol(&code_gen_state->symbols, evaluatefor_str);
			implement_internal_symbol(&code_gen_state->symbols, startfor_str);
			implement_internal_symbol(&code_gen_state->symbols, endfor_str);
			implement_internal_symbol(&code_gen_state->symbols, evaluatefor_str);
			free(startfor_str);
			if(uses_compound_statement){
				free(evaluatefor_str);
				free(endfor_str);
			}else{
				free(unsigned_char_ptr_list_pop(&scope->evaluate_labels));
				free(unsigned_char_ptr_list_pop(&scope->end_labels));
			}
		}else{
			assert(0 &&"Expected for thingy.\n");
		}
	}else if(check_six_children(p, TERMINAL, TERMINAL, EXPRESSION_STATEMENT, EXPRESSION_STATEMENT, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),FOR) &&
			is_terminal_c_token_type(second_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(fifth_child(p),CLOSE_PAREN_CHAR)
		){
			unsigned char * startfor_str;
			unsigned char * endfor_str;
			unsigned int cond_index = code_gen_state->condition_index;
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(sixth_child(p))->type == COMPOUND_STATEMENT;
			(void)scope;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			sprintf_hook("startfor%d", cond_index);
			startfor_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			sprintf_hook("endfor%d", cond_index);
			endfor_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));

			g_expression_statement(third_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", startfor_str);
			/*  Need to keep the value of the expression so we can inspect it */
			if(fourth_child(p)->first_child->type == EXPRESSION){
				g_expression(fourth_child(p)->first_child, code_gen_state);
				buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Go down to value loaded\n");
				buffered_printf(code_gen_state->buffered_output,"loa r1 SP; Load value\n");
				buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Pop value\n");
				consume_scalar_type(code_gen_state, p);
				buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s; If not true, skip to end\n", endfor_str);
			}
			if(uses_compound_statement){
				g_statement(sixth_child(p), code_gen_state, startfor_str, endfor_str);
			}else{
				unsigned_char_ptr_list_add(&scope->evaluate_labels, startfor_str);
				unsigned_char_ptr_list_add(&scope->end_labels, endfor_str);
				g_statement(sixth_child(p), code_gen_state, 0, 0);
			}
			buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s; Test condition again\n", startfor_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", endfor_str);
			require_internal_symbol(&code_gen_state->symbols, startfor_str);
			require_internal_symbol(&code_gen_state->symbols, endfor_str);
			implement_internal_symbol(&code_gen_state->symbols, startfor_str);
			implement_internal_symbol(&code_gen_state->symbols, endfor_str);
			if(uses_compound_statement){
				free(startfor_str);
				free(endfor_str);
			}else{
				free(unsigned_char_ptr_list_pop(&scope->evaluate_labels));
				free(unsigned_char_ptr_list_pop(&scope->end_labels));
			}
		}else{
			assert(0 &&"Expected for thingy.\n");
		}
	}else if(check_five_children(p, TERMINAL, TERMINAL, EXPRESSION, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),WHILE) &&
			is_terminal_c_token_type(second_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_PAREN_CHAR)
		){
			unsigned char * startwhile_str;
			unsigned char * endwhile_str;
			unsigned int cond_index = code_gen_state->condition_index;
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(fifth_child(p))->type == COMPOUND_STATEMENT;
			(void)scope;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			sprintf_hook("endwhile%d", cond_index);
			endwhile_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			sprintf_hook("startwhile%d", cond_index);
			startwhile_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			buffered_printf(code_gen_state->buffered_output,"%s:\n", startwhile_str);
			g_expression(third_child(p), code_gen_state);
			consume_scalar_type(code_gen_state, third_child(p));
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Go down to value loaded\n");
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP; Load value\n");
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Pop value\n");
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s; If not true, skip to end\n", endwhile_str);
			if(uses_compound_statement){
				g_statement(fifth_child(p), code_gen_state, startwhile_str, endwhile_str);
			}else{
				unsigned_char_ptr_list_add(&scope->evaluate_labels, startwhile_str);
				unsigned_char_ptr_list_add(&scope->end_labels, endwhile_str);
				g_statement(fifth_child(p), code_gen_state, 0, 0);
			}
			buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s; Test condition again\n", startwhile_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", endwhile_str);
			require_internal_symbol(&code_gen_state->symbols, startwhile_str);
			require_internal_symbol(&code_gen_state->symbols, endwhile_str);
			implement_internal_symbol(&code_gen_state->symbols, startwhile_str);
			implement_internal_symbol(&code_gen_state->symbols, endwhile_str);
			if(uses_compound_statement){
				free(startwhile_str);
				free(endwhile_str);
			}else{
				free(unsigned_char_ptr_list_pop(&scope->evaluate_labels));
				free(unsigned_char_ptr_list_pop(&scope->end_labels));
			}
		}else{
			assert(0 &&"Expected while thingy.\n");
		}
	}else{
		assert(0 &&"Expected iteration statement.\n");
	}
}

void g_jump_statement(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, EXPRESSION, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),RETURN) &&
			is_terminal_c_token_type(third_child(p),SEMICOLON_CHAR)
		){
			buffered_printf(code_gen_state->buffered_output,";Return this:\n");
			g_expression(second_child(p), code_gen_state);
			return_from_function(code_gen_state, p);
		}else{
			assert(0 &&"Expected return e.\n");
		}
	}else if(check_three_children(p, TERMINAL, TERMINAL, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),GOTO) &&
			is_terminal_c_token_type(second_child(p),IDENTIFIER) &&
			is_terminal_c_token_type(third_child(p),SEMICOLON_CHAR)
		){
			buffered_printf(code_gen_state->buffered_output,";Goto not yet supported\n");
		}else{
			assert(0 &&"Expected goto statement.\n");
		}
	}else if(check_two_children(p, TERMINAL, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),CONTINUE) &&
			is_terminal_c_token_type(second_child(p),SEMICOLON_CHAR)
		){
			struct scope_level * current_scope;
			for(current_scope = get_current_scope_level(code_gen_state); current_scope != 0; current_scope = current_scope->parent_scope){
				if(unsigned_char_ptr_list_size(&current_scope->evaluate_labels)){
					unsigned char * label = unsigned_char_ptr_list_get(&current_scope->evaluate_labels, unsigned_char_ptr_list_size(&current_scope->evaluate_labels) -1);
					clear_locals_from_scope(code_gen_state, current_scope);
					buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;  For continue statement\n", label);
					break;
				}else{
					/*  This is likely an if statement (or switch), clear the locals from stack before branch */
					clear_locals_from_scope(code_gen_state, current_scope);
				}
			}
		}else if(
			is_terminal_c_token_type(first_child(p),BREAK) &&
			is_terminal_c_token_type(second_child(p),SEMICOLON_CHAR)
		){
			struct scope_level * current_scope;
			for(current_scope = get_current_scope_level(code_gen_state); current_scope != 0; current_scope = current_scope->parent_scope){
				if(unsigned_char_ptr_list_size(&current_scope->end_labels)){
					unsigned char * label = unsigned_char_ptr_list_get(&current_scope->end_labels, unsigned_char_ptr_list_size(&current_scope->end_labels) -1);
					clear_locals_from_scope(code_gen_state, current_scope);
					buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;  For break statement\n", label);
					break;
				}else{
					/*  This is likely an if statement clear the locals from stack before branch */
					clear_locals_from_scope(code_gen_state, current_scope);
				}
			}
		}else if(
			is_terminal_c_token_type(first_child(p),RETURN) &&
			is_terminal_c_token_type(second_child(p),SEMICOLON_CHAR)
		){
			create_default_return_value(code_gen_state, 0, first_child(p));
			buffered_printf(code_gen_state->buffered_output,"Return statement with no expr:\n");
		}else{
			assert(0 &&"Expected jump statement.\n");
		}
	}else{
		assert(0 &&"Expected iteration statement.\n");
	}
}


void g_function_definition(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, DECLARATION_SPECIFIERS, DECLARATOR, DECLARATION_LIST, COMPOUND_STATEMENT)){
		g_declaration_specifiers(first_child(p), code_gen_state);
		/*g_declarator(second_child(p), code_gen_state);*/
		/*g_declaration_list(third_child(p), code_gen_state);*/
		g_compound_statement(fourth_child(p), code_gen_state, p, 0, 0);
	}else if(check_three_children(p, DECLARATION_SPECIFIERS, DECLARATOR, COMPOUND_STATEMENT)){
		struct parser_node * identifier = get_identifier_from_declarator(p->first_child->next);
		unsigned char * identifier_str = copy_string(identifier->c_lexer_token->first_byte, identifier->c_lexer_token->last_byte);
		implement_external_symbol(&code_gen_state->symbols, identifier_str);
		buffered_printf(code_gen_state->buffered_output,"%s:\n", identifier_str);
		free(identifier_str);
		g_declaration_specifiers(first_child(p), code_gen_state);
		/*g_declarator(second_child(p), code_gen_state);*/
		g_compound_statement(third_child(p), code_gen_state, p, 0, 0);
	}else if(check_three_children(p, DECLARATOR, DECLARATION_LIST, COMPOUND_STATEMENT)){
		/*g_declarator(first_child(p), code_gen_state);*/
		/*g_declaration_list(second_child(p), code_gen_state);*/
		g_compound_statement(third_child(p), code_gen_state, p, 0, 0);
	}else if(check_two_children(p, DECLARATOR, COMPOUND_STATEMENT)){
		/*g_declarator(first_child(p), code_gen_state);*/
		g_compound_statement(second_child(p), code_gen_state, p, 0, 0);
	}else{
		assert(0 &&"Expected function definition.\n");
	}
}


void g_external_declaration(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_one_child(p, FUNCTION_DEFINITION)){
		g_function_definition(first_child(p), code_gen_state);
	}else if(check_one_child(p, DECLARATION)){
		g_declaration(first_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected function declaration or declaration.\n");
	}
}

void g_translation_unit_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, EXTERNAL_DECLARATION, TRANSLATION_UNIT_REST)){
		g_external_declaration(first_child(p), code_gen_state);
		g_translation_unit_rest(second_child(p), code_gen_state);
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected function declaration or declaration.\n");
	}
}

void g_translation_unit(struct parser_node * p, struct code_gen_state * code_gen_state){
	struct parser_state * parser_state = code_gen_state->parser_state;
	if(check_two_children(p, EXTERNAL_DECLARATION, TRANSLATION_UNIT_REST)){
		unsigned int i;
		unsigned int total_chars = 0;
		/*  Output string literals. */
		for(i = 0; i < char_ptr_list_size(&parser_state->string_literals); i++){
			char * c = char_ptr_list_get(&parser_state->string_literals, i);
			unsigned int len = (unsigned int)strlen(c);
			unsigned int j;
			unsigned char * string_literal_identifier_str;
			sprintf_hook("stringliteral%d", i);
			string_literal_identifier_str = copy_string(get_sprintf_buffer(), get_null_terminator(get_sprintf_buffer()));
			buffered_printf(code_gen_state->buffered_output,"%s:\n", string_literal_identifier_str);
			for(j = 0; j < (len + 1); j++){
				buffered_printf(code_gen_state->buffered_output,"dw 0x%X;\n", 0); /* TODO: these addresses are wrong.  Character constants shouldn't be modified anyway, but might implement ability to modify rvalues */
				if(c[j] == '\n'){
					buffered_printf(code_gen_state->buffered_output,"dw 0x%X; \\n\n", c[j]);
				}else if(c[j] == '\0'){
					buffered_printf(code_gen_state->buffered_output,"dw 0x%X; \\0\n", c[j]);
				}else{
					buffered_printf(code_gen_state->buffered_output,"dw 0x%X; %c\n", c[j], c[j]);
				}
				total_chars++;
			}
			implement_internal_symbol(&code_gen_state->symbols, string_literal_identifier_str);
			free(string_literal_identifier_str);
		}

		g_external_declaration(first_child(p), code_gen_state);
		g_translation_unit_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected function declaration or declaration.\n");
	}
}



struct linker_symbol * make_linker_symbol(unsigned int is_impl, unsigned int is_req, unsigned int is_ext, unsigned int offset){
	struct linker_symbol * new_symbol = malloc(sizeof(struct linker_symbol));
	new_symbol->is_implemented = is_impl;
	new_symbol->is_required = is_req;
	new_symbol->is_external = is_ext;
	new_symbol->instruction_index = offset;
	return new_symbol;
}

void require_internal_symbol(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map, unsigned char * c){
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, c);
	if(existing_symbol){
		assert(!existing_symbol->is_external);
		existing_symbol->is_required = 1;
	}else{
		unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(map, copy_string(c, get_null_terminator(c)), make_linker_symbol(0, 1, 0, 0));
	}
}

void implement_internal_symbol(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map, unsigned char * c){
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, c);
	if(existing_symbol){
		assert(!existing_symbol->is_external);
		existing_symbol->is_implemented = 1;
	}else{
		unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(map, copy_string(c, get_null_terminator(c)), make_linker_symbol(1, 0, 0, 0));
	}
}

void implement_external_symbol(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map, unsigned char * c){
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, c);
	if(existing_symbol){
		assert(existing_symbol->is_external);
		existing_symbol->is_implemented = 1;
	}else{
		unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(map, copy_string(c, get_null_terminator(c)), make_linker_symbol(1, 0, 1, 0));
	}
}

void require_external_symbol(struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map, unsigned char * c){
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, c);
	if(existing_symbol){
		assert(existing_symbol->is_external);
		existing_symbol->is_required = 1;
	}else{
		unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(map, copy_string(c, get_null_terminator(c)), make_linker_symbol(0, 1, 1, 0));
	}
}

int generate_code(struct parser_state * parser_state, struct code_gen_state * code_gen_state){
	code_gen_state->parser_state = parser_state;
	code_gen_state->current_function = 0;
	code_gen_state->condition_index = 0;
	code_gen_state->global_var_ptr_index = 0;
	struct_switch_frame_ptr_list_create(&code_gen_state->switch_frames);
	unsigned_int_list_create(&code_gen_state->scope_index_list);
	code_gen_state->next_scope_index = 0;
	struct_type_description_ptr_list_create(&code_gen_state->type_stack);
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_create(&code_gen_state->symbols, unsigned_strcmp);

	if(parser_state->top_node){
		buffered_printf(code_gen_state->buffered_output,"OFFSET RELOCATABLE;\n");
		if(parser_state->top_node->type == TRANSLATION_UNIT){
			g_translation_unit(parser_state->top_node, code_gen_state);
		}else{
			assert(0 && "Top node was not expected node.\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,";There is no code to generate: Top node was null.\n");
	}
	return 0;
}

int destroy_code_gen_state(struct code_gen_state * code_gen_state){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&code_gen_state->symbols);
	unsigned int size = unsigned_char_ptr_list_size(&keys);
	unsigned int i;
	for(i = 0; i < size; i++){
		free(unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&code_gen_state->symbols, unsigned_char_ptr_list_get(&keys, i)));
	}
	/*  Need to do this twice because we're deleting the data under the keys of the map */
	for(i = 0; i < size; i++){
		free(unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_destroy(&code_gen_state->symbols);
	unsigned_int_list_destroy(&code_gen_state->scope_index_list);
	struct_type_description_ptr_list_destroy(&code_gen_state->type_stack);
	unsigned_char_ptr_list_destroy(&keys);
	struct_switch_frame_ptr_list_destroy(&code_gen_state->switch_frames);
	return 0;
}


int do_code_generation(struct memory_pooler_collection * memory_pooler_collection, unsigned char * in_file, unsigned char * out_file){
	int rtn; 
	unsigned int i; 
	struct unsigned_char_list preprocessed_input;
	struct unsigned_char_list generated_code;
	struct unsigned_char_list buffered_symbol_table;
	struct unsigned_char_list lexer_output;
	struct parser_state parser_state;
	struct code_gen_state code_gen_state;
	struct c_lexer_state c_lexer_state;

	g_format_buffer_use();

	parser_state.memory_pooler_collection = memory_pooler_collection; 
	parser_state.buffered_output = &generated_code; 

	code_gen_state.buffered_output = &generated_code; 
	code_gen_state.buffered_symbol_table = &buffered_symbol_table; 

	c_lexer_state.c.memory_pooler_collection = memory_pooler_collection; 
	c_lexer_state.c.buffered_output = &lexer_output;


	unsigned_char_list_create(&lexer_output);
	unsigned_char_list_create(&preprocessed_input);
	unsigned_char_list_create(&generated_code);
	unsigned_char_list_create(&buffered_symbol_table);

	add_file_to_buffer(&preprocessed_input, (char*)in_file);

	/*  Use the lexer to generate tokens from 'pure c' code (no preprocessor directives) */
	rtn = lex_c(&c_lexer_state, in_file, unsigned_char_list_data(&preprocessed_input), unsigned_char_list_size(&preprocessed_input));
	for(i = 0; i < unsigned_char_list_size(&lexer_output); i++){
		printf("%c", unsigned_char_list_get(&lexer_output, i));
	}

	/*  Parse the tokens the are output fromt the preprocessor */
	if(rtn){
		printf("Lexical analysis failed during code generation of %s\n", in_file);
	}else{
		if(parse(&c_lexer_state, &parser_state, unsigned_char_list_data(&preprocessed_input))){
			printf("Parsing failed during code generation of %s\n", in_file);
		}else{
			if(generate_code(&parser_state, &code_gen_state)){
				printf("Code generation failed for %s\n", in_file);
			}else{
				unsigned char * data;
				struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&code_gen_state.symbols);
				unsigned int size = unsigned_char_ptr_list_size(&keys);
				for(i = 0; i < size; i++){
					unsigned char * key = unsigned_char_ptr_list_get(&keys, i);
					struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&code_gen_state.symbols, key);
					if(symbol->is_required && symbol->is_implemented){
						buffered_printf(code_gen_state.buffered_symbol_table, "REQUIRES, IMPLEMENTS");
					}else if(symbol->is_required){
						buffered_printf(code_gen_state.buffered_symbol_table, "REQUIRES");
					}else if(symbol->is_implemented){
						buffered_printf(code_gen_state.buffered_symbol_table, "IMPLEMENTS");
					}else{
						assert(0 && "Symbol is not required or implemented?");
					}
					if(symbol->is_external){
						buffered_printf(code_gen_state.buffered_symbol_table, " EXTERNAL ");
					}else{
						buffered_printf(code_gen_state.buffered_symbol_table, " INTERNAL ");
					}
					buffered_printf(code_gen_state.buffered_symbol_table, "%s\n", key);
				}
				unsigned_char_ptr_list_destroy(&keys);

				size = unsigned_char_list_size(&generated_code);
				data = unsigned_char_list_data(&generated_code);

				for(i = 0; i < size; i++){
					unsigned_char_list_add(&buffered_symbol_table, data[i]);
				}

				output_buffer_to_file(&buffered_symbol_table, (char *)out_file);
			}
			destroy_code_gen_state(&code_gen_state);
		}
		destroy_parser_state(&parser_state);
	}
	destroy_c_lexer_state(&c_lexer_state);
	unsigned_char_list_destroy(&buffered_symbol_table);
	unsigned_char_list_destroy(&lexer_output);
	unsigned_char_list_destroy(&preprocessed_input);
	unsigned_char_list_destroy(&generated_code);
	g_format_buffer_release();
	return rtn;
}


