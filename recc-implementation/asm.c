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
struct unscoped_tag_specifier_id get_asm_id_for_unscoped_tag_specifier(struct asm_state *, enum asm_unscoped_tag_specifier_kind, unsigned int);
struct unscoped_struct_specifier_id get_aggregate_struct_specifier_id(struct asm_state *, enum asm_unscoped_struct_specifier_kind, unsigned int);
struct unscoped_union_specifier_id get_aggregate_union_specifier_id(struct asm_state *, enum asm_unscoped_union_specifier_kind, unsigned int);
struct unscoped_enum_specifier_id get_aggregate_enum_specifier_id(struct asm_state *, enum asm_unscoped_enum_specifier_kind, unsigned int);
struct function_part_id get_asm_id_for_function_part(struct asm_state *, enum asm_function_part_kind, unsigned int);
struct array_part_id get_asm_id_for_array_part(struct asm_state *, enum asm_array_part_kind, unsigned int);

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

struct scope_guid_id increment_scope_depth(struct asm_state * state, enum scope_level_type t){
	struct scope_guid_id id;
	id.id = 0;
	(void)t;
	(void)state;
	/*state->current_scope_depth = state->current_scope_depth + 1;*/
	return id;
}

void decrement_scope_depth(struct asm_state * state){
	/*assert(state->current_scope_depth > 0);*/
	(void)state;
	/*state->current_scope_depth = state->current_scope_depth - 1;*/
}

void undo_asm_operation(struct asm_state * state, enum asm_operation_type o){
	if(!undo_basic_asm_operation(state, o)){
		return;
	}
	assert(0);
}

struct aggregate_type_specifier_id aggregate_type_specifier_from_simple_type_specifier_id(struct asm_state * state, struct simple_type_specifier_id s){
	return aggregate_type_specifier_from_any_specifier_id(state, ASM_AGGREGATE_TYPE_SPECIFIER_SIMPLE_TYPE_SPECIFIER, s.id);
}

struct aggregate_type_specifier_id aggregate_type_specifier_from_typename_specifier_id(struct asm_state * state, unsigned int s){
	return aggregate_type_specifier_from_any_specifier_id(state, ASM_AGGREGATE_TYPE_SPECIFIER_TYPENAME_SPECIFIER, s);
}

struct aggregate_type_specifier_id aggregate_type_specifier_from_scoped_tag_specifier_id(struct asm_state * state, struct scoped_tag_specifier_id s){
	return aggregate_type_specifier_from_any_specifier_id(state, ASM_AGGREGATE_TYPE_SPECIFIER_TAG_SPECIFIER, s.id);
}

struct aggregate_type_specifier_id aggregate_type_specifier_from_any_specifier_id(struct asm_state * state, enum asm_aggregate_type_specifier_kind type, unsigned int s){
	struct aggregate_type_specifier_details details;
	details.type = type;
	details.id = s;
	if(struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_exists(&state->aggregate_type_specifiers_map, details)){
		return struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_get(&state->aggregate_type_specifiers_map, details);
	}else{
		struct aggregate_type_specifier_id new_id;
		new_id.id = struct_aggregate_type_specifier_details_list_size(&state->aggregate_type_specifiers);
		struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_put(&state->aggregate_type_specifiers_map, details, new_id);
		struct_aggregate_type_specifier_details_list_add_end(&state->aggregate_type_specifiers, details);
		return new_id;
	}
}

struct simple_type_specifier_id get_asm_id_for_simple_type_specifier(struct asm_state * state, enum asm_simple_type_specifier_kind t){
	struct simple_type_specifier_details details;
	details.type = t;
	if(struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_exists(&state->simple_type_specifiers_map, details)){
		return struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_get(&state->simple_type_specifiers_map, details);
	}else{
		struct simple_type_specifier_id new_id;
		new_id.id = struct_simple_type_specifier_details_list_size(&state->simple_type_specifiers);
		struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_put(&state->simple_type_specifiers_map, details, new_id);
		struct_simple_type_specifier_details_list_add_end(&state->simple_type_specifiers, details);
		return new_id;
	}
}

struct simple_storage_class_specifier_id get_asm_id_for_simple_storage_class_specifier(struct asm_state * state, enum asm_simple_storage_class_specifier_kind t){
	struct simple_storage_class_specifier_details details;
	details.type = t;
	if(struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_exists(&state->simple_storage_class_specifiers_map, details)){
		return struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_get(&state->simple_storage_class_specifiers_map, details);
	}else{
		struct simple_storage_class_specifier_id new_id;
		new_id.id = struct_simple_storage_class_specifier_details_list_size(&state->simple_storage_class_specifiers);
		struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_put(&state->simple_storage_class_specifiers_map, details, new_id);
		struct_simple_storage_class_specifier_details_list_add_end(&state->simple_storage_class_specifiers, details);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SIMPLE_STORAGE_CLASS_SPECIFIER);
		return new_id;
	}
}

struct simple_type_qualifier_id get_asm_id_for_simple_qualifier(struct asm_state * state, enum asm_simple_type_qualifier_kind t){
	struct simple_type_qualifier_details details;
	details.type = t;
	if(struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_exists(&state->simple_type_qualifiers_map, details)){
		return struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_get(&state->simple_type_qualifiers_map, details);
	}else{
		struct simple_type_qualifier_id new_id;
		new_id.id = struct_simple_type_qualifier_details_list_size(&state->simple_type_qualifiers);
		struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_put(&state->simple_type_qualifiers_map, details, new_id);
		struct_simple_type_qualifier_details_list_add_end(&state->simple_type_qualifiers, details);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SIMPLE_TYPE_QUALIFIER);
		return new_id;
	}
}

struct specifier_or_qualifier_id specifier_or_qualifier_from_aggregate_type_specifier_id(struct asm_state * state, struct aggregate_type_specifier_id s){
	return get_asm_id_for_any_specifier_or_qualifier(state, ASM_SPECIFIER_OR_QUALIFIER_AGGREGATE_TYPE_SPECIFIER, s.id);
}

struct specifier_or_qualifier_id specifier_or_qualifier_from_type_qualifier_id(struct asm_state * state, struct simple_type_qualifier_id s){
	return get_asm_id_for_any_specifier_or_qualifier(state, ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_TYPE_QUALIFIER, s.id);
}

struct specifier_or_qualifier_id specifier_or_qualifier_from_simple_storage_class_specifier_id(struct asm_state * state, struct simple_storage_class_specifier_id s){
	return get_asm_id_for_any_specifier_or_qualifier(state, ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_STORAGE_CLASS_SPECIFIER, s.id);
}

struct specifier_or_qualifier_id get_asm_id_for_any_specifier_or_qualifier(struct asm_state * state, enum asm_specifier_or_qualifier_kind type, unsigned int index){
	struct specifier_or_qualifier_details details;
	details.type = type;
	details.id = index;
	if(struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_exists(&state->specifier_or_qualifiers_map, details)){
		return struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_get(&state->specifier_or_qualifiers_map, details);
	}else{
		struct specifier_or_qualifier_id new_id;
		new_id.id = struct_specifier_or_qualifier_details_list_size(&state->specifier_or_qualifiers);
		struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_put(&state->specifier_or_qualifiers_map, details, new_id);
		struct_specifier_or_qualifier_details_list_add_end(&state->specifier_or_qualifiers, details);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SPECIFIER_OR_QUALIFIER);
		return new_id;
	}
}

struct array_part_id get_asm_id_for_constant_expression_array_part(struct asm_state * state, unsigned int index){
	struct array_part_id id;
	(void)state;
	id.id = index;
	return id;
}

struct parameter_list_id get_asm_id_for_parameter_list(struct asm_state * state, enum asm_parameter_list_kind type, struct general_type_list_item_id general_type_list_item_id){
	struct parameter_list_details details;
	details.type = type;
	details.general_type_list_item_id = general_type_list_item_id;
	if(struct_parameter_list_details_to_struct_parameter_list_id_map_exists(&state->parameter_lists_map, details)){
		return struct_parameter_list_details_to_struct_parameter_list_id_map_get(&state->parameter_lists_map, details);
	}else{
		struct parameter_list_id new_id;
		new_id.id = struct_parameter_list_details_list_size(&state->parameter_lists);
		struct_parameter_list_details_to_struct_parameter_list_id_map_put(&state->parameter_lists_map, details, new_id);
		struct_parameter_list_details_list_add_end(&state->parameter_lists, details);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_PARAMETER_LIST);
		return new_id;
	}
}

struct scoped_tag_specifier_id get_asm_id_for_scoped_tag_specifier(struct asm_state * state, struct scope_guid_id scope_id, struct unscoped_tag_specifier_id unscoped_tag_specifier_id){
	struct scoped_tag_specifier_details details;
	details.scope_guid_id = scope_id;
	details.unscoped_tag_specifier_id = unscoped_tag_specifier_id;
	if(struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_exists(&state->scoped_tag_specifiers_map, details)){
		return struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_get(&state->scoped_tag_specifiers_map, details);
	}else{
		struct scoped_tag_specifier_id new_id;
		new_id.id = struct_scoped_tag_specifier_details_list_size(&state->scoped_tag_specifiers);
		struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_put(&state->scoped_tag_specifiers_map, details, new_id);
		struct_scoped_tag_specifier_details_list_add_end(&state->scoped_tag_specifiers, details);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SCOPED_TAG_SPECIFIER);
		return new_id;
	}
}

struct unscoped_tag_specifier_id get_struct_specifier_id(struct asm_state * state, struct unscoped_struct_specifier_id index){
	return get_asm_id_for_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_STRUCT_SPECIFIER, index.id);
}

struct unscoped_tag_specifier_id get_union_specifier_id(struct asm_state * state, struct unscoped_union_specifier_id index){
	return get_asm_id_for_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_UNION_SPECIFIER, index.id);
}

struct unscoped_tag_specifier_id get_enum_specifier_id(struct asm_state * state, struct unscoped_enum_specifier_id index){
	return get_asm_id_for_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_ENUM_SPECIFIER, index.id);
}

struct unscoped_tag_specifier_id get_asm_id_for_unscoped_tag_specifier(struct asm_state * state, enum asm_unscoped_tag_specifier_kind type, unsigned int index){
	struct unscoped_tag_specifier_details details;
	details.type = type;
	details.id = index;
	if(struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_exists(&state->unscoped_tag_specifiers_map, details)){
		return struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_get(&state->unscoped_tag_specifiers_map, details);
	}else{
		struct unscoped_tag_specifier_id new_id;
		new_id.id = struct_unscoped_tag_specifier_details_list_size(&state->unscoped_tag_specifiers);
		struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_put(&state->unscoped_tag_specifiers_map, details, new_id);
		struct_unscoped_tag_specifier_details_list_add_end(&state->unscoped_tag_specifiers, details);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_UNSCOPED_TAG_SPECIFIER);
		return new_id;
	}
}


struct unscoped_enum_specifier_id get_named_enum_specifier_id(struct asm_state * state, struct identifier_id index){
	return get_aggregate_enum_specifier_id(state, ASM_UNSCOPED_ENUM_SPECIFIER_NAMED_ENUM, index.id);
}

struct unscoped_enum_specifier_id get_anonymous_enum_specifier_id(struct asm_state * state, unsigned int index){
	return get_aggregate_enum_specifier_id(state, ASM_UNSCOPED_ENUM_SPECIFIER_ANONYMOUS_ENUM, index);
}

struct unscoped_enum_specifier_id get_aggregate_enum_specifier_id(struct asm_state * state, enum asm_unscoped_enum_specifier_kind type, unsigned int index){
	struct unscoped_enum_specifier_id i;
	(void)state;
	(void)type;
	(void)index;
	return i;
}


struct unscoped_union_specifier_id get_named_union_specifier_id(struct asm_state * state, struct identifier_id index){
	return get_aggregate_union_specifier_id(state, ASM_UNSCOPED_UNION_SPECIFIER_NAMED_UNION, index.id);
}

struct unscoped_union_specifier_id get_anonymous_union_specifier_id(struct asm_state * state, unsigned int index){
	return get_aggregate_union_specifier_id(state, ASM_UNSCOPED_UNION_SPECIFIER_ANONYMOUS_UNION, index);
}

struct unscoped_union_specifier_id get_aggregate_union_specifier_id(struct asm_state * state, enum asm_unscoped_union_specifier_kind type, unsigned int index){
	struct unscoped_union_specifier_id i;
	(void)state;
	(void)type;
	(void)index;
	return i;
}

struct unscoped_struct_specifier_id get_named_struct_specifier_id(struct asm_state * state, struct identifier_id index){
	return get_aggregate_struct_specifier_id(state, ASM_UNSCOPED_STRUCT_SPECIFIER_NAMED_STRUCT, index.id);
}

struct unscoped_struct_specifier_id get_anonymous_struct_specifier_id(struct asm_state * state, unsigned int index){
	return get_aggregate_struct_specifier_id(state, ASM_UNSCOPED_STRUCT_SPECIFIER_ANONYMOUS_STRUCT, index);
}

struct unscoped_struct_specifier_id get_aggregate_struct_specifier_id(struct asm_state * state, enum asm_unscoped_struct_specifier_kind type, unsigned int index){
	struct unscoped_struct_specifier_id i;
	(void)state;
	(void)type;
	(void)index;
	return i;
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

void add_ordered_general_type_to_identifier_declaration_namespace(struct asm_state * state, struct identifier_declaration_namespace * dn, struct general_type_id general_type_id){
	(void)dn;
	(void)general_type_id;
	(void)state;
}

void add_identifier_id_to_identifier_declaration_namespace(struct asm_state * state, struct identifier_declaration_namespace * dn, struct general_type_id general_type_id, struct identifier_id identifier_id){
	(void)state;
	(void)dn;
	(void)general_type_id;
	(void)identifier_id;
}

unsigned int add_named_tag_declaration(struct asm_state * state, struct identifier_id identifier_id, struct unscoped_tag_specifier_id unscoped_tag_specifier_id, struct scope_level * scope){
	(void)state;
	(void)identifier_id;
	(void)unscoped_tag_specifier_id;
	(void)scope;
	return 0;
	
}

unsigned int add_struct_or_union_definition(struct asm_state * state, struct identifier_declaration_namespace * dn, struct unscoped_tag_specifier_id unscoped_tag_specifier_id, struct scope_level * scope, enum asm_unscoped_tag_specifier_kind type){
	(void)state;
	(void)dn;
	(void)unscoped_tag_specifier_id;
	(void)scope;
	(void)type;
	return 0;
}

unsigned int add_enum_definition(struct asm_state * state, struct identifier_declaration_namespace * dn, struct unscoped_tag_specifier_id unscoped_tag_specifier_id, struct scope_level * scope){
	(void)state;
	(void)dn;
	(void)unscoped_tag_specifier_id;
	(void)scope;
	return 0;
}

struct scope_level * get_scope_of_closest_tag_declaration(struct asm_state * state, struct scope_level * starting_scope, struct unscoped_tag_specifier_id unscoped_tag_id){
	(void)state;
	(void)starting_scope;
	(void)unscoped_tag_id;
	(void)unscoped_tag_id;
	return (struct scope_level *)0;
}




void print_terminal_specifier_or_qualifier_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct specifier_or_qualifier_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_specifier_or_qualifier_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct specifier_or_qualifier_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_any_statement_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct any_statement_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_any_statement_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct any_statement_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_struct_or_union_member_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct struct_or_union_member_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_struct_or_union_member_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct struct_or_union_member_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_enum_member_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct enum_member_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_enum_member_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct enum_member_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_layout_flag_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct layout_flag_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_layout_flag_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct layout_flag_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_initializer_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct initializer_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_initializer_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct initializer_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_expression_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct expression_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_expression_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct expression_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_declarator_part_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct declarator_part_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_declarator_part_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct declarator_part_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_function_part_k_and_r_c_parameter_list_id(struct asm_state *state, struct unsigned_char_list * out, struct parameter_list_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_array_part_flexible_expression_id(struct asm_state *state, struct unsigned_char_list * out, struct expression_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_general_type_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct general_type_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_general_type_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct general_type_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_postfix_expression_part_list_item_list_start_prev_id(struct asm_state *state, struct unsigned_char_list * out, struct postfix_expression_part_list_item_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_postfix_expression_part_list_item_list_start_item_id(struct asm_state *state, struct unsigned_char_list * out, struct postfix_expression_part_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_postfix_expression_part_inc_id(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_postfix_expression_part_dec_id(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_bitfield_no_bitfield_expression_id(struct asm_state *state, struct unsigned_char_list * out, struct expression_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_jump_statement_return_no_expression_id(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_jump_statement_break_id(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_jump_statement_continue_id(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_if_statement_just_if_s2(struct asm_state *state, struct unsigned_char_list * out, struct any_statement_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_labeled_statement_goto_label_expression_id(struct asm_state *state, struct unsigned_char_list * out, struct expression_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_labeled_statement_default_label_expression_id(struct asm_state *state, struct unsigned_char_list * out, struct expression_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_cast_expression_no_cast_id1(struct asm_state *state, struct unsigned_char_list * out, struct general_type_id a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_byte_package_byte_data(struct asm_state *state, struct unsigned_char_list * out, unsigned char a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_hword_package_hword_data0(struct asm_state *state, struct unsigned_char_list * out, unsigned char a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_hword_package_hword_data1(struct asm_state *state, struct unsigned_char_list * out, unsigned char a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_word_package_word_data(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_dword_package_dword_data0(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_dword_package_dword_data1(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_string_package_string_length(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_string_package_string_data(struct asm_state *state, struct unsigned_char_list * out, unsigned char * a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_identifier_identifier_identifier(struct asm_state *state, struct unsigned_char_list * out, struct c_lexer_token * a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_scope_guid_block_scope_id(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_scope_guid_file_scope_id(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}
void print_terminal_scope_guid_function_scope_id(struct asm_state *state, struct unsigned_char_list * out, unsigned int a){
	(void)state;
	(void)out;
	(void)a;
}

void print_terminal_cast_expression_no_cast_id2(struct asm_state *state, struct unsigned_char_list * out, struct general_type_id a){
	(void)state;
	(void)out;
	(void)a;
}
