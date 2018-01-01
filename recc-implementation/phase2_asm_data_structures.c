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

#include "../recc-implementation/phase2_asm_data_structures.h"

void register_asm_data_structures_objects(struct build_state * state, struct bootstrap_state * b){
	make_map(state, b, make_beb_list(2, "struct aggregate_type_specifier_details", "struct aggregate_type_specifier_id"), make_beb_list(2, "generated/struct_aggregate_type_specifier_details.h", "generated/struct_aggregate_type_specifier_id.h"), "recc-implementation/templates/asm/struct_aggregate_type_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct simple_type_specifier_details", "struct simple_type_specifier_id"), make_beb_list(2, "generated/struct_simple_type_specifier_details.h", "generated/struct_simple_type_specifier_id.h"), "recc-implementation/templates/asm/struct_simple_type_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct simple_type_qualifier_details", "struct simple_type_qualifier_id"), make_beb_list(2, "generated/struct_simple_type_qualifier_details.h", "generated/struct_simple_type_qualifier_id.h"), "recc-implementation/templates/asm/struct_simple_type_qualifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct simple_storage_class_specifier_details", "struct simple_storage_class_specifier_id"), make_beb_list(2, "generated/struct_simple_storage_class_specifier_details.h", "generated/struct_simple_storage_class_specifier_id.h"), "recc-implementation/templates/asm/struct_simple_storage_class_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct specifier_or_qualifier_details", "struct specifier_or_qualifier_id"), make_beb_list(2, "generated/struct_specifier_or_qualifier_details.h", "generated/struct_specifier_or_qualifier_id.h"), "recc-implementation/templates/asm/struct_specifier_or_qualifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct specifier_or_qualifier_list_item_details", "struct specifier_or_qualifier_list_item_id"), make_beb_list(2, "generated/struct_specifier_or_qualifier_list_item_details.h", "generated/struct_specifier_or_qualifier_list_item_id.h"), "recc-implementation/templates/asm/struct_specifier_or_qualifier_list_item_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct parameter_list_details", "struct parameter_list_id"), make_beb_list(2, "generated/struct_parameter_list_details.h", "generated/struct_parameter_list_id.h"), "recc-implementation/templates/asm/struct_parameter_list_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct declarator_part_details", "struct declarator_part_id"), make_beb_list(2, "generated/struct_declarator_part_details.h", "generated/struct_declarator_part_id.h"), "recc-implementation/templates/asm/struct_declarator_part_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct any_statement_list_item_details", "struct any_statement_list_item_id"), make_beb_list(2, "generated/struct_any_statement_list_item_details.h", "generated/struct_any_statement_list_item_id.h"), "recc-implementation/templates/asm/struct_any_statement_list_item_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct anonymous_tag_definition_details", "struct anonymous_tag_definition_id"), make_beb_list(2, "generated/struct_anonymous_tag_definition_details.h", "generated/struct_anonymous_tag_definition_id.h"), "recc-implementation/templates/asm/struct_anonymous_tag_definition_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct named_tag_predeclaration_details", "struct named_tag_predeclaration_id"), make_beb_list(2, "generated/struct_named_tag_predeclaration_details.h", "generated/struct_named_tag_predeclaration_id.h"), "recc-implementation/templates/asm/struct_named_tag_predeclaration_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct named_tag_definition_details", "struct named_tag_definition_id"), make_beb_list(2, "generated/struct_named_tag_definition_details.h", "generated/struct_named_tag_definition_id.h"), "recc-implementation/templates/asm/struct_named_tag_definition_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct tag_definition_details", "struct tag_definition_id"), make_beb_list(2, "generated/struct_tag_definition_details.h", "generated/struct_tag_definition_id.h"), "recc-implementation/templates/asm/struct_tag_definition_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct struct_or_union_member_list_item_details", "struct struct_or_union_member_list_item_id"), make_beb_list(2, "generated/struct_struct_or_union_member_list_item_details.h", "generated/struct_struct_or_union_member_list_item_id.h"), "recc-implementation/templates/asm/struct_struct_or_union_member_list_item_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct enum_member_list_item_details", "struct enum_member_list_item_id"), make_beb_list(2, "generated/struct_enum_member_list_item_details.h", "generated/struct_enum_member_list_item_id.h"), "recc-implementation/templates/asm/struct_enum_member_list_item_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct enum_member_details", "struct enum_member_id"), make_beb_list(2, "generated/struct_enum_member_details.h", "generated/struct_enum_member_id.h"), "recc-implementation/templates/asm/struct_enum_member_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct struct_or_union_member_details", "struct struct_or_union_member_id"), make_beb_list(2, "generated/struct_struct_or_union_member_details.h", "generated/struct_struct_or_union_member_id.h"), "recc-implementation/templates/asm/struct_struct_or_union_member_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct initializer_details", "struct initializer_id"), make_beb_list(2, "generated/struct_initializer_details.h", "generated/struct_initializer_id.h"), "recc-implementation/templates/asm/struct_initializer_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct initializer_list_item_details", "struct initializer_list_item_id"), make_beb_list(2, "generated/struct_initializer_list_item_details.h", "generated/struct_initializer_list_item_id.h"), "recc-implementation/templates/asm/struct_initializer_list_item_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct declarator_part_list_item_details", "struct declarator_part_list_item_id"), make_beb_list(2, "generated/struct_declarator_part_list_item_details.h", "generated/struct_declarator_part_list_item_id.h"), "recc-implementation/templates/asm/struct_declarator_part_list_item_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct pointer_part_details", "struct pointer_part_id"), make_beb_list(2, "generated/struct_pointer_part_details.h", "generated/struct_pointer_part_id.h"), "recc-implementation/templates/asm/struct_pointer_part_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct function_part_details", "struct function_part_id"), make_beb_list(2, "generated/struct_function_part_details.h", "generated/struct_function_part_id.h"), "recc-implementation/templates/asm/struct_function_part_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct array_part_details", "struct array_part_id"), make_beb_list(2, "generated/struct_array_part_details.h", "generated/struct_array_part_id.h"), "recc-implementation/templates/asm/struct_array_part_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct general_type_details", "struct general_type_id"), make_beb_list(2, "generated/struct_general_type_details.h", "generated/struct_general_type_id.h"), "recc-implementation/templates/asm/struct_general_type_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct identifier_namespace_entry_details", "struct identifier_namespace_entry_id"), make_beb_list(2, "generated/struct_identifier_namespace_entry_details.h", "generated/struct_identifier_namespace_entry_id.h"), "recc-implementation/templates/asm/struct_identifier_namespace_entry_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct general_type_list_item_details", "struct general_type_list_item_id"), make_beb_list(2, "generated/struct_general_type_list_item_details.h", "generated/struct_general_type_list_item_id.h"), "recc-implementation/templates/asm/struct_general_type_list_item_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct postfix_expression_part_list_item_details", "struct postfix_expression_part_list_item_id"), make_beb_list(2, "generated/struct_postfix_expression_part_list_item_details.h", "generated/struct_postfix_expression_part_list_item_id.h"), "recc-implementation/templates/asm/struct_postfix_expression_part_list_item_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct postfix_expression_part_details", "struct postfix_expression_part_id"), make_beb_list(2, "generated/struct_postfix_expression_part_details.h", "generated/struct_postfix_expression_part_id.h"), "recc-implementation/templates/asm/struct_postfix_expression_part_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct unscoped_tag_specifier_details", "struct unscoped_tag_specifier_id"), make_beb_list(2, "generated/struct_unscoped_tag_specifier_details.h", "generated/struct_unscoped_tag_specifier_id.h"), "recc-implementation/templates/asm/struct_unscoped_tag_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct scoped_typename_specifier_details", "struct scoped_typename_specifier_id"), make_beb_list(2, "generated/struct_scoped_typename_specifier_details.h", "generated/struct_scoped_typename_specifier_id.h"), "recc-implementation/templates/asm/struct_scoped_typename_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct scoped_tag_specifier_details", "struct scoped_tag_specifier_id"), make_beb_list(2, "generated/struct_scoped_tag_specifier_details.h", "generated/struct_scoped_tag_specifier_id.h"), "recc-implementation/templates/asm/struct_scoped_tag_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct unscoped_union_specifier_details", "struct unscoped_union_specifier_id"), make_beb_list(2, "generated/struct_unscoped_union_specifier_details.h", "generated/struct_unscoped_union_specifier_id.h"), "recc-implementation/templates/asm/struct_unscoped_union_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct unscoped_struct_specifier_details", "struct unscoped_struct_specifier_id"), make_beb_list(2, "generated/struct_unscoped_struct_specifier_details.h", "generated/struct_unscoped_struct_specifier_id.h"), "recc-implementation/templates/asm/struct_unscoped_struct_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct unscoped_enum_specifier_details", "struct unscoped_enum_specifier_id"), make_beb_list(2, "generated/struct_unscoped_enum_specifier_details.h", "generated/struct_unscoped_enum_specifier_id.h"), "recc-implementation/templates/asm/struct_unscoped_enum_specifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct bitfield_details", "struct bitfield_id"), make_beb_list(2, "generated/struct_bitfield_details.h", "generated/struct_bitfield_id.h"), "recc-implementation/templates/asm/struct_bitfield_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct jump_statement_details", "struct jump_statement_id"), make_beb_list(2, "generated/struct_jump_statement_details.h", "generated/struct_jump_statement_id.h"), "recc-implementation/templates/asm/struct_jump_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct switch_statement_details", "struct switch_statement_id"), make_beb_list(2, "generated/struct_switch_statement_details.h", "generated/struct_switch_statement_id.h"), "recc-implementation/templates/asm/struct_switch_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct if_statement_details", "struct if_statement_id"), make_beb_list(2, "generated/struct_if_statement_details.h", "generated/struct_if_statement_id.h"), "recc-implementation/templates/asm/struct_if_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct while_statement_details", "struct while_statement_id"), make_beb_list(2, "generated/struct_while_statement_details.h", "generated/struct_while_statement_id.h"), "recc-implementation/templates/asm/struct_while_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct for_statement_details", "struct for_statement_id"), make_beb_list(2, "generated/struct_for_statement_details.h", "generated/struct_for_statement_id.h"), "recc-implementation/templates/asm/struct_for_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct code_statement_details", "struct code_statement_id"), make_beb_list(2, "generated/struct_code_statement_details.h", "generated/struct_code_statement_id.h"), "recc-implementation/templates/asm/struct_code_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct labeled_statement_details", "struct labeled_statement_id"), make_beb_list(2, "generated/struct_labeled_statement_details.h", "generated/struct_labeled_statement_id.h"), "recc-implementation/templates/asm/struct_labeled_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct postfix_expression_details", "struct postfix_expression_id"), make_beb_list(2, "generated/struct_postfix_expression_details.h", "generated/struct_postfix_expression_id.h"), "recc-implementation/templates/asm/struct_postfix_expression_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct primary_expression_details", "struct primary_expression_id"), make_beb_list(2, "generated/struct_primary_expression_details.h", "generated/struct_primary_expression_id.h"), "recc-implementation/templates/asm/struct_primary_expression_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct any_statement_details", "struct any_statement_id"), make_beb_list(2, "generated/struct_any_statement_details.h", "generated/struct_any_statement_id.h"), "recc-implementation/templates/asm/struct_any_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct scoped_statement_details", "struct scoped_statement_id"), make_beb_list(2, "generated/struct_scoped_statement_details.h", "generated/struct_scoped_statement_id.h"), "recc-implementation/templates/asm/struct_scoped_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct declaration_statement_details", "struct declaration_statement_id"), make_beb_list(2, "generated/struct_declaration_statement_details.h", "generated/struct_declaration_statement_id.h"), "recc-implementation/templates/asm/struct_declaration_statement_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct function_definition_details", "struct function_definition_id"), make_beb_list(2, "generated/struct_function_definition_details.h", "generated/struct_function_definition_id.h"), "recc-implementation/templates/asm/struct_function_definition_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct simple_declaration_details", "struct simple_declaration_id"), make_beb_list(2, "generated/struct_simple_declaration_details.h", "generated/struct_simple_declaration_id.h"), "recc-implementation/templates/asm/struct_simple_declaration_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct expression_details", "struct expression_id"), make_beb_list(2, "generated/struct_expression_details.h", "generated/struct_expression_id.h"), "recc-implementation/templates/asm/struct_expression_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct cast_expression_details", "struct cast_expression_id"), make_beb_list(2, "generated/struct_cast_expression_details.h", "generated/struct_cast_expression_id.h"), "recc-implementation/templates/asm/struct_cast_expression_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct unary_expression_details", "struct unary_expression_id"), make_beb_list(2, "generated/struct_unary_expression_details.h", "generated/struct_unary_expression_id.h"), "recc-implementation/templates/asm/struct_unary_expression_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct binary_expression_details", "struct binary_expression_id"), make_beb_list(2, "generated/struct_binary_expression_details.h", "generated/struct_binary_expression_id.h"), "recc-implementation/templates/asm/struct_binary_expression_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct ternary_expression_details", "struct ternary_expression_id"), make_beb_list(2, "generated/struct_ternary_expression_details.h", "generated/struct_ternary_expression_id.h"), "recc-implementation/templates/asm/struct_ternary_expression_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct constant_description_details", "struct constant_description_id"), make_beb_list(2, "generated/struct_constant_description_details.h", "generated/struct_constant_description_id.h"), "recc-implementation/templates/asm/struct_constant_description_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct data_package_details", "struct data_package_id"), make_beb_list(2, "generated/struct_data_package_details.h", "generated/struct_data_package_id.h"), "recc-implementation/templates/asm/struct_data_package_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct error_node_details", "struct error_node_id"), make_beb_list(2, "generated/struct_error_node_details.h", "generated/struct_error_node_id.h"), "recc-implementation/templates/asm/struct_error_node_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct byte_package_details", "struct byte_package_id"), make_beb_list(2, "generated/struct_byte_package_details.h", "generated/struct_byte_package_id.h"), "recc-implementation/templates/asm/struct_byte_package_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct hword_package_details", "struct hword_package_id"), make_beb_list(2, "generated/struct_hword_package_details.h", "generated/struct_hword_package_id.h"), "recc-implementation/templates/asm/struct_hword_package_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct word_package_details", "struct word_package_id"), make_beb_list(2, "generated/struct_word_package_details.h", "generated/struct_word_package_id.h"), "recc-implementation/templates/asm/struct_word_package_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct qword_package_details", "struct qword_package_id"), make_beb_list(2, "generated/struct_qword_package_details.h", "generated/struct_qword_package_id.h"), "recc-implementation/templates/asm/struct_qword_package_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct dword_package_details", "struct dword_package_id"), make_beb_list(2, "generated/struct_dword_package_details.h", "generated/struct_dword_package_id.h"), "recc-implementation/templates/asm/struct_dword_package_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct string_package_details", "struct string_package_id"), make_beb_list(2, "generated/struct_string_package_details.h", "generated/struct_string_package_id.h"), "recc-implementation/templates/asm/struct_string_package_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct identifier_details", "struct identifier_id"), make_beb_list(2, "generated/struct_identifier_details.h", "generated/struct_identifier_id.h"), "recc-implementation/templates/asm/struct_identifier_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct scope_guid_details", "struct scope_guid_id"), make_beb_list(2, "generated/struct_scope_guid_details.h", "generated/struct_scope_guid_id.h"), "recc-implementation/templates/asm/struct_scope_guid_details_compare.c");
	make_map(state, b, make_beb_list(2, "struct anonymous_entry_details", "struct anonymous_entry_id"), make_beb_list(2, "generated/struct_anonymous_entry_details.h", "generated/struct_anonymous_entry_id.h"), "recc-implementation/templates/asm/struct_anonymous_entry_details_compare.c");
}

void register_asm_details_and_ids(struct build_state * state){
	register_generated_type(state, "types/asm/enum_asm_aggregate_type_specifier_kind.h", "generated/enum_asm_aggregate_type_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_aggregate_type_specifier_id.h", "generated/struct_aggregate_type_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_aggregate_type_specifier_details.h", "generated/struct_aggregate_type_specifier_details.h",
		"generated/enum_asm_aggregate_type_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_simple_type_specifier_kind.h", "generated/enum_asm_simple_type_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_simple_type_specifier_id.h", "generated/struct_simple_type_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_simple_type_specifier_details.h", "generated/struct_simple_type_specifier_details.h",
		"generated/enum_asm_simple_type_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_simple_type_qualifier_kind.h", "generated/enum_asm_simple_type_qualifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_simple_type_qualifier_id.h", "generated/struct_simple_type_qualifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_simple_type_qualifier_details.h", "generated/struct_simple_type_qualifier_details.h",
		"generated/enum_asm_simple_type_qualifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_simple_storage_class_specifier_kind.h", "generated/enum_asm_simple_storage_class_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_simple_storage_class_specifier_id.h", "generated/struct_simple_storage_class_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_simple_storage_class_specifier_details.h", "generated/struct_simple_storage_class_specifier_details.h",
		"generated/enum_asm_simple_storage_class_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_specifier_or_qualifier_kind.h", "generated/enum_asm_specifier_or_qualifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_specifier_or_qualifier_id.h", "generated/struct_specifier_or_qualifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_specifier_or_qualifier_details.h", "generated/struct_specifier_or_qualifier_details.h",
		"generated/enum_asm_specifier_or_qualifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_specifier_or_qualifier_list_item_kind.h", "generated/enum_asm_specifier_or_qualifier_list_item_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_specifier_or_qualifier_list_item_id.h", "generated/struct_specifier_or_qualifier_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_specifier_or_qualifier_list_item_details.h", "generated/struct_specifier_or_qualifier_list_item_details.h",
		"generated/enum_asm_specifier_or_qualifier_list_item_kind.h",
		"generated/struct_specifier_or_qualifier_list_item_id.h",
		"generated/struct_specifier_or_qualifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_parameter_list_kind.h", "generated/enum_asm_parameter_list_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_parameter_list_id.h", "generated/struct_parameter_list_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_parameter_list_details.h", "generated/struct_parameter_list_details.h",
		"generated/enum_asm_parameter_list_kind.h",
		"generated/struct_general_type_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_declarator_part_kind.h", "generated/enum_asm_declarator_part_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_declarator_part_id.h", "generated/struct_declarator_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_declarator_part_details.h", "generated/struct_declarator_part_details.h",
		"generated/enum_asm_declarator_part_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_any_statement_list_item_kind.h", "generated/enum_asm_any_statement_list_item_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_any_statement_list_item_id.h", "generated/struct_any_statement_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_any_statement_list_item_details.h", "generated/struct_any_statement_list_item_details.h",
		"generated/enum_asm_any_statement_list_item_kind.h",
		"generated/struct_any_statement_list_item_id.h",
		"generated/struct_any_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_anonymous_tag_definition_kind.h", "generated/enum_asm_anonymous_tag_definition_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_anonymous_tag_definition_id.h", "generated/struct_anonymous_tag_definition_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_anonymous_tag_definition_details.h", "generated/struct_anonymous_tag_definition_details.h",
		"generated/enum_asm_anonymous_tag_definition_kind.h",
		"generated/struct_tag_definition_id.h",
		"generated/struct_scope_guid_id.h",
		"generated/struct_anonymous_entry_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_named_tag_predeclaration_kind.h", "generated/enum_asm_named_tag_predeclaration_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_named_tag_predeclaration_id.h", "generated/struct_named_tag_predeclaration_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_named_tag_predeclaration_details.h", "generated/struct_named_tag_predeclaration_details.h",
		"generated/enum_asm_named_tag_predeclaration_kind.h",
		"generated/struct_scope_guid_id.h",
		"generated/struct_identifier_id.h",
		"generated/enum_asm_unscoped_tag_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_named_tag_definition_kind.h", "generated/enum_asm_named_tag_definition_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_named_tag_definition_id.h", "generated/struct_named_tag_definition_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_named_tag_definition_details.h", "generated/struct_named_tag_definition_details.h",
		"generated/enum_asm_named_tag_definition_kind.h",
		"generated/struct_tag_definition_id.h",
		"generated/struct_scope_guid_id.h",
		"generated/struct_identifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_tag_definition_kind.h", "generated/enum_asm_tag_definition_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_tag_definition_id.h", "generated/struct_tag_definition_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_tag_definition_details.h", "generated/struct_tag_definition_details.h",
		"generated/enum_asm_tag_definition_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_struct_or_union_member_list_item_kind.h", "generated/enum_asm_struct_or_union_member_list_item_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_struct_or_union_member_list_item_id.h", "generated/struct_struct_or_union_member_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_struct_or_union_member_list_item_details.h", "generated/struct_struct_or_union_member_list_item_details.h",
		"generated/enum_asm_struct_or_union_member_list_item_kind.h",
		"generated/struct_struct_or_union_member_list_item_id.h",
		"generated/struct_struct_or_union_member_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_enum_member_list_item_kind.h", "generated/enum_asm_enum_member_list_item_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_enum_member_list_item_id.h", "generated/struct_enum_member_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_enum_member_list_item_details.h", "generated/struct_enum_member_list_item_details.h",
		"generated/enum_asm_enum_member_list_item_kind.h",
		"generated/struct_enum_member_list_item_id.h",
		"generated/struct_enum_member_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_enum_member_kind.h", "generated/enum_asm_enum_member_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_enum_member_id.h", "generated/struct_enum_member_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_enum_member_details.h", "generated/struct_enum_member_details.h",
		"generated/enum_asm_enum_member_kind.h",
		"generated/struct_identifier_id.h",
		"generated/struct_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_struct_or_union_member_kind.h", "generated/enum_asm_struct_or_union_member_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_struct_or_union_member_id.h", "generated/struct_struct_or_union_member_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_struct_or_union_member_details.h", "generated/struct_struct_or_union_member_details.h",
		"generated/enum_asm_struct_or_union_member_kind.h",
		"generated/struct_general_type_id.h",
		"generated/struct_identifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_initializer_kind.h", "generated/enum_asm_initializer_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_initializer_id.h", "generated/struct_initializer_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_initializer_details.h", "generated/struct_initializer_details.h",
		"generated/enum_asm_initializer_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_initializer_list_item_kind.h", "generated/enum_asm_initializer_list_item_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_initializer_list_item_id.h", "generated/struct_initializer_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_initializer_list_item_details.h", "generated/struct_initializer_list_item_details.h",
		"generated/enum_asm_initializer_list_item_kind.h",
		"generated/struct_initializer_list_item_id.h",
		"generated/struct_initializer_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_declarator_part_list_item_kind.h", "generated/enum_asm_declarator_part_list_item_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_declarator_part_list_item_id.h", "generated/struct_declarator_part_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_declarator_part_list_item_details.h", "generated/struct_declarator_part_list_item_details.h",
		"generated/enum_asm_declarator_part_list_item_kind.h",
		"generated/struct_declarator_part_list_item_id.h",
		"generated/struct_declarator_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_pointer_part_kind.h", "generated/enum_asm_pointer_part_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_pointer_part_id.h", "generated/struct_pointer_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_pointer_part_details.h", "generated/struct_pointer_part_details.h",
		"generated/enum_asm_pointer_part_kind.h",
		"generated/struct_specifier_or_qualifier_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_function_part_kind.h", "generated/enum_asm_function_part_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_function_part_id.h", "generated/struct_function_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_function_part_details.h", "generated/struct_function_part_details.h",
		"generated/enum_asm_function_part_kind.h",
		"generated/struct_parameter_list_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_array_part_kind.h", "generated/enum_asm_array_part_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_array_part_id.h", "generated/struct_array_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_array_part_details.h", "generated/struct_array_part_details.h",
		"generated/enum_asm_array_part_kind.h",
		"generated/struct_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_general_type_kind.h", "generated/enum_asm_general_type_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_general_type_id.h", "generated/struct_general_type_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_general_type_details.h", "generated/struct_general_type_details.h",
		"generated/enum_asm_general_type_kind.h",
		"generated/struct_specifier_or_qualifier_list_item_id.h",
		"generated/struct_declarator_part_list_item_id.h",
		"generated/struct_bitfield_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_identifier_namespace_entry_kind.h", "generated/enum_asm_identifier_namespace_entry_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_identifier_namespace_entry_id.h", "generated/struct_identifier_namespace_entry_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_identifier_namespace_entry_details.h", "generated/struct_identifier_namespace_entry_details.h",
		"generated/enum_asm_identifier_namespace_entry_kind.h",
		"generated/struct_identifier_id.h",
		"generated/struct_scope_guid_id.h",
		"generated/struct_general_type_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_general_type_list_item_kind.h", "generated/enum_asm_general_type_list_item_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_general_type_list_item_id.h", "generated/struct_general_type_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_general_type_list_item_details.h", "generated/struct_general_type_list_item_details.h",
		"generated/enum_asm_general_type_list_item_kind.h",
		"generated/struct_general_type_list_item_id.h",
		"generated/struct_general_type_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_postfix_expression_part_list_item_kind.h", "generated/enum_asm_postfix_expression_part_list_item_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_postfix_expression_part_list_item_id.h", "generated/struct_postfix_expression_part_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_postfix_expression_part_list_item_details.h", "generated/struct_postfix_expression_part_list_item_details.h",
		"generated/enum_asm_postfix_expression_part_list_item_kind.h",
		"generated/struct_postfix_expression_part_list_item_id.h",
		"generated/struct_postfix_expression_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_postfix_expression_part_kind.h", "generated/enum_asm_postfix_expression_part_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_postfix_expression_part_id.h", "generated/struct_postfix_expression_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_postfix_expression_part_details.h", "generated/struct_postfix_expression_part_details.h",
		"generated/enum_asm_postfix_expression_part_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_unscoped_tag_specifier_kind.h", "generated/enum_asm_unscoped_tag_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unscoped_tag_specifier_id.h", "generated/struct_unscoped_tag_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unscoped_tag_specifier_details.h", "generated/struct_unscoped_tag_specifier_details.h",
		"generated/enum_asm_unscoped_tag_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_scoped_typename_specifier_kind.h", "generated/enum_asm_scoped_typename_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_scoped_typename_specifier_id.h", "generated/struct_scoped_typename_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_scoped_typename_specifier_details.h", "generated/struct_scoped_typename_specifier_details.h",
		"generated/enum_asm_scoped_typename_specifier_kind.h",
		"generated/struct_scope_guid_id.h",
		"generated/struct_identifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_scoped_tag_specifier_kind.h", "generated/enum_asm_scoped_tag_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_scoped_tag_specifier_id.h", "generated/struct_scoped_tag_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_scoped_tag_specifier_details.h", "generated/struct_scoped_tag_specifier_details.h",
		"generated/enum_asm_scoped_tag_specifier_kind.h",
		"generated/struct_scope_guid_id.h",
		"generated/struct_unscoped_tag_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_unscoped_union_specifier_kind.h", "generated/enum_asm_unscoped_union_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unscoped_union_specifier_id.h", "generated/struct_unscoped_union_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unscoped_union_specifier_details.h", "generated/struct_unscoped_union_specifier_details.h",
		"generated/enum_asm_unscoped_union_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_unscoped_struct_specifier_kind.h", "generated/enum_asm_unscoped_struct_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unscoped_struct_specifier_id.h", "generated/struct_unscoped_struct_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unscoped_struct_specifier_details.h", "generated/struct_unscoped_struct_specifier_details.h",
		"generated/enum_asm_unscoped_struct_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_unscoped_enum_specifier_kind.h", "generated/enum_asm_unscoped_enum_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unscoped_enum_specifier_id.h", "generated/struct_unscoped_enum_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unscoped_enum_specifier_details.h", "generated/struct_unscoped_enum_specifier_details.h",
		"generated/enum_asm_unscoped_enum_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_bitfield_kind.h", "generated/enum_asm_bitfield_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_bitfield_id.h", "generated/struct_bitfield_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_bitfield_details.h", "generated/struct_bitfield_details.h",
		"generated/enum_asm_bitfield_kind.h",
		"generated/struct_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_jump_statement_kind.h", "generated/enum_asm_jump_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_jump_statement_id.h", "generated/struct_jump_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_jump_statement_details.h", "generated/struct_jump_statement_details.h",
		"generated/enum_asm_jump_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_switch_statement_kind.h", "generated/enum_asm_switch_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_switch_statement_id.h", "generated/struct_switch_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_switch_statement_details.h", "generated/struct_switch_statement_details.h",
		"generated/enum_asm_switch_statement_kind.h",
		"generated/struct_expression_id.h",
		"generated/struct_scoped_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_if_statement_kind.h", "generated/enum_asm_if_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_if_statement_id.h", "generated/struct_if_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_if_statement_details.h", "generated/struct_if_statement_details.h",
		"generated/enum_asm_if_statement_kind.h",
		"generated/struct_scoped_statement_id.h",
		"generated/struct_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_while_statement_kind.h", "generated/enum_asm_while_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_while_statement_id.h", "generated/struct_while_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_while_statement_details.h", "generated/struct_while_statement_details.h",
		"generated/enum_asm_while_statement_kind.h",
		"generated/struct_expression_id.h",
		"generated/struct_scoped_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_for_statement_kind.h", "generated/enum_asm_for_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_for_statement_id.h", "generated/struct_for_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_for_statement_details.h", "generated/struct_for_statement_details.h",
		"generated/enum_asm_for_statement_kind.h",
		"generated/struct_any_statement_id.h",
		"generated/struct_scoped_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_code_statement_kind.h", "generated/enum_asm_code_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_code_statement_id.h", "generated/struct_code_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_code_statement_details.h", "generated/struct_code_statement_details.h",
		"generated/enum_asm_code_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_labeled_statement_kind.h", "generated/enum_asm_labeled_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_labeled_statement_id.h", "generated/struct_labeled_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_labeled_statement_details.h", "generated/struct_labeled_statement_details.h",
		"generated/enum_asm_labeled_statement_kind.h",
		"generated/struct_scoped_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_postfix_expression_kind.h", "generated/enum_asm_postfix_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_postfix_expression_id.h", "generated/struct_postfix_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_postfix_expression_details.h", "generated/struct_postfix_expression_details.h",
		"generated/enum_asm_postfix_expression_kind.h",
		"generated/struct_primary_expression_id.h",
		"generated/struct_postfix_expression_part_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_primary_expression_kind.h", "generated/enum_asm_primary_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_primary_expression_id.h", "generated/struct_primary_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_primary_expression_details.h", "generated/struct_primary_expression_details.h",
		"generated/enum_asm_primary_expression_kind.h",
		"generated/struct_scope_guid_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_any_statement_kind.h", "generated/enum_asm_any_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_any_statement_id.h", "generated/struct_any_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_any_statement_details.h", "generated/struct_any_statement_details.h",
		"generated/enum_asm_any_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_scoped_statement_kind.h", "generated/enum_asm_scoped_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_scoped_statement_id.h", "generated/struct_scoped_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_scoped_statement_details.h", "generated/struct_scoped_statement_details.h",
		"generated/enum_asm_scoped_statement_kind.h",
		"generated/struct_any_statement_list_item_id.h",
		"generated/struct_scope_guid_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_declaration_statement_kind.h", "generated/enum_asm_declaration_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_declaration_statement_id.h", "generated/struct_declaration_statement_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_declaration_statement_details.h", "generated/struct_declaration_statement_details.h",
		"generated/enum_asm_declaration_statement_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_function_definition_kind.h", "generated/enum_asm_function_definition_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_function_definition_id.h", "generated/struct_function_definition_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_function_definition_details.h", "generated/struct_function_definition_details.h",
		"generated/enum_asm_function_definition_kind.h",
		"generated/struct_general_type_id.h",
		"generated/struct_any_statement_list_item_id.h",
		"generated/struct_scoped_statement_id.h",
		"generated/struct_identifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_simple_declaration_kind.h", "generated/enum_asm_simple_declaration_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_simple_declaration_id.h", "generated/struct_simple_declaration_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_simple_declaration_details.h", "generated/struct_simple_declaration_details.h",
		"generated/enum_asm_simple_declaration_kind.h",
		"generated/struct_general_type_id.h",
		"generated/struct_identifier_id.h",
		"generated/struct_initializer_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_expression_kind.h", "generated/enum_asm_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_expression_id.h", "generated/struct_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_expression_details.h", "generated/struct_expression_details.h",
		"generated/enum_asm_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_cast_expression_kind.h", "generated/enum_asm_cast_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_cast_expression_id.h", "generated/struct_cast_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_cast_expression_details.h", "generated/struct_cast_expression_details.h",
		"generated/enum_asm_cast_expression_kind.h",
		"generated/struct_general_type_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_unary_expression_kind.h", "generated/enum_asm_unary_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unary_expression_id.h", "generated/struct_unary_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_unary_expression_details.h", "generated/struct_unary_expression_details.h",
		"generated/enum_asm_unary_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_binary_expression_kind.h", "generated/enum_asm_binary_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_binary_expression_id.h", "generated/struct_binary_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_binary_expression_details.h", "generated/struct_binary_expression_details.h",
		"generated/enum_asm_binary_expression_kind.h",
		"generated/struct_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_ternary_expression_kind.h", "generated/enum_asm_ternary_expression_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_ternary_expression_id.h", "generated/struct_ternary_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_ternary_expression_details.h", "generated/struct_ternary_expression_details.h",
		"generated/enum_asm_ternary_expression_kind.h",
		"generated/struct_expression_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_constant_description_kind.h", "generated/enum_asm_constant_description_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_constant_description_id.h", "generated/struct_constant_description_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_constant_description_details.h", "generated/struct_constant_description_details.h",
		"generated/enum_asm_constant_description_kind.h",
		"generated/struct_general_type_id.h",
		"generated/struct_data_package_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_data_package_kind.h", "generated/enum_asm_data_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_data_package_id.h", "generated/struct_data_package_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_data_package_details.h", "generated/struct_data_package_details.h",
		"generated/enum_asm_data_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_error_node_kind.h", "generated/enum_asm_error_node_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_error_node_id.h", "generated/struct_error_node_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_error_node_details.h", "generated/struct_error_node_details.h",
		"generated/enum_asm_error_node_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_byte_package_kind.h", "generated/enum_asm_byte_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_byte_package_id.h", "generated/struct_byte_package_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_byte_package_details.h", "generated/struct_byte_package_details.h",
		"generated/enum_asm_byte_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_hword_package_kind.h", "generated/enum_asm_hword_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_hword_package_id.h", "generated/struct_hword_package_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_hword_package_details.h", "generated/struct_hword_package_details.h",
		"generated/enum_asm_hword_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_word_package_kind.h", "generated/enum_asm_word_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_word_package_id.h", "generated/struct_word_package_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_word_package_details.h", "generated/struct_word_package_details.h",
		"generated/enum_asm_word_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_qword_package_kind.h", "generated/enum_asm_qword_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_qword_package_id.h", "generated/struct_qword_package_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_qword_package_details.h", "generated/struct_qword_package_details.h",
		"generated/enum_asm_qword_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_dword_package_kind.h", "generated/enum_asm_dword_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_dword_package_id.h", "generated/struct_dword_package_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_dword_package_details.h", "generated/struct_dword_package_details.h",
		"generated/enum_asm_dword_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_string_package_kind.h", "generated/enum_asm_string_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_string_package_id.h", "generated/struct_string_package_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_string_package_details.h", "generated/struct_string_package_details.h",
		"generated/enum_asm_string_package_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_identifier_kind.h", "generated/enum_asm_identifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_identifier_id.h", "generated/struct_identifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_identifier_details.h", "generated/struct_identifier_details.h",
		"generated/enum_asm_identifier_kind.h",
		"generated/struct_c_lexer_token.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_scope_guid_kind.h", "generated/enum_asm_scope_guid_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_scope_guid_id.h", "generated/struct_scope_guid_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_scope_guid_details.h", "generated/struct_scope_guid_details.h",
		"generated/enum_asm_scope_guid_kind.h",
		"generated/struct_scope_guid_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/enum_asm_anonymous_entry_kind.h", "generated/enum_asm_anonymous_entry_kind.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_anonymous_entry_id.h", "generated/struct_anonymous_entry_id.h",
		"stop"
	);
	register_generated_type(state, "types/asm/struct_anonymous_entry_details.h", "generated/struct_anonymous_entry_details.h",
		"generated/enum_asm_anonymous_entry_kind.h",
		"stop"
	);
}

void register_asm_generated_aggregate_includes(struct build_state * state){

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_aggregate_type_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_simple_type_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_simple_type_qualifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_simple_storage_class_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_specifier_or_qualifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_specifier_or_qualifier_list_item_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_parameter_list_details_to_struct_parameter_list_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_parameter_list_details_to_struct_parameter_list_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_parameter_list_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_declarator_part_details_to_struct_declarator_part_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_declarator_part_details_to_struct_declarator_part_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_declarator_part_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_any_statement_list_item_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_anonymous_tag_definition_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_named_tag_predeclaration_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_named_tag_definition_details_to_struct_named_tag_definition_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_named_tag_definition_details_to_struct_named_tag_definition_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_named_tag_definition_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_tag_definition_details_to_struct_tag_definition_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_tag_definition_details_to_struct_tag_definition_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_tag_definition_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_struct_or_union_member_list_item_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_enum_member_list_item_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_enum_member_details_to_struct_enum_member_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_enum_member_details_to_struct_enum_member_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_enum_member_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_struct_or_union_member_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_initializer_details_to_struct_initializer_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_initializer_details_to_struct_initializer_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_initializer_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_initializer_list_item_details_to_struct_initializer_list_item_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_initializer_list_item_details_to_struct_initializer_list_item_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_initializer_list_item_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_declarator_part_list_item_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_pointer_part_details_to_struct_pointer_part_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_pointer_part_details_to_struct_pointer_part_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_pointer_part_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_function_part_details_to_struct_function_part_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_function_part_details_to_struct_function_part_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_function_part_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_array_part_details_to_struct_array_part_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_array_part_details_to_struct_array_part_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_array_part_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_general_type_details_to_struct_general_type_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_general_type_details_to_struct_general_type_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_general_type_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_identifier_namespace_entry_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_general_type_list_item_details_to_struct_general_type_list_item_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_general_type_list_item_details_to_struct_general_type_list_item_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_general_type_list_item_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_postfix_expression_part_list_item_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_postfix_expression_part_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_unscoped_tag_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_scoped_typename_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_scoped_tag_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_unscoped_union_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_unscoped_struct_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_unscoped_enum_specifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_bitfield_details_to_struct_bitfield_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_bitfield_details_to_struct_bitfield_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_bitfield_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_jump_statement_details_to_struct_jump_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_jump_statement_details_to_struct_jump_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_jump_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_switch_statement_details_to_struct_switch_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_switch_statement_details_to_struct_switch_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_switch_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_if_statement_details_to_struct_if_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_if_statement_details_to_struct_if_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_if_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_while_statement_details_to_struct_while_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_while_statement_details_to_struct_while_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_while_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_for_statement_details_to_struct_for_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_for_statement_details_to_struct_for_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_for_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_code_statement_details_to_struct_code_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_code_statement_details_to_struct_code_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_code_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_labeled_statement_details_to_struct_labeled_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_labeled_statement_details_to_struct_labeled_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_labeled_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_postfix_expression_details_to_struct_postfix_expression_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_postfix_expression_details_to_struct_postfix_expression_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_postfix_expression_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_primary_expression_details_to_struct_primary_expression_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_primary_expression_details_to_struct_primary_expression_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_primary_expression_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_any_statement_details_to_struct_any_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_any_statement_details_to_struct_any_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_any_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_scoped_statement_details_to_struct_scoped_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_scoped_statement_details_to_struct_scoped_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_scoped_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_declaration_statement_details_to_struct_declaration_statement_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_declaration_statement_details_to_struct_declaration_statement_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_declaration_statement_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_function_definition_details_to_struct_function_definition_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_function_definition_details_to_struct_function_definition_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_function_definition_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_simple_declaration_details_to_struct_simple_declaration_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_simple_declaration_details_to_struct_simple_declaration_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_simple_declaration_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_expression_details_to_struct_expression_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_expression_details_to_struct_expression_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_expression_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_cast_expression_details_to_struct_cast_expression_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_cast_expression_details_to_struct_cast_expression_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_cast_expression_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_unary_expression_details_to_struct_unary_expression_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_unary_expression_details_to_struct_unary_expression_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_unary_expression_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_binary_expression_details_to_struct_binary_expression_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_binary_expression_details_to_struct_binary_expression_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_binary_expression_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_ternary_expression_details_to_struct_ternary_expression_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_ternary_expression_details_to_struct_ternary_expression_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_ternary_expression_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_constant_description_details_to_struct_constant_description_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_constant_description_details_to_struct_constant_description_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_constant_description_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_data_package_details_to_struct_data_package_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_data_package_details_to_struct_data_package_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_data_package_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_error_node_details_to_struct_error_node_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_error_node_details_to_struct_error_node_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_error_node_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_byte_package_details_to_struct_byte_package_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_byte_package_details_to_struct_byte_package_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_byte_package_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_hword_package_details_to_struct_hword_package_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_hword_package_details_to_struct_hword_package_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_hword_package_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_word_package_details_to_struct_word_package_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_word_package_details_to_struct_word_package_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_word_package_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_qword_package_details_to_struct_qword_package_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_qword_package_details_to_struct_qword_package_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_qword_package_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_dword_package_details_to_struct_dword_package_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_dword_package_details_to_struct_dword_package_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_dword_package_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_string_package_details_to_struct_string_package_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_string_package_details_to_struct_string_package_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_string_package_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_identifier_details_to_struct_identifier_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_identifier_details_to_struct_identifier_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_identifier_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_scope_guid_details_to_struct_scope_guid_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_scope_guid_details_to_struct_scope_guid_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_scope_guid_kind.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_struct_anonymous_entry_details_to_struct_anonymous_entry_id_key_value_pair_compare.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/struct_anonymous_entry_details_to_struct_anonymous_entry_id_map.h"
	);

	register_inclusion(state, "generated/asm_generated_includes.h",
		"generated/enum_asm_anonymous_entry_kind.h"
	);

}

void register_asm_state_includes(struct build_state * state){

	register_generated_type(state, "types/asm/struct_asm_state.h", "generated/struct_asm_state.h",
		"generated/struct_enum_asm_operation_type_list.h",
		"generated/struct_struct_aggregate_type_specifier_details_list.h",
		"generated/struct_struct_aggregate_type_specifier_details_to_struct_aggregate_type_specifier_id_map.h",
		"generated/struct_struct_simple_type_specifier_details_list.h",
		"generated/struct_struct_simple_type_specifier_details_to_struct_simple_type_specifier_id_map.h",
		"generated/struct_struct_simple_type_qualifier_details_list.h",
		"generated/struct_struct_simple_type_qualifier_details_to_struct_simple_type_qualifier_id_map.h",
		"generated/struct_struct_simple_storage_class_specifier_details_list.h",
		"generated/struct_struct_simple_storage_class_specifier_details_to_struct_simple_storage_class_specifier_id_map.h",
		"generated/struct_struct_specifier_or_qualifier_details_list.h",
		"generated/struct_struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map.h",
		"generated/struct_struct_specifier_or_qualifier_list_item_details_list.h",
		"generated/struct_struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map.h",
		"generated/struct_struct_parameter_list_details_list.h",
		"generated/struct_struct_parameter_list_details_to_struct_parameter_list_id_map.h",
		"generated/struct_struct_declarator_part_details_list.h",
		"generated/struct_struct_declarator_part_details_to_struct_declarator_part_id_map.h",
		"generated/struct_struct_any_statement_list_item_details_list.h",
		"generated/struct_struct_any_statement_list_item_details_to_struct_any_statement_list_item_id_map.h",
		"generated/struct_struct_anonymous_tag_definition_details_list.h",
		"generated/struct_struct_anonymous_tag_definition_details_to_struct_anonymous_tag_definition_id_map.h",
		"generated/struct_struct_named_tag_predeclaration_details_list.h",
		"generated/struct_struct_named_tag_predeclaration_details_to_struct_named_tag_predeclaration_id_map.h",
		"generated/struct_struct_named_tag_definition_details_list.h",
		"generated/struct_struct_named_tag_definition_details_to_struct_named_tag_definition_id_map.h",
		"generated/struct_struct_tag_definition_details_list.h",
		"generated/struct_struct_tag_definition_details_to_struct_tag_definition_id_map.h",
		"generated/struct_struct_struct_or_union_member_list_item_details_list.h",
		"generated/struct_struct_struct_or_union_member_list_item_details_to_struct_struct_or_union_member_list_item_id_map.h",
		"generated/struct_struct_enum_member_list_item_details_list.h",
		"generated/struct_struct_enum_member_list_item_details_to_struct_enum_member_list_item_id_map.h",
		"generated/struct_struct_enum_member_details_list.h",
		"generated/struct_struct_enum_member_details_to_struct_enum_member_id_map.h",
		"generated/struct_struct_struct_or_union_member_details_list.h",
		"generated/struct_struct_struct_or_union_member_details_to_struct_struct_or_union_member_id_map.h",
		"generated/struct_struct_initializer_details_list.h",
		"generated/struct_struct_initializer_details_to_struct_initializer_id_map.h",
		"generated/struct_struct_initializer_list_item_details_list.h",
		"generated/struct_struct_initializer_list_item_details_to_struct_initializer_list_item_id_map.h",
		"generated/struct_struct_declarator_part_list_item_details_list.h",
		"generated/struct_struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map.h",
		"generated/struct_struct_pointer_part_details_list.h",
		"generated/struct_struct_pointer_part_details_to_struct_pointer_part_id_map.h",
		"generated/struct_struct_function_part_details_list.h",
		"generated/struct_struct_function_part_details_to_struct_function_part_id_map.h",
		"generated/struct_struct_array_part_details_list.h",
		"generated/struct_struct_array_part_details_to_struct_array_part_id_map.h",
		"generated/struct_struct_general_type_details_list.h",
		"generated/struct_struct_general_type_details_to_struct_general_type_id_map.h",
		"generated/struct_struct_identifier_namespace_entry_details_list.h",
		"generated/struct_struct_identifier_namespace_entry_details_to_struct_identifier_namespace_entry_id_map.h",
		"generated/struct_struct_general_type_list_item_details_list.h",
		"generated/struct_struct_general_type_list_item_details_to_struct_general_type_list_item_id_map.h",
		"generated/struct_struct_postfix_expression_part_list_item_details_list.h",
		"generated/struct_struct_postfix_expression_part_list_item_details_to_struct_postfix_expression_part_list_item_id_map.h",
		"generated/struct_struct_postfix_expression_part_details_list.h",
		"generated/struct_struct_postfix_expression_part_details_to_struct_postfix_expression_part_id_map.h",
		"generated/struct_struct_unscoped_tag_specifier_details_list.h",
		"generated/struct_struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map.h",
		"generated/struct_struct_scoped_typename_specifier_details_list.h",
		"generated/struct_struct_scoped_typename_specifier_details_to_struct_scoped_typename_specifier_id_map.h",
		"generated/struct_struct_scoped_tag_specifier_details_list.h",
		"generated/struct_struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map.h",
		"generated/struct_struct_unscoped_union_specifier_details_list.h",
		"generated/struct_struct_unscoped_union_specifier_details_to_struct_unscoped_union_specifier_id_map.h",
		"generated/struct_struct_unscoped_struct_specifier_details_list.h",
		"generated/struct_struct_unscoped_struct_specifier_details_to_struct_unscoped_struct_specifier_id_map.h",
		"generated/struct_struct_unscoped_enum_specifier_details_list.h",
		"generated/struct_struct_unscoped_enum_specifier_details_to_struct_unscoped_enum_specifier_id_map.h",
		"generated/struct_struct_bitfield_details_list.h",
		"generated/struct_struct_bitfield_details_to_struct_bitfield_id_map.h",
		"generated/struct_struct_jump_statement_details_list.h",
		"generated/struct_struct_jump_statement_details_to_struct_jump_statement_id_map.h",
		"generated/struct_struct_switch_statement_details_list.h",
		"generated/struct_struct_switch_statement_details_to_struct_switch_statement_id_map.h",
		"generated/struct_struct_if_statement_details_list.h",
		"generated/struct_struct_if_statement_details_to_struct_if_statement_id_map.h",
		"generated/struct_struct_while_statement_details_list.h",
		"generated/struct_struct_while_statement_details_to_struct_while_statement_id_map.h",
		"generated/struct_struct_for_statement_details_list.h",
		"generated/struct_struct_for_statement_details_to_struct_for_statement_id_map.h",
		"generated/struct_struct_code_statement_details_list.h",
		"generated/struct_struct_code_statement_details_to_struct_code_statement_id_map.h",
		"generated/struct_struct_labeled_statement_details_list.h",
		"generated/struct_struct_labeled_statement_details_to_struct_labeled_statement_id_map.h",
		"generated/struct_struct_postfix_expression_details_list.h",
		"generated/struct_struct_postfix_expression_details_to_struct_postfix_expression_id_map.h",
		"generated/struct_struct_primary_expression_details_list.h",
		"generated/struct_struct_primary_expression_details_to_struct_primary_expression_id_map.h",
		"generated/struct_struct_any_statement_details_list.h",
		"generated/struct_struct_any_statement_details_to_struct_any_statement_id_map.h",
		"generated/struct_struct_scoped_statement_details_list.h",
		"generated/struct_struct_scoped_statement_details_to_struct_scoped_statement_id_map.h",
		"generated/struct_struct_declaration_statement_details_list.h",
		"generated/struct_struct_declaration_statement_details_to_struct_declaration_statement_id_map.h",
		"generated/struct_struct_function_definition_details_list.h",
		"generated/struct_struct_function_definition_details_to_struct_function_definition_id_map.h",
		"generated/struct_struct_simple_declaration_details_list.h",
		"generated/struct_struct_simple_declaration_details_to_struct_simple_declaration_id_map.h",
		"generated/struct_struct_expression_details_list.h",
		"generated/struct_struct_expression_details_to_struct_expression_id_map.h",
		"generated/struct_struct_cast_expression_details_list.h",
		"generated/struct_struct_cast_expression_details_to_struct_cast_expression_id_map.h",
		"generated/struct_struct_unary_expression_details_list.h",
		"generated/struct_struct_unary_expression_details_to_struct_unary_expression_id_map.h",
		"generated/struct_struct_binary_expression_details_list.h",
		"generated/struct_struct_binary_expression_details_to_struct_binary_expression_id_map.h",
		"generated/struct_struct_ternary_expression_details_list.h",
		"generated/struct_struct_ternary_expression_details_to_struct_ternary_expression_id_map.h",
		"generated/struct_struct_constant_description_details_list.h",
		"generated/struct_struct_constant_description_details_to_struct_constant_description_id_map.h",
		"generated/struct_struct_data_package_details_list.h",
		"generated/struct_struct_data_package_details_to_struct_data_package_id_map.h",
		"generated/struct_struct_error_node_details_list.h",
		"generated/struct_struct_error_node_details_to_struct_error_node_id_map.h",
		"generated/struct_struct_byte_package_details_list.h",
		"generated/struct_struct_byte_package_details_to_struct_byte_package_id_map.h",
		"generated/struct_struct_hword_package_details_list.h",
		"generated/struct_struct_hword_package_details_to_struct_hword_package_id_map.h",
		"generated/struct_struct_word_package_details_list.h",
		"generated/struct_struct_word_package_details_to_struct_word_package_id_map.h",
		"generated/struct_struct_qword_package_details_list.h",
		"generated/struct_struct_qword_package_details_to_struct_qword_package_id_map.h",
		"generated/struct_struct_dword_package_details_list.h",
		"generated/struct_struct_dword_package_details_to_struct_dword_package_id_map.h",
		"generated/struct_struct_string_package_details_list.h",
		"generated/struct_struct_string_package_details_to_struct_string_package_id_map.h",
		"generated/struct_struct_identifier_details_list.h",
		"generated/struct_struct_identifier_details_to_struct_identifier_id_map.h",
		"generated/struct_struct_scope_guid_details_list.h",
		"generated/struct_struct_scope_guid_details_to_struct_scope_guid_id_map.h",
		"generated/struct_struct_anonymous_entry_details_list.h",
		"generated/struct_struct_anonymous_entry_details_to_struct_anonymous_entry_id_map.h",
		"stop"

	);

}

