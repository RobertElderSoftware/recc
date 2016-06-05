#ifndef TYPES_struct_build_state_phase_3_H_
#define TYPES_struct_build_state_phase_3_H_
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
#ifndef MEMORY_POOL_COLLECTION_H_
#include "memory_pool_collection.h"
#endif
#ifndef struct_entity_relationship_list_H_DEFINED_
#include "../generated/struct_entity_relationship_list.h"
#endif
#ifndef struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_H_DEFINED_
#include "../generated/struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map.h"
#endif

struct build_state{
	struct memory_pool_collection * memory_pool_collection;
	struct unsigned_char_ptr_to_struct_entity_ptr_map targets;
	struct struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map relationships;
};

#endif
