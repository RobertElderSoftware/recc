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

#include "asm_functions.h"

/*  Private functions, do not call externally. */
struct aggregate_type_specifier_id make_aggregate_type_specifier(struct asm_state *,enum asm_aggregate_type_specifier_kind,unsigned int);
struct simple_type_specifier_id make_simple_type_specifier(struct asm_state *,enum asm_simple_type_specifier_kind);
struct simple_type_qualifier_id make_simple_type_qualifier(struct asm_state *,enum asm_simple_type_qualifier_kind);
struct simple_storage_class_specifier_id make_simple_storage_class_specifier(struct asm_state *,enum asm_simple_storage_class_specifier_kind);
struct specifier_or_qualifier_id make_specifier_or_qualifier(struct asm_state *,enum asm_specifier_or_qualifier_kind,unsigned int);
struct specifier_or_qualifier_list_item_id make_specifier_or_qualifier_list_item(struct asm_state *,enum asm_specifier_or_qualifier_list_item_kind,struct specifier_or_qualifier_list_item_id,struct specifier_or_qualifier_id);
struct parameter_list_id make_parameter_list(struct asm_state *,enum asm_parameter_list_kind,struct general_type_list_item_id);
struct initializer_id make_initializer(struct asm_state *,enum asm_initializer_kind,unsigned int);
struct declarator_part_id make_declarator_part(struct asm_state *,enum asm_declarator_part_kind,unsigned int);
struct any_statement_list_item_id make_any_statement_list_item(struct asm_state *,enum asm_any_statement_list_item_kind,struct any_statement_list_item_id,struct any_statement_id);
struct anonymous_tag_definition_id make_anonymous_tag_definition(struct asm_state *,enum asm_anonymous_tag_definition_kind,struct tag_definition_id,struct scope_guid_id,struct anonymous_instance_id);
struct named_tag_predeclaration_id make_named_tag_predeclaration(struct asm_state *,enum asm_named_tag_predeclaration_kind,struct scope_guid_id,struct identifier_id);
struct named_tag_definition_id make_named_tag_definition(struct asm_state *,enum asm_named_tag_definition_kind,struct tag_definition_id,struct scope_guid_id,struct identifier_id);
struct tag_definition_id make_tag_definition(struct asm_state *,enum asm_tag_definition_kind,unsigned int);
struct struct_or_union_member_list_item_id make_struct_or_union_member_list_item(struct asm_state *,enum asm_struct_or_union_member_list_item_kind,struct struct_or_union_member_list_item_id,struct struct_or_union_member_id);
struct enum_member_list_item_id make_enum_member_list_item(struct asm_state *,enum asm_enum_member_list_item_kind,struct enum_member_list_item_id,struct enum_member_id);
struct enum_member_id make_enum_member(struct asm_state *,enum asm_enum_member_kind,struct identifier_id,struct expression_id);
struct struct_or_union_member_id make_struct_or_union_member(struct asm_state *,enum asm_struct_or_union_member_kind,struct general_type_id,struct layout_flag_list_item_id,struct identifier_id);
struct layout_flag_list_item_id make_layout_flag_list_item(struct asm_state *,enum asm_layout_flag_list_item_kind,struct layout_flag_list_item_id,struct layout_flag_id);
struct layout_flag_id make_layout_flag(struct asm_state *,enum asm_layout_flag_kind);
struct initializer_list_item_id make_initializer_list_item(struct asm_state *,enum asm_initializer_list_item_kind,struct initializer_list_item_id,struct initializer_id);
struct expression_list_item_id make_expression_list_item(struct asm_state *,enum asm_expression_list_item_kind,struct expression_list_item_id,struct expression_id);
struct declarator_part_list_item_id make_declarator_part_list_item(struct asm_state *,enum asm_declarator_part_list_item_kind,struct declarator_part_list_item_id,struct declarator_part_id);
struct pointer_part_id make_pointer_part(struct asm_state *,enum asm_pointer_part_kind,struct specifier_or_qualifier_list_item_id);
struct function_part_id make_function_part(struct asm_state *,enum asm_function_part_kind,struct parameter_list_id);
struct array_part_id make_array_part(struct asm_state *,enum asm_array_part_kind,struct expression_id);
struct general_type_id make_general_type(struct asm_state *,enum asm_general_type_kind,struct specifier_or_qualifier_list_item_id,struct declarator_part_list_item_id,struct bitfield_id);
struct typedef_entry_id make_typedef_entry(struct asm_state *,enum asm_typedef_entry_kind,struct identifier_id,struct scope_guid_id,struct general_type_id);
struct general_type_list_item_id make_general_type_list_item(struct asm_state *,enum asm_general_type_list_item_kind,struct general_type_list_item_id,struct general_type_id);
struct postfix_expression_part_list_item_id make_postfix_expression_part_list_item(struct asm_state *,enum asm_postfix_expression_part_list_item_kind,struct postfix_expression_part_list_item_id,struct postfix_expression_part_id);
struct postfix_expression_part_id make_postfix_expression_part(struct asm_state *,enum asm_postfix_expression_part_kind,unsigned int);
struct unscoped_tag_specifier_id make_unscoped_tag_specifier(struct asm_state *,enum asm_unscoped_tag_specifier_kind,unsigned int);
struct scoped_typename_specifier_id make_scoped_typename_specifier(struct asm_state *,enum asm_scoped_typename_specifier_kind,struct scope_guid_id,struct identifier_id);
struct scoped_tag_specifier_id make_scoped_tag_specifier(struct asm_state *,enum asm_scoped_tag_specifier_kind,struct scope_guid_id,struct unscoped_tag_specifier_id);
struct unscoped_union_specifier_id make_unscoped_union_specifier(struct asm_state *,enum asm_unscoped_union_specifier_kind,unsigned int);
struct unscoped_struct_specifier_id make_unscoped_struct_specifier(struct asm_state *,enum asm_unscoped_struct_specifier_kind,unsigned int);
struct unscoped_enum_specifier_id make_unscoped_enum_specifier(struct asm_state *,enum asm_unscoped_enum_specifier_kind,unsigned int);
struct bitfield_id make_bitfield(struct asm_state *,enum asm_bitfield_kind,struct expression_id);
struct jump_statement_id make_jump_statement(struct asm_state *,enum asm_jump_statement_kind,unsigned int);
struct switch_statement_id make_switch_statement(struct asm_state *,enum asm_switch_statement_kind,struct expression_id,struct any_statement_id);
struct if_statement_id make_if_statement(struct asm_state *,enum asm_if_statement_kind,struct any_statement_id,struct expression_id,struct any_statement_id);
struct while_statement_id make_while_statement(struct asm_state *,enum asm_while_statement_kind,struct expression_id,struct any_statement_id);
struct for_statement_id make_for_statement(struct asm_state *,enum asm_for_statement_kind,struct expression_list_item_id,struct expression_list_item_id,struct expression_list_item_id);
struct code_statement_id make_code_statement(struct asm_state *,enum asm_code_statement_kind,unsigned int);
struct labeled_statement_id make_labeled_statement(struct asm_state *,enum asm_labeled_statement_kind,struct any_statement_id,struct expression_id);
struct postfix_expression_id make_postfix_expression(struct asm_state *,enum asm_postfix_expression_kind,struct primary_expression_id,struct postfix_expression_part_list_item_id);
struct primary_expression_id make_primary_expression(struct asm_state *,enum asm_primary_expression_kind,unsigned int);
struct any_statement_id make_any_statement(struct asm_state *,enum asm_any_statement_kind,unsigned int);
struct scoped_statement_id make_scoped_statement(struct asm_state *,enum asm_scoped_statement_kind,struct any_statement_list_item_id,struct scope_guid_id);
struct declaration_statement_id make_declaration_statement(struct asm_state *,enum asm_declaration_statement_kind,unsigned int);
struct function_definition_id make_function_definition(struct asm_state *,enum asm_function_definition_kind,struct general_type_id,struct any_statement_list_item_id,struct any_statement_list_item_id,struct identifier_id);
struct simple_declaration_id make_simple_declaration(struct asm_state *,enum asm_simple_declaration_kind,struct general_type_id,struct identifier_id,struct initializer_id);
struct expression_id make_expression(struct asm_state *,enum asm_expression_kind,unsigned int);
struct cast_expression_id make_cast_expression(struct asm_state *,enum asm_cast_expression_kind,unsigned int,struct general_type_id);
struct unary_expression_id make_unary_expression(struct asm_state *,enum asm_unary_expression_kind,unsigned int);
struct binary_expression_id make_binary_expression(struct asm_state *,enum asm_binary_expression_kind,struct expression_id,struct expression_id);
struct ternary_expression_id make_ternary_expression(struct asm_state *,enum asm_ternary_expression_kind,struct expression_id,struct expression_id,struct expression_id);
struct constant_description_id make_constant_description(struct asm_state *,enum asm_constant_description_kind,struct general_type_id,struct data_package_id);
struct data_package_id make_data_package(struct asm_state *,enum asm_data_package_kind,unsigned int);
struct error_node_id make_error_node(struct asm_state *,enum asm_error_node_kind);
struct byte_package_id make_byte_package(struct asm_state *,enum asm_byte_package_kind,unsigned char);
struct hword_package_id make_hword_package(struct asm_state *,enum asm_hword_package_kind,unsigned char,unsigned char);
struct word_package_id make_word_package(struct asm_state *,enum asm_word_package_kind,unsigned int);
struct qword_package_id make_qword_package(struct asm_state *,enum asm_qword_package_kind,unsigned int,unsigned int,unsigned int,unsigned int);
struct dword_package_id make_dword_package(struct asm_state *,enum asm_dword_package_kind,unsigned int,unsigned int);
struct string_package_id make_string_package(struct asm_state *,enum asm_string_package_kind,unsigned int,unsigned char *);
struct identifier_id make_identifier(struct asm_state *,enum asm_identifier_kind,struct c_lexer_token *);
struct scope_guid_id make_scope_guid(struct asm_state *,enum asm_scope_guid_kind,unsigned int);
struct anonymous_instance_id make_anonymous_instance(struct asm_state *,enum asm_anonymous_instance_kind);
unsigned int undo_basic_asm_operation(struct asm_state * state, enum asm_operation_type o){
	switch(o){
		case ASM_OP_ADD_AGGREGATE_TYPE_SPECIFIER:{
			struct aggregate_type_specifier_details d = struct_aggregate_type_specifier_details_list_pop_end(&state->aggregate_type_specifiers);
			struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_remove(&state->aggregate_type_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SIMPLE_TYPE_SPECIFIER:{
			struct simple_type_specifier_details d = struct_simple_type_specifier_details_list_pop_end(&state->simple_type_specifiers);
			struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_remove(&state->simple_type_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SIMPLE_TYPE_QUALIFIER:{
			struct simple_type_qualifier_details d = struct_simple_type_qualifier_details_list_pop_end(&state->simple_type_qualifiers);
			struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_remove(&state->simple_type_qualifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SIMPLE_STORAGE_CLASS_SPECIFIER:{
			struct simple_storage_class_specifier_details d = struct_simple_storage_class_specifier_details_list_pop_end(&state->simple_storage_class_specifiers);
			struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_remove(&state->simple_storage_class_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SPECIFIER_OR_QUALIFIER:{
			struct specifier_or_qualifier_details d = struct_specifier_or_qualifier_details_list_pop_end(&state->specifier_or_qualifiers);
			struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_remove(&state->specifier_or_qualifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SPECIFIER_OR_QUALIFIER_LIST_ITEM:{
			struct specifier_or_qualifier_list_item_details d = struct_specifier_or_qualifier_list_item_details_list_pop_end(&state->specifier_or_qualifier_list_items);
			struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_remove(&state->specifier_or_qualifier_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_PARAMETER_LIST:{
			struct parameter_list_details d = struct_parameter_list_details_list_pop_end(&state->parameter_lists);
			struct_parameter_list_details_to_struct_parameter_list_id_map_remove(&state->parameter_lists_map, d);
			return 0;
		}case ASM_OP_ADD_INITIALIZER:{
			struct initializer_details d = struct_initializer_details_list_pop_end(&state->initializers);
			struct_initializer_details_to_struct_initializer_id_map_remove(&state->initializers_map, d);
			return 0;
		}case ASM_OP_ADD_DECLARATOR_PART:{
			struct declarator_part_details d = struct_declarator_part_details_list_pop_end(&state->declarator_parts);
			struct_declarator_part_details_to_struct_declarator_part_id_map_remove(&state->declarator_parts_map, d);
			return 0;
		}case ASM_OP_ADD_ANY_STATEMENT_LIST_ITEM:{
			struct any_statement_list_item_details d = struct_any_statement_list_item_details_list_pop_end(&state->any_statement_list_items);
			struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map_remove(&state->any_statement_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_ANONYMOUS_TAG_DEFINITION:{
			struct anonymous_tag_definition_details d = struct_anonymous_tag_definition_details_list_pop_end(&state->anonymous_tag_definitions);
			struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map_remove(&state->anonymous_tag_definitions_map, d);
			return 0;
		}case ASM_OP_ADD_NAMED_TAG_PREDECLARATION:{
			struct named_tag_predeclaration_details d = struct_named_tag_predeclaration_details_list_pop_end(&state->named_tag_predeclarations);
			struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map_remove(&state->named_tag_predeclarations_map, d);
			return 0;
		}case ASM_OP_ADD_NAMED_TAG_DEFINITION:{
			struct named_tag_definition_details d = struct_named_tag_definition_details_list_pop_end(&state->named_tag_definitions);
			struct_named_tag_definition_details_to_struct_named_tag_definition_id_map_remove(&state->named_tag_definitions_map, d);
			return 0;
		}case ASM_OP_ADD_TAG_DEFINITION:{
			struct tag_definition_details d = struct_tag_definition_details_list_pop_end(&state->tag_definitions);
			struct_tag_definition_details_to_struct_tag_definition_id_map_remove(&state->tag_definitions_map, d);
			return 0;
		}case ASM_OP_ADD_STRUCT_OR_UNION_MEMBER_LIST_ITEM:{
			struct struct_or_union_member_list_item_details d = struct_struct_or_union_member_list_item_details_list_pop_end(&state->struct_or_union_member_list_items);
			struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map_remove(&state->struct_or_union_member_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_ENUM_MEMBER_LIST_ITEM:{
			struct enum_member_list_item_details d = struct_enum_member_list_item_details_list_pop_end(&state->enum_member_list_items);
			struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map_remove(&state->enum_member_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_ENUM_MEMBER:{
			struct enum_member_details d = struct_enum_member_details_list_pop_end(&state->enum_members);
			struct_enum_member_details_to_struct_enum_member_id_map_remove(&state->enum_members_map, d);
			return 0;
		}case ASM_OP_ADD_STRUCT_OR_UNION_MEMBER:{
			struct struct_or_union_member_details d = struct_struct_or_union_member_details_list_pop_end(&state->struct_or_union_members);
			struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map_remove(&state->struct_or_union_members_map, d);
			return 0;
		}case ASM_OP_ADD_LAYOUT_FLAG_LIST_ITEM:{
			struct layout_flag_list_item_details d = struct_layout_flag_list_item_details_list_pop_end(&state->layout_flag_list_items);
			struct_layout_flag_list_item_details_to_struct_layout_flag_list_item_id_map_remove(&state->layout_flag_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_LAYOUT_FLAG:{
			struct layout_flag_details d = struct_layout_flag_details_list_pop_end(&state->layout_flags);
			struct_layout_flag_details_to_struct_layout_flag_id_map_remove(&state->layout_flags_map, d);
			return 0;
		}case ASM_OP_ADD_INITIALIZER_LIST_ITEM:{
			struct initializer_list_item_details d = struct_initializer_list_item_details_list_pop_end(&state->initializer_list_items);
			struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_remove(&state->initializer_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_EXPRESSION_LIST_ITEM:{
			struct expression_list_item_details d = struct_expression_list_item_details_list_pop_end(&state->expression_list_items);
			struct_expression_list_item_details_to_struct_expression_list_item_id_map_remove(&state->expression_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_DECLARATOR_PART_LIST_ITEM:{
			struct declarator_part_list_item_details d = struct_declarator_part_list_item_details_list_pop_end(&state->declarator_part_list_items);
			struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_remove(&state->declarator_part_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_POINTER_PART:{
			struct pointer_part_details d = struct_pointer_part_details_list_pop_end(&state->pointer_parts);
			struct_pointer_part_details_to_struct_pointer_part_id_map_remove(&state->pointer_parts_map, d);
			return 0;
		}case ASM_OP_ADD_FUNCTION_PART:{
			struct function_part_details d = struct_function_part_details_list_pop_end(&state->function_parts);
			struct_function_part_details_to_struct_function_part_id_map_remove(&state->function_parts_map, d);
			return 0;
		}case ASM_OP_ADD_ARRAY_PART:{
			struct array_part_details d = struct_array_part_details_list_pop_end(&state->array_parts);
			struct_array_part_details_to_struct_array_part_id_map_remove(&state->array_parts_map, d);
			return 0;
		}case ASM_OP_ADD_GENERAL_TYPE:{
			struct general_type_details d = struct_general_type_details_list_pop_end(&state->general_types);
			struct_general_type_details_to_struct_general_type_id_map_remove(&state->general_types_map, d);
			return 0;
		}case ASM_OP_ADD_TYPEDEF_ENTRY:{
			struct typedef_entry_details d = struct_typedef_entry_details_list_pop_end(&state->typedef_entrys);
			struct_typedef_entry_details_to_struct_typedef_entry_id_map_remove(&state->typedef_entrys_map, d);
			return 0;
		}case ASM_OP_ADD_GENERAL_TYPE_LIST_ITEM:{
			struct general_type_list_item_details d = struct_general_type_list_item_details_list_pop_end(&state->general_type_list_items);
			struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_remove(&state->general_type_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_POSTFIX_EXPRESSION_PART_LIST_ITEM:{
			struct postfix_expression_part_list_item_details d = struct_postfix_expression_part_list_item_details_list_pop_end(&state->postfix_expression_part_list_items);
			struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map_remove(&state->postfix_expression_part_list_items_map, d);
			return 0;
		}case ASM_OP_ADD_POSTFIX_EXPRESSION_PART:{
			struct postfix_expression_part_details d = struct_postfix_expression_part_details_list_pop_end(&state->postfix_expression_parts);
			struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map_remove(&state->postfix_expression_parts_map, d);
			return 0;
		}case ASM_OP_ADD_UNSCOPED_TAG_SPECIFIER:{
			struct unscoped_tag_specifier_details d = struct_unscoped_tag_specifier_details_list_pop_end(&state->unscoped_tag_specifiers);
			struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_remove(&state->unscoped_tag_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SCOPED_TYPENAME_SPECIFIER:{
			struct scoped_typename_specifier_details d = struct_scoped_typename_specifier_details_list_pop_end(&state->scoped_typename_specifiers);
			struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map_remove(&state->scoped_typename_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SCOPED_TAG_SPECIFIER:{
			struct scoped_tag_specifier_details d = struct_scoped_tag_specifier_details_list_pop_end(&state->scoped_tag_specifiers);
			struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_remove(&state->scoped_tag_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_UNSCOPED_UNION_SPECIFIER:{
			struct unscoped_union_specifier_details d = struct_unscoped_union_specifier_details_list_pop_end(&state->unscoped_union_specifiers);
			struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map_remove(&state->unscoped_union_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_UNSCOPED_STRUCT_SPECIFIER:{
			struct unscoped_struct_specifier_details d = struct_unscoped_struct_specifier_details_list_pop_end(&state->unscoped_struct_specifiers);
			struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map_remove(&state->unscoped_struct_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_UNSCOPED_ENUM_SPECIFIER:{
			struct unscoped_enum_specifier_details d = struct_unscoped_enum_specifier_details_list_pop_end(&state->unscoped_enum_specifiers);
			struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map_remove(&state->unscoped_enum_specifiers_map, d);
			return 0;
		}case ASM_OP_ADD_BITFIELD:{
			struct bitfield_details d = struct_bitfield_details_list_pop_end(&state->bitfields);
			struct_bitfield_details_to_struct_bitfield_id_map_remove(&state->bitfields_map, d);
			return 0;
		}case ASM_OP_ADD_JUMP_STATEMENT:{
			struct jump_statement_details d = struct_jump_statement_details_list_pop_end(&state->jump_statements);
			struct_jump_statement_details_to_struct_jump_statement_id_map_remove(&state->jump_statements_map, d);
			return 0;
		}case ASM_OP_ADD_SWITCH_STATEMENT:{
			struct switch_statement_details d = struct_switch_statement_details_list_pop_end(&state->switch_statements);
			struct_switch_statement_details_to_struct_switch_statement_id_map_remove(&state->switch_statements_map, d);
			return 0;
		}case ASM_OP_ADD_IF_STATEMENT:{
			struct if_statement_details d = struct_if_statement_details_list_pop_end(&state->if_statements);
			struct_if_statement_details_to_struct_if_statement_id_map_remove(&state->if_statements_map, d);
			return 0;
		}case ASM_OP_ADD_WHILE_STATEMENT:{
			struct while_statement_details d = struct_while_statement_details_list_pop_end(&state->while_statements);
			struct_while_statement_details_to_struct_while_statement_id_map_remove(&state->while_statements_map, d);
			return 0;
		}case ASM_OP_ADD_FOR_STATEMENT:{
			struct for_statement_details d = struct_for_statement_details_list_pop_end(&state->for_statements);
			struct_for_statement_details_to_struct_for_statement_id_map_remove(&state->for_statements_map, d);
			return 0;
		}case ASM_OP_ADD_CODE_STATEMENT:{
			struct code_statement_details d = struct_code_statement_details_list_pop_end(&state->code_statements);
			struct_code_statement_details_to_struct_code_statement_id_map_remove(&state->code_statements_map, d);
			return 0;
		}case ASM_OP_ADD_LABELED_STATEMENT:{
			struct labeled_statement_details d = struct_labeled_statement_details_list_pop_end(&state->labeled_statements);
			struct_labeled_statement_details_to_struct_labeled_statement_id_map_remove(&state->labeled_statements_map, d);
			return 0;
		}case ASM_OP_ADD_POSTFIX_EXPRESSION:{
			struct postfix_expression_details d = struct_postfix_expression_details_list_pop_end(&state->postfix_expressions);
			struct_postfix_expression_details_to_struct_postfix_expression_id_map_remove(&state->postfix_expressions_map, d);
			return 0;
		}case ASM_OP_ADD_PRIMARY_EXPRESSION:{
			struct primary_expression_details d = struct_primary_expression_details_list_pop_end(&state->primary_expressions);
			struct_primary_expression_details_to_struct_primary_expression_id_map_remove(&state->primary_expressions_map, d);
			return 0;
		}case ASM_OP_ADD_ANY_STATEMENT:{
			struct any_statement_details d = struct_any_statement_details_list_pop_end(&state->any_statements);
			struct_any_statement_details_to_struct_any_statement_id_map_remove(&state->any_statements_map, d);
			return 0;
		}case ASM_OP_ADD_SCOPED_STATEMENT:{
			struct scoped_statement_details d = struct_scoped_statement_details_list_pop_end(&state->scoped_statements);
			struct_scoped_statement_details_to_struct_scoped_statement_id_map_remove(&state->scoped_statements_map, d);
			return 0;
		}case ASM_OP_ADD_DECLARATION_STATEMENT:{
			struct declaration_statement_details d = struct_declaration_statement_details_list_pop_end(&state->declaration_statements);
			struct_declaration_statement_details_to_struct_declaration_statement_id_map_remove(&state->declaration_statements_map, d);
			return 0;
		}case ASM_OP_ADD_FUNCTION_DEFINITION:{
			struct function_definition_details d = struct_function_definition_details_list_pop_end(&state->function_definitions);
			struct_function_definition_details_to_struct_function_definition_id_map_remove(&state->function_definitions_map, d);
			return 0;
		}case ASM_OP_ADD_SIMPLE_DECLARATION:{
			struct simple_declaration_details d = struct_simple_declaration_details_list_pop_end(&state->simple_declarations);
			struct_simple_declaration_details_to_struct_simple_declaration_id_map_remove(&state->simple_declarations_map, d);
			return 0;
		}case ASM_OP_ADD_EXPRESSION:{
			struct expression_details d = struct_expression_details_list_pop_end(&state->expressions);
			struct_expression_details_to_struct_expression_id_map_remove(&state->expressions_map, d);
			return 0;
		}case ASM_OP_ADD_CAST_EXPRESSION:{
			struct cast_expression_details d = struct_cast_expression_details_list_pop_end(&state->cast_expressions);
			struct_cast_expression_details_to_struct_cast_expression_id_map_remove(&state->cast_expressions_map, d);
			return 0;
		}case ASM_OP_ADD_UNARY_EXPRESSION:{
			struct unary_expression_details d = struct_unary_expression_details_list_pop_end(&state->unary_expressions);
			struct_unary_expression_details_to_struct_unary_expression_id_map_remove(&state->unary_expressions_map, d);
			return 0;
		}case ASM_OP_ADD_BINARY_EXPRESSION:{
			struct binary_expression_details d = struct_binary_expression_details_list_pop_end(&state->binary_expressions);
			struct_binary_expression_details_to_struct_binary_expression_id_map_remove(&state->binary_expressions_map, d);
			return 0;
		}case ASM_OP_ADD_TERNARY_EXPRESSION:{
			struct ternary_expression_details d = struct_ternary_expression_details_list_pop_end(&state->ternary_expressions);
			struct_ternary_expression_details_to_struct_ternary_expression_id_map_remove(&state->ternary_expressions_map, d);
			return 0;
		}case ASM_OP_ADD_CONSTANT_DESCRIPTION:{
			struct constant_description_details d = struct_constant_description_details_list_pop_end(&state->constant_descriptions);
			struct_constant_description_details_to_struct_constant_description_id_map_remove(&state->constant_descriptions_map, d);
			return 0;
		}case ASM_OP_ADD_DATA_PACKAGE:{
			struct data_package_details d = struct_data_package_details_list_pop_end(&state->data_packages);
			struct_data_package_details_to_struct_data_package_id_map_remove(&state->data_packages_map, d);
			return 0;
		}case ASM_OP_ADD_ERROR_NODE:{
			struct error_node_details d = struct_error_node_details_list_pop_end(&state->error_nodes);
			struct_error_node_details_to_struct_error_node_id_map_remove(&state->error_nodes_map, d);
			return 0;
		}case ASM_OP_ADD_BYTE_PACKAGE:{
			struct byte_package_details d = struct_byte_package_details_list_pop_end(&state->byte_packages);
			struct_byte_package_details_to_struct_byte_package_id_map_remove(&state->byte_packages_map, d);
			return 0;
		}case ASM_OP_ADD_HWORD_PACKAGE:{
			struct hword_package_details d = struct_hword_package_details_list_pop_end(&state->hword_packages);
			struct_hword_package_details_to_struct_hword_package_id_map_remove(&state->hword_packages_map, d);
			return 0;
		}case ASM_OP_ADD_WORD_PACKAGE:{
			struct word_package_details d = struct_word_package_details_list_pop_end(&state->word_packages);
			struct_word_package_details_to_struct_word_package_id_map_remove(&state->word_packages_map, d);
			return 0;
		}case ASM_OP_ADD_QWORD_PACKAGE:{
			struct qword_package_details d = struct_qword_package_details_list_pop_end(&state->qword_packages);
			struct_qword_package_details_to_struct_qword_package_id_map_remove(&state->qword_packages_map, d);
			return 0;
		}case ASM_OP_ADD_DWORD_PACKAGE:{
			struct dword_package_details d = struct_dword_package_details_list_pop_end(&state->dword_packages);
			struct_dword_package_details_to_struct_dword_package_id_map_remove(&state->dword_packages_map, d);
			return 0;
		}case ASM_OP_ADD_STRING_PACKAGE:{
			struct string_package_details d = struct_string_package_details_list_pop_end(&state->string_packages);
			struct_string_package_details_to_struct_string_package_id_map_remove(&state->string_packages_map, d);
			return 0;
		}case ASM_OP_ADD_IDENTIFIER:{
			struct identifier_details d = struct_identifier_details_list_pop_end(&state->identifiers);
			struct_identifier_details_to_struct_identifier_id_map_remove(&state->identifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SCOPE_GUID:{
			struct scope_guid_details d = struct_scope_guid_details_list_pop_end(&state->scope_guids);
			struct_scope_guid_details_to_struct_scope_guid_id_map_remove(&state->scope_guids_map, d);
			state->last_scope_guid--;
			return 0;
		}case ASM_OP_ADD_ANONYMOUS_INSTANCE:{
			struct anonymous_instance_details d = struct_anonymous_instance_details_list_pop_end(&state->anonymous_instances);
			struct_anonymous_instance_details_to_struct_anonymous_instance_id_map_remove(&state->anonymous_instances_map, d);
			return 0;
		}default:{
			return 1;
		}
	}

}

void create_asm_state(struct asm_state * state, struct memory_pool_collection * m){
	(void)m;
	enum_asm_operation_type_list_create(&state->reversible_operations);
	struct_aggregate_type_specifier_details_list_create(&state->aggregate_type_specifiers);
	struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_create(&state->aggregate_type_specifiers_map, struct_struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_key_value_pair_compare);
	struct_simple_type_specifier_details_list_create(&state->simple_type_specifiers);
	struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_create(&state->simple_type_specifiers_map, struct_struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_key_value_pair_compare);
	struct_simple_type_qualifier_details_list_create(&state->simple_type_qualifiers);
	struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_create(&state->simple_type_qualifiers_map, struct_struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_key_value_pair_compare);
	struct_simple_storage_class_specifier_details_list_create(&state->simple_storage_class_specifiers);
	struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_create(&state->simple_storage_class_specifiers_map, struct_struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_key_value_pair_compare);
	struct_specifier_or_qualifier_details_list_create(&state->specifier_or_qualifiers);
	struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_create(&state->specifier_or_qualifiers_map, struct_struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_key_value_pair_compare);
	struct_specifier_or_qualifier_list_item_details_list_create(&state->specifier_or_qualifier_list_items);
	struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_create(&state->specifier_or_qualifier_list_items_map, struct_struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_key_value_pair_compare);
	struct_parameter_list_details_list_create(&state->parameter_lists);
	struct_parameter_list_details_to_struct_parameter_list_id_map_create(&state->parameter_lists_map, struct_struct_parameter_list_details_to_struct_parameter_list_id_key_value_pair_compare);
	struct_initializer_details_list_create(&state->initializers);
	struct_initializer_details_to_struct_initializer_id_map_create(&state->initializers_map, struct_struct_initializer_details_to_struct_initializer_id_key_value_pair_compare);
	struct_declarator_part_details_list_create(&state->declarator_parts);
	struct_declarator_part_details_to_struct_declarator_part_id_map_create(&state->declarator_parts_map, struct_struct_declarator_part_details_to_struct_declarator_part_id_key_value_pair_compare);
	struct_any_statement_list_item_details_list_create(&state->any_statement_list_items);
	struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map_create(&state->any_statement_list_items_map, struct_struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_key_value_pair_compare);
	struct_anonymous_tag_definition_details_list_create(&state->anonymous_tag_definitions);
	struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map_create(&state->anonymous_tag_definitions_map, struct_struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_key_value_pair_compare);
	struct_named_tag_predeclaration_details_list_create(&state->named_tag_predeclarations);
	struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map_create(&state->named_tag_predeclarations_map, struct_struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_key_value_pair_compare);
	struct_named_tag_definition_details_list_create(&state->named_tag_definitions);
	struct_named_tag_definition_details_to_struct_named_tag_definition_id_map_create(&state->named_tag_definitions_map, struct_struct_named_tag_definition_details_to_struct_named_tag_definition_id_key_value_pair_compare);
	struct_tag_definition_details_list_create(&state->tag_definitions);
	struct_tag_definition_details_to_struct_tag_definition_id_map_create(&state->tag_definitions_map, struct_struct_tag_definition_details_to_struct_tag_definition_id_key_value_pair_compare);
	struct_struct_or_union_member_list_item_details_list_create(&state->struct_or_union_member_list_items);
	struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map_create(&state->struct_or_union_member_list_items_map, struct_struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_key_value_pair_compare);
	struct_enum_member_list_item_details_list_create(&state->enum_member_list_items);
	struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map_create(&state->enum_member_list_items_map, struct_struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_key_value_pair_compare);
	struct_enum_member_details_list_create(&state->enum_members);
	struct_enum_member_details_to_struct_enum_member_id_map_create(&state->enum_members_map, struct_struct_enum_member_details_to_struct_enum_member_id_key_value_pair_compare);
	struct_struct_or_union_member_details_list_create(&state->struct_or_union_members);
	struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map_create(&state->struct_or_union_members_map, struct_struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_key_value_pair_compare);
	struct_layout_flag_list_item_details_list_create(&state->layout_flag_list_items);
	struct_layout_flag_list_item_details_to_struct_layout_flag_list_item_id_map_create(&state->layout_flag_list_items_map, struct_struct_layout_flag_list_item_details_to_struct_layout_flag_list_item_id_key_value_pair_compare);
	struct_layout_flag_details_list_create(&state->layout_flags);
	struct_layout_flag_details_to_struct_layout_flag_id_map_create(&state->layout_flags_map, struct_struct_layout_flag_details_to_struct_layout_flag_id_key_value_pair_compare);
	struct_initializer_list_item_details_list_create(&state->initializer_list_items);
	struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_create(&state->initializer_list_items_map, struct_struct_initializer_list_item_details_to_struct_initializer_list_item_id_key_value_pair_compare);
	struct_expression_list_item_details_list_create(&state->expression_list_items);
	struct_expression_list_item_details_to_struct_expression_list_item_id_map_create(&state->expression_list_items_map, struct_struct_expression_list_item_details_to_struct_expression_list_item_id_key_value_pair_compare);
	struct_declarator_part_list_item_details_list_create(&state->declarator_part_list_items);
	struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_create(&state->declarator_part_list_items_map, struct_struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_key_value_pair_compare);
	struct_pointer_part_details_list_create(&state->pointer_parts);
	struct_pointer_part_details_to_struct_pointer_part_id_map_create(&state->pointer_parts_map, struct_struct_pointer_part_details_to_struct_pointer_part_id_key_value_pair_compare);
	struct_function_part_details_list_create(&state->function_parts);
	struct_function_part_details_to_struct_function_part_id_map_create(&state->function_parts_map, struct_struct_function_part_details_to_struct_function_part_id_key_value_pair_compare);
	struct_array_part_details_list_create(&state->array_parts);
	struct_array_part_details_to_struct_array_part_id_map_create(&state->array_parts_map, struct_struct_array_part_details_to_struct_array_part_id_key_value_pair_compare);
	struct_general_type_details_list_create(&state->general_types);
	struct_general_type_details_to_struct_general_type_id_map_create(&state->general_types_map, struct_struct_general_type_details_to_struct_general_type_id_key_value_pair_compare);
	struct_typedef_entry_details_list_create(&state->typedef_entrys);
	struct_typedef_entry_details_to_struct_typedef_entry_id_map_create(&state->typedef_entrys_map, struct_struct_typedef_entry_details_to_struct_typedef_entry_id_key_value_pair_compare);
	struct_general_type_list_item_details_list_create(&state->general_type_list_items);
	struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_create(&state->general_type_list_items_map, struct_struct_general_type_list_item_details_to_struct_general_type_list_item_id_key_value_pair_compare);
	struct_postfix_expression_part_list_item_details_list_create(&state->postfix_expression_part_list_items);
	struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map_create(&state->postfix_expression_part_list_items_map, struct_struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_key_value_pair_compare);
	struct_postfix_expression_part_details_list_create(&state->postfix_expression_parts);
	struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map_create(&state->postfix_expression_parts_map, struct_struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_key_value_pair_compare);
	struct_unscoped_tag_specifier_details_list_create(&state->unscoped_tag_specifiers);
	struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_create(&state->unscoped_tag_specifiers_map, struct_struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_key_value_pair_compare);
	struct_scoped_typename_specifier_details_list_create(&state->scoped_typename_specifiers);
	struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map_create(&state->scoped_typename_specifiers_map, struct_struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_key_value_pair_compare);
	struct_scoped_tag_specifier_details_list_create(&state->scoped_tag_specifiers);
	struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_create(&state->scoped_tag_specifiers_map, struct_struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_key_value_pair_compare);
	struct_unscoped_union_specifier_details_list_create(&state->unscoped_union_specifiers);
	struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map_create(&state->unscoped_union_specifiers_map, struct_struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_key_value_pair_compare);
	struct_unscoped_struct_specifier_details_list_create(&state->unscoped_struct_specifiers);
	struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map_create(&state->unscoped_struct_specifiers_map, struct_struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_key_value_pair_compare);
	struct_unscoped_enum_specifier_details_list_create(&state->unscoped_enum_specifiers);
	struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map_create(&state->unscoped_enum_specifiers_map, struct_struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_key_value_pair_compare);
	struct_bitfield_details_list_create(&state->bitfields);
	struct_bitfield_details_to_struct_bitfield_id_map_create(&state->bitfields_map, struct_struct_bitfield_details_to_struct_bitfield_id_key_value_pair_compare);
	struct_jump_statement_details_list_create(&state->jump_statements);
	struct_jump_statement_details_to_struct_jump_statement_id_map_create(&state->jump_statements_map, struct_struct_jump_statement_details_to_struct_jump_statement_id_key_value_pair_compare);
	struct_switch_statement_details_list_create(&state->switch_statements);
	struct_switch_statement_details_to_struct_switch_statement_id_map_create(&state->switch_statements_map, struct_struct_switch_statement_details_to_struct_switch_statement_id_key_value_pair_compare);
	struct_if_statement_details_list_create(&state->if_statements);
	struct_if_statement_details_to_struct_if_statement_id_map_create(&state->if_statements_map, struct_struct_if_statement_details_to_struct_if_statement_id_key_value_pair_compare);
	struct_while_statement_details_list_create(&state->while_statements);
	struct_while_statement_details_to_struct_while_statement_id_map_create(&state->while_statements_map, struct_struct_while_statement_details_to_struct_while_statement_id_key_value_pair_compare);
	struct_for_statement_details_list_create(&state->for_statements);
	struct_for_statement_details_to_struct_for_statement_id_map_create(&state->for_statements_map, struct_struct_for_statement_details_to_struct_for_statement_id_key_value_pair_compare);
	struct_code_statement_details_list_create(&state->code_statements);
	struct_code_statement_details_to_struct_code_statement_id_map_create(&state->code_statements_map, struct_struct_code_statement_details_to_struct_code_statement_id_key_value_pair_compare);
	struct_labeled_statement_details_list_create(&state->labeled_statements);
	struct_labeled_statement_details_to_struct_labeled_statement_id_map_create(&state->labeled_statements_map, struct_struct_labeled_statement_details_to_struct_labeled_statement_id_key_value_pair_compare);
	struct_postfix_expression_details_list_create(&state->postfix_expressions);
	struct_postfix_expression_details_to_struct_postfix_expression_id_map_create(&state->postfix_expressions_map, struct_struct_postfix_expression_details_to_struct_postfix_expression_id_key_value_pair_compare);
	struct_primary_expression_details_list_create(&state->primary_expressions);
	struct_primary_expression_details_to_struct_primary_expression_id_map_create(&state->primary_expressions_map, struct_struct_primary_expression_details_to_struct_primary_expression_id_key_value_pair_compare);
	struct_any_statement_details_list_create(&state->any_statements);
	struct_any_statement_details_to_struct_any_statement_id_map_create(&state->any_statements_map, struct_struct_any_statement_details_to_struct_any_statement_id_key_value_pair_compare);
	struct_scoped_statement_details_list_create(&state->scoped_statements);
	struct_scoped_statement_details_to_struct_scoped_statement_id_map_create(&state->scoped_statements_map, struct_struct_scoped_statement_details_to_struct_scoped_statement_id_key_value_pair_compare);
	struct_declaration_statement_details_list_create(&state->declaration_statements);
	struct_declaration_statement_details_to_struct_declaration_statement_id_map_create(&state->declaration_statements_map, struct_struct_declaration_statement_details_to_struct_declaration_statement_id_key_value_pair_compare);
	struct_function_definition_details_list_create(&state->function_definitions);
	struct_function_definition_details_to_struct_function_definition_id_map_create(&state->function_definitions_map, struct_struct_function_definition_details_to_struct_function_definition_id_key_value_pair_compare);
	struct_simple_declaration_details_list_create(&state->simple_declarations);
	struct_simple_declaration_details_to_struct_simple_declaration_id_map_create(&state->simple_declarations_map, struct_struct_simple_declaration_details_to_struct_simple_declaration_id_key_value_pair_compare);
	struct_expression_details_list_create(&state->expressions);
	struct_expression_details_to_struct_expression_id_map_create(&state->expressions_map, struct_struct_expression_details_to_struct_expression_id_key_value_pair_compare);
	struct_cast_expression_details_list_create(&state->cast_expressions);
	struct_cast_expression_details_to_struct_cast_expression_id_map_create(&state->cast_expressions_map, struct_struct_cast_expression_details_to_struct_cast_expression_id_key_value_pair_compare);
	struct_unary_expression_details_list_create(&state->unary_expressions);
	struct_unary_expression_details_to_struct_unary_expression_id_map_create(&state->unary_expressions_map, struct_struct_unary_expression_details_to_struct_unary_expression_id_key_value_pair_compare);
	struct_binary_expression_details_list_create(&state->binary_expressions);
	struct_binary_expression_details_to_struct_binary_expression_id_map_create(&state->binary_expressions_map, struct_struct_binary_expression_details_to_struct_binary_expression_id_key_value_pair_compare);
	struct_ternary_expression_details_list_create(&state->ternary_expressions);
	struct_ternary_expression_details_to_struct_ternary_expression_id_map_create(&state->ternary_expressions_map, struct_struct_ternary_expression_details_to_struct_ternary_expression_id_key_value_pair_compare);
	struct_constant_description_details_list_create(&state->constant_descriptions);
	struct_constant_description_details_to_struct_constant_description_id_map_create(&state->constant_descriptions_map, struct_struct_constant_description_details_to_struct_constant_description_id_key_value_pair_compare);
	struct_data_package_details_list_create(&state->data_packages);
	struct_data_package_details_to_struct_data_package_id_map_create(&state->data_packages_map, struct_struct_data_package_details_to_struct_data_package_id_key_value_pair_compare);
	struct_error_node_details_list_create(&state->error_nodes);
	struct_error_node_details_to_struct_error_node_id_map_create(&state->error_nodes_map, struct_struct_error_node_details_to_struct_error_node_id_key_value_pair_compare);
	struct_byte_package_details_list_create(&state->byte_packages);
	struct_byte_package_details_to_struct_byte_package_id_map_create(&state->byte_packages_map, struct_struct_byte_package_details_to_struct_byte_package_id_key_value_pair_compare);
	struct_hword_package_details_list_create(&state->hword_packages);
	struct_hword_package_details_to_struct_hword_package_id_map_create(&state->hword_packages_map, struct_struct_hword_package_details_to_struct_hword_package_id_key_value_pair_compare);
	struct_word_package_details_list_create(&state->word_packages);
	struct_word_package_details_to_struct_word_package_id_map_create(&state->word_packages_map, struct_struct_word_package_details_to_struct_word_package_id_key_value_pair_compare);
	struct_qword_package_details_list_create(&state->qword_packages);
	struct_qword_package_details_to_struct_qword_package_id_map_create(&state->qword_packages_map, struct_struct_qword_package_details_to_struct_qword_package_id_key_value_pair_compare);
	struct_dword_package_details_list_create(&state->dword_packages);
	struct_dword_package_details_to_struct_dword_package_id_map_create(&state->dword_packages_map, struct_struct_dword_package_details_to_struct_dword_package_id_key_value_pair_compare);
	struct_string_package_details_list_create(&state->string_packages);
	struct_string_package_details_to_struct_string_package_id_map_create(&state->string_packages_map, struct_struct_string_package_details_to_struct_string_package_id_key_value_pair_compare);
	struct_identifier_details_list_create(&state->identifiers);
	struct_identifier_details_to_struct_identifier_id_map_create(&state->identifiers_map, struct_struct_identifier_details_to_struct_identifier_id_key_value_pair_compare);
	struct_scope_guid_details_list_create(&state->scope_guids);
	struct_scope_guid_details_to_struct_scope_guid_id_map_create(&state->scope_guids_map, struct_struct_scope_guid_details_to_struct_scope_guid_id_key_value_pair_compare);
	struct_anonymous_instance_details_list_create(&state->anonymous_instances);
	struct_anonymous_instance_details_to_struct_anonymous_instance_id_map_create(&state->anonymous_instances_map, struct_struct_anonymous_instance_details_to_struct_anonymous_instance_id_key_value_pair_compare);
}

void destroy_asm_state(struct asm_state * state){
	enum_asm_operation_type_list_destroy(&state->reversible_operations);
	struct_aggregate_type_specifier_details_list_destroy(&state->aggregate_type_specifiers);
	struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_destroy(&state->aggregate_type_specifiers_map);
	struct_simple_type_specifier_details_list_destroy(&state->simple_type_specifiers);
	struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_destroy(&state->simple_type_specifiers_map);
	struct_simple_type_qualifier_details_list_destroy(&state->simple_type_qualifiers);
	struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_destroy(&state->simple_type_qualifiers_map);
	struct_simple_storage_class_specifier_details_list_destroy(&state->simple_storage_class_specifiers);
	struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_destroy(&state->simple_storage_class_specifiers_map);
	struct_specifier_or_qualifier_details_list_destroy(&state->specifier_or_qualifiers);
	struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_destroy(&state->specifier_or_qualifiers_map);
	struct_specifier_or_qualifier_list_item_details_list_destroy(&state->specifier_or_qualifier_list_items);
	struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_destroy(&state->specifier_or_qualifier_list_items_map);
	struct_parameter_list_details_list_destroy(&state->parameter_lists);
	struct_parameter_list_details_to_struct_parameter_list_id_map_destroy(&state->parameter_lists_map);
	struct_initializer_details_list_destroy(&state->initializers);
	struct_initializer_details_to_struct_initializer_id_map_destroy(&state->initializers_map);
	struct_declarator_part_details_list_destroy(&state->declarator_parts);
	struct_declarator_part_details_to_struct_declarator_part_id_map_destroy(&state->declarator_parts_map);
	struct_any_statement_list_item_details_list_destroy(&state->any_statement_list_items);
	struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map_destroy(&state->any_statement_list_items_map);
	struct_anonymous_tag_definition_details_list_destroy(&state->anonymous_tag_definitions);
	struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map_destroy(&state->anonymous_tag_definitions_map);
	struct_named_tag_predeclaration_details_list_destroy(&state->named_tag_predeclarations);
	struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map_destroy(&state->named_tag_predeclarations_map);
	struct_named_tag_definition_details_list_destroy(&state->named_tag_definitions);
	struct_named_tag_definition_details_to_struct_named_tag_definition_id_map_destroy(&state->named_tag_definitions_map);
	struct_tag_definition_details_list_destroy(&state->tag_definitions);
	struct_tag_definition_details_to_struct_tag_definition_id_map_destroy(&state->tag_definitions_map);
	struct_struct_or_union_member_list_item_details_list_destroy(&state->struct_or_union_member_list_items);
	struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map_destroy(&state->struct_or_union_member_list_items_map);
	struct_enum_member_list_item_details_list_destroy(&state->enum_member_list_items);
	struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map_destroy(&state->enum_member_list_items_map);
	struct_enum_member_details_list_destroy(&state->enum_members);
	struct_enum_member_details_to_struct_enum_member_id_map_destroy(&state->enum_members_map);
	struct_struct_or_union_member_details_list_destroy(&state->struct_or_union_members);
	struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map_destroy(&state->struct_or_union_members_map);
	struct_layout_flag_list_item_details_list_destroy(&state->layout_flag_list_items);
	struct_layout_flag_list_item_details_to_struct_layout_flag_list_item_id_map_destroy(&state->layout_flag_list_items_map);
	struct_layout_flag_details_list_destroy(&state->layout_flags);
	struct_layout_flag_details_to_struct_layout_flag_id_map_destroy(&state->layout_flags_map);
	struct_initializer_list_item_details_list_destroy(&state->initializer_list_items);
	struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_destroy(&state->initializer_list_items_map);
	struct_expression_list_item_details_list_destroy(&state->expression_list_items);
	struct_expression_list_item_details_to_struct_expression_list_item_id_map_destroy(&state->expression_list_items_map);
	struct_declarator_part_list_item_details_list_destroy(&state->declarator_part_list_items);
	struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_destroy(&state->declarator_part_list_items_map);
	struct_pointer_part_details_list_destroy(&state->pointer_parts);
	struct_pointer_part_details_to_struct_pointer_part_id_map_destroy(&state->pointer_parts_map);
	struct_function_part_details_list_destroy(&state->function_parts);
	struct_function_part_details_to_struct_function_part_id_map_destroy(&state->function_parts_map);
	struct_array_part_details_list_destroy(&state->array_parts);
	struct_array_part_details_to_struct_array_part_id_map_destroy(&state->array_parts_map);
	struct_general_type_details_list_destroy(&state->general_types);
	struct_general_type_details_to_struct_general_type_id_map_destroy(&state->general_types_map);
	struct_typedef_entry_details_list_destroy(&state->typedef_entrys);
	struct_typedef_entry_details_to_struct_typedef_entry_id_map_destroy(&state->typedef_entrys_map);
	struct_general_type_list_item_details_list_destroy(&state->general_type_list_items);
	struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_destroy(&state->general_type_list_items_map);
	struct_postfix_expression_part_list_item_details_list_destroy(&state->postfix_expression_part_list_items);
	struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map_destroy(&state->postfix_expression_part_list_items_map);
	struct_postfix_expression_part_details_list_destroy(&state->postfix_expression_parts);
	struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map_destroy(&state->postfix_expression_parts_map);
	struct_unscoped_tag_specifier_details_list_destroy(&state->unscoped_tag_specifiers);
	struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_destroy(&state->unscoped_tag_specifiers_map);
	struct_scoped_typename_specifier_details_list_destroy(&state->scoped_typename_specifiers);
	struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map_destroy(&state->scoped_typename_specifiers_map);
	struct_scoped_tag_specifier_details_list_destroy(&state->scoped_tag_specifiers);
	struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_destroy(&state->scoped_tag_specifiers_map);
	struct_unscoped_union_specifier_details_list_destroy(&state->unscoped_union_specifiers);
	struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map_destroy(&state->unscoped_union_specifiers_map);
	struct_unscoped_struct_specifier_details_list_destroy(&state->unscoped_struct_specifiers);
	struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map_destroy(&state->unscoped_struct_specifiers_map);
	struct_unscoped_enum_specifier_details_list_destroy(&state->unscoped_enum_specifiers);
	struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map_destroy(&state->unscoped_enum_specifiers_map);
	struct_bitfield_details_list_destroy(&state->bitfields);
	struct_bitfield_details_to_struct_bitfield_id_map_destroy(&state->bitfields_map);
	struct_jump_statement_details_list_destroy(&state->jump_statements);
	struct_jump_statement_details_to_struct_jump_statement_id_map_destroy(&state->jump_statements_map);
	struct_switch_statement_details_list_destroy(&state->switch_statements);
	struct_switch_statement_details_to_struct_switch_statement_id_map_destroy(&state->switch_statements_map);
	struct_if_statement_details_list_destroy(&state->if_statements);
	struct_if_statement_details_to_struct_if_statement_id_map_destroy(&state->if_statements_map);
	struct_while_statement_details_list_destroy(&state->while_statements);
	struct_while_statement_details_to_struct_while_statement_id_map_destroy(&state->while_statements_map);
	struct_for_statement_details_list_destroy(&state->for_statements);
	struct_for_statement_details_to_struct_for_statement_id_map_destroy(&state->for_statements_map);
	struct_code_statement_details_list_destroy(&state->code_statements);
	struct_code_statement_details_to_struct_code_statement_id_map_destroy(&state->code_statements_map);
	struct_labeled_statement_details_list_destroy(&state->labeled_statements);
	struct_labeled_statement_details_to_struct_labeled_statement_id_map_destroy(&state->labeled_statements_map);
	struct_postfix_expression_details_list_destroy(&state->postfix_expressions);
	struct_postfix_expression_details_to_struct_postfix_expression_id_map_destroy(&state->postfix_expressions_map);
	struct_primary_expression_details_list_destroy(&state->primary_expressions);
	struct_primary_expression_details_to_struct_primary_expression_id_map_destroy(&state->primary_expressions_map);
	struct_any_statement_details_list_destroy(&state->any_statements);
	struct_any_statement_details_to_struct_any_statement_id_map_destroy(&state->any_statements_map);
	struct_scoped_statement_details_list_destroy(&state->scoped_statements);
	struct_scoped_statement_details_to_struct_scoped_statement_id_map_destroy(&state->scoped_statements_map);
	struct_declaration_statement_details_list_destroy(&state->declaration_statements);
	struct_declaration_statement_details_to_struct_declaration_statement_id_map_destroy(&state->declaration_statements_map);
	struct_function_definition_details_list_destroy(&state->function_definitions);
	struct_function_definition_details_to_struct_function_definition_id_map_destroy(&state->function_definitions_map);
	struct_simple_declaration_details_list_destroy(&state->simple_declarations);
	struct_simple_declaration_details_to_struct_simple_declaration_id_map_destroy(&state->simple_declarations_map);
	struct_expression_details_list_destroy(&state->expressions);
	struct_expression_details_to_struct_expression_id_map_destroy(&state->expressions_map);
	struct_cast_expression_details_list_destroy(&state->cast_expressions);
	struct_cast_expression_details_to_struct_cast_expression_id_map_destroy(&state->cast_expressions_map);
	struct_unary_expression_details_list_destroy(&state->unary_expressions);
	struct_unary_expression_details_to_struct_unary_expression_id_map_destroy(&state->unary_expressions_map);
	struct_binary_expression_details_list_destroy(&state->binary_expressions);
	struct_binary_expression_details_to_struct_binary_expression_id_map_destroy(&state->binary_expressions_map);
	struct_ternary_expression_details_list_destroy(&state->ternary_expressions);
	struct_ternary_expression_details_to_struct_ternary_expression_id_map_destroy(&state->ternary_expressions_map);
	struct_constant_description_details_list_destroy(&state->constant_descriptions);
	struct_constant_description_details_to_struct_constant_description_id_map_destroy(&state->constant_descriptions_map);
	struct_data_package_details_list_destroy(&state->data_packages);
	struct_data_package_details_to_struct_data_package_id_map_destroy(&state->data_packages_map);
	struct_error_node_details_list_destroy(&state->error_nodes);
	struct_error_node_details_to_struct_error_node_id_map_destroy(&state->error_nodes_map);
	struct_byte_package_details_list_destroy(&state->byte_packages);
	struct_byte_package_details_to_struct_byte_package_id_map_destroy(&state->byte_packages_map);
	struct_hword_package_details_list_destroy(&state->hword_packages);
	struct_hword_package_details_to_struct_hword_package_id_map_destroy(&state->hword_packages_map);
	struct_word_package_details_list_destroy(&state->word_packages);
	struct_word_package_details_to_struct_word_package_id_map_destroy(&state->word_packages_map);
	struct_qword_package_details_list_destroy(&state->qword_packages);
	struct_qword_package_details_to_struct_qword_package_id_map_destroy(&state->qword_packages_map);
	struct_dword_package_details_list_destroy(&state->dword_packages);
	struct_dword_package_details_to_struct_dword_package_id_map_destroy(&state->dword_packages_map);
	struct_string_package_details_list_destroy(&state->string_packages);
	struct_string_package_details_to_struct_string_package_id_map_destroy(&state->string_packages_map);
	struct_identifier_details_list_destroy(&state->identifiers);
	struct_identifier_details_to_struct_identifier_id_map_destroy(&state->identifiers_map);
	struct_scope_guid_details_list_destroy(&state->scope_guids);
	struct_scope_guid_details_to_struct_scope_guid_id_map_destroy(&state->scope_guids_map);
	struct_anonymous_instance_details_list_destroy(&state->anonymous_instances);
	struct_anonymous_instance_details_to_struct_anonymous_instance_id_map_destroy(&state->anonymous_instances_map);
}

struct aggregate_type_specifier_id make_aggregate_type_specifier_simple_type_specifier(struct asm_state * state, struct simple_type_specifier_id id){
	return make_aggregate_type_specifier(state, ASM_AGGREGATE_TYPE_SPECIFIER_SIMPLE_TYPE_SPECIFIER, id.id);
}
struct aggregate_type_specifier_id make_aggregate_type_specifier_tag_specifier(struct asm_state * state, struct scoped_tag_specifier_id id){
	return make_aggregate_type_specifier(state, ASM_AGGREGATE_TYPE_SPECIFIER_TAG_SPECIFIER, id.id);
}
struct aggregate_type_specifier_id make_aggregate_type_specifier_typename_specifier(struct asm_state * state, struct scoped_typename_specifier_id id){
	return make_aggregate_type_specifier(state, ASM_AGGREGATE_TYPE_SPECIFIER_TYPENAME_SPECIFIER, id.id);
}
struct aggregate_type_specifier_id make_aggregate_type_specifier(struct asm_state * state, enum asm_aggregate_type_specifier_kind kind, unsigned int id){
	struct aggregate_type_specifier_details d;
	d.type = kind;
	d.id = id;
	if(struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_exists(&state->aggregate_type_specifiers_map, d)){
		return struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_get(&state->aggregate_type_specifiers_map, d);
	}else{
		struct aggregate_type_specifier_id new_id;
		new_id.id = struct_aggregate_type_specifier_details_list_size(&state->aggregate_type_specifiers);
		struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map_put(&state->aggregate_type_specifiers_map, d, new_id);
		struct_aggregate_type_specifier_details_list_add_end(&state->aggregate_type_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_AGGREGATE_TYPE_SPECIFIER);
		return new_id;
	}
}

struct simple_type_specifier_id make_simple_type_specifier_void(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_VOID);
}
struct simple_type_specifier_id make_simple_type_specifier_char(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_CHAR);
}
struct simple_type_specifier_id make_simple_type_specifier_short(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_SHORT);
}
struct simple_type_specifier_id make_simple_type_specifier_int(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_INT);
}
struct simple_type_specifier_id make_simple_type_specifier_long(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_LONG);
}
struct simple_type_specifier_id make_simple_type_specifier_float(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_FLOAT);
}
struct simple_type_specifier_id make_simple_type_specifier_double(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_DOUBLE);
}
struct simple_type_specifier_id make_simple_type_specifier_signed(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_SIGNED);
}
struct simple_type_specifier_id make_simple_type_specifier_unsigned(struct asm_state * state){
	return make_simple_type_specifier(state, ASM_SIMPLE_TYPE_SPECIFIER_UNSIGNED);
}
struct simple_type_specifier_id make_simple_type_specifier(struct asm_state * state, enum asm_simple_type_specifier_kind kind){
	struct simple_type_specifier_details d;
	d.type = kind;
	if(struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_exists(&state->simple_type_specifiers_map, d)){
		return struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_get(&state->simple_type_specifiers_map, d);
	}else{
		struct simple_type_specifier_id new_id;
		new_id.id = struct_simple_type_specifier_details_list_size(&state->simple_type_specifiers);
		struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map_put(&state->simple_type_specifiers_map, d, new_id);
		struct_simple_type_specifier_details_list_add_end(&state->simple_type_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SIMPLE_TYPE_SPECIFIER);
		return new_id;
	}
}

struct simple_type_qualifier_id make_simple_type_qualifier_const(struct asm_state * state){
	return make_simple_type_qualifier(state, ASM_SIMPLE_TYPE_QUALIFIER_CONST);
}
struct simple_type_qualifier_id make_simple_type_qualifier_volatile(struct asm_state * state){
	return make_simple_type_qualifier(state, ASM_SIMPLE_TYPE_QUALIFIER_VOLATILE);
}
struct simple_type_qualifier_id make_simple_type_qualifier(struct asm_state * state, enum asm_simple_type_qualifier_kind kind){
	struct simple_type_qualifier_details d;
	d.type = kind;
	if(struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_exists(&state->simple_type_qualifiers_map, d)){
		return struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_get(&state->simple_type_qualifiers_map, d);
	}else{
		struct simple_type_qualifier_id new_id;
		new_id.id = struct_simple_type_qualifier_details_list_size(&state->simple_type_qualifiers);
		struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map_put(&state->simple_type_qualifiers_map, d, new_id);
		struct_simple_type_qualifier_details_list_add_end(&state->simple_type_qualifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SIMPLE_TYPE_QUALIFIER);
		return new_id;
	}
}

struct simple_storage_class_specifier_id make_simple_storage_class_specifier_typedef(struct asm_state * state){
	return make_simple_storage_class_specifier(state, ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_TYPEDEF);
}
struct simple_storage_class_specifier_id make_simple_storage_class_specifier_extern(struct asm_state * state){
	return make_simple_storage_class_specifier(state, ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_EXTERN);
}
struct simple_storage_class_specifier_id make_simple_storage_class_specifier_static(struct asm_state * state){
	return make_simple_storage_class_specifier(state, ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_STATIC);
}
struct simple_storage_class_specifier_id make_simple_storage_class_specifier_auto(struct asm_state * state){
	return make_simple_storage_class_specifier(state, ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_AUTO);
}
struct simple_storage_class_specifier_id make_simple_storage_class_specifier_register(struct asm_state * state){
	return make_simple_storage_class_specifier(state, ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_REGISTER);
}
struct simple_storage_class_specifier_id make_simple_storage_class_specifier(struct asm_state * state, enum asm_simple_storage_class_specifier_kind kind){
	struct simple_storage_class_specifier_details d;
	d.type = kind;
	if(struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_exists(&state->simple_storage_class_specifiers_map, d)){
		return struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_get(&state->simple_storage_class_specifiers_map, d);
	}else{
		struct simple_storage_class_specifier_id new_id;
		new_id.id = struct_simple_storage_class_specifier_details_list_size(&state->simple_storage_class_specifiers);
		struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map_put(&state->simple_storage_class_specifiers_map, d, new_id);
		struct_simple_storage_class_specifier_details_list_add_end(&state->simple_storage_class_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SIMPLE_STORAGE_CLASS_SPECIFIER);
		return new_id;
	}
}

struct specifier_or_qualifier_id make_specifier_or_qualifier_simple_storage_class_specifier(struct asm_state * state, struct simple_storage_class_specifier_id id){
	return make_specifier_or_qualifier(state, ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_STORAGE_CLASS_SPECIFIER, id.id);
}
struct specifier_or_qualifier_id make_specifier_or_qualifier_simple_type_qualifier(struct asm_state * state, struct simple_type_qualifier_id id){
	return make_specifier_or_qualifier(state, ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_TYPE_QUALIFIER, id.id);
}
struct specifier_or_qualifier_id make_specifier_or_qualifier_aggregate_type_specifier(struct asm_state * state, struct aggregate_type_specifier_id id){
	return make_specifier_or_qualifier(state, ASM_SPECIFIER_OR_QUALIFIER_AGGREGATE_TYPE_SPECIFIER, id.id);
}
struct specifier_or_qualifier_id make_specifier_or_qualifier(struct asm_state * state, enum asm_specifier_or_qualifier_kind kind, unsigned int id){
	struct specifier_or_qualifier_details d;
	d.type = kind;
	d.id = id;
	if(struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_exists(&state->specifier_or_qualifiers_map, d)){
		return struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_get(&state->specifier_or_qualifiers_map, d);
	}else{
		struct specifier_or_qualifier_id new_id;
		new_id.id = struct_specifier_or_qualifier_details_list_size(&state->specifier_or_qualifiers);
		struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_put(&state->specifier_or_qualifiers_map, d, new_id);
		struct_specifier_or_qualifier_details_list_add_end(&state->specifier_or_qualifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SPECIFIER_OR_QUALIFIER);
		return new_id;
	}
}

struct specifier_or_qualifier_list_item_id make_specifier_or_qualifier_list_item_specifier_or_qualifier(struct asm_state * state, struct specifier_or_qualifier_list_item_id prev_id, struct specifier_or_qualifier_id item_id){
	return make_specifier_or_qualifier_list_item(state, ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, prev_id, item_id);
}
struct specifier_or_qualifier_list_item_id make_specifier_or_qualifier_list_item_list_start(struct asm_state * state){
	struct specifier_or_qualifier_list_item_id prev_id;
	struct specifier_or_qualifier_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_specifier_or_qualifier_list_item(state, ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct specifier_or_qualifier_list_item_id make_specifier_or_qualifier_list_item(struct asm_state * state, enum asm_specifier_or_qualifier_list_item_kind kind, struct specifier_or_qualifier_list_item_id prev_id, struct specifier_or_qualifier_id item_id){
	struct specifier_or_qualifier_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_exists(&state->specifier_or_qualifier_list_items_map, d)){
		return struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_get(&state->specifier_or_qualifier_list_items_map, d);
	}else{
		struct specifier_or_qualifier_list_item_id new_id;
		new_id.id = struct_specifier_or_qualifier_list_item_details_list_size(&state->specifier_or_qualifier_list_items);
		struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_put(&state->specifier_or_qualifier_list_items_map, d, new_id);
		struct_specifier_or_qualifier_list_item_details_list_add_end(&state->specifier_or_qualifier_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SPECIFIER_OR_QUALIFIER_LIST_ITEM);
		return new_id;
	}
}

struct parameter_list_id make_parameter_list_variadic(struct asm_state * state, struct general_type_list_item_id general_type_list_item_id){
	return make_parameter_list(state, ASM_PARAMETER_LIST_VARIADIC, general_type_list_item_id);
}
struct parameter_list_id make_parameter_list_non_variadic(struct asm_state * state, struct general_type_list_item_id general_type_list_item_id){
	return make_parameter_list(state, ASM_PARAMETER_LIST_NON_VARIADIC, general_type_list_item_id);
}
struct parameter_list_id make_parameter_list(struct asm_state * state, enum asm_parameter_list_kind kind, struct general_type_list_item_id general_type_list_item_id){
	struct parameter_list_details d;
	d.type = kind;
	d.general_type_list_item_id = general_type_list_item_id;
	if(struct_parameter_list_details_to_struct_parameter_list_id_map_exists(&state->parameter_lists_map, d)){
		return struct_parameter_list_details_to_struct_parameter_list_id_map_get(&state->parameter_lists_map, d);
	}else{
		struct parameter_list_id new_id;
		new_id.id = struct_parameter_list_details_list_size(&state->parameter_lists);
		struct_parameter_list_details_to_struct_parameter_list_id_map_put(&state->parameter_lists_map, d, new_id);
		struct_parameter_list_details_list_add_end(&state->parameter_lists, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_PARAMETER_LIST);
		return new_id;
	}
}

struct initializer_id make_initializer_initializer_list(struct asm_state * state, struct initializer_list_item_id id){
	return make_initializer(state, ASM_INITIALIZER_INITIALIZER_LIST, id.id);
}
struct initializer_id make_initializer_expression(struct asm_state * state, struct expression_id id){
	return make_initializer(state, ASM_INITIALIZER_EXPRESSION, id.id);
}
struct initializer_id make_initializer(struct asm_state * state, enum asm_initializer_kind kind, unsigned int id){
	struct initializer_details d;
	d.type = kind;
	d.id = id;
	if(struct_initializer_details_to_struct_initializer_id_map_exists(&state->initializers_map, d)){
		return struct_initializer_details_to_struct_initializer_id_map_get(&state->initializers_map, d);
	}else{
		struct initializer_id new_id;
		new_id.id = struct_initializer_details_list_size(&state->initializers);
		struct_initializer_details_to_struct_initializer_id_map_put(&state->initializers_map, d, new_id);
		struct_initializer_details_list_add_end(&state->initializers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_INITIALIZER);
		return new_id;
	}
}

struct declarator_part_id make_declarator_part_function(struct asm_state * state, struct function_part_id id){
	return make_declarator_part(state, ASM_DECLARATOR_PART_FUNCTION, id.id);
}
struct declarator_part_id make_declarator_part_pointer(struct asm_state * state, struct pointer_part_id id){
	return make_declarator_part(state, ASM_DECLARATOR_PART_POINTER, id.id);
}
struct declarator_part_id make_declarator_part_array(struct asm_state * state, struct array_part_id id){
	return make_declarator_part(state, ASM_DECLARATOR_PART_ARRAY, id.id);
}
struct declarator_part_id make_declarator_part(struct asm_state * state, enum asm_declarator_part_kind kind, unsigned int id){
	struct declarator_part_details d;
	d.type = kind;
	d.id = id;
	if(struct_declarator_part_details_to_struct_declarator_part_id_map_exists(&state->declarator_parts_map, d)){
		return struct_declarator_part_details_to_struct_declarator_part_id_map_get(&state->declarator_parts_map, d);
	}else{
		struct declarator_part_id new_id;
		new_id.id = struct_declarator_part_details_list_size(&state->declarator_parts);
		struct_declarator_part_details_to_struct_declarator_part_id_map_put(&state->declarator_parts_map, d, new_id);
		struct_declarator_part_details_list_add_end(&state->declarator_parts, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_DECLARATOR_PART);
		return new_id;
	}
}

struct any_statement_list_item_id make_any_statement_list_item_any_statement(struct asm_state * state, struct any_statement_list_item_id prev_id, struct any_statement_id item_id){
	return make_any_statement_list_item(state, ASM_ANY_STATEMENT_LIST_ITEM_ANY_STATEMENT, prev_id, item_id);
}
struct any_statement_list_item_id make_any_statement_list_item_list_start(struct asm_state * state){
	struct any_statement_list_item_id prev_id;
	struct any_statement_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_any_statement_list_item(state, ASM_ANY_STATEMENT_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct any_statement_list_item_id make_any_statement_list_item(struct asm_state * state, enum asm_any_statement_list_item_kind kind, struct any_statement_list_item_id prev_id, struct any_statement_id item_id){
	struct any_statement_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map_exists(&state->any_statement_list_items_map, d)){
		return struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map_get(&state->any_statement_list_items_map, d);
	}else{
		struct any_statement_list_item_id new_id;
		new_id.id = struct_any_statement_list_item_details_list_size(&state->any_statement_list_items);
		struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map_put(&state->any_statement_list_items_map, d, new_id);
		struct_any_statement_list_item_details_list_add_end(&state->any_statement_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ANY_STATEMENT_LIST_ITEM);
		return new_id;
	}
}

struct anonymous_tag_definition_id make_anonymous_tag_definition_anonymous(struct asm_state * state, struct tag_definition_id tag_definition_id, struct scope_guid_id scope_guid_id, struct anonymous_instance_id anonymous_instance_id){
	return make_anonymous_tag_definition(state, ASM_ANONYMOUS_TAG_DEFINITION_ANONYMOUS, tag_definition_id, scope_guid_id, anonymous_instance_id);
}
struct anonymous_tag_definition_id make_anonymous_tag_definition(struct asm_state * state, enum asm_anonymous_tag_definition_kind kind, struct tag_definition_id tag_definition_id, struct scope_guid_id scope_guid_id, struct anonymous_instance_id anonymous_instance_id){
	struct anonymous_tag_definition_details d;
	d.type = kind;
	d.tag_definition_id = tag_definition_id;
	d.scope_guid_id = scope_guid_id;
	d.anonymous_instance_id = anonymous_instance_id;
	if(struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map_exists(&state->anonymous_tag_definitions_map, d)){
		return struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map_get(&state->anonymous_tag_definitions_map, d);
	}else{
		struct anonymous_tag_definition_id new_id;
		new_id.id = struct_anonymous_tag_definition_details_list_size(&state->anonymous_tag_definitions);
		struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map_put(&state->anonymous_tag_definitions_map, d, new_id);
		struct_anonymous_tag_definition_details_list_add_end(&state->anonymous_tag_definitions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ANONYMOUS_TAG_DEFINITION);
		return new_id;
	}
}

struct named_tag_predeclaration_id make_named_tag_predeclaration_named(struct asm_state * state, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id){
	return make_named_tag_predeclaration(state, ASM_NAMED_TAG_PREDECLARATION_NAMED, scope_guid_id, identifier_id);
}
struct named_tag_predeclaration_id make_named_tag_predeclaration(struct asm_state * state, enum asm_named_tag_predeclaration_kind kind, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id){
	struct named_tag_predeclaration_details d;
	d.type = kind;
	d.scope_guid_id = scope_guid_id;
	d.identifier_id = identifier_id;
	if(struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map_exists(&state->named_tag_predeclarations_map, d)){
		return struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map_get(&state->named_tag_predeclarations_map, d);
	}else{
		struct named_tag_predeclaration_id new_id;
		new_id.id = struct_named_tag_predeclaration_details_list_size(&state->named_tag_predeclarations);
		struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map_put(&state->named_tag_predeclarations_map, d, new_id);
		struct_named_tag_predeclaration_details_list_add_end(&state->named_tag_predeclarations, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_NAMED_TAG_PREDECLARATION);
		return new_id;
	}
}

struct named_tag_definition_id make_named_tag_definition_named(struct asm_state * state, struct tag_definition_id tag_definition_id, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id){
	return make_named_tag_definition(state, ASM_NAMED_TAG_DEFINITION_NAMED, tag_definition_id, scope_guid_id, identifier_id);
}
struct named_tag_definition_id make_named_tag_definition(struct asm_state * state, enum asm_named_tag_definition_kind kind, struct tag_definition_id tag_definition_id, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id){
	struct named_tag_definition_details d;
	d.type = kind;
	d.tag_definition_id = tag_definition_id;
	d.scope_guid_id = scope_guid_id;
	d.identifier_id = identifier_id;
	if(struct_named_tag_definition_details_to_struct_named_tag_definition_id_map_exists(&state->named_tag_definitions_map, d)){
		return struct_named_tag_definition_details_to_struct_named_tag_definition_id_map_get(&state->named_tag_definitions_map, d);
	}else{
		struct named_tag_definition_id new_id;
		new_id.id = struct_named_tag_definition_details_list_size(&state->named_tag_definitions);
		struct_named_tag_definition_details_to_struct_named_tag_definition_id_map_put(&state->named_tag_definitions_map, d, new_id);
		struct_named_tag_definition_details_list_add_end(&state->named_tag_definitions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_NAMED_TAG_DEFINITION);
		return new_id;
	}
}

struct tag_definition_id make_tag_definition_enum(struct asm_state * state, struct enum_member_list_item_id id){
	return make_tag_definition(state, ASM_TAG_DEFINITION_ENUM, id.id);
}
struct tag_definition_id make_tag_definition_struct(struct asm_state * state, struct struct_or_union_member_list_item_id id){
	return make_tag_definition(state, ASM_TAG_DEFINITION_STRUCT, id.id);
}
struct tag_definition_id make_tag_definition_union(struct asm_state * state, struct struct_or_union_member_list_item_id id){
	return make_tag_definition(state, ASM_TAG_DEFINITION_UNION, id.id);
}
struct tag_definition_id make_tag_definition(struct asm_state * state, enum asm_tag_definition_kind kind, unsigned int id){
	struct tag_definition_details d;
	d.type = kind;
	d.id = id;
	if(struct_tag_definition_details_to_struct_tag_definition_id_map_exists(&state->tag_definitions_map, d)){
		return struct_tag_definition_details_to_struct_tag_definition_id_map_get(&state->tag_definitions_map, d);
	}else{
		struct tag_definition_id new_id;
		new_id.id = struct_tag_definition_details_list_size(&state->tag_definitions);
		struct_tag_definition_details_to_struct_tag_definition_id_map_put(&state->tag_definitions_map, d, new_id);
		struct_tag_definition_details_list_add_end(&state->tag_definitions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_TAG_DEFINITION);
		return new_id;
	}
}

struct struct_or_union_member_list_item_id make_struct_or_union_member_list_item_member(struct asm_state * state, struct struct_or_union_member_list_item_id prev_id, struct struct_or_union_member_id item_id){
	return make_struct_or_union_member_list_item(state, ASM_STRUCT_OR_UNION_MEMBER_LIST_ITEM_MEMBER, prev_id, item_id);
}
struct struct_or_union_member_list_item_id make_struct_or_union_member_list_item_list_start(struct asm_state * state){
	struct struct_or_union_member_list_item_id prev_id;
	struct struct_or_union_member_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_struct_or_union_member_list_item(state, ASM_STRUCT_OR_UNION_MEMBER_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct struct_or_union_member_list_item_id make_struct_or_union_member_list_item(struct asm_state * state, enum asm_struct_or_union_member_list_item_kind kind, struct struct_or_union_member_list_item_id prev_id, struct struct_or_union_member_id item_id){
	struct struct_or_union_member_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map_exists(&state->struct_or_union_member_list_items_map, d)){
		return struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map_get(&state->struct_or_union_member_list_items_map, d);
	}else{
		struct struct_or_union_member_list_item_id new_id;
		new_id.id = struct_struct_or_union_member_list_item_details_list_size(&state->struct_or_union_member_list_items);
		struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map_put(&state->struct_or_union_member_list_items_map, d, new_id);
		struct_struct_or_union_member_list_item_details_list_add_end(&state->struct_or_union_member_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_STRUCT_OR_UNION_MEMBER_LIST_ITEM);
		return new_id;
	}
}

struct enum_member_list_item_id make_enum_member_list_item_member(struct asm_state * state, struct enum_member_list_item_id prev_id, struct enum_member_id item_id){
	return make_enum_member_list_item(state, ASM_ENUM_MEMBER_LIST_ITEM_MEMBER, prev_id, item_id);
}
struct enum_member_list_item_id make_enum_member_list_item_list_start(struct asm_state * state){
	struct enum_member_list_item_id prev_id;
	struct enum_member_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_enum_member_list_item(state, ASM_ENUM_MEMBER_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct enum_member_list_item_id make_enum_member_list_item(struct asm_state * state, enum asm_enum_member_list_item_kind kind, struct enum_member_list_item_id prev_id, struct enum_member_id item_id){
	struct enum_member_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map_exists(&state->enum_member_list_items_map, d)){
		return struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map_get(&state->enum_member_list_items_map, d);
	}else{
		struct enum_member_list_item_id new_id;
		new_id.id = struct_enum_member_list_item_details_list_size(&state->enum_member_list_items);
		struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map_put(&state->enum_member_list_items_map, d, new_id);
		struct_enum_member_list_item_details_list_add_end(&state->enum_member_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ENUM_MEMBER_LIST_ITEM);
		return new_id;
	}
}

struct enum_member_id make_enum_member_member(struct asm_state * state, struct identifier_id identifier_id, struct expression_id expression_id){
	return make_enum_member(state, ASM_ENUM_MEMBER_MEMBER, identifier_id, expression_id);
}
struct enum_member_id make_enum_member(struct asm_state * state, enum asm_enum_member_kind kind, struct identifier_id identifier_id, struct expression_id expression_id){
	struct enum_member_details d;
	d.type = kind;
	d.identifier_id = identifier_id;
	d.expression_id = expression_id;
	if(struct_enum_member_details_to_struct_enum_member_id_map_exists(&state->enum_members_map, d)){
		return struct_enum_member_details_to_struct_enum_member_id_map_get(&state->enum_members_map, d);
	}else{
		struct enum_member_id new_id;
		new_id.id = struct_enum_member_details_list_size(&state->enum_members);
		struct_enum_member_details_to_struct_enum_member_id_map_put(&state->enum_members_map, d, new_id);
		struct_enum_member_details_list_add_end(&state->enum_members, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ENUM_MEMBER);
		return new_id;
	}
}

struct struct_or_union_member_id make_struct_or_union_member_member(struct asm_state * state, struct general_type_id general_type_id, struct layout_flag_list_item_id layout_flag_list_item_id, struct identifier_id identifier_id){
	return make_struct_or_union_member(state, ASM_STRUCT_OR_UNION_MEMBER_MEMBER, general_type_id, layout_flag_list_item_id, identifier_id);
}
struct struct_or_union_member_id make_struct_or_union_member(struct asm_state * state, enum asm_struct_or_union_member_kind kind, struct general_type_id general_type_id, struct layout_flag_list_item_id layout_flag_list_item_id, struct identifier_id identifier_id){
	struct struct_or_union_member_details d;
	d.type = kind;
	d.general_type_id = general_type_id;
	d.layout_flag_list_item_id = layout_flag_list_item_id;
	d.identifier_id = identifier_id;
	if(struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map_exists(&state->struct_or_union_members_map, d)){
		return struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map_get(&state->struct_or_union_members_map, d);
	}else{
		struct struct_or_union_member_id new_id;
		new_id.id = struct_struct_or_union_member_details_list_size(&state->struct_or_union_members);
		struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map_put(&state->struct_or_union_members_map, d, new_id);
		struct_struct_or_union_member_details_list_add_end(&state->struct_or_union_members, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_STRUCT_OR_UNION_MEMBER);
		return new_id;
	}
}

struct layout_flag_list_item_id make_layout_flag_list_item_layout_flag(struct asm_state * state, struct layout_flag_list_item_id prev_id, struct layout_flag_id item_id){
	return make_layout_flag_list_item(state, ASM_LAYOUT_FLAG_LIST_ITEM_LAYOUT_FLAG, prev_id, item_id);
}
struct layout_flag_list_item_id make_layout_flag_list_item_list_start(struct asm_state * state){
	struct layout_flag_list_item_id prev_id;
	struct layout_flag_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_layout_flag_list_item(state, ASM_LAYOUT_FLAG_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct layout_flag_list_item_id make_layout_flag_list_item(struct asm_state * state, enum asm_layout_flag_list_item_kind kind, struct layout_flag_list_item_id prev_id, struct layout_flag_id item_id){
	struct layout_flag_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_layout_flag_list_item_details_to_struct_layout_flag_list_item_id_map_exists(&state->layout_flag_list_items_map, d)){
		return struct_layout_flag_list_item_details_to_struct_layout_flag_list_item_id_map_get(&state->layout_flag_list_items_map, d);
	}else{
		struct layout_flag_list_item_id new_id;
		new_id.id = struct_layout_flag_list_item_details_list_size(&state->layout_flag_list_items);
		struct_layout_flag_list_item_details_to_struct_layout_flag_list_item_id_map_put(&state->layout_flag_list_items_map, d, new_id);
		struct_layout_flag_list_item_details_list_add_end(&state->layout_flag_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_LAYOUT_FLAG_LIST_ITEM);
		return new_id;
	}
}

struct layout_flag_id make_layout_flag_struct(struct asm_state * state){
	return make_layout_flag(state, ASM_LAYOUT_FLAG_STRUCT);
}
struct layout_flag_id make_layout_flag_union(struct asm_state * state){
	return make_layout_flag(state, ASM_LAYOUT_FLAG_UNION);
}
struct layout_flag_id make_layout_flag(struct asm_state * state, enum asm_layout_flag_kind kind){
	struct layout_flag_details d;
	d.type = kind;
	if(struct_layout_flag_details_to_struct_layout_flag_id_map_exists(&state->layout_flags_map, d)){
		return struct_layout_flag_details_to_struct_layout_flag_id_map_get(&state->layout_flags_map, d);
	}else{
		struct layout_flag_id new_id;
		new_id.id = struct_layout_flag_details_list_size(&state->layout_flags);
		struct_layout_flag_details_to_struct_layout_flag_id_map_put(&state->layout_flags_map, d, new_id);
		struct_layout_flag_details_list_add_end(&state->layout_flags, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_LAYOUT_FLAG);
		return new_id;
	}
}

struct initializer_list_item_id make_initializer_list_item_initializer(struct asm_state * state, struct initializer_list_item_id prev_id, struct initializer_id item_id){
	return make_initializer_list_item(state, ASM_INITIALIZER_LIST_ITEM_INITIALIZER, prev_id, item_id);
}
struct initializer_list_item_id make_initializer_list_item_list_start(struct asm_state * state){
	struct initializer_list_item_id prev_id;
	struct initializer_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_initializer_list_item(state, ASM_INITIALIZER_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct initializer_list_item_id make_initializer_list_item(struct asm_state * state, enum asm_initializer_list_item_kind kind, struct initializer_list_item_id prev_id, struct initializer_id item_id){
	struct initializer_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_exists(&state->initializer_list_items_map, d)){
		return struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_get(&state->initializer_list_items_map, d);
	}else{
		struct initializer_list_item_id new_id;
		new_id.id = struct_initializer_list_item_details_list_size(&state->initializer_list_items);
		struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_put(&state->initializer_list_items_map, d, new_id);
		struct_initializer_list_item_details_list_add_end(&state->initializer_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_INITIALIZER_LIST_ITEM);
		return new_id;
	}
}

struct expression_list_item_id make_expression_list_item_expression(struct asm_state * state, struct expression_list_item_id prev_id, struct expression_id item_id){
	return make_expression_list_item(state, ASM_EXPRESSION_LIST_ITEM_EXPRESSION, prev_id, item_id);
}
struct expression_list_item_id make_expression_list_item_list_start(struct asm_state * state){
	struct expression_list_item_id prev_id;
	struct expression_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_expression_list_item(state, ASM_EXPRESSION_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct expression_list_item_id make_expression_list_item(struct asm_state * state, enum asm_expression_list_item_kind kind, struct expression_list_item_id prev_id, struct expression_id item_id){
	struct expression_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_expression_list_item_details_to_struct_expression_list_item_id_map_exists(&state->expression_list_items_map, d)){
		return struct_expression_list_item_details_to_struct_expression_list_item_id_map_get(&state->expression_list_items_map, d);
	}else{
		struct expression_list_item_id new_id;
		new_id.id = struct_expression_list_item_details_list_size(&state->expression_list_items);
		struct_expression_list_item_details_to_struct_expression_list_item_id_map_put(&state->expression_list_items_map, d, new_id);
		struct_expression_list_item_details_list_add_end(&state->expression_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_EXPRESSION_LIST_ITEM);
		return new_id;
	}
}

struct declarator_part_list_item_id make_declarator_part_list_item_declarator_part(struct asm_state * state, struct declarator_part_list_item_id prev_id, struct declarator_part_id item_id){
	return make_declarator_part_list_item(state, ASM_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, prev_id, item_id);
}
struct declarator_part_list_item_id make_declarator_part_list_item_list_start(struct asm_state * state){
	struct declarator_part_list_item_id prev_id;
	struct declarator_part_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_declarator_part_list_item(state, ASM_DECLARATOR_PART_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct declarator_part_list_item_id make_declarator_part_list_item(struct asm_state * state, enum asm_declarator_part_list_item_kind kind, struct declarator_part_list_item_id prev_id, struct declarator_part_id item_id){
	struct declarator_part_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_exists(&state->declarator_part_list_items_map, d)){
		return struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_get(&state->declarator_part_list_items_map, d);
	}else{
		struct declarator_part_list_item_id new_id;
		new_id.id = struct_declarator_part_list_item_details_list_size(&state->declarator_part_list_items);
		struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_put(&state->declarator_part_list_items_map, d, new_id);
		struct_declarator_part_list_item_details_list_add_end(&state->declarator_part_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_DECLARATOR_PART_LIST_ITEM);
		return new_id;
	}
}

struct pointer_part_id make_pointer_part_pointer(struct asm_state * state, struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_item_id){
	return make_pointer_part(state, ASM_POINTER_PART_POINTER, specifier_or_qualifier_list_item_id);
}
struct pointer_part_id make_pointer_part(struct asm_state * state, enum asm_pointer_part_kind kind, struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_item_id){
	struct pointer_part_details d;
	d.type = kind;
	d.specifier_or_qualifier_list_item_id = specifier_or_qualifier_list_item_id;
	if(struct_pointer_part_details_to_struct_pointer_part_id_map_exists(&state->pointer_parts_map, d)){
		return struct_pointer_part_details_to_struct_pointer_part_id_map_get(&state->pointer_parts_map, d);
	}else{
		struct pointer_part_id new_id;
		new_id.id = struct_pointer_part_details_list_size(&state->pointer_parts);
		struct_pointer_part_details_to_struct_pointer_part_id_map_put(&state->pointer_parts_map, d, new_id);
		struct_pointer_part_details_list_add_end(&state->pointer_parts, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_POINTER_PART);
		return new_id;
	}
}

struct function_part_id make_function_part_prototype(struct asm_state * state, struct parameter_list_id parameter_list_id){
	return make_function_part(state, ASM_FUNCTION_PART_PROTOTYPE, parameter_list_id);
}
struct function_part_id make_function_part_k_and_r_c(struct asm_state * state, struct parameter_list_id parameter_list_id){
	return make_function_part(state, ASM_FUNCTION_PART_K_AND_R_C, parameter_list_id);
}
struct function_part_id make_function_part(struct asm_state * state, enum asm_function_part_kind kind, struct parameter_list_id parameter_list_id){
	struct function_part_details d;
	d.type = kind;
	d.parameter_list_id = parameter_list_id;
	if(struct_function_part_details_to_struct_function_part_id_map_exists(&state->function_parts_map, d)){
		return struct_function_part_details_to_struct_function_part_id_map_get(&state->function_parts_map, d);
	}else{
		struct function_part_id new_id;
		new_id.id = struct_function_part_details_list_size(&state->function_parts);
		struct_function_part_details_to_struct_function_part_id_map_put(&state->function_parts_map, d, new_id);
		struct_function_part_details_list_add_end(&state->function_parts, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_FUNCTION_PART);
		return new_id;
	}
}

struct array_part_id make_array_part_expression(struct asm_state * state, struct expression_id expression_id){
	return make_array_part(state, ASM_ARRAY_PART_EXPRESSION, expression_id);
}
struct array_part_id make_array_part_flexible(struct asm_state * state){
	struct expression_id expression_id;
	expression_id.id = 0;
	return make_array_part(state, ASM_ARRAY_PART_FLEXIBLE, expression_id);
}
struct array_part_id make_array_part(struct asm_state * state, enum asm_array_part_kind kind, struct expression_id expression_id){
	struct array_part_details d;
	d.type = kind;
	d.expression_id = expression_id;
	if(struct_array_part_details_to_struct_array_part_id_map_exists(&state->array_parts_map, d)){
		return struct_array_part_details_to_struct_array_part_id_map_get(&state->array_parts_map, d);
	}else{
		struct array_part_id new_id;
		new_id.id = struct_array_part_details_list_size(&state->array_parts);
		struct_array_part_details_to_struct_array_part_id_map_put(&state->array_parts_map, d, new_id);
		struct_array_part_details_list_add_end(&state->array_parts, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ARRAY_PART);
		return new_id;
	}
}

struct general_type_id make_general_type_general_type(struct asm_state * state, struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_item_id, struct declarator_part_list_item_id declarator_part_list_item_id, struct bitfield_id bitfield_id){
	return make_general_type(state, ASM_GENERAL_TYPE_GENERAL_TYPE, specifier_or_qualifier_list_item_id, declarator_part_list_item_id, bitfield_id);
}
struct general_type_id make_general_type(struct asm_state * state, enum asm_general_type_kind kind, struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_item_id, struct declarator_part_list_item_id declarator_part_list_item_id, struct bitfield_id bitfield_id){
	struct general_type_details d;
	d.type = kind;
	d.specifier_or_qualifier_list_item_id = specifier_or_qualifier_list_item_id;
	d.declarator_part_list_item_id = declarator_part_list_item_id;
	d.bitfield_id = bitfield_id;
	if(struct_general_type_details_to_struct_general_type_id_map_exists(&state->general_types_map, d)){
		return struct_general_type_details_to_struct_general_type_id_map_get(&state->general_types_map, d);
	}else{
		struct general_type_id new_id;
		new_id.id = struct_general_type_details_list_size(&state->general_types);
		struct_general_type_details_to_struct_general_type_id_map_put(&state->general_types_map, d, new_id);
		struct_general_type_details_list_add_end(&state->general_types, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_GENERAL_TYPE);
		return new_id;
	}
}

struct typedef_entry_id make_typedef_entry_typedef(struct asm_state * state, struct identifier_id identifier_id, struct scope_guid_id scope_guid_id, struct general_type_id general_type_id){
	return make_typedef_entry(state, ASM_TYPEDEF_ENTRY_TYPEDEF, identifier_id, scope_guid_id, general_type_id);
}
struct typedef_entry_id make_typedef_entry(struct asm_state * state, enum asm_typedef_entry_kind kind, struct identifier_id identifier_id, struct scope_guid_id scope_guid_id, struct general_type_id general_type_id){
	struct typedef_entry_details d;
	d.type = kind;
	d.identifier_id = identifier_id;
	d.scope_guid_id = scope_guid_id;
	d.general_type_id = general_type_id;
	if(struct_typedef_entry_details_to_struct_typedef_entry_id_map_exists(&state->typedef_entrys_map, d)){
		return struct_typedef_entry_details_to_struct_typedef_entry_id_map_get(&state->typedef_entrys_map, d);
	}else{
		struct typedef_entry_id new_id;
		new_id.id = struct_typedef_entry_details_list_size(&state->typedef_entrys);
		struct_typedef_entry_details_to_struct_typedef_entry_id_map_put(&state->typedef_entrys_map, d, new_id);
		struct_typedef_entry_details_list_add_end(&state->typedef_entrys, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_TYPEDEF_ENTRY);
		return new_id;
	}
}

struct general_type_list_item_id make_general_type_list_item_general_type(struct asm_state * state, struct general_type_list_item_id prev_id, struct general_type_id item_id){
	return make_general_type_list_item(state, ASM_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev_id, item_id);
}
struct general_type_list_item_id make_general_type_list_item_list_start(struct asm_state * state){
	struct general_type_list_item_id prev_id;
	struct general_type_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_general_type_list_item(state, ASM_GENERAL_TYPE_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct general_type_list_item_id make_general_type_list_item(struct asm_state * state, enum asm_general_type_list_item_kind kind, struct general_type_list_item_id prev_id, struct general_type_id item_id){
	struct general_type_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_exists(&state->general_type_list_items_map, d)){
		return struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_get(&state->general_type_list_items_map, d);
	}else{
		struct general_type_list_item_id new_id;
		new_id.id = struct_general_type_list_item_details_list_size(&state->general_type_list_items);
		struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_put(&state->general_type_list_items_map, d, new_id);
		struct_general_type_list_item_details_list_add_end(&state->general_type_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_GENERAL_TYPE_LIST_ITEM);
		return new_id;
	}
}

struct postfix_expression_part_list_item_id make_postfix_expression_part_list_item_postfix_expression_part(struct asm_state * state, struct postfix_expression_part_list_item_id prev_id, struct postfix_expression_part_id item_id){
	return make_postfix_expression_part_list_item(state, ASM_POSTFIX_EXPRESSION_PART_LIST_ITEM_POSTFIX_EXPRESSION_PART, prev_id, item_id);
}
struct postfix_expression_part_list_item_id make_postfix_expression_part_list_item_list_start(struct asm_state * state){
	struct postfix_expression_part_list_item_id prev_id;
	struct postfix_expression_part_id item_id;
	prev_id.id = 0;
	item_id.id = 0;
	return make_postfix_expression_part_list_item(state, ASM_POSTFIX_EXPRESSION_PART_LIST_ITEM_LIST_START, prev_id, item_id);
}
struct postfix_expression_part_list_item_id make_postfix_expression_part_list_item(struct asm_state * state, enum asm_postfix_expression_part_list_item_kind kind, struct postfix_expression_part_list_item_id prev_id, struct postfix_expression_part_id item_id){
	struct postfix_expression_part_list_item_details d;
	d.type = kind;
	d.prev_id = prev_id;
	d.item_id = item_id;
	if(struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map_exists(&state->postfix_expression_part_list_items_map, d)){
		return struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map_get(&state->postfix_expression_part_list_items_map, d);
	}else{
		struct postfix_expression_part_list_item_id new_id;
		new_id.id = struct_postfix_expression_part_list_item_details_list_size(&state->postfix_expression_part_list_items);
		struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map_put(&state->postfix_expression_part_list_items_map, d, new_id);
		struct_postfix_expression_part_list_item_details_list_add_end(&state->postfix_expression_part_list_items, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_POSTFIX_EXPRESSION_PART_LIST_ITEM);
		return new_id;
	}
}

struct postfix_expression_part_id make_postfix_expression_part_array_dereference(struct asm_state * state, struct expression_id id){
	return make_postfix_expression_part(state, ASM_POSTFIX_EXPRESSION_PART_ARRAY_DEREFERENCE, id.id);
}
struct postfix_expression_part_id make_postfix_expression_part_struct_dot_dereference(struct asm_state * state, struct identifier_id id){
	return make_postfix_expression_part(state, ASM_POSTFIX_EXPRESSION_PART_STRUCT_DOT_DEREFERENCE, id.id);
}
struct postfix_expression_part_id make_postfix_expression_part_struct_arrow_dereference(struct asm_state * state, struct identifier_id id){
	return make_postfix_expression_part(state, ASM_POSTFIX_EXPRESSION_PART_STRUCT_ARROW_DEREFERENCE, id.id);
}
struct postfix_expression_part_id make_postfix_expression_part_function_dereference(struct asm_state * state, struct expression_list_item_id id){
	return make_postfix_expression_part(state, ASM_POSTFIX_EXPRESSION_PART_FUNCTION_DEREFERENCE, id.id);
}
struct postfix_expression_part_id make_postfix_expression_part_inc(struct asm_state * state){
	unsigned int id;
	id = 0;
	return make_postfix_expression_part(state, ASM_POSTFIX_EXPRESSION_PART_INC, id);
}
struct postfix_expression_part_id make_postfix_expression_part_dec(struct asm_state * state){
	unsigned int id;
	id = 0;
	return make_postfix_expression_part(state, ASM_POSTFIX_EXPRESSION_PART_DEC, id);
}
struct postfix_expression_part_id make_postfix_expression_part(struct asm_state * state, enum asm_postfix_expression_part_kind kind, unsigned int id){
	struct postfix_expression_part_details d;
	d.type = kind;
	d.id = id;
	if(struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map_exists(&state->postfix_expression_parts_map, d)){
		return struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map_get(&state->postfix_expression_parts_map, d);
	}else{
		struct postfix_expression_part_id new_id;
		new_id.id = struct_postfix_expression_part_details_list_size(&state->postfix_expression_parts);
		struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map_put(&state->postfix_expression_parts_map, d, new_id);
		struct_postfix_expression_part_details_list_add_end(&state->postfix_expression_parts, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_POSTFIX_EXPRESSION_PART);
		return new_id;
	}
}

struct unscoped_tag_specifier_id make_unscoped_tag_specifier_struct_specifier(struct asm_state * state, struct unscoped_struct_specifier_id id){
	return make_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_STRUCT_SPECIFIER, id.id);
}
struct unscoped_tag_specifier_id make_unscoped_tag_specifier_union_specifier(struct asm_state * state, struct unscoped_union_specifier_id id){
	return make_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_UNION_SPECIFIER, id.id);
}
struct unscoped_tag_specifier_id make_unscoped_tag_specifier_enum_specifier(struct asm_state * state, struct unscoped_enum_specifier_id id){
	return make_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_ENUM_SPECIFIER, id.id);
}
struct unscoped_tag_specifier_id make_unscoped_tag_specifier(struct asm_state * state, enum asm_unscoped_tag_specifier_kind kind, unsigned int id){
	struct unscoped_tag_specifier_details d;
	d.type = kind;
	d.id = id;
	if(struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_exists(&state->unscoped_tag_specifiers_map, d)){
		return struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_get(&state->unscoped_tag_specifiers_map, d);
	}else{
		struct unscoped_tag_specifier_id new_id;
		new_id.id = struct_unscoped_tag_specifier_details_list_size(&state->unscoped_tag_specifiers);
		struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map_put(&state->unscoped_tag_specifiers_map, d, new_id);
		struct_unscoped_tag_specifier_details_list_add_end(&state->unscoped_tag_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_UNSCOPED_TAG_SPECIFIER);
		return new_id;
	}
}

struct scoped_typename_specifier_id make_scoped_typename_specifier_scoped_typename(struct asm_state * state, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id){
	return make_scoped_typename_specifier(state, ASM_SCOPED_TYPENAME_SPECIFIER_SCOPED_TYPENAME, scope_guid_id, identifier_id);
}
struct scoped_typename_specifier_id make_scoped_typename_specifier(struct asm_state * state, enum asm_scoped_typename_specifier_kind kind, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id){
	struct scoped_typename_specifier_details d;
	d.type = kind;
	d.scope_guid_id = scope_guid_id;
	d.identifier_id = identifier_id;
	if(struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map_exists(&state->scoped_typename_specifiers_map, d)){
		return struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map_get(&state->scoped_typename_specifiers_map, d);
	}else{
		struct scoped_typename_specifier_id new_id;
		new_id.id = struct_scoped_typename_specifier_details_list_size(&state->scoped_typename_specifiers);
		struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map_put(&state->scoped_typename_specifiers_map, d, new_id);
		struct_scoped_typename_specifier_details_list_add_end(&state->scoped_typename_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SCOPED_TYPENAME_SPECIFIER);
		return new_id;
	}
}

struct scoped_tag_specifier_id make_scoped_tag_specifier_scoped_tag_specifier(struct asm_state * state, struct scope_guid_id scope_guid_id, struct unscoped_tag_specifier_id unscoped_tag_specifier_id){
	return make_scoped_tag_specifier(state, ASM_SCOPED_TAG_SPECIFIER_SCOPED_TAG_SPECIFIER, scope_guid_id, unscoped_tag_specifier_id);
}
struct scoped_tag_specifier_id make_scoped_tag_specifier(struct asm_state * state, enum asm_scoped_tag_specifier_kind kind, struct scope_guid_id scope_guid_id, struct unscoped_tag_specifier_id unscoped_tag_specifier_id){
	struct scoped_tag_specifier_details d;
	d.type = kind;
	d.scope_guid_id = scope_guid_id;
	d.unscoped_tag_specifier_id = unscoped_tag_specifier_id;
	if(struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_exists(&state->scoped_tag_specifiers_map, d)){
		return struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_get(&state->scoped_tag_specifiers_map, d);
	}else{
		struct scoped_tag_specifier_id new_id;
		new_id.id = struct_scoped_tag_specifier_details_list_size(&state->scoped_tag_specifiers);
		struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map_put(&state->scoped_tag_specifiers_map, d, new_id);
		struct_scoped_tag_specifier_details_list_add_end(&state->scoped_tag_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SCOPED_TAG_SPECIFIER);
		return new_id;
	}
}

struct unscoped_union_specifier_id make_unscoped_union_specifier_named_union(struct asm_state * state, struct identifier_id id){
	return make_unscoped_union_specifier(state, ASM_UNSCOPED_UNION_SPECIFIER_NAMED_UNION, id.id);
}
struct unscoped_union_specifier_id make_unscoped_union_specifier_anonymous_union(struct asm_state * state, struct anonymous_instance_id id){
	return make_unscoped_union_specifier(state, ASM_UNSCOPED_UNION_SPECIFIER_ANONYMOUS_UNION, id.id);
}
struct unscoped_union_specifier_id make_unscoped_union_specifier(struct asm_state * state, enum asm_unscoped_union_specifier_kind kind, unsigned int id){
	struct unscoped_union_specifier_details d;
	d.type = kind;
	d.id = id;
	if(struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map_exists(&state->unscoped_union_specifiers_map, d)){
		return struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map_get(&state->unscoped_union_specifiers_map, d);
	}else{
		struct unscoped_union_specifier_id new_id;
		new_id.id = struct_unscoped_union_specifier_details_list_size(&state->unscoped_union_specifiers);
		struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map_put(&state->unscoped_union_specifiers_map, d, new_id);
		struct_unscoped_union_specifier_details_list_add_end(&state->unscoped_union_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_UNSCOPED_UNION_SPECIFIER);
		return new_id;
	}
}

struct unscoped_struct_specifier_id make_unscoped_struct_specifier_named_struct(struct asm_state * state, struct identifier_id id){
	return make_unscoped_struct_specifier(state, ASM_UNSCOPED_STRUCT_SPECIFIER_NAMED_STRUCT, id.id);
}
struct unscoped_struct_specifier_id make_unscoped_struct_specifier_anonymous_struct(struct asm_state * state, struct anonymous_instance_id id){
	return make_unscoped_struct_specifier(state, ASM_UNSCOPED_STRUCT_SPECIFIER_ANONYMOUS_STRUCT, id.id);
}
struct unscoped_struct_specifier_id make_unscoped_struct_specifier(struct asm_state * state, enum asm_unscoped_struct_specifier_kind kind, unsigned int id){
	struct unscoped_struct_specifier_details d;
	d.type = kind;
	d.id = id;
	if(struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map_exists(&state->unscoped_struct_specifiers_map, d)){
		return struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map_get(&state->unscoped_struct_specifiers_map, d);
	}else{
		struct unscoped_struct_specifier_id new_id;
		new_id.id = struct_unscoped_struct_specifier_details_list_size(&state->unscoped_struct_specifiers);
		struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map_put(&state->unscoped_struct_specifiers_map, d, new_id);
		struct_unscoped_struct_specifier_details_list_add_end(&state->unscoped_struct_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_UNSCOPED_STRUCT_SPECIFIER);
		return new_id;
	}
}

struct unscoped_enum_specifier_id make_unscoped_enum_specifier_named_enum(struct asm_state * state, struct identifier_id id){
	return make_unscoped_enum_specifier(state, ASM_UNSCOPED_ENUM_SPECIFIER_NAMED_ENUM, id.id);
}
struct unscoped_enum_specifier_id make_unscoped_enum_specifier_anonymous_enum(struct asm_state * state, struct anonymous_instance_id id){
	return make_unscoped_enum_specifier(state, ASM_UNSCOPED_ENUM_SPECIFIER_ANONYMOUS_ENUM, id.id);
}
struct unscoped_enum_specifier_id make_unscoped_enum_specifier(struct asm_state * state, enum asm_unscoped_enum_specifier_kind kind, unsigned int id){
	struct unscoped_enum_specifier_details d;
	d.type = kind;
	d.id = id;
	if(struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map_exists(&state->unscoped_enum_specifiers_map, d)){
		return struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map_get(&state->unscoped_enum_specifiers_map, d);
	}else{
		struct unscoped_enum_specifier_id new_id;
		new_id.id = struct_unscoped_enum_specifier_details_list_size(&state->unscoped_enum_specifiers);
		struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map_put(&state->unscoped_enum_specifiers_map, d, new_id);
		struct_unscoped_enum_specifier_details_list_add_end(&state->unscoped_enum_specifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_UNSCOPED_ENUM_SPECIFIER);
		return new_id;
	}
}

struct bitfield_id make_bitfield_no_bitfield(struct asm_state * state){
	struct expression_id expression_id;
	expression_id.id = 0;
	return make_bitfield(state, ASM_BITFIELD_NO_BITFIELD, expression_id);
}
struct bitfield_id make_bitfield_constant_bitfield(struct asm_state * state, struct expression_id expression_id){
	return make_bitfield(state, ASM_BITFIELD_CONSTANT_BITFIELD, expression_id);
}
struct bitfield_id make_bitfield(struct asm_state * state, enum asm_bitfield_kind kind, struct expression_id expression_id){
	struct bitfield_details d;
	d.type = kind;
	d.expression_id = expression_id;
	if(struct_bitfield_details_to_struct_bitfield_id_map_exists(&state->bitfields_map, d)){
		return struct_bitfield_details_to_struct_bitfield_id_map_get(&state->bitfields_map, d);
	}else{
		struct bitfield_id new_id;
		new_id.id = struct_bitfield_details_list_size(&state->bitfields);
		struct_bitfield_details_to_struct_bitfield_id_map_put(&state->bitfields_map, d, new_id);
		struct_bitfield_details_list_add_end(&state->bitfields, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_BITFIELD);
		return new_id;
	}
}

struct jump_statement_id make_jump_statement_return_expression(struct asm_state * state, struct expression_id id){
	return make_jump_statement(state, ASM_JUMP_STATEMENT_RETURN_EXPRESSION, id.id);
}
struct jump_statement_id make_jump_statement_return_no_expression(struct asm_state * state){
	unsigned int id;
	id = 0;
	return make_jump_statement(state, ASM_JUMP_STATEMENT_RETURN_NO_EXPRESSION, id);
}
struct jump_statement_id make_jump_statement_break(struct asm_state * state){
	unsigned int id;
	id = 0;
	return make_jump_statement(state, ASM_JUMP_STATEMENT_BREAK, id);
}
struct jump_statement_id make_jump_statement_continue(struct asm_state * state){
	unsigned int id;
	id = 0;
	return make_jump_statement(state, ASM_JUMP_STATEMENT_CONTINUE, id);
}
struct jump_statement_id make_jump_statement_goto(struct asm_state * state, struct identifier_id id){
	return make_jump_statement(state, ASM_JUMP_STATEMENT_GOTO, id.id);
}
struct jump_statement_id make_jump_statement(struct asm_state * state, enum asm_jump_statement_kind kind, unsigned int id){
	struct jump_statement_details d;
	d.type = kind;
	d.id = id;
	if(struct_jump_statement_details_to_struct_jump_statement_id_map_exists(&state->jump_statements_map, d)){
		return struct_jump_statement_details_to_struct_jump_statement_id_map_get(&state->jump_statements_map, d);
	}else{
		struct jump_statement_id new_id;
		new_id.id = struct_jump_statement_details_list_size(&state->jump_statements);
		struct_jump_statement_details_to_struct_jump_statement_id_map_put(&state->jump_statements_map, d, new_id);
		struct_jump_statement_details_list_add_end(&state->jump_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_JUMP_STATEMENT);
		return new_id;
	}
}

struct switch_statement_id make_switch_statement_switch(struct asm_state * state, struct expression_id e1, struct any_statement_id s1){
	return make_switch_statement(state, ASM_SWITCH_STATEMENT_SWITCH, e1, s1);
}
struct switch_statement_id make_switch_statement(struct asm_state * state, enum asm_switch_statement_kind kind, struct expression_id e1, struct any_statement_id s1){
	struct switch_statement_details d;
	d.type = kind;
	d.e1 = e1;
	d.s1 = s1;
	if(struct_switch_statement_details_to_struct_switch_statement_id_map_exists(&state->switch_statements_map, d)){
		return struct_switch_statement_details_to_struct_switch_statement_id_map_get(&state->switch_statements_map, d);
	}else{
		struct switch_statement_id new_id;
		new_id.id = struct_switch_statement_details_list_size(&state->switch_statements);
		struct_switch_statement_details_to_struct_switch_statement_id_map_put(&state->switch_statements_map, d, new_id);
		struct_switch_statement_details_list_add_end(&state->switch_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SWITCH_STATEMENT);
		return new_id;
	}
}

struct if_statement_id make_if_statement_just_if(struct asm_state * state, struct any_statement_id s1, struct expression_id e1){
	struct any_statement_id s2;
	s2.id = 0;
	return make_if_statement(state, ASM_IF_STATEMENT_JUST_IF, s1, e1, s2);
}
struct if_statement_id make_if_statement_if_else(struct asm_state * state, struct any_statement_id s1, struct expression_id e1, struct any_statement_id s2){
	return make_if_statement(state, ASM_IF_STATEMENT_IF_ELSE, s1, e1, s2);
}
struct if_statement_id make_if_statement(struct asm_state * state, enum asm_if_statement_kind kind, struct any_statement_id s1, struct expression_id e1, struct any_statement_id s2){
	struct if_statement_details d;
	d.type = kind;
	d.s1 = s1;
	d.e1 = e1;
	d.s2 = s2;
	if(struct_if_statement_details_to_struct_if_statement_id_map_exists(&state->if_statements_map, d)){
		return struct_if_statement_details_to_struct_if_statement_id_map_get(&state->if_statements_map, d);
	}else{
		struct if_statement_id new_id;
		new_id.id = struct_if_statement_details_list_size(&state->if_statements);
		struct_if_statement_details_to_struct_if_statement_id_map_put(&state->if_statements_map, d, new_id);
		struct_if_statement_details_list_add_end(&state->if_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_IF_STATEMENT);
		return new_id;
	}
}

struct while_statement_id make_while_statement_do_while(struct asm_state * state, struct expression_id e1, struct any_statement_id s1){
	return make_while_statement(state, ASM_WHILE_STATEMENT_DO_WHILE, e1, s1);
}
struct while_statement_id make_while_statement_while(struct asm_state * state, struct expression_id e1, struct any_statement_id s1){
	return make_while_statement(state, ASM_WHILE_STATEMENT_WHILE, e1, s1);
}
struct while_statement_id make_while_statement(struct asm_state * state, enum asm_while_statement_kind kind, struct expression_id e1, struct any_statement_id s1){
	struct while_statement_details d;
	d.type = kind;
	d.e1 = e1;
	d.s1 = s1;
	if(struct_while_statement_details_to_struct_while_statement_id_map_exists(&state->while_statements_map, d)){
		return struct_while_statement_details_to_struct_while_statement_id_map_get(&state->while_statements_map, d);
	}else{
		struct while_statement_id new_id;
		new_id.id = struct_while_statement_details_list_size(&state->while_statements);
		struct_while_statement_details_to_struct_while_statement_id_map_put(&state->while_statements_map, d, new_id);
		struct_while_statement_details_list_add_end(&state->while_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_WHILE_STATEMENT);
		return new_id;
	}
}

struct for_statement_id make_for_statement_for(struct asm_state * state, struct expression_list_item_id e1, struct expression_list_item_id e2, struct expression_list_item_id e3){
	return make_for_statement(state, ASM_FOR_STATEMENT_FOR, e1, e2, e3);
}
struct for_statement_id make_for_statement(struct asm_state * state, enum asm_for_statement_kind kind, struct expression_list_item_id e1, struct expression_list_item_id e2, struct expression_list_item_id e3){
	struct for_statement_details d;
	d.type = kind;
	d.e1 = e1;
	d.e2 = e2;
	d.e3 = e3;
	if(struct_for_statement_details_to_struct_for_statement_id_map_exists(&state->for_statements_map, d)){
		return struct_for_statement_details_to_struct_for_statement_id_map_get(&state->for_statements_map, d);
	}else{
		struct for_statement_id new_id;
		new_id.id = struct_for_statement_details_list_size(&state->for_statements);
		struct_for_statement_details_to_struct_for_statement_id_map_put(&state->for_statements_map, d, new_id);
		struct_for_statement_details_list_add_end(&state->for_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_FOR_STATEMENT);
		return new_id;
	}
}

struct code_statement_id make_code_statement_labeled_statement(struct asm_state * state, struct labeled_statement_id id){
	return make_code_statement(state, ASM_CODE_STATEMENT_LABELED_STATEMENT, id.id);
}
struct code_statement_id make_code_statement_compound_statement(struct asm_state * state, struct any_statement_list_item_id id){
	return make_code_statement(state, ASM_CODE_STATEMENT_COMPOUND_STATEMENT, id.id);
}
struct code_statement_id make_code_statement_expression_statement(struct asm_state * state, struct expression_list_item_id id){
	return make_code_statement(state, ASM_CODE_STATEMENT_EXPRESSION_STATEMENT, id.id);
}
struct code_statement_id make_code_statement_for_statement(struct asm_state * state, struct for_statement_id id){
	return make_code_statement(state, ASM_CODE_STATEMENT_FOR_STATEMENT, id.id);
}
struct code_statement_id make_code_statement_while_statement(struct asm_state * state, struct while_statement_id id){
	return make_code_statement(state, ASM_CODE_STATEMENT_WHILE_STATEMENT, id.id);
}
struct code_statement_id make_code_statement_if_statement(struct asm_state * state, struct if_statement_id id){
	return make_code_statement(state, ASM_CODE_STATEMENT_IF_STATEMENT, id.id);
}
struct code_statement_id make_code_statement_switch_statement(struct asm_state * state, struct switch_statement_id id){
	return make_code_statement(state, ASM_CODE_STATEMENT_SWITCH_STATEMENT, id.id);
}
struct code_statement_id make_code_statement_jump_statement(struct asm_state * state, struct jump_statement_id id){
	return make_code_statement(state, ASM_CODE_STATEMENT_JUMP_STATEMENT, id.id);
}
struct code_statement_id make_code_statement(struct asm_state * state, enum asm_code_statement_kind kind, unsigned int id){
	struct code_statement_details d;
	d.type = kind;
	d.id = id;
	if(struct_code_statement_details_to_struct_code_statement_id_map_exists(&state->code_statements_map, d)){
		return struct_code_statement_details_to_struct_code_statement_id_map_get(&state->code_statements_map, d);
	}else{
		struct code_statement_id new_id;
		new_id.id = struct_code_statement_details_list_size(&state->code_statements);
		struct_code_statement_details_to_struct_code_statement_id_map_put(&state->code_statements_map, d, new_id);
		struct_code_statement_details_list_add_end(&state->code_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_CODE_STATEMENT);
		return new_id;
	}
}

struct labeled_statement_id make_labeled_statement_goto_label(struct asm_state * state, struct any_statement_id any_statement_id){
	struct expression_id expression_id;
	expression_id.id = 0;
	return make_labeled_statement(state, ASM_LABELED_STATEMENT_GOTO_LABEL, any_statement_id, expression_id);
}
struct labeled_statement_id make_labeled_statement_case_label(struct asm_state * state, struct any_statement_id any_statement_id, struct expression_id expression_id){
	return make_labeled_statement(state, ASM_LABELED_STATEMENT_CASE_LABEL, any_statement_id, expression_id);
}
struct labeled_statement_id make_labeled_statement_default_label(struct asm_state * state, struct any_statement_id any_statement_id){
	struct expression_id expression_id;
	expression_id.id = 0;
	return make_labeled_statement(state, ASM_LABELED_STATEMENT_DEFAULT_LABEL, any_statement_id, expression_id);
}
struct labeled_statement_id make_labeled_statement(struct asm_state * state, enum asm_labeled_statement_kind kind, struct any_statement_id any_statement_id, struct expression_id expression_id){
	struct labeled_statement_details d;
	d.type = kind;
	d.any_statement_id = any_statement_id;
	d.expression_id = expression_id;
	if(struct_labeled_statement_details_to_struct_labeled_statement_id_map_exists(&state->labeled_statements_map, d)){
		return struct_labeled_statement_details_to_struct_labeled_statement_id_map_get(&state->labeled_statements_map, d);
	}else{
		struct labeled_statement_id new_id;
		new_id.id = struct_labeled_statement_details_list_size(&state->labeled_statements);
		struct_labeled_statement_details_to_struct_labeled_statement_id_map_put(&state->labeled_statements_map, d, new_id);
		struct_labeled_statement_details_list_add_end(&state->labeled_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_LABELED_STATEMENT);
		return new_id;
	}
}

struct postfix_expression_id make_postfix_expression_postfix_expression(struct asm_state * state, struct primary_expression_id primary_expression_id, struct postfix_expression_part_list_item_id postfix_expression_part_list_item_id){
	return make_postfix_expression(state, ASM_POSTFIX_EXPRESSION_POSTFIX_EXPRESSION, primary_expression_id, postfix_expression_part_list_item_id);
}
struct postfix_expression_id make_postfix_expression(struct asm_state * state, enum asm_postfix_expression_kind kind, struct primary_expression_id primary_expression_id, struct postfix_expression_part_list_item_id postfix_expression_part_list_item_id){
	struct postfix_expression_details d;
	d.type = kind;
	d.primary_expression_id = primary_expression_id;
	d.postfix_expression_part_list_item_id = postfix_expression_part_list_item_id;
	if(struct_postfix_expression_details_to_struct_postfix_expression_id_map_exists(&state->postfix_expressions_map, d)){
		return struct_postfix_expression_details_to_struct_postfix_expression_id_map_get(&state->postfix_expressions_map, d);
	}else{
		struct postfix_expression_id new_id;
		new_id.id = struct_postfix_expression_details_list_size(&state->postfix_expressions);
		struct_postfix_expression_details_to_struct_postfix_expression_id_map_put(&state->postfix_expressions_map, d, new_id);
		struct_postfix_expression_details_list_add_end(&state->postfix_expressions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_POSTFIX_EXPRESSION);
		return new_id;
	}
}

struct primary_expression_id make_primary_expression_constant_description(struct asm_state * state, struct constant_description_id id){
	return make_primary_expression(state, ASM_PRIMARY_EXPRESSION_CONSTANT_DESCRIPTION, id.id);
}
struct primary_expression_id make_primary_expression_identifier(struct asm_state * state, struct identifier_id id){
	return make_primary_expression(state, ASM_PRIMARY_EXPRESSION_IDENTIFIER, id.id);
}
struct primary_expression_id make_primary_expression_expression(struct asm_state * state, struct expression_id id){
	return make_primary_expression(state, ASM_PRIMARY_EXPRESSION_EXPRESSION, id.id);
}
struct primary_expression_id make_primary_expression(struct asm_state * state, enum asm_primary_expression_kind kind, unsigned int id){
	struct primary_expression_details d;
	d.type = kind;
	d.id = id;
	if(struct_primary_expression_details_to_struct_primary_expression_id_map_exists(&state->primary_expressions_map, d)){
		return struct_primary_expression_details_to_struct_primary_expression_id_map_get(&state->primary_expressions_map, d);
	}else{
		struct primary_expression_id new_id;
		new_id.id = struct_primary_expression_details_list_size(&state->primary_expressions);
		struct_primary_expression_details_to_struct_primary_expression_id_map_put(&state->primary_expressions_map, d, new_id);
		struct_primary_expression_details_list_add_end(&state->primary_expressions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_PRIMARY_EXPRESSION);
		return new_id;
	}
}

struct any_statement_id make_any_statement_declaration_statement(struct asm_state * state, struct declaration_statement_id id){
	return make_any_statement(state, ASM_ANY_STATEMENT_DECLARATION_STATEMENT, id.id);
}
struct any_statement_id make_any_statement_scoped_statement(struct asm_state * state, struct scoped_statement_id id){
	return make_any_statement(state, ASM_ANY_STATEMENT_SCOPED_STATEMENT, id.id);
}
struct any_statement_id make_any_statement_code_statement(struct asm_state * state, struct code_statement_id id){
	return make_any_statement(state, ASM_ANY_STATEMENT_CODE_STATEMENT, id.id);
}
struct any_statement_id make_any_statement(struct asm_state * state, enum asm_any_statement_kind kind, unsigned int id){
	struct any_statement_details d;
	d.type = kind;
	d.id = id;
	if(struct_any_statement_details_to_struct_any_statement_id_map_exists(&state->any_statements_map, d)){
		return struct_any_statement_details_to_struct_any_statement_id_map_get(&state->any_statements_map, d);
	}else{
		struct any_statement_id new_id;
		new_id.id = struct_any_statement_details_list_size(&state->any_statements);
		struct_any_statement_details_to_struct_any_statement_id_map_put(&state->any_statements_map, d, new_id);
		struct_any_statement_details_list_add_end(&state->any_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ANY_STATEMENT);
		return new_id;
	}
}

struct scoped_statement_id make_scoped_statement_function_scope(struct asm_state * state, struct any_statement_list_item_id any_statement_list_item_id, struct scope_guid_id scope_guid_id){
	return make_scoped_statement(state, ASM_SCOPED_STATEMENT_FUNCTION_SCOPE, any_statement_list_item_id, scope_guid_id);
}
struct scoped_statement_id make_scoped_statement_block_scope(struct asm_state * state, struct any_statement_list_item_id any_statement_list_item_id, struct scope_guid_id scope_guid_id){
	return make_scoped_statement(state, ASM_SCOPED_STATEMENT_BLOCK_SCOPE, any_statement_list_item_id, scope_guid_id);
}
struct scoped_statement_id make_scoped_statement_file_scope(struct asm_state * state, struct any_statement_list_item_id any_statement_list_item_id, struct scope_guid_id scope_guid_id){
	return make_scoped_statement(state, ASM_SCOPED_STATEMENT_FILE_SCOPE, any_statement_list_item_id, scope_guid_id);
}
struct scoped_statement_id make_scoped_statement(struct asm_state * state, enum asm_scoped_statement_kind kind, struct any_statement_list_item_id any_statement_list_item_id, struct scope_guid_id scope_guid_id){
	struct scoped_statement_details d;
	d.type = kind;
	d.any_statement_list_item_id = any_statement_list_item_id;
	d.scope_guid_id = scope_guid_id;
	if(struct_scoped_statement_details_to_struct_scoped_statement_id_map_exists(&state->scoped_statements_map, d)){
		return struct_scoped_statement_details_to_struct_scoped_statement_id_map_get(&state->scoped_statements_map, d);
	}else{
		struct scoped_statement_id new_id;
		new_id.id = struct_scoped_statement_details_list_size(&state->scoped_statements);
		struct_scoped_statement_details_to_struct_scoped_statement_id_map_put(&state->scoped_statements_map, d, new_id);
		struct_scoped_statement_details_list_add_end(&state->scoped_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SCOPED_STATEMENT);
		return new_id;
	}
}

struct declaration_statement_id make_declaration_statement_simple_declaration(struct asm_state * state, struct simple_declaration_id id){
	return make_declaration_statement(state, ASM_DECLARATION_STATEMENT_SIMPLE_DECLARATION, id.id);
}
struct declaration_statement_id make_declaration_statement_function_definition(struct asm_state * state, struct function_definition_id id){
	return make_declaration_statement(state, ASM_DECLARATION_STATEMENT_FUNCTION_DEFINITION, id.id);
}
struct declaration_statement_id make_declaration_statement(struct asm_state * state, enum asm_declaration_statement_kind kind, unsigned int id){
	struct declaration_statement_details d;
	d.type = kind;
	d.id = id;
	if(struct_declaration_statement_details_to_struct_declaration_statement_id_map_exists(&state->declaration_statements_map, d)){
		return struct_declaration_statement_details_to_struct_declaration_statement_id_map_get(&state->declaration_statements_map, d);
	}else{
		struct declaration_statement_id new_id;
		new_id.id = struct_declaration_statement_details_list_size(&state->declaration_statements);
		struct_declaration_statement_details_to_struct_declaration_statement_id_map_put(&state->declaration_statements_map, d, new_id);
		struct_declaration_statement_details_list_add_end(&state->declaration_statements, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_DECLARATION_STATEMENT);
		return new_id;
	}
}

struct function_definition_id make_function_definition_prototyped(struct asm_state * state, struct general_type_id general_type_id, struct any_statement_list_item_id params, struct any_statement_list_item_id body, struct identifier_id identifier_id){
	return make_function_definition(state, ASM_FUNCTION_DEFINITION_PROTOTYPED, general_type_id, params, body, identifier_id);
}
struct function_definition_id make_function_definition_k_and_r_c(struct asm_state * state, struct general_type_id general_type_id, struct any_statement_list_item_id params, struct any_statement_list_item_id body, struct identifier_id identifier_id){
	return make_function_definition(state, ASM_FUNCTION_DEFINITION_K_AND_R_C, general_type_id, params, body, identifier_id);
}
struct function_definition_id make_function_definition(struct asm_state * state, enum asm_function_definition_kind kind, struct general_type_id general_type_id, struct any_statement_list_item_id params, struct any_statement_list_item_id body, struct identifier_id identifier_id){
	struct function_definition_details d;
	d.type = kind;
	d.general_type_id = general_type_id;
	d.params = params;
	d.body = body;
	d.identifier_id = identifier_id;
	if(struct_function_definition_details_to_struct_function_definition_id_map_exists(&state->function_definitions_map, d)){
		return struct_function_definition_details_to_struct_function_definition_id_map_get(&state->function_definitions_map, d);
	}else{
		struct function_definition_id new_id;
		new_id.id = struct_function_definition_details_list_size(&state->function_definitions);
		struct_function_definition_details_to_struct_function_definition_id_map_put(&state->function_definitions_map, d, new_id);
		struct_function_definition_details_list_add_end(&state->function_definitions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_FUNCTION_DEFINITION);
		return new_id;
	}
}

struct simple_declaration_id make_simple_declaration_simple_declaration(struct asm_state * state, struct general_type_id general_type_id, struct identifier_id identifier_id, struct initializer_id initializer_id){
	return make_simple_declaration(state, ASM_SIMPLE_DECLARATION_SIMPLE_DECLARATION, general_type_id, identifier_id, initializer_id);
}
struct simple_declaration_id make_simple_declaration(struct asm_state * state, enum asm_simple_declaration_kind kind, struct general_type_id general_type_id, struct identifier_id identifier_id, struct initializer_id initializer_id){
	struct simple_declaration_details d;
	d.type = kind;
	d.general_type_id = general_type_id;
	d.identifier_id = identifier_id;
	d.initializer_id = initializer_id;
	if(struct_simple_declaration_details_to_struct_simple_declaration_id_map_exists(&state->simple_declarations_map, d)){
		return struct_simple_declaration_details_to_struct_simple_declaration_id_map_get(&state->simple_declarations_map, d);
	}else{
		struct simple_declaration_id new_id;
		new_id.id = struct_simple_declaration_details_list_size(&state->simple_declarations);
		struct_simple_declaration_details_to_struct_simple_declaration_id_map_put(&state->simple_declarations_map, d, new_id);
		struct_simple_declaration_details_list_add_end(&state->simple_declarations, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SIMPLE_DECLARATION);
		return new_id;
	}
}

struct expression_id make_expression_cast_expression(struct asm_state * state, struct cast_expression_id id){
	return make_expression(state, ASM_EXPRESSION_CAST_EXPRESSION, id.id);
}
struct expression_id make_expression_binary_expression(struct asm_state * state, struct binary_expression_id id){
	return make_expression(state, ASM_EXPRESSION_BINARY_EXPRESSION, id.id);
}
struct expression_id make_expression_ternary_expression(struct asm_state * state, struct ternary_expression_id id){
	return make_expression(state, ASM_EXPRESSION_TERNARY_EXPRESSION, id.id);
}
struct expression_id make_expression(struct asm_state * state, enum asm_expression_kind kind, unsigned int id){
	struct expression_details d;
	d.type = kind;
	d.id = id;
	if(struct_expression_details_to_struct_expression_id_map_exists(&state->expressions_map, d)){
		return struct_expression_details_to_struct_expression_id_map_get(&state->expressions_map, d);
	}else{
		struct expression_id new_id;
		new_id.id = struct_expression_details_list_size(&state->expressions);
		struct_expression_details_to_struct_expression_id_map_put(&state->expressions_map, d, new_id);
		struct_expression_details_list_add_end(&state->expressions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_EXPRESSION);
		return new_id;
	}
}

struct cast_expression_id make_cast_expression_has_cast(struct asm_state * state, struct cast_expression_id id1, struct general_type_id id2){
	return make_cast_expression(state, ASM_CAST_EXPRESSION_HAS_CAST, id1.id, id2);
}
struct cast_expression_id make_cast_expression_no_cast(struct asm_state * state, struct unary_expression_id id1){
	struct general_type_id id2;
	id2.id = 0;
	return make_cast_expression(state, ASM_CAST_EXPRESSION_NO_CAST, id1.id, id2);
}
struct cast_expression_id make_cast_expression(struct asm_state * state, enum asm_cast_expression_kind kind, unsigned int id1, struct general_type_id id2){
	struct cast_expression_details d;
	d.type = kind;
	d.id1 = id1;
	d.id2 = id2;
	if(struct_cast_expression_details_to_struct_cast_expression_id_map_exists(&state->cast_expressions_map, d)){
		return struct_cast_expression_details_to_struct_cast_expression_id_map_get(&state->cast_expressions_map, d);
	}else{
		struct cast_expression_id new_id;
		new_id.id = struct_cast_expression_details_list_size(&state->cast_expressions);
		struct_cast_expression_details_to_struct_cast_expression_id_map_put(&state->cast_expressions_map, d, new_id);
		struct_cast_expression_details_list_add_end(&state->cast_expressions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_CAST_EXPRESSION);
		return new_id;
	}
}

struct unary_expression_id make_unary_expression_sizeof_expression(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_SIZEOF_EXPRESSION, id1.id);
}
struct unary_expression_id make_unary_expression_postfix_expression(struct asm_state * state, struct postfix_expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_POSTFIX_EXPRESSION, id1.id);
}
struct unary_expression_id make_unary_expression_sizeof_type(struct asm_state * state, struct general_type_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_SIZEOF_TYPE, id1.id);
}
struct unary_expression_id make_unary_expression_inc(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_INC, id1.id);
}
struct unary_expression_id make_unary_expression_dec(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_DEC, id1.id);
}
struct unary_expression_id make_unary_expression_address(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_ADDRESS, id1.id);
}
struct unary_expression_id make_unary_expression_point_to(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_POINT_TO, id1.id);
}
struct unary_expression_id make_unary_expression_positive(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_POSITIVE, id1.id);
}
struct unary_expression_id make_unary_expression_negative(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_NEGATIVE, id1.id);
}
struct unary_expression_id make_unary_expression_bitwise_not(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_BITWISE_NOT, id1.id);
}
struct unary_expression_id make_unary_expression_logical_not(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_LOGICAL_NOT, id1.id);
}
struct unary_expression_id make_unary_expression(struct asm_state * state, enum asm_unary_expression_kind kind, unsigned int id1){
	struct unary_expression_details d;
	d.type = kind;
	d.id1 = id1;
	if(struct_unary_expression_details_to_struct_unary_expression_id_map_exists(&state->unary_expressions_map, d)){
		return struct_unary_expression_details_to_struct_unary_expression_id_map_get(&state->unary_expressions_map, d);
	}else{
		struct unary_expression_id new_id;
		new_id.id = struct_unary_expression_details_list_size(&state->unary_expressions);
		struct_unary_expression_details_to_struct_unary_expression_id_map_put(&state->unary_expressions_map, d, new_id);
		struct_unary_expression_details_list_add_end(&state->unary_expressions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_UNARY_EXPRESSION);
		return new_id;
	}
}

struct binary_expression_id make_binary_expression_plus(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_PLUS, e1, e2);
}
struct binary_expression_id make_binary_expression_minus(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_MINUS, e1, e2);
}
struct binary_expression_id make_binary_expression_divide(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_DIVIDE, e1, e2);
}
struct binary_expression_id make_binary_expression_multiply(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_MULTIPLY, e1, e2);
}
struct binary_expression_id make_binary_expression_exclusive_or(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_EXCLUSIVE_OR, e1, e2);
}
struct binary_expression_id make_binary_expression_modulus(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_MODULUS, e1, e2);
}
struct binary_expression_id make_binary_expression_bitwise_or(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_BITWISE_OR, e1, e2);
}
struct binary_expression_id make_binary_expression_logical_or(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_LOGICAL_OR, e1, e2);
}
struct binary_expression_id make_binary_expression_bitwise_and(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_BITWISE_AND, e1, e2);
}
struct binary_expression_id make_binary_expression_logical_and(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_LOGICAL_AND, e1, e2);
}
struct binary_expression_id make_binary_expression_lt(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_LT, e1, e2);
}
struct binary_expression_id make_binary_expression_gt(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_GT, e1, e2);
}
struct binary_expression_id make_binary_expression_le(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_LE, e1, e2);
}
struct binary_expression_id make_binary_expression_ge(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_GE, e1, e2);
}
struct binary_expression_id make_binary_expression_eq(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_EQ, e1, e2);
}
struct binary_expression_id make_binary_expression_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_add_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_ADD_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_sub_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_SUB_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_div_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_DIV_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_mul_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_MUL_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_and_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_AND_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_or_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_OR_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_mod_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_MOD_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression(struct asm_state * state, enum asm_binary_expression_kind kind, struct expression_id e1, struct expression_id e2){
	struct binary_expression_details d;
	d.type = kind;
	d.e1 = e1;
	d.e2 = e2;
	if(struct_binary_expression_details_to_struct_binary_expression_id_map_exists(&state->binary_expressions_map, d)){
		return struct_binary_expression_details_to_struct_binary_expression_id_map_get(&state->binary_expressions_map, d);
	}else{
		struct binary_expression_id new_id;
		new_id.id = struct_binary_expression_details_list_size(&state->binary_expressions);
		struct_binary_expression_details_to_struct_binary_expression_id_map_put(&state->binary_expressions_map, d, new_id);
		struct_binary_expression_details_list_add_end(&state->binary_expressions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_BINARY_EXPRESSION);
		return new_id;
	}
}

struct ternary_expression_id make_ternary_expression_ternary_operator(struct asm_state * state, struct expression_id e1, struct expression_id e2, struct expression_id e3){
	return make_ternary_expression(state, ASM_TERNARY_EXPRESSION_TERNARY_OPERATOR, e1, e2, e3);
}
struct ternary_expression_id make_ternary_expression(struct asm_state * state, enum asm_ternary_expression_kind kind, struct expression_id e1, struct expression_id e2, struct expression_id e3){
	struct ternary_expression_details d;
	d.type = kind;
	d.e1 = e1;
	d.e2 = e2;
	d.e3 = e3;
	if(struct_ternary_expression_details_to_struct_ternary_expression_id_map_exists(&state->ternary_expressions_map, d)){
		return struct_ternary_expression_details_to_struct_ternary_expression_id_map_get(&state->ternary_expressions_map, d);
	}else{
		struct ternary_expression_id new_id;
		new_id.id = struct_ternary_expression_details_list_size(&state->ternary_expressions);
		struct_ternary_expression_details_to_struct_ternary_expression_id_map_put(&state->ternary_expressions_map, d, new_id);
		struct_ternary_expression_details_list_add_end(&state->ternary_expressions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_TERNARY_EXPRESSION);
		return new_id;
	}
}

struct constant_description_id make_constant_description_constant_description(struct asm_state * state, struct general_type_id general_type_id, struct data_package_id data_package_id){
	return make_constant_description(state, ASM_CONSTANT_DESCRIPTION_CONSTANT_DESCRIPTION, general_type_id, data_package_id);
}
struct constant_description_id make_constant_description(struct asm_state * state, enum asm_constant_description_kind kind, struct general_type_id general_type_id, struct data_package_id data_package_id){
	struct constant_description_details d;
	d.type = kind;
	d.general_type_id = general_type_id;
	d.data_package_id = data_package_id;
	if(struct_constant_description_details_to_struct_constant_description_id_map_exists(&state->constant_descriptions_map, d)){
		return struct_constant_description_details_to_struct_constant_description_id_map_get(&state->constant_descriptions_map, d);
	}else{
		struct constant_description_id new_id;
		new_id.id = struct_constant_description_details_list_size(&state->constant_descriptions);
		struct_constant_description_details_to_struct_constant_description_id_map_put(&state->constant_descriptions_map, d, new_id);
		struct_constant_description_details_list_add_end(&state->constant_descriptions, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_CONSTANT_DESCRIPTION);
		return new_id;
	}
}

struct data_package_id make_data_package_error(struct asm_state * state, struct error_node_id id){
	return make_data_package(state, ASM_DATA_PACKAGE_ERROR, id.id);
}
struct data_package_id make_data_package_byte(struct asm_state * state, struct byte_package_id id){
	return make_data_package(state, ASM_DATA_PACKAGE_BYTE, id.id);
}
struct data_package_id make_data_package_hword(struct asm_state * state, struct hword_package_id id){
	return make_data_package(state, ASM_DATA_PACKAGE_HWORD, id.id);
}
struct data_package_id make_data_package_word(struct asm_state * state, struct word_package_id id){
	return make_data_package(state, ASM_DATA_PACKAGE_WORD, id.id);
}
struct data_package_id make_data_package_dword(struct asm_state * state, struct dword_package_id id){
	return make_data_package(state, ASM_DATA_PACKAGE_DWORD, id.id);
}
struct data_package_id make_data_package_qword(struct asm_state * state, struct qword_package_id id){
	return make_data_package(state, ASM_DATA_PACKAGE_QWORD, id.id);
}
struct data_package_id make_data_package_string(struct asm_state * state, struct string_package_id id){
	return make_data_package(state, ASM_DATA_PACKAGE_STRING, id.id);
}
struct data_package_id make_data_package(struct asm_state * state, enum asm_data_package_kind kind, unsigned int id){
	struct data_package_details d;
	d.type = kind;
	d.id = id;
	if(struct_data_package_details_to_struct_data_package_id_map_exists(&state->data_packages_map, d)){
		return struct_data_package_details_to_struct_data_package_id_map_get(&state->data_packages_map, d);
	}else{
		struct data_package_id new_id;
		new_id.id = struct_data_package_details_list_size(&state->data_packages);
		struct_data_package_details_to_struct_data_package_id_map_put(&state->data_packages_map, d, new_id);
		struct_data_package_details_list_add_end(&state->data_packages, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_DATA_PACKAGE);
		return new_id;
	}
}

struct error_node_id make_error_node_error(struct asm_state * state){
	return make_error_node(state, ASM_ERROR_NODE_ERROR);
}
struct error_node_id make_error_node(struct asm_state * state, enum asm_error_node_kind kind){
	struct error_node_details d;
	d.type = kind;
	if(struct_error_node_details_to_struct_error_node_id_map_exists(&state->error_nodes_map, d)){
		return struct_error_node_details_to_struct_error_node_id_map_get(&state->error_nodes_map, d);
	}else{
		struct error_node_id new_id;
		new_id.id = struct_error_node_details_list_size(&state->error_nodes);
		struct_error_node_details_to_struct_error_node_id_map_put(&state->error_nodes_map, d, new_id);
		struct_error_node_details_list_add_end(&state->error_nodes, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ERROR_NODE);
		return new_id;
	}
}

struct byte_package_id make_byte_package_byte(struct asm_state * state, unsigned char data){
	return make_byte_package(state, ASM_BYTE_PACKAGE_BYTE, data);
}
struct byte_package_id make_byte_package(struct asm_state * state, enum asm_byte_package_kind kind, unsigned char data){
	struct byte_package_details d;
	d.type = kind;
	d.data = data;
	if(struct_byte_package_details_to_struct_byte_package_id_map_exists(&state->byte_packages_map, d)){
		return struct_byte_package_details_to_struct_byte_package_id_map_get(&state->byte_packages_map, d);
	}else{
		struct byte_package_id new_id;
		new_id.id = struct_byte_package_details_list_size(&state->byte_packages);
		struct_byte_package_details_to_struct_byte_package_id_map_put(&state->byte_packages_map, d, new_id);
		struct_byte_package_details_list_add_end(&state->byte_packages, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_BYTE_PACKAGE);
		return new_id;
	}
}

struct hword_package_id make_hword_package_hword(struct asm_state * state, unsigned char data0, unsigned char data1){
	return make_hword_package(state, ASM_HWORD_PACKAGE_HWORD, data0, data1);
}
struct hword_package_id make_hword_package(struct asm_state * state, enum asm_hword_package_kind kind, unsigned char data0, unsigned char data1){
	struct hword_package_details d;
	d.type = kind;
	d.data0 = data0;
	d.data1 = data1;
	if(struct_hword_package_details_to_struct_hword_package_id_map_exists(&state->hword_packages_map, d)){
		return struct_hword_package_details_to_struct_hword_package_id_map_get(&state->hword_packages_map, d);
	}else{
		struct hword_package_id new_id;
		new_id.id = struct_hword_package_details_list_size(&state->hword_packages);
		struct_hword_package_details_to_struct_hword_package_id_map_put(&state->hword_packages_map, d, new_id);
		struct_hword_package_details_list_add_end(&state->hword_packages, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_HWORD_PACKAGE);
		return new_id;
	}
}

struct word_package_id make_word_package_word(struct asm_state * state, unsigned int data){
	return make_word_package(state, ASM_WORD_PACKAGE_WORD, data);
}
struct word_package_id make_word_package(struct asm_state * state, enum asm_word_package_kind kind, unsigned int data){
	struct word_package_details d;
	d.type = kind;
	d.data = data;
	if(struct_word_package_details_to_struct_word_package_id_map_exists(&state->word_packages_map, d)){
		return struct_word_package_details_to_struct_word_package_id_map_get(&state->word_packages_map, d);
	}else{
		struct word_package_id new_id;
		new_id.id = struct_word_package_details_list_size(&state->word_packages);
		struct_word_package_details_to_struct_word_package_id_map_put(&state->word_packages_map, d, new_id);
		struct_word_package_details_list_add_end(&state->word_packages, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_WORD_PACKAGE);
		return new_id;
	}
}

struct qword_package_id make_qword_package_qword(struct asm_state * state, unsigned int data0, unsigned int data1, unsigned int data2, unsigned int data3){
	return make_qword_package(state, ASM_QWORD_PACKAGE_QWORD, data0, data1, data2, data3);
}
struct qword_package_id make_qword_package(struct asm_state * state, enum asm_qword_package_kind kind, unsigned int data0, unsigned int data1, unsigned int data2, unsigned int data3){
	struct qword_package_details d;
	d.type = kind;
	d.data0 = data0;
	d.data1 = data1;
	d.data2 = data2;
	d.data3 = data3;
	if(struct_qword_package_details_to_struct_qword_package_id_map_exists(&state->qword_packages_map, d)){
		return struct_qword_package_details_to_struct_qword_package_id_map_get(&state->qword_packages_map, d);
	}else{
		struct qword_package_id new_id;
		new_id.id = struct_qword_package_details_list_size(&state->qword_packages);
		struct_qword_package_details_to_struct_qword_package_id_map_put(&state->qword_packages_map, d, new_id);
		struct_qword_package_details_list_add_end(&state->qword_packages, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_QWORD_PACKAGE);
		return new_id;
	}
}

struct dword_package_id make_dword_package_dword(struct asm_state * state, unsigned int data0, unsigned int data1){
	return make_dword_package(state, ASM_DWORD_PACKAGE_DWORD, data0, data1);
}
struct dword_package_id make_dword_package(struct asm_state * state, enum asm_dword_package_kind kind, unsigned int data0, unsigned int data1){
	struct dword_package_details d;
	d.type = kind;
	d.data0 = data0;
	d.data1 = data1;
	if(struct_dword_package_details_to_struct_dword_package_id_map_exists(&state->dword_packages_map, d)){
		return struct_dword_package_details_to_struct_dword_package_id_map_get(&state->dword_packages_map, d);
	}else{
		struct dword_package_id new_id;
		new_id.id = struct_dword_package_details_list_size(&state->dword_packages);
		struct_dword_package_details_to_struct_dword_package_id_map_put(&state->dword_packages_map, d, new_id);
		struct_dword_package_details_list_add_end(&state->dword_packages, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_DWORD_PACKAGE);
		return new_id;
	}
}

struct string_package_id make_string_package_string(struct asm_state * state, unsigned int length, unsigned char * data){
	return make_string_package(state, ASM_STRING_PACKAGE_STRING, length, data);
}
struct string_package_id make_string_package(struct asm_state * state, enum asm_string_package_kind kind, unsigned int length, unsigned char * data){
	struct string_package_details d;
	d.type = kind;
	d.length = length;
	d.data = data;
	if(struct_string_package_details_to_struct_string_package_id_map_exists(&state->string_packages_map, d)){
		return struct_string_package_details_to_struct_string_package_id_map_get(&state->string_packages_map, d);
	}else{
		struct string_package_id new_id;
		new_id.id = struct_string_package_details_list_size(&state->string_packages);
		struct_string_package_details_to_struct_string_package_id_map_put(&state->string_packages_map, d, new_id);
		struct_string_package_details_list_add_end(&state->string_packages, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_STRING_PACKAGE);
		return new_id;
	}
}

struct identifier_id make_identifier_identifier(struct asm_state * state, struct c_lexer_token * identifier){
	return make_identifier(state, ASM_IDENTIFIER_IDENTIFIER, identifier);
}
struct identifier_id make_identifier(struct asm_state * state, enum asm_identifier_kind kind, struct c_lexer_token * identifier){
	struct identifier_details d;
	d.type = kind;
	d.identifier = identifier;
	if(struct_identifier_details_to_struct_identifier_id_map_exists(&state->identifiers_map, d)){
		return struct_identifier_details_to_struct_identifier_id_map_get(&state->identifiers_map, d);
	}else{
		struct identifier_id new_id;
		new_id.id = struct_identifier_details_list_size(&state->identifiers);
		struct_identifier_details_to_struct_identifier_id_map_put(&state->identifiers_map, d, new_id);
		struct_identifier_details_list_add_end(&state->identifiers, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_IDENTIFIER);
		return new_id;
	}
}

struct scope_guid_id make_scope_guid_block_scope(struct asm_state * state){
	unsigned int id;
	id = state->last_scope_guid++;
	return make_scope_guid(state, ASM_SCOPE_GUID_BLOCK_SCOPE, id);
}
struct scope_guid_id make_scope_guid_file_scope(struct asm_state * state){
	unsigned int id;
	id = state->last_scope_guid++;
	return make_scope_guid(state, ASM_SCOPE_GUID_FILE_SCOPE, id);
}
struct scope_guid_id make_scope_guid_function_scope(struct asm_state * state){
	unsigned int id;
	id = state->last_scope_guid++;
	return make_scope_guid(state, ASM_SCOPE_GUID_FUNCTION_SCOPE, id);
}
struct scope_guid_id make_scope_guid(struct asm_state * state, enum asm_scope_guid_kind kind, unsigned int id){
	struct scope_guid_details d;
	d.type = kind;
	d.id = id;
	if(struct_scope_guid_details_to_struct_scope_guid_id_map_exists(&state->scope_guids_map, d)){
		return struct_scope_guid_details_to_struct_scope_guid_id_map_get(&state->scope_guids_map, d);
	}else{
		struct scope_guid_id new_id;
		new_id.id = struct_scope_guid_details_list_size(&state->scope_guids);
		struct_scope_guid_details_to_struct_scope_guid_id_map_put(&state->scope_guids_map, d, new_id);
		struct_scope_guid_details_list_add_end(&state->scope_guids, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_SCOPE_GUID);
		return new_id;
	}
}

struct anonymous_instance_id make_anonymous_instance_anonymous_entry(struct asm_state * state){
	return make_anonymous_instance(state, ASM_ANONYMOUS_INSTANCE_ANONYMOUS_ENTRY);
}
struct anonymous_instance_id make_anonymous_instance(struct asm_state * state, enum asm_anonymous_instance_kind kind){
	struct anonymous_instance_details d;
	d.type = kind;
	if(struct_anonymous_instance_details_to_struct_anonymous_instance_id_map_exists(&state->anonymous_instances_map, d)){
		return struct_anonymous_instance_details_to_struct_anonymous_instance_id_map_get(&state->anonymous_instances_map, d);
	}else{
		struct anonymous_instance_id new_id;
		new_id.id = struct_anonymous_instance_details_list_size(&state->anonymous_instances);
		struct_anonymous_instance_details_to_struct_anonymous_instance_id_map_put(&state->anonymous_instances_map, d, new_id);
		struct_anonymous_instance_details_list_add_end(&state->anonymous_instances, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ANONYMOUS_INSTANCE);
		return new_id;
	}
}

void print_aggregate_type_specifier(struct asm_state * state, struct unsigned_char_list * out, struct aggregate_type_specifier_id id){
	struct aggregate_type_specifier_details details = struct_aggregate_type_specifier_details_list_get(&state->aggregate_type_specifiers, id.id);
	switch(details.type){
		case ASM_AGGREGATE_TYPE_SPECIFIER_SIMPLE_TYPE_SPECIFIER:{
			struct simple_type_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_simple_type_specifier(state, out, id_tmp);
			break;
		}case ASM_AGGREGATE_TYPE_SPECIFIER_TAG_SPECIFIER:{
			struct scoped_tag_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_scoped_tag_specifier(state, out, id_tmp);
			break;
		}case ASM_AGGREGATE_TYPE_SPECIFIER_TYPENAME_SPECIFIER:{
			struct scoped_typename_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_scoped_typename_specifier(state, out, id_tmp);
			break;
		}
	}
}

void print_simple_type_specifier(struct asm_state * state, struct unsigned_char_list * out, struct simple_type_specifier_id id){
	struct simple_type_specifier_details details = struct_simple_type_specifier_details_list_get(&state->simple_type_specifiers, id.id);
	switch(details.type){
		case ASM_SIMPLE_TYPE_SPECIFIER_VOID:{
			buffered_printf(out, "void");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_CHAR:{
			buffered_printf(out, "char");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_SHORT:{
			buffered_printf(out, "short");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_INT:{
			buffered_printf(out, "int");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_LONG:{
			buffered_printf(out, "long");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_FLOAT:{
			buffered_printf(out, "float");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_DOUBLE:{
			buffered_printf(out, "double");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_SIGNED:{
			buffered_printf(out, "signed");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_UNSIGNED:{
			buffered_printf(out, "unsigned");
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

void print_simple_type_qualifier(struct asm_state * state, struct unsigned_char_list * out, struct simple_type_qualifier_id id){
	struct simple_type_qualifier_details details = struct_simple_type_qualifier_details_list_get(&state->simple_type_qualifiers, id.id);
	switch(details.type){
		case ASM_SIMPLE_TYPE_QUALIFIER_CONST:{
			buffered_printf(out, "const");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_QUALIFIER_VOLATILE:{
			buffered_printf(out, "volatile");
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

void print_simple_storage_class_specifier(struct asm_state * state, struct unsigned_char_list * out, struct simple_storage_class_specifier_id id){
	struct simple_storage_class_specifier_details details = struct_simple_storage_class_specifier_details_list_get(&state->simple_storage_class_specifiers, id.id);
	switch(details.type){
		case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_TYPEDEF:{
			buffered_printf(out, "typedef");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_EXTERN:{
			buffered_printf(out, "extern");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_STATIC:{
			buffered_printf(out, "static");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_AUTO:{
			buffered_printf(out, "auto");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_REGISTER:{
			buffered_printf(out, "register");
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

void print_specifier_or_qualifier(struct asm_state * state, struct unsigned_char_list * out, struct specifier_or_qualifier_id id){
	struct specifier_or_qualifier_details details = struct_specifier_or_qualifier_details_list_get(&state->specifier_or_qualifiers, id.id);
	switch(details.type){
		case ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_STORAGE_CLASS_SPECIFIER:{
			struct simple_storage_class_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_simple_storage_class_specifier(state, out, id_tmp);
			break;
		}case ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_TYPE_QUALIFIER:{
			struct simple_type_qualifier_id id_tmp;
			id_tmp.id = details.id;
			print_simple_type_qualifier(state, out, id_tmp);
			break;
		}case ASM_SPECIFIER_OR_QUALIFIER_AGGREGATE_TYPE_SPECIFIER:{
			struct aggregate_type_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_aggregate_type_specifier(state, out, id_tmp);
			break;
		}
	}
}

void print_specifier_or_qualifier_list_item(struct asm_state * state, struct unsigned_char_list * out, struct specifier_or_qualifier_list_item_id id){
	struct specifier_or_qualifier_list_item_details details = struct_specifier_or_qualifier_list_item_details_list_get(&state->specifier_or_qualifier_list_items, id.id);
	switch(details.type){
		case ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER:{
			print_specifier_or_qualifier_list_item(state, out, details.prev_id);
			buffered_printf(out, " ");
			print_specifier_or_qualifier(state, out, details.item_id);
			break;
		}case ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START:{
			print_terminal_specifier_or_qualifier_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_specifier_or_qualifier_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_parameter_list(struct asm_state * state, struct unsigned_char_list * out, struct parameter_list_id id){
	struct parameter_list_details details = struct_parameter_list_details_list_get(&state->parameter_lists, id.id);
	switch(details.type){
		case ASM_PARAMETER_LIST_VARIADIC:{
			print_general_type_list_item(state, out, details.general_type_list_item_id);
			break;
		}case ASM_PARAMETER_LIST_NON_VARIADIC:{
			print_general_type_list_item(state, out, details.general_type_list_item_id);
			break;
		}
	}
}

void print_initializer(struct asm_state * state, struct unsigned_char_list * out, struct initializer_id id){
	struct initializer_details details = struct_initializer_details_list_get(&state->initializers, id.id);
	switch(details.type){
		case ASM_INITIALIZER_INITIALIZER_LIST:{
			struct initializer_list_item_id id_tmp;
			id_tmp.id = details.id;
			print_initializer_list_item(state, out, id_tmp);
			break;
		}case ASM_INITIALIZER_EXPRESSION:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			print_expression(state, out, id_tmp);
			break;
		}
	}
}

void print_declarator_part(struct asm_state * state, struct unsigned_char_list * out, struct declarator_part_id id){
	struct declarator_part_details details = struct_declarator_part_details_list_get(&state->declarator_parts, id.id);
	switch(details.type){
		case ASM_DECLARATOR_PART_FUNCTION:{
			struct function_part_id id_tmp;
			id_tmp.id = details.id;
			print_function_part(state, out, id_tmp);
			break;
		}case ASM_DECLARATOR_PART_POINTER:{
			struct pointer_part_id id_tmp;
			id_tmp.id = details.id;
			print_pointer_part(state, out, id_tmp);
			break;
		}case ASM_DECLARATOR_PART_ARRAY:{
			struct array_part_id id_tmp;
			id_tmp.id = details.id;
			print_array_part(state, out, id_tmp);
			break;
		}
	}
}

void print_any_statement_list_item(struct asm_state * state, struct unsigned_char_list * out, struct any_statement_list_item_id id){
	struct any_statement_list_item_details details = struct_any_statement_list_item_details_list_get(&state->any_statement_list_items, id.id);
	switch(details.type){
		case ASM_ANY_STATEMENT_LIST_ITEM_ANY_STATEMENT:{
			print_any_statement_list_item(state, out, details.prev_id);
			print_any_statement(state, out, details.item_id);
			break;
		}case ASM_ANY_STATEMENT_LIST_ITEM_LIST_START:{
			print_terminal_any_statement_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_any_statement_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_anonymous_tag_definition(struct asm_state * state, struct unsigned_char_list * out, struct anonymous_tag_definition_id id){
	struct anonymous_tag_definition_details details = struct_anonymous_tag_definition_details_list_get(&state->anonymous_tag_definitions, id.id);
	switch(details.type){
		case ASM_ANONYMOUS_TAG_DEFINITION_ANONYMOUS:{
			print_tag_definition(state, out, details.tag_definition_id);
			print_scope_guid(state, out, details.scope_guid_id);
			print_anonymous_instance(state, out, details.anonymous_instance_id);
			break;
		}
	}
}

void print_named_tag_predeclaration(struct asm_state * state, struct unsigned_char_list * out, struct named_tag_predeclaration_id id){
	struct named_tag_predeclaration_details details = struct_named_tag_predeclaration_details_list_get(&state->named_tag_predeclarations, id.id);
	switch(details.type){
		case ASM_NAMED_TAG_PREDECLARATION_NAMED:{
			print_scope_guid(state, out, details.scope_guid_id);
			print_identifier(state, out, details.identifier_id);
			break;
		}
	}
}

void print_named_tag_definition(struct asm_state * state, struct unsigned_char_list * out, struct named_tag_definition_id id){
	struct named_tag_definition_details details = struct_named_tag_definition_details_list_get(&state->named_tag_definitions, id.id);
	switch(details.type){
		case ASM_NAMED_TAG_DEFINITION_NAMED:{
			print_tag_definition(state, out, details.tag_definition_id);
			print_scope_guid(state, out, details.scope_guid_id);
			print_identifier(state, out, details.identifier_id);
			break;
		}
	}
}

void print_tag_definition(struct asm_state * state, struct unsigned_char_list * out, struct tag_definition_id id){
	struct tag_definition_details details = struct_tag_definition_details_list_get(&state->tag_definitions, id.id);
	switch(details.type){
		case ASM_TAG_DEFINITION_ENUM:{
			struct enum_member_list_item_id id_tmp;
			id_tmp.id = details.id;
			print_enum_member_list_item(state, out, id_tmp);
			break;
		}case ASM_TAG_DEFINITION_STRUCT:{
			struct struct_or_union_member_list_item_id id_tmp;
			id_tmp.id = details.id;
			print_struct_or_union_member_list_item(state, out, id_tmp);
			break;
		}case ASM_TAG_DEFINITION_UNION:{
			struct struct_or_union_member_list_item_id id_tmp;
			id_tmp.id = details.id;
			print_struct_or_union_member_list_item(state, out, id_tmp);
			break;
		}
	}
}

void print_struct_or_union_member_list_item(struct asm_state * state, struct unsigned_char_list * out, struct struct_or_union_member_list_item_id id){
	struct struct_or_union_member_list_item_details details = struct_struct_or_union_member_list_item_details_list_get(&state->struct_or_union_member_list_items, id.id);
	switch(details.type){
		case ASM_STRUCT_OR_UNION_MEMBER_LIST_ITEM_MEMBER:{
			print_struct_or_union_member_list_item(state, out, details.prev_id);
			print_struct_or_union_member(state, out, details.item_id);
			break;
		}case ASM_STRUCT_OR_UNION_MEMBER_LIST_ITEM_LIST_START:{
			print_terminal_struct_or_union_member_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_struct_or_union_member_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_enum_member_list_item(struct asm_state * state, struct unsigned_char_list * out, struct enum_member_list_item_id id){
	struct enum_member_list_item_details details = struct_enum_member_list_item_details_list_get(&state->enum_member_list_items, id.id);
	switch(details.type){
		case ASM_ENUM_MEMBER_LIST_ITEM_MEMBER:{
			print_enum_member_list_item(state, out, details.prev_id);
			print_enum_member(state, out, details.item_id);
			break;
		}case ASM_ENUM_MEMBER_LIST_ITEM_LIST_START:{
			print_terminal_enum_member_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_enum_member_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_enum_member(struct asm_state * state, struct unsigned_char_list * out, struct enum_member_id id){
	struct enum_member_details details = struct_enum_member_details_list_get(&state->enum_members, id.id);
	switch(details.type){
		case ASM_ENUM_MEMBER_MEMBER:{
			print_identifier(state, out, details.identifier_id);
			print_expression(state, out, details.expression_id);
			break;
		}
	}
}

void print_struct_or_union_member(struct asm_state * state, struct unsigned_char_list * out, struct struct_or_union_member_id id){
	struct struct_or_union_member_details details = struct_struct_or_union_member_details_list_get(&state->struct_or_union_members, id.id);
	switch(details.type){
		case ASM_STRUCT_OR_UNION_MEMBER_MEMBER:{
			print_general_type(state, out, details.general_type_id);
			print_layout_flag_list_item(state, out, details.layout_flag_list_item_id);
			print_identifier(state, out, details.identifier_id);
			break;
		}
	}
}

void print_layout_flag_list_item(struct asm_state * state, struct unsigned_char_list * out, struct layout_flag_list_item_id id){
	struct layout_flag_list_item_details details = struct_layout_flag_list_item_details_list_get(&state->layout_flag_list_items, id.id);
	switch(details.type){
		case ASM_LAYOUT_FLAG_LIST_ITEM_LAYOUT_FLAG:{
			print_layout_flag_list_item(state, out, details.prev_id);
			print_layout_flag(state, out, details.item_id);
			break;
		}case ASM_LAYOUT_FLAG_LIST_ITEM_LIST_START:{
			print_terminal_layout_flag_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_layout_flag_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_layout_flag(struct asm_state * state, struct unsigned_char_list * out, struct layout_flag_id id){
	struct layout_flag_details details = struct_layout_flag_details_list_get(&state->layout_flags, id.id);
	switch(details.type){
		case ASM_LAYOUT_FLAG_STRUCT:{
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_LAYOUT_FLAG_UNION:{
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

void print_initializer_list_item(struct asm_state * state, struct unsigned_char_list * out, struct initializer_list_item_id id){
	struct initializer_list_item_details details = struct_initializer_list_item_details_list_get(&state->initializer_list_items, id.id);
	switch(details.type){
		case ASM_INITIALIZER_LIST_ITEM_INITIALIZER:{
			print_initializer_list_item(state, out, details.prev_id);
			print_initializer(state, out, details.item_id);
			break;
		}case ASM_INITIALIZER_LIST_ITEM_LIST_START:{
			print_terminal_initializer_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_initializer_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_expression_list_item(struct asm_state * state, struct unsigned_char_list * out, struct expression_list_item_id id){
	struct expression_list_item_details details = struct_expression_list_item_details_list_get(&state->expression_list_items, id.id);
	switch(details.type){
		case ASM_EXPRESSION_LIST_ITEM_EXPRESSION:{
			print_expression_list_item(state, out, details.prev_id);
			print_expression(state, out, details.item_id);
			break;
		}case ASM_EXPRESSION_LIST_ITEM_LIST_START:{
			print_terminal_expression_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_expression_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_declarator_part_list_item(struct asm_state * state, struct unsigned_char_list * out, struct declarator_part_list_item_id id){
	struct declarator_part_list_item_details details = struct_declarator_part_list_item_details_list_get(&state->declarator_part_list_items, id.id);
	switch(details.type){
		case ASM_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART:{
			print_declarator_part_list_item(state, out, details.prev_id);
			buffered_printf(out, " ");
			print_declarator_part(state, out, details.item_id);
			break;
		}case ASM_DECLARATOR_PART_LIST_ITEM_LIST_START:{
			print_terminal_declarator_part_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_declarator_part_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_pointer_part(struct asm_state * state, struct unsigned_char_list * out, struct pointer_part_id id){
	struct pointer_part_details details = struct_pointer_part_details_list_get(&state->pointer_parts, id.id);
	switch(details.type){
		case ASM_POINTER_PART_POINTER:{
			buffered_printf(out, "*");
			print_specifier_or_qualifier_list_item(state, out, details.specifier_or_qualifier_list_item_id);
			break;
		}
	}
}

void print_function_part(struct asm_state * state, struct unsigned_char_list * out, struct function_part_id id){
	struct function_part_details details = struct_function_part_details_list_get(&state->function_parts, id.id);
	switch(details.type){
		case ASM_FUNCTION_PART_PROTOTYPE:{
			print_parameter_list(state, out, details.parameter_list_id);
			break;
		}case ASM_FUNCTION_PART_K_AND_R_C:{
			print_parameter_list(state, out, details.parameter_list_id);
			break;
		}
	}
}

void print_array_part(struct asm_state * state, struct unsigned_char_list * out, struct array_part_id id){
	struct array_part_details details = struct_array_part_details_list_get(&state->array_parts, id.id);
	switch(details.type){
		case ASM_ARRAY_PART_EXPRESSION:{
			print_expression(state, out, details.expression_id);
			break;
		}case ASM_ARRAY_PART_FLEXIBLE:{
			print_terminal_array_part_flexible_expression_id(state, out, details.expression_id);
			break;
		}
	}
}

void print_general_type(struct asm_state * state, struct unsigned_char_list * out, struct general_type_id id){
	struct general_type_details details = struct_general_type_details_list_get(&state->general_types, id.id);
	switch(details.type){
		case ASM_GENERAL_TYPE_GENERAL_TYPE:{
			print_specifier_or_qualifier_list_item(state, out, details.specifier_or_qualifier_list_item_id);
			print_declarator_part_list_item(state, out, details.declarator_part_list_item_id);
			print_bitfield(state, out, details.bitfield_id);
			break;
		}
	}
}

void print_typedef_entry(struct asm_state * state, struct unsigned_char_list * out, struct typedef_entry_id id){
	struct typedef_entry_details details = struct_typedef_entry_details_list_get(&state->typedef_entrys, id.id);
	switch(details.type){
		case ASM_TYPEDEF_ENTRY_TYPEDEF:{
			print_identifier(state, out, details.identifier_id);
			print_scope_guid(state, out, details.scope_guid_id);
			print_general_type(state, out, details.general_type_id);
			break;
		}
	}
}

void print_general_type_list_item(struct asm_state * state, struct unsigned_char_list * out, struct general_type_list_item_id id){
	struct general_type_list_item_details details = struct_general_type_list_item_details_list_get(&state->general_type_list_items, id.id);
	switch(details.type){
		case ASM_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE:{
			print_general_type_list_item(state, out, details.prev_id);
			print_general_type(state, out, details.item_id);
			break;
		}case ASM_GENERAL_TYPE_LIST_ITEM_LIST_START:{
			print_terminal_general_type_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_general_type_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_postfix_expression_part_list_item(struct asm_state * state, struct unsigned_char_list * out, struct postfix_expression_part_list_item_id id){
	struct postfix_expression_part_list_item_details details = struct_postfix_expression_part_list_item_details_list_get(&state->postfix_expression_part_list_items, id.id);
	switch(details.type){
		case ASM_POSTFIX_EXPRESSION_PART_LIST_ITEM_POSTFIX_EXPRESSION_PART:{
			print_postfix_expression_part_list_item(state, out, details.prev_id);
			print_postfix_expression_part(state, out, details.item_id);
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_LIST_ITEM_LIST_START:{
			print_terminal_postfix_expression_part_list_item_list_start_prev_id(state, out, details.prev_id);
			print_terminal_postfix_expression_part_list_item_list_start_item_id(state, out, details.item_id);
			break;
		}
	}
}

void print_postfix_expression_part(struct asm_state * state, struct unsigned_char_list * out, struct postfix_expression_part_id id){
	struct postfix_expression_part_details details = struct_postfix_expression_part_details_list_get(&state->postfix_expression_parts, id.id);
	switch(details.type){
		case ASM_POSTFIX_EXPRESSION_PART_ARRAY_DEREFERENCE:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			print_expression(state, out, id_tmp);
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_STRUCT_DOT_DEREFERENCE:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			print_identifier(state, out, id_tmp);
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_STRUCT_ARROW_DEREFERENCE:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			print_identifier(state, out, id_tmp);
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_FUNCTION_DEREFERENCE:{
			struct expression_list_item_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "(");
			print_expression_list_item(state, out, id_tmp);
			buffered_printf(out, ")");
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_INC:{
			buffered_printf(out, "++");
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_DEC:{
			buffered_printf(out, "--");
			break;
		}
	}
}

void print_unscoped_tag_specifier(struct asm_state * state, struct unsigned_char_list * out, struct unscoped_tag_specifier_id id){
	struct unscoped_tag_specifier_details details = struct_unscoped_tag_specifier_details_list_get(&state->unscoped_tag_specifiers, id.id);
	switch(details.type){
		case ASM_UNSCOPED_TAG_SPECIFIER_STRUCT_SPECIFIER:{
			struct unscoped_struct_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_unscoped_struct_specifier(state, out, id_tmp);
			break;
		}case ASM_UNSCOPED_TAG_SPECIFIER_UNION_SPECIFIER:{
			struct unscoped_union_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_unscoped_union_specifier(state, out, id_tmp);
			break;
		}case ASM_UNSCOPED_TAG_SPECIFIER_ENUM_SPECIFIER:{
			struct unscoped_enum_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_unscoped_enum_specifier(state, out, id_tmp);
			break;
		}
	}
}

void print_scoped_typename_specifier(struct asm_state * state, struct unsigned_char_list * out, struct scoped_typename_specifier_id id){
	struct scoped_typename_specifier_details details = struct_scoped_typename_specifier_details_list_get(&state->scoped_typename_specifiers, id.id);
	switch(details.type){
		case ASM_SCOPED_TYPENAME_SPECIFIER_SCOPED_TYPENAME:{
			print_scope_guid(state, out, details.scope_guid_id);
			print_identifier(state, out, details.identifier_id);
			break;
		}
	}
}

void print_scoped_tag_specifier(struct asm_state * state, struct unsigned_char_list * out, struct scoped_tag_specifier_id id){
	struct scoped_tag_specifier_details details = struct_scoped_tag_specifier_details_list_get(&state->scoped_tag_specifiers, id.id);
	switch(details.type){
		case ASM_SCOPED_TAG_SPECIFIER_SCOPED_TAG_SPECIFIER:{
			print_scope_guid(state, out, details.scope_guid_id);
			print_unscoped_tag_specifier(state, out, details.unscoped_tag_specifier_id);
			break;
		}
	}
}

void print_unscoped_union_specifier(struct asm_state * state, struct unsigned_char_list * out, struct unscoped_union_specifier_id id){
	struct unscoped_union_specifier_details details = struct_unscoped_union_specifier_details_list_get(&state->unscoped_union_specifiers, id.id);
	switch(details.type){
		case ASM_UNSCOPED_UNION_SPECIFIER_NAMED_UNION:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			print_identifier(state, out, id_tmp);
			break;
		}case ASM_UNSCOPED_UNION_SPECIFIER_ANONYMOUS_UNION:{
			struct anonymous_instance_id id_tmp;
			id_tmp.id = details.id;
			print_anonymous_instance(state, out, id_tmp);
			break;
		}
	}
}

void print_unscoped_struct_specifier(struct asm_state * state, struct unsigned_char_list * out, struct unscoped_struct_specifier_id id){
	struct unscoped_struct_specifier_details details = struct_unscoped_struct_specifier_details_list_get(&state->unscoped_struct_specifiers, id.id);
	switch(details.type){
		case ASM_UNSCOPED_STRUCT_SPECIFIER_NAMED_STRUCT:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			print_identifier(state, out, id_tmp);
			break;
		}case ASM_UNSCOPED_STRUCT_SPECIFIER_ANONYMOUS_STRUCT:{
			struct anonymous_instance_id id_tmp;
			id_tmp.id = details.id;
			print_anonymous_instance(state, out, id_tmp);
			break;
		}
	}
}

void print_unscoped_enum_specifier(struct asm_state * state, struct unsigned_char_list * out, struct unscoped_enum_specifier_id id){
	struct unscoped_enum_specifier_details details = struct_unscoped_enum_specifier_details_list_get(&state->unscoped_enum_specifiers, id.id);
	switch(details.type){
		case ASM_UNSCOPED_ENUM_SPECIFIER_NAMED_ENUM:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			print_identifier(state, out, id_tmp);
			break;
		}case ASM_UNSCOPED_ENUM_SPECIFIER_ANONYMOUS_ENUM:{
			struct anonymous_instance_id id_tmp;
			id_tmp.id = details.id;
			print_anonymous_instance(state, out, id_tmp);
			break;
		}
	}
}

void print_bitfield(struct asm_state * state, struct unsigned_char_list * out, struct bitfield_id id){
	struct bitfield_details details = struct_bitfield_details_list_get(&state->bitfields, id.id);
	switch(details.type){
		case ASM_BITFIELD_NO_BITFIELD:{
			print_terminal_bitfield_no_bitfield_expression_id(state, out, details.expression_id);
			break;
		}case ASM_BITFIELD_CONSTANT_BITFIELD:{
			print_expression(state, out, details.expression_id);
			break;
		}
	}
}

void print_jump_statement(struct asm_state * state, struct unsigned_char_list * out, struct jump_statement_id id){
	struct jump_statement_details details = struct_jump_statement_details_list_get(&state->jump_statements, id.id);
	switch(details.type){
		case ASM_JUMP_STATEMENT_RETURN_EXPRESSION:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			print_expression(state, out, id_tmp);
			break;
		}case ASM_JUMP_STATEMENT_RETURN_NO_EXPRESSION:{
			print_terminal_jump_statement_return_no_expression_id(state, out, details.id);
			break;
		}case ASM_JUMP_STATEMENT_BREAK:{
			print_terminal_jump_statement_break_id(state, out, details.id);
			break;
		}case ASM_JUMP_STATEMENT_CONTINUE:{
			print_terminal_jump_statement_continue_id(state, out, details.id);
			break;
		}case ASM_JUMP_STATEMENT_GOTO:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			print_identifier(state, out, id_tmp);
			break;
		}
	}
}

void print_switch_statement(struct asm_state * state, struct unsigned_char_list * out, struct switch_statement_id id){
	struct switch_statement_details details = struct_switch_statement_details_list_get(&state->switch_statements, id.id);
	switch(details.type){
		case ASM_SWITCH_STATEMENT_SWITCH:{
			print_expression(state, out, details.e1);
			print_any_statement(state, out, details.s1);
			break;
		}
	}
}

void print_if_statement(struct asm_state * state, struct unsigned_char_list * out, struct if_statement_id id){
	struct if_statement_details details = struct_if_statement_details_list_get(&state->if_statements, id.id);
	switch(details.type){
		case ASM_IF_STATEMENT_JUST_IF:{
			print_any_statement(state, out, details.s1);
			print_expression(state, out, details.e1);
			print_terminal_if_statement_just_if_s2(state, out, details.s2);
			break;
		}case ASM_IF_STATEMENT_IF_ELSE:{
			print_any_statement(state, out, details.s1);
			print_expression(state, out, details.e1);
			print_any_statement(state, out, details.s2);
			break;
		}
	}
}

void print_while_statement(struct asm_state * state, struct unsigned_char_list * out, struct while_statement_id id){
	struct while_statement_details details = struct_while_statement_details_list_get(&state->while_statements, id.id);
	switch(details.type){
		case ASM_WHILE_STATEMENT_DO_WHILE:{
			print_expression(state, out, details.e1);
			print_any_statement(state, out, details.s1);
			break;
		}case ASM_WHILE_STATEMENT_WHILE:{
			print_expression(state, out, details.e1);
			print_any_statement(state, out, details.s1);
			break;
		}
	}
}

void print_for_statement(struct asm_state * state, struct unsigned_char_list * out, struct for_statement_id id){
	struct for_statement_details details = struct_for_statement_details_list_get(&state->for_statements, id.id);
	switch(details.type){
		case ASM_FOR_STATEMENT_FOR:{
			print_expression_list_item(state, out, details.e1);
			print_expression_list_item(state, out, details.e2);
			print_expression_list_item(state, out, details.e3);
			break;
		}
	}
}

void print_code_statement(struct asm_state * state, struct unsigned_char_list * out, struct code_statement_id id){
	struct code_statement_details details = struct_code_statement_details_list_get(&state->code_statements, id.id);
	switch(details.type){
		case ASM_CODE_STATEMENT_LABELED_STATEMENT:{
			struct labeled_statement_id id_tmp;
			id_tmp.id = details.id;
			print_labeled_statement(state, out, id_tmp);
			break;
		}case ASM_CODE_STATEMENT_COMPOUND_STATEMENT:{
			struct any_statement_list_item_id id_tmp;
			id_tmp.id = details.id;
			print_any_statement_list_item(state, out, id_tmp);
			break;
		}case ASM_CODE_STATEMENT_EXPRESSION_STATEMENT:{
			struct expression_list_item_id id_tmp;
			id_tmp.id = details.id;
			print_expression_list_item(state, out, id_tmp);
			break;
		}case ASM_CODE_STATEMENT_FOR_STATEMENT:{
			struct for_statement_id id_tmp;
			id_tmp.id = details.id;
			print_for_statement(state, out, id_tmp);
			break;
		}case ASM_CODE_STATEMENT_WHILE_STATEMENT:{
			struct while_statement_id id_tmp;
			id_tmp.id = details.id;
			print_while_statement(state, out, id_tmp);
			break;
		}case ASM_CODE_STATEMENT_IF_STATEMENT:{
			struct if_statement_id id_tmp;
			id_tmp.id = details.id;
			print_if_statement(state, out, id_tmp);
			break;
		}case ASM_CODE_STATEMENT_SWITCH_STATEMENT:{
			struct switch_statement_id id_tmp;
			id_tmp.id = details.id;
			print_switch_statement(state, out, id_tmp);
			break;
		}case ASM_CODE_STATEMENT_JUMP_STATEMENT:{
			struct jump_statement_id id_tmp;
			id_tmp.id = details.id;
			print_jump_statement(state, out, id_tmp);
			break;
		}
	}
}

void print_labeled_statement(struct asm_state * state, struct unsigned_char_list * out, struct labeled_statement_id id){
	struct labeled_statement_details details = struct_labeled_statement_details_list_get(&state->labeled_statements, id.id);
	switch(details.type){
		case ASM_LABELED_STATEMENT_GOTO_LABEL:{
			print_any_statement(state, out, details.any_statement_id);
			print_terminal_labeled_statement_goto_label_expression_id(state, out, details.expression_id);
			break;
		}case ASM_LABELED_STATEMENT_CASE_LABEL:{
			print_any_statement(state, out, details.any_statement_id);
			print_expression(state, out, details.expression_id);
			break;
		}case ASM_LABELED_STATEMENT_DEFAULT_LABEL:{
			print_any_statement(state, out, details.any_statement_id);
			print_terminal_labeled_statement_default_label_expression_id(state, out, details.expression_id);
			break;
		}
	}
}

void print_postfix_expression(struct asm_state * state, struct unsigned_char_list * out, struct postfix_expression_id id){
	struct postfix_expression_details details = struct_postfix_expression_details_list_get(&state->postfix_expressions, id.id);
	switch(details.type){
		case ASM_POSTFIX_EXPRESSION_POSTFIX_EXPRESSION:{
			print_primary_expression(state, out, details.primary_expression_id);
			print_postfix_expression_part_list_item(state, out, details.postfix_expression_part_list_item_id);
			break;
		}
	}
}

void print_primary_expression(struct asm_state * state, struct unsigned_char_list * out, struct primary_expression_id id){
	struct primary_expression_details details = struct_primary_expression_details_list_get(&state->primary_expressions, id.id);
	switch(details.type){
		case ASM_PRIMARY_EXPRESSION_CONSTANT_DESCRIPTION:{
			struct constant_description_id id_tmp;
			id_tmp.id = details.id;
			print_constant_description(state, out, id_tmp);
			break;
		}case ASM_PRIMARY_EXPRESSION_IDENTIFIER:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			print_identifier(state, out, id_tmp);
			break;
		}case ASM_PRIMARY_EXPRESSION_EXPRESSION:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			print_expression(state, out, id_tmp);
			break;
		}
	}
}

void print_any_statement(struct asm_state * state, struct unsigned_char_list * out, struct any_statement_id id){
	struct any_statement_details details = struct_any_statement_details_list_get(&state->any_statements, id.id);
	switch(details.type){
		case ASM_ANY_STATEMENT_DECLARATION_STATEMENT:{
			struct declaration_statement_id id_tmp;
			id_tmp.id = details.id;
			print_declaration_statement(state, out, id_tmp);
			break;
		}case ASM_ANY_STATEMENT_SCOPED_STATEMENT:{
			struct scoped_statement_id id_tmp;
			id_tmp.id = details.id;
			print_scoped_statement(state, out, id_tmp);
			break;
		}case ASM_ANY_STATEMENT_CODE_STATEMENT:{
			struct code_statement_id id_tmp;
			id_tmp.id = details.id;
			print_code_statement(state, out, id_tmp);
			break;
		}
	}
}

void print_scoped_statement(struct asm_state * state, struct unsigned_char_list * out, struct scoped_statement_id id){
	struct scoped_statement_details details = struct_scoped_statement_details_list_get(&state->scoped_statements, id.id);
	switch(details.type){
		case ASM_SCOPED_STATEMENT_FUNCTION_SCOPE:{
			print_any_statement_list_item(state, out, details.any_statement_list_item_id);
			print_scope_guid(state, out, details.scope_guid_id);
			break;
		}case ASM_SCOPED_STATEMENT_BLOCK_SCOPE:{
			print_any_statement_list_item(state, out, details.any_statement_list_item_id);
			print_scope_guid(state, out, details.scope_guid_id);
			break;
		}case ASM_SCOPED_STATEMENT_FILE_SCOPE:{
			print_any_statement_list_item(state, out, details.any_statement_list_item_id);
			print_scope_guid(state, out, details.scope_guid_id);
			break;
		}
	}
}

void print_declaration_statement(struct asm_state * state, struct unsigned_char_list * out, struct declaration_statement_id id){
	struct declaration_statement_details details = struct_declaration_statement_details_list_get(&state->declaration_statements, id.id);
	switch(details.type){
		case ASM_DECLARATION_STATEMENT_SIMPLE_DECLARATION:{
			struct simple_declaration_id id_tmp;
			id_tmp.id = details.id;
			print_simple_declaration(state, out, id_tmp);
			break;
		}case ASM_DECLARATION_STATEMENT_FUNCTION_DEFINITION:{
			struct function_definition_id id_tmp;
			id_tmp.id = details.id;
			print_function_definition(state, out, id_tmp);
			break;
		}
	}
}

void print_function_definition(struct asm_state * state, struct unsigned_char_list * out, struct function_definition_id id){
	struct function_definition_details details = struct_function_definition_details_list_get(&state->function_definitions, id.id);
	switch(details.type){
		case ASM_FUNCTION_DEFINITION_PROTOTYPED:{
			print_general_type(state, out, details.general_type_id);
			print_any_statement_list_item(state, out, details.params);
			print_any_statement_list_item(state, out, details.body);
			print_identifier(state, out, details.identifier_id);
			break;
		}case ASM_FUNCTION_DEFINITION_K_AND_R_C:{
			print_general_type(state, out, details.general_type_id);
			print_any_statement_list_item(state, out, details.params);
			print_any_statement_list_item(state, out, details.body);
			print_identifier(state, out, details.identifier_id);
			break;
		}
	}
}

void print_simple_declaration(struct asm_state * state, struct unsigned_char_list * out, struct simple_declaration_id id){
	struct simple_declaration_details details = struct_simple_declaration_details_list_get(&state->simple_declarations, id.id);
	switch(details.type){
		case ASM_SIMPLE_DECLARATION_SIMPLE_DECLARATION:{
			print_general_type(state, out, details.general_type_id);
			print_identifier(state, out, details.identifier_id);
			print_initializer(state, out, details.initializer_id);
			break;
		}
	}
}

void print_expression(struct asm_state * state, struct unsigned_char_list * out, struct expression_id id){
	struct expression_details details = struct_expression_details_list_get(&state->expressions, id.id);
	switch(details.type){
		case ASM_EXPRESSION_CAST_EXPRESSION:{
			struct cast_expression_id id_tmp;
			id_tmp.id = details.id;
			print_cast_expression(state, out, id_tmp);
			break;
		}case ASM_EXPRESSION_BINARY_EXPRESSION:{
			struct binary_expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "(");
			print_binary_expression(state, out, id_tmp);
			buffered_printf(out, ")");
			break;
		}case ASM_EXPRESSION_TERNARY_EXPRESSION:{
			struct ternary_expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "(");
			print_ternary_expression(state, out, id_tmp);
			buffered_printf(out, ")");
			break;
		}
	}
}

void print_cast_expression(struct asm_state * state, struct unsigned_char_list * out, struct cast_expression_id id){
	struct cast_expression_details details = struct_cast_expression_details_list_get(&state->cast_expressions, id.id);
	switch(details.type){
		case ASM_CAST_EXPRESSION_HAS_CAST:{
			struct cast_expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_cast_expression(state, out, id1_tmp);
			print_general_type(state, out, details.id2);
			break;
		}case ASM_CAST_EXPRESSION_NO_CAST:{
			struct unary_expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_unary_expression(state, out, id1_tmp);
			print_terminal_cast_expression_no_cast_id2(state, out, details.id2);
			break;
		}
	}
}

void print_unary_expression(struct asm_state * state, struct unsigned_char_list * out, struct unary_expression_id id){
	struct unary_expression_details details = struct_unary_expression_details_list_get(&state->unary_expressions, id.id);
	switch(details.type){
		case ASM_UNARY_EXPRESSION_SIZEOF_EXPRESSION:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_POSTFIX_EXPRESSION:{
			struct postfix_expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_postfix_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_SIZEOF_TYPE:{
			struct general_type_id id1_tmp;
			id1_tmp.id = details.id1;
			print_general_type(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_INC:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_DEC:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_ADDRESS:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_POINT_TO:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_POSITIVE:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_NEGATIVE:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_BITWISE_NOT:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}case ASM_UNARY_EXPRESSION_LOGICAL_NOT:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			print_expression(state, out, id1_tmp);
			break;
		}
	}
}

void print_binary_expression(struct asm_state * state, struct unsigned_char_list * out, struct binary_expression_id id){
	struct binary_expression_details details = struct_binary_expression_details_list_get(&state->binary_expressions, id.id);
	switch(details.type){
		case ASM_BINARY_EXPRESSION_PLUS:{
			print_expression(state, out, details.e1);
			buffered_printf(out, " + ");
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_MINUS:{
			print_expression(state, out, details.e1);
			buffered_printf(out, " - ");
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_DIVIDE:{
			print_expression(state, out, details.e1);
			buffered_printf(out, " / ");
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_MULTIPLY:{
			print_expression(state, out, details.e1);
			buffered_printf(out, " * ");
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_EXCLUSIVE_OR:{
			print_expression(state, out, details.e1);
			buffered_printf(out, " ^ ");
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_MODULUS:{
			print_expression(state, out, details.e1);
			buffered_printf(out, " % ");
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_BITWISE_OR:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_LOGICAL_OR:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_BITWISE_AND:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_LOGICAL_AND:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_LT:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_GT:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_LE:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_GE:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_EQ:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_ASSIGN:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_ADD_ASSIGN:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_SUB_ASSIGN:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_DIV_ASSIGN:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_MUL_ASSIGN:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_AND_ASSIGN:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_OR_ASSIGN:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}case ASM_BINARY_EXPRESSION_MOD_ASSIGN:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			break;
		}
	}
}

void print_ternary_expression(struct asm_state * state, struct unsigned_char_list * out, struct ternary_expression_id id){
	struct ternary_expression_details details = struct_ternary_expression_details_list_get(&state->ternary_expressions, id.id);
	switch(details.type){
		case ASM_TERNARY_EXPRESSION_TERNARY_OPERATOR:{
			print_expression(state, out, details.e1);
			print_expression(state, out, details.e2);
			print_expression(state, out, details.e3);
			break;
		}
	}
}

void print_constant_description(struct asm_state * state, struct unsigned_char_list * out, struct constant_description_id id){
	struct constant_description_details details = struct_constant_description_details_list_get(&state->constant_descriptions, id.id);
	switch(details.type){
		case ASM_CONSTANT_DESCRIPTION_CONSTANT_DESCRIPTION:{
			buffered_printf(out, "[ type='");
			print_general_type(state, out, details.general_type_id);
			buffered_printf(out, "', value=");
			print_data_package(state, out, details.data_package_id);
			buffered_printf(out, " ]");
			break;
		}
	}
}

void print_data_package(struct asm_state * state, struct unsigned_char_list * out, struct data_package_id id){
	struct data_package_details details = struct_data_package_details_list_get(&state->data_packages, id.id);
	switch(details.type){
		case ASM_DATA_PACKAGE_ERROR:{
			struct error_node_id id_tmp;
			id_tmp.id = details.id;
			print_error_node(state, out, id_tmp);
			break;
		}case ASM_DATA_PACKAGE_BYTE:{
			struct byte_package_id id_tmp;
			id_tmp.id = details.id;
			print_byte_package(state, out, id_tmp);
			break;
		}case ASM_DATA_PACKAGE_HWORD:{
			struct hword_package_id id_tmp;
			id_tmp.id = details.id;
			print_hword_package(state, out, id_tmp);
			break;
		}case ASM_DATA_PACKAGE_WORD:{
			struct word_package_id id_tmp;
			id_tmp.id = details.id;
			print_word_package(state, out, id_tmp);
			break;
		}case ASM_DATA_PACKAGE_DWORD:{
			struct dword_package_id id_tmp;
			id_tmp.id = details.id;
			print_dword_package(state, out, id_tmp);
			break;
		}case ASM_DATA_PACKAGE_QWORD:{
			struct qword_package_id id_tmp;
			id_tmp.id = details.id;
			print_qword_package(state, out, id_tmp);
			break;
		}case ASM_DATA_PACKAGE_STRING:{
			struct string_package_id id_tmp;
			id_tmp.id = details.id;
			print_string_package(state, out, id_tmp);
			break;
		}
	}
}

void print_error_node(struct asm_state * state, struct unsigned_char_list * out, struct error_node_id id){
	struct error_node_details details = struct_error_node_details_list_get(&state->error_nodes, id.id);
	switch(details.type){
		case ASM_ERROR_NODE_ERROR:{
			buffered_printf(out, "This node represents an error.");
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

void print_byte_package(struct asm_state * state, struct unsigned_char_list * out, struct byte_package_id id){
	struct byte_package_details details = struct_byte_package_details_list_get(&state->byte_packages, id.id);
	switch(details.type){
		case ASM_BYTE_PACKAGE_BYTE:{
			print_terminal_byte_package_byte_data(state, out, details.data);
			break;
		}
	}
}

void print_hword_package(struct asm_state * state, struct unsigned_char_list * out, struct hword_package_id id){
	struct hword_package_details details = struct_hword_package_details_list_get(&state->hword_packages, id.id);
	switch(details.type){
		case ASM_HWORD_PACKAGE_HWORD:{
			print_terminal_hword_package_hword_data0(state, out, details.data0);
			print_terminal_hword_package_hword_data1(state, out, details.data1);
			break;
		}
	}
}

void print_word_package(struct asm_state * state, struct unsigned_char_list * out, struct word_package_id id){
	struct word_package_details details = struct_word_package_details_list_get(&state->word_packages, id.id);
	switch(details.type){
		case ASM_WORD_PACKAGE_WORD:{
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data);
			break;
		}
	}
}

void print_qword_package(struct asm_state * state, struct unsigned_char_list * out, struct qword_package_id id){
	struct qword_package_details details = struct_qword_package_details_list_get(&state->qword_packages, id.id);
	switch(details.type){
		case ASM_QWORD_PACKAGE_QWORD:{
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data0);
			buffered_printf(out, "%08X", details.data1);
			buffered_printf(out, "%08X", details.data2);
			buffered_printf(out, "%08X", details.data3);
			break;
		}
	}
}

void print_dword_package(struct asm_state * state, struct unsigned_char_list * out, struct dword_package_id id){
	struct dword_package_details details = struct_dword_package_details_list_get(&state->dword_packages, id.id);
	switch(details.type){
		case ASM_DWORD_PACKAGE_DWORD:{
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data0);
			buffered_printf(out, "%08X", details.data1);
			break;
		}
	}
}

void print_string_package(struct asm_state * state, struct unsigned_char_list * out, struct string_package_id id){
	struct string_package_details details = struct_string_package_details_list_get(&state->string_packages, id.id);
	switch(details.type){
		case ASM_STRING_PACKAGE_STRING:{
			print_terminal_string_package_string_length(state, out, details.length);
			print_terminal_string_package_string_data(state, out, details.data);
			break;
		}
	}
}

void print_identifier(struct asm_state * state, struct unsigned_char_list * out, struct identifier_id id){
	struct identifier_details details = struct_identifier_details_list_get(&state->identifiers, id.id);
	switch(details.type){
		case ASM_IDENTIFIER_IDENTIFIER:{
			add_string_to_buffer(out, details.identifier->first_byte, details.identifier->last_byte);
			break;
		}
	}
}

void print_scope_guid(struct asm_state * state, struct unsigned_char_list * out, struct scope_guid_id id){
	struct scope_guid_details details = struct_scope_guid_details_list_get(&state->scope_guids, id.id);
	switch(details.type){
		case ASM_SCOPE_GUID_BLOCK_SCOPE:{
			print_terminal_scope_guid_block_scope_id(state, out, details.id);
			break;
		}case ASM_SCOPE_GUID_FILE_SCOPE:{
			print_terminal_scope_guid_file_scope_id(state, out, details.id);
			break;
		}case ASM_SCOPE_GUID_FUNCTION_SCOPE:{
			print_terminal_scope_guid_function_scope_id(state, out, details.id);
			break;
		}
	}
}

void print_anonymous_instance(struct asm_state * state, struct unsigned_char_list * out, struct anonymous_instance_id id){
	struct anonymous_instance_details details = struct_anonymous_instance_details_list_get(&state->anonymous_instances, id.id);
	switch(details.type){
		case ASM_ANONYMOUS_INSTANCE_ANONYMOUS_ENTRY:{
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

