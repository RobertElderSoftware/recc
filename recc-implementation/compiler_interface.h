#ifndef COMPILER_INTERFACE_H_
#define COMPILER_INTERFACE_H_
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

#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef STDLIB_H_DEFINED_
#include <stdlib.h>
#endif
#ifndef PREPROCESSOR_H_
#include "preprocessor.h"
#endif
#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef PARSER_H_
#include "parser.h"
#endif
#ifndef LINKER_H_
#include "linker.h"
#endif
#ifndef GENERATE_CODE_ 
#include "code_generator.h"
#endif
#ifndef unsigned_char_ptr_list_H_DEFINED_
#include "../data-structures/unsigned_char_ptr_list.h"
#endif
#ifndef unsigned_char_ptr_to_struct_build_target_ptr_map_H_DEFINED_
#include "../data-structures/unsigned_char_ptr_to_struct_build_target_ptr_map.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../data-structures/unsigned_char_list.h"
#endif
#ifndef struct_c_lexer_token_memory_pool_H_DEFINED_
#include "../data-structures/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef struct_l2_lexer_token_memory_pool_H_DEFINED_
#include "../data-structures/struct_l2_lexer_token_memory_pool.h"
#endif
#ifndef struct_parser_node_memory_pool_H_DEFINED_
#include "../data-structures/struct_parser_node_memory_pool.h"
#endif
#ifndef struct_type_description_memory_pool_H_DEFINED_
#include "../data-structures/struct_type_description_memory_pool.h"
#endif
#ifndef MEMORY_POOL_COLLECTION_H_
#include "memory_pool_collection.h"
#endif
#ifndef TYPES_struct_build_target_H_
#include "../types/recc-implementation/struct_build_target.h"
#endif
#ifndef TYPES_struct_build_state_H_
#include "../types/recc-implementation/struct_build_state.h"
#endif
#ifndef L0_GENERATOR_H_
#include "l0_generator.h"
#endif
#ifndef FILESYSTEM_COMPILER_H_
#include "../recc-implementation/filesystem_compiler.h"
#endif

struct build_state * create_build_state(void);
void destroy_build_state(struct build_state *);
const char * register_build_target(struct build_state *, const char *, enum build_target_type);
const char * register_dependency(struct build_state *, const char *, const char *);
void construct_build_target(struct build_state *, const char *);
void register_c_to_l1(struct build_state *, const char *);
void register_c_to_l2(struct build_state *, const char *);
void register_l0_file(struct build_state *, const char *, const char *, const char *);
void register_l1_file(struct build_state *, const char *, unsigned int, unsigned int);
void register_l2_file(struct build_state *, const char *, const char *);

#endif
