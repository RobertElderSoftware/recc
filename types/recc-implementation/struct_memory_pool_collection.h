#ifndef TYPES_struct_memory_pool_collection_H__
#define TYPES_struct_memory_pool_collection_H__
/*
    Copyright 2015 Robert Elder Software Inc.
    
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

struct memory_pool_collection{
	struct struct_l2_lexer_token_memory_pool * struct_l2_lexer_token_pool;
	struct struct_c_lexer_token_memory_pool * struct_c_lexer_token_pool;
	struct struct_parser_node_memory_pool * struct_parser_node_pool;
	struct struct_l2_parser_node_memory_pool * struct_l2_parser_node_pool;
	struct struct_type_description_memory_pool * struct_type_description_pool;
	struct struct_l2_item_memory_pool * struct_l2_item_pool;
	struct struct_linker_symbol_memory_pool * struct_linker_symbol_pool;
	struct heap_memory_pool * heap_pool; /*  Used for pooling memory objects of arbitrary size */
};

#endif
