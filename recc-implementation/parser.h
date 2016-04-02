#ifndef PARSER_H_
#define PARSER_H_
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
#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef unsigned_int_stack_H_DEFINED_
#include "../data-structures/unsigned_int_stack.h"
#endif
#ifndef struct_type_description_reference_list_H_DEFINED_
#include "../data-structures/struct_type_description_reference_list.h"
#endif
#ifndef char_ptr_list_H_DEFINED_
#include "../data-structures/char_ptr_list.h"
#endif
#ifndef struct_parser_operation_stack_H_DEFINED_
#include "../data-structures/struct_parser_operation_stack.h"
#endif
#ifndef struct_normalized_specifier_ptr_list_H_DEFINED_
#include "../data-structures/struct_normalized_specifier_ptr_list.h"
#endif
#ifndef struct_normalized_declarator_ptr_list_H_DEFINED_
#include "../data-structures/struct_normalized_declarator_ptr_list.h"
#endif
#ifndef struct_normalized_declaration_element_ptr_list_H_DEFINED_
#include "../data-structures/struct_normalized_declaration_element_ptr_list.h"
#endif
#ifndef struct_namespace_object_ptr_list_H_DEFINED_
#include "../data-structures/struct_namespace_object_ptr_list.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../data-structures/unsigned_char_list.h"
#endif
#ifndef unsigned_char_ptr_list_H_DEFINED_
#include "../data-structures/unsigned_char_ptr_list.h"
#endif
#ifndef unsigned_char_ptr_to_struct_constant_description_ptr_map_H_DEFINED_
#include "../data-structures/unsigned_char_ptr_to_struct_constant_description_ptr_map.h"
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
#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef STDARG_H_DEFINED_
#include <stdarg.h>
#endif
#ifndef struct_parser_node_memory_pool_H_DEFINED_
#include "../data-structures/struct_parser_node_memory_pool.h"
#endif
#ifndef struct_c_lexer_token_memory_pool_H_DEFINED_
#include "../data-structures/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef struct_type_description_memory_pool_H_DEFINED_
#include "../data-structures/struct_type_description_memory_pool.h"
#endif
#ifndef TYPES_PARSER_enum_parser_operation_type_H_
#include "../types/parser/enum_parser_operation_type.h"
#endif
#ifndef TYPES_PARSER_enum_type_class_H_
#include "../types/parser/enum_type_class.h"
#endif
#ifndef TYPES_PARSER_enum_scope_type_H_
#include "../types/parser/enum_scope_type.h"
#endif
#ifndef TYPES_PARSER_enum_add_or_remove_H_
#include "../types/parser/enum_add_or_remove.h"
#endif
#ifndef TYPES_PARSER_enum_declaration_or_definition_H_
#include "../types/parser/enum_declaration_or_definition.h"
#endif
#ifndef TYPES_PARSER_enum_normalized_declaration_type_H_
#include "../types/parser/enum_normalized_declaration_type.h"
#endif
#ifndef TYPES_PARSER_enum_normalized_declarator_type_H_
#include "../types/parser/enum_normalized_declarator_type.h"
#endif
#ifndef TYPES_PARSER_enum_normalized_specifier_type_H_
#include "../types/parser/enum_normalized_specifier_type.h"
#endif
#ifndef TYPES_PARSER_enum_value_type_H_
#include "../types/parser/enum_value_type.h"
#endif
#ifndef TYPES_PARSER_enum_object_location_H_
#include "../types/parser/enum_object_location.h"
#endif
#ifndef TYPES_PARSER_struct_type_description_H_
#include "../types/parser/struct_type_description.h"
#endif
#ifndef TYPES_PARSER_struct_parser_state_H_
#include "../types/parser/struct_parser_state.h"
#endif
#ifndef TYPES_PARSER_struct_normalized_specifier_H_
#include "../types/parser/struct_normalized_specifier.h"
#endif
#ifndef TYPES_PARSER_struct_normalized_declarator_H_
#include "../types/parser/struct_normalized_declarator.h"
#endif
#ifndef TYPES_PARSER_struct_normalized_declaration_set_H_
#include "../types/parser/struct_normalized_declaration_set.h"
#endif
#ifndef TYPES_PARSER_struct_namespace_modification_H_
#include "../types/parser/struct_namespace_modification.h"
#endif
#ifndef TYPES_PARSER_struct_namespace_object_change_H_
#include "../types/parser/struct_namespace_object_change.h"
#endif
#ifndef TYPES_PARSER_struct_first_and_last_namespace_object_H_
#include "../types/parser/struct_first_and_last_namespace_object.h"
#endif
#ifndef TYPES_PARSER_struct_current_function_change_H_
#include "../types/parser/struct_current_function_change.h"
#endif
#ifndef TYPES_PARSER_struct_parser_operation_H_
#include "../types/parser/struct_parser_operation.h"
#endif
#ifndef TYPES_PARSER_struct_parser_checkpoint_H_
#include "../types/parser/struct_parser_checkpoint.h"
#endif
#ifndef TYPE_ENGINE_H_
#include "type_engine.h"
#endif

void * push_operation(struct parser_state *, enum parser_operation_type, void *);
void pop_operation(struct parser_state *);
void backtrack(struct parser_state *, struct parser_checkpoint);

struct parser_node * get_identifier_from_declarator(struct parser_node *);
unsigned char * copy_l2_token(struct l2_lexer_token *);
void create_parser_state(struct parser_state *, struct memory_pool_collection *, struct c_lexer_state *, struct unsigned_char_list *, unsigned char *, struct type_engine_state *);
unsigned int parse(struct parser_state *);
void destroy_parser_state(struct parser_state *);
const char ** get_node_type_names(void);
struct namespace_object * get_namespace_object_from_scope_namespace_using_string(struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *, unsigned char *);
struct constant_description * find_constant(struct parser_state *, unsigned char *);
struct parser_node * create_abstract_declarator_from_normalized_declarator(struct memory_pool_collection *, struct normalized_declarator *);
struct parser_node * get_constant_expression_from_abstract_declarator(struct parser_node *);
struct type_description_reference create_type_description_from_normalized_declaration_element(struct memory_pool_collection *, struct normalized_declaration_element *, struct parser_node *, struct scope_level *, enum value_type);
void destroy_type_description(struct memory_pool_collection *, struct type_description_reference);
void print_normalized_declaration_element(struct memory_pool_collection *, struct unsigned_char_list *, struct normalized_declaration_element *, unsigned int);
void print_normalized_declaration_declarator_and_specifiers(struct memory_pool_collection *, struct unsigned_char_list *, struct normalized_declarator *, struct struct_normalized_specifier_ptr_list *, struct parser_node *, unsigned int);

struct parser_node * copy_parser_node_children_only(struct memory_pool_collection *, struct parser_node *);
struct parser_node * copy_parser_node_tree_and_c_lexer_tokens(struct memory_pool_collection *, struct parser_node *);
struct parser_node * destroy_parser_node_tree_and_c_lexer_tokens(struct memory_pool_collection *, struct parser_node *);
struct type_description_reference copy_type_description(struct memory_pool_collection *, struct type_description_reference);
unsigned int type_description_cmp(struct memory_pool_collection *, struct type_description_reference, struct type_description_reference);
struct type_description_reference create_type_description_from_normalized_declarator_and_specifiers(struct memory_pool_collection *, struct normalized_declarator *, struct struct_normalized_specifier_ptr_list *, struct parser_node *, struct scope_level *, enum value_type, struct normalized_declaration_element *);
struct parser_node * convert_abstract_declarator_to_function_type(struct memory_pool_collection *, struct parser_node *);
void convert_to_untypedefed_type_description(struct memory_pool_collection *, struct type_description_reference);
struct type_description_reference create_address_type_description_from_type_description(struct memory_pool_collection *, struct type_description_reference);
struct type_description_reference create_dereferenced_pointer_type_description_from_type_description(struct memory_pool_collection *, struct type_description_reference);
struct type_description_reference create_dereferenced_array_type_description_from_type_description(struct memory_pool_collection *, struct type_description_reference);
unsigned int is_type_description_a_function(struct memory_pool_collection *, struct type_description_reference);
unsigned int is_type_description_a_function_pointer(struct memory_pool_collection *, struct type_description_reference);
struct type_description_reference get_current_function_return_type_description(struct memory_pool_collection *, struct type_description_reference);
struct c_lexer_token * get_identifier_token_from_normalized_declarator(struct normalized_declarator *);

struct type_description_reference create_type_description_from_type_name(struct memory_pool_collection *, struct parser_state *, struct parser_node *);
unsigned int is_signed(struct type_description_reference t);
void print_node_context(struct c_lexer_state *, struct parser_node *);
struct parser_node * get_struct_or_union_or_enum_specifier(struct struct_normalized_specifier_ptr_list *);
unsigned int is_struct(struct parser_node *);
unsigned int is_union(struct parser_node *);
unsigned int contains_struct_or_union_or_enum_definition(struct namespace_object *);
unsigned int contains_function_definition(struct namespace_object *);
struct parser_node * get_function_definition_from_namespace_object(struct namespace_object *);
unsigned int is_enum(struct parser_node *);
unsigned int get_enum_value(struct memory_pool_collection *, struct normalized_declaration_element *);
unsigned int convert_decimal_constant(unsigned char *);

enum type_class determine_type_class(struct memory_pool_collection *, struct type_description_reference);
unsigned int void_type_size(struct memory_pool_collection *, enum value_type);
unsigned int arithmetic_type_size(struct memory_pool_collection *, struct type_description_reference, enum value_type);
unsigned int pointer_type_size(struct memory_pool_collection *, struct type_description_reference, enum value_type);
unsigned int enum_type_size(struct memory_pool_collection *, struct type_description_reference, enum value_type);
unsigned int get_ceil_modulo(unsigned int, unsigned int);
void remove_enum(struct memory_pool_collection *, struct type_description_reference);
void remove_specifier(struct memory_pool_collection *, struct type_description_reference, unsigned int, enum c_token_type);
struct type_description_reference add_specifier(struct memory_pool_collection *, struct type_description_reference, enum c_token_type);
unsigned int count_specifiers(struct type_description_reference, enum c_token_type);
struct type_description_reference create_empty_type_description(struct memory_pool_collection *);
unsigned int is_integral_type(struct memory_pool_collection *, struct type_description_reference);
unsigned int is_floating_type(struct memory_pool_collection *, struct type_description_reference);
unsigned int is_arithmetic_type(struct memory_pool_collection *, struct type_description_reference);
unsigned int is_scalar_type(struct memory_pool_collection *, struct type_description_reference);
struct c_lexer_token * get_struct_or_union_or_enum_tag_token(struct parser_node *);
struct parser_node * get_struct_declaration_list(struct parser_node *);
struct normalized_declaration_set * create_normalized_declaration_set_from_parser_node(struct memory_pool_collection *, struct parser_node *, struct normalized_declaration_set *);
struct struct_normalized_declaration_element_ptr_list * create_normalized_declaration_element_list(struct normalized_declaration_set *);
void destroy_normalized_declaration_element_list(struct memory_pool_collection *, struct struct_normalized_declaration_element_ptr_list*);
struct parser_node * get_enumerator_list(struct parser_node *);
struct namespace_object * get_namespace_object_from_closest_namespace(unsigned char *, enum scope_type, struct scope_level *, unsigned int, struct memory_pool_collection *);
unsigned char * make_up_identifier(struct normalized_declaration_element *, struct memory_pool_collection *);
struct normalized_declarator * make_array_brackets(struct memory_pool_collection *);
void print_error_with_types(struct c_lexer_state *, struct type_description_reference, struct type_description_reference, struct parser_node *, const char *);
void print_error_with_type(struct c_lexer_state *, struct type_description_reference, struct parser_node *, const char *);
struct type_description_reference get_type_description_from_suffix(struct memory_pool_collection *, unsigned char *);
struct parser_node * get_nth_parameter_declaration_from_parameter_type_list(struct parser_node *, unsigned int);
struct parser_node * get_parameter_type_list_from_abstract_declarator(struct parser_node *);
unsigned int is_function_variadic(struct parser_node *);

struct parser_node * specifier_qualifier_list(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * type_qualifier_list_rest(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * type_qualifier_list(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * declaration_specifiers(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * storage_class_specifier(struct parser_state *, unsigned int *);
struct parser_node * type_qualifier(struct parser_state *, unsigned int *);
struct parser_node * type_specifier(struct parser_state *, unsigned int *);
struct parser_node * get_identifier_node(struct parser_state *, unsigned int *);
struct parser_node * parameter_list(struct parser_state *, unsigned int *);
struct parser_node * declarator(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * direct_declarator(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * direct_declarator_rest(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * abstract_declarator(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * direct_abstract_declarator_rest(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * direct_abstract_declarator(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * pointer(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * init_declarator(struct parser_state *, unsigned int *);
struct parser_node * struct_declarator(struct parser_state *, unsigned int *);
struct parser_node * struct_declarator_list_rest(struct parser_state *, unsigned int, unsigned int, unsigned int *);
struct parser_node * struct_declarator_list(struct parser_state *, unsigned int, unsigned int, unsigned int *);
struct parser_node * init_declarator_list_rest(struct parser_state *, unsigned int, unsigned int, unsigned int *);
struct parser_node * init_declarator_list(struct parser_state *, unsigned int, unsigned int, unsigned int *);
struct parser_node * parameter_declaration(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * parameter_list_rest(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * parameter_list(struct parser_state *, unsigned int *);
struct parser_node * struct_declaration(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * struct_declaration_list_rest(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * struct_declaration_list(struct parser_state *, unsigned int, unsigned int *);
struct parser_node * declaration(struct parser_state *, unsigned int *);

#endif
