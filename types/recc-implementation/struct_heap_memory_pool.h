#ifndef TYPES_struct_heap_memory_pool_H_
#define TYPES_struct_heap_memory_pool_H_
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

#ifndef void_ptr_memory_pool_H_DEFINED_
#include "../../data-structures/void_ptr_memory_pool.h"
#endif
#ifndef void_ptr_to_unsigned_int_map_H_DEFINED_
#include "../../data-structures/void_ptr_to_unsigned_int_map.h"
#endif

#define NUM_HEAP_POOLS 20

struct heap_memory_pool{
	struct void_ptr_to_unsigned_int_map contiguous_heap_bock_entries; /*  Stores pointers that can be used to deduce which pool similar pointers came from*/
	struct void_ptr_memory_pool heap_pools[NUM_HEAP_POOLS];
};

#endif
