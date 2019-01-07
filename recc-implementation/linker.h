#ifndef LINKER_H_
#define LINKER_H_
/*
    Copyright 2019 Robert Elder Software Inc.
    
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
#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef IO_H_
#include "io.h"
#endif
#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef PARSER_H_
#include "parser.h"
#endif
#ifndef L2_PARSER_H_
#include "l2_parser.h"
#endif
#ifndef LINKER_H_
#include "linker.h"
#endif
#ifndef L0_GENERATOR_H_
#include "l0_generator.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../generated/unsigned_char_list.h"
#endif
#ifndef unsigned_char_ptr_list_H_DEFINED_
#include "../generated/unsigned_char_ptr_list.h"
#endif
#ifndef struct_unsigned_char_list_ptr_list_H_DEFINED_
#include "../generated/struct_unsigned_char_list_ptr_list.h"
#endif
#ifndef struct_l2_item_ptr_list_H_DEFINED_
#include "../generated/struct_l2_item_ptr_list.h"
#endif
#ifndef struct_linker_object_ptr_list_H_DEFINED_
#include "../generated/struct_linker_object_ptr_list.h"
#endif
#ifndef struct_l2_lexer_state_ptr_list_H_DEFINED_
#include "../generated/struct_l2_lexer_state_ptr_list.h"
#endif
#ifndef struct_l2_parser_state_ptr_list_H_DEFINED_
#include "../generated/struct_l2_parser_state_ptr_list.h"
#endif
#ifndef struct_linker_file_ptr_list_H_DEFINED_
#include "../generated/struct_linker_file_ptr_list.h"
#endif
#ifndef struct_linker_region_list_H_DEFINED_
#include "../generated/struct_linker_region_list.h"
#endif
#ifndef struct_start_end_list_H_DEFINED_
#include "../generated/struct_start_end_list.h"
#endif
#ifndef unsigned_char_ptr_to_struct_linker_symbol_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h"
#endif
#ifndef struct_linker_file_ptr_merge_sort_H_DEFINED_
#include "../generated/struct_linker_file_ptr_merge_sort.h"
#endif
#ifndef struct_start_end_merge_sort_H_DEFINED_
#include "../generated/struct_start_end_merge_sort.h"
#endif
#ifndef struct_l2_item_memory_pool_H_DEFINED_
#include "../generated/struct_l2_item_memory_pool.h"
#endif
#ifndef struct_linker_symbol_memory_pool_H_DEFINED_
#include "../generated/struct_linker_symbol_memory_pool.h"
#endif
#ifndef TYPES_LINKER_struct_linker_symbol_h_
#include "../generated/struct_linker_symbol.h"
#endif
#ifndef TYPES_LINKER_struct_l2_item_h_
#include "../generated/struct_l2_item.h"
#endif
#ifndef OP_CPU_H_
#include "../emulators/c/op-cpu.h"
#endif
#ifndef TYPES_LINKER_struct_linker_state_h_
#include "../generated/struct_linker_state.h"
#endif
#ifndef struct_start_end_ptr_compare_H_DEFINED_
#include "../generated/struct_start_end_compare.h"
#endif
#ifndef unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_compare_H_DEFINED_
#include "../generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_compare.h"
#endif


int do_link(struct memory_pool_collection *, struct unsigned_char_ptr_list *, unsigned char *, unsigned char *, enum entity_type, unsigned char *, unsigned int, unsigned int);
struct linker_file * create_linker_file(struct linker_state *, struct l2_parser_state *, unsigned char *);
void set_symbol_l2_item_pointer(struct linker_state *, struct linker_file *, struct l2_lexer_token *, struct l2_item *, unsigned int);
void add_internal_linker_symbol(struct linker_state *, struct linker_file *, struct l2_lexer_token *, unsigned int, unsigned int);
void add_external_linker_symbol(struct linker_state *, struct linker_file *, struct l2_lexer_token *, unsigned int, unsigned int);
void output_artifacts(struct linker_state *, struct unsigned_char_list *, struct linker_file *);
void output_symbols(struct linker_state *, struct linker_file *, struct unsigned_char_list *, unsigned char *);
unsigned int get_relative_symbol_offset(struct linker_state *, struct linker_file *, struct linker_symbol *, unsigned int *);
unsigned int get_absolute_symbol_offset(struct linker_state *, unsigned char *, struct linker_file *, unsigned int *);
void verify_symbol_declaration(struct linker_state *, struct linker_file *, struct l2_lexer_token *);
void linker_state_destroy(struct linker_state *);
void linker_state_create(struct linker_state *, struct memory_pool_collection *, struct unsigned_char_ptr_list *, unsigned char *, unsigned char *, enum entity_type, unsigned char *, unsigned int, unsigned int);
void do_link_to_l1(struct linker_state *);
void do_link_to_l2(struct linker_state *);
void set_all_post_linking_offsets(struct linker_state *);
void group_and_page_align_linker_files(struct linker_state *);
struct linker_symbol * get_absolute_symbol(struct linker_state *, unsigned char *, struct linker_file *);

#endif
