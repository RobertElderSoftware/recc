#include "../compiler_interface.h"

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
        register_c_to_l2(state, "data-structures/struct_heap_ptr_index_pair_merge_sort");
        register_c_to_l2(state, "data-structures/struct_heap_ptr_index_pair_binary_search");
        register_c_to_l2(state, "data-structures/struct_heap_ptr_index_pair_list");
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

}
