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

#include "../recc-implementation/compiler_interface_phase_2.h"

void make_compare(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer, unsigned char *, const char *, const char *);
void make_list(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_kvp(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_map(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer, const char *);
void make_merge_sort(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_binary_search(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_memory_pool(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void register_generated_aggregate_include(struct build_state *, const char *, ...);
void va_register_generated_aggregate_include(struct build_state *, const char *, va_list);
void register_generated_type(struct build_state *, const char *, const char *, ...);
void va_register_generated_type(struct build_state *, const char *, const char *, va_list);

void new_register_data_structures_objects(struct build_state *);
void new_register_data_structures_objects(struct build_state * state){
	struct bootstrap_state bootstrap_state;
	struct bootstrap_state * b = &bootstrap_state;
	const char * simple = "recc-implementation/templates/comparison-functions/simple_compare.c";
	const char * strcmp = "recc-implementation/templates/comparison-functions/strcmp_compare.c";
	const char * generic_list_item_cmp = "recc-implementation/templates/comparison-functions/generic_list_item_compare.c";
	const char * token_cmp = "recc-implementation/templates/comparison-functions/c_lexer_token_compare.c";
	const char * details_cmp = "recc-implementation/templates/comparison-functions/type_engine_details_compare.c";
	const char * id_cmp = "recc-implementation/templates/comparison-functions/type_engine_id_compare.c";
	const char * general_type_details_cmp = "recc-implementation/templates/comparison-functions/type_engine_general_type_details_compare.c";
	const char * type_engine_list_item_cmp = "recc-implementation/templates/comparison-functions/type_engine_list_item_compare.c";
	const char * type_engine_scoped_type_cmp = "recc-implementation/templates/comparison-functions/type_engine_scoped_type_compare.c";
	bootstrap_state_create(&bootstrap_state);

	make_map(state, b, make_beb_list(2, "struct unscoped_tag_specifier_id", "struct struct_or_union_definition *"), make_beb_list(2, "generated/struct_unscoped_tag_specifier_id.h", "generated/struct_struct_or_union_definition.h"), id_cmp);
	make_map(state, b, make_beb_list(2, "struct unscoped_tag_specifier_id", "struct enum_definition *"), make_beb_list(2, "generated/struct_unscoped_tag_specifier_id.h", "generated/struct_enum_definition.h"), id_cmp);
	make_map(state, b, make_beb_list(2, "struct general_type_id", "unsigned int"), make_beb_list(1, "generated/struct_general_type_id.h"), id_cmp);
	make_map(state, b, make_beb_list(2, "struct identifier_id", "struct general_type_id"), make_beb_list(2, "generated/struct_identifier_id.h", "generated/struct_general_type_id.h"), id_cmp);
	make_map(state, b, make_beb_list(2, "struct identifier_id", "unsigned int"), make_beb_list(1, "generated/struct_identifier_id.h"), id_cmp);
	make_map(state, b, make_beb_list(2, "struct unscoped_tag_specifier_id", "unsigned int"), make_beb_list(1, "generated/struct_unscoped_tag_specifier_id.h"), id_cmp);
	make_map(state, b, make_beb_list(2, "struct identifier_id", "struct unscoped_tag_specifier_id"), make_beb_list(2, "generated/struct_identifier_id.h", "generated/struct_unscoped_tag_specifier_id.h"), id_cmp);
	make_map(state, b, make_beb_list(2, "struct general_type_list_item_details", "struct general_type_list_item_id"), make_beb_list(2, "generated/struct_general_type_list_item_details.h", "generated/struct_general_type_list_item_id.h"), type_engine_list_item_cmp);
	make_map(state, b, make_beb_list(2, "struct declarator_part_list_item_details", "struct declarator_part_list_item_id"), make_beb_list(2, "generated/struct_declarator_part_list_item_details.h", "generated/struct_declarator_part_list_item_id.h"), type_engine_list_item_cmp);
	make_map(state, b, make_beb_list(2, "struct declarator_part_details", "struct declarator_part_id"), make_beb_list(2, "generated/struct_declarator_part_details.h", "generated/struct_declarator_part_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct struct_specifier_details", "struct struct_specifier_id"), make_beb_list(2, "generated/struct_struct_specifier_details.h", "generated/struct_struct_specifier_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct union_specifier_details", "struct union_specifier_id"), make_beb_list(2, "generated/struct_union_specifier_details.h", "generated/struct_union_specifier_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct enum_specifier_details", "struct enum_specifier_id"), make_beb_list(2, "generated/struct_enum_specifier_details.h", "generated/struct_enum_specifier_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct function_part_details", "struct function_part_id"), make_beb_list(2, "generated/struct_function_part_details.h", "generated/struct_function_part_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct array_part_details", "struct array_part_id"), make_beb_list(2, "generated/struct_array_part_details.h", "generated/struct_array_part_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct pointer_part_details", "struct pointer_part_id"), make_beb_list(2, "generated/struct_pointer_part_details.h", "generated/struct_pointer_part_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct specifier_or_qualifier_list_item_details", "struct specifier_or_qualifier_list_item_id"), make_beb_list(2, "generated/struct_specifier_or_qualifier_list_item_details.h", "generated/struct_specifier_or_qualifier_list_item_id.h"), type_engine_list_item_cmp);
	make_map(state, b, make_beb_list(2, "struct scoped_tag_specifier_details", "struct scoped_tag_specifier_id"), make_beb_list(2, "generated/struct_scoped_tag_specifier_details.h", "generated/struct_scoped_tag_specifier_id.h"), type_engine_scoped_type_cmp);
	make_map(state, b, make_beb_list(2, "struct unscoped_tag_specifier_details", "struct unscoped_tag_specifier_id"), make_beb_list(2, "generated/struct_unscoped_tag_specifier_details.h", "generated/struct_unscoped_tag_specifier_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct specifier_or_qualifier_details", "struct specifier_or_qualifier_id"), make_beb_list(2, "generated/struct_specifier_or_qualifier_details.h", "generated/struct_specifier_or_qualifier_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct general_type_details", "struct general_type_id"), make_beb_list(2, "generated/struct_general_type_details.h", "generated/struct_general_type_id.h"), general_type_details_cmp);
	make_map(state, b, make_beb_list(2, "struct bitfield_or_declarator_details", "struct bitfield_or_declarator_id"), make_beb_list(2, "generated/struct_bitfield_or_declarator_details.h", "generated/struct_bitfield_or_declarator_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct parameter_list_details", "struct parameter_list_id"), make_beb_list(2, "generated/struct_parameter_list_details.h", "generated/struct_parameter_list_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "struct aggregate_specifier_details", "struct aggregate_specifier_id"), make_beb_list(2, "generated/struct_aggregate_specifier_details.h", "generated/struct_aggregate_specifier_id.h"), details_cmp);
	make_map(state, b, make_beb_list(2, "enum c_token_type", "struct type_qualifier_id"), make_beb_list(2, "generated/enum_c_token_type.h", "generated/struct_type_qualifier_id.h"), simple);
	make_map(state, b, make_beb_list(2, "enum c_token_type", "struct simple_storage_class_specifier_id"), make_beb_list(2, "generated/enum_c_token_type.h", "generated/struct_simple_storage_class_specifier_id.h"), simple);
	make_map(state, b, make_beb_list(2, "enum c_token_type", "struct simple_type_specifier_id"), make_beb_list(2, "generated/enum_c_token_type.h", "generated/struct_simple_type_specifier_id.h"), simple);
	make_map(state, b, make_beb_list(2, "struct generic_list_item", "unsigned int"), make_beb_list(1, "generated/struct_generic_list_item.h"), generic_list_item_cmp);
	make_map(state, b, make_beb_list(2, "struct normalized_declaration_element *", "unsigned int"), make_beb_list(1, "generated/struct_normalized_declaration_element.h"), simple);
	make_map(state, b, make_beb_list(2, "struct parser_node *", "struct parser_node_id"), make_beb_list(2, "generated/struct_parser_node.h", "generated/struct_parser_node_id.h"), simple);
	make_map(state, b, make_beb_list(2, "struct parser_node *", "struct scope_level_id"), make_beb_list(2, "generated/struct_parser_node.h", "generated/struct_scope_level_id.h"), simple);
	make_map(state, b, make_beb_list(2, "struct scope_level *", "struct scope_level_id"), make_beb_list(2, "generated/struct_scope_level.h", "generated/struct_scope_level_id.h"), simple);
	make_map(state, b, make_beb_list(2, "unsigned int", "void *"), make_beb_list(0), simple);
	make_map(state, b, make_beb_list(2, "unsigned int", "unsigned int"), make_beb_list(0), simple);
	make_map(state, b, make_beb_list(2, "void *", "unsigned int"), make_beb_list(0), simple);
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct linker_symbol *"), make_beb_list(1, "generated/struct_linker_symbol.h"), strcmp);
	make_map(state, b, make_beb_list(2, "unsigned char *", "unsigned char *"), make_beb_list(0), strcmp);
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct constant_description *"), make_beb_list(1, "generated/struct_constant_description.h"), strcmp);
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct macro_parameter *"), make_beb_list(1, "generated/struct_macro_parameter.h"), strcmp);
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct macro_definition *"), make_beb_list(1, "generated/struct_macro_definition.h"), strcmp);
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct special_macro_definition *"), make_beb_list(1, "generated/struct_special_macro_definition.h"), strcmp);
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct namespace_object *"), make_beb_list(1, "generated/struct_namespace_object.h"), strcmp);
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct entity *"), make_beb_list(1, "recc-implementation/struct_entity.h"), strcmp);
	make_map(state, b, make_beb_list(2, "struct entity_relationship", "struct struct_entity_ptr_list *"), make_beb_list(2, "recc-implementation/struct_entity_relationship.h", "generated/struct_struct_entity_ptr_list.h"), "recc-implementation/templates/comparison-functions/entity_relationship_compare.c");
	make_map(state, b, make_beb_list(2, "struct c_lexer_token *", "struct c_lexer_token *"), make_beb_list(1, "generated/struct_c_lexer_token.h"), simple);
	make_map(state, b, make_beb_list(2, "struct c_lexer_token *", "unsigned char *"), make_beb_list(1, "generated/struct_c_lexer_token.h"), simple);
	make_map(state, b, make_beb_list(2, "struct c_lexer_token", "struct identifier_id"), make_beb_list(2, "generated/struct_c_lexer_token.h", "generated/struct_identifier_id.h"), token_cmp);

	make_list(state, b, make_beb_list(1, "enum type_engine_operation_type"), make_beb_list(1, "generated/enum_type_engine_operation_type.h"));
	make_list(state, b, make_beb_list(1, "unsigned int *"), make_beb_list(0));
	make_list(state, b, make_beb_list(1, "struct c_lexer_state *"), make_beb_list(1, "generated/struct_c_lexer_state.h"));
	make_list(state, b, make_beb_list(1, "char"), make_beb_list(0));
	make_list(state, b, make_beb_list(1, "char *"), make_beb_list(0));
	make_list(state, b, make_beb_list(1, "unsigned char"), make_beb_list(0));
	make_list(state, b, make_beb_list(1, "struct parser_error"), make_beb_list(1, "generated/struct_parser_error.h"));
	make_list(state, b, make_beb_list(1, "struct c_lexer_token"), make_beb_list(1, "generated/struct_c_lexer_token.h"));
	make_list(state, b, make_beb_list(1, "struct c_lexer_token *"), make_beb_list(1, "generated/struct_c_lexer_token.h"));
	make_list(state, b, make_beb_list(1, "struct parser_node *"), make_beb_list(1, "generated/struct_parser_node.h"));
	make_list(state, b, make_beb_list(1, "struct parser_state *"), make_beb_list(1, "generated/struct_parser_state.h"));
	make_list(state, b, make_beb_list(1, "struct struct_c_lexer_token_ptr_list *"), make_beb_list(1, "generated/struct_struct_c_lexer_token_ptr_list.h"));
	make_list(state, b, make_beb_list(1, "struct l2_lexer_token *"), make_beb_list(1, "generated/struct_l2_lexer_token.h"));
	make_list(state, b, make_beb_list(1, "struct normalized_specifier *"), make_beb_list(1, "generated/struct_normalized_specifier.h"));
	make_list(state, b, make_beb_list(1, "struct normalized_declarator *"), make_beb_list(1, "generated/struct_normalized_declarator.h"));
	make_list(state, b, make_beb_list(1, "struct normalized_declaration_element *"), make_beb_list(1, "generated/struct_normalized_declaration_element.h"));
	make_list(state, b, make_beb_list(1, "struct switch_frame *"), make_beb_list(1, "generated/struct_switch_frame.h"));
	make_list(state, b, make_beb_list(1, "struct unsigned_char_list *"), make_beb_list(1, "generated/struct_unsigned_char_list.h"));
	make_list(state, b, make_beb_list(1, "struct l2_item *"), make_beb_list(1, "generated/struct_l2_item.h"));
	make_list(state, b, make_beb_list(1, "struct l2_lexer_state *"), make_beb_list(1, "generated/struct_l2_lexer_state.h"));
	make_list(state, b, make_beb_list(1, "struct l2_parser_state *"), make_beb_list(1, "generated/struct_l2_parser_state.h"));
	make_list(state, b, make_beb_list(1, "struct linker_file *"), make_beb_list(1, "generated/struct_linker_file.h"));
	make_list(state, b, make_beb_list(1, "struct type_description *"), make_beb_list(1, "generated/struct_type_description.h"));
	make_list(state, b, make_beb_list(1, "struct constant_initializer_level *"), make_beb_list(1, "generated/struct_constant_initializer_level.h"));
	make_list(state, b, make_beb_list(1, "struct compile_time_constant *"), make_beb_list(1, "generated/struct_compile_time_constant.h"));
	make_list(state, b, make_beb_list(1, "struct type_traversal *"), make_beb_list(1, "generated/struct_type_traversal.h"));
	make_list(state, b, make_beb_list(1, "struct preprocessor_if_branch *"), make_beb_list(1, "generated/struct_preprocessor_if_branch.h"));
	make_list(state, b, make_beb_list(1, "struct preprocessor_file_context *"), make_beb_list(1, "generated/struct_preprocessor_file_context.h"));
	make_list(state, b, make_beb_list(1, "struct preprocessor_macro_level *"), make_beb_list(1, "generated/struct_preprocessor_macro_level.h"));
	make_list(state, b, make_beb_list(1, "struct linker_object *"), make_beb_list(1, "generated/struct_linker_object.h"));
	make_list(state, b, make_beb_list(1, "struct start_end"), make_beb_list(1, "generated/struct_start_end.h"));
	make_list(state, b, make_beb_list(1, "struct linker_region"), make_beb_list(1, "generated/struct_linker_region.h"));
	make_list(state, b, make_beb_list(1, "struct parser_operation"), make_beb_list(1, "generated/struct_parser_operation.h"));
	make_list(state, b, make_beb_list(1, "struct l2_parser_operation"), make_beb_list(1, "generated/struct_l2_parser_operation.h"));
	make_list(state, b, make_beb_list(1, "struct tag_declaration_namespace *"), make_beb_list(1, "generated/struct_tag_declaration_namespace.h"));
	make_list(state, b, make_beb_list(1, "struct identifier_declaration_namespace *"), make_beb_list(1, "generated/struct_identifier_declaration_namespace.h"));

	make_merge_sort(state, b, make_beb_list(1, "struct start_end"), make_beb_list(1, "generated/struct_start_end.h"));
	make_merge_sort(state, b, make_beb_list(1, "struct linker_file *"), make_beb_list(1, "generated/struct_linker_file.h"));
	make_merge_sort(state, b, make_beb_list(1, "unsigned int"), make_beb_list(0));

	make_binary_search(state, b, make_beb_list(1, "unsigned int"), make_beb_list(0));

	make_memory_pool(state, b, make_beb_list(1, "struct regex_computation_node"), make_beb_list(1, "generated/struct_regex_computation_node.h"));
	make_memory_pool(state, b, make_beb_list(1, "struct regex_parser_node"), make_beb_list(1, "generated/struct_regex_parser_node.h"));
	make_memory_pool(state, b, make_beb_list(1, "struct c_lexer_token"), make_beb_list(1, "generated/struct_c_lexer_token.h"));
	make_memory_pool(state, b, make_beb_list(1, "struct parser_node"), make_beb_list(1, "generated/struct_parser_node.h"));
	make_memory_pool(state, b, make_beb_list(1, "struct l2_parser_node"), make_beb_list(1, "generated/struct_l2_parser_node.h"));
	make_memory_pool(state, b, make_beb_list(1, "struct l2_lexer_token"), make_beb_list(1, "generated/struct_l2_lexer_token.h"));
	make_memory_pool(state, b, make_beb_list(1, "struct type_description"), make_beb_list(1, "generated/struct_type_description.h"));
	make_memory_pool(state, b, make_beb_list(1, "struct l2_item"), make_beb_list(1, "generated/struct_l2_item.h"));
	make_memory_pool(state, b, make_beb_list(1, "struct linker_symbol"), make_beb_list(1, "generated/struct_linker_symbol.h"));
	make_memory_pool(state, b, make_beb_list(1, "void *"), make_beb_list(0));


	make_kvp(state, b, make_beb_list(2, "unsigned char *", "struct linker_file *"), make_beb_list(1, "generated/struct_linker_file.h"));

	make_compare(state, b, make_beb_list(1, "struct start_end"), make_beb_list(1, "generated/struct_start_end.h"), (unsigned char *)"generated/struct_start_end_compare.h", "recc-implementation/templates/comparison-functions/simple_compare.h", "recc-implementation/templates/comparison-functions/start_end_compare.c");
	make_compare(state, b, make_beb_list(1, "struct unsigned_char_ptr_to_struct_linker_file_ptr_key_value_pair"), make_beb_list(1, "generated/struct_unsigned_char_ptr_to_struct_linker_file_ptr_key_value_pair.h"), (unsigned char *)"generated/struct_unsigned_char_ptr_to_struct_linker_file_ptr_key_value_pair_compare.h", "recc-implementation/templates/comparison-functions/simple_compare.h", "recc-implementation/templates/comparison-functions/strcmp_compare.c");

	bootstrap_state_output_makefiles(b, "");
	bootstrap_state_destroy(b);

	register_generated_type(state, "types/regex_engine/struct_regex_match.h", "generated/struct_regex_match.h",
		"stop"
	);

	register_generated_type(state, "types/regex_engine/struct_regex_computation_node.h", "generated/struct_regex_computation_node.h",
		"generated/enum_regex_computation_node_type.h",
		"generated/struct_character_class_range.h",
		"stop"
	);

	register_generated_type(state, "types/regex_engine/struct_regex_parser_node.h", "generated/struct_regex_parser_node.h",
		"generated/enum_regex_parser_node_type.h",
		"stop"
	);

	register_generated_type(state, "types/regex_engine/struct_regex_matcher_state.h", "generated/struct_regex_matcher_state.h",
		"generated/struct_regex_compiler_state.h",
		"generated/struct_regex_match.h",
		"stop"
	);


	register_generated_type(state, "types/regex_engine/enum_regex_computation_node_type.h", "generated/enum_regex_computation_node_type.h",
		"stop"
	);
	register_generated_type(state, "types/regex_engine/struct_character_class_range.h", "generated/struct_character_class_range.h",
		"stop"
	);
	register_generated_type(state, "types/regex_engine/enum_regex_parser_node_type.h", "generated/enum_regex_parser_node_type.h",
		"stop"
	);
	register_generated_type(state, "types/regex_engine/struct_regex_compiler_error.h", "generated/struct_regex_compiler_error.h",
		"generated/enum_regex_compiler_error_type.h",
		"stop"
	);
	register_generated_type(state, "types/regex_engine/enum_regex_compiler_error_type.h", "generated/enum_regex_compiler_error_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_l2_parser_operation_type.h", "generated/enum_l2_parser_operation_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_object_location.h", "generated/enum_object_location.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_normalized_declaration_type.h", "generated/enum_normalized_declaration_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_parser_checkpoint.h", "generated/struct_parser_checkpoint.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_declaration_or_definition.h", "generated/enum_declaration_or_definition.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_scope_type.h", "generated/enum_scope_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_namespace_object_change.h", "generated/struct_namespace_object_change.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_normalized_declarator_type.h", "generated/enum_normalized_declarator_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_current_function_change.h", "generated/struct_current_function_change.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_add_or_remove.h", "generated/enum_add_or_remove.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_normalized_specifier_type.h", "generated/enum_normalized_specifier_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_parser_error_type.h", "generated/enum_parser_error_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_l2_node_type.h", "generated/enum_l2_node_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_namespace_modification.h", "generated/struct_namespace_modification.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_parser_operation_type.h", "generated/enum_parser_operation_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_value_type.h", "generated/enum_value_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_node_type.h", "generated/enum_node_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/enum_type_class.h", "generated/enum_type_class.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_parser_node_id.h", "generated/struct_parser_node_id.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_scope_level_id.h", "generated/struct_scope_level_id.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_identifier_id.h", "generated/struct_identifier_id.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_union_specifier_id.h", "generated/struct_union_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_enum_specifier_id.h", "generated/struct_enum_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_union_specifier_details.h", "generated/struct_union_specifier_details.h",
		"generated/enum_type_engine_union_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_enum_specifier_details.h", "generated/struct_enum_specifier_details.h",
		"generated/enum_type_engine_enum_specifier_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_struct_specifier_id.h", "generated/struct_struct_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_struct_specifier_details.h", "generated/struct_struct_specifier_details.h",
		"generated/enum_type_engine_struct_specifier_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_array_part_id.h", "generated/struct_array_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_array_part_details.h", "generated/struct_array_part_details.h",
		"generated/enum_type_engine_array_part_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_declarator_part_id.h", "generated/struct_declarator_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_declarator_part_details.h", "generated/struct_declarator_part_details.h",
		"generated/enum_type_engine_declarator_part_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_function_part_id.h", "generated/struct_function_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_function_part_details.h", "generated/struct_function_part_details.h",
		"generated/enum_type_engine_function_part_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_pointer_part_id.h", "generated/struct_pointer_part_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_pointer_part_details.h", "generated/struct_pointer_part_details.h",
		"generated/enum_type_engine_pointer_part_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_declarator_part_list_item_id.h", "generated/struct_declarator_part_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_declarator_part_list_item_details.h", "generated/struct_declarator_part_list_item_details.h",
		"generated/enum_type_engine_declarator_part_list_item_kind.h",
		"generated/struct_declarator_part_list_item_id.h",
		"generated/struct_declarator_part_id.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_general_type_list_item_id.h", "generated/struct_general_type_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_general_type_list_item_details.h", "generated/struct_general_type_list_item_details.h",
		"generated/enum_type_engine_general_type_list_item_kind.h",
		"generated/struct_general_type_list_item_id.h",
		"generated/struct_general_type_id.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_specifier_or_qualifier_list_item_id.h", "generated/struct_specifier_or_qualifier_list_item_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_specifier_or_qualifier_list_item_details.h", "generated/struct_specifier_or_qualifier_list_item_details.h",
		"generated/enum_type_engine_specifier_or_qualifier_list_item_kind.h",
		"generated/struct_specifier_or_qualifier_list_item_id.h",
		"generated/struct_specifier_or_qualifier_id.h",
		"stop"
	);


	register_generated_type(state, "types/type_engine/struct_parameter_list_id.h", "generated/struct_parameter_list_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_parameter_list_details.h", "generated/struct_parameter_list_details.h",
		"generated/enum_type_engine_parameter_list_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_bitfield_or_declarator_id.h", "generated/struct_bitfield_or_declarator_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_bitfield_or_declarator_details.h", "generated/struct_bitfield_or_declarator_details.h",
		"generated/enum_type_engine_bitfield_or_declarator_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_unscoped_tag_specifier_id.h", "generated/struct_unscoped_tag_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_unscoped_tag_specifier_details.h", "generated/struct_unscoped_tag_specifier_details.h",
		"generated/enum_type_engine_unscoped_tag_specifier_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_scoped_tag_specifier_id.h", "generated/struct_scoped_tag_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_scoped_tag_specifier_details.h", "generated/struct_scoped_tag_specifier_details.h",
		"generated/struct_scope_level_id.h",
		"generated/struct_unscoped_tag_specifier_id.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_specifier_or_qualifier_id.h", "generated/struct_specifier_or_qualifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_specifier_or_qualifier_details.h", "generated/struct_specifier_or_qualifier_details.h",
		"generated/enum_type_engine_specifier_or_qualifier_kind.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_general_type_id.h", "generated/struct_general_type_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_general_type_details.h", "generated/struct_general_type_details.h",
		"generated/struct_bitfield_or_declarator_id.h",
		"generated/struct_specifier_or_qualifier_list_item_id.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_aggregate_specifier_id.h", "generated/struct_aggregate_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_aggregate_specifier_details.h", "generated/struct_aggregate_specifier_details.h",
		"generated/enum_type_engine_aggregate_specifier_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_print_type.h", "generated/enum_type_engine_print_type.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_aggregate_specifier_kind.h", "generated/enum_type_engine_aggregate_specifier_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_specifier_or_qualifier_kind.h", "generated/enum_type_engine_specifier_or_qualifier_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_specifier_or_qualifier_list_item_kind.h", "generated/enum_type_engine_specifier_or_qualifier_list_item_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_declarator_part_list_item_kind.h", "generated/enum_type_engine_declarator_part_list_item_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_general_type_list_item_kind.h", "generated/enum_type_engine_general_type_list_item_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_declarator_part_kind.h", "generated/enum_type_engine_declarator_part_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_parameter_list_kind.h", "generated/enum_type_engine_parameter_list_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_pointer_part_kind.h", "generated/enum_type_engine_pointer_part_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_function_part_kind.h", "generated/enum_type_engine_function_part_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_array_part_kind.h", "generated/enum_type_engine_array_part_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_bitfield_or_declarator_kind.h", "generated/enum_type_engine_bitfield_or_declarator_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_unscoped_tag_specifier_kind.h", "generated/enum_type_engine_unscoped_tag_specifier_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_enum_specifier_kind.h", "generated/enum_type_engine_enum_specifier_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_union_specifier_kind.h", "generated/enum_type_engine_union_specifier_kind.h",
		"stop"
	);
        register_generated_type(state, "types/type_engine/enum_type_engine_struct_specifier_kind.h", "generated/enum_type_engine_struct_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/enum_type_engine_operation_type.h", "generated/enum_type_engine_operation_type.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/enum_scope_level_type.h", "generated/enum_scope_level_type.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_identifier_from_declarator.h", "generated/struct_identifier_from_declarator.h",
		"generated/struct_identifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_enum_definition.h", "generated/struct_enum_definition.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_function_definition.h", "generated/struct_function_definition.h",
		"stop"
	);
	register_generated_type(state, "types/code_generator/enum_copy_method.h", "generated/enum_copy_method.h",
		"stop"
	);
	register_generated_type(state, "types/l0_generator/enum_l0_language_type.h", "generated/enum_l0_language_type.h",
		"stop"
	);
	register_generated_type(state, "types/builtin/struct_printing_state.h", "generated/struct_printing_state.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/struct_preprocessor_if_branch.h", "generated/struct_preprocessor_if_branch.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/enum_special_macro_type.h", "generated/enum_special_macro_type.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/enum_macro_definition_type.h", "generated/enum_macro_definition_type.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/struct_macro_parameter.h", "generated/struct_macro_parameter.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/struct_preprocessor_file_context.h", "generated/struct_preprocessor_file_context.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/struct_preprocessor_state.h", "generated/struct_preprocessor_state.h",
		"stop"
	);
	register_generated_type(state, "types/recc-implementation/struct_generic_list_item.h", "generated/struct_generic_list_item.h",
		"stop"
	);
	register_generated_type(state, "types/recc-implementation/enum_entity_type.h", "generated/enum_entity_type.h",
		"stop"
	);
	register_generated_type(state, "types/linker/enum_relocation_type.h", "generated/enum_relocation_type.h",
		"stop"
	);
	register_generated_type(state, "types/linker/struct_linker_region.h", "generated/struct_linker_region.h",
		"stop"
	);
	register_generated_type(state, "types/lexer/enum_c_token_type.h", "generated/enum_c_token_type.h",
		"stop"
	);
	register_generated_type(state, "types/lexer/enum_l2_token_type.h", "generated/enum_l2_token_type.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_simple_type_specifier_id.h", "generated/struct_simple_type_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_simple_storage_class_specifier_id.h", "generated/struct_simple_storage_class_specifier_id.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_type_qualifier_id.h", "generated/struct_type_qualifier_id.h",
		"stop"
	);

	register_generated_type(state, "types/parser/struct_type_description.h", "generated/struct_type_description.h",
		"generated/enum_value_type.h",
		"stop"
	);
	register_generated_type(state, "types/regex_engine/struct_regex_compiler_state.h", "generated/struct_regex_compiler_state.h",
		"generated/struct_regex_compiler_error.h",
		"generated/struct_regex_parser_node.h",
		"generated/struct_regex_computation_node.h",
		"generated/struct_memory_pool_collection.h",
		"generated/enum_regex_compiler_error_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_normalized_declarator.h", "generated/struct_normalized_declarator.h",
		"generated/struct_parser_node.h",
		"generated/enum_normalized_declarator_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_first_and_last_namespace_object.h", "generated/struct_first_and_last_namespace_object.h",
		"generated/struct_namespace_object.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_namespace_object.h", "generated/struct_namespace_object.h",
		"generated/struct_struct_normalized_declaration_element_ptr_list.h",
		"generated/struct_scope_level.h",
		"generated/enum_object_location.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_parser_state.h", "generated/struct_parser_state.h",
		"generated/struct_struct_normalized_declaration_element_ptr_to_unsigned_int_map.h",
		"generated/struct_type_engine_state.h",
		"generated/struct_struct_parser_operation_list.h",
		"generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_map.h",
		"generated/struct_struct_parser_error_list.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_normalized_specifier.h", "generated/struct_normalized_specifier.h",
		"generated/enum_normalized_specifier_type.h",
		"generated/struct_parser_node.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_l2_parser_state.h", "generated/struct_l2_parser_state.h",
		"generated/struct_memory_pool_collection.h",
		"generated/struct_l2_lexer_state.h",
		"generated/struct_l2_parser_node.h",
		"generated/struct_unsigned_char_list.h",
		"generated/struct_struct_l2_parser_operation_list.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_parser_error.h", "generated/struct_parser_error.h",
		"generated/enum_parser_error_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_parser_node.h", "generated/struct_parser_node.h",
		"generated/enum_node_type.h",
		"generated/struct_c_lexer_token.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_normalized_declaration_element.h", "generated/struct_normalized_declaration_element.h",
		"generated/struct_normalized_declaration_set.h",
		"generated/struct_normalized_declarator.h",
		"generated/struct_struct_normalized_specifier_ptr_list.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_parser_operation.h", "generated/struct_parser_operation.h",
		"generated/enum_parser_operation_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_constant_description.h", "generated/struct_constant_description.h",
		"generated/struct_type_description.h",
		"generated/enum_c_token_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_normalized_declaration_set.h", "generated/struct_normalized_declaration_set.h",
		"generated/struct_parser_node.h",
		"generated/struct_struct_normalized_specifier_ptr_list.h",
		"generated/struct_struct_normalized_declarator_ptr_list.h",
		"generated/enum_normalized_declaration_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_l2_parser_operation.h", "generated/struct_l2_parser_operation.h",
		"generated/enum_l2_parser_operation_type.h",
		"stop"
	);
	register_generated_type(state, "types/parser/struct_l2_parser_node.h", "generated/struct_l2_parser_node.h",
		"generated/enum_l2_node_type.h",
		"generated/struct_l2_lexer_token.h",
		"stop"
	);

	register_generated_type(state, "types/type_engine/struct_identifier_declaration_namespace.h", "generated/struct_identifier_declaration_namespace.h",
		"generated/struct_struct_general_type_id_list.h",
		"generated/struct_struct_identifier_id_to_struct_general_type_id_map.h",
		"generated/struct_struct_identifier_id_to_unsigned_int_map.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_tag_declaration_namespace.h", "generated/struct_tag_declaration_namespace.h",
		"generated/struct_struct_unscoped_tag_specifier_id_list.h",
		"generated/struct_struct_identifier_id_to_struct_unscoped_tag_specifier_id_map.h",
		"generated/struct_struct_unscoped_tag_specifier_id_to_unsigned_int_map.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_function_definition_collection.h", "generated/struct_function_definition_collection.h",
		"generated/struct_void_ptr_list.h",
		"generated/struct_unsigned_int_to_void_ptr_map.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_scope_level.h", "generated/struct_scope_level.h",
		"generated/struct_unsigned_char_ptr_list.h",
		"generated/struct_struct_scope_level_ptr_list.h",
		"generated/struct_namespace_object.h",
		"generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_map.h",
		"generated/struct_tag_declaration_namespace.h",
		"generated/struct_identifier_declaration_namespace.h",
		"generated/struct_struct_unscoped_tag_specifier_id_to_struct_struct_or_union_definition_ptr_map.h",
		"generated/struct_struct_unscoped_tag_specifier_id_to_struct_enum_definition_ptr_map.h",
		"generated/enum_scope_level_type.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_struct_or_union_definition.h", "generated/struct_struct_or_union_definition.h",
		"generated/enum_type_engine_unscoped_tag_specifier_kind.h",
		"stop"
	);
	register_generated_type(state, "types/type_engine/struct_type_engine_state.h", "generated/struct_type_engine_state.h",
		"generated/struct_struct_tag_declaration_namespace_ptr_list.h",
		"generated/struct_struct_identifier_declaration_namespace_ptr_list.h",
		"generated/struct_struct_unscoped_tag_specifier_id_list.h",
		"generated/struct_enum_type_engine_operation_type_list.h",
		"generated/struct_struct_identifier_id_list.h",
		"generated/struct_struct_parser_node_ptr_list.h",
		"generated/struct_struct_scope_level_ptr_list.h",
		"generated/struct_struct_c_lexer_token_list.h",
		"generated/struct_struct_generic_list_item_list.h",
		"generated/struct_void_ptr_list.h",
		"generated/struct_unsigned_int_list.h",
		"generated/struct_unsigned_int_to_unsigned_int_map.h",
		"generated/struct_struct_c_lexer_token_to_struct_identifier_id_map.h",
		"generated/struct_struct_scope_level_ptr_to_struct_scope_level_id_map.h",
		"generated/struct_struct_parser_node_ptr_to_struct_parser_node_id_map.h",
		"generated/struct_struct_parser_node_ptr_to_struct_scope_level_id_map.h",
		"generated/struct_struct_generic_list_item_to_unsigned_int_map.h",
		"generated/struct_function_definition_collection.h",
		"generated/struct_enum_c_token_type_list.h",
		"generated/struct_enum_c_token_type_to_struct_simple_type_specifier_id_map.h",
		"generated/struct_enum_c_token_type_to_struct_simple_storage_class_specifier_id_map.h",
		"generated/struct_enum_c_token_type_to_struct_type_qualifier_id_map.h",
		"generated/struct_struct_aggregate_specifier_details_list.h",
		"generated/struct_struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map.h",
		"generated/struct_struct_specifier_or_qualifier_details_list.h",
		"generated/struct_struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map.h",
		"generated/struct_struct_specifier_or_qualifier_list_item_details_list.h",
		"generated/struct_struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map.h",
		"generated/struct_struct_pointer_part_details_list.h",
		"generated/struct_struct_pointer_part_details_to_struct_pointer_part_id_map.h",
		"generated/struct_struct_function_part_details_list.h",
		"generated/struct_struct_function_part_details_to_struct_function_part_id_map.h",
		"generated/struct_struct_array_part_details_list.h",
		"generated/struct_struct_array_part_details_to_struct_array_part_id_map.h",
		"generated/struct_struct_declarator_part_details_list.h",
		"generated/struct_struct_declarator_part_details_to_struct_declarator_part_id_map.h",
		"generated/struct_struct_declarator_part_list_item_details_list.h",
		"generated/struct_struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map.h",
		"generated/struct_struct_parameter_list_details_list.h",
		"generated/struct_struct_parameter_list_details_to_struct_parameter_list_id_map.h",
		"generated/struct_struct_bitfield_or_declarator_details_list.h",
		"generated/struct_struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_map.h",
		"generated/struct_struct_general_type_details_list.h",
		"generated/struct_struct_general_type_details_to_struct_general_type_id_map.h",
		"generated/struct_struct_general_type_list_item_details_list.h",
		"generated/struct_struct_general_type_list_item_details_to_struct_general_type_list_item_id_map.h",
		"generated/struct_struct_unscoped_tag_specifier_details_list.h",
		"generated/struct_struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map.h",
		"generated/struct_struct_enum_specifier_details_list.h",
		"generated/struct_struct_enum_specifier_details_to_struct_enum_specifier_id_map.h",
		"generated/struct_struct_union_specifier_details_list.h",
		"generated/struct_struct_union_specifier_details_to_struct_union_specifier_id_map.h",
		"generated/struct_struct_struct_specifier_details_list.h",
		"generated/struct_struct_struct_specifier_details_to_struct_struct_specifier_id_map.h",
		"generated/struct_struct_scoped_tag_specifier_details_list.h",
		"generated/struct_struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map.h",
		"stop"
	);
	register_generated_type(state, "types/code_generator/struct_compile_time_constant.h", "generated/struct_compile_time_constant.h",
		"generated/struct_constant_description.h",
		"generated/struct_normalized_declaration_element.h",
		"stop"
	);
	register_generated_type(state, "types/code_generator/struct_switch_frame.h", "generated/struct_switch_frame.h",
		"generated/struct_unsigned_int_list.h",
		"stop"
	);
	register_generated_type(state, "types/code_generator/struct_code_gen_state.h", "generated/struct_code_gen_state.h",
		"generated/struct_struct_compile_time_constant_ptr_list.h",
		"generated/struct_struct_constant_description_ptr_list.h",
		"generated/struct_struct_linker_object_ptr_list.h",
		"stop"
	);
	register_generated_type(state, "types/code_generator/struct_constant_initializer_level.h", "generated/struct_constant_initializer_level.h",
		"generated/struct_compile_time_constant.h",
		"generated/struct_struct_constant_initializer_level_ptr_list.h",
		"stop"
	);
	register_generated_type(state, "types/code_generator/struct_type_traversal.h", "generated/struct_type_traversal.h",
		"generated/enum_type_class.h",
		"generated/struct_type_description.h",
		"generated/struct_struct_type_traversal_ptr_list.h",
		"stop"
	);
	register_generated_type(state, "types/l0_generator/struct_l0_generator_state.h", "generated/struct_l0_generator_state.h",
		"generated/struct_l2_parser_state.h",
		"generated/struct_l2_lexer_state.h",
		"generated/enum_l0_language_type.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/struct_preprocessor_macro_level.h", "generated/struct_preprocessor_macro_level.h",
		"generated/struct_c_lexer_token.h",
		"generated/struct_struct_c_lexer_token_ptr_list.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/struct_macro_definition.h", "generated/struct_macro_definition.h",
		"generated/enum_macro_definition_type.h",
		"generated/struct_struct_c_lexer_token_ptr_list.h",
		"generated/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h",
		"stop"
	);
	register_generated_type(state, "types/preprocessor/struct_special_macro_definition.h", "generated/struct_special_macro_definition.h",
		"generated/enum_special_macro_type.h",
		"stop"
	);
	register_generated_type(state, "types/recc-implementation/struct_memory_pool_collection.h", "generated/struct_memory_pool_collection.h",
		"generated/struct_void_ptr_memory_pool.h",
		"generated/struct_void_ptr_to_unsigned_int_map.h",
		"generated/struct_regex_matcher_state.h",
		"stop"
	);
	register_generated_type(state, "types/linker/struct_linker_symbol.h", "generated/struct_linker_symbol.h",
		"generated/struct_l2_item.h",
		"stop"
	);
	register_generated_type(state, "types/linker/struct_start_end.h", "generated/struct_start_end.h",
		"generated/struct_linker_symbol.h",
		"generated/struct_linker_object.h",
		"stop"
	);
	register_generated_type(state, "types/linker/struct_l2_item.h", "generated/struct_l2_item.h",
		"generated/struct_l2_lexer_token.h",
		"stop"
	);
	register_generated_type(state, "types/linker/struct_linker_state.h", "generated/struct_linker_state.h",
		"generated/enum_entity_type.h",
		"generated/struct_struct_linker_region_list.h",
		"stop"
	);
	register_generated_type(state, "types/linker/struct_linker_object.h", "generated/struct_linker_object.h",
		"generated/enum_l2_token_type.h",
		"stop"
	);
	register_generated_type(state, "types/linker/struct_linker_file.h", "generated/struct_linker_file.h",
		"generated/struct_l2_lexer_state.h",
		"generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h",
		"generated/struct_struct_l2_item_ptr_list.h",
		"generated/struct_struct_linker_object_ptr_list.h",
		"generated/enum_relocation_type.h",
		"stop"
	);
	register_generated_type(state, "types/lexer/struct_c_lexer_token.h", "generated/struct_c_lexer_token.h",
		"generated/enum_c_token_type.h",
		"stop"
	);
	register_generated_type(state, "types/lexer/struct_l2_lexer_state.h", "generated/struct_l2_lexer_state.h",
		"generated/struct_common_lexer_state.h",
		"generated/struct_struct_l2_lexer_token_ptr_list.h",
		"stop"
	);
	register_generated_type(state, "types/lexer/struct_common_lexer_state.h", "generated/struct_common_lexer_state.h",
		"generated/struct_memory_pool_collection.h",
		"generated/struct_unsigned_char_list.h",
		"generated/struct_regex_matcher_state.h",
		"stop"
	);
	register_generated_type(state, "types/lexer/struct_c_lexer_state.h", "generated/struct_c_lexer_state.h",
		"generated/struct_common_lexer_state.h",
		"generated/struct_struct_c_lexer_token_ptr_list.h",
		"stop"
	);
	register_generated_type(state, "types/lexer/struct_l2_lexer_token.h", "generated/struct_l2_lexer_token.h",
		"generated/enum_l2_token_type.h",
		"stop"
	);

	register_generated_aggregate_include(state, "generated/lexer_generated_includes.h",
		"generated/struct_regex_matcher_state.h",
		"generated/struct_l2_lexer_token_memory_pool.h",
		"generated/struct_l2_lexer_state.h",
		"generated/struct_c_lexer_state.h",
		"generated/struct_c_lexer_token_memory_pool.h",
		"generated/unsigned_char_list.h",
		"generated/struct_l2_lexer_token_ptr_list.h",
		"generated/struct_c_lexer_token_ptr_list.h",
		"stop"
	);

	register_generated_aggregate_include(state, "generated/parser_generated_includes.h",
		"generated/enum_type_class.h",
		"generated/enum_scope_type.h",
		"generated/struct_current_function_change.h",
		"generated/unsigned_int_list.h",
		"generated/struct_type_description_ptr_list.h",
		"generated/struct_parser_error_list.h",
		"generated/char_ptr_list.h",
		"generated/struct_parser_operation_list.h",
		"generated/struct_normalized_specifier_ptr_list.h",
		"generated/struct_normalized_declarator_ptr_list.h",
		"generated/struct_normalized_declaration_element_ptr_list.h",
		"generated/struct_namespace_object_ptr_list.h",
		"generated/unsigned_char_list.h",
		"generated/unsigned_char_ptr_list.h",
		"generated/unsigned_char_ptr_to_struct_constant_description_ptr_map.h",
		"generated/struct_parser_node_memory_pool.h",
		"generated/struct_c_lexer_token_memory_pool.h",
		"generated/struct_type_description_memory_pool.h",
		"generated/struct_parser_state.h",
		"generated/struct_type_description.h",
		"generated/struct_normalized_specifier.h",
		"generated/struct_normalized_declarator.h",
		"generated/struct_normalized_declaration_set.h",
		"generated/struct_namespace_modification.h",
		"generated/struct_namespace_object_change.h",
		"generated/struct_first_and_last_namespace_object.h",
		"generated/struct_parser_checkpoint.h",
		"generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_compare.h",
		"generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_compare.h",
		"generated/struct_struct_normalized_declaration_element_ptr_to_unsigned_int_key_value_pair_compare.h",
		"generated/struct_normalized_declaration_element_ptr_to_unsigned_int_map.h",
		"stop"
	);

	register_generated_aggregate_include(state, "generated/code_generator_generated_includes.h",
		"generated/unsigned_int_list.h",
		"generated/struct_type_description_ptr_list.h",
		"generated/unsigned_char_list.h",
		"generated/unsigned_int_ptr_list.h",
		"generated/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h",
		"generated/struct_switch_frame_ptr_list.h",
		"generated/struct_constant_initializer_level_ptr_list.h",
		"generated/struct_type_traversal_ptr_list.h",
		"generated/struct_compile_time_constant_ptr_list.h",
		"generated/struct_linker_object_ptr_list.h",
		"generated/char_ptr_list.h",
		"generated/struct_parser_node_memory_pool.h",
		"generated/struct_c_lexer_token_memory_pool.h",
		"generated/struct_linker_symbol_memory_pool.h",
		"generated/struct_code_gen_state.h",
		"generated/struct_constant_initializer_level.h",
		"generated/struct_type_traversal.h",
		"generated/struct_switch_frame.h",
		"generated/enum_l2_token_type.h",
		"generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_compare.h",
		"stop"
	);

	register_generated_aggregate_include(state, "generated/type_engine_generated_includes.h",
		"generated/struct_type_description_memory_pool.h",
		"generated/struct_scope_level_id.h",
		"generated/struct_aggregate_specifier_id.h",
		"generated/struct_aggregate_specifier_details.h",
		"generated/enum_type_engine_print_type.h",
		"generated/enum_type_engine_aggregate_specifier_kind.h",
		"generated/enum_type_engine_specifier_or_qualifier_kind.h",
		"generated/enum_type_engine_specifier_or_qualifier_list_item_kind.h",
		"generated/enum_type_engine_declarator_part_list_item_kind.h",
		"generated/enum_type_engine_general_type_list_item_kind.h",
		"generated/enum_type_engine_declarator_part_kind.h",
		"generated/enum_type_engine_parameter_list_kind.h",
		"generated/enum_type_engine_pointer_part_kind.h",
		"generated/enum_type_engine_function_part_kind.h",
		"generated/enum_type_engine_array_part_kind.h",
		"generated/enum_type_engine_bitfield_or_declarator_kind.h",
		"generated/enum_type_engine_unscoped_tag_specifier_kind.h",
		"generated/enum_type_engine_union_specifier_kind.h",
		"generated/enum_type_engine_struct_specifier_kind.h",
		"generated/struct_c_lexer_token_memory_pool.h",
		"generated/struct_parser_node_memory_pool.h",
		"generated/enum_type_class.h",
		"generated/struct_type_description.h",
		"generated/struct_memory_pool_collection.h",
		"generated/unsigned_char_list.h",
		"generated/struct_tag_declaration_namespace_ptr_list.h",
		"generated/struct_identifier_declaration_namespace_ptr_list.h",
		"generated/struct_c_lexer_token_list.h",
		"generated/struct_general_type_id_list.h",
		"generated/struct_unscoped_tag_specifier_id_list.h",
		"generated/struct_normalized_specifier_ptr_list.h",
		"generated/struct_normalized_declarator_ptr_list.h",
		"generated/struct_normalized_declaration_element_ptr_list.h",
		"generated/enum_scope_type.h",
		"generated/struct_generic_list_item_list.h",
		"generated/struct_scope_level_ptr_list.h",
		"generated/struct_parser_node_ptr_list.h",
		"generated/unsigned_int_list.h",
		"generated/unsigned_char_ptr_to_struct_namespace_object_ptr_map.h",
		"generated/struct_c_lexer_token_to_struct_identifier_id_map.h",
		"generated/unsigned_int_to_void_ptr_map.h",
		"generated/unsigned_int_to_unsigned_int_map.h",
		"generated/struct_parser_node_ptr_to_struct_parser_node_id_map.h",
		"generated/struct_parser_node_ptr_to_struct_scope_level_id_map.h",
		"generated/struct_generic_list_item_to_unsigned_int_map.h",
		"generated/struct_scope_level_ptr_to_struct_scope_level_id_map.h",
		"generated/struct_identifier_id_to_struct_general_type_id_map.h",
		"generated/struct_identifier_id_to_struct_unscoped_tag_specifier_id_map.h",
		"generated/struct_unscoped_tag_specifier_id_to_unsigned_int_map.h",
		"generated/struct_identifier_id_to_unsigned_int_map.h",
		"generated/enum_c_token_type_list.h",
		"generated/struct_type_engine_state.h",
		"generated/enum_type_engine_operation_type.h",
		"generated/struct_identifier_from_declarator.h",
		"generated/struct_enum_definition.h",
		"generated/struct_struct_or_union_definition.h",
		"generated/enum_add_or_remove.h",
		"generated/enum_type_engine_operation_type_list.h",
		"generated/struct_unscoped_tag_specifier_id_to_struct_struct_or_union_definition_ptr_map.h",
		"generated/struct_unscoped_tag_specifier_id_to_struct_enum_definition_ptr_map.h",
		"generated/struct_struct_unscoped_tag_specifier_id_to_struct_struct_or_union_definition_ptr_key_value_pair_compare.h",
		"generated/struct_struct_unscoped_tag_specifier_id_to_struct_enum_definition_ptr_key_value_pair_compare.h",
		"generated/struct_struct_identifier_id_to_struct_general_type_id_key_value_pair_compare.h",
		"generated/struct_struct_identifier_id_to_struct_unscoped_tag_specifier_id_key_value_pair_compare.h",
		"generated/struct_struct_identifier_id_to_unsigned_int_key_value_pair_compare.h",
		"generated/struct_struct_unscoped_tag_specifier_id_to_unsigned_int_key_value_pair_compare.h",
		"generated/struct_struct_parser_node_ptr_to_struct_parser_node_id_key_value_pair_compare.h",
		"generated/struct_struct_parser_node_ptr_to_struct_scope_level_id_key_value_pair_compare.h",
		"generated/struct_unsigned_int_to_unsigned_int_key_value_pair_compare.h",
		"generated/struct_struct_generic_list_item_to_unsigned_int_key_value_pair_compare.h",
		"generated/struct_struct_scope_level_ptr_to_struct_scope_level_id_key_value_pair_compare.h",
		"generated/struct_struct_c_lexer_token_to_struct_identifier_id_key_value_pair_compare.h",
		"generated/struct_unsigned_int_to_void_ptr_key_value_pair_compare.h",
		"generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_compare.h",
		"generated/struct_enum_c_token_type_to_struct_simple_storage_class_specifier_id_key_value_pair_compare.h",
		"generated/enum_c_token_type_to_struct_simple_storage_class_specifier_id_map.h",
		"generated/struct_enum_c_token_type_to_struct_simple_type_specifier_id_key_value_pair_compare.h",
		"generated/enum_c_token_type_to_struct_simple_type_specifier_id_map.h",
		"generated/struct_enum_c_token_type_to_struct_type_qualifier_id_key_value_pair_compare.h",
		"generated/enum_c_token_type_to_struct_type_qualifier_id_map.h",
		"generated/struct_struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_key_value_pair_compare.h",
		"generated/struct_aggregate_specifier_details_list.h",
		"generated/struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map.h",
		"generated/struct_struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_key_value_pair_compare.h",
		"generated/struct_specifier_or_qualifier_details_list.h",
		"generated/struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map.h",
		"generated/struct_struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_key_value_pair_compare.h",
		"generated/struct_specifier_or_qualifier_list_item_details_list.h",
		"generated/struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map.h",
		"generated/struct_struct_pointer_part_details_to_struct_pointer_part_id_key_value_pair_compare.h",
		"generated/struct_pointer_part_details_list.h",
		"generated/struct_pointer_part_details_to_struct_pointer_part_id_map.h",
		"generated/struct_struct_function_part_details_to_struct_function_part_id_key_value_pair_compare.h",
		"generated/struct_function_part_details_list.h",
		"generated/struct_function_part_details_to_struct_function_part_id_map.h",
		"generated/struct_struct_array_part_details_to_struct_array_part_id_key_value_pair_compare.h",
		"generated/struct_array_part_details_list.h",
		"generated/struct_array_part_details_to_struct_array_part_id_map.h",
		"generated/struct_struct_declarator_part_details_to_struct_declarator_part_id_key_value_pair_compare.h",
		"generated/struct_declarator_part_details_list.h",
		"generated/struct_declarator_part_details_to_struct_declarator_part_id_map.h",
		"generated/struct_struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_key_value_pair_compare.h",
		"generated/struct_declarator_part_list_item_details_list.h",
		"generated/struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map.h",
		"generated/struct_struct_parameter_list_details_to_struct_parameter_list_id_key_value_pair_compare.h",
		"generated/struct_parameter_list_details_list.h",
		"generated/struct_parameter_list_details_to_struct_parameter_list_id_map.h",
		"generated/struct_struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_key_value_pair_compare.h",
		"generated/struct_bitfield_or_declarator_details_list.h",
		"generated/struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_map.h",
		"generated/struct_struct_general_type_details_to_struct_general_type_id_key_value_pair_compare.h",
		"generated/struct_general_type_details_list.h",
		"generated/struct_general_type_details_to_struct_general_type_id_map.h",
		"generated/struct_struct_general_type_list_item_details_to_struct_general_type_list_item_id_key_value_pair_compare.h",
		"generated/struct_general_type_list_item_details_list.h",
		"generated/struct_general_type_list_item_details_to_struct_general_type_list_item_id_map.h",
		"generated/struct_struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_key_value_pair_compare.h",
		"generated/struct_unscoped_tag_specifier_details_list.h",
		"generated/struct_unscoped_tag_specifier_details_to_struct_unscoped_tag_specifier_id_map.h",
		"generated/struct_struct_enum_specifier_details_to_struct_enum_specifier_id_key_value_pair_compare.h",
		"generated/struct_enum_specifier_details_list.h",
		"generated/struct_enum_specifier_details_to_struct_enum_specifier_id_map.h",
		"generated/struct_struct_union_specifier_details_to_struct_union_specifier_id_key_value_pair_compare.h",
		"generated/struct_union_specifier_details_list.h",
		"generated/struct_union_specifier_details_to_struct_union_specifier_id_map.h",
		"generated/struct_struct_struct_specifier_details_to_struct_struct_specifier_id_key_value_pair_compare.h",
		"generated/struct_struct_specifier_details_list.h",
		"generated/struct_struct_specifier_details_to_struct_struct_specifier_id_map.h",
		"generated/struct_struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_key_value_pair_compare.h",
		"generated/struct_scoped_tag_specifier_details_list.h",
		"generated/struct_scoped_tag_specifier_details_to_struct_scoped_tag_specifier_id_map.h",
		"stop"
	);
}

void va_register_generated_type(struct build_state * state, const char * source_type, const char * generated_include, va_list l){
	const char * next_dependency = (const char *)0;
	register_entity(state, source_type, ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, source_type, "terminal", "true");
	register_entity(state, generated_include, ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, generated_include, "is_header", "true");
	register_composition(state, generated_include, source_type);
	while(strcmp(next_dependency = va_arg(l, const char *), "stop")){
		register_inclusion(state, generated_include, next_dependency);
	}
}

void register_generated_type(struct build_state * state, const char * source_type, const char * generated_include,  ...){
	va_list l;
	va_start(l, generated_include);
	va_register_generated_type(state, source_type, generated_include, l);
	va_end(l);
}

void va_register_generated_aggregate_include(struct build_state * state, const char * generated_include, va_list l){
	const char * next_dependency = (const char *)0;
	register_entity(state, generated_include, ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, generated_include, "is_header", "true");
	while(strcmp(next_dependency = va_arg(l, const char *), "stop")){
		register_inclusion(state, generated_include, next_dependency);
	}
}

void register_generated_aggregate_include(struct build_state * state, const char * generated_include, ...){
	va_list l;
	va_start(l, generated_include);
	va_register_generated_aggregate_include(state, generated_include, l);
	va_end(l);
}

void make_compare(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer header_dependencies, unsigned char * algorithm_include, const char * header_template, const char * algorithm_template){
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_COMPARE_ALGORITHM,
				algorithm_template, 
				binary_exponential_buffer_copy(&types),
				make_beb_list(1, algorithm_include)
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_COMPARE_HEADER,
				header_template, 
				types,
				header_dependencies
			)
		)
	);
}

void make_kvp(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer header_dependencies){
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_KEY_VALUE_PAIR_TYPE,
				"recc-implementation/templates/generic_type_key_value_pair.h", 
				types,
				header_dependencies
			)
		)
	);
}

void make_map(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer header_dependencies, const char * compare_template){
	struct binary_exponential_buffer list1_literal;
	struct binary_exponential_buffer list2_literal;
	struct binary_exponential_buffer file_replacement_rules;
	struct replacement_context * r = replacement_context_create();
	struct replacement_context * kvp_header = replacement_context_create();
	struct replacement_context * comparison_header = replacement_context_create();
	unsigned int i;
	binary_exponential_buffer_create(&file_replacement_rules, sizeof(struct replacement_rule *));
	binary_exponential_buffer_create(&list1_literal, sizeof(unsigned char *));
	binary_exponential_buffer_create(&list2_literal, sizeof(unsigned char *));
	binary_exponential_buffer_increment(&list1_literal, 1);
	binary_exponential_buffer_increment(&list2_literal, 1);
	((unsigned char **)binary_exponential_buffer_data(&list1_literal))[0] = ((unsigned char **)binary_exponential_buffer_data(&types))[0];
	((unsigned char **)binary_exponential_buffer_data(&list2_literal))[0] = ((unsigned char **)binary_exponential_buffer_data(&types))[1];
	/*  Every map needs list types */
	/*  TODO:  The list shouldn't really be sent the dependencies for both key and value, but this works for now. */
	make_list(state, b, list1_literal, binary_exponential_buffer_copy(&header_dependencies));
	make_list(state, b, list2_literal, binary_exponential_buffer_copy(&header_dependencies));
	/*  Every map also requires a KVP type */
	make_kvp(state, b, binary_exponential_buffer_copy(&types), binary_exponential_buffer_copy(&header_dependencies));

	make_type_replacement_rules(&file_replacement_rules, &types);
	for(i = 0; i < binary_exponential_buffer_size(&file_replacement_rules); i++){
		struct replacement_rule ** d = (struct replacement_rule **)binary_exponential_buffer_data(&file_replacement_rules);
		replacement_context_associate_replacement(r, d[i]);
		replacement_context_associate_replacement(kvp_header, d[i]);
		replacement_context_associate_replacement(comparison_header, d[i]);
	}
	add_string_to_binary_exponential_buffer_with_null_terminator((unsigned char *)"struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair", &r->in_characters);
	add_string_to_binary_exponential_buffer_with_null_terminator((unsigned char *)"generated/struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair.h", &kvp_header->in_characters);
	add_string_to_binary_exponential_buffer_with_null_terminator((unsigned char *)"generated/struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_compare.h", &comparison_header->in_characters);
	do_string_replacements(r);
	do_string_replacements(kvp_header);
	do_string_replacements(comparison_header);

	make_binary_search(state, b, make_beb_list(1, (unsigned char *)binary_exponential_buffer_data(&r->out_characters)), make_beb_list(1, (unsigned char *)binary_exponential_buffer_data(&kvp_header->out_characters)));

	make_compare(state, b, make_beb_list(1, (unsigned char *)binary_exponential_buffer_data(&r->out_characters)), make_beb_list(1, (unsigned char *)binary_exponential_buffer_data(&kvp_header->out_characters)), (unsigned char *)binary_exponential_buffer_data(&comparison_header->out_characters), "recc-implementation/templates/comparison-functions/simple_compare.h", compare_template);

	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_MAP_ALGORITHM,
				"recc-implementation/templates/generic_algorithm_map.c", 
				binary_exponential_buffer_copy(&types),
				make_beb_list(0)
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_MAP_HEADER,
				"recc-implementation/templates/generic_header_map.h", 
				binary_exponential_buffer_copy(&types),
				binary_exponential_buffer_copy(&header_dependencies)
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_MAP_TYPE,
				"recc-implementation/templates/generic_type_map.h", 
				types,
				header_dependencies
			)
		)
	);

	for(i = 0; i < binary_exponential_buffer_size(&file_replacement_rules); i++){
		struct replacement_rule ** d = (struct replacement_rule **)binary_exponential_buffer_data(&file_replacement_rules);
		replacement_rule_destroy(d[i]);
	}
	binary_exponential_buffer_destroy(&file_replacement_rules);
	replacement_context_destroy(r);
	replacement_context_destroy(kvp_header);
	replacement_context_destroy(comparison_header);
}

void make_list(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer algorithm_dependencies){
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_LIST_ALGORITHM,
				"recc-implementation/templates/generic_algorithm_list.c", 
				binary_exponential_buffer_copy(&types),
				make_beb_list(0)
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_LIST_HEADER,
				"recc-implementation/templates/generic_header_list.h", 
				binary_exponential_buffer_copy(&types),
				algorithm_dependencies
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_LIST_TYPE,
				"recc-implementation/templates/generic_type_list.h", 
				types,
				make_beb_list(0)
			)
		)
	);
}

void make_merge_sort(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer algorithm_dependencies){
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_MERGE_SORT_ALGORITHM,
				"recc-implementation/templates/generic_algorithm_merge_sort.c", 
				binary_exponential_buffer_copy(&types),
				make_beb_list(0)
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_MERGE_SORT_HEADER,
				"recc-implementation/templates/generic_header_merge_sort.h", 
				types,
				algorithm_dependencies
			)
		)
	);
}

void make_binary_search(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer algorithm_dependencies){
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_BINARY_SEARCH_ALGORITHM,
				"recc-implementation/templates/generic_algorithm_binary_search.c", 
				binary_exponential_buffer_copy(&types),
				make_beb_list(0)
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_BINARY_SEARCH_HEADER,
				"recc-implementation/templates/generic_header_binary_search.h", 
				types,
				algorithm_dependencies
			)
		)
	);
}

void make_memory_pool(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer algorithm_dependencies){
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_MEMORY_POOL_ALGORITHM,
				"recc-implementation/templates/generic_algorithm_memory_pool.c", 
				binary_exponential_buffer_copy(&types),
				make_beb_list(0)
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_MEMORY_POOL_HEADER,
				"recc-implementation/templates/generic_header_memory_pool.h", 
				binary_exponential_buffer_copy(&types),
				algorithm_dependencies
			)
		)
	);
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				GENERATED_FILE_TYPE_MEMORY_POOL_TYPE,
				"recc-implementation/templates/generic_type_memory_pool.h", 
				types,
				make_beb_list(0)
			)
		)
	);
}
