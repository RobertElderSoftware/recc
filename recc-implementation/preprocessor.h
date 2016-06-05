#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_ 
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
#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef STRING_H_DEFINED_
#include <string.h>
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef STDLIB_H_DEFINED_
#include <stdlib.h>
#endif
#ifndef IO_H_
#include "io.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../generated/unsigned_char_list.h"
#endif
#ifndef struct_preprocessor_if_branch_ptr_list_H_DEFINED_
#include "../generated/struct_preprocessor_if_branch_ptr_list.h"
#endif
#ifndef struct_struct_c_lexer_token_ptr_list_ptr_list_H_DEFINED_
#include "../generated/struct_struct_c_lexer_token_ptr_list_ptr_list.h"
#endif
#ifndef unsigned_char_ptr_to_unsigned_char_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#endif
#ifndef unsigned_char_ptr_to_struct_macro_definition_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_struct_macro_definition_ptr_map.h"
#endif
#ifndef unsigned_char_ptr_to_struct_special_macro_definition_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_struct_special_macro_definition_ptr_map.h"
#endif
#ifndef struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map_H_DEFINED_
#include "../generated/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.h"
#endif
#ifndef struct_c_lexer_token_ptr_to_unsigned_char_ptr_map_H_DEFINED_
#include "../generated/struct_c_lexer_token_ptr_to_unsigned_char_ptr_map.h"
#endif
#ifndef unsigned_char_ptr_to_struct_macro_parameter_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h"
#endif
#ifndef struct_unsigned_char_list_ptr_list_H_DEFINED_
#include "../generated/struct_unsigned_char_list_ptr_list.h"
#endif
#ifndef struct_c_lexer_state_ptr_list_H_DEFINED_
#include "../generated/struct_c_lexer_state_ptr_list.h"
#endif
#ifndef struct_preprocessor_file_context_ptr_list_H_DEFINED_
#include "../generated/struct_preprocessor_file_context_ptr_list.h"
#endif
#ifndef struct_preprocessor_macro_level_ptr_list_H_DEFINED_
#include "../generated/struct_preprocessor_macro_level_ptr_list.h"
#endif
#ifndef TYPES_PREPROCESSOR_struct_preprocessor_macro_level_H_
#include "../generated/struct_preprocessor_macro_level.h"
#endif
#ifndef TYPES_PREPROCESSOR_struct_special_macro_definition_H_
#include "../generated/struct_special_macro_definition.h"
#endif
#ifndef TYPES_PREPROCESSOR_struct_macro_parameter_H_
#include "../generated/struct_macro_parameter.h"
#endif
#ifndef TYPES_PREPROCESSOR_struct_macro_definition_H_
#include "../generated/struct_macro_definition.h"
#endif
#ifndef TYPES_PREPROCESSOR_struct_preprocessor_if_branch_H_
#include "../generated/struct_preprocessor_if_branch.h"
#endif
#ifndef TYPES_PREPROCESSOR_struct_preprocessor_file_context_H_
#include "../generated/struct_preprocessor_file_context.h"
#endif
#ifndef TYPES_PREPROCESSOR_struct_preprocessor_state_H_
#include "../generated/struct_preprocessor_state.h"
#endif
#ifndef unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_compare.h"
#endif
#ifndef unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_compare.h"
#endif
#ifndef unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_compare.h"
#endif
#ifndef struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_compare_H_DEFINED_
#include "../recc-implementation/comparison-functions/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_compare.h"
#endif

enum search_state{
	MACRO_SEARCH,
	COMPLETE_DIRECTIVE,
	EXPECT_DIRECTIVE,
	BEGIN_LINE,
	NONE
};

enum directive_type{
	DEFINE_DIRECTIVE,
	INCLUDE_DIRECTIVE,
	IF_DIRECTIVE,
	IFDEF_DIRECTIVE,
	IFNDEF_DIRECTIVE,
	ENDIF_DIRECTIVE,
	ERROR_DIRECTIVE,
	PRAGMA_DIRECTIVE
};

void process_tokens(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, enum search_state, unsigned int, struct struct_preprocessor_macro_level_ptr_list *);
void process_line_continuators(struct unsigned_char_list *, struct unsigned_char_list *);
void free_macro_definition_map(struct preprocessor_state *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *);
void free_special_macro_definition_map(struct preprocessor_state *, struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_map *);
int get_preprocessed_output_from_file(struct preprocessor_state *, unsigned char *, struct struct_c_lexer_token_ptr_list *);

struct c_lexer_token * read_until_next_token(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
unsigned int is_non_whitespace_inline_token(struct c_lexer_token *);
void release_working_tokens(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
void process_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
enum directive_type get_directive_type(struct preprocessor_state * state, struct c_lexer_token *);
void process_define_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
void process_include_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
void process_ifndef_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
unsigned int process_identifier_if_macro(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
void split_tokens_into_parameters(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *);
void perform_argument_prescan_all(struct preprocessor_state *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
void fully_expand_macros(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *);
void evaluate_function_macro_body(struct preprocessor_state *, struct macro_definition *, struct struct_c_lexer_token_ptr_list *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *);
struct preprocessor_state * create_preprocessor_state(struct memory_pool_collection *);
void destroy_preprocessor_state(struct preprocessor_state *);
void process_endif_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
unsigned int is_in_active_branch(struct preprocessor_state *);
unsigned int always_processed_directive(enum directive_type);
unsigned char * convert_filename_to_directory(unsigned char *);
void removes_quotes_from_string_literal(unsigned char *);
struct unsigned_char_list * add_tokenizable_input_buffer(struct preprocessor_state *);
struct c_lexer_state * add_c_lexer_state(struct preprocessor_state *);
unsigned char * get_current_file(struct preprocessor_state *);
int do_preprocess(struct memory_pool_collection *, unsigned char *, unsigned char *);
struct c_lexer_token * make_stringified_token(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *);
void add_stringified_character(struct unsigned_char_list *, unsigned int *, unsigned char);
void make_trimmed_arg_list(struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *);
void add_special_macros(struct preprocessor_state *, struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_map *);
void evaluate_special_macro(struct preprocessor_state *, struct special_macro_definition *, struct struct_c_lexer_token_ptr_list *);
void destroy_preprocessor_file_context(struct preprocessor_state *, struct preprocessor_file_context *);
struct preprocessor_file_context * make_preprocessor_file_context(struct preprocessor_state *, unsigned char *);
void increment_current_line(struct preprocessor_state *);
unsigned int get_current_file_line(struct preprocessor_state *);
unsigned char * make_current_file_line_string(struct preprocessor_state *);
unsigned char * make_current_file_string(struct preprocessor_state *);
int c_lexer_token_cmp(struct c_lexer_token *, struct c_lexer_token *);
void print_file_stack(struct preprocessor_state *);
void put_back_token(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct c_lexer_token *, struct struct_preprocessor_macro_level_ptr_list *);
void destroy_preprocessor_macro_level(struct preprocessor_macro_level *, struct memory_pool_collection *);
struct preprocessor_macro_level * create_preprocessor_macro_level(struct macro_definition *, unsigned char *, struct memory_pool_collection *);
struct c_lexer_token * read_one_token(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_preprocessor_macro_level_ptr_list *);
void reduce_multiple_whitespace_to_single(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *);
void shorten_file_path(struct preprocessor_state *, struct unsigned_char_list *);

#endif
