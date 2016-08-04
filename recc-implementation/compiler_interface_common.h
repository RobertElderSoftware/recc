#ifndef COMPILER_INTERFACE_COMMON_H_
#define COMPILER_INTERFACE_COMMON_H_
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
#ifndef struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map_H_DEFINED_
#include "../generated/struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_map.h"
#endif
#ifndef unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare_H_DEFINED_
#include "../generated/struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair_compare.h"
#endif
#ifndef unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_compare_H_DEFINED_
#include "../generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_compare.h"
#endif
#ifndef struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare_H_DEFINED_
#include "../generated/struct_struct_entity_relationship_to_struct_struct_entity_ptr_list_ptr_key_value_pair_compare.h"
#endif
#ifndef enum_entity_relation_type_H_
#include "enum_entity_relation_type.h"
#endif
#ifndef LIBBOOTSTRAP_H_DEFINED_
#include "../recc-implementation/libbootstrap.h"
#endif
#include "../recc-implementation/struct_build_state.h"

unsigned int make_header_target(struct build_state *, struct entity *);
struct build_state * create_build_state(void);
void destroy_build_state(struct build_state *);
const char * register_entity(struct build_state *, const char *, enum entity_type);
const char * register_composition(struct build_state *, const char *, const char *);
const char * register_inclusion(struct build_state *, const char *, const char *);
const char * register_dependency(struct build_state *, const char *, const char *);
unsigned int construct_entity(struct build_state *, const char *);
void add_entity_attribute(struct build_state *, const char *, const char *, const char *);
struct files_for_type * make_build_rules_for_typed_files(struct bootstrap_state *, struct build_state *, struct files_for_type *);
struct entity * get_entity(struct build_state *, const char *);
void construct_generated_c_entities(struct build_state *);
void destroy_build_state(struct build_state *);
unsigned int make_target(struct build_state *, struct entity*);
unsigned char * copy_of_string(unsigned char *);
unsigned char * get_parent_symbol_file(struct build_state *, struct entity *);
struct entity * make_new_entity(unsigned char *);
void destroy_entity(struct entity *);
void register_relationship(struct build_state *, unsigned char *, unsigned char *, enum entity_relation_type);
struct unsigned_char_ptr_list get_entity_name_list_for_relationship(struct build_state *, struct entity *, enum entity_relation_type);
struct struct_entity_ptr_list * get_entity_list_for_relationship(struct build_state *, struct entity *, enum entity_relation_type);
int struct_entity_cmp(struct entity *, struct entity *);

#endif
