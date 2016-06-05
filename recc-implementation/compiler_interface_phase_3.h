#ifndef COMPILER_INTERFACE_H_
#define COMPILER_INTERFACE_H_
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
#include "../generated/unsigned_char_ptr_list.h"
#endif
#ifndef unsigned_char_ptr_to_struct_entity_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_struct_entity_ptr_map.h"
#endif
#ifndef unsigned_char_ptr_to_unsigned_char_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../generated/unsigned_char_list.h"
#endif
#ifndef struct_c_lexer_token_memory_pool_H_DEFINED_
#include "../generated/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef struct_l2_lexer_token_memory_pool_H_DEFINED_
#include "../generated/struct_l2_lexer_token_memory_pool.h"
#endif
#ifndef struct_parser_node_memory_pool_H_DEFINED_
#include "../generated/struct_parser_node_memory_pool.h"
#endif
#ifndef struct_type_description_memory_pool_H_DEFINED_
#include "../generated/struct_type_description_memory_pool.h"
#endif
#ifndef MEMORY_POOL_COLLECTION_H_
#include "memory_pool_collection.h"
#endif
#include "../recc-implementation/struct_build_state_phase_3.h"
#ifndef L0_GENERATOR_H_
#include "l0_generator.h"
#endif
#ifndef FILESYSTEM_COMPILER_H_
#include "../recc-implementation/filesystem_compiler.h"
#endif
#ifndef unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare.h"
#endif
#ifndef struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare.h"
#endif
#ifndef struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_H_DEFINED_
#include "../generated/struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map.h"
#endif
#ifndef unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare.h"
#endif
#ifndef struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare.h"
#endif

#include "../recc-implementation/libbootstrap.h"

struct build_state * create_build_state(void);
void destroy_build_state(struct build_state *);
const char * register_entity(struct build_state *, const char *, enum entity_type);
const char * register_dependency(struct build_state *, const char *, const char *);
unsigned int construct_entity(struct build_state *, const char *);
void add_entity_attribute(struct build_state *, const char *, const char *, const char *);
struct files_for_type * make_build_rules_for_typed_files(struct bootstrap_state *, struct build_state *, struct files_for_type *);
struct entity * get_entity(struct build_state *, const char *);
void construct_generated_c_entities(struct build_state *);

#endif
