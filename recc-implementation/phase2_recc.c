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

void make_list(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_kvp(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_map(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_merge_sort(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_binary_search(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_memory_pool(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);

void make_kvp(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer header_dependencies){
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				b,
				GENERATED_FILE_TYPE_KEY_VALUE_PAIR,
				types,
				header_dependencies,
				make_beb_list(0)
			)
		)
	);
}

void make_map(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer header_dependencies){
	struct binary_exponential_buffer list1_literal;
	struct binary_exponential_buffer list2_literal;
	struct binary_exponential_buffer file_replacement_rules;
	struct replacement_context * r = replacement_context_create();
	struct replacement_context * header = replacement_context_create();
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
		replacement_context_associate_replacement(header, d[i]);
	}
	add_string_to_binary_exponential_buffer_with_null_terminator((unsigned char *)"struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair", &r->in_characters);
	add_string_to_binary_exponential_buffer_with_null_terminator((unsigned char *)"generated/struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair.h", &header->in_characters);
	do_string_replacements(r);
	do_string_replacements(header);

	make_binary_search(state, b, make_beb_list(1, (unsigned char *)binary_exponential_buffer_data(&r->out_characters)), make_beb_list(1, (unsigned char *)binary_exponential_buffer_data(&header->out_characters)));

	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				b,
				GENERATED_FILE_TYPE_MAP,
				types,
				header_dependencies,
				make_beb_list(0)
			)
		)
	);
	for(i = 0; i < binary_exponential_buffer_size(&file_replacement_rules); i++){
		struct replacement_rule ** d = (struct replacement_rule **)binary_exponential_buffer_data(&file_replacement_rules);
		replacement_rule_destroy(d[i]);
	}
	binary_exponential_buffer_destroy(&file_replacement_rules);
	replacement_context_destroy(r);
	replacement_context_destroy(header);
}

void make_list(struct build_state * state, struct bootstrap_state * b, struct binary_exponential_buffer types, struct binary_exponential_buffer algorithm_dependencies){
	cleanup_files_for_type(
		make_build_rules_for_typed_files(
			b,
			state,
			make_files_for_type(
				b,
				GENERATED_FILE_TYPE_LIST,
				types,
				make_beb_list(0),
				algorithm_dependencies
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
				b,
				GENERATED_FILE_TYPE_MERGE_SORT,
				types,
				make_beb_list(0),
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
				b,
				GENERATED_FILE_TYPE_BINARY_SEARCH,
				types,
				make_beb_list(0),
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
				b,
				GENERATED_FILE_TYPE_MEMORY_POOL,
				types,
				make_beb_list(0),
				algorithm_dependencies
			)
		)
	);
}

void new_register_data_structures_objects(struct build_state *);
void new_register_data_structures_objects(struct build_state * state){
	struct bootstrap_state bootstrap_state;
	struct bootstrap_state * b = &bootstrap_state;
	bootstrap_state_create(&bootstrap_state);

	make_map(state, b, make_beb_list(2, "struct generic_list_item", "unsigned int"), make_beb_list(1, "generated/struct_generic_list_item.h"));
	make_map(state, b, make_beb_list(2, "struct parser_node *", "unsigned int"), make_beb_list(1, "generated/struct_parser_node.h"));
	make_map(state, b, make_beb_list(2, "struct scope_level *", "unsigned int"), make_beb_list(1, "generated/struct_scope_level.h"));
	make_map(state, b, make_beb_list(2, "unsigned int", "void *"), make_beb_list(0));
	make_map(state, b, make_beb_list(2, "unsigned int", "unsigned int"), make_beb_list(0));
	make_map(state, b, make_beb_list(2, "void *", "unsigned int"), make_beb_list(0));
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct linker_symbol *"), make_beb_list(1, "generated/struct_linker_symbol.h"));
	make_map(state, b, make_beb_list(2, "unsigned char *", "unsigned char *"), make_beb_list(0));
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct constant_description *"), make_beb_list(1, "generated/struct_constant_description.h"));
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct macro_parameter *"), make_beb_list(1, "generated/struct_macro_parameter.h"));
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct macro_definition *"), make_beb_list(1, "generated/struct_macro_definition.h"));
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct special_macro_definition *"), make_beb_list(1, "generated/struct_special_macro_definition.h"));
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct namespace_object *"), make_beb_list(1, "generated/struct_namespace_object.h"));
	make_map(state, b, make_beb_list(2, "unsigned char *", "struct entity *"), make_beb_list(1, "recc-implementation/struct_entity.h"));
	make_map(state, b, make_beb_list(2, "struct entity_relationship", "struct struct_entity_ptr_list *"), make_beb_list(2, "recc-implementation/struct_entity_relationship.h", "generated/struct_struct_entity_ptr_list.h"));
	make_map(state, b, make_beb_list(2, "struct c_lexer_token *", "struct c_lexer_token *"), make_beb_list(1, "generated/struct_c_lexer_token.h"));
	make_map(state, b, make_beb_list(2, "struct c_lexer_token *", "unsigned char *"), make_beb_list(1, "generated/struct_c_lexer_token.h"));
	make_map(state, b, make_beb_list(2, "struct c_lexer_token", "unsigned int"), make_beb_list(1, "generated/struct_c_lexer_token.h"));
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
	make_list(state, b, make_beb_list(1, "struct type_description_reference"), make_beb_list(1, "generated/struct_constant_description.h"));
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

	bootstrap_state_output_makefiles(b, "");
	bootstrap_state_destroy(b);

	/*  Generated aggregate includes */
	register_entity(state, "generated/parser_generated_includes.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/parser_generated_includes.h", "is_header", "true");
	register_entity(state, "generated/code_generator_generated_includes.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/code_generator_generated_includes.h", "is_header", "true");

	register_entity(state, "generated/struct_regex_match.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_regex_match.h", "is_header", "true");
	register_entity(state, "generated/struct_regex_computation_node.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_regex_computation_node.h", "is_header", "true");
	register_entity(state, "generated/struct_regex_parser_node.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_regex_parser_node.h", "is_header", "true");
	register_entity(state, "generated/struct_regex_matcher_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_regex_matcher_state.h", "is_header", "true");
	register_entity(state, "generated/enum_regex_computation_node_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_regex_computation_node_type.h", "is_header", "true");
	register_entity(state, "generated/struct_character_class_range.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_character_class_range.h", "is_header", "true");
	register_entity(state, "generated/struct_regex_compiler_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_regex_compiler_state.h", "is_header", "true");
	register_entity(state, "generated/enum_regex_parser_node_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_regex_parser_node_type.h", "is_header", "true");
	register_entity(state, "generated/struct_regex_compiler_error.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_regex_compiler_error.h", "is_header", "true");
	register_entity(state, "generated/enum_regex_compiler_error_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_regex_compiler_error_type.h", "is_header", "true");
	register_entity(state, "generated/enum_l2_parser_operation_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_l2_parser_operation_type.h", "is_header", "true");
	register_entity(state, "generated/enum_object_location.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_object_location.h", "is_header", "true");
	register_entity(state, "generated/enum_normalized_declaration_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_normalized_declaration_type.h", "is_header", "true");
	register_entity(state, "generated/struct_parser_checkpoint.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_parser_checkpoint.h", "is_header", "true");
	register_entity(state, "generated/struct_normalized_declarator.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_normalized_declarator.h", "is_header", "true");
	register_entity(state, "generated/enum_declaration_or_definition.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_declaration_or_definition.h", "is_header", "true");
	register_entity(state, "generated/enum_scope_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_scope_type.h", "is_header", "true");
	register_entity(state, "generated/struct_namespace_object_change.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_namespace_object_change.h", "is_header", "true");
	register_entity(state, "generated/enum_normalized_declarator_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_normalized_declarator_type.h", "is_header", "true");
	register_entity(state, "generated/struct_current_function_change.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_current_function_change.h", "is_header", "true");
	register_entity(state, "generated/struct_first_and_last_namespace_object.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_first_and_last_namespace_object.h", "is_header", "true");
	register_entity(state, "generated/enum_add_or_remove.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_add_or_remove.h", "is_header", "true");
	register_entity(state, "generated/enum_normalized_specifier_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_normalized_specifier_type.h", "is_header", "true");
	register_entity(state, "generated/struct_namespace_object.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_namespace_object.h", "is_header", "true");
	register_entity(state, "generated/struct_parser_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_parser_state.h", "is_header", "true");
	register_entity(state, "generated/enum_parser_error_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_parser_error_type.h", "is_header", "true");
	register_entity(state, "generated/struct_normalized_specifier.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_normalized_specifier.h", "is_header", "true");
	register_entity(state, "generated/struct_l2_parser_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_l2_parser_state.h", "is_header", "true");
	register_entity(state, "generated/enum_l2_node_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_l2_node_type.h", "is_header", "true");
	register_entity(state, "generated/struct_parser_error.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_parser_error.h", "is_header", "true");
	register_entity(state, "generated/struct_parser_node.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_parser_node.h", "is_header", "true");
	register_entity(state, "generated/struct_normalized_declaration_element.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_normalized_declaration_element.h", "is_header", "true");
	register_entity(state, "generated/struct_type_description.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_type_description.h", "is_header", "true");
	register_entity(state, "generated/struct_type_description_reference.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_type_description_reference.h", "is_header", "true");
	register_entity(state, "generated/struct_parser_operation.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_parser_operation.h", "is_header", "true");
	register_entity(state, "generated/struct_namespace_modification.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_namespace_modification.h", "is_header", "true");
	register_entity(state, "generated/enum_parser_operation_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_parser_operation_type.h", "is_header", "true");
	register_entity(state, "generated/struct_constant_description.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_constant_description.h", "is_header", "true");
	register_entity(state, "generated/enum_value_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_value_type.h", "is_header", "true");
	register_entity(state, "generated/enum_node_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_node_type.h", "is_header", "true");
	register_entity(state, "generated/struct_normalized_declaration_set.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_normalized_declaration_set.h", "is_header", "true");
	register_entity(state, "generated/struct_l2_parser_operation.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_l2_parser_operation.h", "is_header", "true");
	register_entity(state, "generated/enum_type_class.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_type_class.h", "is_header", "true");
	register_entity(state, "generated/struct_l2_parser_node.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_l2_parser_node.h", "is_header", "true");
	register_entity(state, "generated/enum_type_engine_operation_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_type_engine_operation_type.h", "is_header", "true");
	register_entity(state, "generated/enum_scope_level_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_scope_level_type.h", "is_header", "true");
	register_entity(state, "generated/struct_identifier_from_declarator.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_identifier_from_declarator.h", "is_header", "true");
	register_entity(state, "generated/struct_struct_or_union_definition_collection.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_struct_or_union_definition_collection.h", "is_header", "true");
	register_entity(state, "generated/struct_declaration_namespace.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_declaration_namespace.h", "is_header", "true");
	register_entity(state, "generated/struct_function_definition_collection.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_function_definition_collection.h", "is_header", "true");
	register_entity(state, "generated/struct_enum_definition_collection.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_enum_definition_collection.h", "is_header", "true");
	register_entity(state, "generated/struct_scope_level.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_scope_level.h", "is_header", "true");
	register_entity(state, "generated/struct_enum_definition.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_enum_definition.h", "is_header", "true");
	register_entity(state, "generated/struct_function_definition.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_function_definition.h", "is_header", "true");
	register_entity(state, "generated/struct_struct_or_union_definition.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_struct_or_union_definition.h", "is_header", "true");
	register_entity(state, "generated/struct_type_engine_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_type_engine_state.h", "is_header", "true");
	register_entity(state, "generated/struct_compile_time_constant.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_compile_time_constant.h", "is_header", "true");
	register_entity(state, "generated/enum_copy_method.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_copy_method.h", "is_header", "true");
	register_entity(state, "generated/struct_switch_frame.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_switch_frame.h", "is_header", "true");
	register_entity(state, "generated/struct_code_gen_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_code_gen_state.h", "is_header", "true");
	register_entity(state, "generated/struct_constant_initializer_level.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_constant_initializer_level.h", "is_header", "true");
	register_entity(state, "generated/struct_type_traversal.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_type_traversal.h", "is_header", "true");
	register_entity(state, "generated/enum_l0_language_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_l0_language_type.h", "is_header", "true");
	register_entity(state, "generated/struct_l0_generator_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_l0_generator_state.h", "is_header", "true");
	register_entity(state, "generated/struct_printing_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_printing_state.h", "is_header", "true");
	register_entity(state, "generated/struct_preprocessor_macro_level.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_preprocessor_macro_level.h", "is_header", "true");
	register_entity(state, "generated/struct_preprocessor_if_branch.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_preprocessor_if_branch.h", "is_header", "true");
	register_entity(state, "generated/struct_macro_definition.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_macro_definition.h", "is_header", "true");
	register_entity(state, "generated/enum_special_macro_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_special_macro_type.h", "is_header", "true");
	register_entity(state, "generated/enum_macro_definition_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_macro_definition_type.h", "is_header", "true");
	register_entity(state, "generated/struct_macro_parameter.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_macro_parameter.h", "is_header", "true");
	register_entity(state, "generated/struct_preprocessor_file_context.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_preprocessor_file_context.h", "is_header", "true");
	register_entity(state, "generated/struct_special_macro_definition.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_special_macro_definition.h", "is_header", "true");
	register_entity(state, "generated/struct_preprocessor_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_preprocessor_state.h", "is_header", "true");
	register_entity(state, "generated/struct_generic_list_item.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_generic_list_item.h", "is_header", "true");
	register_entity(state, "generated/enum_entity_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_entity_type.h", "is_header", "true");
	register_entity(state, "generated/struct_heap_memory_pool.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_heap_memory_pool.h", "is_header", "true");
	register_entity(state, "generated/struct_memory_pool_collection.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_memory_pool_collection.h", "is_header", "true");
	register_entity(state, "generated/struct_linker_symbol.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_linker_symbol.h", "is_header", "true");
	register_entity(state, "generated/enum_relocation_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_relocation_type.h", "is_header", "true");
	register_entity(state, "generated/struct_start_end.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_start_end.h", "is_header", "true");
	register_entity(state, "generated/struct_l2_item.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_l2_item.h", "is_header", "true");
	register_entity(state, "generated/struct_linker_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_linker_state.h", "is_header", "true");
	register_entity(state, "generated/struct_linker_object.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_linker_object.h", "is_header", "true");
	register_entity(state, "generated/struct_linker_file.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_linker_file.h", "is_header", "true");
	register_entity(state, "generated/struct_linker_region.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_linker_region.h", "is_header", "true");
	register_entity(state, "generated/struct_c_lexer_token.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_c_lexer_token.h", "is_header", "true");
	register_entity(state, "generated/struct_l2_lexer_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_l2_lexer_state.h", "is_header", "true");
	register_entity(state, "generated/struct_common_lexer_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_common_lexer_state.h", "is_header", "true");
	register_entity(state, "generated/struct_c_lexer_state.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_c_lexer_state.h", "is_header", "true");
	register_entity(state, "generated/enum_c_token_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_c_token_type.h", "is_header", "true");
	register_entity(state, "generated/struct_l2_lexer_token.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/struct_l2_lexer_token.h", "is_header", "true");
	register_entity(state, "generated/enum_l2_token_type.h", ENTITY_TYPE_GENERATED_C_CODE);
	add_entity_attribute(state, "generated/enum_l2_token_type.h", "is_header", "true");

	register_entity(state, "types/regex_engine/struct_regex_match.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/struct_regex_match.h", "terminal", "true");
	register_entity(state, "types/regex_engine/struct_regex_computation_node.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/struct_regex_computation_node.h", "terminal", "true");
	register_entity(state, "types/regex_engine/struct_regex_parser_node.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/struct_regex_parser_node.h", "terminal", "true");
	register_entity(state, "types/regex_engine/struct_regex_matcher_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/struct_regex_matcher_state.h", "terminal", "true");
	register_entity(state, "types/regex_engine/enum_regex_computation_node_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/enum_regex_computation_node_type.h", "terminal", "true");
	register_entity(state, "types/regex_engine/struct_character_class_range.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/struct_character_class_range.h", "terminal", "true");
	register_entity(state, "types/regex_engine/struct_regex_compiler_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/struct_regex_compiler_state.h", "terminal", "true");
	register_entity(state, "types/regex_engine/enum_regex_parser_node_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/enum_regex_parser_node_type.h", "terminal", "true");
	register_entity(state, "types/regex_engine/struct_regex_compiler_error.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/struct_regex_compiler_error.h", "terminal", "true");
	register_entity(state, "types/regex_engine/enum_regex_compiler_error_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/regex_engine/enum_regex_compiler_error_type.h", "terminal", "true");
	register_entity(state, "types/parser/enum_l2_parser_operation_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_l2_parser_operation_type.h", "terminal", "true");
	register_entity(state, "types/parser/enum_object_location.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_object_location.h", "terminal", "true");
	register_entity(state, "types/parser/enum_normalized_declaration_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_normalized_declaration_type.h", "terminal", "true");
	register_entity(state, "types/parser/struct_parser_checkpoint.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_parser_checkpoint.h", "terminal", "true");
	register_entity(state, "types/parser/struct_normalized_declarator.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_normalized_declarator.h", "terminal", "true");
	register_entity(state, "types/parser/enum_declaration_or_definition.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_declaration_or_definition.h", "terminal", "true");
	register_entity(state, "types/parser/enum_scope_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_scope_type.h", "terminal", "true");
	register_entity(state, "types/parser/struct_namespace_object_change.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_namespace_object_change.h", "terminal", "true");
	register_entity(state, "types/parser/enum_normalized_declarator_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_normalized_declarator_type.h", "terminal", "true");
	register_entity(state, "types/parser/struct_current_function_change.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_current_function_change.h", "terminal", "true");
	register_entity(state, "types/parser/struct_first_and_last_namespace_object.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_first_and_last_namespace_object.h", "terminal", "true");
	register_entity(state, "types/parser/enum_add_or_remove.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_add_or_remove.h", "terminal", "true");
	register_entity(state, "types/parser/enum_normalized_specifier_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_normalized_specifier_type.h", "terminal", "true");
	register_entity(state, "types/parser/struct_namespace_object.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_namespace_object.h", "terminal", "true");
	register_entity(state, "types/parser/struct_parser_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_parser_state.h", "terminal", "true");
	register_entity(state, "types/parser/enum_parser_error_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_parser_error_type.h", "terminal", "true");
	register_entity(state, "types/parser/struct_normalized_specifier.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_normalized_specifier.h", "terminal", "true");
	register_entity(state, "types/parser/struct_l2_parser_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_l2_parser_state.h", "terminal", "true");
	register_entity(state, "types/parser/enum_l2_node_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_l2_node_type.h", "terminal", "true");
	register_entity(state, "types/parser/struct_parser_error.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_parser_error.h", "terminal", "true");
	register_entity(state, "types/parser/struct_parser_node.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_parser_node.h", "terminal", "true");
	register_entity(state, "types/parser/struct_normalized_declaration_element.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_normalized_declaration_element.h", "terminal", "true");
	register_entity(state, "types/parser/struct_type_description.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_type_description.h", "terminal", "true");
	register_entity(state, "types/parser/struct_type_description_reference.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_type_description_reference.h", "terminal", "true");
	register_entity(state, "types/parser/struct_parser_operation.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_parser_operation.h", "terminal", "true");
	register_entity(state, "types/parser/struct_namespace_modification.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_namespace_modification.h", "terminal", "true");
	register_entity(state, "types/parser/enum_parser_operation_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_parser_operation_type.h", "terminal", "true");
	register_entity(state, "types/parser/struct_constant_description.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_constant_description.h", "terminal", "true");
	register_entity(state, "types/parser/enum_value_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_value_type.h", "terminal", "true");
	register_entity(state, "types/parser/enum_node_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_node_type.h", "terminal", "true");
	register_entity(state, "types/parser/struct_normalized_declaration_set.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_normalized_declaration_set.h", "terminal", "true");
	register_entity(state, "types/parser/struct_l2_parser_operation.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_l2_parser_operation.h", "terminal", "true");
	register_entity(state, "types/parser/enum_type_class.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/enum_type_class.h", "terminal", "true");
	register_entity(state, "types/parser/struct_l2_parser_node.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/parser/struct_l2_parser_node.h", "terminal", "true");
	register_entity(state, "types/type_engine/enum_type_engine_operation_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/enum_type_engine_operation_type.h", "terminal", "true");
	register_entity(state, "types/type_engine/enum_scope_level_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/enum_scope_level_type.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_identifier_from_declarator.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_identifier_from_declarator.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_struct_or_union_definition_collection.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_struct_or_union_definition_collection.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_declaration_namespace.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_declaration_namespace.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_function_definition_collection.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_function_definition_collection.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_enum_definition_collection.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_enum_definition_collection.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_scope_level.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_scope_level.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_enum_definition.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_enum_definition.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_function_definition.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_function_definition.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_struct_or_union_definition.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_struct_or_union_definition.h", "terminal", "true");
	register_entity(state, "types/type_engine/struct_type_engine_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/type_engine/struct_type_engine_state.h", "terminal", "true");
	register_entity(state, "types/code_generator/struct_compile_time_constant.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/code_generator/struct_compile_time_constant.h", "terminal", "true");
	register_entity(state, "types/code_generator/enum_copy_method.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/code_generator/enum_copy_method.h", "terminal", "true");
	register_entity(state, "types/code_generator/struct_switch_frame.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/code_generator/struct_switch_frame.h", "terminal", "true");
	register_entity(state, "types/code_generator/struct_code_gen_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/code_generator/struct_code_gen_state.h", "terminal", "true");
	register_entity(state, "types/code_generator/struct_constant_initializer_level.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/code_generator/struct_constant_initializer_level.h", "terminal", "true");
	register_entity(state, "types/code_generator/struct_type_traversal.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/code_generator/struct_type_traversal.h", "terminal", "true");
	register_entity(state, "types/l0_generator/enum_l0_language_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/l0_generator/enum_l0_language_type.h", "terminal", "true");
	register_entity(state, "types/l0_generator/struct_l0_generator_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/l0_generator/struct_l0_generator_state.h", "terminal", "true");
	register_entity(state, "types/builtin/struct_printing_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/builtin/struct_printing_state.h", "terminal", "true");
	register_entity(state, "types/preprocessor/struct_preprocessor_macro_level.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/struct_preprocessor_macro_level.h", "terminal", "true");
	register_entity(state, "types/preprocessor/struct_preprocessor_if_branch.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/struct_preprocessor_if_branch.h", "terminal", "true");
	register_entity(state, "types/preprocessor/struct_macro_definition.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/struct_macro_definition.h", "terminal", "true");
	register_entity(state, "types/preprocessor/enum_special_macro_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/enum_special_macro_type.h", "terminal", "true");
	register_entity(state, "types/preprocessor/enum_macro_definition_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/enum_macro_definition_type.h", "terminal", "true");
	register_entity(state, "types/preprocessor/struct_macro_parameter.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/struct_macro_parameter.h", "terminal", "true");
	register_entity(state, "types/preprocessor/struct_preprocessor_file_context.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/struct_preprocessor_file_context.h", "terminal", "true");
	register_entity(state, "types/preprocessor/struct_special_macro_definition.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/struct_special_macro_definition.h", "terminal", "true");
	register_entity(state, "types/preprocessor/struct_preprocessor_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/preprocessor/struct_preprocessor_state.h", "terminal", "true");
	register_entity(state, "types/recc-implementation/struct_generic_list_item.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/recc-implementation/struct_generic_list_item.h", "terminal", "true");
	register_entity(state, "types/recc-implementation/enum_entity_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/recc-implementation/enum_entity_type.h", "terminal", "true");
	register_entity(state, "types/recc-implementation/struct_heap_memory_pool.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/recc-implementation/struct_heap_memory_pool.h", "terminal", "true");
	register_entity(state, "types/recc-implementation/struct_memory_pool_collection.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/recc-implementation/struct_memory_pool_collection.h", "terminal", "true");
	register_entity(state, "types/linker/struct_linker_symbol.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/linker/struct_linker_symbol.h", "terminal", "true");
	register_entity(state, "types/linker/enum_relocation_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/linker/enum_relocation_type.h", "terminal", "true");
	register_entity(state, "types/linker/struct_start_end.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/linker/struct_start_end.h", "terminal", "true");
	register_entity(state, "types/linker/struct_l2_item.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/linker/struct_l2_item.h", "terminal", "true");
	register_entity(state, "types/linker/struct_linker_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/linker/struct_linker_state.h", "terminal", "true");
	register_entity(state, "types/linker/struct_linker_object.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/linker/struct_linker_object.h", "terminal", "true");
	register_entity(state, "types/linker/struct_linker_file.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/linker/struct_linker_file.h", "terminal", "true");
	register_entity(state, "types/linker/struct_linker_region.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/linker/struct_linker_region.h", "terminal", "true");
	register_entity(state, "types/lexer/struct_c_lexer_token.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/lexer/struct_c_lexer_token.h", "terminal", "true");
	register_entity(state, "types/lexer/struct_l2_lexer_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/lexer/struct_l2_lexer_state.h", "terminal", "true");
	register_entity(state, "types/lexer/struct_common_lexer_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/lexer/struct_common_lexer_state.h", "terminal", "true");
	register_entity(state, "types/lexer/struct_c_lexer_state.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/lexer/struct_c_lexer_state.h", "terminal", "true");
	register_entity(state, "types/lexer/enum_c_token_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/lexer/enum_c_token_type.h", "terminal", "true");
	register_entity(state, "types/lexer/struct_l2_lexer_token.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/lexer/struct_l2_lexer_token.h", "terminal", "true");
	register_entity(state, "types/lexer/enum_l2_token_type.h", ENTITY_TYPE_SOURCE);
	add_entity_attribute(state, "types/lexer/enum_l2_token_type.h", "terminal", "true");

	register_composition(state, "generated/struct_regex_match.h", "types/regex_engine/struct_regex_match.h");
	register_composition(state, "generated/struct_regex_computation_node.h", "types/regex_engine/struct_regex_computation_node.h");
	register_composition(state, "generated/struct_regex_parser_node.h", "types/regex_engine/struct_regex_parser_node.h");
	register_composition(state, "generated/struct_regex_matcher_state.h", "types/regex_engine/struct_regex_matcher_state.h");
	register_composition(state, "generated/enum_regex_computation_node_type.h", "types/regex_engine/enum_regex_computation_node_type.h");
	register_composition(state, "generated/struct_character_class_range.h", "types/regex_engine/struct_character_class_range.h");
	register_composition(state, "generated/struct_regex_compiler_state.h", "types/regex_engine/struct_regex_compiler_state.h");
	register_composition(state, "generated/enum_regex_parser_node_type.h", "types/regex_engine/enum_regex_parser_node_type.h");
	register_composition(state, "generated/struct_regex_compiler_error.h", "types/regex_engine/struct_regex_compiler_error.h");
	register_composition(state, "generated/enum_regex_compiler_error_type.h", "types/regex_engine/enum_regex_compiler_error_type.h");
	register_composition(state, "generated/enum_l2_parser_operation_type.h", "types/parser/enum_l2_parser_operation_type.h");
	register_composition(state, "generated/enum_object_location.h", "types/parser/enum_object_location.h");
	register_composition(state, "generated/enum_normalized_declaration_type.h", "types/parser/enum_normalized_declaration_type.h");
	register_composition(state, "generated/struct_parser_checkpoint.h", "types/parser/struct_parser_checkpoint.h");
	register_composition(state, "generated/struct_normalized_declarator.h", "types/parser/struct_normalized_declarator.h");
	register_composition(state, "generated/enum_declaration_or_definition.h", "types/parser/enum_declaration_or_definition.h");
	register_composition(state, "generated/enum_scope_type.h", "types/parser/enum_scope_type.h");
	register_composition(state, "generated/struct_namespace_object_change.h", "types/parser/struct_namespace_object_change.h");
	register_composition(state, "generated/enum_normalized_declarator_type.h", "types/parser/enum_normalized_declarator_type.h");
	register_composition(state, "generated/struct_current_function_change.h", "types/parser/struct_current_function_change.h");
	register_composition(state, "generated/struct_first_and_last_namespace_object.h", "types/parser/struct_first_and_last_namespace_object.h");
	register_composition(state, "generated/enum_add_or_remove.h", "types/parser/enum_add_or_remove.h");
	register_composition(state, "generated/enum_normalized_specifier_type.h", "types/parser/enum_normalized_specifier_type.h");
	register_composition(state, "generated/struct_namespace_object.h", "types/parser/struct_namespace_object.h");
	register_composition(state, "generated/struct_parser_state.h", "types/parser/struct_parser_state.h");
	register_composition(state, "generated/enum_parser_error_type.h", "types/parser/enum_parser_error_type.h");
	register_composition(state, "generated/struct_normalized_specifier.h", "types/parser/struct_normalized_specifier.h");
	register_composition(state, "generated/struct_l2_parser_state.h", "types/parser/struct_l2_parser_state.h");
	register_composition(state, "generated/enum_l2_node_type.h", "types/parser/enum_l2_node_type.h");
	register_composition(state, "generated/struct_parser_error.h", "types/parser/struct_parser_error.h");
	register_composition(state, "generated/struct_parser_node.h", "types/parser/struct_parser_node.h");
	register_composition(state, "generated/struct_normalized_declaration_element.h", "types/parser/struct_normalized_declaration_element.h");
	register_composition(state, "generated/struct_type_description.h", "types/parser/struct_type_description.h");
	register_composition(state, "generated/struct_type_description_reference.h", "types/parser/struct_type_description_reference.h");
	register_composition(state, "generated/struct_parser_operation.h", "types/parser/struct_parser_operation.h");
	register_composition(state, "generated/struct_namespace_modification.h", "types/parser/struct_namespace_modification.h");
	register_composition(state, "generated/enum_parser_operation_type.h", "types/parser/enum_parser_operation_type.h");
	register_composition(state, "generated/struct_constant_description.h", "types/parser/struct_constant_description.h");
	register_composition(state, "generated/enum_value_type.h", "types/parser/enum_value_type.h");
	register_composition(state, "generated/enum_node_type.h", "types/parser/enum_node_type.h");
	register_composition(state, "generated/struct_normalized_declaration_set.h", "types/parser/struct_normalized_declaration_set.h");
	register_composition(state, "generated/struct_l2_parser_operation.h", "types/parser/struct_l2_parser_operation.h");
	register_composition(state, "generated/enum_type_class.h", "types/parser/enum_type_class.h");
	register_composition(state, "generated/struct_l2_parser_node.h", "types/parser/struct_l2_parser_node.h");
	register_composition(state, "generated/enum_type_engine_operation_type.h", "types/type_engine/enum_type_engine_operation_type.h");
	register_composition(state, "generated/enum_scope_level_type.h", "types/type_engine/enum_scope_level_type.h");
	register_composition(state, "generated/struct_identifier_from_declarator.h", "types/type_engine/struct_identifier_from_declarator.h");
	register_composition(state, "generated/struct_struct_or_union_definition_collection.h", "types/type_engine/struct_struct_or_union_definition_collection.h");
	register_composition(state, "generated/struct_declaration_namespace.h", "types/type_engine/struct_declaration_namespace.h");
	register_composition(state, "generated/struct_function_definition_collection.h", "types/type_engine/struct_function_definition_collection.h");
	register_composition(state, "generated/struct_enum_definition_collection.h", "types/type_engine/struct_enum_definition_collection.h");
	register_composition(state, "generated/struct_scope_level.h", "types/type_engine/struct_scope_level.h");
	register_composition(state, "generated/struct_enum_definition.h", "types/type_engine/struct_enum_definition.h");
	register_composition(state, "generated/struct_function_definition.h", "types/type_engine/struct_function_definition.h");
	register_composition(state, "generated/struct_struct_or_union_definition.h", "types/type_engine/struct_struct_or_union_definition.h");
	register_composition(state, "generated/struct_type_engine_state.h", "types/type_engine/struct_type_engine_state.h");
	register_composition(state, "generated/struct_compile_time_constant.h", "types/code_generator/struct_compile_time_constant.h");
	register_composition(state, "generated/enum_copy_method.h", "types/code_generator/enum_copy_method.h");
	register_composition(state, "generated/struct_switch_frame.h", "types/code_generator/struct_switch_frame.h");
	register_composition(state, "generated/struct_code_gen_state.h", "types/code_generator/struct_code_gen_state.h");
	register_composition(state, "generated/struct_constant_initializer_level.h", "types/code_generator/struct_constant_initializer_level.h");
	register_composition(state, "generated/struct_type_traversal.h", "types/code_generator/struct_type_traversal.h");
	register_composition(state, "generated/enum_l0_language_type.h", "types/l0_generator/enum_l0_language_type.h");
	register_composition(state, "generated/struct_l0_generator_state.h", "types/l0_generator/struct_l0_generator_state.h");
	register_composition(state, "generated/struct_printing_state.h", "types/builtin/struct_printing_state.h");
	register_composition(state, "generated/struct_preprocessor_macro_level.h", "types/preprocessor/struct_preprocessor_macro_level.h");
	register_composition(state, "generated/struct_preprocessor_if_branch.h", "types/preprocessor/struct_preprocessor_if_branch.h");
	register_composition(state, "generated/struct_macro_definition.h", "types/preprocessor/struct_macro_definition.h");
	register_composition(state, "generated/enum_special_macro_type.h", "types/preprocessor/enum_special_macro_type.h");
	register_composition(state, "generated/enum_macro_definition_type.h", "types/preprocessor/enum_macro_definition_type.h");
	register_composition(state, "generated/struct_macro_parameter.h", "types/preprocessor/struct_macro_parameter.h");
	register_composition(state, "generated/struct_preprocessor_file_context.h", "types/preprocessor/struct_preprocessor_file_context.h");
	register_composition(state, "generated/struct_special_macro_definition.h", "types/preprocessor/struct_special_macro_definition.h");
	register_composition(state, "generated/struct_preprocessor_state.h", "types/preprocessor/struct_preprocessor_state.h");
	register_composition(state, "generated/struct_generic_list_item.h", "types/recc-implementation/struct_generic_list_item.h");
	register_composition(state, "generated/enum_entity_type.h", "types/recc-implementation/enum_entity_type.h");
	register_composition(state, "generated/struct_heap_memory_pool.h", "types/recc-implementation/struct_heap_memory_pool.h");
	register_composition(state, "generated/struct_memory_pool_collection.h", "types/recc-implementation/struct_memory_pool_collection.h");
	register_composition(state, "generated/struct_linker_symbol.h", "types/linker/struct_linker_symbol.h");
	register_composition(state, "generated/enum_relocation_type.h", "types/linker/enum_relocation_type.h");
	register_composition(state, "generated/struct_start_end.h", "types/linker/struct_start_end.h");
	register_composition(state, "generated/struct_l2_item.h", "types/linker/struct_l2_item.h");
	register_composition(state, "generated/struct_linker_state.h", "types/linker/struct_linker_state.h");
	register_composition(state, "generated/struct_linker_object.h", "types/linker/struct_linker_object.h");
	register_composition(state, "generated/struct_linker_file.h", "types/linker/struct_linker_file.h");
	register_composition(state, "generated/struct_linker_region.h", "types/linker/struct_linker_region.h");
	register_composition(state, "generated/struct_c_lexer_token.h", "types/lexer/struct_c_lexer_token.h");
	register_composition(state, "generated/struct_l2_lexer_state.h", "types/lexer/struct_l2_lexer_state.h");
	register_composition(state, "generated/struct_common_lexer_state.h", "types/lexer/struct_common_lexer_state.h");
	register_composition(state, "generated/struct_c_lexer_state.h", "types/lexer/struct_c_lexer_state.h");
	register_composition(state, "generated/enum_c_token_type.h", "types/lexer/enum_c_token_type.h");
	register_composition(state, "generated/struct_l2_lexer_token.h", "types/lexer/struct_l2_lexer_token.h");
	register_composition(state, "generated/enum_l2_token_type.h", "types/lexer/enum_l2_token_type.h");

	/*  Dependencies for types */
	register_inclusion(state, "generated/struct_type_description.h", "generated/unsigned_int_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/unsigned_char_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/unsigned_int_ptr_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_switch_frame_ptr_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_type_description_reference_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_constant_initializer_level_ptr_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_type_traversal_ptr_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_compile_time_constant_ptr_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_linker_object_ptr_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/char_ptr_list.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_parser_node_memory_pool.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_c_lexer_token_memory_pool.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_linker_symbol_memory_pool.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_code_gen_state.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_constant_initializer_level.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_type_traversal.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/struct_switch_frame.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/enum_l2_token_type.h");
	register_inclusion(state, "generated/struct_type_description.h", "generated/enum_value_type.h");

	register_inclusion(state, "generated/struct_regex_computation_node.h", "generated/enum_regex_computation_node_type.h");
	register_inclusion(state, "generated/struct_regex_computation_node.h", "generated/struct_character_class_range.h");
	register_inclusion(state, "generated/struct_regex_parser_node.h", "generated/enum_regex_parser_node_type.h");
	register_inclusion(state, "generated/struct_regex_matcher_state.h", "generated/struct_regex_compiler_state.h");
	register_inclusion(state, "generated/struct_regex_matcher_state.h", "generated/struct_regex_match.h");
	register_inclusion(state, "generated/struct_regex_compiler_state.h", "generated/struct_regex_compiler_error.h");
	register_inclusion(state, "generated/struct_regex_compiler_state.h", "generated/struct_regex_parser_node.h");
	register_inclusion(state, "generated/struct_regex_compiler_state.h", "generated/struct_regex_computation_node.h");
	register_inclusion(state, "generated/struct_regex_compiler_state.h", "generated/struct_memory_pool_collection.h");
	register_inclusion(state, "generated/struct_regex_compiler_error.h", "generated/enum_regex_compiler_error_type.h");
	register_inclusion(state, "generated/struct_normalized_declarator.h", "generated/struct_parser_node.h");
	register_inclusion(state, "generated/struct_normalized_declarator.h", "generated/enum_normalized_declarator_type.h");
	register_inclusion(state, "generated/struct_first_and_last_namespace_object.h", "generated/struct_namespace_object.h");
	register_inclusion(state, "generated/struct_namespace_object.h", "generated/struct_struct_normalized_declaration_element_ptr_list.h");
	register_inclusion(state, "generated/struct_namespace_object.h", "generated/struct_scope_level.h");
	register_inclusion(state, "generated/struct_namespace_object.h", "generated/enum_object_location.h");
	register_inclusion(state, "generated/struct_parser_state.h", "generated/struct_type_engine_state.h");
	register_inclusion(state, "generated/struct_parser_state.h", "generated/struct_struct_parser_operation_list.h");
	register_inclusion(state, "generated/struct_parser_state.h", "generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_map.h");
	register_inclusion(state, "generated/struct_parser_state.h", "generated/struct_struct_parser_error_list.h");
	register_inclusion(state, "generated/struct_normalized_specifier.h", "generated/enum_normalized_specifier_type.h");
	register_inclusion(state, "generated/struct_normalized_specifier.h", "generated/struct_parser_node.h");
	register_inclusion(state, "generated/struct_l2_parser_state.h", "generated/struct_memory_pool_collection.h");
	register_inclusion(state, "generated/struct_l2_parser_state.h", "generated/struct_l2_lexer_state.h");
	register_inclusion(state, "generated/struct_l2_parser_state.h", "generated/struct_l2_parser_node.h");
	register_inclusion(state, "generated/struct_l2_parser_state.h", "generated/struct_unsigned_char_list.h");
	register_inclusion(state, "generated/struct_l2_parser_state.h", "generated/struct_struct_l2_parser_operation_list.h");
	register_inclusion(state, "generated/struct_parser_error.h", "generated/enum_parser_error_type.h");
	register_inclusion(state, "generated/struct_parser_node.h", "generated/enum_node_type.h");
	register_inclusion(state, "generated/struct_parser_node.h", "generated/struct_c_lexer_token.h");
	register_inclusion(state, "generated/struct_normalized_declaration_element.h", "generated/struct_normalized_declaration_set.h");
	register_inclusion(state, "generated/struct_normalized_declaration_element.h", "generated/struct_normalized_declarator.h");
	register_inclusion(state, "generated/struct_normalized_declaration_element.h", "generated/struct_struct_normalized_specifier_ptr_list.h");
	register_inclusion(state, "generated/struct_parser_operation.h", "generated/enum_parser_operation_type.h");
	register_inclusion(state, "generated/struct_constant_description.h", "generated/struct_type_description_reference.h");
	register_inclusion(state, "generated/struct_constant_description.h", "generated/enum_c_token_type.h");
	register_inclusion(state, "generated/struct_normalized_declaration_set.h", "generated/struct_parser_node.h");
	register_inclusion(state, "generated/struct_normalized_declaration_set.h", "generated/struct_struct_normalized_specifier_ptr_list.h");
	register_inclusion(state, "generated/struct_normalized_declaration_set.h", "generated/struct_struct_normalized_declarator_ptr_list.h");
	register_inclusion(state, "generated/struct_normalized_declaration_set.h", "generated/enum_normalized_declaration_type.h");
	register_inclusion(state, "generated/struct_l2_parser_operation.h", "generated/enum_l2_parser_operation_type.h");
	register_inclusion(state, "generated/struct_l2_parser_node.h", "generated/enum_l2_node_type.h");
	register_inclusion(state, "generated/struct_l2_parser_node.h", "generated/struct_l2_lexer_token.h");
	register_inclusion(state, "generated/struct_struct_or_union_definition_collection.h", "generated/struct_void_ptr_list.h");
	register_inclusion(state, "generated/struct_struct_or_union_definition_collection.h", "generated/struct_unsigned_int_to_void_ptr_map.h");
	register_inclusion(state, "generated/struct_declaration_namespace.h", "generated/struct_unsigned_int_list.h");
	register_inclusion(state, "generated/struct_declaration_namespace.h", "generated/struct_unsigned_int_to_unsigned_int_map.h");
	register_inclusion(state, "generated/struct_function_definition_collection.h", "generated/struct_void_ptr_list.h");
	register_inclusion(state, "generated/struct_function_definition_collection.h", "generated/struct_unsigned_int_to_void_ptr_map.h");
	register_inclusion(state, "generated/struct_enum_definition_collection.h", "generated/struct_void_ptr_list.h");
	register_inclusion(state, "generated/struct_enum_definition_collection.h", "generated/struct_unsigned_int_to_void_ptr_map.h");
	register_inclusion(state, "generated/struct_scope_level.h", "generated/struct_unsigned_char_ptr_list.h");
	register_inclusion(state, "generated/struct_scope_level.h", "generated/struct_struct_scope_level_ptr_list.h");
	register_inclusion(state, "generated/struct_scope_level.h", "generated/struct_namespace_object.h");
	register_inclusion(state, "generated/struct_scope_level.h", "generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_map.h");
	register_inclusion(state, "generated/struct_scope_level.h", "generated/struct_declaration_namespace.h");
	register_inclusion(state, "generated/struct_scope_level.h", "generated/struct_struct_or_union_definition_collection.h");
	register_inclusion(state, "generated/struct_scope_level.h", "generated/struct_enum_definition_collection.h");
	register_inclusion(state, "generated/struct_scope_level.h", "generated/enum_scope_level_type.h");
	register_inclusion(state, "generated/struct_struct_or_union_definition.h", "generated/enum_c_token_type.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_struct_parser_node_ptr_list.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_struct_scope_level_ptr_list.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_struct_c_lexer_token_list.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_struct_generic_list_item_list.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_void_ptr_list.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_unsigned_int_list.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_unsigned_int_to_unsigned_int_map.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_struct_c_lexer_token_to_unsigned_int_map.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_struct_scope_level_ptr_to_unsigned_int_map.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_struct_parser_node_ptr_to_unsigned_int_map.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_struct_generic_list_item_to_unsigned_int_map.h");
	register_inclusion(state, "generated/struct_type_engine_state.h", "generated/struct_function_definition_collection.h");
	register_inclusion(state, "generated/struct_compile_time_constant.h", "generated/struct_constant_description.h");
	register_inclusion(state, "generated/struct_compile_time_constant.h", "generated/struct_normalized_declaration_element.h");
	register_inclusion(state, "generated/struct_switch_frame.h", "generated/struct_unsigned_int_list.h");
	register_inclusion(state, "generated/struct_code_gen_state.h", "generated/struct_struct_compile_time_constant_ptr_list.h");
	register_inclusion(state, "generated/struct_code_gen_state.h", "generated/struct_struct_constant_description_ptr_list.h");
	register_inclusion(state, "generated/struct_code_gen_state.h", "generated/struct_struct_linker_object_ptr_list.h");
	register_inclusion(state, "generated/struct_constant_initializer_level.h", "generated/struct_compile_time_constant.h");
	register_inclusion(state, "generated/struct_constant_initializer_level.h", "generated/struct_struct_constant_initializer_level_ptr_list.h");
	register_inclusion(state, "generated/struct_type_traversal.h", "generated/enum_type_class.h");
	register_inclusion(state, "generated/struct_type_traversal.h", "generated/struct_type_description_reference.h");
	register_inclusion(state, "generated/struct_type_traversal.h", "generated/struct_struct_type_traversal_ptr_list.h");
	register_inclusion(state, "generated/struct_l0_generator_state.h", "generated/struct_l2_parser_state.h");
	register_inclusion(state, "generated/struct_l0_generator_state.h", "generated/struct_l2_lexer_state.h");
	register_inclusion(state, "generated/struct_l0_generator_state.h", "generated/enum_l0_language_type.h");
	register_inclusion(state, "generated/struct_preprocessor_macro_level.h", "generated/struct_c_lexer_token.h");
	register_inclusion(state, "generated/struct_preprocessor_macro_level.h", "generated/struct_struct_c_lexer_token_ptr_list.h");
	register_inclusion(state, "generated/struct_macro_definition.h", "generated/enum_macro_definition_type.h");
	register_inclusion(state, "generated/struct_macro_definition.h", "generated/struct_struct_c_lexer_token_ptr_list.h");
	register_inclusion(state, "generated/struct_macro_definition.h", "generated/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h");
	register_inclusion(state, "generated/struct_special_macro_definition.h", "generated/enum_special_macro_type.h");
	register_inclusion(state, "generated/struct_heap_memory_pool.h", "generated/struct_void_ptr_memory_pool.h");
	register_inclusion(state, "generated/struct_heap_memory_pool.h", "generated/struct_void_ptr_to_unsigned_int_map.h");
	register_inclusion(state, "generated/struct_memory_pool_collection.h", "generated/struct_regex_matcher_state.h");
	register_inclusion(state, "generated/struct_linker_symbol.h", "generated/struct_l2_item.h");
	register_inclusion(state, "generated/struct_start_end.h", "generated/struct_linker_symbol.h");
	register_inclusion(state, "generated/struct_start_end.h", "generated/struct_linker_object.h");
	register_inclusion(state, "generated/struct_l2_item.h", "generated/struct_l2_lexer_token.h");
	register_inclusion(state, "generated/struct_linker_state.h", "generated/enum_entity_type.h");
	register_inclusion(state, "generated/struct_linker_state.h", "generated/struct_struct_linker_region_list.h");
	register_inclusion(state, "generated/struct_linker_object.h", "generated/enum_l2_token_type.h");
	register_inclusion(state, "generated/struct_linker_file.h", "generated/struct_l2_lexer_state.h");
	register_inclusion(state, "generated/struct_linker_file.h", "generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h");
	register_inclusion(state, "generated/struct_linker_file.h", "generated/struct_struct_l2_item_ptr_list.h");
	register_inclusion(state, "generated/struct_linker_file.h", "generated/struct_struct_linker_object_ptr_list.h");
	register_inclusion(state, "generated/struct_linker_file.h", "generated/enum_relocation_type.h");
	register_inclusion(state, "generated/struct_c_lexer_token.h", "generated/enum_c_token_type.h");
	register_inclusion(state, "generated/struct_l2_lexer_state.h", "generated/struct_common_lexer_state.h");
	register_inclusion(state, "generated/struct_l2_lexer_state.h", "generated/struct_struct_l2_lexer_token_ptr_list.h");
	register_inclusion(state, "generated/struct_common_lexer_state.h", "generated/struct_memory_pool_collection.h");
	register_inclusion(state, "generated/struct_common_lexer_state.h", "generated/struct_unsigned_char_list.h");
	register_inclusion(state, "generated/struct_common_lexer_state.h", "generated/struct_regex_matcher_state.h");
	register_inclusion(state, "generated/struct_c_lexer_state.h", "generated/struct_common_lexer_state.h");
	register_inclusion(state, "generated/struct_c_lexer_state.h", "generated/struct_struct_c_lexer_token_ptr_list.h");
	register_inclusion(state, "generated/struct_l2_lexer_token.h", "generated/enum_l2_token_type.h");

	/*  Dependencies for generated files */
        register_inclusion(state, "generated/parser_generated_includes.h","generated/enum_type_class.h");
        register_inclusion(state, "generated/parser_generated_includes.h","generated/enum_scope_type.h");
        register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_current_function_change.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/unsigned_int_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_type_description_reference_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_parser_error_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/char_ptr_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_parser_operation_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_normalized_specifier_ptr_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_normalized_declarator_ptr_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_normalized_declaration_element_ptr_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_namespace_object_ptr_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/unsigned_char_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/unsigned_char_ptr_list.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/unsigned_char_ptr_to_struct_constant_description_ptr_map.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_parser_node_memory_pool.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_c_lexer_token_memory_pool.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_type_description_memory_pool.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_parser_state.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_type_description.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_normalized_specifier.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_normalized_declarator.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_normalized_declaration_set.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_namespace_modification.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_namespace_object_change.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_first_and_last_namespace_object.h");
	register_inclusion(state, "generated/parser_generated_includes.h","generated/struct_parser_checkpoint.h");


	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/unsigned_int_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_type_description_reference_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/unsigned_char_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/unsigned_int_ptr_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_switch_frame_ptr_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_constant_initializer_level_ptr_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_type_traversal_ptr_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_compile_time_constant_ptr_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_linker_object_ptr_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/char_ptr_list.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_parser_node_memory_pool.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_c_lexer_token_memory_pool.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_linker_symbol_memory_pool.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_code_gen_state.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_constant_initializer_level.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_type_traversal.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/struct_switch_frame.h");
	register_inclusion(state, "generated/code_generator_generated_includes.h", "generated/enum_l2_token_type.h");

}

