#ifndef TYPES_DATA_STRUCTURES_struct_parser_node_memory_pool_H_
#define TYPES_DATA_STRUCTURES_struct_parser_node_memory_pool_H_
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

#ifndef TYPES_DATA_STRUCTURES_struct_void_ptr_list_H_
#include "struct_void_ptr_list.h"
#endif
#ifndef TYPES_PARSER_struct_parser_node_H_
#include "../../types/parser/struct_parser_node.h"
#endif

struct struct_parser_node_memory_pool{
	void ** freed_pointers;  /* Pointers that we can just hand out next time we get a memory request */
	struct void_ptr_list block_pointers;  /* An array of pointers to blocks of object_size * 2^n */
	unsigned int object_size; /*  The size of the object (in bytes) for which we're providing this service. */
	unsigned int objects_allocated;  /*  The total number of outstanding non-freed objects */
	unsigned int next_preallocation;  /*  If we're asked to allocate more memory, and we find that objects_allocated is next_allocation, allocate another block */
	unsigned int objects_to_preallocate;  /*  The number of objects we'll allocate next time we allocate another block */
	unsigned int num_freed_pointers;  /*  The number of pointers in freed_pointers that we can just hand out for a malloc */
	unsigned int pooling_active;
};

#endif
