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
#ifndef struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare_H_DEFINED_
#include "struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare.h"
#endif

int struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare(struct struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair * a, struct struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair * b){
	if(a->key.entity > b->key.entity){
		return 1;
	}else if(a->key.entity < b->key.entity){
		return -1;
	}else{
		if(a->key.type > b->key.type){
			return 1;
		}else if(a->key.type < b->key.type){
			return -1;
		}else{
			return 0;
		}
	}
}
