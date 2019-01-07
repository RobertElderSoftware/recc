

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

/*GETS_REPLACED_WITH_INCLUDES*/

#define NUM_HEAP_POOLS 20

struct memory_pool_collection{
	struct regex_compiler_state ** l2_token_regexes;
	struct regex_matcher_state * l2_token_matcher;
	struct regex_compiler_state ** c_token_regexes;
	struct regex_matcher_state * c_token_matcher;
	struct struct_regex_computation_node_memory_pool * struct_regex_computation_node_pool;
	struct struct_regex_parser_node_memory_pool * struct_regex_parser_node_pool;
	struct struct_l2_lexer_token_memory_pool * struct_l2_lexer_token_pool;
	struct struct_c_lexer_token_memory_pool * struct_c_lexer_token_pool;
	struct struct_parser_node_memory_pool * struct_parser_node_pool;
	struct struct_l2_parser_node_memory_pool * struct_l2_parser_node_pool;
	struct struct_type_description_memory_pool * struct_type_description_pool;
	struct struct_l2_item_memory_pool * struct_l2_item_pool;
	struct struct_linker_symbol_memory_pool * struct_linker_symbol_pool;
	struct heap_memory_pool * heap_pool; /*  Used for pooling memory objects of arbitrary size */

	struct void_ptr_to_unsigned_int_map contiguous_heap_bock_entries; /*  Stores pointers that can be used to deduce which pool similar pointers came from*/
	struct void_ptr_memory_pool * heap_pools[NUM_HEAP_POOLS];
	struct void_ptr_memory_pool * void_ptr_pool; /*  Not used currently, but required due to various code generation things...*/

};
