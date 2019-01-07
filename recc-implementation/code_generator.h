#ifndef GENERATE_CODE_ 
#define GENERATE_CODE_ 
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

#ifndef STDLIB_H_DEFINED_
#include <stdlib.h>
#endif
#ifndef STRING_H_DEFINED_
#include <string.h>
#endif
#ifndef PARSER_H_
#include "parser.h"
#endif
#ifndef generated_DIR_code_generator_generated_includes_DOT_h
#include "../generated/code_generator_generated_includes.h"
#endif
#ifndef LINKER_H_
#include "linker.h"
#endif
#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef STDARG_H_DEFINED_
#include <stdarg.h>
#endif
#ifndef OP_CPU_H_
#include "../emulators/c/op-cpu.h"
#endif


void create_code_gen_state(struct code_gen_state *, struct parser_state *, struct unsigned_char_list *, struct unsigned_char_list *);
int generate_code(struct code_gen_state *);
unsigned int do_code_generation(struct memory_pool_collection *, unsigned char *, unsigned char *, unsigned char *);
int destroy_code_gen_state(struct code_gen_state *);
struct compile_time_constant * evaluate_constant_constant_expression(struct code_gen_state *, struct parser_node *);
unsigned int type_size(struct code_gen_state *, struct type_description *, enum value_type, unsigned int, struct scope_level *);
unsigned int struct_type_size(struct code_gen_state *, struct type_description *, enum value_type, struct scope_level *);


struct parser_node * first_child(struct parser_node *);
struct parser_node * second_child(struct parser_node *);
struct parser_node * third_child(struct parser_node *);
struct parser_node * fourth_child(struct parser_node *);
struct parser_node * fifth_child(struct parser_node *);
struct parser_node * sixth_child(struct parser_node *);
struct parser_node * seventh_child(struct parser_node *);
struct parser_node * is_first_child_type(struct parser_node *, enum node_type);
struct parser_node * is_second_child_type(struct parser_node *, enum node_type);
struct parser_node * is_third_child_type(struct parser_node *, enum node_type);
struct parser_node * is_fourth_child_type(struct parser_node *, enum node_type);
struct parser_node * is_fifth_child_type(struct parser_node *, enum node_type);
struct parser_node * is_sixth_child_type(struct parser_node *, enum node_type);
struct parser_node * is_seventh_child_type(struct parser_node *, enum node_type);
int check_one_child(struct parser_node *, enum node_type);
int check_two_children(struct parser_node *, enum node_type, enum node_type);
int check_three_children(struct parser_node *, enum node_type, enum node_type, enum node_type);
int check_four_children(struct parser_node *, enum node_type, enum node_type, enum node_type, enum node_type);
int check_five_children(struct parser_node *, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type);
int check_six_children(struct parser_node *, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type);
int check_seven_children(struct parser_node *, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type, enum node_type);
int is_terminal_c_token_type(struct parser_node *, enum c_token_type);

void function_call(struct parser_node *, struct parser_node *, struct code_gen_state *, struct parser_node *);
unsigned int get_word_size(void);
void exit_scope_for_current_parser_node(struct code_gen_state *, struct parser_node *);
void update_scope_for_current_parser_node(struct code_gen_state *, struct parser_node *);
void create_default_return_value(struct code_gen_state *, struct parser_node *, struct parser_node *);
void do_default_function_return(struct code_gen_state *, struct parser_node *, struct parser_node *);
void return_from_function(struct code_gen_state *, struct parser_node *);
void load_identifier(struct code_gen_state *, unsigned char * identifier, struct parser_node *);
void push_type(struct code_gen_state *, struct type_description *, struct parser_node *);
struct type_description * pop_type(struct code_gen_state *, struct type_description *, struct parser_node *);
struct type_description * pop_type_without_type_check(struct code_gen_state *, struct parser_node *);
void backtrack_type_stack(struct code_gen_state *, unsigned int, struct parser_node *);
void do_assignment(struct code_gen_state *, struct parser_node *);

unsigned int get_parameter_offset(struct code_gen_state *, struct namespace_object *);
unsigned int get_local_offset_h1(struct code_gen_state *, struct namespace_object *, unsigned int);
unsigned int get_local_offset(struct code_gen_state *, struct namespace_object *);
unsigned int get_namespace_object_size(struct code_gen_state *, struct namespace_object *, unsigned int);
struct type_description * consume_scalar_type(struct code_gen_state *, struct parser_node *);
struct type_description * usual_arithmetic_conversion(struct code_gen_state *, struct parser_node *);
struct type_description * perform_pointer_conversion(struct code_gen_state *, struct parser_node *);

void require_external_symbol(struct memory_pool_collection *, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *, unsigned char * );
void implement_external_symbol(struct memory_pool_collection *, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *, unsigned char * );
void require_internal_symbol(struct memory_pool_collection *, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *, unsigned char * );
void implement_internal_symbol(struct memory_pool_collection *, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *, unsigned char * );
struct linker_symbol * make_linker_symbol(struct memory_pool_collection *, unsigned int, unsigned int, unsigned int);
void pop(struct code_gen_state *, const char *, enum value_type);
void push(struct code_gen_state *, const char *, enum value_type);
void do_signed_operation_function_call(struct code_gen_state *, const char *);
void manage_local_space(struct code_gen_state *, struct namespace_object *, unsigned);
void clear_locals_from_scope(struct code_gen_state *, struct scope_level *);
void find_child_case_labels(struct code_gen_state *, struct parser_node *, struct switch_frame *);
void find_child_case_labels_h(struct code_gen_state *, struct parser_node *, struct switch_frame *);
void do_struct_dereference_operator(struct code_gen_state *, unsigned char *, struct parser_node *, unsigned int);
unsigned int calculate_type_stack_size(struct code_gen_state *);
void delete_top_type(struct code_gen_state *);
void copy_words(struct code_gen_state *, const char *, const char *, const char *, unsigned int);
struct type_description * manage_assignment_type_change(struct code_gen_state *, struct parser_node *);
struct type_traversal * construct_type_traversal(struct code_gen_state *, struct type_description *, struct scope_level *, unsigned int);
void destroy_type_traversal(struct memory_pool_collection *, struct type_traversal *);
void move_pointers_down(struct code_gen_state *, const char *, const char *);
struct constant_initializer_level * evaluate_constant_initializer(struct code_gen_state *, struct parser_node *);
struct compile_time_constant * evaluate_constant_literal(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_sizeof_type_name(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_identifier(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_primary_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_postfix_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_cast_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_unary_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_multiplicative_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_additive_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_equality_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_relational_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_shift_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_and_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_inclusive_or_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_exclusive_or_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_logical_and_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_logical_or_expression(struct code_gen_state * code_gen_state, struct parser_node *);
struct compile_time_constant * evaluate_constant_assignment_expression(struct code_gen_state *, struct parser_node *);
struct compile_time_constant * evaluate_constant_conditional_expression(struct code_gen_state *, struct parser_node *);
void evaluate_constant_initializer_list(struct code_gen_state *, struct constant_initializer_level *, struct parser_node *);
unsigned int evaluate_compile_time_constant(struct code_gen_state * code_gen_state, struct compile_time_constant *);
void destroy_constant_initializer_level(struct constant_initializer_level *);
void setup_global_type(struct code_gen_state *, struct type_traversal *, struct constant_initializer_level *, struct struct_compile_time_constant_ptr_list *, struct struct_type_description_ptr_list *);
void setup_global_primative(struct code_gen_state *, struct type_description *, struct constant_initializer_level *);
void ensure_top_values_are_rvalues(struct code_gen_state *, struct parser_node *);
void l_to_lr_transform(struct code_gen_state *, struct type_description *);
void do_multiplicative_expression(struct code_gen_state *, struct parser_node *, enum c_token_type);
void do_additive_expression(struct code_gen_state *, struct parser_node *, enum c_token_type);
void do_shift_expression(struct code_gen_state *, struct parser_node *, enum c_token_type);
void do_and_expression(struct code_gen_state *, struct parser_node *, enum c_token_type);
void do_or_expression(struct code_gen_state *, struct parser_node *, enum c_token_type);
void do_xor_expression(struct code_gen_state *, struct parser_node *, enum c_token_type);
struct type_description * ensure_top_type_is_r_value(struct code_gen_state *, struct parser_node *);
void copy_lvalue_into_rvalue(struct code_gen_state *, const char *, const char *, const char *, const char *, unsigned int);
void perform_integral_promotion(struct code_gen_state * code_gen_state, struct type_description **, const char *);
void perform_integral_promotions(struct code_gen_state * code_gen_state, struct type_description **, struct type_description **);
void do_character_rvalue_conversion(struct code_gen_state *, const char *, const char *, const char *, const char *, const char *);
void convert_top_rvalue_to_target_type(struct code_gen_state *, struct type_description *, struct type_description *);
unsigned int decay_to_pointer_if_array(struct code_gen_state *, struct type_description **);
void g_rest_of_logical_or(struct parser_node *, struct code_gen_state *);
void g_rest_of_logical_and(struct parser_node *, struct code_gen_state *);
unsigned int do_specifiers_contain_extern(struct parser_node *, enum c_token_type);
void compare_function_argument_type(struct code_gen_state *, struct parser_node *, unsigned int, struct parser_node *);
struct namespace_object * get_current_function(struct code_gen_state *);


#endif
