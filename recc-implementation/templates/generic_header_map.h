

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

#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_binary_search_H_DEFINED_
#include "struct_T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair_binary_search.h"
#endif
#ifndef T0_IDENTIFIER_list_H_DEFINED_
#include "T0_IDENTIFIER_list.h"
#endif
#ifndef T1_IDENTIFIER_list_H_DEFINED_
#include "T1_IDENTIFIER_list.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_T0_IDENTIFIER_to_T1_IDENTIFIER_map_H_
#include "../generated/struct_T0_IDENTIFIER_to_T1_IDENTIFIER_map.h"
#endif
/*GETS_REPLACED_WITH_INCLUDES*/

T1_LITERAL * T0_IDENTIFIER_to_T1_IDENTIFIER_map_exists(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *, T0_LITERAL);
void T0_IDENTIFIER_to_T1_IDENTIFIER_map_create(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map * map, int (*)(struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *, struct T0_IDENTIFIER_to_T1_IDENTIFIER_key_value_pair *));
void T0_IDENTIFIER_to_T1_IDENTIFIER_map_destroy(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *);
void T0_IDENTIFIER_to_T1_IDENTIFIER_map_put(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *, T0_LITERAL, T1_LITERAL);
T1_LITERAL T0_IDENTIFIER_to_T1_IDENTIFIER_map_get(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *, T0_LITERAL);
struct T0_IDENTIFIER_list T0_IDENTIFIER_to_T1_IDENTIFIER_map_keys(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *);
struct T1_IDENTIFIER_list T0_IDENTIFIER_to_T1_IDENTIFIER_map_values(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *);
unsigned int T0_IDENTIFIER_to_T1_IDENTIFIER_map_size(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *);
void T0_IDENTIFIER_to_T1_IDENTIFIER_map_remove(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *, T0_LITERAL);
struct T0_IDENTIFIER_to_T1_IDENTIFIER_map T0_IDENTIFIER_to_T1_IDENTIFIER_map_copy(struct T0_IDENTIFIER_to_T1_IDENTIFIER_map *);
