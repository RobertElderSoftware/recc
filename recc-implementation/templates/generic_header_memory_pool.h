

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

#ifndef ASSERT_H_DEFINED__
#include <assert.h>
#endif
#ifndef void_ptr_list__H__DEFINED__
#include "void_ptr_list.h"
#endif
#ifndef unsigned_int_list__H__DEFINED__
#include "unsigned_int_list.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_T0_IDENTIFIER_memory_pool_H__
#include "../generated/struct_T0_IDENTIFIER_memory_pool.h"
#endif
/*GETS_REPLACED_WITH_INCLUDES*/

void T0_IDENTIFIER_memory_pool_create(struct T0_IDENTIFIER_memory_pool *);
void T0_IDENTIFIER_memory_pool_destroy(struct T0_IDENTIFIER_memory_pool *);
T0_LITERAL * T0_IDENTIFIER_memory_pool_malloc(struct T0_IDENTIFIER_memory_pool *);
T0_LITERAL * T0_IDENTIFIER_memory_pool_malloc_tracking(struct T0_IDENTIFIER_memory_pool *, void **);
void T0_IDENTIFIER_memory_pool_free(struct T0_IDENTIFIER_memory_pool *, T0_LITERAL *);
