#ifndef L0_GENERATOR_H_
#define L0_GENERATOR_H_
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

#ifndef IO_H_
#include "io.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../data-structures/unsigned_char_list.h"
#endif
#ifndef struct_l0_item_list_H_DEFINED_
#include "../data-structures/struct_l0_item_list.h"
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef STDLIB_H_DEFINED_
#include <stdlib.h>
#endif
#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef L2_PARSER_H_
#include "l2_parser.h"
#endif
#ifndef OP_CPU_H_
#include "../emulators/c/op-cpu.h"
#endif
#ifndef TYPES_L0_GENERATOR_struct_l0_generator_state_H_
#include "../types/l0_generator/struct_l0_generator_state.h"
#endif

struct l0_generator_state * l0_generator_state_create(struct memory_pool_collection *, unsigned char *, unsigned char *, unsigned char *, unsigned char *);
void l0_generator_state_destroy(struct l0_generator_state *);

#endif
