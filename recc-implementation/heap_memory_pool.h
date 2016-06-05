#ifndef HEAP_MEMORY_POOL_H_
#define HEAP_MEMORY_POOL_H_
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

#ifndef void_ptr_memory_pool_H_DEFINED_
#include "../generated/void_ptr_memory_pool.h"
#endif
#ifndef TYPES_struct_heap_memory_pool_H_
#include "../generated/struct_heap_memory_pool.h"
#endif
#ifndef void_ptr_to_unsigned_int_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/void_ptr_to_unsigned_int_key_value_pair_compare.h"
#endif
#ifndef void_ptr_to_unsigned_int_map_H_DEFINED_
#include "../generated/void_ptr_to_unsigned_int_map.h"
#endif

void heap_memory_pool_create(struct heap_memory_pool *);
void heap_memory_pool_destroy(struct heap_memory_pool *);
void * heap_memory_pool_malloc(struct heap_memory_pool *, unsigned int);
void heap_memory_pool_free(struct heap_memory_pool *, void *);

#endif
