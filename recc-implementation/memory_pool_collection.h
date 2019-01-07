#ifndef MEMORY_POOL_COLLECTION_H_
#define MEMORY_POOL_COLLECTION_H_
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

#ifndef TYPES_struct_memory_pool_collection_H_
#include "../generated/struct_memory_pool_collection.h"
#endif
#ifndef TYPES_LEXER_enum_c_token_type_H_
#include "../generated/enum_c_token_type.h"
#endif
#ifndef TYPES_LEXER_enum_l2_token_type_H_
#include "../generated/enum_l2_token_type.h"
#endif
#ifndef REGEX_ENGINE_H_
#include "regex_engine.h"
#endif
#ifndef void_ptr_memory_pool_H_DEFINED_
#include "../generated/void_ptr_memory_pool.h"
#endif
#ifndef void_ptr_to_unsigned_int_key_value_pair_compare_H_DEFINED_
#include "../generated/struct_void_ptr_to_unsigned_int_key_value_pair_compare.h"
#endif
#ifndef generated_DIR_void_ptr_to_unsigned_int_map_DOT_h
#include "../generated/void_ptr_to_unsigned_int_map.h"
#endif


void heap_memory_pool_create(struct memory_pool_collection *);
void heap_memory_pool_destroy(struct memory_pool_collection *);
void * heap_memory_pool_malloc(struct memory_pool_collection *, unsigned int);
void heap_memory_pool_free(struct memory_pool_collection *, void *);

void memory_pool_collection_create(struct memory_pool_collection *);
void memory_pool_collection_destroy(struct memory_pool_collection *);

#endif
