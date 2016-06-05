

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

/*GETS_REPLACED_WITH_INCLUDES*/

#define NUM_HEAP_POOLS 20

struct heap_memory_pool{
	struct void_ptr_to_unsigned_int_map contiguous_heap_bock_entries; /*  Stores pointers that can be used to deduce which pool similar pointers came from*/
	struct void_ptr_memory_pool heap_pools[NUM_HEAP_POOLS];
};
