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

#include "code_generator.h"

void g_expression(struct parser_node *, struct code_gen_state *);
void g_expression_rest(struct parser_node *, struct code_gen_state *);
void g_declarator(struct parser_node *, struct code_gen_state *);
void g_initializer(struct parser_node *, struct code_gen_state *);
void g_struct_or_union_specifier(struct parser_node *, struct code_gen_state *);
void g_abstract_declarator(struct parser_node *, struct code_gen_state *);
void g_declaration_list(struct parser_node *, struct code_gen_state *);
void g_init_declarator(struct parser_node *, struct parser_node *, struct code_gen_state *);
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
unsigned int g_argument_expression_list_rest(struct parser_node *, struct code_gen_state *, struct parser_node *, unsigned int, unsigned int);
unsigned int g_argument_expression_list(struct parser_node *, struct code_gen_state *, struct parser_node *, unsigned int, unsigned int);
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
void g_init_declarator_list_rest(struct parser_node *, struct parser_node *, struct code_gen_state *);
void g_init_declarator_list(struct parser_node *, struct parser_node *, struct code_gen_state *);
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


void destroy_constant_initializer_level(struct constant_initializer_level * level){
	unsigned int i;
	for(i = 0; i < struct_constant_initializer_level_ptr_list_size(&level->children); i++){
		destroy_constant_initializer_level(struct_constant_initializer_level_ptr_list_get(&level->children, i));
	}
	struct_constant_initializer_level_ptr_list_destroy(&level->children);
	free(level);
}

unsigned int evaluate_compile_time_constant(struct code_gen_state * code_gen_state, struct compile_time_constant * constant){
	unsigned int rtn;
	(void)code_gen_state;
	if(constant->constant_description){
		rtn = constant->constant_description->native_data[0];
	}else if(constant->element){
		rtn = get_enum_value(code_gen_state->memory_pool_collection, constant->element);
	}else{
		assert(0 && "Error.");
	}
	free(constant); /* TODO: put this somewhere better */
	return rtn;
}

struct compile_time_constant * evaluate_constant_sizeof_type_name(struct code_gen_state * code_gen_state, struct parser_node * n){
	struct type_description * sized_type;
	struct type_description * result_type;
	unsigned int size;
	struct compile_time_constant * rtn;
	sized_type = create_type_description_from_type_name(code_gen_state->memory_pool_collection, code_gen_state->parser_state, n);
	convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, sized_type);
	result_type = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, create_empty_type_description(code_gen_state->memory_pool_collection), UNSIGNED), INT);
	size = type_size(code_gen_state, sized_type, MINIMAL_RVALUE, 0, get_current_scope_level(code_gen_state));
	rtn = (struct compile_time_constant *)malloc(sizeof(struct compile_time_constant));
	rtn->element = (struct normalized_declaration_element *)0;
	rtn->constant_description = (struct constant_description*)malloc(sizeof(struct constant_description));
	rtn->constant_description->native_data = (unsigned int *)malloc(sizeof(unsigned int));
	*((unsigned int *)rtn->constant_description->native_data) = 0;
	rtn->constant_description->type = SIZEOF;
	*(rtn->constant_description->native_data) = size;
	rtn->constant_description->type_description = result_type;
	rtn->constant_description->type_description->value_type = WORD_ALIGNED_RVALUE;
	rtn->constant_description->type_description->source_scope_level = get_current_scope_level(code_gen_state);
	destroy_type_description(code_gen_state->memory_pool_collection, sized_type);
	return rtn;
}

struct compile_time_constant * evaluate_constant_literal(struct code_gen_state * code_gen_state, struct parser_node * n){
	struct compile_time_constant * rtn;
	struct constant_description * constant_description;
	unsigned char * str;
	assert(n);
	rtn = (struct compile_time_constant *)malloc(sizeof(struct compile_time_constant));
	rtn->constant_description = (struct constant_description *)0;
	rtn->element = (struct normalized_declaration_element *)0;
	str = copy_string(n->c_lexer_token->first_byte, n->c_lexer_token->last_byte, code_gen_state->memory_pool_collection);
	constant_description = find_constant(code_gen_state->parser_state, str);
	assert(constant_description);
	heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, str);
	rtn->constant_description = constant_description;
	return rtn;
}

struct compile_time_constant * evaluate_constant_identifier(struct code_gen_state * code_gen_state, struct parser_node * n){
	struct compile_time_constant * rtn;
	unsigned char * identifier = copy_string(n->c_lexer_token->first_byte, n->c_lexer_token->last_byte, code_gen_state->memory_pool_collection);
	struct namespace_object * obj;
	struct normalized_declaration_element * element;
	rtn = (struct compile_time_constant *)malloc(sizeof(struct compile_time_constant));
	rtn->constant_description = (struct constant_description *)0;
	rtn->element = (struct normalized_declaration_element *)0;

	obj = get_namespace_object_from_closest_namespace(identifier, IDENTIFIER_NAMESPACE, get_current_scope_level(code_gen_state), 0, code_gen_state->memory_pool_collection);
	assert(obj);
	element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, 0);
	if(element->normalized_declarator && element->normalized_declarator->type == NORMALIZED_ENUMERATOR){
		rtn->element = element;
	}else{
		printf("Attempting to determine compile-time constant value of non enum identifier '%s'.\n", identifier);
		assert(0 && "Attempting to determine compile-time constant value of non enum identifier.");
	}
	heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, identifier);
	return rtn;
}

struct compile_time_constant * evaluate_constant_constant_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n && n->type == CONSTANT_EXPRESSION);
	return evaluate_constant_conditional_expression(code_gen_state, n->first_child);
}

struct compile_time_constant * evaluate_constant_primary_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == PRIMARY_EXPRESSION);
	if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == STRING_LITERAL){
		return evaluate_constant_literal(code_gen_state, n->first_child);
	}else if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == CONSTANT_DECIMAL){
		return evaluate_constant_literal(code_gen_state, n->first_child);
	}else if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == CONSTANT_CHARACTER){
		return evaluate_constant_literal(code_gen_state, n->first_child);
	}else if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == CONSTANT_HEX){
		return evaluate_constant_literal(code_gen_state, n->first_child);
	}else if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == IDENTIFIER){
		return evaluate_constant_identifier(code_gen_state, n->first_child);
	}else{
		assert(0);
	}
}

struct compile_time_constant * evaluate_constant_postfix_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == POSTFIX_EXPRESSION);
	if(n->first_child && n->first_child->type == PRIMARY_EXPRESSION){
		return evaluate_constant_primary_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_postfix_expression");
	}
}

struct compile_time_constant * evaluate_constant_unary_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == UNARY_EXPRESSION);
	if(n->first_child && n->first_child->type == POSTFIX_EXPRESSION){
		return evaluate_constant_postfix_expression(code_gen_state, n->first_child);
	}else if(n->first_child && n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == SIZEOF){
		return evaluate_constant_sizeof_type_name(code_gen_state, n->first_child->next->next);
	}else{
		assert(0 && "Not currently supporting constant_unary_expression");
	}
}

struct compile_time_constant * evaluate_constant_cast_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == CAST_EXPRESSION);
	if(n->first_child && n->first_child->type == UNARY_EXPRESSION){
		return evaluate_constant_unary_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_unary_expression");
	}
}

struct compile_time_constant * evaluate_constant_multiplicative_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == MULTIPLICATIVE_EXPRESSION);
	if(n->first_child && n->first_child->type == CAST_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_cast_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_multiplicative_expression");
	}
}

struct compile_time_constant * evaluate_constant_additive_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == ADDITIVE_EXPRESSION);
	if(n->first_child && n->first_child->type == MULTIPLICATIVE_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_multiplicative_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_additive_expression");
	}
}

struct compile_time_constant * evaluate_constant_shift_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == SHIFT_EXPRESSION);
	if(n->first_child && n->first_child->type == ADDITIVE_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_additive_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_shift_expression");
	}
}

struct compile_time_constant * evaluate_constant_relational_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == RELATIONAL_EXPRESSION);
	if(n->first_child && n->first_child->type == SHIFT_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_shift_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_relational_expression");
	}
}

struct compile_time_constant * evaluate_constant_equality_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == EQUALITY_EXPRESSION);
	if(n->first_child && n->first_child->type == RELATIONAL_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_relational_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_relational_expression");
	}
}

struct compile_time_constant * evaluate_constant_and_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == AND_EXPRESSION);
	if(n->first_child && n->first_child->type == EQUALITY_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_equality_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_and_expression");
	}
}

struct compile_time_constant * evaluate_constant_exclusive_or_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == EXCLUSIVE_OR_EXPRESSION);
	if(n->first_child && n->first_child->type == AND_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_and_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_exclusive_or_expression");
	}
}

struct compile_time_constant * evaluate_constant_inclusive_or_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == INCLUSIVE_OR_EXPRESSION);
	if(n->first_child && n->first_child->type == EXCLUSIVE_OR_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_exclusive_or_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_inclusive_or_expression");
	}
}

struct compile_time_constant * evaluate_constant_logical_and_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == LOGICAL_AND_EXPRESSION);
	if(n->first_child && n->first_child->type == INCLUSIVE_OR_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_inclusive_or_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_logical_and_expression");
	}
}

struct compile_time_constant * evaluate_constant_logical_or_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == LOGICAL_OR_EXPRESSION);
	if(n->first_child && n->first_child->type == LOGICAL_AND_EXPRESSION){
		assert(n->first_child->next->first_child->type == EPSILON);
		return evaluate_constant_logical_and_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant_logical_or_expression.");
	}
}

struct compile_time_constant * evaluate_constant_conditional_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == CONDITIONAL_EXPRESSION);
	if(n->first_child && n->first_child->type == LOGICAL_OR_EXPRESSION){
		return evaluate_constant_logical_or_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting constant conditional expressions.");
	}
}

struct compile_time_constant * evaluate_constant_assignment_expression(struct code_gen_state * code_gen_state, struct parser_node * n){
	assert(n->type == ASSIGNMENT_EXPRESSION);
	if(n->first_child && n->first_child->type == CONDITIONAL_EXPRESSION){
		return evaluate_constant_conditional_expression(code_gen_state, n->first_child);
	}else{
		assert(0 && "Not currently supporting assignment expressions in initializers.");
		return 0;
	}
}

void evaluate_constant_initializer_list(struct code_gen_state * code_gen_state, struct constant_initializer_level * parent_level, struct parser_node * n){
	struct parser_node * initializer = (struct parser_node *)0;
	assert((n && n->type == INITIALIZER_LIST) || (n && n->type == INITIALIZER_LIST_REST));
	if(!n){
		assert(0 && "Should not be passed null.");
	}
	
	if(n->first_child && n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == COMMA_CHAR){
		initializer = n->first_child->next;
	}else if(n->first_child && n->first_child->type == INITIALIZER){
		initializer = n->first_child;
	}else if(n->first_child && n->first_child->type == EPSILON){
		assert(0 && "Should not pass in epsilon.");
	}else{
		assert(0 && "Unknown case.");
	}

	struct_constant_initializer_level_ptr_list_add_end(&parent_level->children, evaluate_constant_initializer(code_gen_state, initializer));
	if(initializer->next->first_child->type != EPSILON){
		evaluate_constant_initializer_list(code_gen_state, parent_level, initializer->next);
	}
}

struct constant_initializer_level * evaluate_constant_initializer(struct code_gen_state * code_gen_state, struct parser_node * n){
	struct constant_initializer_level * level = (struct constant_initializer_level *)malloc(sizeof(struct constant_initializer_level));
	struct_constant_initializer_level_ptr_list_create(&level->children);
	assert(n && n->type == INITIALIZER);
	if(n->first_child && n->first_child->type == ASSIGNMENT_EXPRESSION){
		level->constant = evaluate_constant_assignment_expression(code_gen_state, n->first_child);
	}else if(n->first_child && n->first_child->next && n->first_child->next->type == INITIALIZER_LIST){
		level->constant = (struct compile_time_constant *)0;
		evaluate_constant_initializer_list(code_gen_state, level, n->first_child->next);
	}else{
		assert(0 && "Unsupported initializer.");
		return 0;
	}
	return level;
}

void perform_integral_promotion(struct code_gen_state * code_gen_state, struct type_description ** t, const char * reg){
	enum type_class c = determine_type_class(code_gen_state->memory_pool_collection, *t);
	switch(c){
		case TYPE_CLASS_CHAR:{
			if(is_signed(*t)){
				/*  If a negative signed character is promoted to an int, we need to sign extend */
				buffered_printf(code_gen_state->buffered_output, "loa r5 %s;          Copy char\n", reg); 
				buffered_printf(code_gen_state->buffered_output, "ll r6 0x80;          Sign bit\n"); 
				buffered_printf(code_gen_state->buffered_output, "and r7 r6 r5;          Is it signed?\n"); 
				buffered_printf(code_gen_state->buffered_output, "beq r7 ZR 8;         If not signed skip\n"); 
				buffered_printf(code_gen_state->buffered_output, "ll r8 0xFFFF;         High bits to set\n"); 
				buffered_printf(code_gen_state->buffered_output, "shl r8 WR;         shift to 0xFFFF0\n"); 
				buffered_printf(code_gen_state->buffered_output, "shl r8 WR;         shift to 0xFFFF00\n"); 
				buffered_printf(code_gen_state->buffered_output, "or r5 r5 r8;         Set sign bits\n"); 
				buffered_printf(code_gen_state->buffered_output, "shl r8 WR;         shift to 0xFFFF000\n"); 
				buffered_printf(code_gen_state->buffered_output, "shl r8 WR;         shift to 0xFFFF0000\n"); 
				buffered_printf(code_gen_state->buffered_output, "or r5 r5 r8;         Set more sign bits\n"); 
				buffered_printf(code_gen_state->buffered_output, "sto %s r5;         Store\n", reg); 
			}
			remove_specifier(code_gen_state->memory_pool_collection, *t, 0, CHAR);
			add_specifier(code_gen_state->memory_pool_collection, *t, INT);
			break;
		}case TYPE_CLASS_SHORT:{
			remove_specifier(code_gen_state->memory_pool_collection, *t, 0, SHORT);
			if(!count_specifiers(*t,INT)){
				add_specifier(code_gen_state->memory_pool_collection, *t, INT);
			}
			break;
		}case TYPE_CLASS_ENUM:{
			remove_enum(code_gen_state->memory_pool_collection, *t);
			add_specifier(code_gen_state->memory_pool_collection, *t, INT);
			break;
		}case TYPE_CLASS_POINTER:{
			/*  Replace with unsigned int */
			struct type_description * old_t = *t;
			*t = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, create_empty_type_description(code_gen_state->memory_pool_collection), UNSIGNED), INT);
			(*t)->value_type = old_t->value_type;
			destroy_type_description(code_gen_state->memory_pool_collection, old_t);
			break;
		}default:{
			/* No need to do anything */
			break;
		}
	}
}

void perform_integral_promotions(struct code_gen_state * code_gen_state, struct type_description ** t1, struct type_description ** t2){
	unsigned int current_size_top = type_size(code_gen_state, *t1, (*t1)->value_type, 1, (*t1)->source_scope_level);

	perform_integral_promotion(code_gen_state, t1, "SP");
	buffered_printf(code_gen_state->buffered_output, "ll r1 0x%X;          Amount down to value\n", current_size_top); 
	buffered_printf(code_gen_state->buffered_output, "add r2 SP r1;          Ptr to bottom value\n", current_size_top); 
	perform_integral_promotion(code_gen_state, t2, "r2");
}

void ensure_top_values_are_rvalues(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * top_type = pop_type_without_type_check(code_gen_state, context);
	struct type_description * bottom_type = pop_type_without_type_check(code_gen_state, context);
	enum value_type target_value_type_top = WORD_ALIGNED_RVALUE;
	enum value_type target_value_type_bottom = WORD_ALIGNED_RVALUE;
	unsigned int current_size_top = type_size(code_gen_state, top_type, top_type->value_type, 1, top_type->source_scope_level);
	unsigned int current_size_bottom = type_size(code_gen_state, bottom_type, bottom_type->value_type, 1, bottom_type->source_scope_level);
	unsigned int size_as_minimal_top = type_size(code_gen_state, top_type, MINIMAL_RVALUE, 1, top_type->source_scope_level);
	unsigned int size_as_minimal_bottom = type_size(code_gen_state, bottom_type, MINIMAL_RVALUE, 1, bottom_type->source_scope_level);
	unsigned int target_size_top = type_size(code_gen_state, top_type, target_value_type_top, 1, top_type->source_scope_level);
	unsigned int target_size_bottom = type_size(code_gen_state, bottom_type, target_value_type_bottom, 1, bottom_type->source_scope_level);

	unsigned int total_current = current_size_top + current_size_bottom;
	unsigned int total_target = target_size_top + target_size_bottom;
	struct type_traversal * type_traversal;

	(void)type_traversal;

	if(top_type->value_type == WORD_ALIGNED_RVALUE && bottom_type->value_type == WORD_ALIGNED_RVALUE){
		/*  Nothing to do */
	}else if(top_type->value_type == LVALUE && bottom_type->value_type == LVALUE){
		buffered_printf(code_gen_state->buffered_output, "add r1 SP ZR;        Make a copy of stack pointer\n");
		buffered_printf(code_gen_state->buffered_output, "ll r2 0x%X;          Amount to push on SP\n", total_target - total_current); 
		buffered_printf(code_gen_state->buffered_output, "sub SP SP r2;        Push SP to final resting place\n");
		buffered_printf(code_gen_state->buffered_output, "loa r3 r1;           Load top lvalue into r3\n");
		buffered_printf(code_gen_state->buffered_output, "add r1 r1 WR;        Go down to next lvalue\n");
		buffered_printf(code_gen_state->buffered_output, "loa r4 r1;           Load bottom lvalue into r4\n");
		buffered_printf(code_gen_state->buffered_output, "ll r2 0x%X;          For bottom rvalue\n", target_size_bottom - 4); 
		if(size_as_minimal_top == 1){
			do_character_rvalue_conversion(code_gen_state, "r4", "r1", "r6", "r7", "r8");
		}else{
			copy_lvalue_into_rvalue(code_gen_state, "r4", "r1", "r2", "r5", target_size_bottom);
		}
		buffered_printf(code_gen_state->buffered_output, "sub r1 r1 WR;        Go up to where we copy the next rvalue\n");
		buffered_printf(code_gen_state->buffered_output, "ll r2 0x%X;          For top rvalue\n", target_size_top - 4); 
		if(size_as_minimal_bottom == 1){
			do_character_rvalue_conversion(code_gen_state, "r3", "r1", "r6", "r7", "r8");
		}else{
			copy_lvalue_into_rvalue(code_gen_state, "r3", "r1", "r2", "r5", target_size_top);
		}
	}else if(top_type->value_type == WORD_ALIGNED_RVALUE && bottom_type->value_type == LVALUE){
		buffered_printf(code_gen_state->buffered_output, "add r1 SP ZR;        Make a copy of top of stack\n");
		/*  The difference needed to add when lvalue on bottom is expanded. */
		buffered_printf(code_gen_state->buffered_output, "ll r2 0x%X;          Amount to push on SP\n", target_size_bottom - current_size_bottom);
		buffered_printf(code_gen_state->buffered_output, "sub SP SP r2;        Push SP to final resting place\n");
		buffered_printf(code_gen_state->buffered_output, "ll r3 0x%X;          Amount down to last word of top rvalue\n", target_size_top - 4);
		buffered_printf(code_gen_state->buffered_output, "add r4 r1 ZR;        Copy of where top of rvalue is (the lvalue of the top)\n");
		buffered_printf(code_gen_state->buffered_output, "add r5 SP r3;        Bottom word of resting place of top rvalue\n");
		copy_lvalue_into_rvalue(code_gen_state, "r4", "r5", "r3", "r6", target_size_top);
		buffered_printf(code_gen_state->buffered_output, "ll r2 0x%X;          Amount down from SP to lvalue\n", total_target - 4);
		buffered_printf(code_gen_state->buffered_output, "add r1 SP r2;        r1 points to lvalue\n");
		buffered_printf(code_gen_state->buffered_output, "loa r2 r1;           r2 contains lvalue\n");
		buffered_printf(code_gen_state->buffered_output, "ll r3 0x%X;          rvalue size -4\n", target_size_bottom - 4);
		if(size_as_minimal_bottom == 1){
			do_character_rvalue_conversion(code_gen_state, "r2", "r1", "r6", "r7", "r8");
		}else{
			copy_lvalue_into_rvalue(code_gen_state, "r2", "r1", "r3", "r6", target_size_bottom);
		}
	}else if(top_type->value_type == LVALUE && bottom_type->value_type == WORD_ALIGNED_RVALUE){
		buffered_printf(code_gen_state->buffered_output, "add r1 SP ZR;        Make a copy of top of stack\n");
		buffered_printf(code_gen_state->buffered_output, "ll r2 0x%X;          Amount to push on SP\n", target_size_top - current_size_top); 
		buffered_printf(code_gen_state->buffered_output, "sub SP SP r2;        Push SP to final resting place\n");
		buffered_printf(code_gen_state->buffered_output, "loa r3 r1;           Load the lvalue into r3\n"); 
		buffered_printf(code_gen_state->buffered_output, "ll r2 0x%X;          For copy\n", target_size_top - 4); 
		if(size_as_minimal_top == 1){
			do_character_rvalue_conversion(code_gen_state, "r3", "r1", "r6", "r7", "r8");
		}else{
			copy_lvalue_into_rvalue(code_gen_state, "r3", "r1", "r2", "r5", target_size_top);
		}
	}else{
		assert(0);
	}

	decay_to_pointer_if_array(code_gen_state, &top_type);
	decay_to_pointer_if_array(code_gen_state, &bottom_type);

	top_type->value_type = target_value_type_top;
	bottom_type->value_type = target_value_type_bottom;

	push_type(code_gen_state, bottom_type, context);
	push_type(code_gen_state, top_type, context);
}

struct type_description * usual_arithmetic_conversion(struct code_gen_state * code_gen_state, struct parser_node * context){
	/* Implements Usual arithmetic conversions */
	struct parser_state * parser_state = code_gen_state->parser_state;
	struct type_description * t1 = pop_type_without_type_check(code_gen_state, context);
	struct type_description * t2 = pop_type_without_type_check(code_gen_state, context);

	enum type_class c1;
	enum type_class c2;

	perform_integral_promotions(code_gen_state, &t1, &t2);

	/*  These may have changed after the integer type promotion */
	c1 = determine_type_class(code_gen_state->memory_pool_collection, t1);
	c2 = determine_type_class(code_gen_state->memory_pool_collection, t2);

	if(is_signed(t1) == is_signed(t2) && c1 == c2){
		destroy_type_description(code_gen_state->memory_pool_collection, t1);
		return t2;
	}

	if((is_signed(t1) && !is_signed(t2)) || (is_signed(t2) && !is_signed(t1))){
		if(c1 == TYPE_CLASS_INT && c2 == TYPE_CLASS_INT){
			struct type_description * rtn;
			rtn = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, create_empty_type_description(code_gen_state->memory_pool_collection), UNSIGNED), INT);
			rtn->value_type = WORD_ALIGNED_RVALUE;
			rtn->context = t1->context;
			rtn->source_element = t1->source_element;
			rtn->source_scope_level = t1->source_scope_level;
			destroy_type_description(code_gen_state->memory_pool_collection, t1);
			destroy_type_description(code_gen_state->memory_pool_collection, t2);
			return rtn;
		}else{
			print_error_with_types(parser_state->c_lexer_state, t1, t2, context, "Here are the two types we're doing usual arithmetic conversion on:");
			assert(0 && "This case is not supported.\n");
			return (struct type_description *)0;
		}
	}else{
		print_error_with_types(parser_state->c_lexer_state, t1, t2, context, "Here are the two types we're doing usual arithmetic conversion on:");
		assert(0 && "This case is not supported.\n");
		return (struct type_description *)0;
	}
}

void push_type(struct code_gen_state * code_gen_state, struct type_description * type, struct parser_node * context){
	(void)context;
	assert(type);
	struct_type_description_ptr_list_add_end(&code_gen_state->type_stack, type);
}

struct type_description * pop_type_without_type_check(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * found_type = struct_type_description_ptr_list_pop_end(&code_gen_state->type_stack);
	(void)context;
	return found_type;
}

struct type_description * pop_type(struct code_gen_state * code_gen_state, struct type_description * required_type, struct parser_node * context){
	struct type_description * found_type = struct_type_description_ptr_list_pop_end(&code_gen_state->type_stack);
	unsigned int compare_successful = 0;
	assert(required_type);
	assert(found_type);
	compare_successful = !type_description_cmp(code_gen_state->memory_pool_collection, required_type, found_type);
	if(!compare_successful){
		print_error_with_types(code_gen_state->parser_state->c_lexer_state, required_type, found_type, context, "Type missmatch.  Required former, but found latter.");
		assert(0 && "Compare failed.");
	}
	return found_type;
}

void backtrack_type_stack(struct code_gen_state * code_gen_state, unsigned int target, struct parser_node * context){
	while(struct_type_description_ptr_list_size(&code_gen_state->type_stack) != target){
		destroy_type_description(code_gen_state->memory_pool_collection, pop_type_without_type_check(code_gen_state, context));
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
	) ? first_child(p) : (struct parser_node *)0;
}

struct parser_node * is_second_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		second_child(p)->type == t
	) ? second_child(p) : (struct parser_node *)0;
}

struct parser_node * is_third_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		third_child(p) &&
		third_child(p)->type == t
	) ? third_child(p) : (struct parser_node *)0;
}

struct parser_node * is_fourth_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		third_child(p) &&
		fourth_child(p) &&
		fourth_child(p)->type == t
	) ? fourth_child(p) : (struct parser_node *)0;
}

struct parser_node * is_fifth_child_type(struct parser_node * p, enum node_type t){
	return (
		first_child(p) &&
		second_child(p) &&
		third_child(p) &&
		fourth_child(p) &&
		fifth_child(p) &&
		fifth_child(p)->type == t
	) ? fifth_child(p) : (struct parser_node *)0;
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
	) ? sixth_child(p) : (struct parser_node *)0;
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
	) ? seventh_child(p) : (struct parser_node *)0;
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

unsigned int struct_type_size(struct code_gen_state * code_gen_state, struct type_description * t, enum value_type tsc, struct scope_level * source_scope_level){
	/*  Uses tsc instead of t->value type in case we want to see how big an lvalu's rvalue is */
	struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(t->specifiers);
	struct c_lexer_token * token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
	unsigned char * tag_identifier = token ? copy_string(token->first_byte, token->last_byte, code_gen_state->memory_pool_collection) : make_up_identifier(t->source_element, code_gen_state->memory_pool_collection);
	unsigned int num_children;
	struct namespace_object * obj;
	struct normalized_declaration_element * element;
	unsigned int i;
	unsigned int total_size = 0;
	struct struct_namespace_object_ptr_list children;

	obj = get_namespace_object_from_closest_namespace(tag_identifier, TAG_NAMESPACE, source_scope_level, 1, code_gen_state->memory_pool_collection);
	if(!obj){
		printf("Unknown structure: %s in file %s.\n", tag_identifier, code_gen_state->parser_state->c_lexer_state->c.filename);
		assert(obj && "Unknown identifier in struct_type_size.");
	}
	heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, tag_identifier);

	children = unsigned_char_ptr_to_struct_namespace_object_ptr_map_values(obj->children);
	num_children = struct_namespace_object_ptr_list_size(&children);
	assert(num_children);

	if(tsc == WORD_ALIGNED_RVALUE || tsc == MINIMAL_RVALUE){
		for(i = num_children - 1; ; i--){
			struct namespace_object * child_obj = struct_namespace_object_ptr_list_get(&children, i);
			unsigned int size;
			struct type_description * member_description;
			assert(struct_normalized_declaration_element_ptr_list_size(&child_obj->elements));
			element = struct_normalized_declaration_element_ptr_list_get(&child_obj->elements, 0);
			member_description = create_type_description_from_normalized_declaration_element(code_gen_state->memory_pool_collection, element, element->normalized_declaration_set->set, child_obj->scope_level, LVALUE);
			size = type_size(code_gen_state, member_description, tsc, 0, obj->scope_level);
			total_size += size;
			destroy_type_description(code_gen_state->memory_pool_collection, member_description);
			if(i == 0){
				break;
			}
		}
		struct_namespace_object_ptr_list_destroy(&children);
		return total_size;
	}else if(tsc == LVALUE){
		struct_namespace_object_ptr_list_destroy(&children);
		return pointer_type_size(code_gen_state->memory_pool_collection, t, tsc);
	}else{
		assert(0);
		return 0;
	}
}

void move_pointers_down(struct code_gen_state * code_gen_state, const char * ra, const char * rb){
	buffered_printf(code_gen_state->buffered_output, "sub %s %s WR;\n", ra, ra);
	buffered_printf(code_gen_state->buffered_output, "sub %s %s WR;\n", rb, rb);
}

void destroy_type_traversal(struct memory_pool_collection * m, struct type_traversal * traversal){
	if(traversal->type_class == TYPE_CLASS_STRUCT || traversal->type_class == TYPE_CLASS_ARRAY){
		unsigned int i;
		for(i = 0; i < struct_type_traversal_ptr_list_size(&traversal->children); i++){
			destroy_type_traversal(m, struct_type_traversal_ptr_list_get(&traversal->children, i));
		}
		struct_type_traversal_ptr_list_destroy(&traversal->children);
	}
	destroy_type_description(m, traversal->type_description);
	free(traversal);
}

struct type_traversal * construct_type_traversal(struct code_gen_state * code_gen_state, struct type_description * t, struct scope_level * source_scope_level, unsigned int arrays_as_pointers){
	struct type_traversal * traversal = (struct type_traversal *)malloc(sizeof(struct type_traversal));
	traversal->type_class = determine_type_class(code_gen_state->memory_pool_collection, t);
	traversal->type_description = copy_type_description(code_gen_state->memory_pool_collection, t);
	traversal->type_description->source_scope_level = source_scope_level;
	switch(traversal->type_class){
		case TYPE_CLASS_POINTER:{
			break;
		}case TYPE_CLASS_ARRAY:{
			struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(code_gen_state->memory_pool_collection, t->declarator);
			struct parser_node * constant_expression = get_constant_expression_from_abstract_declarator(abstract_declarator);
			struct type_description * arr_type = create_dereferenced_array_type_description_from_type_description(code_gen_state->memory_pool_collection, t);
			struct type_traversal * element_traversal = construct_type_traversal(code_gen_state, arr_type, source_scope_level, arrays_as_pointers);
			traversal->arrays_as_pointers = arrays_as_pointers;
			struct_type_traversal_ptr_list_create(&traversal->children);
			struct_type_traversal_ptr_list_add_end(&traversal->children, element_traversal);
			destroy_type_description(code_gen_state->memory_pool_collection, arr_type);
			if(constant_expression){
				assert(constant_expression->type == CONSTANT_EXPRESSION);
				traversal->arity = evaluate_compile_time_constant(code_gen_state, evaluate_constant_constant_expression(code_gen_state, constant_expression));
			}else{
				if(!arrays_as_pointers){
					assert(0 && "not implemented.");
				}
			}
			destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);
			break;
		}case TYPE_CLASS_FUNCTION:{
			assert(0 && "Not implemented.");
			break;
		}case TYPE_CLASS_STRUCT:{
			struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(t->specifiers);
			struct c_lexer_token * token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
			unsigned char * tag_identifier = token ? copy_string(token->first_byte, token->last_byte, code_gen_state->memory_pool_collection) : make_up_identifier(t->source_element, code_gen_state->memory_pool_collection);
			unsigned int num_children;
			struct namespace_object * obj;
			struct normalized_declaration_element * element;
			unsigned int i;
			struct struct_namespace_object_ptr_list children;
			obj = get_namespace_object_from_closest_namespace(tag_identifier, TAG_NAMESPACE, source_scope_level, 1, code_gen_state->memory_pool_collection);
			if(!obj){
				printf("Unknown structure: %s in file %s.\n", tag_identifier, code_gen_state->parser_state->c_lexer_state->c.filename);
				assert(obj && "Unknown identifier in construct_type_traversal.");
			}
        
			children = unsigned_char_ptr_to_struct_namespace_object_ptr_map_values(obj->children);
			num_children = struct_namespace_object_ptr_list_size(&children);
			assert(num_children);

			struct_type_traversal_ptr_list_create(&traversal->children);
        
			for(i = 0; i < num_children; i++){
				struct namespace_object * child_obj = struct_namespace_object_ptr_list_get(&children, i);
				struct type_description * member_description;
				struct type_traversal * member_traversal;
				assert(struct_normalized_declaration_element_ptr_list_size(&child_obj->elements));
				element = struct_normalized_declaration_element_ptr_list_get(&child_obj->elements, 0);
				member_description = create_type_description_from_normalized_declaration_element(code_gen_state->memory_pool_collection, element, element->normalized_declaration_set->set, source_scope_level, LVALUE);
        
				member_traversal = construct_type_traversal(code_gen_state, member_description, source_scope_level, 0);
				struct_type_traversal_ptr_list_add_end(&traversal->children, member_traversal);
        
				destroy_type_description(code_gen_state->memory_pool_collection, member_description);
			}
			heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, tag_identifier);
			struct_namespace_object_ptr_list_destroy(&children);
			break;
		}case TYPE_CLASS_UNION:{
			assert(0 && "Not implemented.");
			break;
		}case TYPE_CLASS_ENUM:{
			break;
		}case TYPE_CLASS_VOID:{
			break;
		}default:{
			break;
		}
	}
	return traversal;
}

void setup_global_type(struct code_gen_state * code_gen_state, struct type_traversal * type_traversal, struct constant_initializer_level * initializer_level){
	switch(type_traversal->type_class){
		case TYPE_CLASS_POINTER:{
			setup_global_primative(code_gen_state, type_traversal->type_description, initializer_level);
			break;
		}case TYPE_CLASS_ARRAY:{
			unsigned int i;
			struct type_traversal * element_traversal = struct_type_traversal_ptr_list_get(&type_traversal->children, 0);
			for(i = 0; i < type_traversal->arity; i++){
				struct constant_initializer_level * current_initializer_level = initializer_level ? struct_constant_initializer_level_ptr_list_get(&initializer_level->children, i) : (struct constant_initializer_level *)0; 
				setup_global_type(code_gen_state, element_traversal, current_initializer_level);
			}
			break;
		}case TYPE_CLASS_FUNCTION:{
			assert(0 && "Not implemented.");
			break;
		}case TYPE_CLASS_STRUCT:{
			unsigned int num_children = struct_type_traversal_ptr_list_size(&type_traversal->children);
			unsigned int i;
			for(i = 0; i < num_children; i++){
				struct type_traversal * member = struct_type_traversal_ptr_list_get(&type_traversal->children, i);
				setup_global_type(code_gen_state, member, (struct constant_initializer_level *)0);
			}
			break;
		}case TYPE_CLASS_UNION:{
			assert(0);
			break;
		}case TYPE_CLASS_ENUM:{
			setup_global_primative(code_gen_state, type_traversal->type_description, initializer_level);
			break;
		}case TYPE_CLASS_VOID:{
			setup_global_primative(code_gen_state, type_traversal->type_description, initializer_level);
			break;
		}default: {
			setup_global_primative(code_gen_state, type_traversal->type_description, initializer_level);
			break;
		}
	}
}

void setup_global_primative(struct code_gen_state * code_gen_state, struct type_description * t, struct constant_initializer_level * current_initializer_level){
	unsigned int element_size = type_size(code_gen_state, t, WORD_ALIGNED_RVALUE, 0, t->source_scope_level);
	struct compile_time_constant * constant = current_initializer_level ? current_initializer_level->constant : (struct compile_time_constant *)0;
	struct constant_description * constant_description = constant ? constant->constant_description : (struct constant_description *)0;
	unsigned int value = constant_description ? constant_description->native_data[0] : 0;

	(void)element_size;

	if(constant_description && constant_description->type == STRING_LITERAL){
		/*  If it is a string literal, we don't know the addr, use the label */
		struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
		unsigned char * string_literal_identifier_str = create_formatted_string(m, 20, "_%psl", (void *)current_initializer_level->constant->constant_description->native_data);
		buffered_printf(code_gen_state->buffered_output, "dw %s;\n", string_literal_identifier_str);
		heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, string_literal_identifier_str);
	}else{
		buffered_printf(code_gen_state->buffered_output, "dw 0x%X;\n", value);
	}

	if(constant_description && constant_description->type == SIZEOF){
		/*  TODO:  Need to improve how memory is consumed for constants */
		/*  and managed when constants are added/subtracted etc. */
		destroy_type_description(code_gen_state->memory_pool_collection, constant_description->type_description);
		free(constant_description->native_data);
		free(constant_description);
	}

	if(current_initializer_level && current_initializer_level->constant){
		free(current_initializer_level->constant);
	}
}

unsigned int type_size(struct code_gen_state * code_gen_state, struct type_description * t, enum value_type tsc, unsigned int force_arity_1, struct scope_level * source_scope_level){
	enum type_class c = determine_type_class(code_gen_state->memory_pool_collection, t);
	if(c == TYPE_CLASS_POINTER){
		return pointer_type_size(code_gen_state->memory_pool_collection, t, tsc);
	}

	if(c == TYPE_CLASS_ARRAY){
		struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(code_gen_state->memory_pool_collection, t->declarator);
		struct parser_node * constant_expression = get_constant_expression_from_abstract_declarator(abstract_declarator);
		if(constant_expression){
			unsigned int size;
			struct type_description * arr_type = create_dereferenced_array_type_description_from_type_description(code_gen_state->memory_pool_collection, t);
			unsigned element_size = type_size(code_gen_state, arr_type, tsc, 0, source_scope_level);
			assert(constant_expression->type == CONSTANT_EXPRESSION);
			size = evaluate_compile_time_constant(code_gen_state, evaluate_constant_constant_expression(code_gen_state, constant_expression));
			destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);
			if(force_arity_1){
				struct type_description * ptr_type = create_address_type_description_from_type_description(code_gen_state->memory_pool_collection, arr_type);
				unsigned int rtn = pointer_type_size(code_gen_state->memory_pool_collection, ptr_type, tsc);
				destroy_type_description(code_gen_state->memory_pool_collection, arr_type);
				destroy_type_description(code_gen_state->memory_pool_collection, ptr_type);
				return rtn;
			}
			destroy_type_description(code_gen_state->memory_pool_collection, arr_type);
			return element_size * size;
		}else{
			destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);
			return 8; /* TODO: 8 is wrong. This case is when an array was declared as [] without an expression */
		}
	}

	if(c == TYPE_CLASS_FUNCTION){
		assert(0 && "Should not occur.  Function coersion problem somewhere.");
		return 0;
	}

	if(c == TYPE_CLASS_STRUCT){
		return struct_type_size(code_gen_state, t, tsc, source_scope_level);
	}else if(c == TYPE_CLASS_UNION){
		assert(0);
		return 8; /* Works sometimes */
	}else if(c == TYPE_CLASS_ENUM){
		return enum_type_size(code_gen_state->memory_pool_collection, t, tsc);
	}else if(c == TYPE_CLASS_VOID){
		return void_type_size(code_gen_state->memory_pool_collection, tsc);
	}else{
		return arithmetic_type_size(code_gen_state->memory_pool_collection, t, tsc);
	}
}

unsigned int get_normalized_declaration_element_size(struct code_gen_state * code_gen_state, struct normalized_declaration_element * element, unsigned int force_arity_1, struct scope_level * source_scope_level){
	unsigned int rtn;
	struct type_description * type_description = create_type_description_from_normalized_declaration_element(code_gen_state->memory_pool_collection, element, element->normalized_declaration_set->set, source_scope_level, WORD_ALIGNED_RVALUE);
	convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, type_description);
	rtn = type_size(code_gen_state, type_description, type_description->value_type, force_arity_1, source_scope_level);
	destroy_type_description(code_gen_state->memory_pool_collection, type_description);
	return rtn;
}

unsigned int get_namespace_object_size(struct code_gen_state * code_gen_state, struct namespace_object * obj, unsigned int force_arity_1){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, num_elements - 1);
	return get_normalized_declaration_element_size(code_gen_state, element, force_arity_1, obj->scope_level);
}

unsigned int get_parameter_offset(struct code_gen_state * code_gen_state, struct namespace_object * obj){
	if(obj){
		unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, num_elements - 1);
		unsigned int rtn;
		struct type_description * type_description = create_type_description_from_normalized_declaration_element(code_gen_state->memory_pool_collection, element, (struct parser_node *)0, obj->scope_level, LVALUE);
		struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(code_gen_state->memory_pool_collection, element->normalized_declarator);
		convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, type_description);

		if(is_array(code_gen_state->memory_pool_collection, abstract_declarator)){
			/*  Array parameters are just the pointer */
			rtn = 4 + get_parameter_offset(code_gen_state, obj->previous);
		}else{
			rtn = get_namespace_object_size(code_gen_state, obj, 0) + get_parameter_offset(code_gen_state, obj->previous);
		}
		
		destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);
		destroy_type_description(code_gen_state->memory_pool_collection, type_description);
		return rtn;
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
		unsigned int force_arity_1 = (in_progress == variable_scope) ? 1 : 0;
		if(start_obj){
			unsigned int a;
			a = get_local_offset_h1(code_gen_state, start_obj, force_arity_1);
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
		identifier = copy_string(token->first_byte, token->last_byte, code_gen_state->memory_pool_collection);
		a = get_namespace_object_size(code_gen_state, obj, force_arity_1);
		buffered_printf(code_gen_state->buffered_output, ";Add offset of %d due to local %s\n", a, identifier);
		heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, identifier);
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
		unsigned int current_type_size = type_size(code_gen_state, t, t->value_type, 1, get_current_scope_level(code_gen_state));
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
	t = struct_type_description_ptr_list_pop_end(&code_gen_state->type_stack);
	size = type_size(code_gen_state, t, t->value_type, 1, get_current_scope_level(code_gen_state));
	num_words = size / 4;
	assert(size - (4*num_words) == 0);
	for(i = 0; i < num_words; i++){
		buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Deleting top type word %d.\n", i);
	}
	destroy_type_description(code_gen_state->memory_pool_collection, t);
}

void copy_words(struct code_gen_state * code_gen_state, const char * src_addr, const char * dst_addr, const char * tmp, unsigned int bytes_to_copy){
	/*  src_addr is a string that represents the register containing the address of bytes to copy */
	/*  Copy starts at high address and goes down for stack and src_addr  */
	unsigned int words_to_copy = bytes_to_copy / 4;
	unsigned int i;
	assert(bytes_to_copy != 0);
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
	struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
	obj = get_namespace_object_from_closest_namespace(identifier, IDENTIFIER_NAMESPACE, get_current_scope_level(code_gen_state), 0, m);
	if(!obj){
		printf("Unknown identifier: %s in file %s.\n", identifier, parser_state->c_lexer_state->c.filename);
		assert(obj && "Unknown identifier.");
	}
	num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, num_elements - 1);
	buffered_printf(code_gen_state->buffered_output,"; Loading identifier %s ", identifier);
	buffered_printf(code_gen_state->buffered_output,"from location %d\n", obj->object_location);

	type_description = create_type_description_from_normalized_declaration_element(m, element, context, obj->scope_level, LVALUE);
	convert_to_untypedefed_type_description(m, type_description);
	assert(type_description->value_type == LVALUE);

	abstract_declarator = create_abstract_declarator_from_normalized_declarator(m, type_description->declarator);
	switch(obj->object_location){
		case GLOBAL:{
			if(is_function(m, abstract_declarator)){
				struct type_description * ptr = create_address_type_description_from_type_description(m, type_description);
				buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Push \n");
				buffered_printf(code_gen_state->buffered_output,"ll r1 %s;  Load the variable %s\n", identifier, identifier); 
				buffered_printf(code_gen_state->buffered_output,"sto SP r1;  Store it on the stack.\n");
				destroy_type_description(m, type_description);
				require_external_symbol(m, &code_gen_state->symbols, identifier);
				ptr->value_type = WORD_ALIGNED_RVALUE;
				push_type(code_gen_state, ptr, context);
			}else{
				unsigned char * name = create_formatted_string(m, 20, "globalvar_%s", identifier);
				buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Push\n");
				buffered_printf(code_gen_state->buffered_output,"ll r1 %s;  Load address of %s\n", name, name); 
				buffered_printf(code_gen_state->buffered_output,"sto SP r1;  store address.\n"); 
				push_type(code_gen_state, type_description, context);
				require_external_symbol(m, &code_gen_state->symbols, name);
				heap_memory_pool_free(m->heap_pool, name);
			}
			break;
		}case PARAMETER:{
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Push\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; This is the number of bytes up from the FP where the value is stored\n", get_parameter_offset(code_gen_state, obj));
			buffered_printf(code_gen_state->buffered_output,"sub r1 FP r1; r1 now stores the addr of the value of the argument.\n");
			if(is_array(m, abstract_declarator)){
				buffered_printf(code_gen_state->buffered_output,"loa r1 r1;  For an array, parameter data is one extra level of indirection.\n");
			}
			buffered_printf(code_gen_state->buffered_output,"sto SP r1; Store lvalue\n");

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
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; Offset due to locals (%d) + %d for temps\n", (offset + temps_size) - current_obj_size, offset, temps_size);
			buffered_printf(code_gen_state->buffered_output,"add r1 SP r1; r1 now stores the addr the the local.\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Push\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1; store address\n");
			push_type(code_gen_state, type_description, context);
			break;
		}case ENUM_IDENTIFIER:{
			unsigned int enum_value = get_enum_value(m, element);
			buffered_printf(code_gen_state->buffered_output,";Load our enum value\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Point to new value\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X; load enum integer value\n", enum_value);
			buffered_printf(code_gen_state->buffered_output,"sto SP r1; Store variable ptr in value too\n");
			type_description->value_type = WORD_ALIGNED_RVALUE;
			push_type(code_gen_state, type_description, context);
			break;
		}case LOCATION_STRUCT:{
			destroy_type_description(m, type_description);
			break;
		}default:{
			assert(0 && "Unknown object location.");
		}

	}
	destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator);
}

void g_primary_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	struct parser_state * parser_state = code_gen_state->parser_state;
	struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
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
			unsigned char * constant = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte, m);
			struct constant_description * description = find_constant(parser_state, constant);
			assert(description);
			buffered_printf(code_gen_state->buffered_output,";Code to load %s with data 0x%X\n", constant, ((unsigned char *)description->native_data)[0]);
			buffered_printf(code_gen_state->buffered_output,"; onto top of stack\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", ((unsigned char *)description->native_data)[0]);
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;\n");
			push_type(code_gen_state, copy_type_description(m, description->type_description), p);
			heap_memory_pool_free(m->heap_pool, constant);
		}else if(is_terminal_c_token_type(first_child(p), CONSTANT_DECIMAL) || is_terminal_c_token_type(first_child(p), CONSTANT_HEX)){
			unsigned char * constant = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte, m);
			struct constant_description * description = find_constant(parser_state, constant);
			assert(description);
			buffered_printf(code_gen_state->buffered_output,";Code to load %s with data %d\n", constant, *(description->native_data));
			buffered_printf(code_gen_state->buffered_output,"; onto top of stack\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			
			if(((unsigned int)MAX_LL_CONSTANT) < *(description->native_data)){
				/*  Value must be loaded in 2 parts with shift */
				buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;  Load upper 16 bits\n", ((*(description->native_data)) & (~((unsigned int)MAX_LL_CONSTANT))) >> 16);
				buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;  Number of bits to shift them\n", 16);
				buffered_printf(code_gen_state->buffered_output,"shl r1 r2;  r1 contains upper 16 bits of result\n");
				buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;  Load lower 16 bits\n", (*(description->native_data)) & ((unsigned int)MAX_LL_CONSTANT));
				buffered_printf(code_gen_state->buffered_output,"or r1 r1 r2;  Put the pieces together\n");
			}else{
				/*  If value can be loaded directly */
				buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", *(description->native_data));
			}

			buffered_printf(code_gen_state->buffered_output,"sto SP r1;\n");
			push_type(code_gen_state, copy_type_description(m, description->type_description), p);
			heap_memory_pool_free(m->heap_pool, constant);
		}else if(is_terminal_c_token_type(first_child(p), IDENTIFIER)){
			unsigned char * identifier = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte, m);
			load_identifier(code_gen_state, identifier, p);
			heap_memory_pool_free(m->heap_pool, identifier);
		}else if(is_terminal_c_token_type(first_child(p), STRING_LITERAL)){
			unsigned char * constant = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte, m);
			struct constant_description * description = find_constant(parser_state, constant);
			unsigned char * string_literal_identifier_str = create_formatted_string(m, 20, "_%psl", (void *)description->native_data);
			assert(description);
			buffered_printf(code_gen_state->buffered_output,"; onto top of stack\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 %s;\n", string_literal_identifier_str);
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;\n");
			push_type(code_gen_state, copy_type_description(m, description->type_description), p);
			require_internal_symbol(m, &code_gen_state->symbols, string_literal_identifier_str);
			heap_memory_pool_free(m->heap_pool, string_literal_identifier_str);
			heap_memory_pool_free(m->heap_pool, constant);
		}else{
			assert(0 && "Unknown terminal in primary expression\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported primary expression.\n");
	}
}

void g_epsilon(void){
}



void copy_lvalue_into_rvalue(struct code_gen_state * code_gen_state, const char * source_lvalue, const char * dst, const char * size_minus_4, const char * tmp, unsigned int size){
	buffered_printf(code_gen_state->buffered_output,"add r3 r3 %s;     Copy from last word of rvalue\n", size_minus_4);
	copy_words(code_gen_state, source_lvalue, dst, tmp, size);
}

void do_character_rvalue_conversion(struct code_gen_state * code_gen_state, const char * lvalue, const char * dst, const char * tmp1, const char * tmp2, const char * tmp3){
	/*  Assumes that reg is pointing to an lvalue the represents the character */
	buffered_printf(code_gen_state->buffered_output,"div %s %s WR;     stores int div amount\n", tmp3, lvalue);
	buffered_printf(code_gen_state->buffered_output,"mul %s %s WR;     4 byte aligned address\n", tmp2, tmp3);
	buffered_printf(code_gen_state->buffered_output,"loa %s %s;        stores the value with other bytes\n", tmp1, tmp2);
	buffered_printf(code_gen_state->buffered_output,"sub %s %s %s;     mod amount\n", tmp2, lvalue, tmp2);
	buffered_printf(code_gen_state->buffered_output,"ll %s 0xFF;       Byte mask\n", tmp3);
	buffered_printf(code_gen_state->buffered_output,"mul %s WR %s;     Bytes to shift left\n", tmp2, tmp2);
	buffered_printf(code_gen_state->buffered_output,"shl %s %s;        Shift mask to target byte\n", tmp3, tmp2);
	buffered_printf(code_gen_state->buffered_output,"shl %s %s;        Shift mask to target byte\n", tmp3, tmp2);
	buffered_printf(code_gen_state->buffered_output,"and %s %s %s;     Grab the bits we want\n", tmp1, tmp3, tmp1);
	buffered_printf(code_gen_state->buffered_output,"shr %s %s;        Shift result to right\n", tmp1, tmp2);
	buffered_printf(code_gen_state->buffered_output,"shr %s %s;        Shift result to right\n", tmp1, tmp2);
	buffered_printf(code_gen_state->buffered_output,"sto %s %s;        Store result\n", dst, tmp1);
}

unsigned int decay_to_pointer_if_array(struct code_gen_state * code_gen_state, struct type_description ** t){
	struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(code_gen_state->memory_pool_collection, (*t)->declarator);
	unsigned int its_an_array = (unsigned int)is_array(code_gen_state->memory_pool_collection, abstract_declarator);
	destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);
	if(its_an_array){
		/*  Arrays decay to pointers */
		struct type_description * dereferenced_array_type = create_dereferenced_array_type_description_from_type_description(code_gen_state->memory_pool_collection, *t);
       		struct type_description * pointer_description = create_address_type_description_from_type_description(code_gen_state->memory_pool_collection, dereferenced_array_type);
		buffered_printf(code_gen_state->buffered_output,";       Converted an array lvalue to pointer rvalue\n");
		destroy_type_description(code_gen_state->memory_pool_collection, dereferenced_array_type);
		destroy_type_description(code_gen_state->memory_pool_collection, *t);
		*t = pointer_description;
		return 1;
	}else{
		return 0;
	}
}

struct type_description * ensure_top_type_is_r_value(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * description = pop_type_without_type_check(code_gen_state, context);
	unsigned int size = type_size(code_gen_state, description, MINIMAL_RVALUE, 1, description->source_scope_level);
	if(description->value_type == LVALUE){
		if(!decay_to_pointer_if_array(code_gen_state, &description)){
			if(size == 1){
				buffered_printf(code_gen_state->buffered_output,"loa r3 SP;        r3 now stores the lvalue\n");
				do_character_rvalue_conversion(code_gen_state, "r3", "SP", "r1", "r2", "r4");
			}else{
				buffered_printf(code_gen_state->buffered_output,";       convert to rvalue\n");
				buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;       r2 now stores num bytes to end of rvalue\n", (size -4));
				buffered_printf(code_gen_state->buffered_output,"loa r3 SP;        r3 now stores the lvalue\n");
				copy_lvalue_into_rvalue(code_gen_state, "r3", "SP", "r2", "r5", size);
			}
		}
		description->value_type = WORD_ALIGNED_RVALUE;
		return description;
	}
	return description;
}

void compare_function_argument_type(struct code_gen_state * code_gen_state, struct parser_node * parameter_declaration, unsigned int param_index, struct parser_node * context){
	struct normalized_declaration_set * declaration_set;
	struct type_description * tmp_param_type;
	struct type_description * param_type;
	struct type_description * top_type;
	enum type_class type_class_required;
	enum type_class type_class_observed;
	top_type = pop_type_without_type_check(code_gen_state, context);
	assert(parameter_declaration);
	declaration_set = create_normalized_declaration_set_from_parser_node(code_gen_state->memory_pool_collection, parameter_declaration, (struct normalized_declaration_set *)0);
	tmp_param_type = struct_type_description_memory_pool_malloc(code_gen_state->memory_pool_collection->struct_type_description_pool);
	tmp_param_type->specifiers = declaration_set->normalized_specifiers;

	if(struct_normalized_declarator_ptr_list_size(declaration_set->normalized_declarators) == 1){
		tmp_param_type->declarator = struct_normalized_declarator_ptr_list_get(declaration_set->normalized_declarators, 0);
	}else if(struct_normalized_declarator_ptr_list_size(declaration_set->normalized_declarators) == 0){
		tmp_param_type->declarator = (struct normalized_declarator *)0;
	}else{
		assert(0 && "Not possible");
	}
	param_type = copy_type_description(code_gen_state->memory_pool_collection, tmp_param_type);
	param_type->value_type = top_type->value_type;
	param_type->source_scope_level = code_gen_state->parser_state->top_scope;
	param_type->context = top_type->context;
	param_type->source_element = top_type->source_element;

	struct_type_description_memory_pool_free(code_gen_state->memory_pool_collection->struct_type_description_pool, tmp_param_type);
	destroy_normalized_declaration_element_list(code_gen_state->memory_pool_collection, create_normalized_declaration_element_list(declaration_set));

	convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, top_type);
	convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, param_type);

	type_class_observed = determine_type_class(code_gen_state->memory_pool_collection, top_type);
	type_class_required = determine_type_class(code_gen_state->memory_pool_collection, param_type);

	/*  Normalize to pointers */
	decay_to_pointer_if_array(code_gen_state, &top_type);
	decay_to_pointer_if_array(code_gen_state, &param_type);

	/*  May need to promote the argument */
	if((type_class_observed == TYPE_CLASS_CHAR && type_class_required == TYPE_CLASS_INT)){
		perform_integral_promotion(code_gen_state, &top_type, "SP");
	}else if((type_class_observed == TYPE_CLASS_SHORT && type_class_required == TYPE_CLASS_INT)){
		perform_integral_promotion(code_gen_state, &top_type, "SP");
	}else if((type_class_observed == TYPE_CLASS_INT && type_class_required == TYPE_CLASS_CHAR)){
		struct type_description * new_t = create_empty_type_description(code_gen_state->memory_pool_collection);
		if(is_signed(param_type)){
			new_t = add_specifier(code_gen_state->memory_pool_collection, new_t, CHAR);
		}else{
			new_t = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, new_t, UNSIGNED), CHAR);
		}
		new_t->value_type = top_type->value_type;
		new_t->source_scope_level = top_type->source_scope_level;
		new_t->context = top_type->context;
		new_t->source_element = top_type->source_element;
		destroy_type_description(code_gen_state->memory_pool_collection, top_type);
		top_type = new_t;
	}

	if(!type_description_cmp(code_gen_state->memory_pool_collection, param_type, top_type)){
		/*  Put the parameter type back on the stack.*/
		push_type(code_gen_state, top_type, context);
		destroy_type_description(code_gen_state->memory_pool_collection, param_type);
	}else{
		print_error_with_types(code_gen_state->parser_state->c_lexer_state, param_type, top_type, context, "\nType missmatch for argument.");
		printf("Missmatch from argument with zero based index %d.  \n", param_index);
		print_error_with_type(code_gen_state->parser_state->c_lexer_state, param_type, parameter_declaration, "Function was declared here:");
		assert(0);
	}

}

unsigned int g_argument_expression_list_rest(struct parser_node * p, struct code_gen_state * code_gen_state, struct parser_node * parameter_type_list, unsigned int current_parameter_index, unsigned int is_variadic){
	if(check_three_children(p, TERMINAL, ASSIGNMENT_EXPRESSION, ARGUMENT_EXPRESSION_LIST_REST)){
		if(is_terminal_c_token_type(first_child(p),COMMA_CHAR)){
			unsigned int arg_type_size;
			struct type_description * arg_type;
			struct parser_node * parameter_declaration;
			buffered_printf(code_gen_state->buffered_output,";       About to load function argument\n");
			g_assignment_expression(second_child(p), code_gen_state);
			arg_type = ensure_top_type_is_r_value(code_gen_state, second_child(p));
			arg_type_size = type_size(code_gen_state, arg_type, arg_type->value_type, 1, arg_type->source_scope_level);
			push_type(code_gen_state, arg_type, second_child(p));
			parameter_declaration = get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list, current_parameter_index);
			if(is_variadic){
				/* TODO: Integral promotion */
			}else if(parameter_declaration){
				compare_function_argument_type(code_gen_state, parameter_declaration, current_parameter_index, second_child(p));
			}else{
				print_error_with_type(code_gen_state->parser_state->c_lexer_state, arg_type, second_child(p), "Too many parameters.");
				assert(0 && "Too many parameters.");
			}
			return arg_type_size + g_argument_expression_list_rest(third_child(p), code_gen_state, parameter_type_list, current_parameter_index + 1, is_variadic);
		}else{
			assert(0 &&"Expected COMMA_CHAR.\n");
		}
	}else if(check_one_child(p, EPSILON)){
		/*  Make sure there are no extra parameters */
		assert(!get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list, current_parameter_index) && "Missing parameters passed.");
		return 0;
	}else{
		assert(0 &&"Expected init_declarator_list_rest.\n");
		return 0;
	}
}

unsigned int g_argument_expression_list(struct parser_node * p, struct code_gen_state * code_gen_state, struct parser_node * parameter_type_list, unsigned int current_parameter_index, unsigned int is_variadic){
	if(check_two_children(p, ASSIGNMENT_EXPRESSION, ARGUMENT_EXPRESSION_LIST_REST)){
		unsigned int arg_type_size;
		struct type_description * arg_type;
		struct parser_node * parameter_declaration;
		buffered_printf(code_gen_state->buffered_output,";       About to load function argument\n");
		g_assignment_expression(first_child(p), code_gen_state);
		arg_type = ensure_top_type_is_r_value(code_gen_state, first_child(p));
		arg_type_size = type_size(code_gen_state, arg_type, arg_type->value_type, 1, arg_type->source_scope_level);
		push_type(code_gen_state, arg_type, first_child(p));
		parameter_declaration = get_nth_parameter_declaration_from_parameter_type_list(parameter_type_list, current_parameter_index);
		if(is_variadic){
			/* TODO: Integral promotion */
		}else if(parameter_declaration){
			compare_function_argument_type(code_gen_state, parameter_declaration, current_parameter_index, first_child(p));
		}else{
			print_error_with_type(code_gen_state->parser_state->c_lexer_state, arg_type, first_child(p), "Too many parameters.");
			assert(0 && "Too many parameters.");
		}
		return arg_type_size + g_argument_expression_list_rest(second_child(p), code_gen_state, parameter_type_list, current_parameter_index + 1, is_variadic);
	}else{
		assert(0 &&"Expected init_declarator_list.\n");
		return 0;
	}
}


void function_call(struct parser_node * argument_expression_list, struct parser_node * postfix_experssion_rest, struct code_gen_state * code_gen_state, struct parser_node * context){
        unsigned int bytes_return_value;
        unsigned int bytes_arguments = 0;
        unsigned int type_stack_size_before;
        /*  Grab the function type from the top of the stack */
        struct type_description * fcn_ptr_type = ensure_top_type_is_r_value(code_gen_state, context);
        struct type_description * fcn_type = create_dereferenced_pointer_type_description_from_type_description(code_gen_state->memory_pool_collection, fcn_ptr_type);
	struct type_description * return_type_description = get_current_function_return_type_description(code_gen_state->memory_pool_collection, fcn_type);
	struct type_description * word_type = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, create_empty_type_description(code_gen_state->memory_pool_collection), UNSIGNED), INT);
	struct parser_node * abstract_declarator = create_abstract_declarator_from_normalized_declarator(code_gen_state->memory_pool_collection, fcn_type->declarator);
	struct parser_node * parameter_type_list = get_parameter_type_list_from_abstract_declarator(abstract_declarator);
	assert(parameter_type_list);
	word_type->source_scope_level = get_current_scope_level(code_gen_state);
	word_type->value_type = WORD_ALIGNED_RVALUE;
	convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, return_type_description);

	return_type_description->value_type = WORD_ALIGNED_RVALUE;
	push_type(code_gen_state, return_type_description, context);
        bytes_return_value = type_size(code_gen_state, return_type_description, WORD_ALIGNED_RVALUE, 0, return_type_description->source_scope_level);
	/*  -8 because the function address that was loaded gets overwritten with the return value. */
        buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;  Number of extra bytes for rtn val\n", bytes_return_value - 4);
        buffered_printf(code_gen_state->buffered_output,"sub SP SP r2; SP points to top of rtn value space\n");
        buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Inc another 4 bytes for the return address.\n");
        buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;  Push another 4 bytes for the frame pointer.\n");
        buffered_printf(code_gen_state->buffered_output,"sto SP FP;  Put the frame pointer on the stack\n");
	push_type(code_gen_state, copy_type_description(code_gen_state->memory_pool_collection, word_type), context); /* For FP */
	push_type(code_gen_state, copy_type_description(code_gen_state->memory_pool_collection, word_type), context); /* For return addr */
	type_stack_size_before = struct_type_description_ptr_list_size(&code_gen_state->type_stack);
	if(argument_expression_list){
		bytes_arguments = g_argument_expression_list(argument_expression_list, code_gen_state, parameter_type_list, 0, is_function_variadic(abstract_declarator));
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

	/*  Pop each of the arguments loaded in the argument_expression_list */
	backtrack_type_stack(code_gen_state, type_stack_size_before, context);
	buffered_printf(code_gen_state->buffered_output,";  Finished poping arguments.\n");
	/*  two more pop for fp and rtnaddr */
	destroy_type_description(code_gen_state->memory_pool_collection, pop_type(code_gen_state, word_type, context));
	destroy_type_description(code_gen_state->memory_pool_collection, pop_type(code_gen_state, word_type, context));
	destroy_type_description(code_gen_state->memory_pool_collection, fcn_ptr_type);
	destroy_type_description(code_gen_state->memory_pool_collection, fcn_type);
	destroy_type_description(code_gen_state->memory_pool_collection, word_type);
	g_postfix_expression_rest(postfix_experssion_rest, code_gen_state);
	destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);
}

void do_struct_dereference_operator(struct code_gen_state * code_gen_state, unsigned char * target_member_identifier, struct parser_node * context, unsigned int is_ptr){
	struct parser_node * struct_or_union_or_enum_specifier;
	struct c_lexer_token * struct_tag_token;
	unsigned char * struct_tag_identifier;
	unsigned int num_children;
	struct namespace_object * obj;
	struct normalized_declaration_element * element;
	unsigned int i;
	unsigned int bytes_before = 0;
	unsigned int bytes_total = 0;
	unsigned int bytes_member = 0;
	struct type_description * t = pop_type_without_type_check(code_gen_state, context);
	unsigned int member_found = 0;
	struct parser_node * abstract_declarator = (struct parser_node *)0;
	unsigned int is_lvalue = t->value_type == LVALUE;
	struct struct_namespace_object_ptr_list children;
	struct namespace_object * member_object;
	struct namespace_object * first_struct_object;
	struct namespace_object * current_struct_object;

	struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(t->specifiers);
	if(!struct_or_union_or_enum_specifier){
		print_error_with_type(code_gen_state->parser_state->c_lexer_state, t, context, "Bad structure type:");
		assert(0);
	}
	struct_tag_token = get_struct_or_union_or_enum_tag_token(struct_or_union_or_enum_specifier);
	struct_tag_identifier = struct_tag_token ? copy_string(struct_tag_token->first_byte, struct_tag_token->last_byte, code_gen_state->memory_pool_collection) : make_up_identifier(t->source_element, code_gen_state->memory_pool_collection);

	obj = get_namespace_object_from_closest_namespace(struct_tag_identifier, TAG_NAMESPACE, t->source_scope_level, 1, code_gen_state->memory_pool_collection);
	if(!obj){
		printf("Unknown structure: %s in file %s.\n", struct_tag_identifier, code_gen_state->parser_state->c_lexer_state->c.filename);
		assert(obj && "Unknown identifier.");
	}
	children = unsigned_char_ptr_to_struct_namespace_object_ptr_map_values(obj->children);
	assert(unsigned_char_ptr_to_struct_namespace_object_ptr_map_exists(obj->children, target_member_identifier) && "Unknown structure member.");
	member_object = unsigned_char_ptr_to_struct_namespace_object_ptr_map_get(obj->children, target_member_identifier);
	/*  Find a reference to the first struct member of the struct this member belongs to */
	first_struct_object = member_object;
	while(first_struct_object->previous) { first_struct_object = first_struct_object->previous; }

	num_children = struct_namespace_object_ptr_list_size(&children);
	assert(num_children);

	current_struct_object = first_struct_object;

	for(i = 0; i < num_children; i++){
		struct namespace_object * child_obj = current_struct_object;
		unsigned int size;
		struct type_description * member_description;
		struct c_lexer_token * member_identifier_token;
		unsigned char * member_name;
		assert(current_struct_object);
		assert(struct_normalized_declaration_element_ptr_list_size(&child_obj->elements));
		element = struct_normalized_declaration_element_ptr_list_get(&child_obj->elements, 0);
		member_identifier_token = get_identifier_token_from_normalized_declarator(element->normalized_declarator);
		member_description = create_type_description_from_normalized_declaration_element(code_gen_state->memory_pool_collection, element, context, obj->scope_level, LVALUE);
		member_name = copy_string(member_identifier_token->first_byte, member_identifier_token->last_byte, code_gen_state->memory_pool_collection);
		size = type_size(code_gen_state, member_description, WORD_ALIGNED_RVALUE, 0, obj->scope_level);
		bytes_total += size;
		buffered_printf(code_gen_state->buffered_output,";member %s has size %d      \n", member_name, size);
		if(!unsigned_strcmp(member_name, target_member_identifier)){
			if(is_lvalue || is_ptr){
				member_description->value_type = LVALUE;
			}else{
				member_description->value_type = WORD_ALIGNED_RVALUE;
			}
			push_type(code_gen_state, member_description, context);
			member_found = 1;
			bytes_member = size;
			abstract_declarator = create_abstract_declarator_from_normalized_declarator(code_gen_state->memory_pool_collection, member_description->declarator);
			heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, member_name);
			break;
		}else{
			if(!member_found){
				bytes_before += size;
			}
			destroy_type_description(code_gen_state->memory_pool_collection, member_description);
		}
		heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, member_name);
		current_struct_object = current_struct_object->next;
	}
	struct_namespace_object_ptr_list_destroy(&children);
	if(!member_found){
		assert(0 && "Member of struct not found.");
	}

	if(is_lvalue){
		buffered_printf(code_gen_state->buffered_output,"loa r1 SP;      Load lvalue of structure\n");
	}else{
		buffered_printf(code_gen_state->buffered_output,"add r1 SP ZR;      Structure already on stack, start with SP\n");
	}

	if(is_ptr){
		buffered_printf(code_gen_state->buffered_output,"loa r1 r1;      Go through extra indirection level\n");
	}

	buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;     Offset to item\n", bytes_before);
	buffered_printf(code_gen_state->buffered_output,"add r1 r1 r2;   r1 stores address of item\n");

	/*  If original was an lvalue, then we can store the result as an lvalue too */
	if(is_lvalue || is_ptr){
		buffered_printf(code_gen_state->buffered_output,"sto SP r1;      Save as new lvalue \n");
	}else{
		/*  Otherwise must create new rvalue on stack */
		buffered_printf(code_gen_state->buffered_output,"ll r3 0x%X;      Bytes to last word of struct rvalue \n", bytes_total - 4);
		buffered_printf(code_gen_state->buffered_output,"ll r5 0x%X;      Bytes to last word of member\n", bytes_member - 4);
		buffered_printf(code_gen_state->buffered_output,"add r4 SP r3;      r4 pointing to last word of struct rvalue\n");
		buffered_printf(code_gen_state->buffered_output,"add r6 r1 r5;      r6 pointing to last word of member to copy out\n");
		copy_words(code_gen_state, "r6", "r4", "r7", bytes_total);
		buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;      Bytes to pop off stack\n", bytes_total - bytes_member);
		buffered_printf(code_gen_state->buffered_output,"add SP SP r1;      Bytes to pop off stack\n");
	}

	destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);
	heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, struct_tag_identifier);
	destroy_type_description(code_gen_state->memory_pool_collection, t);
}

void g_postfix_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_four_children(p, TERMINAL, EXPRESSION, TERMINAL, POSTFIX_EXPRESSION_REST)){
		if(
			is_terminal_c_token_type(first_child(p), OPEN_SQUARE_BRACKET_CHAR) &&
			is_terminal_c_token_type(third_child(p), CLOSE_SQUARE_BRACKET_CHAR)
		){
			struct type_description * index_type;
			struct type_description * array_type;
			struct type_description * dereferenced_array_type;
			unsigned int element_size;
			unsigned int its_an_array;
			struct parser_node * abstract_declarator;
			unsigned int is_lvalue;
			g_expression(second_child(p), code_gen_state);

			index_type = ensure_top_type_is_r_value(code_gen_state, p);
			pop(code_gen_state, "r1", index_type->value_type); /* Array index */
			destroy_type_description(code_gen_state->memory_pool_collection, index_type);

			array_type = pop_type_without_type_check(code_gen_state, p);
			is_lvalue = array_type->value_type == LVALUE;

			dereferenced_array_type = create_dereferenced_array_type_description_from_type_description(code_gen_state->memory_pool_collection, array_type);
			push_type(code_gen_state, dereferenced_array_type, p);

			element_size = type_size(code_gen_state, dereferenced_array_type, MINIMAL_RVALUE, 0, dereferenced_array_type->source_scope_level);
			/*  Determine if the result will be an array (multi-dimensional array case) */
			abstract_declarator = create_abstract_declarator_from_normalized_declarator(code_gen_state->memory_pool_collection, array_type->declarator);
			/*  If it is not an array it should be a pointer */
			its_an_array = (unsigned int)is_array(code_gen_state->memory_pool_collection, abstract_declarator);
			destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);

			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Load the address of the first arr element\n");

			if((!its_an_array && is_lvalue)){
				buffered_printf(code_gen_state->buffered_output,"loa r2 r2; Load whatever the pointer was\n");
			}

			buffered_printf(code_gen_state->buffered_output,"ll r5 0x%X;\n", element_size);
			buffered_printf(code_gen_state->buffered_output,"mul r3 r1 r5; compute offset in array \n");
			buffered_printf(code_gen_state->buffered_output,"add r1 r2 r3; Actual address of item \n");

			/*  rvalue arrays */
			if((its_an_array && !is_lvalue)){
				if(element_size == 1){ /* Characters */
					assert(0); /*TODO*/
				}else{
					buffered_printf(code_gen_state->buffered_output,"ll r6 0x%X;\n", element_size -4);
					buffered_printf(code_gen_state->buffered_output,"add r3 SP ZR; Copy stack pointer\n");
					buffered_printf(code_gen_state->buffered_output,"sub SP SP r6; Make room on stack\n");
					copy_words(code_gen_state, "r2", "r3", "r4", element_size);
				}
			}else{
				buffered_printf(code_gen_state->buffered_output,"sto SP r1; Write address\n");
			}

			if((its_an_array && is_lvalue) || (!its_an_array && !is_lvalue) || (!its_an_array && is_lvalue)){
				buffered_printf(code_gen_state->buffered_output,";Result of array indexing was lvalue\n");
				dereferenced_array_type->value_type = LVALUE;
			}else if(its_an_array && !is_lvalue){
				buffered_printf(code_gen_state->buffered_output,";Result of array indexing was rvalue\n");
				dereferenced_array_type->value_type = WORD_ALIGNED_RVALUE;
			}

			destroy_type_description(code_gen_state->memory_pool_collection, array_type);
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
			function_call((struct parser_node *)0, third_child(p), code_gen_state, p);
		}else if(
			is_terminal_c_token_type(first_child(p), DOT_CHAR) &&
			is_terminal_c_token_type(second_child(p), IDENTIFIER)
		){
			unsigned char * identifier = copy_string(second_child(p)->c_lexer_token->first_byte, second_child(p)->c_lexer_token->last_byte, code_gen_state->memory_pool_collection);
			do_struct_dereference_operator(code_gen_state, identifier, second_child(p), 0);
			g_postfix_expression_rest(third_child(p), code_gen_state);
			heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, identifier);
		}else if(
			is_terminal_c_token_type(first_child(p), PTR_OP) &&
			is_terminal_c_token_type(second_child(p), IDENTIFIER)
		){
			unsigned char * identifier = copy_string(second_child(p)->c_lexer_token->first_byte, second_child(p)->c_lexer_token->last_byte, code_gen_state->memory_pool_collection);
			do_struct_dereference_operator(code_gen_state, identifier, second_child(p), 1);
			g_postfix_expression_rest(third_child(p), code_gen_state);
			heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, identifier);
		}else{
			assert(0 &&"Expected () in postfix expression rest.\n");
		}
	}else if(check_two_children(p, TERMINAL, POSTFIX_EXPRESSION_REST)){
		if(is_terminal_c_token_type(first_child(p), INC_OP)){
			struct type_description * orig_lvalue = pop_type_without_type_check(code_gen_state, p); /*  Pop the base type */
			struct type_description * copy_lvalue = copy_type_description(code_gen_state->memory_pool_collection, orig_lvalue);
			struct type_description * t = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, create_empty_type_description(code_gen_state->memory_pool_collection), UNSIGNED), INT);
			struct type_description * result_description;
			t->value_type = WORD_ALIGNED_RVALUE;
			t->source_scope_level = get_current_scope_level(code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;    Lvalue\n");
			buffered_printf(code_gen_state->buffered_output,"loa r9 r1;    value before\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Push for new lvalue\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;    Write copy of lvalue\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x1;    Increment\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Push for increment \n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;    Write increment value\n");
			push_type(code_gen_state, orig_lvalue, p);
			push_type(code_gen_state, copy_lvalue, p);
			push_type(code_gen_state, t, p);
			do_additive_expression(code_gen_state, p, PLUS_CHAR);
			do_assignment(code_gen_state, p);
			buffered_printf(code_gen_state->buffered_output,"sto SP r9;    Previous value\n");
			result_description = pop_type_without_type_check(code_gen_state, p); /*  Pop the base type */
			result_description->value_type = WORD_ALIGNED_RVALUE;
			push_type(code_gen_state, result_description, p);
			g_postfix_expression_rest(second_child(p), code_gen_state);
		}else if(is_terminal_c_token_type(first_child(p), DEC_OP)){
			struct type_description * orig_lvalue = pop_type_without_type_check(code_gen_state, p); /*  Pop the base type */
			struct type_description * copy_lvalue = copy_type_description(code_gen_state->memory_pool_collection, orig_lvalue);
			struct type_description * t = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, create_empty_type_description(code_gen_state->memory_pool_collection), UNSIGNED), INT);
			struct type_description * result_description;
			t->value_type = WORD_ALIGNED_RVALUE;
			t->source_scope_level = get_current_scope_level(code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;    Lvalue\n");
			buffered_printf(code_gen_state->buffered_output,"loa r9 r1;    value before\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Push for new lvalue\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;    Write copy of lvalue\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x1;    Decrement\n");
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Push for decrement\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;    Write decrement value\n");
			push_type(code_gen_state, orig_lvalue, p);
			push_type(code_gen_state, copy_lvalue, p);
			push_type(code_gen_state, t, p);
			do_additive_expression(code_gen_state, p, MINUS_CHAR);
			do_assignment(code_gen_state, p);
			buffered_printf(code_gen_state->buffered_output,"sto SP r9;    Previous value\n");
			result_description = pop_type_without_type_check(code_gen_state, p); /*  Pop the base type */
			result_description->value_type = WORD_ALIGNED_RVALUE;
			push_type(code_gen_state, result_description, p);
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
	if(check_one_child(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p), AMPERSAND_CHAR)){
			struct type_description * found;
			struct type_description * address_type;
			found = pop_type_without_type_check(code_gen_state, p); /*  Pop the base type */

			assert(found->value_type == LVALUE);
			buffered_printf(code_gen_state->buffered_output,"; Perform & operator\n");

        		address_type = create_address_type_description_from_type_description(code_gen_state->memory_pool_collection, found);
        		address_type->value_type = WORD_ALIGNED_RVALUE;
			push_type(code_gen_state, address_type, p); /* Put back the pointer type */

			destroy_type_description(code_gen_state->memory_pool_collection, found);
		}else if(is_terminal_c_token_type(first_child(p), MULTIPLY_CHAR)){
			struct type_description * found_type = pop_type_without_type_check(code_gen_state, p);
			struct type_description * dereferenced_type = create_dereferenced_pointer_type_description_from_type_description(code_gen_state->memory_pool_collection, found_type);
			struct parser_node * abstract_declarator = (struct parser_node *)0;

			if(found_type->value_type == LVALUE){
				buffered_printf(code_gen_state->buffered_output,";                 Pointer Dereference\n");
				buffered_printf(code_gen_state->buffered_output,"loa r1 SP;        Copy value, which is an address\n");
				buffered_printf(code_gen_state->buffered_output,"loa r1 r1;        Load the address that it points to\n");
				buffered_printf(code_gen_state->buffered_output,"sto SP r1;     store address\n");
			}else{
				/*  If the pointer was an rvalue, we just need to convert it to an lvalue */
				dereferenced_type->value_type = LVALUE;
			}

			abstract_declarator = create_abstract_declarator_from_normalized_declarator(code_gen_state->memory_pool_collection, dereferenced_type->declarator);

			destroy_type_description(code_gen_state->memory_pool_collection, found_type);
			push_type(code_gen_state, dereferenced_type, p);
			destroy_parser_node_tree_and_c_lexer_tokens(code_gen_state->memory_pool_collection, abstract_declarator);
		}else if(is_terminal_c_token_type(first_child(p), PLUS_CHAR)){
			struct type_description * found_type = ensure_top_type_is_r_value(code_gen_state, p);
			buffered_printf(code_gen_state->buffered_output,";unary plus operator (Do Nothing).\n");
			push_type(code_gen_state, found_type, p); /* Put back the type */
		}else if(is_terminal_c_token_type(first_child(p), MINUS_CHAR)){
			struct type_description * found_type = ensure_top_type_is_r_value(code_gen_state, p);
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;  Copy value\n");
			buffered_printf(code_gen_state->buffered_output,"not r1 r1;  Ones complement\n");
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x1;\n");
			buffered_printf(code_gen_state->buffered_output,"add r1 r1 r2; Twos complement\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;  Save\n");
			push_type(code_gen_state, found_type, p); /* Put back the type */
		}else if(is_terminal_c_token_type(first_child(p), TILDE_CHAR)){
			struct type_description * found_type = ensure_top_type_is_r_value(code_gen_state, p);
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;  Copy value\n");
			buffered_printf(code_gen_state->buffered_output,"not r1 r1;  logical not\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;  Save value\n");
			push_type(code_gen_state, found_type, p); /* Put back the type */
		}else if(is_terminal_c_token_type(first_child(p), EXCLAMATION_MARK_CHAR)){
			struct type_description * found_type = ensure_top_type_is_r_value(code_gen_state, p);
			buffered_printf(code_gen_state->buffered_output,"loa r1 SP;  Copy value\n");
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x1; Assume false and invert to 1\n");
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR 1; If it is zero\n");
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x0; It was not 0, inverse is 0\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r2;  Save value\n");
			push_type(code_gen_state, found_type, p); /* Put back the type */
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
			struct type_description * sized_type;
			struct type_description * result_type = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, create_empty_type_description(code_gen_state->memory_pool_collection), UNSIGNED), INT);
			unsigned int size;
			sized_type = create_type_description_from_type_name(code_gen_state->memory_pool_collection, code_gen_state->parser_state, third_child(p));
			convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, sized_type);
			size = type_size(code_gen_state, sized_type, MINIMAL_RVALUE, 0, get_current_scope_level(code_gen_state));
			buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;\n", size);
			buffered_printf(code_gen_state->buffered_output,"sto SP r1;\n");
			result_type->value_type = WORD_ALIGNED_RVALUE;
			result_type->source_scope_level = get_current_scope_level(code_gen_state);
			push_type(code_gen_state, result_type, p);
			destroy_type_description(code_gen_state->memory_pool_collection, sized_type);
		}else{
			assert(0 &&"Expected sizeof type.\n");
		}
	}else if(check_two_children(p, UNARY_OPERATOR, CAST_EXPRESSION)){
		g_cast_expression(second_child(p), code_gen_state);
		g_unary_operator(first_child(p), code_gen_state);
	}else if(check_two_children(p, TERMINAL, UNARY_EXPRESSION)){
		if(is_terminal_c_token_type(first_child(p), INC_OP)){
			struct type_description * found_type;
			g_unary_expression(second_child(p), code_gen_state);
			found_type = pop_type_without_type_check(code_gen_state, p);
			assert(found_type->value_type == LVALUE);
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x1; Value to dec by\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Load item lvalue\n");
			buffered_printf(code_gen_state->buffered_output,"loa r3 r2; Load original value\n");
			buffered_printf(code_gen_state->buffered_output,"add r4 r3 r1; Increase that val by 1\n");
			buffered_printf(code_gen_state->buffered_output,"sto r2 r4; Store the original value at origin\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r2; Result is lvalue\n");
			push_type(code_gen_state, found_type, p); /* Put back the type */
		}else if(is_terminal_c_token_type(first_child(p), DEC_OP)){
			struct type_description * found_type;
			g_unary_expression(second_child(p), code_gen_state);
			found_type = pop_type_without_type_check(code_gen_state, p);
			assert(found_type->value_type == LVALUE);
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x1; Value to dec by\n");
			buffered_printf(code_gen_state->buffered_output,"loa r2 SP; Load item lvalue\n");
			buffered_printf(code_gen_state->buffered_output,"loa r3 r2; Load original value\n");
			buffered_printf(code_gen_state->buffered_output,"sub r4 r3 r1; Decrement that val by 1\n");
			buffered_printf(code_gen_state->buffered_output,"sto r2 r4; Store the original value at origin\n");
			buffered_printf(code_gen_state->buffered_output,"sto SP r2; Result is lvalue\n");
			push_type(code_gen_state, found_type, p); /* Put back the type */
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
			struct type_description * old_type;
			struct type_description * new_type;
			unsigned int is_new_type_integral;
			enum type_class old_type_class;
			new_type = create_type_description_from_type_name(code_gen_state->memory_pool_collection, code_gen_state->parser_state, second_child(p));
			convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, new_type);
			is_new_type_integral = is_integral_type(code_gen_state->memory_pool_collection, new_type);

			g_cast_expression(fourth_child(p), code_gen_state);

			old_type = pop_type_without_type_check(code_gen_state, p);
			convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, old_type);

			if(decay_to_pointer_if_array(code_gen_state, &old_type)){
				old_type->value_type = WORD_ALIGNED_RVALUE; /*  Array lvalue becomes a pointer rvalue */
			}
			convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, old_type);
			old_type_class = determine_type_class(code_gen_state->memory_pool_collection, old_type);
			if(is_new_type_integral && old_type_class == TYPE_CLASS_POINTER){
				/*  TODO: Check if this is actually a special case */
				struct type_description * dereferenced = create_dereferenced_pointer_type_description_from_type_description(code_gen_state->memory_pool_collection, old_type);
				convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, dereferenced);
				destroy_type_description(code_gen_state->memory_pool_collection, dereferenced);
			}

			buffered_printf(code_gen_state->buffered_output,";  Performing an explicit type cast.\n");
			new_type->value_type = old_type->value_type;
			push_type(code_gen_state, new_type, p);
			destroy_type_description(code_gen_state->memory_pool_collection, old_type);
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unsupported cast expression.\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported cast expression.\n");
	}
}

void pop(struct code_gen_state * code_gen_state, const char * r, enum value_type value_type){
	(void)value_type;
	buffered_printf(code_gen_state->buffered_output, "loa %s SP;\n", r); /* Load the value into register */
	buffered_printf(code_gen_state->buffered_output, "add SP SP WR;\n"); /* Pop value */
}

void push(struct code_gen_state * code_gen_state, const char * r, enum value_type value_type){
	(void)value_type;
	buffered_printf(code_gen_state->buffered_output, "sub SP SP WR;\n"); /*  Point to value  */
	buffered_printf(code_gen_state->buffered_output, "sto SP %s;\n", r); /*  Assign value  */
}

void do_signed_operation_function_call(struct code_gen_state * code_gen_state, const char * function_name){
	push(code_gen_state, "ZR", WORD_ALIGNED_RVALUE); /*  Space for the integer return value */
	/* Start setting up a stack frame */
	buffered_printf(code_gen_state->buffered_output,"ll r3 %s;\n", function_name);
	buffered_printf(code_gen_state->buffered_output,"sub r3 r3 PC;   Diff from current SP to call point\n");
	buffered_printf(code_gen_state->buffered_output,"ll r4 0x40;     Constant offset from above evaluated PC value to return.\n");
	buffered_printf(code_gen_state->buffered_output,"sub r3 r3 r4;   Subtract offset\n");
	buffered_printf(code_gen_state->buffered_output,"ll r4 0x30;     Offset from evaluated PC to return point\n");
	buffered_printf(code_gen_state->buffered_output,"add r4 r4 PC;\n");
	buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;   Push another 4 bytes for return address.\n");
	buffered_printf(code_gen_state->buffered_output,"sto SP r4;      Store the return address.\n");
	buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;   Push another 4 bytes for the frame pointer.\n");
	buffered_printf(code_gen_state->buffered_output,"sto SP FP;      Put the frame pointer on the stack\n");
	/* Load arguments */
	buffered_printf(code_gen_state->buffered_output, "sub SP SP WR;\n"); /*  Point to value  */
	buffered_printf(code_gen_state->buffered_output, "sto SP r1;\n");    /*  Assign value  */
	buffered_printf(code_gen_state->buffered_output, "sub SP SP WR;\n"); /*  Point to value  */
	buffered_printf(code_gen_state->buffered_output, "sto SP r2;\n");    /*  Assign value  */

	buffered_printf(code_gen_state->buffered_output,"add FP SP ZR;   Copy the stack pointer into the frame pointer.\n");
	buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;     But need to add the bytes arguments\n", 8);
	buffered_printf(code_gen_state->buffered_output,"add FP FP r1;   Because we just incremented the SP by that much.\n");
	buffered_printf(code_gen_state->buffered_output,"add PC PC r3;\n");
	require_external_symbol(code_gen_state->memory_pool_collection, &code_gen_state->symbols, (unsigned char *)function_name);
}

void do_multiplicative_expression(struct code_gen_state * code_gen_state, struct parser_node * p, enum c_token_type type){
	unsigned int iz_signed;
	struct type_description * t;
	ensure_top_values_are_rvalues(code_gen_state, p);
	t = usual_arithmetic_conversion(code_gen_state, p);
	iz_signed = is_signed(t);

	pop(code_gen_state, "r2", t->value_type); /* Arg 2 */
	pop(code_gen_state, "r1", t->value_type); /* Arg 1 */

	switch(type){
		case MULTIPLY_CHAR:{
			buffered_printf(code_gen_state->buffered_output,"mul r1 r1 r2;\n");
			push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
			break;
		}case DIVIDE_CHAR:{
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_division");
			}else{
				buffered_printf(code_gen_state->buffered_output,"div r1 r1 r2;\n");
				push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
			}
			break;
		}case PERCENT_CHAR:{
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_modulo");
			}else{
				buffered_printf(code_gen_state->buffered_output,"div r3 r1 r2;\n");
				buffered_printf(code_gen_state->buffered_output,"mul r3 r3 r2;\n");
				buffered_printf(code_gen_state->buffered_output,"sub r1 r1 r3;\n");
				push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
			}
			break;
		}default:{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in multiplicative_expression_rest.\n");
		}
	}
	push_type(code_gen_state, t, p);
}

void g_multiplicative_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, CAST_EXPRESSION, MULTIPLICATIVE_EXPRESSION_REST)){
		do_multiplicative_expression(code_gen_state, p, p->first_child->c_lexer_token->type);

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

	enum type_class c1 = determine_type_class(code_gen_state->memory_pool_collection, t1);
	enum type_class c2 = determine_type_class(code_gen_state->memory_pool_collection, t2);
	unsigned int is_integral_type1 = is_integral_type(code_gen_state->memory_pool_collection, t1);
	unsigned int is_integral_type2 = is_integral_type(code_gen_state->memory_pool_collection, t2);

	if(c1 == TYPE_CLASS_POINTER || c2 == TYPE_CLASS_POINTER){
		push_type(code_gen_state, t2, context);
		push_type(code_gen_state, t1, context);
		t1 = ensure_top_type_is_r_value(code_gen_state, context);
		pop(code_gen_state, "r1", t1->value_type);
		t2 = ensure_top_type_is_r_value(code_gen_state, context);
		pop(code_gen_state, "r2", t2->value_type);

		if(c1 == TYPE_CLASS_POINTER && is_integral_type2){
			struct type_description * dereferenced = create_dereferenced_pointer_type_description_from_type_description(code_gen_state->memory_pool_collection, t1);
			unsigned int size = type_size(code_gen_state, dereferenced, MINIMAL_RVALUE, 0, get_current_scope_level(code_gen_state));
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x%X; Load the arr element size\n", size);
			buffered_printf(code_gen_state->buffered_output,"mul r2 r2 r3; Multiply by amount to add or sub\n");
			push(code_gen_state, "r2", WORD_ALIGNED_RVALUE);
			push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
			destroy_type_description(code_gen_state->memory_pool_collection, dereferenced);
			destroy_type_description(code_gen_state->memory_pool_collection, t2);
			return t1;
		}else if(c2 == TYPE_CLASS_POINTER && is_integral_type1){
			struct type_description * dereferenced = create_dereferenced_pointer_type_description_from_type_description(code_gen_state->memory_pool_collection, t2);
			unsigned int size = type_size(code_gen_state, dereferenced, MINIMAL_RVALUE, 0, get_current_scope_level(code_gen_state));
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x%X; Load the arr element size\n", size);
			buffered_printf(code_gen_state->buffered_output,"mul r1 r1 r3; Multiply by amount to add or sub\n");
			push(code_gen_state, "r2", WORD_ALIGNED_RVALUE);
			push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
			destroy_type_description(code_gen_state->memory_pool_collection, dereferenced);
			destroy_type_description(code_gen_state->memory_pool_collection, t1);
			return t2;
		}else if(c1 == TYPE_CLASS_POINTER && c2 == TYPE_CLASS_POINTER){
			push(code_gen_state, "r2", WORD_ALIGNED_RVALUE);
			push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
			destroy_type_description(code_gen_state->memory_pool_collection, t1);
			return t2;
		}
	}
	/*  Neither one is a pointer */
	push_type(code_gen_state, t2, context);
	push_type(code_gen_state, t1, context);
	return (struct type_description *)0;
}

void do_additive_expression(struct code_gen_state * code_gen_state, struct parser_node * p, enum c_token_type type){
	struct type_description * t;
	enum type_class type_class;
	ensure_top_values_are_rvalues(code_gen_state, p);
	t = perform_pointer_conversion(code_gen_state, p);
	if(!t){
		t = usual_arithmetic_conversion(code_gen_state, p);
	}
	type_class = determine_type_class(code_gen_state->memory_pool_collection, t);
	pop(code_gen_state, "r2", t->value_type);
	pop(code_gen_state, "r1", t->value_type);
	switch(type){
		case PLUS_CHAR:{
			buffered_printf(code_gen_state->buffered_output,"add r1 r1 r2;\n"); /* Addition */
			break;
		}case MINUS_CHAR:{
			buffered_printf(code_gen_state->buffered_output,"sub r1 r1 r2;\n"); /* Subtraction */
			break;
		}default:{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in multiplicative_expression_rest.\n");
		}
	}
	if(type == MINUS_CHAR && type_class == TYPE_CLASS_POINTER){
		/*  Pointer subtraction returns the number of elements between the pointers */
		struct type_description * dereferenced = create_dereferenced_pointer_type_description_from_type_description(code_gen_state->memory_pool_collection, t);
		unsigned int unit_size;
		struct type_description * result_type;
		result_type = add_specifier(code_gen_state->memory_pool_collection, add_specifier(code_gen_state->memory_pool_collection, create_empty_type_description(code_gen_state->memory_pool_collection), UNSIGNED), INT);
		result_type->value_type = WORD_ALIGNED_RVALUE;
		result_type->context = t->context;
		result_type->source_element = t->source_element;
		result_type->source_scope_level = t->source_scope_level;
		convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, dereferenced);
		unit_size = type_size(code_gen_state, dereferenced, MINIMAL_RVALUE, 0, dereferenced->source_scope_level);
		destroy_type_description(code_gen_state->memory_pool_collection, dereferenced);
		destroy_type_description(code_gen_state->memory_pool_collection, t);
		buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X; Unit size\n", unit_size);
		buffered_printf(code_gen_state->buffered_output,"div r1 r1 r2;\n");
		push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
		push_type(code_gen_state, result_type, p);
	}else{
		push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
		push_type(code_gen_state, t, p);
	}
}

void g_additive_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, MULTIPLICATIVE_EXPRESSION, ADDITIVE_EXPRESSION_REST)){
		do_additive_expression(code_gen_state, p, p->first_child->c_lexer_token->type);
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

void do_xor_expression(struct code_gen_state * code_gen_state, struct parser_node * p, enum c_token_type type){
	struct type_description * t;
	ensure_top_values_are_rvalues(code_gen_state, p);
	t = perform_pointer_conversion(code_gen_state, p);
	if(!t){
		t = usual_arithmetic_conversion(code_gen_state, p);
	}
	pop(code_gen_state, "r2", t->value_type);
	pop(code_gen_state, "r1", t->value_type);
	switch(type){
		case CARET_CHAR:{
			buffered_printf(code_gen_state->buffered_output,"or r3 r1 r2;   Bitwise or\n");
			buffered_printf(code_gen_state->buffered_output,"and r5 r1 r2;  Bitwise and\n");
			buffered_printf(code_gen_state->buffered_output,"not r5 r5;     Not both\n");
			buffered_printf(code_gen_state->buffered_output,"and r1 r3 r5;  One of them, and not both\n");
			break;
		}default:{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in xor expression.\n");
		}
	}
	push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
	push_type(code_gen_state, t, p);
}

void do_or_expression(struct code_gen_state * code_gen_state, struct parser_node * p, enum c_token_type type){
	struct type_description * t;
	ensure_top_values_are_rvalues(code_gen_state, p);
	t = perform_pointer_conversion(code_gen_state, p);
	if(!t){
		t = usual_arithmetic_conversion(code_gen_state, p);
	}
	pop(code_gen_state, "r2", t->value_type);
	pop(code_gen_state, "r1", t->value_type);
	switch(type){
		case PIPE_CHAR:{
			buffered_printf(code_gen_state->buffered_output,"or r1 r1 r2;\n"); /* Bitwise or */
			break;
		}default:{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in or expression.\n");
		}
	}
	push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
	push_type(code_gen_state, t, p);
}

void do_and_expression(struct code_gen_state * code_gen_state, struct parser_node * p, enum c_token_type type){
	struct type_description * t;
	ensure_top_values_are_rvalues(code_gen_state, p);
	t = perform_pointer_conversion(code_gen_state, p);
	if(!t){
		t = usual_arithmetic_conversion(code_gen_state, p);
	}
	pop(code_gen_state, "r2", t->value_type);
	pop(code_gen_state, "r1", t->value_type);
	switch(type){
		case AMPERSAND_CHAR:{
			buffered_printf(code_gen_state->buffered_output,"and r1 r1 r2;\n"); /* Bitwise and */
			break;
		}default:{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in and expression.\n");
		}
	}
	push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
	push_type(code_gen_state, t, p);
}

void do_shift_expression(struct code_gen_state * code_gen_state, struct parser_node * p, enum c_token_type type){
	struct type_description * t;
	ensure_top_values_are_rvalues(code_gen_state, p);
	t = perform_pointer_conversion(code_gen_state, p);
	if(!t){
		t = usual_arithmetic_conversion(code_gen_state, p);
	}
	pop(code_gen_state, "r2", t->value_type);
	pop(code_gen_state, "r1", t->value_type);
	switch(type){
		case LEFT_OP:{
			buffered_printf(code_gen_state->buffered_output,"shl r1 r2;\n");
			break;
		}case RIGHT_OP:{
			buffered_printf(code_gen_state->buffered_output,"shr r1 r2;\n");
			break;
		}default:{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in shift expression.\n");
		}
	}
	push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
	push_type(code_gen_state, t, p);
}

void g_shift_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, ADDITIVE_EXPRESSION, SHIFT_EXPRESSION_REST)){
		do_shift_expression(code_gen_state, p, first_child(p)->c_lexer_token->type);
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
		unsigned int iz_signed;
		struct type_description * t;
		ensure_top_values_are_rvalues(code_gen_state, p);
		t = usual_arithmetic_conversion(code_gen_state, p);
		iz_signed = is_signed(t);
		pop(code_gen_state, "r2", t->value_type);
		pop(code_gen_state, "r1", t->value_type);
		if(is_terminal_c_token_type(first_child(p),OPEN_ANGLE_BRACKET_CHAR)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_less_than");
			}else{
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x1; Assume r1 < r2\n");
				buffered_printf(code_gen_state->buffered_output,"blt r1 r2 1;\n");
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n");
				push(code_gen_state, "r3", WORD_ALIGNED_RVALUE);
			}
		}else if(is_terminal_c_token_type(first_child(p),CLOSE_ANGLE_BRACKET_CHAR)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_greater_than");
			}else{
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x0; Assume r1 > r2 is not true\n");
				buffered_printf(code_gen_state->buffered_output,"blt r1 r2 2;\n");
				buffered_printf(code_gen_state->buffered_output,"beq r1 r2 1;\n");
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n");
				push(code_gen_state, "r3", WORD_ALIGNED_RVALUE);
			}
		}else if(is_terminal_c_token_type(first_child(p),LE_OP)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_less_than_or_equal_to");
			}else{
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x1; Assume r1 <= r2 is true\n");
				buffered_printf(code_gen_state->buffered_output,"blt r1 r2 2;\n");
				buffered_printf(code_gen_state->buffered_output,"beq r1 r2 1;\n");
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n");
				push(code_gen_state, "r3", WORD_ALIGNED_RVALUE);
			}
		}else if(is_terminal_c_token_type(first_child(p),GE_OP)){
			if(iz_signed){
				do_signed_operation_function_call(code_gen_state, "signed_greater_than_or_equal_to");
			}else{
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x0; Assume r1 >= r2 is not true\n");
				buffered_printf(code_gen_state->buffered_output,"blt r1 r2 1;\n");
				buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n");
				push(code_gen_state, "r3", WORD_ALIGNED_RVALUE);
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
		struct type_description * t;
		ensure_top_values_are_rvalues(code_gen_state, p);
		t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2", t->value_type);
		pop(code_gen_state, "r1", t->value_type);
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
		push(code_gen_state, "r3", WORD_ALIGNED_RVALUE);
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
		do_and_expression(code_gen_state, p, first_child(p)->c_lexer_token->type);
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
		do_xor_expression(code_gen_state, p, first_child(p)->c_lexer_token->type);

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
		do_or_expression(code_gen_state, p, first_child(p)->c_lexer_token->type);
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
		struct type_description * t;
		ensure_top_values_are_rvalues(code_gen_state, p);
		t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2", t->value_type);
		pop(code_gen_state, "r1", t->value_type);
		if(is_terminal_c_token_type(first_child(p),AND_OP)){
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n"); /* Assume false */
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR 2;\n");
			buffered_printf(code_gen_state->buffered_output,"beq r2 ZR 1;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n"); /* Didn't branch, both must have been true */
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in logical_and_expression_rest.\n");
		}
		push(code_gen_state, "r3", WORD_ALIGNED_RVALUE);
		push_type(code_gen_state, t, p);

		g_rest_of_logical_and(third_child(p), code_gen_state);
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported logical_and_expression_rest.\n");
	}
}

void g_rest_of_logical_and(struct parser_node * possible_non_empty_rest, struct code_gen_state * code_gen_state){
	if(is_second_child_type(possible_non_empty_rest, INCLUSIVE_OR_EXPRESSION)){
		struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
		unsigned int cond_index = code_gen_state->condition_index;
		unsigned char * dontshort_str = create_formatted_string(m, 20, "_%ddsc", cond_index);
		unsigned char * afterand_str = create_formatted_string(m, 20, "_%daa", cond_index);
		struct type_description * top_type;
		top_type = ensure_top_type_is_r_value(code_gen_state, possible_non_empty_rest);
		push_type(code_gen_state, top_type, possible_non_empty_rest);
		/*  Short circuit evaluation */
		code_gen_state->condition_index = code_gen_state->condition_index + 1;
		pop(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
		buffered_printf(code_gen_state->buffered_output,"beq ZR r1 1;\n");
		buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;\n", dontshort_str);
		buffered_printf(code_gen_state->buffered_output,"add r1 ZR ZR;  Load value 0\n");
		push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
		buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;\n", afterand_str);
		buffered_printf(code_gen_state->buffered_output,"%s:\n", dontshort_str);
		push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
		g_inclusive_or_expression(second_child(possible_non_empty_rest), code_gen_state);
		g_logical_and_expression_rest(possible_non_empty_rest, code_gen_state);
		buffered_printf(code_gen_state->buffered_output,"%s:\n", afterand_str);
		require_internal_symbol(m, &code_gen_state->symbols, dontshort_str);
		implement_internal_symbol(m, &code_gen_state->symbols, dontshort_str);
		require_internal_symbol(m, &code_gen_state->symbols, afterand_str);
		implement_internal_symbol(m, &code_gen_state->symbols, afterand_str);
		heap_memory_pool_free(m->heap_pool, dontshort_str);
		heap_memory_pool_free(m->heap_pool, afterand_str);
	}
}

void g_logical_and_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, INCLUSIVE_OR_EXPRESSION, LOGICAL_AND_EXPRESSION_REST)){
		g_inclusive_or_expression(first_child(p), code_gen_state);
		g_rest_of_logical_and(second_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported logical_and_expression.\n");
	}
}

void g_logical_or_expression_rest(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, LOGICAL_AND_EXPRESSION, LOGICAL_OR_EXPRESSION_REST)){
		struct type_description * t;
		ensure_top_values_are_rvalues(code_gen_state, p);
		t = usual_arithmetic_conversion(code_gen_state, p);
		pop(code_gen_state, "r2", t->value_type);
		pop(code_gen_state, "r1", t->value_type);
		if(is_terminal_c_token_type(first_child(p),OR_OP)){
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x0;\n"); /* Assume false */
			buffered_printf(code_gen_state->buffered_output,"or r1 r1 r2;\n"); /* Put all the one bits in one variable */
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR 1;\n");
			buffered_printf(code_gen_state->buffered_output,"ll r3 0x1;\n");
		}else{
			buffered_printf(code_gen_state->buffered_output,"Unknown operator in logical_or_expression_rest.\n");
		}
		push(code_gen_state, "r3", WORD_ALIGNED_RVALUE);
		push_type(code_gen_state, t, p);

		g_rest_of_logical_or(third_child(p), code_gen_state);
	}else if(is_second_child_type(p,EPSILON)){
		/*  Nothing for epsilon */
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported logical_or_expression_rest.\n");
	}
}

void g_rest_of_logical_or(struct parser_node * possible_non_empty_rest, struct code_gen_state * code_gen_state){
	if(is_second_child_type(possible_non_empty_rest, LOGICAL_AND_EXPRESSION)){
		struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
		unsigned int cond_index = code_gen_state->condition_index;
		unsigned char * dontshort_str = create_formatted_string(m, 20, "_%ddsc", cond_index);
		unsigned char * afteror_str = create_formatted_string(m, 20, "_%dao", cond_index);
		struct type_description * top_type;
		top_type = ensure_top_type_is_r_value(code_gen_state, possible_non_empty_rest);
		push_type(code_gen_state, top_type, possible_non_empty_rest);
		/*  Short circuit evaluation */
		code_gen_state->condition_index = code_gen_state->condition_index + 1;
		pop(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
		buffered_printf(code_gen_state->buffered_output,"beq ZR r1 %s;\n", dontshort_str);
		buffered_printf(code_gen_state->buffered_output,"div r1 r1 r1;  Load value 1\n");
		push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
		buffered_printf(code_gen_state->buffered_output,"beq ZR ZR %s;\n", afteror_str);
		buffered_printf(code_gen_state->buffered_output,"%s:\n", dontshort_str);
		push(code_gen_state, "r1", WORD_ALIGNED_RVALUE);
		g_logical_and_expression(second_child(possible_non_empty_rest), code_gen_state);
		g_logical_or_expression_rest(possible_non_empty_rest, code_gen_state);
		buffered_printf(code_gen_state->buffered_output,"%s:\n", afteror_str);
		require_internal_symbol(m, &code_gen_state->symbols, dontshort_str);
		implement_internal_symbol(m, &code_gen_state->symbols, dontshort_str);
		require_internal_symbol(m, &code_gen_state->symbols, afteror_str);
		implement_internal_symbol(m, &code_gen_state->symbols, afteror_str);
		heap_memory_pool_free(m->heap_pool, dontshort_str);
		heap_memory_pool_free(m->heap_pool, afteror_str);
	}
}

void g_logical_or_expression(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, LOGICAL_AND_EXPRESSION, LOGICAL_OR_EXPRESSION_REST)){
		g_logical_and_expression(first_child(p), code_gen_state);
		g_rest_of_logical_or(second_child(p), code_gen_state);
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
			struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * false_condition_str = create_formatted_string(m, 20, "_%dfc", cond_index);
			unsigned char * after_condition_str = create_formatted_string(m, 20, "_%dac", cond_index);
			struct type_description * t1;
			struct type_description * t2;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			g_logical_or_expression(first_child(p), code_gen_state);
			/*  Pop the item that was loaded in the conditional */
			t1 = ensure_top_type_is_r_value(code_gen_state, first_child(p));
			push_type(code_gen_state, t1, first_child(p));
			t1 = consume_scalar_type(code_gen_state, first_child(p));
			pop(code_gen_state, "r1", t1->value_type);
			destroy_type_description(m, t1);
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s;\n", false_condition_str);
			g_expression(third_child(p), code_gen_state);
			t1 = ensure_top_type_is_r_value(code_gen_state, p);
			buffered_printf(code_gen_state->buffered_output,"loa PC PC;\n");
			buffered_printf(code_gen_state->buffered_output,"dw %s;\n", after_condition_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", false_condition_str);
			g_conditional_expression(fifth_child(p), code_gen_state);
			t2 = ensure_top_type_is_r_value(code_gen_state, p);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", after_condition_str);
			require_internal_symbol(m, &code_gen_state->symbols, false_condition_str);
			implement_internal_symbol(m, &code_gen_state->symbols, false_condition_str);
			require_internal_symbol(m, &code_gen_state->symbols, after_condition_str);
			implement_internal_symbol(m, &code_gen_state->symbols, after_condition_str);
			heap_memory_pool_free(m->heap_pool, after_condition_str);
			heap_memory_pool_free(m->heap_pool, false_condition_str);
			if(!type_description_cmp(m, t1, t2)){
				push_type(code_gen_state, t1, p);
        			destroy_type_description(m, t2);
			}else{
				print_node_context(code_gen_state->parser_state->c_lexer_state, p);
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
	struct type_description * top = pop_type_without_type_check(code_gen_state, p);
	struct type_description * bottom = pop_type_without_type_check(code_gen_state, p);
	unsigned int top_current_size = type_size(code_gen_state, top, top->value_type, 1, top->source_scope_level);

	push_type(code_gen_state, bottom, p);
	if(is_terminal_c_token_type(first_child(p),EQUALS_CHAR)){
		/*  nothing to do for simple assignment case */
		push_type(code_gen_state, top, p);
		return;
	}

	/*  Insert a copy of the lvaue we're assigning to */
	buffered_printf(code_gen_state->buffered_output,"sub SP SP WR;     Create space for the lvalue\n");
	buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;       Bytes in top value\n", top_current_size);
	buffered_printf(code_gen_state->buffered_output,"add r2 SP r1;     Now pointing at last word of rvalue\n");
	buffered_printf(code_gen_state->buffered_output,"sub r3 r2 WR;     r3 now has bottom word of destination\n");
	/*  Move top value up one word */
	copy_words(code_gen_state, "r2", "r3", "r4", top_current_size);
	buffered_printf(code_gen_state->buffered_output,"add r2 SP r1;     Now pointing at last word of where rvalue was\n");
	buffered_printf(code_gen_state->buffered_output,"add r3 r2 WR;     Now pointing to lvalue\n");
	buffered_printf(code_gen_state->buffered_output,"loa r4 r3;        Load lvalue\n");
	buffered_printf(code_gen_state->buffered_output,"sto r2 r4;        Store lvalue\n");

	push_type(code_gen_state, copy_type_description(code_gen_state->memory_pool_collection, bottom), p);
	push_type(code_gen_state, top, p);
	if(check_one_child(p, TERMINAL)){
		if(is_terminal_c_token_type(first_child(p),EQUALS_CHAR)){
			/*  Do nothing */
		}else if(is_terminal_c_token_type(first_child(p),MUL_ASSIGN)){
			do_multiplicative_expression(code_gen_state, p, MULTIPLY_CHAR);
		}else if(is_terminal_c_token_type(first_child(p),DIV_ASSIGN)){
			do_multiplicative_expression(code_gen_state, p, DIVIDE_CHAR);
		}else if(is_terminal_c_token_type(first_child(p),MOD_ASSIGN)){
			do_multiplicative_expression(code_gen_state, p, PERCENT_CHAR);
		}else if(is_terminal_c_token_type(first_child(p),ADD_ASSIGN)){
			do_additive_expression(code_gen_state, p, PLUS_CHAR);
		}else if(is_terminal_c_token_type(first_child(p),SUB_ASSIGN)){
			do_additive_expression(code_gen_state, p, MINUS_CHAR);
		}else if(is_terminal_c_token_type(first_child(p),LEFT_ASSIGN)){
			do_shift_expression(code_gen_state, p, LEFT_OP);
		}else if(is_terminal_c_token_type(first_child(p),RIGHT_ASSIGN)){
			do_shift_expression(code_gen_state, p, RIGHT_OP);
		}else if(is_terminal_c_token_type(first_child(p),AND_ASSIGN)){
			do_and_expression(code_gen_state, p, AMPERSAND_CHAR);
		}else if(is_terminal_c_token_type(first_child(p),XOR_ASSIGN)){
			do_xor_expression(code_gen_state, p, CARET_CHAR);
		}else if(is_terminal_c_token_type(first_child(p),OR_ASSIGN)){
			do_or_expression(code_gen_state, p, PIPE_CHAR);
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
			assert(0 && "Semicolon not found.\n");
        	}
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported expression_statement.\n");
	}
}

void g_statement(struct parser_node * p, struct code_gen_state * code_gen_state, unsigned char * evaluate_label, unsigned char * end_label){
	if(is_first_child_type(p, LABELED_STATEMENT)){
		g_labeled_statement(first_child(p), code_gen_state);
	}else if(is_first_child_type(p, COMPOUND_STATEMENT)){
		g_compound_statement(first_child(p), code_gen_state, (struct parser_node *)0, evaluate_label, end_label);
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
		g_statement(first_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
		buffered_printf(code_gen_state->buffered_output,";--- End statement---.\n");
		g_statement_list_rest(second_child(p), code_gen_state);
	}else{
		buffered_printf(code_gen_state->buffered_output,"Unsupported statement list rest.\n");
	}
}

void g_statement_list(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, STATEMENT, STATEMENT_LIST_REST)){
		buffered_printf(code_gen_state->buffered_output,";--- Start statement---.\n");
		g_statement(first_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
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
	unsigned_int_list_add_end(&code_gen_state->scope_index_list, code_gen_state->next_scope_index);
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
	code_gen_state->next_scope_index = 1 + unsigned_int_list_pop_end(&code_gen_state->scope_index_list);
	if(unsigned_int_list_size(&code_gen_state->scope_index_list) == 0){
		code_gen_state->current_function = (struct namespace_object *)0; /*  We're not currently inside any function */
	}
}

void create_default_return_value(struct code_gen_state * code_gen_state, struct parser_node * possible_function, struct parser_node * context){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&code_gen_state->current_function->elements);
	struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&code_gen_state->current_function->elements, num_elements - 1);
	struct type_description * t = create_type_description_from_normalized_declaration_element(code_gen_state->memory_pool_collection, element, context, get_current_scope_level(code_gen_state), WORD_ALIGNED_RVALUE);
	struct type_description * return_type_description = get_current_function_return_type_description(code_gen_state->memory_pool_collection, t);
	unsigned int rtn_val_size;
	unsigned int num_words;
	unsigned int i;
        (void)possible_function;
	convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, return_type_description);
	rtn_val_size = type_size(code_gen_state, return_type_description, return_type_description->value_type, 1, get_current_scope_level(code_gen_state));
	num_words = rtn_val_size / 4;
	for(i = 0; i < num_words; i++){
		buffered_printf(code_gen_state->buffered_output,"sub SP SP WR; Creating default rtn value, word %d.\n", i);
	}
        destroy_type_description(code_gen_state->memory_pool_collection, t);

	push_type(code_gen_state, copy_type_description(code_gen_state->memory_pool_collection, return_type_description), context);
	destroy_type_description(code_gen_state->memory_pool_collection, return_type_description);
}

void return_from_function(struct code_gen_state * code_gen_state, struct parser_node * context){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&code_gen_state->current_function->elements);
	struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&code_gen_state->current_function->elements, num_elements - 1);
	struct type_description * t = create_type_description_from_normalized_declaration_element(code_gen_state->memory_pool_collection, element, context, get_current_scope_level(code_gen_state), WORD_ALIGNED_RVALUE);
	struct type_description * return_type_description = get_current_function_return_type_description(code_gen_state->memory_pool_collection, t);
	unsigned int rtn_val_size;
	destroy_type_description(code_gen_state->memory_pool_collection, ensure_top_type_is_r_value(code_gen_state, context));
	convert_to_untypedefed_type_description(code_gen_state->memory_pool_collection, return_type_description);
	rtn_val_size = type_size(code_gen_state, return_type_description, return_type_description->value_type, 1, get_current_scope_level(code_gen_state));
        destroy_type_description(code_gen_state->memory_pool_collection, t);
        /*  SP should now be pointing to the top of the return value */
        buffered_printf(code_gen_state->buffered_output,"add r1 FP ZR;  r1 points to FP.\n");
        buffered_printf(code_gen_state->buffered_output,"add r1 r1 WR;  r1 points to rtn address\n");
        buffered_printf(code_gen_state->buffered_output,"add r1 r1 WR;  r1 points top of rtn space \n");
        buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;    Distance to last word of rtn space\n", rtn_val_size - 4);
        buffered_printf(code_gen_state->buffered_output,"add r1 r2 r1;  Destination\n");
        buffered_printf(code_gen_state->buffered_output,"add r5 r2 SP;  Source\n");
	copy_words(code_gen_state, "r5", "r1", "r2", rtn_val_size);
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

	destroy_type_description(code_gen_state->memory_pool_collection, return_type_description);
	/*  There should never be anything on the type stack after we've poped returned the final value */
	if(struct_type_description_ptr_list_size(&code_gen_state->type_stack) != 0){
		print_node_context(code_gen_state->parser_state->c_lexer_state, context);
		assert(0);
	}
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
				unsigned_char_ptr_list_add_end(&scope->evaluate_labels, evaluate_label);
			}
			if(end_label){
				unsigned_char_ptr_list_add_end(&scope->end_labels, end_label);
			}
			g_declaration_list(second_child(p), code_gen_state);
			g_statement_list(third_child(p), code_gen_state);
			do_default_function_return(code_gen_state, possible_function, fourth_child(p));
			if(evaluate_label){
				unsigned_char_ptr_list_pop_end(&scope->evaluate_labels);
			}
			if(end_label){
				unsigned_char_ptr_list_pop_end(&scope->end_labels);
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
				unsigned_char_ptr_list_add_end(&scope->evaluate_labels, evaluate_label);
			}
			if(end_label){
				unsigned_char_ptr_list_add_end(&scope->end_labels, end_label);
			}
			g_declaration_list(second_child(p), code_gen_state);
			do_default_function_return(code_gen_state, possible_function, third_child(p));
			if(evaluate_label){
				unsigned_char_ptr_list_pop_end(&scope->evaluate_labels);
			}
			if(end_label){
				unsigned_char_ptr_list_pop_end(&scope->end_labels);
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
				unsigned_char_ptr_list_add_end(&scope->evaluate_labels, evaluate_label);
			}
			if(end_label){
				unsigned_char_ptr_list_add_end(&scope->end_labels, end_label);
			}
			g_statement_list(second_child(p), code_gen_state);
			do_default_function_return(code_gen_state, possible_function, third_child(p));
			if(evaluate_label){
				unsigned_char_ptr_list_pop_end(&scope->evaluate_labels);
			}
			if(end_label){
				unsigned_char_ptr_list_pop_end(&scope->end_labels);
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
				unsigned_char_ptr_list_add_end(&scope->evaluate_labels, evaluate_label);
			}
			if(end_label){
				unsigned_char_ptr_list_add_end(&scope->end_labels, end_label);
			}
			do_default_function_return(code_gen_state, possible_function, second_child(p));
			if(evaluate_label){
				unsigned_char_ptr_list_pop_end(&scope->evaluate_labels);
			}
			if(end_label){
				unsigned_char_ptr_list_pop_end(&scope->end_labels);
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

void g_init_declarator_list_rest(struct parser_node * specifiers, struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, TERMINAL, INIT_DECLARATOR, INIT_DECLARATOR_LIST_REST)){
		if(is_terminal_c_token_type(first_child(p),COMMA_CHAR)){
			g_init_declarator(specifiers, second_child(p), code_gen_state);
			g_init_declarator_list_rest(specifiers, third_child(p), code_gen_state);
		}else{
			assert(0 &&"Expected COMMA_CHAR.\n");
		}
	}else if(check_one_child(p, EPSILON)){
		/*  Do nothing */
	}else{
		assert(0 &&"Expected init_declarator_list_rest.\n");
	}
}

void g_init_declarator_list(struct parser_node * specifiers, struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_two_children(p, INIT_DECLARATOR,INIT_DECLARATOR_LIST_REST)){
		g_init_declarator(specifiers, first_child(p), code_gen_state);
		g_init_declarator_list_rest(specifiers, second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected init_declarator_list.\n");
	}
}

void g_declaration(struct parser_node * p, struct code_gen_state * code_gen_state){
	if(check_three_children(p, DECLARATION_SPECIFIERS,INIT_DECLARATOR_LIST, TERMINAL)){
		if(is_terminal_c_token_type(third_child(p),SEMICOLON_CHAR)){
			g_declaration_specifiers(first_child(p), code_gen_state);
			g_init_declarator_list(first_child(p), second_child(p), code_gen_state);
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
			/*  In the future this might be necessary to calculate at run time for variables sized arrays */
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
	arity = obj_size / 4;
	assert((obj_size - (arity * 4)) == 0);
	for(i = 0; i < arity; i++){
		buffered_printf(code_gen_state->buffered_output,"%s SP SP WR; Room for local val\n", instruction);
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
			unsigned char * identifier = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte, code_gen_state->memory_pool_collection);
			load_identifier(code_gen_state, identifier, p);
			heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, identifier);
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

struct type_description * consume_scalar_type(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * observed_type = pop_type_without_type_check(code_gen_state, context);
	if(!is_scalar_type(code_gen_state->memory_pool_collection, observed_type)){
		print_error_with_type(code_gen_state->parser_state->c_lexer_state, observed_type, context, "This is not a scalar type:");
		assert(0 && "This is not a scalar type.\n");
	}
	return observed_type;
}

void convert_top_rvalue_to_target_type(struct code_gen_state * code_gen_state, struct type_description * rvalue_type, struct type_description * target_type){
	enum type_class tcrvalue = determine_type_class(code_gen_state->memory_pool_collection, rvalue_type);
	enum type_class tctarget = determine_type_class(code_gen_state->memory_pool_collection, target_type);
	switch(tctarget){
		case TYPE_CLASS_CHAR:{
			switch(tcrvalue){
				case TYPE_CLASS_CHAR:{
					/* No conversion needed */
					break;
				}case TYPE_CLASS_INT:{
        				buffered_printf(code_gen_state->buffered_output,"loa r1 SP;    \n");
        				buffered_printf(code_gen_state->buffered_output,"ll r2 0xFF;    \n");
        				buffered_printf(code_gen_state->buffered_output,"and r1 r1 r2;   Filter out higher bits \n");
        				buffered_printf(code_gen_state->buffered_output,"sto SP r1;    \n");
					break;
				}default:{
					print_error_with_types(code_gen_state->parser_state->c_lexer_state, rvalue_type, target_type, target_type->context, "Unsupported conversion to character:");
					assert(0);
				}
			}
			break;
		}default:{
			print_error_with_types(code_gen_state->parser_state->c_lexer_state, rvalue_type, target_type, target_type->context, "Unsupported conversion rule:");
			assert(0);
		}
	}
}

struct type_description * manage_assignment_type_change(struct code_gen_state * code_gen_state, struct parser_node * context){
	struct type_description * t1 = ensure_top_type_is_r_value(code_gen_state, context);
	struct type_description * t2 = pop_type_without_type_check(code_gen_state, context);
	if(t2->value_type != LVALUE){
		print_error_with_types(code_gen_state->parser_state->c_lexer_state, t1, t2, context, "Performing assignment with left operand as non lvalue.");
		assert(0);
	}

	if(is_arithmetic_type(code_gen_state->memory_pool_collection, t2)){
		enum type_class tc1 = determine_type_class(code_gen_state->memory_pool_collection, t1);
		enum type_class tc2 = determine_type_class(code_gen_state->memory_pool_collection, t2);
		if(tc1 == TYPE_CLASS_CHAR || tc1 == TYPE_CLASS_SHORT){
			perform_integral_promotion(code_gen_state, &t1, "SP");
		}
		if(tc2 == TYPE_CLASS_CHAR || tc2 == TYPE_CLASS_SHORT){
			convert_top_rvalue_to_target_type(code_gen_state, t1, t2);
			destroy_type_description(code_gen_state->memory_pool_collection, t1);
			return t2;
		}else{
			push_type(code_gen_state, t2, context); /*  Poped inside usual_arithmetic_conversion */
			push_type(code_gen_state, t1, context);
			return usual_arithmetic_conversion(code_gen_state, context);
		}
	}else if(determine_type_class(code_gen_state->memory_pool_collection, t2) == TYPE_CLASS_POINTER){
		unsigned int compare_successful;
		compare_successful = !type_description_cmp(code_gen_state->memory_pool_collection, t1, t2);
		if(compare_successful){
			destroy_type_description(code_gen_state->memory_pool_collection, t2);
			t1->value_type = LVALUE;
			return t1;
		}
	}else{
		unsigned int compare_successful = !type_description_cmp(code_gen_state->memory_pool_collection, t1, t2);
		if(compare_successful){
			destroy_type_description(code_gen_state->memory_pool_collection, t2);
			t1->value_type = LVALUE;
			return t1;
		}
	}
	print_error_with_types(code_gen_state->parser_state->c_lexer_state, t1, t2, context, "Unsupported type for assignment:");
	assert(0);
	return (struct type_description *)0;
}

void do_assignment(struct code_gen_state * code_gen_state, struct parser_node * context){
        /*  Stack top is thing we assign to, stack top -1 is thing to assign  */
        struct type_description * assign_type = manage_assignment_type_change(code_gen_state, context);
	unsigned int size = type_size(code_gen_state, assign_type, WORD_ALIGNED_RVALUE, 0, assign_type->source_scope_level);
	assign_type->value_type = LVALUE;
	push_type(code_gen_state, assign_type, context);

        if(determine_type_class(code_gen_state->memory_pool_collection, assign_type) == TYPE_CLASS_CHAR){
		buffered_printf(code_gen_state->buffered_output,"add r1 SP WR;     ptr to lvalue\n");
		buffered_printf(code_gen_state->buffered_output,"loa r1 r1;        load lvalue\n");
		buffered_printf(code_gen_state->buffered_output,"loa r2 SP;        load rvalue\n"); /*  Assume rvalue already has upper bits zeroed */
		buffered_printf(code_gen_state->buffered_output,"div r3 r1 WR;     stores address div 4\n");
		buffered_printf(code_gen_state->buffered_output,"mul r3 r3 WR;     4 byte aligned address\n");
		buffered_printf(code_gen_state->buffered_output,"loa r5 r3;        Character word\n");
		buffered_printf(code_gen_state->buffered_output,"sub r4 r1 r3;     mod amount\n");
		
		/*  Get the word with target byte zeroed out */
		buffered_printf(code_gen_state->buffered_output,"ll r6 0xFF;       Byte mask\n");
		buffered_printf(code_gen_state->buffered_output,"mul r7 WR r4;     Bytes to shift left\n");
		buffered_printf(code_gen_state->buffered_output,"shl r6 r7;        Shift mask to target byte\n");
		buffered_printf(code_gen_state->buffered_output,"shl r6 r7;        Shift mask to target byte\n");
		buffered_printf(code_gen_state->buffered_output,"not r6 r6;        Invert mask\n");
		buffered_printf(code_gen_state->buffered_output,"and r5 r5 r6;     Word without target byte\n");

		/*  Put byte into word  */
		buffered_printf(code_gen_state->buffered_output,"shl r2 r7;        Shift mask to target byte\n");
		buffered_printf(code_gen_state->buffered_output,"shl r2 r7;        Shift mask to target byte\n");
		buffered_printf(code_gen_state->buffered_output,"or r2 r2 r5;      Add byte to word\n");
		buffered_printf(code_gen_state->buffered_output,"sto r3 r2;        Store new word\n");
		buffered_printf(code_gen_state->buffered_output,"add SP SP WR;     Pop rvalue off stack\n");
        }else{
		/*  Expects the top of the stack to contain the rvalue to assign, and below that is an lvalue */
		buffered_printf(code_gen_state->buffered_output,";                 Begin Assignment\n");
		if(size != 4){
			buffered_printf(code_gen_state->buffered_output,"ll r1 0x%X;       r1 now stores num bytes in rvalue\n", size);
			buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;       r2 now stores num bytes to end of rvalue\n", (size -4));
		}

		if(size == 4){
			buffered_printf(code_gen_state->buffered_output,"add r3 SP WR;     now stores the address of lvalue on stack\n");
		}else{
			buffered_printf(code_gen_state->buffered_output,"add r3 SP r1;     now stores the address of lvalue on stack\n");
		}

		buffered_printf(code_gen_state->buffered_output,"loa r3 r3;        now stores top place to assign to\n");

		if(size == 4){
			buffered_printf(code_gen_state->buffered_output,"add r4 SP ZR;     stores bottom word of rvalue on stack\n");
		}else{
			buffered_printf(code_gen_state->buffered_output,"add r3 r3 r2;     last word of assign space\n");
			buffered_printf(code_gen_state->buffered_output,"add r4 SP r2;     stores bottom word of rvalue on stack\n");
		}

		copy_words(code_gen_state, "r4", "r3", "r5", size);

		if(size == 4){
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR;     Remove the rvalue, and leave the lvalue as result\n");
		}else{
			buffered_printf(code_gen_state->buffered_output,"add SP SP r1;     Remove the rvalue, and leave the lvalue as result\n");
		}
        }
}

unsigned int do_specifiers_contain_extern(struct parser_node * specifiers, enum c_token_type ty){
	assert(specifiers->type == DECLARATION_SPECIFIERS);
	if(specifiers->first_child->type == STORAGE_CLASS_SPECIFIER && specifiers->first_child->first_child->c_lexer_token->type == ty){
		return 1;
	}
	if(specifiers->first_child->next){
		return do_specifiers_contain_extern(specifiers->first_child->next, ty);
	}
	return 0;
}

void g_init_declarator(struct parser_node * specifiers, struct parser_node * p, struct code_gen_state * code_gen_state){
	struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
	struct parser_node * identifier = get_identifier_from_declarator(first_child(p));
	unsigned char * identifier_str = copy_string(identifier->c_lexer_token->first_byte, identifier->c_lexer_token->last_byte, m);
	struct namespace_object * obj;
	unsigned int num_elements;
	struct normalized_declaration_element * element;
	struct type_description * type_description;
	unsigned int is_global;
	unsigned int is_extern;
	unsigned int is_typedef;
	struct parser_node * abstract_declarator;
	obj = get_namespace_object_from_closest_namespace(identifier_str, IDENTIFIER_NAMESPACE, get_current_scope_level(code_gen_state), 0, m);
	num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, num_elements -1);
	type_description = create_type_description_from_normalized_declaration_element(m, element, p, obj->scope_level, LVALUE);
	convert_to_untypedefed_type_description(m, type_description);
	is_global = obj->scope_level == code_gen_state->parser_state->top_scope;

	abstract_declarator = create_abstract_declarator_from_normalized_declarator(m, element->normalized_declarator);

	is_extern = do_specifiers_contain_extern(specifiers, EXTERN);
	/*  TODO:  Typedefed declarations should never generate code, but right now they
	    only don't for globals (because that causes linker object conflicts), but they do for
	    locals because there isn't a good way to tell if a local declaration is an instance
	    of the typedefed type, or a typedef declaration.  Need a better way of telling which
	    declaration element is associated with the current parser nodes.
	*/
	is_typedef = do_specifiers_contain_extern(specifiers, TYPEDEF);

	if(!is_function(m, abstract_declarator)){
		if(check_three_children(p, DECLARATOR, TERMINAL, INITIALIZER)){
			if(is_terminal_c_token_type(second_child(p),EQUALS_CHAR)){
				if(is_global){
					if(!is_extern && !is_typedef){
						unsigned char * name = create_formatted_string(m, 20, "globalvar_%s", identifier_str);
						struct constant_initializer_level * initializer_level = evaluate_constant_initializer(code_gen_state, third_child(p));
						struct type_traversal * type_traversal;
						buffered_printf(code_gen_state->buffered_output,"%s:\n", name);

						type_traversal = construct_type_traversal(code_gen_state, type_description, get_current_scope_level(code_gen_state), 0);
						setup_global_type(code_gen_state, type_traversal, initializer_level);
						destroy_type_traversal(m, type_traversal);

						implement_external_symbol(m, &code_gen_state->symbols, name);
						heap_memory_pool_free(m->heap_pool, name);
						destroy_constant_initializer_level(initializer_level);
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
				if(!is_extern && !is_typedef){
					unsigned char * name = create_formatted_string(m, 20, "globalvar_%s", identifier_str);
					unsigned int size = type_size(code_gen_state, type_description, WORD_ALIGNED_RVALUE, 0, type_description->source_scope_level);
					assert(size % 4 == 0);
					buffered_printf(code_gen_state->buffered_output,"%s:\n", name);
					/*  Don't need to create thousands of dw directives */
					buffered_printf(code_gen_state->buffered_output,"sw 0x%X;\n", size / 4);
					implement_external_symbol(m, &code_gen_state->symbols, name);
					heap_memory_pool_free(m->heap_pool, name);

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

	destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_declarator);
	destroy_type_description(m, type_description);
	heap_memory_pool_free(m->heap_pool, identifier_str);
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
			/*  Not evaluating expression here, because I don't think its necessary and it loads a type we don't want */
			/*  In the future this might be necessary to calculate at run time for variables sized arrays */
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
	struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
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
			switch_value = evaluate_compile_time_constant(code_gen_state, evaluate_constant_constant_expression(code_gen_state, second_child(p)));
			/*  Determine which label this from the perspective if the outer switch statement. */
			for(i = 0; i < unsigned_int_list_size(&frame->values); i++){
				if(unsigned_int_list_get(&frame->values, i) == switch_value){
					switch_index = i;
					break;
				}
				assert(i != unsigned_int_list_size(&frame->values) -1); /* Not found. */
			}

			case_str = create_formatted_string(m, 20, "_%d_%dsc", frame->condition_index, switch_index);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", case_str);
			implement_internal_symbol(m, &code_gen_state->symbols, case_str);
			heap_memory_pool_free(m->heap_pool, case_str);
			g_statement(fourth_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
		}else{
			assert(0 &&"Expected case and colon.\n");
		}
	}else if(check_three_children(p, TERMINAL, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),IDENTIFIER) &&
			is_terminal_c_token_type(second_child(p),COLON_CHAR)
		){
			unsigned char * default_str;
			default_str = copy_string(first_child(p)->c_lexer_token->first_byte, first_child(p)->c_lexer_token->last_byte, m);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", default_str);
			implement_internal_symbol(m, &code_gen_state->symbols, default_str);
			g_statement(third_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			heap_memory_pool_free(m->heap_pool, default_str);
		}else if(
			is_terminal_c_token_type(first_child(p),DEFAULT) &&
			is_terminal_c_token_type(second_child(p),COLON_CHAR)
		){
			struct switch_frame * frame;
			unsigned char * default_str;
			assert(struct_switch_frame_ptr_list_size(&code_gen_state->switch_frames));
			frame = struct_switch_frame_ptr_list_get(&code_gen_state->switch_frames, struct_switch_frame_ptr_list_size(&code_gen_state->switch_frames)-1);
			default_str = create_formatted_string(m, 20, "_%dsd", frame->condition_index);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", default_str);
			implement_internal_symbol(m, &code_gen_state->symbols, default_str);
			heap_memory_pool_free(m->heap_pool, default_str);
			g_statement(third_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
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
				size = evaluate_compile_time_constant(code_gen_state, evaluate_constant_constant_expression(code_gen_state, n->first_child->next));
				unsigned_int_list_add_end(&frame->values, size);
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
	struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
	if(check_seven_children(p, TERMINAL, TERMINAL, EXPRESSION, TERMINAL, STATEMENT, TERMINAL, STATEMENT)){
		if(
			is_terminal_c_token_type(first_child(p),IF) &&
			is_terminal_c_token_type(second_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_PAREN_CHAR) &&
			is_terminal_c_token_type(sixth_child(p),ELSE)
		){
			struct type_description * t;
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * false_condition_str = create_formatted_string(m, 20, "_%dfc", cond_index);
			unsigned char * after_condition_str = create_formatted_string(m, 20, "_%dac", cond_index);
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			g_expression(third_child(p), code_gen_state);
			/*  Pop the item that was loaded in the conditional */
			t = ensure_top_type_is_r_value(code_gen_state, p);
			push_type(code_gen_state, t, p);
			t = consume_scalar_type(code_gen_state, third_child(p));
			pop(code_gen_state, "r1", t->value_type);
			destroy_type_description(m, t);
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s;\n", false_condition_str);
			g_statement(fifth_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			buffered_printf(code_gen_state->buffered_output,"loa PC PC;\n");
			buffered_printf(code_gen_state->buffered_output,"dw %s;\n", after_condition_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", false_condition_str);
			g_statement(seventh_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", after_condition_str);
			require_internal_symbol(m, &code_gen_state->symbols, false_condition_str);
			implement_internal_symbol(m, &code_gen_state->symbols, false_condition_str);
			require_internal_symbol(m, &code_gen_state->symbols, after_condition_str);
			implement_internal_symbol(m, &code_gen_state->symbols, after_condition_str);
			heap_memory_pool_free(m->heap_pool, after_condition_str);
			heap_memory_pool_free(m->heap_pool, false_condition_str);
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
			unsigned char * after_condition_str = create_formatted_string(m, 20, "_%dac", cond_index);
			struct type_description * t;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			g_expression(third_child(p), code_gen_state);
			/*  Pop the item that was loaded in the conditional */
			t = ensure_top_type_is_r_value(code_gen_state, third_child(p));
			push_type(code_gen_state, t, third_child(p));
			t = consume_scalar_type(code_gen_state, third_child(p));
			pop(code_gen_state, "r1", t->value_type);
			destroy_type_description(m, t);
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s;\n", after_condition_str);
			g_statement(fifth_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", after_condition_str);
			require_internal_symbol(m, &code_gen_state->symbols, after_condition_str);
			implement_internal_symbol(m, &code_gen_state->symbols, after_condition_str);
			heap_memory_pool_free(m->heap_pool, after_condition_str);
		}else if(
			is_terminal_c_token_type(first_child(p),SWITCH) &&
			is_terminal_c_token_type(second_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(fourth_child(p),CLOSE_PAREN_CHAR) 
		){
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * after_condition_str = create_formatted_string(m, 20, "_%dac", cond_index);
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(fifth_child(p))->type == COMPOUND_STATEMENT;
			struct switch_frame * frame = (struct switch_frame *)malloc(sizeof(struct switch_frame));
			unsigned int num_cases;
			unsigned int i;
			struct type_description * t;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			frame->condition_index = code_gen_state->condition_index;
			g_expression(third_child(p), code_gen_state);

			t = ensure_top_type_is_r_value(code_gen_state, third_child(p));
			push_type(code_gen_state, t, third_child(p));
			t = consume_scalar_type(code_gen_state, third_child(p));
			pop(code_gen_state, "r1", t->value_type); /* This is the value we need to compare against in the jump table */
			destroy_type_description(m, t);

			find_child_case_labels(code_gen_state, fifth_child(p), frame);
			num_cases = unsigned_int_list_size(&frame->values);
			for(i = 0; i < num_cases; i++){
				unsigned int value = unsigned_int_list_get(&frame->values, i);
				unsigned char * case_str = create_formatted_string(m, 20, "_%d_%dsc", frame->condition_index, i);
				buffered_printf(code_gen_state->buffered_output,"ll r2 0x%X;\n", value);
				buffered_printf(code_gen_state->buffered_output,"beq r1 r2 %s;\n", case_str);
				require_internal_symbol(m, &code_gen_state->symbols, case_str);
				heap_memory_pool_free(m->heap_pool, case_str);
			}

			if(frame->has_default){
				unsigned char * default_str = create_formatted_string(m, 20, "_%dsd", frame->condition_index);
				buffered_printf(code_gen_state->buffered_output,"loa PC PC;\n");
				buffered_printf(code_gen_state->buffered_output,"dw %s;\n", default_str);
				require_internal_symbol(m, &code_gen_state->symbols, default_str);
				heap_memory_pool_free(m->heap_pool, default_str);
			}else{
				/*  There is no case in this switch, jump directly to the end */
				buffered_printf(code_gen_state->buffered_output,"loa PC PC;\n");
				buffered_printf(code_gen_state->buffered_output,"dw %s;\n", after_condition_str);
			}

			struct_switch_frame_ptr_list_add_end(&code_gen_state->switch_frames, frame);
			if(uses_compound_statement){
				g_statement(fifth_child(p), code_gen_state, (unsigned char *)0, after_condition_str);
			}else{
				unsigned_char_ptr_list_add_end(&scope->end_labels, after_condition_str);
				g_statement(fifth_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			}
			unsigned_int_list_destroy(&frame->values);
			free(struct_switch_frame_ptr_list_pop_end(&code_gen_state->switch_frames));

			buffered_printf(code_gen_state->buffered_output,"%s:\n", after_condition_str);
			require_internal_symbol(m, &code_gen_state->symbols, after_condition_str);
			implement_internal_symbol(m, &code_gen_state->symbols, after_condition_str);
			if(uses_compound_statement){
				heap_memory_pool_free(m->heap_pool, after_condition_str);
			}else{
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->end_labels));
			}
		}else{
			assert(0 &&"Expected if or switch.\n");
		}

	}else{
		assert(0 &&"Expected selection statement.\n");
	}
}

void g_iteration_statement(struct parser_node * p, struct code_gen_state * code_gen_state){
	struct memory_pool_collection * m = code_gen_state->memory_pool_collection;
	if(check_seven_children(p, TERMINAL, STATEMENT, TERMINAL, TERMINAL, EXPRESSION, TERMINAL, TERMINAL)){
		if(
			is_terminal_c_token_type(first_child(p),DO) &&
			is_terminal_c_token_type(third_child(p),WHILE) &&
			is_terminal_c_token_type(fourth_child(p),OPEN_PAREN_CHAR) &&
			is_terminal_c_token_type(sixth_child(p),CLOSE_PAREN_CHAR) &&
			is_terminal_c_token_type(seventh_child(p),SEMICOLON_CHAR)
		){

			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * startwhile_str = create_formatted_string(m, 20, "_%dsw", cond_index);
			unsigned char * evaluatewhile_str = create_formatted_string(m, 20, "_%devw", cond_index);
			unsigned char * endwhile_str = create_formatted_string(m, 20, "_%dew", cond_index);
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(second_child(p))->type == COMPOUND_STATEMENT;
			struct type_description * t;
			(void)scope;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			buffered_printf(code_gen_state->buffered_output,"%s:\n", startwhile_str);
			if(uses_compound_statement){
				g_statement(second_child(p), code_gen_state, evaluatewhile_str, endwhile_str);
			}else{
				unsigned_char_ptr_list_add_end(&scope->evaluate_labels, evaluatewhile_str);
				unsigned_char_ptr_list_add_end(&scope->end_labels, endwhile_str);
				g_statement(second_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			}
			buffered_printf(code_gen_state->buffered_output,"%s:\n", evaluatewhile_str);
			g_expression(fifth_child(p), code_gen_state);

			t = ensure_top_type_is_r_value(code_gen_state, third_child(p));
			push_type(code_gen_state, t, third_child(p));
			t = consume_scalar_type(code_gen_state, third_child(p));
			pop(code_gen_state, "r1", t->value_type);
			destroy_type_description(m, t);

			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s; If not true, skip to end\n", endwhile_str);
			buffered_printf(code_gen_state->buffered_output,"loa PC PC; Test condition again\n");
			buffered_printf(code_gen_state->buffered_output,"dw %s; Test condition again\n", startwhile_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", endwhile_str);
			require_internal_symbol(m, &code_gen_state->symbols, evaluatewhile_str);
			require_internal_symbol(m, &code_gen_state->symbols, startwhile_str);
			require_internal_symbol(m, &code_gen_state->symbols, endwhile_str);
			implement_internal_symbol(m, &code_gen_state->symbols, evaluatewhile_str);
			implement_internal_symbol(m, &code_gen_state->symbols, startwhile_str);
			implement_internal_symbol(m, &code_gen_state->symbols, endwhile_str);
			heap_memory_pool_free(m->heap_pool, startwhile_str);
			if(uses_compound_statement){
				heap_memory_pool_free(m->heap_pool, evaluatewhile_str);
				heap_memory_pool_free(m->heap_pool, endwhile_str);
			}else{
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->evaluate_labels));
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->end_labels));
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
			unsigned char * startfor_str = create_formatted_string(m, 20, "_%dsf", cond_index);
			unsigned char * endfor_str = create_formatted_string(m, 20, "_%def", cond_index);
			unsigned char * evaluatefor_str = create_formatted_string(m, 20, "_%devf", cond_index);
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(seventh_child(p))->type == COMPOUND_STATEMENT;
			(void)scope;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			g_expression_statement(third_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", startfor_str);
			/*  Need to keep the value of the expression so we can inspect it */
			if(fourth_child(p)->first_child->type == EXPRESSION){
				struct type_description * t;
				g_expression(fourth_child(p)->first_child, code_gen_state);
				t = ensure_top_type_is_r_value(code_gen_state, p);
				push_type(code_gen_state, t, p);
				t = consume_scalar_type(code_gen_state, p);
				pop(code_gen_state, "r1", t->value_type);
				destroy_type_description(m, t);

				buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s; If not true, skip to end\n", endfor_str);
			}
			if(uses_compound_statement){
				g_statement(seventh_child(p), code_gen_state, evaluatefor_str, endfor_str);
			}else{
				unsigned_char_ptr_list_add_end(&scope->evaluate_labels, evaluatefor_str);
				unsigned_char_ptr_list_add_end(&scope->end_labels, endfor_str);
				g_statement(seventh_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			}
			buffered_printf(code_gen_state->buffered_output,"%s:\n", evaluatefor_str);
			g_expression(fifth_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"add SP SP WR; Pop value of statement\n");
			destroy_type_description(m, pop_type_without_type_check(code_gen_state, fifth_child(p)));
			buffered_printf(code_gen_state->buffered_output,"loa PC PC; Test condition again\n");
			buffered_printf(code_gen_state->buffered_output,"dw %s; Test condition again\n", startfor_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", endfor_str);
			require_internal_symbol(m, &code_gen_state->symbols, startfor_str);
			require_internal_symbol(m, &code_gen_state->symbols, endfor_str);
			require_internal_symbol(m, &code_gen_state->symbols, evaluatefor_str);
			implement_internal_symbol(m, &code_gen_state->symbols, startfor_str);
			implement_internal_symbol(m, &code_gen_state->symbols, endfor_str);
			implement_internal_symbol(m, &code_gen_state->symbols, evaluatefor_str);
			heap_memory_pool_free(m->heap_pool, startfor_str);
			if(uses_compound_statement){
				heap_memory_pool_free(m->heap_pool, evaluatefor_str);
				heap_memory_pool_free(m->heap_pool, endfor_str);
			}else{
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->evaluate_labels));
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->end_labels));
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
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * startfor_str = create_formatted_string(m, 20, "_%dsf", cond_index);
			unsigned char * endfor_str = create_formatted_string(m, 20, "_%def", cond_index);
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(sixth_child(p))->type == COMPOUND_STATEMENT;
			(void)scope;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			g_expression_statement(third_child(p), code_gen_state);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", startfor_str);
			/*  Need to keep the value of the expression so we can inspect it */
			if(fourth_child(p)->first_child->type == EXPRESSION){
				struct type_description * t;
				g_expression(fourth_child(p)->first_child, code_gen_state);
				t = ensure_top_type_is_r_value(code_gen_state, p);
				push_type(code_gen_state, t, p);
				t = consume_scalar_type(code_gen_state, p);
				pop(code_gen_state, "r1", t->value_type);
				destroy_type_description(m, t);
				buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s; If not true, skip to end\n", endfor_str);
			}
			if(uses_compound_statement){
				g_statement(sixth_child(p), code_gen_state, startfor_str, endfor_str);
			}else{
				unsigned_char_ptr_list_add_end(&scope->evaluate_labels, startfor_str);
				unsigned_char_ptr_list_add_end(&scope->end_labels, endfor_str);
				g_statement(sixth_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			}
			buffered_printf(code_gen_state->buffered_output,"loa PC PC; Test condition again\n");
			buffered_printf(code_gen_state->buffered_output,"dw %s; Test condition again\n", startfor_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", endfor_str);
			require_internal_symbol(m, &code_gen_state->symbols, startfor_str);
			require_internal_symbol(m, &code_gen_state->symbols, endfor_str);
			implement_internal_symbol(m, &code_gen_state->symbols, startfor_str);
			implement_internal_symbol(m, &code_gen_state->symbols, endfor_str);
			if(uses_compound_statement){
				heap_memory_pool_free(m->heap_pool, startfor_str);
				heap_memory_pool_free(m->heap_pool, endfor_str);
			}else{
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->evaluate_labels));
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->end_labels));
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
			unsigned int cond_index = code_gen_state->condition_index;
			unsigned char * startwhile_str = create_formatted_string(m, 20, "_%dsw", cond_index);
			unsigned char * endwhile_str = create_formatted_string(m, 20, "_%dew", cond_index);
			struct scope_level * scope = get_current_scope_level(code_gen_state);
			unsigned int uses_compound_statement = first_child(fifth_child(p))->type == COMPOUND_STATEMENT;
			struct type_description * t;
			(void)scope;
			code_gen_state->condition_index = code_gen_state->condition_index + 1;
			buffered_printf(code_gen_state->buffered_output,"%s:\n", startwhile_str);
			g_expression(third_child(p), code_gen_state);
			t = ensure_top_type_is_r_value(code_gen_state, p);
			push_type(code_gen_state, t, p);
			t = consume_scalar_type(code_gen_state, p);
			pop(code_gen_state, "r1", t->value_type);
			destroy_type_description(m, t);
			buffered_printf(code_gen_state->buffered_output,"beq r1 ZR %s; If not true, skip to end\n", endwhile_str);
			if(uses_compound_statement){
				g_statement(fifth_child(p), code_gen_state, startwhile_str, endwhile_str);
			}else{
				unsigned_char_ptr_list_add_end(&scope->evaluate_labels, startwhile_str);
				unsigned_char_ptr_list_add_end(&scope->end_labels, endwhile_str);
				g_statement(fifth_child(p), code_gen_state, (unsigned char *)0, (unsigned char *)0);
			}
			buffered_printf(code_gen_state->buffered_output,"loa PC PC; Test condition again\n");
			buffered_printf(code_gen_state->buffered_output,"dw %s; Test condition again\n", startwhile_str);
			buffered_printf(code_gen_state->buffered_output,"%s:\n", endwhile_str);
			require_internal_symbol(m, &code_gen_state->symbols, startwhile_str);
			require_internal_symbol(m, &code_gen_state->symbols, endwhile_str);
			implement_internal_symbol(m, &code_gen_state->symbols, startwhile_str);
			implement_internal_symbol(m, &code_gen_state->symbols, endwhile_str);
			if(uses_compound_statement){
				heap_memory_pool_free(m->heap_pool, startwhile_str);
				heap_memory_pool_free(m->heap_pool, endwhile_str);
			}else{
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->evaluate_labels));
				heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_pop_end(&scope->end_labels));
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
					buffered_printf(code_gen_state->buffered_output,"loa PC PC;  For continue statement\n");
					buffered_printf(code_gen_state->buffered_output,"dw %s;  For continue statement\n", label);
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
					buffered_printf(code_gen_state->buffered_output,"loa PC PC;  For break statement\n");
					buffered_printf(code_gen_state->buffered_output,"dw %s;  For break statement\n", label);
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
			create_default_return_value(code_gen_state, (struct parser_node *)0, first_child(p));
			return_from_function(code_gen_state, p);
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
		g_compound_statement(fourth_child(p), code_gen_state, p, (unsigned char *)0, (unsigned char *)0);
	}else if(check_three_children(p, DECLARATION_SPECIFIERS, DECLARATOR, COMPOUND_STATEMENT)){
		struct parser_node * identifier = get_identifier_from_declarator(p->first_child->next);
		unsigned char * identifier_str = copy_string(identifier->c_lexer_token->first_byte, identifier->c_lexer_token->last_byte, code_gen_state->memory_pool_collection);
		implement_external_symbol(code_gen_state->memory_pool_collection, &code_gen_state->symbols, identifier_str);
		buffered_printf(code_gen_state->buffered_output,"%s:\n", identifier_str);
		heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, identifier_str);
		g_declaration_specifiers(first_child(p), code_gen_state);
		/*g_declarator(second_child(p), code_gen_state);*/
		g_compound_statement(third_child(p), code_gen_state, p, (unsigned char *)0, (unsigned char *)0);
	}else if(check_three_children(p, DECLARATOR, DECLARATION_LIST, COMPOUND_STATEMENT)){
		/*g_declarator(first_child(p), code_gen_state);*/
		/*g_declaration_list(second_child(p), code_gen_state);*/
		g_compound_statement(third_child(p), code_gen_state, p, (unsigned char *)0, (unsigned char *)0);
	}else if(check_two_children(p, DECLARATOR, COMPOUND_STATEMENT)){
		/*g_declarator(first_child(p), code_gen_state);*/
		g_compound_statement(second_child(p), code_gen_state, p, (unsigned char *)0, (unsigned char *)0);
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
	if(check_two_children(p, EXTERNAL_DECLARATION, TRANSLATION_UNIT_REST)){
		unsigned int i;
		struct struct_constant_description_ptr_list constants = unsigned_char_ptr_to_struct_constant_description_ptr_map_values(&code_gen_state->parser_state->constant_map);
		struct struct_constant_description_ptr_list string_literals;
		unsigned int num_constants = struct_constant_description_ptr_list_size(&constants);
		unsigned int num_string_literals;
		struct_constant_description_ptr_list_create(&string_literals);
		for(i = 0; i < num_constants; i++){
			struct constant_description * description = struct_constant_description_ptr_list_get(&constants, i);
			if(description->type == STRING_LITERAL){
				struct_constant_description_ptr_list_add_end(&string_literals, description);
			}
		}
		num_string_literals = struct_constant_description_ptr_list_size(&string_literals);
		/*  Output string literals. */
		for(i = 0; i < num_string_literals; i++){
			struct constant_description * description = struct_constant_description_ptr_list_get(&string_literals, i);
			unsigned int j;
			unsigned char * string_literal_identifier_str = create_formatted_string(code_gen_state->memory_pool_collection, 20, "_%psl", description->native_data);
			unsigned int * c = (unsigned int *)description->native_data;
			buffered_printf(code_gen_state->buffered_output,"%s:\n", string_literal_identifier_str);
			assert(description->size_in_bytes % sizeof(unsigned int) == 0);
			for(j = 0; j < (description->size_in_bytes / sizeof(unsigned int)); j++){
				buffered_printf(code_gen_state->buffered_output,"dw 0x%X; \n", c[j]);
			}
			implement_internal_symbol(code_gen_state->memory_pool_collection, &code_gen_state->symbols, string_literal_identifier_str);
			heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, string_literal_identifier_str);
		}
		struct_constant_description_ptr_list_destroy(&string_literals);
		struct_constant_description_ptr_list_destroy(&constants);

		g_external_declaration(first_child(p), code_gen_state);
		g_translation_unit_rest(second_child(p), code_gen_state);
	}else{
		assert(0 &&"Expected function declaration or declaration.\n");
	}
}

struct linker_symbol * make_linker_symbol(struct memory_pool_collection * m, unsigned int is_impl, unsigned int is_req, unsigned int is_ext, unsigned int offset){
	struct linker_symbol * new_symbol = struct_linker_symbol_memory_pool_malloc(m->struct_linker_symbol_pool);
	new_symbol->is_implemented = is_impl;
	new_symbol->is_required = is_req;
	new_symbol->is_external = is_ext;
	new_symbol->instruction_index = offset;
	return new_symbol;
}

void require_internal_symbol(struct memory_pool_collection * m, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map, unsigned char * c){
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(map, c) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, c) : (struct linker_symbol *)0;
	if(existing_symbol){
		assert(!existing_symbol->is_external);
		existing_symbol->is_required = 1;
	}else{
		unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(map, copy_string(c, get_null_terminator(c), m), make_linker_symbol(m, 0, 1, 0, 0));
	}
}

void implement_internal_symbol(struct memory_pool_collection * m, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map, unsigned char * c){
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(map, c) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, c) : (struct linker_symbol *)0;
	if(existing_symbol){
		assert(!existing_symbol->is_external);
		existing_symbol->is_implemented = 1;
	}else{
		unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(map, copy_string(c, get_null_terminator(c), m), make_linker_symbol(m, 1, 0, 0, 0));
	}
}

void implement_external_symbol(struct memory_pool_collection * m, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map, unsigned char * c){
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(map, c) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, c) : (struct linker_symbol *)0;
	if(existing_symbol){
		assert(existing_symbol->is_external);
		existing_symbol->is_implemented = 1;
	}else{
		unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(map, copy_string(c, get_null_terminator(c), m), make_linker_symbol(m, 1, 0, 1, 0));
	}
}

void require_external_symbol(struct memory_pool_collection * m, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map, unsigned char * c){
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(map, c) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, c) : (struct linker_symbol *)0;
	if(existing_symbol){
		assert(existing_symbol->is_external);
		existing_symbol->is_required = 1;
	}else{
		unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(map, copy_string(c, get_null_terminator(c), m), make_linker_symbol(m, 0, 1, 1, 0));
	}
}

int generate_code(struct code_gen_state * code_gen_state){
	if(code_gen_state->parser_state->top_node){
		if(code_gen_state->parser_state->top_node->type == TRANSLATION_UNIT){
			g_translation_unit(code_gen_state->parser_state->top_node, code_gen_state);
		}else{
			assert(0 && "Top node was not expected node.\n");
		}
	}else{
		buffered_printf(code_gen_state->buffered_output,";There is no code to generate: Top node was null.\n");
	}
	return 0;
}

void create_code_gen_state(struct code_gen_state * code_gen_state, struct parser_state * parser_state, struct unsigned_char_list * buffered_output, struct unsigned_char_list * buffered_symbol_table){
	code_gen_state->memory_pool_collection = parser_state->memory_pool_collection; 
	code_gen_state->buffered_output = buffered_output; 
	code_gen_state->buffered_symbol_table = buffered_symbol_table; 
	code_gen_state->parser_state = parser_state;
	code_gen_state->current_function = (struct namespace_object *)0;
	code_gen_state->condition_index = 0;
	code_gen_state->global_var_ptr_index = 0;
	struct_switch_frame_ptr_list_create(&code_gen_state->switch_frames);
	unsigned_int_list_create(&code_gen_state->scope_index_list);
	code_gen_state->next_scope_index = 0;
	struct_type_description_ptr_list_create(&code_gen_state->type_stack);
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_create(&code_gen_state->symbols);
}

int destroy_code_gen_state(struct code_gen_state * code_gen_state){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&code_gen_state->symbols);
	unsigned int size = unsigned_char_ptr_list_size(&keys);
	unsigned int i;
	for(i = 0; i < size; i++){
		struct_linker_symbol_memory_pool_free(code_gen_state->memory_pool_collection->struct_linker_symbol_pool, unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&code_gen_state->symbols, unsigned_char_ptr_list_get(&keys, i)));
	}
	/*  Need to do this twice because we're deleting the data under the keys of the map */
	for(i = 0; i < size; i++){
		heap_memory_pool_free(code_gen_state->memory_pool_collection->heap_pool, unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_destroy(&code_gen_state->symbols);
	unsigned_int_list_destroy(&code_gen_state->scope_index_list);
	struct_type_description_ptr_list_destroy(&code_gen_state->type_stack);
	unsigned_char_ptr_list_destroy(&keys);
	struct_switch_frame_ptr_list_destroy(&code_gen_state->switch_frames);
	return 0;
}


int do_code_generation(struct memory_pool_collection * memory_pool_collection, unsigned char * in_file, unsigned char * out_file){
	int rtn; 
	unsigned int i; 
	struct unsigned_char_list preprocessed_input;
	struct unsigned_char_list generated_code;
	struct unsigned_char_list buffered_symbol_table;
	struct unsigned_char_list lexer_output;
	struct parser_state parser_state;
	struct code_gen_state code_gen_state;
	struct c_lexer_state c_lexer_state;
	/*

	Different types of operations on stack:
	Convert top type to rvalue.  Make lvalue into pointer.
	Perform char to int sign extension on top type
	Do copy of type to place at lvalue 
	Conversion of top types to rvalues
	Multiply, add divide etc. top two types
	
	Must know:  Type of operation to perform.  Type of operation to perform will deduce type sizes.  What about struct assignment?
	Figure out what to do with copy words.
	
	*/

	unsigned_char_list_create(&lexer_output);
	unsigned_char_list_create(&preprocessed_input);
	unsigned_char_list_create(&generated_code);
	unsigned_char_list_create(&buffered_symbol_table);

	add_file_to_buffer(&preprocessed_input, (char*)in_file);

	create_c_lexer_state(&c_lexer_state, &lexer_output, memory_pool_collection, in_file, unsigned_char_list_data(&preprocessed_input), unsigned_char_list_size(&preprocessed_input));
	/*  Use the lexer to generate tokens from 'pure c' code (no preprocessor directives) */
	rtn = lex_c(&c_lexer_state);
	for(i = 0; i < unsigned_char_list_size(&lexer_output); i++){
		printf("%c", unsigned_char_list_get(&lexer_output, i));
	}

	/*  Parse the tokens the are output fromt the preprocessor */
	if(rtn){
		printf("Lexical analysis failed during code generation of %s\n", in_file);
	}else{
		create_parser_state(&parser_state, memory_pool_collection, &c_lexer_state, &generated_code, unsigned_char_list_data(&preprocessed_input));
		if(parse(&parser_state)){
			printf("Parsing failed during code generation of %s\n", in_file);
		}else{
			create_code_gen_state(&code_gen_state, &parser_state, &generated_code, &buffered_symbol_table);
			if(generate_code(&code_gen_state)){
				printf("Code generation failed for %s\n", in_file);
			}else{
				struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&code_gen_state.symbols);
				unsigned int size = unsigned_char_ptr_list_size(&keys);
				buffered_printf(code_gen_state.buffered_symbol_table, "OFFSET RELOCATABLE;\n");
				for(i = 0; i < size; i++){
					unsigned char * key = unsigned_char_ptr_list_get(&keys, i);
					struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&code_gen_state.symbols, key) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&code_gen_state.symbols, key) : (struct linker_symbol *)0;
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
				unsigned_char_list_add_all_end(&buffered_symbol_table, &generated_code);
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
	return rtn;
}


