#ifndef TYPES_CODE_GENERATOR_struct_code_gen_state_H_
#define TYPES_CODE_GENERATOR_struct_code_gen_state_H_
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


#ifndef TYPES_DATA_STRUCTURES_struct_struct_compile_time_constant_ptr_list_H_
#include "../data-structures/struct_struct_compile_time_constant_ptr_list.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_struct_linker_object_ptr_list_H_
#include "../../types/data-structures/struct_struct_linker_object_ptr_list.h"
#endif

struct code_gen_state{
	struct parser_state * parser_state;
	struct memory_pool_collection * memory_pool_collection;
	struct namespace_object * current_function;
	struct struct_switch_frame_ptr_list switch_frames; /*  Used for keeping track of the case labels under switches */
	struct unsigned_int_list scope_index_list;
	struct struct_type_description_reference_list type_stack;
	struct unsigned_char_list * buffered_output;
	struct unsigned_char_list * buffered_symbol_table;
	struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map symbols;
	struct struct_compile_time_constant_ptr_list created_compile_time_constants;
	struct struct_constant_description_ptr_list created_constant_descriptions;
	struct struct_constant_initializer_level_ptr_list created_constant_initializer_levels;
	struct void_ptr_list created_data; /*  the 'created_data' member in the case of compile time constants for sizeof type names */
	struct struct_linker_object_ptr_list object_declarations;
	unsigned int condition_index;
	unsigned int next_scope_index;
	unsigned int global_var_ptr_index;
	unsigned int pad;
};

#endif
