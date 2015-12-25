#ifndef MEMORY_POOL_COLLECTION_H_
#define MEMORY_POOL_COLLECTION_H_
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

#ifndef struct_parser_node_memory_pool_H_DEFINED_
#include "../data-structures/struct_parser_node_memory_pool.h"
#endif
#ifndef struct_l2_parser_node_memory_pool_H_DEFINED_
#include "../data-structures/struct_l2_parser_node_memory_pool.h"
#endif
#ifndef struct_c_lexer_token_memory_pool_H_DEFINED_
#include "../data-structures/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef struct_l2_lexer_token_memory_pool_H_DEFINED_
#include "../data-structures/struct_l2_lexer_token_memory_pool.h"
#endif
#ifndef struct_type_description_memory_pool_H_DEFINED_
#include "../data-structures/struct_type_description_memory_pool.h"
#endif
#ifndef struct_l2_item_memory_pool_H_DEFINED_
#include "../data-structures/struct_l2_item_memory_pool.h"
#endif
#ifndef struct_linker_symbol_memory_pool_H_DEFINED_
#include "../data-structures/struct_linker_symbol_memory_pool.h"
#endif
#ifndef HEAP_MEMORY_POOL_H_
#include "heap_memory_pool.h"
#endif
#ifndef TYPES_struct_memory_pool_collection_H_
#include "../types/recc-implementation/struct_memory_pool_collection.h"
#endif

void memory_pool_collection_create(struct memory_pool_collection *);
void memory_pool_collection_destroy(struct memory_pool_collection *);

#endif
