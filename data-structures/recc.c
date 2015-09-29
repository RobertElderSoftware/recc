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

#include "../recc-implementation/compiler_interface.h"

void register_data_structures_objects(struct build_state *);

void register_data_structures_objects(struct build_state * state){
	/*  Data structures */
        register_c_to_l2(state, "data-structures/void_ptr_to_unsigned_int_map");
        register_c_to_l2(state, "data-structures/struct_void_ptr_to_unsigned_int_key_value_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_void_ptr_to_unsigned_int_key_value_pair_merge_sort");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_merge_sort");
        register_c_to_l2(state, "data-structures/void_ptr_memory_pool");
        register_c_to_l2(state, "data-structures/struct_linker_symbol_memory_pool");
        register_c_to_l2(state, "data-structures/struct_asm_instruction_memory_pool");
        register_c_to_l2(state, "data-structures/struct_type_description_memory_pool");
        register_c_to_l2(state, "data-structures/struct_parser_node_memory_pool");
        register_c_to_l2(state, "data-structures/struct_asm_lexer_token_memory_pool");
        register_c_to_l2(state, "data-structures/struct_c_lexer_token_memory_pool");
        register_c_to_l2(state, "data-structures/unsigned_int_binary_search");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_merge_sort");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_merge_sort");
        register_c_to_l2(state, "data-structures/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_merge_sort");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_merge_sort");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_merge_sort");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_merge_sort");
        register_c_to_l2(state, "data-structures/void_ptr_compare");
        register_c_to_l2(state, "data-structures/struct_c_lexer_token_ptr_compare");
        register_c_to_l2(state, "data-structures/unsigned_char_ptr_compare");
        register_c_to_l2(state, "data-structures/unsigned_int_merge_sort");
        register_c_to_l2(state, "data-structures/struct_linker_symbol_ptr_list");
        register_c_to_l2(state, "data-structures/unsigned_char_ptr_to_struct_linker_symbol_ptr_map");
        register_c_to_l2(state, "data-structures/struct_switch_frame_ptr_list");
        register_c_to_l2(state, "data-structures/struct_type_traversal_ptr_list");
        register_c_to_l2(state, "data-structures/struct_type_description_ptr_list");
        register_c_to_l2(state, "data-structures/struct_constant_initializer_level_ptr_list");
        register_c_to_l2(state, "data-structures/struct_c_lexer_state_ptr_list");
        register_c_to_l2(state, "data-structures/struct_unsigned_char_list_ptr_list");
        register_c_to_l2(state, "data-structures/struct_special_macro_definition_ptr_list");
        register_c_to_l2(state, "data-structures/unsigned_char_ptr_to_struct_special_macro_definition_ptr_map");
        register_c_to_l2(state, "data-structures/unsigned_char_ptr_to_struct_namespace_object_ptr_map");
        register_c_to_l2(state, "data-structures/struct_struct_c_lexer_token_ptr_list_ptr_list");
        register_c_to_l2(state, "data-structures/binary_exponential_buffer");
        register_c_to_l2(state, "data-structures/struct_preprocessor_file_context_ptr_list");
        register_c_to_l2(state, "data-structures/struct_macro_parameter_ptr_list");
        register_c_to_l2(state, "data-structures/unsigned_char_ptr_to_struct_macro_parameter_ptr_map");
        register_c_to_l2(state, "data-structures/struct_preprocessor_if_branch_ptr_list");
        register_c_to_l2(state, "data-structures/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map");
        register_c_to_l2(state, "data-structures/struct_macro_definition_ptr_list");
        register_c_to_l2(state, "data-structures/unsigned_char_ptr_to_struct_macro_definition_ptr_map");
        register_c_to_l2(state, "data-structures/struct_constant_description_ptr_list");
        register_c_to_l2(state, "data-structures/struct_parser_operation_stack");
        register_c_to_l2(state, "data-structures/unsigned_char_ptr_to_struct_constant_description_ptr_map");
        register_c_to_l2(state, "data-structures/struct_namespace_object_ptr_list");
        register_c_to_l2(state, "data-structures/struct_normalized_declarator_ptr_list");
        register_c_to_l2(state, "data-structures/struct_normalized_declaration_element_ptr_list");
        register_c_to_l2(state, "data-structures/struct_normalized_specifier_ptr_list");
        register_c_to_l2(state, "data-structures/unsigned_char_list");
        register_c_to_l2(state, "data-structures/unsigned_char_ptr_list");
        register_c_to_l2(state, "data-structures/void_ptr_list");
        register_c_to_l2(state, "data-structures/unsigned_int_list");
        register_c_to_l2(state, "data-structures/struct_c_lexer_token_ptr_list");
        register_c_to_l2(state, "data-structures/struct_asm_lexer_token_ptr_list");
        register_c_to_l2(state, "data-structures/struct_preprocessor_macro_level_ptr_list");

	register_build_target(state, "data-structures/libdata-structures.l2", BUILD_TARGET_L2_FILE);
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/void_ptr_to_unsigned_int_map.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_void_ptr_to_unsigned_int_key_value_pair_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_void_ptr_to_unsigned_int_key_value_pair_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/void_ptr_memory_pool.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_linker_symbol_memory_pool.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_asm_instruction_memory_pool.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_type_description_memory_pool.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_parser_node_memory_pool.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_asm_lexer_token_memory_pool.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_c_lexer_token_memory_pool.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_int_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_binary_search.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/void_ptr_compare.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_c_lexer_token_ptr_compare.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_ptr_compare.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_int_merge_sort.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_linker_symbol_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_switch_frame_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_type_traversal_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_type_description_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_constant_initializer_level_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_c_lexer_state_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_unsigned_char_list_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_special_macro_definition_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_ptr_to_struct_special_macro_definition_ptr_map.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_ptr_to_struct_namespace_object_ptr_map.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_struct_c_lexer_token_ptr_list_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/binary_exponential_buffer.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_preprocessor_file_context_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_macro_parameter_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_preprocessor_if_branch_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_macro_definition_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_ptr_to_struct_macro_definition_ptr_map.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_constant_description_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_parser_operation_stack.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_ptr_to_struct_constant_description_ptr_map.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_namespace_object_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_normalized_declarator_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_normalized_declaration_element_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_normalized_specifier_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_char_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/void_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/unsigned_int_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_c_lexer_token_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_asm_lexer_token_ptr_list.l2");
        register_dependency(state, "data-structures/libdata-structures.l2", "data-structures/struct_preprocessor_macro_level_ptr_list.l2");
}
