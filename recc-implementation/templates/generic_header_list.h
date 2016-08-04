

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
#ifndef TYPES_DATA_STRUCTURES_struct_T0_IDENTIFIER_list_H_
#include "../generated/struct_T0_IDENTIFIER_list.h"
#endif
#ifndef STRING_H_DEFINED_
#include <string.h>
#endif
/*GETS_REPLACED_WITH_INCLUDES*/

void T0_IDENTIFIER_list_add_end(struct T0_IDENTIFIER_list *, T0_LITERAL);
void T0_IDENTIFIER_list_add_all_end(struct T0_IDENTIFIER_list *, struct T0_IDENTIFIER_list *);
T0_LITERAL T0_IDENTIFIER_list_pop_end(struct T0_IDENTIFIER_list *);
void T0_IDENTIFIER_list_remove_all(struct T0_IDENTIFIER_list *, T0_LITERAL, int (*)(T0_LITERAL, T0_LITERAL));
unsigned int T0_IDENTIFIER_list_occurrences(struct T0_IDENTIFIER_list *, T0_LITERAL, int (*)(T0_LITERAL, T0_LITERAL));
unsigned int T0_IDENTIFIER_list_size(struct T0_IDENTIFIER_list *);
T0_LITERAL * T0_IDENTIFIER_list_data(struct T0_IDENTIFIER_list *);
T0_LITERAL T0_IDENTIFIER_list_get(struct T0_IDENTIFIER_list *, unsigned int);
void T0_IDENTIFIER_list_destroy(struct T0_IDENTIFIER_list *);
void T0_IDENTIFIER_list_create(struct T0_IDENTIFIER_list *);
struct T0_IDENTIFIER_list T0_IDENTIFIER_list_copy(struct T0_IDENTIFIER_list *);
void T0_IDENTIFIER_list_reverse(struct T0_IDENTIFIER_list *);
