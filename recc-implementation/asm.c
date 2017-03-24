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
#include "asm.h"

/*  Private functions:  Do not call externally */
struct aggregate_type_specifier_id aggregate_type_specifier_from_any_specifier_id(struct asm_state *, enum asm_aggregate_type_specifier_kind, unsigned int);
struct specifier_or_qualifier_id get_asm_id_for_any_specifier_or_qualifier(struct asm_state *, enum asm_specifier_or_qualifier_kind, unsigned int);
struct specifier_or_qualifier_list_item_id get_asm_specifier_qualifier_list_id(struct asm_state *, enum asm_specifier_or_qualifier_list_item_kind, struct specifier_or_qualifier_list_item_id, struct specifier_or_qualifier_id);
struct declarator_part_list_item_id get_asm_declarator_part_list_id(struct asm_state *, enum asm_declarator_part_list_item_kind, struct declarator_part_list_item_id, struct declarator_part_id);
struct declarator_part_id get_asm_id_for_declarator_part(struct asm_state * state, enum asm_declarator_part_kind type, unsigned int index);
struct parameter_list_id get_asm_id_for_parameter_list(struct asm_state *, enum asm_parameter_list_kind, struct general_type_list_item_id);
struct general_type_list_item_id get_asm_general_type_list_item_id(struct asm_state *, enum asm_general_type_list_item_kind, struct general_type_list_item_id, struct general_type_id);
struct unscoped_struct_specifier_id get_aggregate_struct_specifier_id(struct asm_state *, enum asm_unscoped_struct_specifier_kind, unsigned int);
struct unscoped_union_specifier_id get_aggregate_union_specifier_id(struct asm_state *, enum asm_unscoped_union_specifier_kind, unsigned int);
struct unscoped_enum_specifier_id get_aggregate_enum_specifier_id(struct asm_state *, enum asm_unscoped_enum_specifier_kind, unsigned int);

enum c_token_type simple_type_qualifier_kind_to_c_token_type(enum asm_simple_type_qualifier_kind kind){
	switch(kind){
		case ASM_SIMPLE_TYPE_QUALIFIER_CONST:{
			return CONST;
		}case ASM_SIMPLE_TYPE_QUALIFIER_VOLATILE:{
			return VOLATILE;
		}default:{
			assert(0); /* Error */
			return SPACE;
		}
	}
}

enum asm_simple_type_qualifier_kind c_token_type_to_simple_type_qualifier_kind(enum c_token_type t){
	switch(t){
		case CONST:{
			return ASM_SIMPLE_TYPE_QUALIFIER_CONST;
		}case VOLATILE:{
			return ASM_SIMPLE_TYPE_QUALIFIER_VOLATILE;
		}default:{
			assert(0); /* Error */
			return ASM_SIMPLE_TYPE_QUALIFIER_CONST;
		}
	}
}

enum asm_simple_type_specifier_kind c_token_type_to_simple_type_specifier_kind(enum c_token_type t){
	switch(t){
		case VOID:{
			return ASM_SIMPLE_TYPE_SPECIFIER_VOID;
		}case CHAR:{
			return ASM_SIMPLE_TYPE_SPECIFIER_CHAR;
		}case SHORT:{
			return ASM_SIMPLE_TYPE_SPECIFIER_SHORT;
		}case INT:{
			return ASM_SIMPLE_TYPE_SPECIFIER_INT;
		}case LONG:{
			return ASM_SIMPLE_TYPE_SPECIFIER_LONG;
		}case FLOAT:{
			return ASM_SIMPLE_TYPE_SPECIFIER_FLOAT;
		}case DOUBLE:{
			return ASM_SIMPLE_TYPE_SPECIFIER_DOUBLE;
		}case SIGNED:{
			return ASM_SIMPLE_TYPE_SPECIFIER_SIGNED;
		}case UNSIGNED:{
			return ASM_SIMPLE_TYPE_SPECIFIER_UNSIGNED;
		}default:{
			assert(0); /* Error */
			return ASM_SIMPLE_TYPE_SPECIFIER_UNSIGNED;
		}
	}
}

enum c_token_type simple_type_specifier_kind_to_c_token_type(enum asm_simple_type_specifier_kind kind){
	switch(kind){
		case ASM_SIMPLE_TYPE_SPECIFIER_VOID:{
			return VOID;
		}case ASM_SIMPLE_TYPE_SPECIFIER_CHAR:{
			return CHAR;
		}case ASM_SIMPLE_TYPE_SPECIFIER_SHORT:{
			return SHORT;
		}case ASM_SIMPLE_TYPE_SPECIFIER_INT:{
			return INT;
		}case ASM_SIMPLE_TYPE_SPECIFIER_LONG:{
			return LONG;
		}case ASM_SIMPLE_TYPE_SPECIFIER_FLOAT:{
			return FLOAT;
		}case ASM_SIMPLE_TYPE_SPECIFIER_DOUBLE:{
			return DOUBLE;
		}case ASM_SIMPLE_TYPE_SPECIFIER_SIGNED:{
			return SIGNED;
		}case ASM_SIMPLE_TYPE_SPECIFIER_UNSIGNED:{
			return UNSIGNED;
		}default:{
			assert(0); /* Error */
			return SPACE;
		}
	}
}

enum asm_simple_storage_class_specifier_kind c_token_type_to_simple_storage_class_specifier_kind(enum c_token_type t){
	switch(t){
		case TYPEDEF:{
			return ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_TYPEDEF;
		}case EXTERN:{
			return ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_EXTERN;
		}case STATIC:{
			return ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_STATIC;
		}case AUTO:{
			return ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_AUTO;
		}case REGISTER:{
			return ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_REGISTER;
		}default:{
			assert(0); /* Error */
			return ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_REGISTER;
		}
	}
}

enum c_token_type simple_storage_class_specifier_kind_to_c_token_type(enum asm_simple_storage_class_specifier_kind kind){
	switch(kind){
		case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_TYPEDEF:{
			return TYPEDEF;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_EXTERN:{
			return EXTERN;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_STATIC:{
			return STATIC;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_AUTO:{
			return AUTO;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_REGISTER:{
			return REGISTER;
		}default:{
			assert(0); /* Error */
			return SPACE;
		}
	}
}

void syntax_model_print_char(struct unsigned_char_list * out, unsigned char c){
	if(c > 31 && c < 127){
		buffered_printf(out, "'%c'", c);
	}else{
		buffered_printf(out, "'\\x%X'", c);
	}
}

unsigned int asm_checkpoint(struct asm_state * state){
	/*  Returns the number of operations that have been performed so far */
	return enum_asm_operation_type_list_size(&state->reversible_operations);
}

void backtrack_asm(struct asm_state * state, unsigned int target_number){
	assert(enum_asm_operation_type_list_size(&state->reversible_operations) >= target_number);
	while(enum_asm_operation_type_list_size(&state->reversible_operations) > target_number){
		enum asm_operation_type operation = enum_asm_operation_type_list_pop_end(&state->reversible_operations);
		undo_asm_operation(state, operation);
	}
}

void free_string_package_pointers(struct asm_state * state){
	/*  Need to explicitly free the string buffers because those were manually allocated. */
	unsigned int i;
	unsigned int size = struct_string_package_details_list_size(&state->string_packages);
	for(i = 0; i < size; i++){
		struct string_package_details details = struct_string_package_details_list_get(&state->string_packages, i);
		free(details.data);
	}
}

void undo_asm_operation(struct asm_state * state, enum asm_operation_type o){
	if(!undo_basic_asm_operation(state, o)){
		return;
	}
	assert(0);
}

void add_declarators_to_list(struct asm_state * state, struct declarator_part_list_item_id * most_recent_list_id, struct declarator_part_list_item_id current_declarator_id, unsigned int reverse){
	/*  Add the declarators onto the end of the list */
	struct declarator_part_list_item_details details = struct_declarator_part_list_item_details_list_get(&state->declarator_part_list_items, current_declarator_id.id);
	if(details.type == ASM_DECLARATOR_PART_LIST_ITEM_LIST_START){
		/*  base case */
	}else{
		if(reverse){
			/*  Now add these items to the end of the list */
			*most_recent_list_id = make_declarator_part_list_item_declarator_part(state, *most_recent_list_id, details.item_id);
			/*  Go down to base case, */
			add_declarators_to_list(state, most_recent_list_id, details.prev_id, reverse);
		}else{
			/*  Go down to base case, */
			add_declarators_to_list(state, most_recent_list_id, details.prev_id, reverse);
			/*  Now add these items to the end of the list */
			*most_recent_list_id = make_declarator_part_list_item_declarator_part(state, *most_recent_list_id, details.item_id);
		}
	}
}

struct declarator_part_list_item_id add_pointer_and_direct_declarators_to_list(struct asm_state * state, struct declarator_part_list_item_id previous_list_item, struct declarator_part_list_item_id pointer_end_list_item, struct declarator_part_list_item_id direct_declarator_end_list_item){
	/*  This function takes care of the fact that declarators aren't parsed in the same order that they are read in the way they are logically interpreted  */
	add_declarators_to_list(state, &previous_list_item, direct_declarator_end_list_item, 0);
	add_declarators_to_list(state, &previous_list_item, pointer_end_list_item, 1);
	return previous_list_item;
}


struct identifier_declaration_namespace * create_identifier_declaration_namespace(struct asm_state * state){
	(void)state;
	return (struct identifier_declaration_namespace *)0;
}

struct tag_declaration_namespace * create_tag_declaration_namespace(struct asm_state * state){
	(void)state;
	return (struct tag_declaration_namespace *)0;
}

unsigned int add_enum_definition(struct asm_state * state, struct identifier_declaration_namespace * dn, struct unscoped_tag_specifier_id unscoped_tag_specifier_id, struct scope_level * scope){
	(void)state;
	(void)dn;
	(void)unscoped_tag_specifier_id;
	(void)scope;
	return 0;
}


void print_terminal_struct_or_union_member_list_item_list_start_prev_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct struct_or_union_member_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_struct_or_union_member_list_item_list_start_item_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct struct_or_union_member_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_enum_member_list_item_list_start_prev_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct enum_member_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_enum_member_list_item_list_start_item_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct enum_member_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_initializer_list_item_list_start_prev_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct initializer_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_initializer_list_item_list_start_item_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct initializer_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_general_type_list_item_list_start_prev_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct general_type_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_general_type_list_item_list_start_item_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct general_type_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_postfix_expression_part_list_item_list_start_prev_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct postfix_expression_part_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_postfix_expression_part_list_item_list_start_item_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct postfix_expression_part_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_bitfield_no_bitfield_expression_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct expression_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_jump_statement_return_no_expression_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_jump_statement_break_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_jump_statement_continue_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_if_statement_just_if_s2(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct any_statement_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_labeled_statement_goto_label_expression_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct expression_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_labeled_statement_default_label_expression_id(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, struct expression_id a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_byte_package_byte_data(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, unsigned char a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_hword_package_hword_data0(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, unsigned char a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_hword_package_hword_data1(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, unsigned char a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_string_package_string_length(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_string_package_string_data(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, unsigned char * a){
	(void)state;
	(void)out;
	(void)a;
	(void)scope_depth;
}
void print_terminal_named_tag_predeclaration_named_tag_type(struct asm_state *state, unsigned int scope_depth, struct unsigned_char_list * out, enum asm_unscoped_tag_specifier_kind a){
	switch(a){
		case ASM_UNSCOPED_TAG_SPECIFIER_STRUCT:{
			buffered_printf(out, "struct");
			break;
		}case ASM_UNSCOPED_TAG_SPECIFIER_UNION:{
			buffered_printf(out, "union");
			break;
		}case ASM_UNSCOPED_TAG_SPECIFIER_ENUM:{
			buffered_printf(out, "enum");
			break;
		}
	}
	(void)state;
	(void)scope_depth;
}
