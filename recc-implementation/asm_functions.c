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

#include "asm_functions.h"

struct aggregate_type_specifier_id make_aggregate_type_specifier(struct asm_state *,enum asm_aggregate_type_specifier_kind,unsigned int);
struct simple_type_specifier_id make_simple_type_specifier(struct asm_state *,enum asm_simple_type_specifier_kind);
struct simple_type_qualifier_id make_simple_type_qualifier(struct asm_state *,enum asm_simple_type_qualifier_kind);
struct simple_storage_class_specifier_id make_simple_storage_class_specifier(struct asm_state *,enum asm_simple_storage_class_specifier_kind);
struct specifier_or_qualifier_id make_specifier_or_qualifier(struct asm_state *,enum asm_specifier_or_qualifier_kind,unsigned int);
struct specifier_or_qualifier_list_item_id make_specifier_or_qualifier_list_item(struct asm_state *,enum asm_specifier_or_qualifier_list_item_kind,struct specifier_or_qualifier_list_item_id,struct specifier_or_qualifier_id);
struct parameter_list_id make_parameter_list(struct asm_state *,enum asm_parameter_list_kind,struct general_type_list_item_id);
struct declarator_part_id make_declarator_part(struct asm_state *,enum asm_declarator_part_kind,unsigned int);
struct any_statement_list_item_id make_any_statement_list_item(struct asm_state *,enum asm_any_statement_list_item_kind,struct any_statement_list_item_id,struct any_statement_id);
struct anonymous_tag_definition_id make_anonymous_tag_definition(struct asm_state *,enum asm_anonymous_tag_definition_kind,struct tag_definition_id,struct scope_guid_id,struct anonymous_entry_id);
struct named_tag_predeclaration_id make_named_tag_predeclaration(struct asm_state *,enum asm_named_tag_predeclaration_kind,struct scope_guid_id,struct identifier_id,enum asm_unscoped_tag_specifier_kind,unsigned int);
struct named_tag_definition_id make_named_tag_definition(struct asm_state *,enum asm_named_tag_definition_kind,struct tag_definition_id,struct scope_guid_id,struct identifier_id,unsigned int);
struct tag_definition_id make_tag_definition(struct asm_state *,enum asm_tag_definition_kind,unsigned int);
struct struct_or_union_member_list_item_id make_struct_or_union_member_list_item(struct asm_state *,enum asm_struct_or_union_member_list_item_kind,struct struct_or_union_member_list_item_id,struct struct_or_union_member_id);
struct enum_member_list_item_id make_enum_member_list_item(struct asm_state *,enum asm_enum_member_list_item_kind,struct enum_member_list_item_id,struct enum_member_id);
struct enum_member_id make_enum_member(struct asm_state *,enum asm_enum_member_kind,struct identifier_id,struct expression_id);
struct struct_or_union_member_id make_struct_or_union_member(struct asm_state *,enum asm_struct_or_union_member_kind,struct general_type_id,struct identifier_id);
struct initializer_id make_initializer(struct asm_state *,enum asm_initializer_kind,unsigned int);
struct initializer_list_item_id make_initializer_list_item(struct asm_state *,enum asm_initializer_list_item_kind,struct initializer_list_item_id,struct initializer_id);
struct declarator_part_list_item_id make_declarator_part_list_item(struct asm_state *,enum asm_declarator_part_list_item_kind,struct declarator_part_list_item_id,struct declarator_part_id);
struct pointer_part_id make_pointer_part(struct asm_state *,enum asm_pointer_part_kind,struct specifier_or_qualifier_list_item_id);
struct function_part_id make_function_part(struct asm_state *,enum asm_function_part_kind,struct parameter_list_id);
struct array_part_id make_array_part(struct asm_state *,enum asm_array_part_kind,struct expression_id);
struct general_type_id make_general_type(struct asm_state *,enum asm_general_type_kind,struct specifier_or_qualifier_list_item_id,struct declarator_part_list_item_id,struct bitfield_id);
struct identifier_namespace_entry_id make_identifier_namespace_entry(struct asm_state *,enum asm_identifier_namespace_entry_kind,struct identifier_id,struct scope_guid_id,struct general_type_id,unsigned int);
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
struct switch_statement_id make_switch_statement(struct asm_state *,enum asm_switch_statement_kind,struct expression_id,struct scoped_statement_id);
struct if_statement_id make_if_statement(struct asm_state *,enum asm_if_statement_kind,struct scoped_statement_id,struct expression_id,struct scoped_statement_id);
struct while_statement_id make_while_statement(struct asm_state *,enum asm_while_statement_kind,struct expression_id,struct scoped_statement_id);
struct for_statement_id make_for_statement(struct asm_state *,enum asm_for_statement_kind,struct any_statement_id,struct any_statement_id,struct any_statement_id,struct scoped_statement_id);
struct code_statement_id make_code_statement(struct asm_state *,enum asm_code_statement_kind,unsigned int);
struct labeled_statement_id make_labeled_statement(struct asm_state *,enum asm_labeled_statement_kind,struct scoped_statement_id,unsigned int);
struct postfix_expression_id make_postfix_expression(struct asm_state *,enum asm_postfix_expression_kind,struct primary_expression_id,struct postfix_expression_part_list_item_id);
struct primary_expression_id make_primary_expression(struct asm_state *,enum asm_primary_expression_kind,unsigned int,struct scope_guid_id);
struct any_statement_id make_any_statement(struct asm_state *,enum asm_any_statement_kind,unsigned int);
struct scoped_statement_id make_scoped_statement(struct asm_state *,enum asm_scoped_statement_kind,struct any_statement_list_item_id,struct scope_guid_id);
struct declaration_statement_id make_declaration_statement(struct asm_state *,enum asm_declaration_statement_kind,unsigned int);
struct function_definition_id make_function_definition(struct asm_state *,enum asm_function_definition_kind,struct general_type_id,struct any_statement_list_item_id,struct any_statement_list_item_id,struct scoped_statement_id,struct identifier_id);
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
struct string_package_id make_string_package(struct asm_state *,enum asm_string_package_kind,unsigned int,unsigned int,unsigned int *);
struct identifier_id make_identifier(struct asm_state *,enum asm_identifier_kind,struct c_lexer_token *);
struct scope_guid_id make_scope_guid(struct asm_state *,enum asm_scope_guid_kind,struct scope_guid_id,unsigned int);
struct anonymous_entry_id make_anonymous_entry(struct asm_state *,enum asm_anonymous_entry_kind,unsigned int);
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
		}case ASM_OP_ADD_INITIALIZER:{
			struct initializer_details d = struct_initializer_details_list_pop_end(&state->initializers);
			struct_initializer_details_to_struct_initializer_id_map_remove(&state->initializers_map, d);
			return 0;
		}case ASM_OP_ADD_INITIALIZER_LIST_ITEM:{
			struct initializer_list_item_details d = struct_initializer_list_item_details_list_pop_end(&state->initializer_list_items);
			struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_remove(&state->initializer_list_items_map, d);
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
		}case ASM_OP_ADD_IDENTIFIER_NAMESPACE_ENTRY:{
			struct identifier_namespace_entry_details d = struct_identifier_namespace_entry_details_list_pop_end(&state->identifier_namespace_entrys);
			struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map_remove(&state->identifier_namespace_entrys_map, d);
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
			return 0;
		}case ASM_OP_ADD_IDENTIFIER:{
			struct identifier_details d = struct_identifier_details_list_pop_end(&state->identifiers);
			struct_identifier_details_to_struct_identifier_id_map_remove(&state->identifiers_map, d);
			return 0;
		}case ASM_OP_ADD_SCOPE_GUID:{
			struct scope_guid_details d = struct_scope_guid_details_list_pop_end(&state->scope_guids);
			struct_scope_guid_details_to_struct_scope_guid_id_map_remove(&state->scope_guids_map, d);
			state->next_scope_guid--;
			return 0;
		}case ASM_OP_ADD_ANONYMOUS_ENTRY:{
			struct anonymous_entry_details d = struct_anonymous_entry_details_list_pop_end(&state->anonymous_entrys);
			struct_anonymous_entry_details_to_struct_anonymous_entry_id_map_remove(&state->anonymous_entrys_map, d);
			state->next_anonymous_entry--;
			return 0;
		}default:{
			return 1;
		}
	}

}

void create_asm_state(struct asm_state * state, struct memory_pool_collection * m){
	(void)m;
	state->next_scope_guid = 0;
	state->next_anonymous_entry = 0;
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
	struct_initializer_details_list_create(&state->initializers);
	struct_initializer_details_to_struct_initializer_id_map_create(&state->initializers_map, struct_struct_initializer_details_to_struct_initializer_id_key_value_pair_compare);
	struct_initializer_list_item_details_list_create(&state->initializer_list_items);
	struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_create(&state->initializer_list_items_map, struct_struct_initializer_list_item_details_to_struct_initializer_list_item_id_key_value_pair_compare);
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
	struct_identifier_namespace_entry_details_list_create(&state->identifier_namespace_entrys);
	struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map_create(&state->identifier_namespace_entrys_map, struct_struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_key_value_pair_compare);
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
	struct_anonymous_entry_details_list_create(&state->anonymous_entrys);
	struct_anonymous_entry_details_to_struct_anonymous_entry_id_map_create(&state->anonymous_entrys_map, struct_struct_anonymous_entry_details_to_struct_anonymous_entry_id_key_value_pair_compare);
}

void destroy_asm_state(struct asm_state * state){
	enum_asm_operation_type_list_destroy(&state->reversible_operations);
	free_string_package_pointers(state);
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
	struct_initializer_details_list_destroy(&state->initializers);
	struct_initializer_details_to_struct_initializer_id_map_destroy(&state->initializers_map);
	struct_initializer_list_item_details_list_destroy(&state->initializer_list_items);
	struct_initializer_list_item_details_to_struct_initializer_list_item_id_map_destroy(&state->initializer_list_items_map);
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
	struct_identifier_namespace_entry_details_list_destroy(&state->identifier_namespace_entrys);
	struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map_destroy(&state->identifier_namespace_entrys_map);
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
	struct_anonymous_entry_details_list_destroy(&state->anonymous_entrys);
	struct_anonymous_entry_details_to_struct_anonymous_entry_id_map_destroy(&state->anonymous_entrys_map);
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

struct anonymous_tag_definition_id make_anonymous_tag_definition_anonymous(struct asm_state * state, struct tag_definition_id tag_definition_id, struct scope_guid_id scope_guid_id, struct anonymous_entry_id anonymous_entry_id){
	return make_anonymous_tag_definition(state, ASM_ANONYMOUS_TAG_DEFINITION_ANONYMOUS, tag_definition_id, scope_guid_id, anonymous_entry_id);
}
struct anonymous_tag_definition_id make_anonymous_tag_definition(struct asm_state * state, enum asm_anonymous_tag_definition_kind kind, struct tag_definition_id tag_definition_id, struct scope_guid_id scope_guid_id, struct anonymous_entry_id anonymous_entry_id){
	struct anonymous_tag_definition_details d;
	d.type = kind;
	d.tag_definition_id = tag_definition_id;
	d.scope_guid_id = scope_guid_id;
	d.anonymous_entry_id = anonymous_entry_id;
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

struct named_tag_predeclaration_id make_named_tag_predeclaration_named(struct asm_state * state, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id, enum asm_unscoped_tag_specifier_kind tag_type, unsigned int order_id){
	return make_named_tag_predeclaration(state, ASM_NAMED_TAG_PREDECLARATION_NAMED, scope_guid_id, identifier_id, tag_type, order_id);
}
struct named_tag_predeclaration_id make_named_tag_predeclaration(struct asm_state * state, enum asm_named_tag_predeclaration_kind kind, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id, enum asm_unscoped_tag_specifier_kind tag_type, unsigned int order_id){
	struct named_tag_predeclaration_details d;
	d.type = kind;
	d.scope_guid_id = scope_guid_id;
	d.identifier_id = identifier_id;
	d.tag_type = tag_type;
	d.order_id = order_id;
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

struct named_tag_definition_id make_named_tag_definition_named(struct asm_state * state, struct tag_definition_id tag_definition_id, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id, unsigned int order_id){
	return make_named_tag_definition(state, ASM_NAMED_TAG_DEFINITION_NAMED, tag_definition_id, scope_guid_id, identifier_id, order_id);
}
struct named_tag_definition_id make_named_tag_definition(struct asm_state * state, enum asm_named_tag_definition_kind kind, struct tag_definition_id tag_definition_id, struct scope_guid_id scope_guid_id, struct identifier_id identifier_id, unsigned int order_id){
	struct named_tag_definition_details d;
	d.type = kind;
	d.tag_definition_id = tag_definition_id;
	d.scope_guid_id = scope_guid_id;
	d.identifier_id = identifier_id;
	d.order_id = order_id;
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

struct enum_member_id make_enum_member_has_expression(struct asm_state * state, struct identifier_id identifier_id, struct expression_id expression_id){
	return make_enum_member(state, ASM_ENUM_MEMBER_HAS_EXPRESSION, identifier_id, expression_id);
}
struct enum_member_id make_enum_member_no_expression(struct asm_state * state, struct identifier_id identifier_id){
	struct expression_id expression_id;
	expression_id.id = 0;
	return make_enum_member(state, ASM_ENUM_MEMBER_NO_EXPRESSION, identifier_id, expression_id);
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

struct struct_or_union_member_id make_struct_or_union_member_member(struct asm_state * state, struct general_type_id general_type_id, struct identifier_id identifier_id){
	return make_struct_or_union_member(state, ASM_STRUCT_OR_UNION_MEMBER_MEMBER, general_type_id, identifier_id);
}
struct struct_or_union_member_id make_struct_or_union_member_padding(struct asm_state * state, struct general_type_id general_type_id){
	struct identifier_id identifier_id;
	identifier_id.id = 0;
	return make_struct_or_union_member(state, ASM_STRUCT_OR_UNION_MEMBER_PADDING, general_type_id, identifier_id);
}
struct struct_or_union_member_id make_struct_or_union_member_c11_anonymous_member(struct asm_state * state, struct general_type_id general_type_id){
	struct identifier_id identifier_id;
	identifier_id.id = 0;
	return make_struct_or_union_member(state, ASM_STRUCT_OR_UNION_MEMBER_C11_ANONYMOUS_MEMBER, general_type_id, identifier_id);
}
struct struct_or_union_member_id make_struct_or_union_member(struct asm_state * state, enum asm_struct_or_union_member_kind kind, struct general_type_id general_type_id, struct identifier_id identifier_id){
	struct struct_or_union_member_details d;
	d.type = kind;
	d.general_type_id = general_type_id;
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

struct initializer_id make_initializer_initializer_list(struct asm_state * state, struct initializer_list_item_id id){
	return make_initializer(state, ASM_INITIALIZER_INITIALIZER_LIST, id.id);
}
struct initializer_id make_initializer_no_initializer(struct asm_state * state){
	unsigned int id;
	id = 0;
	return make_initializer(state, ASM_INITIALIZER_NO_INITIALIZER, id);
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

struct identifier_namespace_entry_id make_identifier_namespace_entry_typedef(struct asm_state * state, struct identifier_id identifier_id, struct scope_guid_id scope_guid_id, struct general_type_id general_type_id, unsigned int order_id){
	return make_identifier_namespace_entry(state, ASM_IDENTIFIER_NAMESPACE_ENTRY_TYPEDEF, identifier_id, scope_guid_id, general_type_id, order_id);
}
struct identifier_namespace_entry_id make_identifier_namespace_entry_declaration(struct asm_state * state, struct identifier_id identifier_id, struct scope_guid_id scope_guid_id, struct general_type_id general_type_id, unsigned int order_id){
	return make_identifier_namespace_entry(state, ASM_IDENTIFIER_NAMESPACE_ENTRY_DECLARATION, identifier_id, scope_guid_id, general_type_id, order_id);
}
struct identifier_namespace_entry_id make_identifier_namespace_entry(struct asm_state * state, enum asm_identifier_namespace_entry_kind kind, struct identifier_id identifier_id, struct scope_guid_id scope_guid_id, struct general_type_id general_type_id, unsigned int order_id){
	struct identifier_namespace_entry_details d;
	d.type = kind;
	d.identifier_id = identifier_id;
	d.scope_guid_id = scope_guid_id;
	d.general_type_id = general_type_id;
	d.order_id = order_id;
	if(struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map_exists(&state->identifier_namespace_entrys_map, d)){
		return struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map_get(&state->identifier_namespace_entrys_map, d);
	}else{
		struct identifier_namespace_entry_id new_id;
		new_id.id = struct_identifier_namespace_entry_details_list_size(&state->identifier_namespace_entrys);
		struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map_put(&state->identifier_namespace_entrys_map, d, new_id);
		struct_identifier_namespace_entry_details_list_add_end(&state->identifier_namespace_entrys, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_IDENTIFIER_NAMESPACE_ENTRY);
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
struct postfix_expression_part_id make_postfix_expression_part_function_dereference(struct asm_state * state, struct any_statement_list_item_id id){
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

struct unscoped_tag_specifier_id make_unscoped_tag_specifier_struct(struct asm_state * state, struct unscoped_struct_specifier_id id){
	return make_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_STRUCT, id.id);
}
struct unscoped_tag_specifier_id make_unscoped_tag_specifier_union(struct asm_state * state, struct unscoped_union_specifier_id id){
	return make_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_UNION, id.id);
}
struct unscoped_tag_specifier_id make_unscoped_tag_specifier_enum(struct asm_state * state, struct unscoped_enum_specifier_id id){
	return make_unscoped_tag_specifier(state, ASM_UNSCOPED_TAG_SPECIFIER_ENUM, id.id);
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

struct unscoped_union_specifier_id make_unscoped_union_specifier_named(struct asm_state * state, struct identifier_id id){
	return make_unscoped_union_specifier(state, ASM_UNSCOPED_UNION_SPECIFIER_NAMED, id.id);
}
struct unscoped_union_specifier_id make_unscoped_union_specifier_anonymous(struct asm_state * state, struct anonymous_entry_id id){
	return make_unscoped_union_specifier(state, ASM_UNSCOPED_UNION_SPECIFIER_ANONYMOUS, id.id);
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

struct unscoped_struct_specifier_id make_unscoped_struct_specifier_named(struct asm_state * state, struct identifier_id id){
	return make_unscoped_struct_specifier(state, ASM_UNSCOPED_STRUCT_SPECIFIER_NAMED, id.id);
}
struct unscoped_struct_specifier_id make_unscoped_struct_specifier_anonymous(struct asm_state * state, struct anonymous_entry_id id){
	return make_unscoped_struct_specifier(state, ASM_UNSCOPED_STRUCT_SPECIFIER_ANONYMOUS, id.id);
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

struct unscoped_enum_specifier_id make_unscoped_enum_specifier_named(struct asm_state * state, struct identifier_id id){
	return make_unscoped_enum_specifier(state, ASM_UNSCOPED_ENUM_SPECIFIER_NAMED, id.id);
}
struct unscoped_enum_specifier_id make_unscoped_enum_specifier_anonymous(struct asm_state * state, struct anonymous_entry_id id){
	return make_unscoped_enum_specifier(state, ASM_UNSCOPED_ENUM_SPECIFIER_ANONYMOUS, id.id);
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

struct switch_statement_id make_switch_statement_switch(struct asm_state * state, struct expression_id e1, struct scoped_statement_id s1){
	return make_switch_statement(state, ASM_SWITCH_STATEMENT_SWITCH, e1, s1);
}
struct switch_statement_id make_switch_statement(struct asm_state * state, enum asm_switch_statement_kind kind, struct expression_id e1, struct scoped_statement_id s1){
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

struct if_statement_id make_if_statement_just_if(struct asm_state * state, struct scoped_statement_id s1, struct expression_id e1){
	struct scoped_statement_id s2;
	s2.id = 0;
	return make_if_statement(state, ASM_IF_STATEMENT_JUST_IF, s1, e1, s2);
}
struct if_statement_id make_if_statement_if_else(struct asm_state * state, struct scoped_statement_id s1, struct expression_id e1, struct scoped_statement_id s2){
	return make_if_statement(state, ASM_IF_STATEMENT_IF_ELSE, s1, e1, s2);
}
struct if_statement_id make_if_statement(struct asm_state * state, enum asm_if_statement_kind kind, struct scoped_statement_id s1, struct expression_id e1, struct scoped_statement_id s2){
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

struct while_statement_id make_while_statement_do_while(struct asm_state * state, struct expression_id e1, struct scoped_statement_id s1){
	return make_while_statement(state, ASM_WHILE_STATEMENT_DO_WHILE, e1, s1);
}
struct while_statement_id make_while_statement_while(struct asm_state * state, struct expression_id e1, struct scoped_statement_id s1){
	return make_while_statement(state, ASM_WHILE_STATEMENT_WHILE, e1, s1);
}
struct while_statement_id make_while_statement(struct asm_state * state, enum asm_while_statement_kind kind, struct expression_id e1, struct scoped_statement_id s1){
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

struct for_statement_id make_for_statement_for(struct asm_state * state, struct any_statement_id e1, struct any_statement_id e2, struct any_statement_id e3, struct scoped_statement_id s1){
	return make_for_statement(state, ASM_FOR_STATEMENT_FOR, e1, e2, e3, s1);
}
struct for_statement_id make_for_statement(struct asm_state * state, enum asm_for_statement_kind kind, struct any_statement_id e1, struct any_statement_id e2, struct any_statement_id e3, struct scoped_statement_id s1){
	struct for_statement_details d;
	d.type = kind;
	d.e1 = e1;
	d.e2 = e2;
	d.e3 = e3;
	d.s1 = s1;
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
struct code_statement_id make_code_statement_expression_statement(struct asm_state * state, struct expression_id id){
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

struct labeled_statement_id make_labeled_statement_goto_label(struct asm_state * state, struct scoped_statement_id scoped_statement_id, struct identifier_id id){
	return make_labeled_statement(state, ASM_LABELED_STATEMENT_GOTO_LABEL, scoped_statement_id, id.id);
}
struct labeled_statement_id make_labeled_statement_case_label(struct asm_state * state, struct scoped_statement_id scoped_statement_id, struct expression_id id){
	return make_labeled_statement(state, ASM_LABELED_STATEMENT_CASE_LABEL, scoped_statement_id, id.id);
}
struct labeled_statement_id make_labeled_statement_default_label(struct asm_state * state, struct scoped_statement_id scoped_statement_id){
	unsigned int id;
	id = 0;
	return make_labeled_statement(state, ASM_LABELED_STATEMENT_DEFAULT_LABEL, scoped_statement_id, id);
}
struct labeled_statement_id make_labeled_statement(struct asm_state * state, enum asm_labeled_statement_kind kind, struct scoped_statement_id scoped_statement_id, unsigned int id){
	struct labeled_statement_details d;
	d.type = kind;
	d.scoped_statement_id = scoped_statement_id;
	d.id = id;
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
	struct scope_guid_id scope_guid_id;
	scope_guid_id.id = 0;
	return make_primary_expression(state, ASM_PRIMARY_EXPRESSION_CONSTANT_DESCRIPTION, id.id, scope_guid_id);
}
struct primary_expression_id make_primary_expression_identifier(struct asm_state * state, struct identifier_id id, struct scope_guid_id scope_guid_id){
	return make_primary_expression(state, ASM_PRIMARY_EXPRESSION_IDENTIFIER, id.id, scope_guid_id);
}
struct primary_expression_id make_primary_expression_expression(struct asm_state * state, struct expression_id id){
	struct scope_guid_id scope_guid_id;
	scope_guid_id.id = 0;
	return make_primary_expression(state, ASM_PRIMARY_EXPRESSION_EXPRESSION, id.id, scope_guid_id);
}
struct primary_expression_id make_primary_expression(struct asm_state * state, enum asm_primary_expression_kind kind, unsigned int id, struct scope_guid_id scope_guid_id){
	struct primary_expression_details d;
	d.type = kind;
	d.id = id;
	d.scope_guid_id = scope_guid_id;
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

struct scoped_statement_id make_scoped_statement_scoped_statement(struct asm_state * state, struct any_statement_list_item_id any_statement_list_item_id, struct scope_guid_id scope_guid_id){
	return make_scoped_statement(state, ASM_SCOPED_STATEMENT_SCOPED_STATEMENT, any_statement_list_item_id, scope_guid_id);
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

struct function_definition_id make_function_definition_function_definition(struct asm_state * state, struct general_type_id general_type_id, struct any_statement_list_item_id k_and_r_declarations, struct any_statement_list_item_id params, struct scoped_statement_id body, struct identifier_id identifier_id){
	return make_function_definition(state, ASM_FUNCTION_DEFINITION_FUNCTION_DEFINITION, general_type_id, k_and_r_declarations, params, body, identifier_id);
}
struct function_definition_id make_function_definition(struct asm_state * state, enum asm_function_definition_kind kind, struct general_type_id general_type_id, struct any_statement_list_item_id k_and_r_declarations, struct any_statement_list_item_id params, struct scoped_statement_id body, struct identifier_id identifier_id){
	struct function_definition_details d;
	d.type = kind;
	d.general_type_id = general_type_id;
	d.k_and_r_declarations = k_and_r_declarations;
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

struct expression_id make_expression_nullary_expression(struct asm_state * state){
	unsigned int id;
	id = 0;
	return make_expression(state, ASM_EXPRESSION_NULLARY_EXPRESSION, id);
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
struct unary_expression_id make_unary_expression_increment(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_INCREMENT, id1.id);
}
struct unary_expression_id make_unary_expression_decrement(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_DECREMENT, id1.id);
}
struct unary_expression_id make_unary_expression_address_of(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_ADDRESS_OF, id1.id);
}
struct unary_expression_id make_unary_expression_point_to(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_POINT_TO, id1.id);
}
struct unary_expression_id make_unary_expression_plus(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_PLUS, id1.id);
}
struct unary_expression_id make_unary_expression_minus(struct asm_state * state, struct expression_id id1){
	return make_unary_expression(state, ASM_UNARY_EXPRESSION_MINUS, id1.id);
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

struct binary_expression_id make_binary_expression_comma(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_COMMA, e1, e2);
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
struct binary_expression_id make_binary_expression_right(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_RIGHT, e1, e2);
}
struct binary_expression_id make_binary_expression_left(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_LEFT, e1, e2);
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
struct binary_expression_id make_binary_expression_ne(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_NE, e1, e2);
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
struct binary_expression_id make_binary_expression_xor_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_XOR_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_mod_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_MOD_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_right_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_RIGHT_ASSIGN, e1, e2);
}
struct binary_expression_id make_binary_expression_left_assign(struct asm_state * state, struct expression_id e1, struct expression_id e2){
	return make_binary_expression(state, ASM_BINARY_EXPRESSION_LEFT_ASSIGN, e1, e2);
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

struct string_package_id make_string_package_string(struct asm_state * state, unsigned int byte_length, unsigned int word_length, unsigned int * data){
	return make_string_package(state, ASM_STRING_PACKAGE_STRING, byte_length, word_length, data);
}
struct string_package_id make_string_package(struct asm_state * state, enum asm_string_package_kind kind, unsigned int byte_length, unsigned int word_length, unsigned int * data){
	struct string_package_details d;
	d.type = kind;
	d.byte_length = byte_length;
	d.word_length = word_length;
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

struct scope_guid_id make_scope_guid_block_scope(struct asm_state * state, struct scope_guid_id parent){
	unsigned int id;
	id = state->next_scope_guid++;
	return make_scope_guid(state, ASM_SCOPE_GUID_BLOCK_SCOPE, parent, id);
}
struct scope_guid_id make_scope_guid_file_scope(struct asm_state * state){
	struct scope_guid_id parent;
	unsigned int id;
	parent.id = 0;
	id = state->next_scope_guid++;
	return make_scope_guid(state, ASM_SCOPE_GUID_FILE_SCOPE, parent, id);
}
struct scope_guid_id make_scope_guid_function_scope(struct asm_state * state, struct scope_guid_id parent){
	unsigned int id;
	id = state->next_scope_guid++;
	return make_scope_guid(state, ASM_SCOPE_GUID_FUNCTION_SCOPE, parent, id);
}
struct scope_guid_id make_scope_guid_function_prototype_scope(struct asm_state * state, struct scope_guid_id parent){
	unsigned int id;
	id = state->next_scope_guid++;
	return make_scope_guid(state, ASM_SCOPE_GUID_FUNCTION_PROTOTYPE_SCOPE, parent, id);
}
struct scope_guid_id make_scope_guid(struct asm_state * state, enum asm_scope_guid_kind kind, struct scope_guid_id parent, unsigned int id){
	struct scope_guid_details d;
	d.type = kind;
	d.parent = parent;
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

struct anonymous_entry_id make_anonymous_entry_anonymous_entry(struct asm_state * state){
	unsigned int id;
	id = state->next_anonymous_entry++;
	return make_anonymous_entry(state, ASM_ANONYMOUS_ENTRY_ANONYMOUS_ENTRY, id);
}
struct anonymous_entry_id make_anonymous_entry(struct asm_state * state, enum asm_anonymous_entry_kind kind, unsigned int id){
	struct anonymous_entry_details d;
	d.type = kind;
	d.id = id;
	if(struct_anonymous_entry_details_to_struct_anonymous_entry_id_map_exists(&state->anonymous_entrys_map, d)){
		return struct_anonymous_entry_details_to_struct_anonymous_entry_id_map_get(&state->anonymous_entrys_map, d);
	}else{
		struct anonymous_entry_id new_id;
		new_id.id = struct_anonymous_entry_details_list_size(&state->anonymous_entrys);
		struct_anonymous_entry_details_to_struct_anonymous_entry_id_map_put(&state->anonymous_entrys_map, d, new_id);
		struct_anonymous_entry_details_list_add_end(&state->anonymous_entrys, d);
		enum_asm_operation_type_list_add_end(&state->reversible_operations, ASM_OP_ADD_ANONYMOUS_ENTRY);
		return new_id;
	}
}

struct aggregate_type_specifier_details get_aggregate_type_specifier_details_by_id(struct asm_state * s, struct aggregate_type_specifier_id id){
	return struct_aggregate_type_specifier_details_list_get(&s->aggregate_type_specifiers, id.id);
}
void print_all_aggregate_type_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_aggregate_type_specifier_details_list_size(&state->aggregate_type_specifiers);
	struct aggregate_type_specifier_id id;
	buffered_printf(out, "There are %u aggregate_type_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_aggregate_type_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_aggregate_type_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct aggregate_type_specifier_id id){
	unsigned int tabs_printed;
	struct aggregate_type_specifier_details details = struct_aggregate_type_specifier_details_list_get(&state->aggregate_type_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_AGGREGATE_TYPE_SPECIFIER_SIMPLE_TYPE_SPECIFIER:{
			struct simple_type_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_simple_type_specifier(state, indent_depth, out, id_tmp);
			break;
		}case ASM_AGGREGATE_TYPE_SPECIFIER_TAG_SPECIFIER:{
			struct scoped_tag_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_scoped_tag_specifier(state, indent_depth, out, id_tmp);
			break;
		}case ASM_AGGREGATE_TYPE_SPECIFIER_TYPENAME_SPECIFIER:{
			struct scoped_typename_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_scoped_typename_specifier(state, indent_depth, out, id_tmp);
			break;
		}
	}
}

struct simple_type_specifier_details get_simple_type_specifier_details_by_id(struct asm_state * s, struct simple_type_specifier_id id){
	return struct_simple_type_specifier_details_list_get(&s->simple_type_specifiers, id.id);
}
void print_all_simple_type_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_simple_type_specifier_details_list_size(&state->simple_type_specifiers);
	struct simple_type_specifier_id id;
	buffered_printf(out, "There are %u simple_type_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_simple_type_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_simple_type_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct simple_type_specifier_id id){
	unsigned int tabs_printed;
	struct simple_type_specifier_details details = struct_simple_type_specifier_details_list_get(&state->simple_type_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SIMPLE_TYPE_SPECIFIER_VOID:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"void\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_CHAR:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"char\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_SHORT:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"short\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_INT:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"int\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_LONG:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"long\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_FLOAT:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"float\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_DOUBLE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"double\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_SIGNED:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"signed\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_SPECIFIER_UNSIGNED:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_type_specifier\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"unsigned\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

struct simple_type_qualifier_details get_simple_type_qualifier_details_by_id(struct asm_state * s, struct simple_type_qualifier_id id){
	return struct_simple_type_qualifier_details_list_get(&s->simple_type_qualifiers, id.id);
}
void print_all_simple_type_qualifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_simple_type_qualifier_details_list_size(&state->simple_type_qualifiers);
	struct simple_type_qualifier_id id;
	buffered_printf(out, "There are %u simple_type_qualifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_simple_type_qualifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_simple_type_qualifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct simple_type_qualifier_id id){
	unsigned int tabs_printed;
	struct simple_type_qualifier_details details = struct_simple_type_qualifier_details_list_get(&state->simple_type_qualifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SIMPLE_TYPE_QUALIFIER_CONST:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"type_qualifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"const\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_TYPE_QUALIFIER_VOLATILE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"type_qualifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"volatile\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

struct simple_storage_class_specifier_details get_simple_storage_class_specifier_details_by_id(struct asm_state * s, struct simple_storage_class_specifier_id id){
	return struct_simple_storage_class_specifier_details_list_get(&s->simple_storage_class_specifiers, id.id);
}
void print_all_simple_storage_class_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_simple_storage_class_specifier_details_list_size(&state->simple_storage_class_specifiers);
	struct simple_storage_class_specifier_id id;
	buffered_printf(out, "There are %u simple_storage_class_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_simple_storage_class_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_simple_storage_class_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct simple_storage_class_specifier_id id){
	unsigned int tabs_printed;
	struct simple_storage_class_specifier_details details = struct_simple_storage_class_specifier_details_list_get(&state->simple_storage_class_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_TYPEDEF:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"storage_class_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"typedef\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_EXTERN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"storage_class_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"extern\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_STATIC:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"storage_class_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"static\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_AUTO:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"storage_class_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"auto\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}case ASM_SIMPLE_STORAGE_CLASS_SPECIFIER_REGISTER:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"storage_class_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			buffered_printf(out, "\"register\"");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			(void)state;
			(void)out;
			(void)details;
			break;
		}
	}
}

struct specifier_or_qualifier_details get_specifier_or_qualifier_details_by_id(struct asm_state * s, struct specifier_or_qualifier_id id){
	return struct_specifier_or_qualifier_details_list_get(&s->specifier_or_qualifiers, id.id);
}
void print_all_specifier_or_qualifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_specifier_or_qualifier_details_list_size(&state->specifier_or_qualifiers);
	struct specifier_or_qualifier_id id;
	buffered_printf(out, "There are %u specifier_or_qualifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_specifier_or_qualifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_specifier_or_qualifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct specifier_or_qualifier_id id){
	unsigned int tabs_printed;
	struct specifier_or_qualifier_details details = struct_specifier_or_qualifier_details_list_get(&state->specifier_or_qualifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_STORAGE_CLASS_SPECIFIER:{
			struct simple_storage_class_specifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"specifier_or_qualifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			print_simple_storage_class_specifier(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_SPECIFIER_OR_QUALIFIER_SIMPLE_TYPE_QUALIFIER:{
			struct simple_type_qualifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"specifier_or_qualifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			print_simple_type_qualifier(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_SPECIFIER_OR_QUALIFIER_AGGREGATE_TYPE_SPECIFIER:{
			struct aggregate_type_specifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"specifier_or_qualifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			print_aggregate_type_specifier(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct specifier_or_qualifier_list_item_details get_specifier_or_qualifier_list_item_details_by_id(struct asm_state * s, struct specifier_or_qualifier_list_item_id id){
	return struct_specifier_or_qualifier_list_item_details_list_get(&s->specifier_or_qualifier_list_items, id.id);
}
void print_all_specifier_or_qualifier_list_items(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_specifier_or_qualifier_list_item_details_list_size(&state->specifier_or_qualifier_list_items);
	struct specifier_or_qualifier_list_item_id id;
	buffered_printf(out, "There are %u specifier_or_qualifier_list_items:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_specifier_or_qualifier_list_item(state, indent_depth, out, id, 0);
		buffered_printf(out, "\n");
	}
}
void print_specifier_or_qualifier_list_item(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct specifier_or_qualifier_list_item_id id, unsigned int recursion_depth){
	unsigned int tabs_printed;
	struct specifier_or_qualifier_list_item_details details = struct_specifier_or_qualifier_list_item_details_list_get(&state->specifier_or_qualifier_list_items, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	(void)recursion_depth;
	switch(details.type){
		case ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER:{
			if(!(get_specifier_or_qualifier_list_item_details_by_id(state, details.prev_id).type != ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START)){
			buffered_printf(out, "[\n");
			}
			print_specifier_or_qualifier_list_item(state, indent_depth, out, details.prev_id, recursion_depth + 1);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			print_specifier_or_qualifier(state, indent_depth, out, details.item_id);
			if(!(recursion_depth == 0)){
			buffered_printf(out, ",\n");
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "\n");
			}
			if(!(recursion_depth > 0)){
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "]");
			}
			break;
		}case ASM_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START:{
			if(!(recursion_depth > 0)){
			buffered_printf(out, "[]");
			}
			break;
		}
	}
}

struct parameter_list_details get_parameter_list_details_by_id(struct asm_state * s, struct parameter_list_id id){
	return struct_parameter_list_details_list_get(&s->parameter_lists, id.id);
}
void print_all_parameter_lists(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_parameter_list_details_list_size(&state->parameter_lists);
	struct parameter_list_id id;
	buffered_printf(out, "There are %u parameter_lists:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_parameter_list(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_parameter_list(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct parameter_list_id id){
	unsigned int tabs_printed;
	struct parameter_list_details details = struct_parameter_list_details_list_get(&state->parameter_lists, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_PARAMETER_LIST_VARIADIC:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"parameter_list\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"variadic\" : 1\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"parameters\" : ");
			print_general_type_list_item(state, indent_depth + 1, out, details.general_type_list_item_id, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_PARAMETER_LIST_NON_VARIADIC:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"parameter_list\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"variadic\" : 0\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"parameters\" : ");
			print_general_type_list_item(state, indent_depth + 1, out, details.general_type_list_item_id, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct declarator_part_details get_declarator_part_details_by_id(struct asm_state * s, struct declarator_part_id id){
	return struct_declarator_part_details_list_get(&s->declarator_parts, id.id);
}
void print_all_declarator_parts(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_declarator_part_details_list_size(&state->declarator_parts);
	struct declarator_part_id id;
	buffered_printf(out, "There are %u declarator_parts:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_declarator_part(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_declarator_part(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct declarator_part_id id){
	unsigned int tabs_printed;
	struct declarator_part_details details = struct_declarator_part_details_list_get(&state->declarator_parts, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_DECLARATOR_PART_FUNCTION:{
			struct function_part_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"declarator_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"part\" : ");
			print_function_part(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DECLARATOR_PART_POINTER:{
			struct pointer_part_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"declarator_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"part\" : ");
			print_pointer_part(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DECLARATOR_PART_ARRAY:{
			struct array_part_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"declarator_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"part\" : ");
			print_array_part(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct any_statement_list_item_details get_any_statement_list_item_details_by_id(struct asm_state * s, struct any_statement_list_item_id id){
	return struct_any_statement_list_item_details_list_get(&s->any_statement_list_items, id.id);
}
void print_all_any_statement_list_items(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_any_statement_list_item_details_list_size(&state->any_statement_list_items);
	struct any_statement_list_item_id id;
	buffered_printf(out, "There are %u any_statement_list_items:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_any_statement_list_item(state, indent_depth, out, id, 0);
		buffered_printf(out, "\n");
	}
}
void print_any_statement_list_item(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct any_statement_list_item_id id, unsigned int recursion_depth){
	unsigned int tabs_printed;
	struct any_statement_list_item_details details = struct_any_statement_list_item_details_list_get(&state->any_statement_list_items, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	(void)recursion_depth;
	switch(details.type){
		case ASM_ANY_STATEMENT_LIST_ITEM_ANY_STATEMENT:{
			if(!(get_any_statement_list_item_details_by_id(state, details.prev_id).type != ASM_ANY_STATEMENT_LIST_ITEM_LIST_START)){
			buffered_printf(out, "[\n");
			}
			print_any_statement_list_item(state, indent_depth, out, details.prev_id, recursion_depth + 1);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			print_any_statement(state, indent_depth, out, details.item_id);
			if(!(recursion_depth == 0)){
			buffered_printf(out, ",\n");
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "\n");
			}
			if(!(recursion_depth > 0)){
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "]");
			}
			break;
		}case ASM_ANY_STATEMENT_LIST_ITEM_LIST_START:{
			if(!(recursion_depth > 0)){
			buffered_printf(out, "[]");
			}
			break;
		}
	}
}

struct anonymous_tag_definition_details get_anonymous_tag_definition_details_by_id(struct asm_state * s, struct anonymous_tag_definition_id id){
	return struct_anonymous_tag_definition_details_list_get(&s->anonymous_tag_definitions, id.id);
}
void print_all_anonymous_tag_definitions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_anonymous_tag_definition_details_list_size(&state->anonymous_tag_definitions);
	struct anonymous_tag_definition_id id;
	buffered_printf(out, "There are %u anonymous_tag_definitions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_anonymous_tag_definition(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_anonymous_tag_definition(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct anonymous_tag_definition_id id){
	unsigned int tabs_printed;
	struct anonymous_tag_definition_details details = struct_anonymous_tag_definition_details_list_get(&state->anonymous_tag_definitions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_ANONYMOUS_TAG_DEFINITION_ANONYMOUS:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"anonymous_entry_id\" : ");
			print_anonymous_entry(state, indent_depth, out, details.anonymous_entry_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"scope_guid\" : ");
			buffered_printf(out, "%u", details.scope_guid_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"tag_definition_id\" : ");
			buffered_printf(out, "%u", details.tag_definition_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct named_tag_predeclaration_details get_named_tag_predeclaration_details_by_id(struct asm_state * s, struct named_tag_predeclaration_id id){
	return struct_named_tag_predeclaration_details_list_get(&s->named_tag_predeclarations, id.id);
}
void print_all_named_tag_predeclarations(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_named_tag_predeclaration_details_list_size(&state->named_tag_predeclarations);
	struct named_tag_predeclaration_id id;
	buffered_printf(out, "There are %u named_tag_predeclarations:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_named_tag_predeclaration(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_named_tag_predeclaration(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct named_tag_predeclaration_id id){
	unsigned int tabs_printed;
	struct named_tag_predeclaration_details details = struct_named_tag_predeclaration_details_list_get(&state->named_tag_predeclarations, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_NAMED_TAG_PREDECLARATION_NAMED:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"tag_type\" : \"");
			print_terminal_named_tag_predeclaration_named_tag_type(state, indent_depth, out, details.tag_type);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.scope_guid_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"order_id\" : ");
			buffered_printf(out, "%u", details.order_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth, out, details.identifier_id);
			buffered_printf(out, "\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct named_tag_definition_details get_named_tag_definition_details_by_id(struct asm_state * s, struct named_tag_definition_id id){
	return struct_named_tag_definition_details_list_get(&s->named_tag_definitions, id.id);
}
void print_all_named_tag_definitions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_named_tag_definition_details_list_size(&state->named_tag_definitions);
	struct named_tag_definition_id id;
	buffered_printf(out, "There are %u named_tag_definitions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_named_tag_definition(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_named_tag_definition(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct named_tag_definition_id id){
	unsigned int tabs_printed;
	struct named_tag_definition_details details = struct_named_tag_definition_details_list_get(&state->named_tag_definitions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_NAMED_TAG_DEFINITION_NAMED:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"identifier\" : \"");
			print_identifier(state, indent_depth, out, details.identifier_id);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"scope_guid\" : ");
			buffered_printf(out, "%u", details.scope_guid_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"order_id\" : ");
			buffered_printf(out, "%u", details.order_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"tag_definition_id\" : ");
			buffered_printf(out, "%u", details.tag_definition_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct tag_definition_details get_tag_definition_details_by_id(struct asm_state * s, struct tag_definition_id id){
	return struct_tag_definition_details_list_get(&s->tag_definitions, id.id);
}
void print_all_tag_definitions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_tag_definition_details_list_size(&state->tag_definitions);
	struct tag_definition_id id;
	buffered_printf(out, "There are %u tag_definitions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_tag_definition(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_tag_definition(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct tag_definition_id id){
	unsigned int tabs_printed;
	struct tag_definition_details details = struct_tag_definition_details_list_get(&state->tag_definitions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_TAG_DEFINITION_ENUM:{
			struct enum_member_list_item_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"tag_type\" : \"enum\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"members\" : ");
			print_enum_member_list_item(state, indent_depth + 1, out, id_tmp, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_TAG_DEFINITION_STRUCT:{
			struct struct_or_union_member_list_item_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"tag_type\" : \"struct\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"members\" : ");
			print_struct_or_union_member_list_item(state, indent_depth + 1, out, id_tmp, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_TAG_DEFINITION_UNION:{
			struct struct_or_union_member_list_item_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"tag_type\" : \"union\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"members\" : ");
			print_struct_or_union_member_list_item(state, indent_depth + 1, out, id_tmp, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct struct_or_union_member_list_item_details get_struct_or_union_member_list_item_details_by_id(struct asm_state * s, struct struct_or_union_member_list_item_id id){
	return struct_struct_or_union_member_list_item_details_list_get(&s->struct_or_union_member_list_items, id.id);
}
void print_all_struct_or_union_member_list_items(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_struct_or_union_member_list_item_details_list_size(&state->struct_or_union_member_list_items);
	struct struct_or_union_member_list_item_id id;
	buffered_printf(out, "There are %u struct_or_union_member_list_items:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_struct_or_union_member_list_item(state, indent_depth, out, id, 0);
		buffered_printf(out, "\n");
	}
}
void print_struct_or_union_member_list_item(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct struct_or_union_member_list_item_id id, unsigned int recursion_depth){
	unsigned int tabs_printed;
	struct struct_or_union_member_list_item_details details = struct_struct_or_union_member_list_item_details_list_get(&state->struct_or_union_member_list_items, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	(void)recursion_depth;
	switch(details.type){
		case ASM_STRUCT_OR_UNION_MEMBER_LIST_ITEM_MEMBER:{
			if(!(get_struct_or_union_member_list_item_details_by_id(state, details.prev_id).type != ASM_STRUCT_OR_UNION_MEMBER_LIST_ITEM_LIST_START)){
			buffered_printf(out, "[\n");
			}
			print_struct_or_union_member_list_item(state, indent_depth, out, details.prev_id, recursion_depth + 1);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			print_struct_or_union_member(state, indent_depth, out, details.item_id);
			if(!(recursion_depth == 0)){
			buffered_printf(out, ",\n");
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "\n");
			}
			if(!(recursion_depth > 0)){
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "]");
			}
			break;
		}case ASM_STRUCT_OR_UNION_MEMBER_LIST_ITEM_LIST_START:{
			if(!(recursion_depth > 0)){
			buffered_printf(out, "[]");
			}
			break;
		}
	}
}

struct enum_member_list_item_details get_enum_member_list_item_details_by_id(struct asm_state * s, struct enum_member_list_item_id id){
	return struct_enum_member_list_item_details_list_get(&s->enum_member_list_items, id.id);
}
void print_all_enum_member_list_items(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_enum_member_list_item_details_list_size(&state->enum_member_list_items);
	struct enum_member_list_item_id id;
	buffered_printf(out, "There are %u enum_member_list_items:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_enum_member_list_item(state, indent_depth, out, id, 0);
		buffered_printf(out, "\n");
	}
}
void print_enum_member_list_item(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct enum_member_list_item_id id, unsigned int recursion_depth){
	unsigned int tabs_printed;
	struct enum_member_list_item_details details = struct_enum_member_list_item_details_list_get(&state->enum_member_list_items, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	(void)recursion_depth;
	switch(details.type){
		case ASM_ENUM_MEMBER_LIST_ITEM_MEMBER:{
			if(!(get_enum_member_list_item_details_by_id(state, details.prev_id).type != ASM_ENUM_MEMBER_LIST_ITEM_LIST_START)){
			buffered_printf(out, "[\n");
			}
			print_enum_member_list_item(state, indent_depth, out, details.prev_id, recursion_depth + 1);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			print_enum_member(state, indent_depth, out, details.item_id);
			if(!(recursion_depth == 0)){
			buffered_printf(out, ",\n");
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "\n");
			}
			if(!(recursion_depth > 0)){
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "]");
			}
			break;
		}case ASM_ENUM_MEMBER_LIST_ITEM_LIST_START:{
			if(!(recursion_depth > 0)){
			buffered_printf(out, "[]");
			}
			break;
		}
	}
}

struct enum_member_details get_enum_member_details_by_id(struct asm_state * s, struct enum_member_id id){
	return struct_enum_member_details_list_get(&s->enum_members, id.id);
}
void print_all_enum_members(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_enum_member_details_list_size(&state->enum_members);
	struct enum_member_id id;
	buffered_printf(out, "There are %u enum_members:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_enum_member(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_enum_member(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct enum_member_id id){
	unsigned int tabs_printed;
	struct enum_member_details details = struct_enum_member_details_list_get(&state->enum_members, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_ENUM_MEMBER_HAS_EXPRESSION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"enum_member\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, details.identifier_id);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"expression\" : ");
			print_expression(state, indent_depth + 1, out, details.expression_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_ENUM_MEMBER_NO_EXPRESSION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"enum_member\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, details.identifier_id);
			buffered_printf(out, "\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct struct_or_union_member_details get_struct_or_union_member_details_by_id(struct asm_state * s, struct struct_or_union_member_id id){
	return struct_struct_or_union_member_details_list_get(&s->struct_or_union_members, id.id);
}
void print_all_struct_or_union_members(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_struct_or_union_member_details_list_size(&state->struct_or_union_members);
	struct struct_or_union_member_id id;
	buffered_printf(out, "There are %u struct_or_union_members:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_struct_or_union_member(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_struct_or_union_member(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct struct_or_union_member_id id){
	unsigned int tabs_printed;
	struct struct_or_union_member_details details = struct_struct_or_union_member_details_list_get(&state->struct_or_union_members, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_STRUCT_OR_UNION_MEMBER_MEMBER:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"struct_or_union_member\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"purpose\" : \"member\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, details.identifier_id);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"type\" : ");
			print_general_type(state, indent_depth + 1, out, details.general_type_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_STRUCT_OR_UNION_MEMBER_PADDING:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"struct_or_union_member\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"purpose\" : \"padding\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"type\" : ");
			print_general_type(state, indent_depth + 1, out, details.general_type_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_STRUCT_OR_UNION_MEMBER_C11_ANONYMOUS_MEMBER:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"struct_or_union_member\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"purpose\" : \"c11_anonymous_inner_tag\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"type\" : ");
			print_general_type(state, indent_depth + 1, out, details.general_type_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct initializer_details get_initializer_details_by_id(struct asm_state * s, struct initializer_id id){
	return struct_initializer_details_list_get(&s->initializers, id.id);
}
void print_all_initializers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_initializer_details_list_size(&state->initializers);
	struct initializer_id id;
	buffered_printf(out, "There are %u initializers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_initializer(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_initializer(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct initializer_id id){
	unsigned int tabs_printed;
	struct initializer_details details = struct_initializer_details_list_get(&state->initializers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_INITIALIZER_INITIALIZER_LIST:{
			struct initializer_list_item_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"initializer\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"initializer_list\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"initializer_list\" : ");
			print_initializer_list_item(state, indent_depth + 1, out, id_tmp, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_INITIALIZER_NO_INITIALIZER:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"initializer\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"no_initializer\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_INITIALIZER_EXPRESSION:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"initializer\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"expression\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct initializer_list_item_details get_initializer_list_item_details_by_id(struct asm_state * s, struct initializer_list_item_id id){
	return struct_initializer_list_item_details_list_get(&s->initializer_list_items, id.id);
}
void print_all_initializer_list_items(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_initializer_list_item_details_list_size(&state->initializer_list_items);
	struct initializer_list_item_id id;
	buffered_printf(out, "There are %u initializer_list_items:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_initializer_list_item(state, indent_depth, out, id, 0);
		buffered_printf(out, "\n");
	}
}
void print_initializer_list_item(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct initializer_list_item_id id, unsigned int recursion_depth){
	unsigned int tabs_printed;
	struct initializer_list_item_details details = struct_initializer_list_item_details_list_get(&state->initializer_list_items, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	(void)recursion_depth;
	switch(details.type){
		case ASM_INITIALIZER_LIST_ITEM_INITIALIZER:{
			if(!(get_initializer_list_item_details_by_id(state, details.prev_id).type != ASM_INITIALIZER_LIST_ITEM_LIST_START)){
			buffered_printf(out, "[\n");
			}
			print_initializer_list_item(state, indent_depth, out, details.prev_id, recursion_depth + 1);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			print_initializer(state, indent_depth, out, details.item_id);
			if(!(recursion_depth == 0)){
			buffered_printf(out, ",\n");
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "\n");
			}
			if(!(recursion_depth > 0)){
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "]");
			}
			break;
		}case ASM_INITIALIZER_LIST_ITEM_LIST_START:{
			if(!(recursion_depth > 0)){
			buffered_printf(out, "[]");
			}
			break;
		}
	}
}

struct declarator_part_list_item_details get_declarator_part_list_item_details_by_id(struct asm_state * s, struct declarator_part_list_item_id id){
	return struct_declarator_part_list_item_details_list_get(&s->declarator_part_list_items, id.id);
}
void print_all_declarator_part_list_items(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_declarator_part_list_item_details_list_size(&state->declarator_part_list_items);
	struct declarator_part_list_item_id id;
	buffered_printf(out, "There are %u declarator_part_list_items:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_declarator_part_list_item(state, indent_depth, out, id, 0);
		buffered_printf(out, "\n");
	}
}
void print_declarator_part_list_item(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct declarator_part_list_item_id id, unsigned int recursion_depth){
	unsigned int tabs_printed;
	struct declarator_part_list_item_details details = struct_declarator_part_list_item_details_list_get(&state->declarator_part_list_items, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	(void)recursion_depth;
	switch(details.type){
		case ASM_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART:{
			if(!(get_declarator_part_list_item_details_by_id(state, details.prev_id).type != ASM_DECLARATOR_PART_LIST_ITEM_LIST_START)){
			buffered_printf(out, "[\n");
			}
			print_declarator_part_list_item(state, indent_depth, out, details.prev_id, recursion_depth + 1);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			print_declarator_part(state, indent_depth, out, details.item_id);
			if(!(recursion_depth == 0)){
			buffered_printf(out, ",\n");
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "\n");
			}
			if(!(recursion_depth > 0)){
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "]");
			}
			break;
		}case ASM_DECLARATOR_PART_LIST_ITEM_LIST_START:{
			if(!(recursion_depth > 0)){
			buffered_printf(out, "[]");
			}
			break;
		}
	}
}

struct pointer_part_details get_pointer_part_details_by_id(struct asm_state * s, struct pointer_part_id id){
	return struct_pointer_part_details_list_get(&s->pointer_parts, id.id);
}
void print_all_pointer_parts(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_pointer_part_details_list_size(&state->pointer_parts);
	struct pointer_part_id id;
	buffered_printf(out, "There are %u pointer_parts:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_pointer_part(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_pointer_part(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct pointer_part_id id){
	unsigned int tabs_printed;
	struct pointer_part_details details = struct_pointer_part_details_list_get(&state->pointer_parts, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_POINTER_PART_POINTER:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"pointer\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"specifiers\" : ");
			print_specifier_or_qualifier_list_item(state, indent_depth + 1, out, details.specifier_or_qualifier_list_item_id, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct function_part_details get_function_part_details_by_id(struct asm_state * s, struct function_part_id id){
	return struct_function_part_details_list_get(&s->function_parts, id.id);
}
void print_all_function_parts(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_function_part_details_list_size(&state->function_parts);
	struct function_part_id id;
	buffered_printf(out, "There are %u function_parts:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_function_part(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_function_part(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct function_part_id id){
	unsigned int tabs_printed;
	struct function_part_details details = struct_function_part_details_list_get(&state->function_parts, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_FUNCTION_PART_PROTOTYPE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"function_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"parameter_list\" : ");
			print_parameter_list(state, indent_depth + 1, out, details.parameter_list_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_FUNCTION_PART_K_AND_R_C:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"function_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"parameter_list\" : ");
			print_parameter_list(state, indent_depth + 1, out, details.parameter_list_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct array_part_details get_array_part_details_by_id(struct asm_state * s, struct array_part_id id){
	return struct_array_part_details_list_get(&s->array_parts, id.id);
}
void print_all_array_parts(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_array_part_details_list_size(&state->array_parts);
	struct array_part_id id;
	buffered_printf(out, "There are %u array_parts:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_array_part(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_array_part(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct array_part_id id){
	unsigned int tabs_printed;
	struct array_part_details details = struct_array_part_details_list_get(&state->array_parts, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_ARRAY_PART_EXPRESSION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"array_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"flexible\" : 0,\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t");
			print_expression(state, indent_depth + 1, out, details.expression_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_ARRAY_PART_FLEXIBLE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"array_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"flexible\" : 1\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct general_type_details get_general_type_details_by_id(struct asm_state * s, struct general_type_id id){
	return struct_general_type_details_list_get(&s->general_types, id.id);
}
void print_all_general_types(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_general_type_details_list_size(&state->general_types);
	struct general_type_id id;
	buffered_printf(out, "There are %u general_types:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_general_type(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_general_type(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct general_type_id id){
	unsigned int tabs_printed;
	struct general_type_details details = struct_general_type_details_list_get(&state->general_types, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_GENERAL_TYPE_GENERAL_TYPE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"general_type\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"specifier_or_qualifiers\" : ");
			print_specifier_or_qualifier_list_item(state, indent_depth + 1, out, details.specifier_or_qualifier_list_item_id, 0);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"declarator_parts\" : ");
			print_declarator_part_list_item(state, indent_depth + 1, out, details.declarator_part_list_item_id, 0);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"bitfield\" : ");
			print_bitfield(state, indent_depth + 1, out, details.bitfield_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct identifier_namespace_entry_details get_identifier_namespace_entry_details_by_id(struct asm_state * s, struct identifier_namespace_entry_id id){
	return struct_identifier_namespace_entry_details_list_get(&s->identifier_namespace_entrys, id.id);
}
void print_all_identifier_namespace_entrys(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_identifier_namespace_entry_details_list_size(&state->identifier_namespace_entrys);
	struct identifier_namespace_entry_id id;
	buffered_printf(out, "There are %u identifier_namespace_entrys:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_identifier_namespace_entry(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_identifier_namespace_entry(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct identifier_namespace_entry_id id){
	unsigned int tabs_printed;
	struct identifier_namespace_entry_details details = struct_identifier_namespace_entry_details_list_get(&state->identifier_namespace_entrys, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_IDENTIFIER_NAMESPACE_ENTRY_TYPEDEF:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"identifier_namespace_entry\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"purpose\" : \"typename\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, details.identifier_id);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"order_id\" : ");
			buffered_printf(out, "%u", details.order_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"type\" : ");
			print_general_type(state, indent_depth + 1, out, details.general_type_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.scope_guid_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_IDENTIFIER_NAMESPACE_ENTRY_DECLARATION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"identifier_namespace_entry\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"purpose\" : \"variable\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, details.identifier_id);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"order_id\" : ");
			buffered_printf(out, "%u", details.order_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"type\" : ");
			print_general_type(state, indent_depth + 1, out, details.general_type_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.scope_guid_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct general_type_list_item_details get_general_type_list_item_details_by_id(struct asm_state * s, struct general_type_list_item_id id){
	return struct_general_type_list_item_details_list_get(&s->general_type_list_items, id.id);
}
void print_all_general_type_list_items(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_general_type_list_item_details_list_size(&state->general_type_list_items);
	struct general_type_list_item_id id;
	buffered_printf(out, "There are %u general_type_list_items:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_general_type_list_item(state, indent_depth, out, id, 0);
		buffered_printf(out, "\n");
	}
}
void print_general_type_list_item(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct general_type_list_item_id id, unsigned int recursion_depth){
	unsigned int tabs_printed;
	struct general_type_list_item_details details = struct_general_type_list_item_details_list_get(&state->general_type_list_items, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	(void)recursion_depth;
	switch(details.type){
		case ASM_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE:{
			if(!(get_general_type_list_item_details_by_id(state, details.prev_id).type != ASM_GENERAL_TYPE_LIST_ITEM_LIST_START)){
			buffered_printf(out, "[\n");
			}
			print_general_type_list_item(state, indent_depth, out, details.prev_id, recursion_depth + 1);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			print_general_type(state, indent_depth, out, details.item_id);
			if(!(recursion_depth == 0)){
			buffered_printf(out, ",\n");
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "\n");
			}
			if(!(recursion_depth > 0)){
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "]");
			}
			break;
		}case ASM_GENERAL_TYPE_LIST_ITEM_LIST_START:{
			if(!(recursion_depth > 0)){
			buffered_printf(out, "[]");
			}
			break;
		}
	}
}

struct postfix_expression_part_list_item_details get_postfix_expression_part_list_item_details_by_id(struct asm_state * s, struct postfix_expression_part_list_item_id id){
	return struct_postfix_expression_part_list_item_details_list_get(&s->postfix_expression_part_list_items, id.id);
}
void print_all_postfix_expression_part_list_items(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_postfix_expression_part_list_item_details_list_size(&state->postfix_expression_part_list_items);
	struct postfix_expression_part_list_item_id id;
	buffered_printf(out, "There are %u postfix_expression_part_list_items:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_postfix_expression_part_list_item(state, indent_depth, out, id, 0);
		buffered_printf(out, "\n");
	}
}
void print_postfix_expression_part_list_item(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct postfix_expression_part_list_item_id id, unsigned int recursion_depth){
	unsigned int tabs_printed;
	struct postfix_expression_part_list_item_details details = struct_postfix_expression_part_list_item_details_list_get(&state->postfix_expression_part_list_items, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	(void)recursion_depth;
	switch(details.type){
		case ASM_POSTFIX_EXPRESSION_PART_LIST_ITEM_POSTFIX_EXPRESSION_PART:{
			if(!(get_postfix_expression_part_list_item_details_by_id(state, details.prev_id).type != ASM_POSTFIX_EXPRESSION_PART_LIST_ITEM_LIST_START)){
			buffered_printf(out, "[\n");
			}
			print_postfix_expression_part_list_item(state, indent_depth, out, details.prev_id, recursion_depth + 1);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			print_postfix_expression_part(state, indent_depth, out, details.item_id);
			if(!(recursion_depth == 0)){
			buffered_printf(out, ",\n");
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "\n");
			}
			if(!(recursion_depth > 0)){
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			}
			if(!(recursion_depth > 0)){
			buffered_printf(out, "]");
			}
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_LIST_ITEM_LIST_START:{
			if(!(recursion_depth > 0)){
			buffered_printf(out, "[]");
			}
			break;
		}
	}
}

struct postfix_expression_part_details get_postfix_expression_part_details_by_id(struct asm_state * s, struct postfix_expression_part_id id){
	return struct_postfix_expression_part_details_list_get(&s->postfix_expression_parts, id.id);
}
void print_all_postfix_expression_parts(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_postfix_expression_part_details_list_size(&state->postfix_expression_parts);
	struct postfix_expression_part_id id;
	buffered_printf(out, "There are %u postfix_expression_parts:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_postfix_expression_part(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_postfix_expression_part(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct postfix_expression_part_id id){
	unsigned int tabs_printed;
	struct postfix_expression_part_details details = struct_postfix_expression_part_details_list_get(&state->postfix_expression_parts, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_POSTFIX_EXPRESSION_PART_ARRAY_DEREFERENCE:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"postfix_expression_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"postfix_type\" : \"array_dereference\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_STRUCT_DOT_DEREFERENCE:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"postfix_expression_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"postfix_type\" : \"dot_dereference\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : ");
			print_identifier(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_STRUCT_ARROW_DEREFERENCE:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"postfix_expression_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"postfix_type\" : \"arrow_dereference\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : ");
			print_identifier(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_FUNCTION_DEREFERENCE:{
			struct any_statement_list_item_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"postfix_expression_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"postfix_type\" : \"function_dereference\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statements\" : ");
			print_any_statement_list_item(state, indent_depth + 1, out, id_tmp, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_INC:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"postfix_expression_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"postfix_type\" : \"increment\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_POSTFIX_EXPRESSION_PART_DEC:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"postfix_expression_part\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"postfix_type\" : \"decrement\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct unscoped_tag_specifier_details get_unscoped_tag_specifier_details_by_id(struct asm_state * s, struct unscoped_tag_specifier_id id){
	return struct_unscoped_tag_specifier_details_list_get(&s->unscoped_tag_specifiers, id.id);
}
void print_all_unscoped_tag_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_unscoped_tag_specifier_details_list_size(&state->unscoped_tag_specifiers);
	struct unscoped_tag_specifier_id id;
	buffered_printf(out, "There are %u unscoped_tag_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_unscoped_tag_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_unscoped_tag_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct unscoped_tag_specifier_id id){
	unsigned int tabs_printed;
	struct unscoped_tag_specifier_details details = struct_unscoped_tag_specifier_details_list_get(&state->unscoped_tag_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_UNSCOPED_TAG_SPECIFIER_STRUCT:{
			struct unscoped_struct_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_unscoped_struct_specifier(state, indent_depth, out, id_tmp);
			break;
		}case ASM_UNSCOPED_TAG_SPECIFIER_UNION:{
			struct unscoped_union_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_unscoped_union_specifier(state, indent_depth, out, id_tmp);
			break;
		}case ASM_UNSCOPED_TAG_SPECIFIER_ENUM:{
			struct unscoped_enum_specifier_id id_tmp;
			id_tmp.id = details.id;
			print_unscoped_enum_specifier(state, indent_depth, out, id_tmp);
			break;
		}
	}
}

struct scoped_typename_specifier_details get_scoped_typename_specifier_details_by_id(struct asm_state * s, struct scoped_typename_specifier_id id){
	return struct_scoped_typename_specifier_details_list_get(&s->scoped_typename_specifiers, id.id);
}
void print_all_scoped_typename_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_scoped_typename_specifier_details_list_size(&state->scoped_typename_specifiers);
	struct scoped_typename_specifier_id id;
	buffered_printf(out, "There are %u scoped_typename_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_scoped_typename_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_scoped_typename_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct scoped_typename_specifier_id id){
	unsigned int tabs_printed;
	struct scoped_typename_specifier_details details = struct_scoped_typename_specifier_details_list_get(&state->scoped_typename_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SCOPED_TYPENAME_SPECIFIER_SCOPED_TYPENAME:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"scoped_typename_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			print_scope_guid(state, indent_depth + 1, out, details.scope_guid_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, details.identifier_id);
			buffered_printf(out, "\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct scoped_tag_specifier_details get_scoped_tag_specifier_details_by_id(struct asm_state * s, struct scoped_tag_specifier_id id){
	return struct_scoped_tag_specifier_details_list_get(&s->scoped_tag_specifiers, id.id);
}
void print_all_scoped_tag_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_scoped_tag_specifier_details_list_size(&state->scoped_tag_specifiers);
	struct scoped_tag_specifier_id id;
	buffered_printf(out, "There are %u scoped_tag_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_scoped_tag_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_scoped_tag_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct scoped_tag_specifier_id id){
	unsigned int tabs_printed;
	struct scoped_tag_specifier_details details = struct_scoped_tag_specifier_details_list_get(&state->scoped_tag_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SCOPED_TAG_SPECIFIER_SCOPED_TAG_SPECIFIER:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"scoped_tag_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.scope_guid_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"tag\" : ");
			print_unscoped_tag_specifier(state, indent_depth + 1, out, details.unscoped_tag_specifier_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct unscoped_union_specifier_details get_unscoped_union_specifier_details_by_id(struct asm_state * s, struct unscoped_union_specifier_id id){
	return struct_unscoped_union_specifier_details_list_get(&s->unscoped_union_specifiers, id.id);
}
void print_all_unscoped_union_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_unscoped_union_specifier_details_list_size(&state->unscoped_union_specifiers);
	struct unscoped_union_specifier_id id;
	buffered_printf(out, "There are %u unscoped_union_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_unscoped_union_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_unscoped_union_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct unscoped_union_specifier_id id){
	unsigned int tabs_printed;
	struct unscoped_union_specifier_details details = struct_unscoped_union_specifier_details_list_get(&state->unscoped_union_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_UNSCOPED_UNION_SPECIFIER_NAMED:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unscoped_union_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth, out, id_tmp);
			buffered_printf(out, "\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNSCOPED_UNION_SPECIFIER_ANONYMOUS:{
			struct anonymous_entry_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unscoped_union_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"anonymous_entry_id\" : ");
			print_anonymous_entry(state, indent_depth, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct unscoped_struct_specifier_details get_unscoped_struct_specifier_details_by_id(struct asm_state * s, struct unscoped_struct_specifier_id id){
	return struct_unscoped_struct_specifier_details_list_get(&s->unscoped_struct_specifiers, id.id);
}
void print_all_unscoped_struct_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_unscoped_struct_specifier_details_list_size(&state->unscoped_struct_specifiers);
	struct unscoped_struct_specifier_id id;
	buffered_printf(out, "There are %u unscoped_struct_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_unscoped_struct_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_unscoped_struct_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct unscoped_struct_specifier_id id){
	unsigned int tabs_printed;
	struct unscoped_struct_specifier_details details = struct_unscoped_struct_specifier_details_list_get(&state->unscoped_struct_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_UNSCOPED_STRUCT_SPECIFIER_NAMED:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unscoped_struct_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth, out, id_tmp);
			buffered_printf(out, "\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNSCOPED_STRUCT_SPECIFIER_ANONYMOUS:{
			struct anonymous_entry_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unscoped_struct_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"anonymous_entry_id\" : ");
			print_anonymous_entry(state, indent_depth, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct unscoped_enum_specifier_details get_unscoped_enum_specifier_details_by_id(struct asm_state * s, struct unscoped_enum_specifier_id id){
	return struct_unscoped_enum_specifier_details_list_get(&s->unscoped_enum_specifiers, id.id);
}
void print_all_unscoped_enum_specifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_unscoped_enum_specifier_details_list_size(&state->unscoped_enum_specifiers);
	struct unscoped_enum_specifier_id id;
	buffered_printf(out, "There are %u unscoped_enum_specifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_unscoped_enum_specifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_unscoped_enum_specifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct unscoped_enum_specifier_id id){
	unsigned int tabs_printed;
	struct unscoped_enum_specifier_details details = struct_unscoped_enum_specifier_details_list_get(&state->unscoped_enum_specifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_UNSCOPED_ENUM_SPECIFIER_NAMED:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unscoped_enum_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth, out, id_tmp);
			buffered_printf(out, "\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNSCOPED_ENUM_SPECIFIER_ANONYMOUS:{
			struct anonymous_entry_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unscoped_enum_specifier\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"anonymous_entry_id\" : ");
			print_anonymous_entry(state, indent_depth, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct bitfield_details get_bitfield_details_by_id(struct asm_state * s, struct bitfield_id id){
	return struct_bitfield_details_list_get(&s->bitfields, id.id);
}
void print_all_bitfields(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_bitfield_details_list_size(&state->bitfields);
	struct bitfield_id id;
	buffered_printf(out, "There are %u bitfields:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_bitfield(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_bitfield(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct bitfield_id id){
	unsigned int tabs_printed;
	struct bitfield_details details = struct_bitfield_details_list_get(&state->bitfields, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_BITFIELD_NO_BITFIELD:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"bitfield\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"no_bitfield\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BITFIELD_CONSTANT_BITFIELD:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"bitfield\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"bitfield\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"bitfield\" : ");
			print_expression(state, indent_depth + 1, out, details.expression_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct jump_statement_details get_jump_statement_details_by_id(struct asm_state * s, struct jump_statement_id id){
	return struct_jump_statement_details_list_get(&s->jump_statements, id.id);
}
void print_all_jump_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_jump_statement_details_list_size(&state->jump_statements);
	struct jump_statement_id id;
	buffered_printf(out, "There are %u jump_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_jump_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_jump_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct jump_statement_id id){
	unsigned int tabs_printed;
	struct jump_statement_details details = struct_jump_statement_details_list_get(&state->jump_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_JUMP_STATEMENT_RETURN_EXPRESSION:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"jump_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"return_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_JUMP_STATEMENT_RETURN_NO_EXPRESSION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"jump_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"return_no_expression\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_JUMP_STATEMENT_BREAK:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"jump_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"break\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_JUMP_STATEMENT_CONTINUE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"jump_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"continue\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_JUMP_STATEMENT_GOTO:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"jump_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"goto\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct switch_statement_details get_switch_statement_details_by_id(struct asm_state * s, struct switch_statement_id id){
	return struct_switch_statement_details_list_get(&s->switch_statements, id.id);
}
void print_all_switch_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_switch_statement_details_list_size(&state->switch_statements);
	struct switch_statement_id id;
	buffered_printf(out, "There are %u switch_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_switch_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_switch_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct switch_statement_id id){
	unsigned int tabs_printed;
	struct switch_statement_details details = struct_switch_statement_details_list_get(&state->switch_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SWITCH_STATEMENT_SWITCH:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"switch_statement\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.s1);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct if_statement_details get_if_statement_details_by_id(struct asm_state * s, struct if_statement_id id){
	return struct_if_statement_details_list_get(&s->if_statements, id.id);
}
void print_all_if_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_if_statement_details_list_size(&state->if_statements);
	struct if_statement_id id;
	buffered_printf(out, "There are %u if_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_if_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_if_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct if_statement_id id){
	unsigned int tabs_printed;
	struct if_statement_details details = struct_if_statement_details_list_get(&state->if_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_IF_STATEMENT_JUST_IF:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"if_statement\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"just_if\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement0\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.s1);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_IF_STATEMENT_IF_ELSE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"if_statement\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"if_else\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement0\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.s1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement1\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.s2);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct while_statement_details get_while_statement_details_by_id(struct asm_state * s, struct while_statement_id id){
	return struct_while_statement_details_list_get(&s->while_statements, id.id);
}
void print_all_while_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_while_statement_details_list_size(&state->while_statements);
	struct while_statement_id id;
	buffered_printf(out, "There are %u while_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_while_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_while_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct while_statement_id id){
	unsigned int tabs_printed;
	struct while_statement_details details = struct_while_statement_details_list_get(&state->while_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_WHILE_STATEMENT_DO_WHILE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"while_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"do_while\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.s1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			break;
		}case ASM_WHILE_STATEMENT_WHILE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"while_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"do_while\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.s1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			break;
		}
	}
}

struct for_statement_details get_for_statement_details_by_id(struct asm_state * s, struct for_statement_id id){
	return struct_for_statement_details_list_get(&s->for_statements, id.id);
}
void print_all_for_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_for_statement_details_list_size(&state->for_statements);
	struct for_statement_id id;
	buffered_printf(out, "There are %u for_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_for_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_for_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct for_statement_id id){
	unsigned int tabs_printed;
	struct for_statement_details details = struct_for_statement_details_list_get(&state->for_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_FOR_STATEMENT_FOR:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"for_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement1\" : ");
			print_any_statement(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement2\" : ");
			print_any_statement(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement3\" : ");
			print_any_statement(state, indent_depth + 1, out, details.e3);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"body\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.s1);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			break;
		}
	}
}

struct code_statement_details get_code_statement_details_by_id(struct asm_state * s, struct code_statement_id id){
	return struct_code_statement_details_list_get(&s->code_statements, id.id);
}
void print_all_code_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_code_statement_details_list_size(&state->code_statements);
	struct code_statement_id id;
	buffered_printf(out, "There are %u code_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_code_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_code_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct code_statement_id id){
	unsigned int tabs_printed;
	struct code_statement_details details = struct_code_statement_details_list_get(&state->code_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_CODE_STATEMENT_LABELED_STATEMENT:{
			struct labeled_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"code_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_labeled_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_CODE_STATEMENT_COMPOUND_STATEMENT:{
			struct any_statement_list_item_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"code_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_any_statement_list_item(state, indent_depth + 1, out, id_tmp, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_CODE_STATEMENT_EXPRESSION_STATEMENT:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"code_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_CODE_STATEMENT_FOR_STATEMENT:{
			struct for_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"code_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_for_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_CODE_STATEMENT_WHILE_STATEMENT:{
			struct while_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"code_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_while_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_CODE_STATEMENT_IF_STATEMENT:{
			struct if_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"code_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_if_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_CODE_STATEMENT_SWITCH_STATEMENT:{
			struct switch_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"code_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_switch_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_CODE_STATEMENT_JUMP_STATEMENT:{
			struct jump_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"code_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_jump_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct labeled_statement_details get_labeled_statement_details_by_id(struct asm_state * s, struct labeled_statement_id id){
	return struct_labeled_statement_details_list_get(&s->labeled_statements, id.id);
}
void print_all_labeled_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_labeled_statement_details_list_size(&state->labeled_statements);
	struct labeled_statement_id id;
	buffered_printf(out, "There are %u labeled_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_labeled_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_labeled_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct labeled_statement_id id){
	unsigned int tabs_printed;
	struct labeled_statement_details details = struct_labeled_statement_details_list_get(&state->labeled_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_LABELED_STATEMENT_GOTO_LABEL:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"labeled_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"goto\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.scoped_statement_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_LABELED_STATEMENT_CASE_LABEL:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"labeled_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"case\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.scoped_statement_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_LABELED_STATEMENT_DEFAULT_LABEL:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"labeled_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"default\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.scoped_statement_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct postfix_expression_details get_postfix_expression_details_by_id(struct asm_state * s, struct postfix_expression_id id){
	return struct_postfix_expression_details_list_get(&s->postfix_expressions, id.id);
}
void print_all_postfix_expressions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_postfix_expression_details_list_size(&state->postfix_expressions);
	struct postfix_expression_id id;
	buffered_printf(out, "There are %u postfix_expressions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_postfix_expression(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_postfix_expression(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct postfix_expression_id id){
	unsigned int tabs_printed;
	struct postfix_expression_details details = struct_postfix_expression_details_list_get(&state->postfix_expressions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_POSTFIX_EXPRESSION_POSTFIX_EXPRESSION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"postfix_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_primary_expression(state, indent_depth + 1, out, details.primary_expression_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"postfix_expression_parts\" : ");
			print_postfix_expression_part_list_item(state, indent_depth + 1, out, details.postfix_expression_part_list_item_id, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct primary_expression_details get_primary_expression_details_by_id(struct asm_state * s, struct primary_expression_id id){
	return struct_primary_expression_details_list_get(&s->primary_expressions, id.id);
}
void print_all_primary_expressions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_primary_expression_details_list_size(&state->primary_expressions);
	struct primary_expression_id id;
	buffered_printf(out, "There are %u primary_expressions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_primary_expression(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_primary_expression(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct primary_expression_id id){
	unsigned int tabs_printed;
	struct primary_expression_details details = struct_primary_expression_details_list_get(&state->primary_expressions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_PRIMARY_EXPRESSION_CONSTANT_DESCRIPTION:{
			struct constant_description_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"primary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"constant\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value\" : ");
			print_constant_description(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_PRIMARY_EXPRESSION_IDENTIFIER:{
			struct identifier_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"primary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"variable\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.scope_guid_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_PRIMARY_EXPRESSION_EXPRESSION:{
			struct expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"primary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct any_statement_details get_any_statement_details_by_id(struct asm_state * s, struct any_statement_id id){
	return struct_any_statement_details_list_get(&s->any_statements, id.id);
}
void print_all_any_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_any_statement_details_list_size(&state->any_statements);
	struct any_statement_id id;
	buffered_printf(out, "There are %u any_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_any_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_any_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct any_statement_id id){
	unsigned int tabs_printed;
	struct any_statement_details details = struct_any_statement_details_list_get(&state->any_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_ANY_STATEMENT_DECLARATION_STATEMENT:{
			struct declaration_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"any_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_declaration_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_ANY_STATEMENT_SCOPED_STATEMENT:{
			struct scoped_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"any_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_scoped_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_ANY_STATEMENT_CODE_STATEMENT:{
			struct code_statement_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"any_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"statement\" : ");
			print_code_statement(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct scoped_statement_details get_scoped_statement_details_by_id(struct asm_state * s, struct scoped_statement_id id){
	return struct_scoped_statement_details_list_get(&s->scoped_statements, id.id);
}
void print_all_scoped_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_scoped_statement_details_list_size(&state->scoped_statements);
	struct scoped_statement_id id;
	buffered_printf(out, "There are %u scoped_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_scoped_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_scoped_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct scoped_statement_id id){
	unsigned int tabs_printed;
	struct scoped_statement_details details = struct_scoped_statement_details_list_get(&state->scoped_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SCOPED_STATEMENT_SCOPED_STATEMENT:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"scoped_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"scope_guid\" : ");
			buffered_printf(out, "%u", details.scope_guid_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"any_statements\" : ");
			print_any_statement_list_item(state, indent_depth + 1, out, details.any_statement_list_item_id, 0);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct declaration_statement_details get_declaration_statement_details_by_id(struct asm_state * s, struct declaration_statement_id id){
	return struct_declaration_statement_details_list_get(&s->declaration_statements, id.id);
}
void print_all_declaration_statements(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_declaration_statement_details_list_size(&state->declaration_statements);
	struct declaration_statement_id id;
	buffered_printf(out, "There are %u declaration_statements:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_declaration_statement(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_declaration_statement(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct declaration_statement_id id){
	unsigned int tabs_printed;
	struct declaration_statement_details details = struct_declaration_statement_details_list_get(&state->declaration_statements, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_DECLARATION_STATEMENT_SIMPLE_DECLARATION:{
			struct simple_declaration_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"declaration_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"declaration\" : ");
			print_simple_declaration(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DECLARATION_STATEMENT_FUNCTION_DEFINITION:{
			struct function_definition_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"declaration_statement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"function_definition\" : ");
			print_function_definition(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct function_definition_details get_function_definition_details_by_id(struct asm_state * s, struct function_definition_id id){
	return struct_function_definition_details_list_get(&s->function_definitions, id.id);
}
void print_all_function_definitions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_function_definition_details_list_size(&state->function_definitions);
	struct function_definition_id id;
	buffered_printf(out, "There are %u function_definitions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_function_definition(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_function_definition(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct function_definition_id id){
	unsigned int tabs_printed;
	struct function_definition_details details = struct_function_definition_details_list_get(&state->function_definitions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_FUNCTION_DEFINITION_FUNCTION_DEFINITION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"node_type\" : \"function_definition\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, details.identifier_id);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"function_type\" : ");
			print_general_type(state, indent_depth + 1, out, details.general_type_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"k_r_declarations\" : ");
			print_any_statement_list_item(state, indent_depth + 1, out, details.k_and_r_declarations, 0);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"parameters\" : ");
			print_any_statement_list_item(state, indent_depth + 1, out, details.params, 0);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "	\"body\" : ");
			print_scoped_statement(state, indent_depth + 1, out, details.body);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct simple_declaration_details get_simple_declaration_details_by_id(struct asm_state * s, struct simple_declaration_id id){
	return struct_simple_declaration_details_list_get(&s->simple_declarations, id.id);
}
void print_all_simple_declarations(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_simple_declaration_details_list_size(&state->simple_declarations);
	struct simple_declaration_id id;
	buffered_printf(out, "There are %u simple_declarations:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_simple_declaration(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_simple_declaration(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct simple_declaration_id id){
	unsigned int tabs_printed;
	struct simple_declaration_details details = struct_simple_declaration_details_list_get(&state->simple_declarations, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SIMPLE_DECLARATION_SIMPLE_DECLARATION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"simple_declaration\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"type\" : ");
			print_general_type(state, indent_depth + 1, out, details.general_type_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"identifier\" : \"");
			print_identifier(state, indent_depth + 1, out, details.identifier_id);
			buffered_printf(out, "\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"initializer\" : \"");
			print_initializer(state, indent_depth + 1, out, details.initializer_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct expression_details get_expression_details_by_id(struct asm_state * s, struct expression_id id){
	return struct_expression_details_list_get(&s->expressions, id.id);
}
void print_all_expressions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_expression_details_list_size(&state->expressions);
	struct expression_id id;
	buffered_printf(out, "There are %u expressions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_expression(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_expression(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct expression_id id){
	unsigned int tabs_printed;
	struct expression_details details = struct_expression_details_list_get(&state->expressions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_EXPRESSION_NULLARY_EXPRESSION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			buffered_printf(out, "\t{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\t\"node_type\" : \"nullary_expression\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t}\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_EXPRESSION_CAST_EXPRESSION:{
			struct cast_expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_cast_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_EXPRESSION_BINARY_EXPRESSION:{
			struct binary_expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_binary_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_EXPRESSION_TERNARY_EXPRESSION:{
			struct ternary_expression_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_ternary_expression(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct cast_expression_details get_cast_expression_details_by_id(struct asm_state * s, struct cast_expression_id id){
	return struct_cast_expression_details_list_get(&s->cast_expressions, id.id);
}
void print_all_cast_expressions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_cast_expression_details_list_size(&state->cast_expressions);
	struct cast_expression_id id;
	buffered_printf(out, "There are %u cast_expressions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_cast_expression(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_cast_expression(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct cast_expression_id id){
	unsigned int tabs_printed;
	struct cast_expression_details details = struct_cast_expression_details_list_get(&state->cast_expressions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_CAST_EXPRESSION_HAS_CAST:{
			struct cast_expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"cast_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"target_type\" : ");
			print_general_type(state, indent_depth + 1, out, details.id2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_cast_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_CAST_EXPRESSION_NO_CAST:{
			struct unary_expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"cast_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_unary_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct unary_expression_details get_unary_expression_details_by_id(struct asm_state * s, struct unary_expression_id id){
	return struct_unary_expression_details_list_get(&s->unary_expressions, id.id);
}
void print_all_unary_expressions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_unary_expression_details_list_size(&state->unary_expressions);
	struct unary_expression_id id;
	buffered_printf(out, "There are %u unary_expressions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_unary_expression(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_unary_expression(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct unary_expression_id id){
	unsigned int tabs_printed;
	struct unary_expression_details details = struct_unary_expression_details_list_get(&state->unary_expressions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_UNARY_EXPRESSION_SIZEOF_EXPRESSION:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"sizeof_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_POSTFIX_EXPRESSION:{
			struct postfix_expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"postfix_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_postfix_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_SIZEOF_TYPE:{
			struct general_type_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"sizeof_type\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"type\" : ");
			print_general_type(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_INCREMENT:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"increment\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_DECREMENT:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"decrement\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_ADDRESS_OF:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"address_of\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_POINT_TO:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"point_to\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_PLUS:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"positive\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_MINUS:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"negative\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_BITWISE_NOT:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"bitwise_not\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_UNARY_EXPRESSION_LOGICAL_NOT:{
			struct expression_id id1_tmp;
			id1_tmp.id = details.id1;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"unary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"kind\" : \"logical_not\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression\" : ");
			print_expression(state, indent_depth + 1, out, id1_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct binary_expression_details get_binary_expression_details_by_id(struct asm_state * s, struct binary_expression_id id){
	return struct_binary_expression_details_list_get(&s->binary_expressions, id.id);
}
void print_all_binary_expressions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_binary_expression_details_list_size(&state->binary_expressions);
	struct binary_expression_id id;
	buffered_printf(out, "There are %u binary_expressions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_binary_expression(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_binary_expression(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct binary_expression_id id){
	unsigned int tabs_printed;
	struct binary_expression_details details = struct_binary_expression_details_list_get(&state->binary_expressions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_BINARY_EXPRESSION_COMMA:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"comma\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_PLUS:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"plus\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_MINUS:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"minus\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_DIVIDE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"divide\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_MULTIPLY:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"multiply\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_EXCLUSIVE_OR:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"exclusive_or\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_MODULUS:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"modulus\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_RIGHT:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"right\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_LEFT:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"left\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_BITWISE_OR:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"bitwise_or\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_LOGICAL_OR:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"logical_or\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_BITWISE_AND:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"bitwise_and\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_LOGICAL_AND:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"logical_and\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_LT:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"less_than\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_GT:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"greater_than\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_LE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"less_than_or_equal\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_GE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"greater_than_or_equal\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_EQ:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"equal_to\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_NE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"not_equal_to\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_ADD_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"add_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_SUB_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"sub_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_DIV_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"div_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_MUL_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"mul_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_AND_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"and_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_OR_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"or_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_XOR_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"xor_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_MOD_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"mod_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_RIGHT_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"right_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_BINARY_EXPRESSION_LEFT_ASSIGN:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"binary_expression\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression1\" : ");
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"expression2\" : ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"operation\" : \"left_assign\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct ternary_expression_details get_ternary_expression_details_by_id(struct asm_state * s, struct ternary_expression_id id){
	return struct_ternary_expression_details_list_get(&s->ternary_expressions, id.id);
}
void print_all_ternary_expressions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_ternary_expression_details_list_size(&state->ternary_expressions);
	struct ternary_expression_id id;
	buffered_printf(out, "There are %u ternary_expressions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_ternary_expression(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_ternary_expression(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct ternary_expression_id id){
	unsigned int tabs_printed;
	struct ternary_expression_details details = struct_ternary_expression_details_list_get(&state->ternary_expressions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_TERNARY_EXPRESSION_TERNARY_OPERATOR:{
			print_expression(state, indent_depth + 1, out, details.e1);
			buffered_printf(out, " ? ");
			print_expression(state, indent_depth + 1, out, details.e2);
			buffered_printf(out, " : ");
			print_expression(state, indent_depth + 1, out, details.e3);
			break;
		}
	}
}

struct constant_description_details get_constant_description_details_by_id(struct asm_state * s, struct constant_description_id id){
	return struct_constant_description_details_list_get(&s->constant_descriptions, id.id);
}
void print_all_constant_descriptions(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_constant_description_details_list_size(&state->constant_descriptions);
	struct constant_description_id id;
	buffered_printf(out, "There are %u constant_descriptions:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_constant_description(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_constant_description(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct constant_description_id id){
	unsigned int tabs_printed;
	struct constant_description_details details = struct_constant_description_details_list_get(&state->constant_descriptions, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_CONSTANT_DESCRIPTION_CONSTANT_DESCRIPTION:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"constant_description\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"type\" : ");
			print_general_type(state, indent_depth + 1, out, details.general_type_id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"data_package\" : ");
			print_data_package(state, indent_depth + 1, out, details.data_package_id);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct data_package_details get_data_package_details_by_id(struct asm_state * s, struct data_package_id id){
	return struct_data_package_details_list_get(&s->data_packages, id.id);
}
void print_all_data_packages(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_data_package_details_list_size(&state->data_packages);
	struct data_package_id id;
	buffered_printf(out, "There are %u data_packages:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_data_package(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_data_package(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct data_package_id id){
	unsigned int tabs_printed;
	struct data_package_details details = struct_data_package_details_list_get(&state->data_packages, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_DATA_PACKAGE_ERROR:{
			struct error_node_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"data_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"error\" : \"");
			print_error_node(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DATA_PACKAGE_BYTE:{
			struct byte_package_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"data_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"data\" : ");
			print_byte_package(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DATA_PACKAGE_HWORD:{
			struct hword_package_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"data_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"data\" : ");
			print_hword_package(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DATA_PACKAGE_WORD:{
			struct word_package_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"data_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"data\" : ");
			print_word_package(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DATA_PACKAGE_DWORD:{
			struct dword_package_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"data_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"data\" : ");
			print_dword_package(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DATA_PACKAGE_QWORD:{
			struct qword_package_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"data_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"data\" : ");
			print_qword_package(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_DATA_PACKAGE_STRING:{
			struct string_package_id id_tmp;
			id_tmp.id = details.id;
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"data_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"data\" : ");
			print_string_package(state, indent_depth + 1, out, id_tmp);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct error_node_details get_error_node_details_by_id(struct asm_state * s, struct error_node_id id){
	return struct_error_node_details_list_get(&s->error_nodes, id.id);
}
void print_all_error_nodes(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_error_node_details_list_size(&state->error_nodes);
	struct error_node_id id;
	buffered_printf(out, "There are %u error_nodes:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_error_node(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_error_node(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct error_node_id id){
	unsigned int tabs_printed;
	struct error_node_details details = struct_error_node_details_list_get(&state->error_nodes, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
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

struct byte_package_details get_byte_package_details_by_id(struct asm_state * s, struct byte_package_id id){
	return struct_byte_package_details_list_get(&s->byte_packages, id.id);
}
void print_all_byte_packages(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_byte_package_details_list_size(&state->byte_packages);
	struct byte_package_id id;
	buffered_printf(out, "There are %u byte_packages:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_byte_package(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_byte_package(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct byte_package_id id){
	unsigned int tabs_printed;
	struct byte_package_details details = struct_byte_package_details_list_get(&state->byte_packages, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_BYTE_PACKAGE_BYTE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"byte_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value_0\" : ");
			buffered_printf(out, "\n");
			print_terminal_byte_package_byte_data(state, indent_depth + 1, out, details.data);
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct hword_package_details get_hword_package_details_by_id(struct asm_state * s, struct hword_package_id id){
	return struct_hword_package_details_list_get(&s->hword_packages, id.id);
}
void print_all_hword_packages(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_hword_package_details_list_size(&state->hword_packages);
	struct hword_package_id id;
	buffered_printf(out, "There are %u hword_packages:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_hword_package(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_hword_package(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct hword_package_id id){
	unsigned int tabs_printed;
	struct hword_package_details details = struct_hword_package_details_list_get(&state->hword_packages, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_HWORD_PACKAGE_HWORD:{
			print_terminal_hword_package_hword_data0(state, indent_depth + 1, out, details.data0);
			print_terminal_hword_package_hword_data1(state, indent_depth + 1, out, details.data1);
			break;
		}
	}
}

struct word_package_details get_word_package_details_by_id(struct asm_state * s, struct word_package_id id){
	return struct_word_package_details_list_get(&s->word_packages, id.id);
}
void print_all_word_packages(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_word_package_details_list_size(&state->word_packages);
	struct word_package_id id;
	buffered_printf(out, "There are %u word_packages:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_word_package(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_word_package(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct word_package_id id){
	unsigned int tabs_printed;
	struct word_package_details details = struct_word_package_details_list_get(&state->word_packages, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_WORD_PACKAGE_WORD:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"word_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value_0\" : ");
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct qword_package_details get_qword_package_details_by_id(struct asm_state * s, struct qword_package_id id){
	return struct_qword_package_details_list_get(&s->qword_packages, id.id);
}
void print_all_qword_packages(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_qword_package_details_list_size(&state->qword_packages);
	struct qword_package_id id;
	buffered_printf(out, "There are %u qword_packages:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_qword_package(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_qword_package(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct qword_package_id id){
	unsigned int tabs_printed;
	struct qword_package_details details = struct_qword_package_details_list_get(&state->qword_packages, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_QWORD_PACKAGE_QWORD:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"qword_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value_0\" : ");
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data0);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value_1\" : ");
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data1);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value_2\" : ");
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data2);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value_3\" : ");
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data3);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct dword_package_details get_dword_package_details_by_id(struct asm_state * s, struct dword_package_id id){
	return struct_dword_package_details_list_get(&s->dword_packages, id.id);
}
void print_all_dword_packages(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_dword_package_details_list_size(&state->dword_packages);
	struct dword_package_id id;
	buffered_printf(out, "There are %u dword_packages:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_dword_package(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_dword_package(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct dword_package_id id){
	unsigned int tabs_printed;
	struct dword_package_details details = struct_dword_package_details_list_get(&state->dword_packages, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_DWORD_PACKAGE_DWORD:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"dword_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value_0\" : ");
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data0);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"value_1\" : ");
			buffered_printf(out, "0x");
			buffered_printf(out, "%08X", details.data1);
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct string_package_details get_string_package_details_by_id(struct asm_state * s, struct string_package_id id){
	return struct_string_package_details_list_get(&s->string_packages, id.id);
}
void print_all_string_packages(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_string_package_details_list_size(&state->string_packages);
	struct string_package_id id;
	buffered_printf(out, "There are %u string_packages:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_string_package(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_string_package(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct string_package_id id){
	unsigned int tabs_printed;
	struct string_package_details details = struct_string_package_details_list_get(&state->string_packages, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_STRING_PACKAGE_STRING:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"node_type\" : \"string_package\",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"byte_length\" : ");
			buffered_printf(out, "%u", details.byte_length);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"word_length\" : ");
			buffered_printf(out, "%u", details.word_length);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"string_data\" : ");
					buffered_printf(out, "[");
			for(i = 0; i < details.byte_length; i++){
				syntax_model_print_char(out, ((unsigned char *)(&details.data[i / sizeof(unsigned int)]))[i % sizeof(unsigned int)]);
				if(i != (details.byte_length -1)){
					buffered_printf(out, ", ");
				}
			}
			buffered_printf(out, "]");
			buffered_printf(out, "\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct identifier_details get_identifier_details_by_id(struct asm_state * s, struct identifier_id id){
	return struct_identifier_details_list_get(&s->identifiers, id.id);
}
void print_all_identifiers(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_identifier_details_list_size(&state->identifiers);
	struct identifier_id id;
	buffered_printf(out, "There are %u identifiers:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_identifier(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_identifier(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct identifier_id id){
	unsigned int tabs_printed;
	struct identifier_details details = struct_identifier_details_list_get(&state->identifiers, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_IDENTIFIER_IDENTIFIER:{
			add_string_to_buffer(out, details.identifier->first_byte, details.identifier->last_byte);
			break;
		}
	}
}

struct scope_guid_details get_scope_guid_details_by_id(struct asm_state * s, struct scope_guid_id id){
	return struct_scope_guid_details_list_get(&s->scope_guids, id.id);
}
void print_all_scope_guids(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_scope_guid_details_list_size(&state->scope_guids);
	struct scope_guid_id id;
	buffered_printf(out, "There are %u scope_guids:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_scope_guid(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_scope_guid(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct scope_guid_id id){
	unsigned int tabs_printed;
	struct scope_guid_details details = struct_scope_guid_details_list_get(&state->scope_guids, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_SCOPE_GUID_BLOCK_SCOPE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"parent\" : ");
			buffered_printf(out, "%u", details.parent);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_type\" : \"BLOCK_SCOPE\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_SCOPE_GUID_FILE_SCOPE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"parent\" : ");
			buffered_printf(out, "%u", details.parent);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_type\" : \"FILE_SCOPE\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_SCOPE_GUID_FUNCTION_SCOPE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"parent\" : ");
			buffered_printf(out, "%u", details.parent);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_type\" : \"FUNCTION_SCOPE\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}case ASM_SCOPE_GUID_FUNCTION_PROTOTYPE_SCOPE:{
			buffered_printf(out, "{\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_guid\" : ");
			buffered_printf(out, "%u", details.id);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"parent\" : ");
			buffered_printf(out, "%u", details.parent);
			buffered_printf(out, ",\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "\t\"scope_type\" : \"FUNCTION_PROTOTYPE_SCOPE\"\n");
			for(tabs_printed = 0; tabs_printed < indent_depth; tabs_printed++){
				buffered_printf(out, "\t");
			}
			buffered_printf(out, "}");
			break;
		}
	}
}

struct anonymous_entry_details get_anonymous_entry_details_by_id(struct asm_state * s, struct anonymous_entry_id id){
	return struct_anonymous_entry_details_list_get(&s->anonymous_entrys, id.id);
}
void print_all_anonymous_entrys(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out){
	unsigned int i;
	unsigned int size = struct_anonymous_entry_details_list_size(&state->anonymous_entrys);
	struct anonymous_entry_id id;
	buffered_printf(out, "There are %u anonymous_entrys:\n", size);
	for(i = 0; i < size; i++){
		id.id = i;
		buffered_printf(out, "%u) ", i);
		print_anonymous_entry(state, indent_depth, out, id);
		buffered_printf(out, "\n");
	}
}
void print_anonymous_entry(struct asm_state * state, unsigned int indent_depth, struct unsigned_char_list * out, struct anonymous_entry_id id){
	unsigned int tabs_printed;
	struct anonymous_entry_details details = struct_anonymous_entry_details_list_get(&state->anonymous_entrys, id.id);
	unsigned int i;
	(void)indent_depth;
	(void)tabs_printed;
	(void)i;
	switch(details.type){
		case ASM_ANONYMOUS_ENTRY_ANONYMOUS_ENTRY:{
			buffered_printf(out, "%u", details.id);
			break;
		}
	}
}

struct simple_type_specifier_id get_simple_type_specifier_id_simple_type_specifier_from_aggregate_type_specifier(struct aggregate_type_specifier_details details){
	struct simple_type_specifier_id id;
	id.id = details.id;
	return id;
}
struct scoped_tag_specifier_id get_scoped_tag_specifier_id_tag_specifier_from_aggregate_type_specifier(struct aggregate_type_specifier_details details){
	struct scoped_tag_specifier_id id;
	id.id = details.id;
	return id;
}
struct scoped_typename_specifier_id get_scoped_typename_specifier_id_typename_specifier_from_aggregate_type_specifier(struct aggregate_type_specifier_details details){
	struct scoped_typename_specifier_id id;
	id.id = details.id;
	return id;
}
struct simple_storage_class_specifier_id get_simple_storage_class_specifier_id_simple_storage_class_specifier_from_specifier_or_qualifier(struct specifier_or_qualifier_details details){
	struct simple_storage_class_specifier_id id;
	id.id = details.id;
	return id;
}
struct simple_type_qualifier_id get_simple_type_qualifier_id_simple_type_qualifier_from_specifier_or_qualifier(struct specifier_or_qualifier_details details){
	struct simple_type_qualifier_id id;
	id.id = details.id;
	return id;
}
struct aggregate_type_specifier_id get_aggregate_type_specifier_id_aggregate_type_specifier_from_specifier_or_qualifier(struct specifier_or_qualifier_details details){
	struct aggregate_type_specifier_id id;
	id.id = details.id;
	return id;
}
struct specifier_or_qualifier_list_item_id get_specifier_or_qualifier_list_item_prev_id_specifier_or_qualifier_from_specifier_or_qualifier_list_item(struct specifier_or_qualifier_list_item_details details){
	return details.prev_id;
}
struct specifier_or_qualifier_id get_specifier_or_qualifier_item_id_specifier_or_qualifier_from_specifier_or_qualifier_list_item(struct specifier_or_qualifier_list_item_details details){
	return details.item_id;
}
struct general_type_list_item_id get_general_type_list_item_general_type_list_item_id_variadic_from_parameter_list(struct parameter_list_details details){
	return details.general_type_list_item_id;
}
struct general_type_list_item_id get_general_type_list_item_general_type_list_item_id_non_variadic_from_parameter_list(struct parameter_list_details details){
	return details.general_type_list_item_id;
}
struct function_part_id get_function_part_id_function_from_declarator_part(struct declarator_part_details details){
	struct function_part_id id;
	id.id = details.id;
	return id;
}
struct pointer_part_id get_pointer_part_id_pointer_from_declarator_part(struct declarator_part_details details){
	struct pointer_part_id id;
	id.id = details.id;
	return id;
}
struct array_part_id get_array_part_id_array_from_declarator_part(struct declarator_part_details details){
	struct array_part_id id;
	id.id = details.id;
	return id;
}
struct any_statement_list_item_id get_any_statement_list_item_prev_id_any_statement_from_any_statement_list_item(struct any_statement_list_item_details details){
	return details.prev_id;
}
struct any_statement_id get_any_statement_item_id_any_statement_from_any_statement_list_item(struct any_statement_list_item_details details){
	return details.item_id;
}
struct tag_definition_id get_tag_definition_tag_definition_id_anonymous_from_anonymous_tag_definition(struct anonymous_tag_definition_details details){
	return details.tag_definition_id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_anonymous_from_anonymous_tag_definition(struct anonymous_tag_definition_details details){
	return details.scope_guid_id;
}
struct anonymous_entry_id get_anonymous_entry_anonymous_entry_id_anonymous_from_anonymous_tag_definition(struct anonymous_tag_definition_details details){
	return details.anonymous_entry_id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_named_from_named_tag_predeclaration(struct named_tag_predeclaration_details details){
	return details.scope_guid_id;
}
struct identifier_id get_identifier_identifier_id_named_from_named_tag_predeclaration(struct named_tag_predeclaration_details details){
	return details.identifier_id;
}
struct tag_definition_id get_tag_definition_tag_definition_id_named_from_named_tag_definition(struct named_tag_definition_details details){
	return details.tag_definition_id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_named_from_named_tag_definition(struct named_tag_definition_details details){
	return details.scope_guid_id;
}
struct identifier_id get_identifier_identifier_id_named_from_named_tag_definition(struct named_tag_definition_details details){
	return details.identifier_id;
}
struct enum_member_list_item_id get_enum_member_list_item_id_enum_from_tag_definition(struct tag_definition_details details){
	struct enum_member_list_item_id id;
	id.id = details.id;
	return id;
}
struct struct_or_union_member_list_item_id get_struct_or_union_member_list_item_id_struct_from_tag_definition(struct tag_definition_details details){
	struct struct_or_union_member_list_item_id id;
	id.id = details.id;
	return id;
}
struct struct_or_union_member_list_item_id get_struct_or_union_member_list_item_id_union_from_tag_definition(struct tag_definition_details details){
	struct struct_or_union_member_list_item_id id;
	id.id = details.id;
	return id;
}
struct struct_or_union_member_list_item_id get_struct_or_union_member_list_item_prev_id_member_from_struct_or_union_member_list_item(struct struct_or_union_member_list_item_details details){
	return details.prev_id;
}
struct struct_or_union_member_id get_struct_or_union_member_item_id_member_from_struct_or_union_member_list_item(struct struct_or_union_member_list_item_details details){
	return details.item_id;
}
struct enum_member_list_item_id get_enum_member_list_item_prev_id_member_from_enum_member_list_item(struct enum_member_list_item_details details){
	return details.prev_id;
}
struct enum_member_id get_enum_member_item_id_member_from_enum_member_list_item(struct enum_member_list_item_details details){
	return details.item_id;
}
struct identifier_id get_identifier_identifier_id_has_expression_from_enum_member(struct enum_member_details details){
	return details.identifier_id;
}
struct expression_id get_expression_expression_id_has_expression_from_enum_member(struct enum_member_details details){
	return details.expression_id;
}
struct identifier_id get_identifier_identifier_id_no_expression_from_enum_member(struct enum_member_details details){
	return details.identifier_id;
}
struct general_type_id get_general_type_general_type_id_member_from_struct_or_union_member(struct struct_or_union_member_details details){
	return details.general_type_id;
}
struct identifier_id get_identifier_identifier_id_member_from_struct_or_union_member(struct struct_or_union_member_details details){
	return details.identifier_id;
}
struct general_type_id get_general_type_general_type_id_padding_from_struct_or_union_member(struct struct_or_union_member_details details){
	return details.general_type_id;
}
struct general_type_id get_general_type_general_type_id_c11_anonymous_member_from_struct_or_union_member(struct struct_or_union_member_details details){
	return details.general_type_id;
}
struct initializer_list_item_id get_initializer_list_item_id_initializer_list_from_initializer(struct initializer_details details){
	struct initializer_list_item_id id;
	id.id = details.id;
	return id;
}
struct expression_id get_expression_id_expression_from_initializer(struct initializer_details details){
	struct expression_id id;
	id.id = details.id;
	return id;
}
struct initializer_list_item_id get_initializer_list_item_prev_id_initializer_from_initializer_list_item(struct initializer_list_item_details details){
	return details.prev_id;
}
struct initializer_id get_initializer_item_id_initializer_from_initializer_list_item(struct initializer_list_item_details details){
	return details.item_id;
}
struct declarator_part_list_item_id get_declarator_part_list_item_prev_id_declarator_part_from_declarator_part_list_item(struct declarator_part_list_item_details details){
	return details.prev_id;
}
struct declarator_part_id get_declarator_part_item_id_declarator_part_from_declarator_part_list_item(struct declarator_part_list_item_details details){
	return details.item_id;
}
struct specifier_or_qualifier_list_item_id get_specifier_or_qualifier_list_item_specifier_or_qualifier_list_item_id_pointer_from_pointer_part(struct pointer_part_details details){
	return details.specifier_or_qualifier_list_item_id;
}
struct parameter_list_id get_parameter_list_parameter_list_id_prototype_from_function_part(struct function_part_details details){
	return details.parameter_list_id;
}
struct parameter_list_id get_parameter_list_parameter_list_id_k_and_r_c_from_function_part(struct function_part_details details){
	return details.parameter_list_id;
}
struct expression_id get_expression_expression_id_expression_from_array_part(struct array_part_details details){
	return details.expression_id;
}
struct specifier_or_qualifier_list_item_id get_specifier_or_qualifier_list_item_specifier_or_qualifier_list_item_id_general_type_from_general_type(struct general_type_details details){
	return details.specifier_or_qualifier_list_item_id;
}
struct declarator_part_list_item_id get_declarator_part_list_item_declarator_part_list_item_id_general_type_from_general_type(struct general_type_details details){
	return details.declarator_part_list_item_id;
}
struct bitfield_id get_bitfield_bitfield_id_general_type_from_general_type(struct general_type_details details){
	return details.bitfield_id;
}
struct identifier_id get_identifier_identifier_id_typedef_from_identifier_namespace_entry(struct identifier_namespace_entry_details details){
	return details.identifier_id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_typedef_from_identifier_namespace_entry(struct identifier_namespace_entry_details details){
	return details.scope_guid_id;
}
struct general_type_id get_general_type_general_type_id_typedef_from_identifier_namespace_entry(struct identifier_namespace_entry_details details){
	return details.general_type_id;
}
struct identifier_id get_identifier_identifier_id_declaration_from_identifier_namespace_entry(struct identifier_namespace_entry_details details){
	return details.identifier_id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_declaration_from_identifier_namespace_entry(struct identifier_namespace_entry_details details){
	return details.scope_guid_id;
}
struct general_type_id get_general_type_general_type_id_declaration_from_identifier_namespace_entry(struct identifier_namespace_entry_details details){
	return details.general_type_id;
}
struct general_type_list_item_id get_general_type_list_item_prev_id_general_type_from_general_type_list_item(struct general_type_list_item_details details){
	return details.prev_id;
}
struct general_type_id get_general_type_item_id_general_type_from_general_type_list_item(struct general_type_list_item_details details){
	return details.item_id;
}
struct postfix_expression_part_list_item_id get_postfix_expression_part_list_item_prev_id_postfix_expression_part_from_postfix_expression_part_list_item(struct postfix_expression_part_list_item_details details){
	return details.prev_id;
}
struct postfix_expression_part_id get_postfix_expression_part_item_id_postfix_expression_part_from_postfix_expression_part_list_item(struct postfix_expression_part_list_item_details details){
	return details.item_id;
}
struct expression_id get_expression_id_array_dereference_from_postfix_expression_part(struct postfix_expression_part_details details){
	struct expression_id id;
	id.id = details.id;
	return id;
}
struct identifier_id get_identifier_id_struct_dot_dereference_from_postfix_expression_part(struct postfix_expression_part_details details){
	struct identifier_id id;
	id.id = details.id;
	return id;
}
struct identifier_id get_identifier_id_struct_arrow_dereference_from_postfix_expression_part(struct postfix_expression_part_details details){
	struct identifier_id id;
	id.id = details.id;
	return id;
}
struct any_statement_list_item_id get_any_statement_list_item_id_function_dereference_from_postfix_expression_part(struct postfix_expression_part_details details){
	struct any_statement_list_item_id id;
	id.id = details.id;
	return id;
}
struct unscoped_struct_specifier_id get_unscoped_struct_specifier_id_struct_from_unscoped_tag_specifier(struct unscoped_tag_specifier_details details){
	struct unscoped_struct_specifier_id id;
	id.id = details.id;
	return id;
}
struct unscoped_union_specifier_id get_unscoped_union_specifier_id_union_from_unscoped_tag_specifier(struct unscoped_tag_specifier_details details){
	struct unscoped_union_specifier_id id;
	id.id = details.id;
	return id;
}
struct unscoped_enum_specifier_id get_unscoped_enum_specifier_id_enum_from_unscoped_tag_specifier(struct unscoped_tag_specifier_details details){
	struct unscoped_enum_specifier_id id;
	id.id = details.id;
	return id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_scoped_typename_from_scoped_typename_specifier(struct scoped_typename_specifier_details details){
	return details.scope_guid_id;
}
struct identifier_id get_identifier_identifier_id_scoped_typename_from_scoped_typename_specifier(struct scoped_typename_specifier_details details){
	return details.identifier_id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_scoped_tag_specifier_from_scoped_tag_specifier(struct scoped_tag_specifier_details details){
	return details.scope_guid_id;
}
struct unscoped_tag_specifier_id get_unscoped_tag_specifier_unscoped_tag_specifier_id_scoped_tag_specifier_from_scoped_tag_specifier(struct scoped_tag_specifier_details details){
	return details.unscoped_tag_specifier_id;
}
struct identifier_id get_identifier_id_named_from_unscoped_union_specifier(struct unscoped_union_specifier_details details){
	struct identifier_id id;
	id.id = details.id;
	return id;
}
struct anonymous_entry_id get_anonymous_entry_id_anonymous_from_unscoped_union_specifier(struct unscoped_union_specifier_details details){
	struct anonymous_entry_id id;
	id.id = details.id;
	return id;
}
struct identifier_id get_identifier_id_named_from_unscoped_struct_specifier(struct unscoped_struct_specifier_details details){
	struct identifier_id id;
	id.id = details.id;
	return id;
}
struct anonymous_entry_id get_anonymous_entry_id_anonymous_from_unscoped_struct_specifier(struct unscoped_struct_specifier_details details){
	struct anonymous_entry_id id;
	id.id = details.id;
	return id;
}
struct identifier_id get_identifier_id_named_from_unscoped_enum_specifier(struct unscoped_enum_specifier_details details){
	struct identifier_id id;
	id.id = details.id;
	return id;
}
struct anonymous_entry_id get_anonymous_entry_id_anonymous_from_unscoped_enum_specifier(struct unscoped_enum_specifier_details details){
	struct anonymous_entry_id id;
	id.id = details.id;
	return id;
}
struct expression_id get_expression_expression_id_constant_bitfield_from_bitfield(struct bitfield_details details){
	return details.expression_id;
}
struct expression_id get_expression_id_return_expression_from_jump_statement(struct jump_statement_details details){
	struct expression_id id;
	id.id = details.id;
	return id;
}
struct identifier_id get_identifier_id_goto_from_jump_statement(struct jump_statement_details details){
	struct identifier_id id;
	id.id = details.id;
	return id;
}
struct expression_id get_expression_e1_switch_from_switch_statement(struct switch_statement_details details){
	return details.e1;
}
struct scoped_statement_id get_scoped_statement_s1_switch_from_switch_statement(struct switch_statement_details details){
	return details.s1;
}
struct scoped_statement_id get_scoped_statement_s1_just_if_from_if_statement(struct if_statement_details details){
	return details.s1;
}
struct expression_id get_expression_e1_just_if_from_if_statement(struct if_statement_details details){
	return details.e1;
}
struct scoped_statement_id get_scoped_statement_s1_if_else_from_if_statement(struct if_statement_details details){
	return details.s1;
}
struct expression_id get_expression_e1_if_else_from_if_statement(struct if_statement_details details){
	return details.e1;
}
struct scoped_statement_id get_scoped_statement_s2_if_else_from_if_statement(struct if_statement_details details){
	return details.s2;
}
struct expression_id get_expression_e1_do_while_from_while_statement(struct while_statement_details details){
	return details.e1;
}
struct scoped_statement_id get_scoped_statement_s1_do_while_from_while_statement(struct while_statement_details details){
	return details.s1;
}
struct expression_id get_expression_e1_while_from_while_statement(struct while_statement_details details){
	return details.e1;
}
struct scoped_statement_id get_scoped_statement_s1_while_from_while_statement(struct while_statement_details details){
	return details.s1;
}
struct any_statement_id get_any_statement_e1_for_from_for_statement(struct for_statement_details details){
	return details.e1;
}
struct any_statement_id get_any_statement_e2_for_from_for_statement(struct for_statement_details details){
	return details.e2;
}
struct any_statement_id get_any_statement_e3_for_from_for_statement(struct for_statement_details details){
	return details.e3;
}
struct scoped_statement_id get_scoped_statement_s1_for_from_for_statement(struct for_statement_details details){
	return details.s1;
}
struct labeled_statement_id get_labeled_statement_id_labeled_statement_from_code_statement(struct code_statement_details details){
	struct labeled_statement_id id;
	id.id = details.id;
	return id;
}
struct any_statement_list_item_id get_any_statement_list_item_id_compound_statement_from_code_statement(struct code_statement_details details){
	struct any_statement_list_item_id id;
	id.id = details.id;
	return id;
}
struct expression_id get_expression_id_expression_statement_from_code_statement(struct code_statement_details details){
	struct expression_id id;
	id.id = details.id;
	return id;
}
struct for_statement_id get_for_statement_id_for_statement_from_code_statement(struct code_statement_details details){
	struct for_statement_id id;
	id.id = details.id;
	return id;
}
struct while_statement_id get_while_statement_id_while_statement_from_code_statement(struct code_statement_details details){
	struct while_statement_id id;
	id.id = details.id;
	return id;
}
struct if_statement_id get_if_statement_id_if_statement_from_code_statement(struct code_statement_details details){
	struct if_statement_id id;
	id.id = details.id;
	return id;
}
struct switch_statement_id get_switch_statement_id_switch_statement_from_code_statement(struct code_statement_details details){
	struct switch_statement_id id;
	id.id = details.id;
	return id;
}
struct jump_statement_id get_jump_statement_id_jump_statement_from_code_statement(struct code_statement_details details){
	struct jump_statement_id id;
	id.id = details.id;
	return id;
}
struct scoped_statement_id get_scoped_statement_scoped_statement_id_goto_label_from_labeled_statement(struct labeled_statement_details details){
	return details.scoped_statement_id;
}
struct identifier_id get_identifier_id_goto_label_from_labeled_statement(struct labeled_statement_details details){
	struct identifier_id id;
	id.id = details.id;
	return id;
}
struct scoped_statement_id get_scoped_statement_scoped_statement_id_case_label_from_labeled_statement(struct labeled_statement_details details){
	return details.scoped_statement_id;
}
struct expression_id get_expression_id_case_label_from_labeled_statement(struct labeled_statement_details details){
	struct expression_id id;
	id.id = details.id;
	return id;
}
struct scoped_statement_id get_scoped_statement_scoped_statement_id_default_label_from_labeled_statement(struct labeled_statement_details details){
	return details.scoped_statement_id;
}
struct primary_expression_id get_primary_expression_primary_expression_id_postfix_expression_from_postfix_expression(struct postfix_expression_details details){
	return details.primary_expression_id;
}
struct postfix_expression_part_list_item_id get_postfix_expression_part_list_item_postfix_expression_part_list_item_id_postfix_expression_from_postfix_expression(struct postfix_expression_details details){
	return details.postfix_expression_part_list_item_id;
}
struct constant_description_id get_constant_description_id_constant_description_from_primary_expression(struct primary_expression_details details){
	struct constant_description_id id;
	id.id = details.id;
	return id;
}
struct identifier_id get_identifier_id_identifier_from_primary_expression(struct primary_expression_details details){
	struct identifier_id id;
	id.id = details.id;
	return id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_identifier_from_primary_expression(struct primary_expression_details details){
	return details.scope_guid_id;
}
struct expression_id get_expression_id_expression_from_primary_expression(struct primary_expression_details details){
	struct expression_id id;
	id.id = details.id;
	return id;
}
struct declaration_statement_id get_declaration_statement_id_declaration_statement_from_any_statement(struct any_statement_details details){
	struct declaration_statement_id id;
	id.id = details.id;
	return id;
}
struct scoped_statement_id get_scoped_statement_id_scoped_statement_from_any_statement(struct any_statement_details details){
	struct scoped_statement_id id;
	id.id = details.id;
	return id;
}
struct code_statement_id get_code_statement_id_code_statement_from_any_statement(struct any_statement_details details){
	struct code_statement_id id;
	id.id = details.id;
	return id;
}
struct any_statement_list_item_id get_any_statement_list_item_any_statement_list_item_id_scoped_statement_from_scoped_statement(struct scoped_statement_details details){
	return details.any_statement_list_item_id;
}
struct scope_guid_id get_scope_guid_scope_guid_id_scoped_statement_from_scoped_statement(struct scoped_statement_details details){
	return details.scope_guid_id;
}
struct simple_declaration_id get_simple_declaration_id_simple_declaration_from_declaration_statement(struct declaration_statement_details details){
	struct simple_declaration_id id;
	id.id = details.id;
	return id;
}
struct function_definition_id get_function_definition_id_function_definition_from_declaration_statement(struct declaration_statement_details details){
	struct function_definition_id id;
	id.id = details.id;
	return id;
}
struct general_type_id get_general_type_general_type_id_function_definition_from_function_definition(struct function_definition_details details){
	return details.general_type_id;
}
struct any_statement_list_item_id get_any_statement_list_item_k_and_r_declarations_function_definition_from_function_definition(struct function_definition_details details){
	return details.k_and_r_declarations;
}
struct any_statement_list_item_id get_any_statement_list_item_params_function_definition_from_function_definition(struct function_definition_details details){
	return details.params;
}
struct scoped_statement_id get_scoped_statement_body_function_definition_from_function_definition(struct function_definition_details details){
	return details.body;
}
struct identifier_id get_identifier_identifier_id_function_definition_from_function_definition(struct function_definition_details details){
	return details.identifier_id;
}
struct general_type_id get_general_type_general_type_id_simple_declaration_from_simple_declaration(struct simple_declaration_details details){
	return details.general_type_id;
}
struct identifier_id get_identifier_identifier_id_simple_declaration_from_simple_declaration(struct simple_declaration_details details){
	return details.identifier_id;
}
struct initializer_id get_initializer_initializer_id_simple_declaration_from_simple_declaration(struct simple_declaration_details details){
	return details.initializer_id;
}
struct cast_expression_id get_cast_expression_id_cast_expression_from_expression(struct expression_details details){
	struct cast_expression_id id;
	id.id = details.id;
	return id;
}
struct binary_expression_id get_binary_expression_id_binary_expression_from_expression(struct expression_details details){
	struct binary_expression_id id;
	id.id = details.id;
	return id;
}
struct ternary_expression_id get_ternary_expression_id_ternary_expression_from_expression(struct expression_details details){
	struct ternary_expression_id id;
	id.id = details.id;
	return id;
}
struct cast_expression_id get_cast_expression_id1_has_cast_from_cast_expression(struct cast_expression_details details){
	struct cast_expression_id id;
	id.id = details.id1;
	return id;
}
struct general_type_id get_general_type_id2_has_cast_from_cast_expression(struct cast_expression_details details){
	return details.id2;
}
struct unary_expression_id get_unary_expression_id1_no_cast_from_cast_expression(struct cast_expression_details details){
	struct unary_expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_sizeof_expression_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct postfix_expression_id get_postfix_expression_id1_postfix_expression_from_unary_expression(struct unary_expression_details details){
	struct postfix_expression_id id;
	id.id = details.id1;
	return id;
}
struct general_type_id get_general_type_id1_sizeof_type_from_unary_expression(struct unary_expression_details details){
	struct general_type_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_increment_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_decrement_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_address_of_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_point_to_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_plus_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_minus_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_bitwise_not_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_id1_logical_not_from_unary_expression(struct unary_expression_details details){
	struct expression_id id;
	id.id = details.id1;
	return id;
}
struct expression_id get_expression_e1_comma_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_comma_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_plus_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_plus_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_minus_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_minus_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_divide_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_divide_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_multiply_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_multiply_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_exclusive_or_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_exclusive_or_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_modulus_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_modulus_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_right_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_right_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_left_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_left_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_bitwise_or_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_bitwise_or_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_logical_or_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_logical_or_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_bitwise_and_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_bitwise_and_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_logical_and_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_logical_and_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_lt_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_lt_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_gt_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_gt_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_le_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_le_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_ge_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_ge_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_eq_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_eq_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_ne_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_ne_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_add_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_add_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_sub_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_sub_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_div_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_div_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_mul_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_mul_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_and_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_and_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_or_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_or_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_xor_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_xor_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_mod_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_mod_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_right_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_right_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_left_assign_from_binary_expression(struct binary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_left_assign_from_binary_expression(struct binary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e1_ternary_operator_from_ternary_expression(struct ternary_expression_details details){
	return details.e1;
}
struct expression_id get_expression_e2_ternary_operator_from_ternary_expression(struct ternary_expression_details details){
	return details.e2;
}
struct expression_id get_expression_e3_ternary_operator_from_ternary_expression(struct ternary_expression_details details){
	return details.e3;
}
struct general_type_id get_general_type_general_type_id_constant_description_from_constant_description(struct constant_description_details details){
	return details.general_type_id;
}
struct data_package_id get_data_package_data_package_id_constant_description_from_constant_description(struct constant_description_details details){
	return details.data_package_id;
}
struct error_node_id get_error_node_id_error_from_data_package(struct data_package_details details){
	struct error_node_id id;
	id.id = details.id;
	return id;
}
struct byte_package_id get_byte_package_id_byte_from_data_package(struct data_package_details details){
	struct byte_package_id id;
	id.id = details.id;
	return id;
}
struct hword_package_id get_hword_package_id_hword_from_data_package(struct data_package_details details){
	struct hword_package_id id;
	id.id = details.id;
	return id;
}
struct word_package_id get_word_package_id_word_from_data_package(struct data_package_details details){
	struct word_package_id id;
	id.id = details.id;
	return id;
}
struct dword_package_id get_dword_package_id_dword_from_data_package(struct data_package_details details){
	struct dword_package_id id;
	id.id = details.id;
	return id;
}
struct qword_package_id get_qword_package_id_qword_from_data_package(struct data_package_details details){
	struct qword_package_id id;
	id.id = details.id;
	return id;
}
struct string_package_id get_string_package_id_string_from_data_package(struct data_package_details details){
	struct string_package_id id;
	id.id = details.id;
	return id;
}
struct scope_guid_id get_scope_guid_parent_block_scope_from_scope_guid(struct scope_guid_details details){
	return details.parent;
}
struct scope_guid_id get_scope_guid_parent_function_scope_from_scope_guid(struct scope_guid_details details){
	return details.parent;
}
struct scope_guid_id get_scope_guid_parent_function_prototype_scope_from_scope_guid(struct scope_guid_details details){
	return details.parent;
}
