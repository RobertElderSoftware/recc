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

#ifndef generated_DIR_parser_generated_includes_DOT_h
#include "../generated/parser_generated_includes.h"
#endif
#ifndef IO_H_
#include "io.h"
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
#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef STDARG_H_DEFINED_
#include <stdarg.h>
#endif
#ifndef ASM_H_
#include "asm.h"
#endif

void * push_operation(struct parser_state *, enum parser_operation_type, void *);
void pop_operation(struct parser_state *);
void backtrack(struct parser_state *, struct parser_checkpoint);

struct parser_node * get_identifier_from_declarator(struct parser_node *);
unsigned char * copy_l2_token(struct l2_lexer_token *);
void create_parser_state(struct parser_state *, struct memory_pool_collection *, struct c_lexer_state *, struct unsigned_char_list *, unsigned char *, struct asm_state *);
unsigned int parse(struct parser_state *);
void destroy_parser_state(struct parser_state *);
const char ** get_node_type_names(void);
struct constant_description * find_constant(struct parser_state *, unsigned char *);
struct parser_node * get_constant_expression_from_abstract_declarator(struct parser_node *);
void destroy_type_description(struct memory_pool_collection *, struct type_description *);

struct parser_node * copy_parser_node_children_only(struct memory_pool_collection *, struct parser_node *);
struct parser_node * copy_parser_node_tree_and_c_lexer_tokens(struct memory_pool_collection *, struct parser_node *);
struct parser_node * destroy_parser_node_tree_and_c_lexer_tokens(struct memory_pool_collection *, struct parser_node *);
struct type_description * copy_type_description(struct memory_pool_collection *, struct type_description *);
unsigned int type_description_cmp(struct memory_pool_collection *, struct type_description *, struct type_description *);
struct parser_node * convert_abstract_declarator_to_function_type(struct memory_pool_collection *, struct parser_node *);
void convert_to_untypedefed_type_description(struct memory_pool_collection *, struct type_description *);
struct type_description * create_address_type_description_from_type_description(struct memory_pool_collection *, struct type_description *);
struct type_description * create_dereferenced_pointer_type_description_from_type_description(struct memory_pool_collection *, struct type_description *);
struct type_description * create_dereferenced_array_type_description_from_type_description(struct memory_pool_collection *, struct type_description *);
unsigned int is_type_description_a_function(struct memory_pool_collection *, struct type_description *);
unsigned int is_type_description_a_function_pointer(struct memory_pool_collection *, struct type_description *);
struct type_description * get_current_function_return_type_description(struct memory_pool_collection *, struct type_description *);

struct type_description * create_type_description_from_type_name(struct memory_pool_collection *, struct parser_state *, struct parser_node *);
unsigned int is_signed(struct type_description * t);
void print_node_context(struct c_lexer_state *, struct parser_node *);
unsigned int is_struct(struct parser_node *);
unsigned int is_union(struct parser_node *);
unsigned int is_enum(struct parser_node *);

enum type_class determine_type_class(struct memory_pool_collection *, struct type_description *);
unsigned int void_type_size(struct memory_pool_collection *, enum value_type);
unsigned int arithmetic_type_size(struct memory_pool_collection *, struct type_description *, enum value_type);
unsigned int pointer_type_size(struct memory_pool_collection *, struct type_description *, enum value_type);
unsigned int enum_type_size(struct memory_pool_collection *, struct type_description *, enum value_type);
unsigned int get_ceil_modulo(unsigned int, unsigned int);
void remove_enum(struct memory_pool_collection *, struct type_description *);
void remove_specifier(struct memory_pool_collection *, struct type_description *, unsigned int, enum c_token_type);
struct type_description * add_specifier(struct memory_pool_collection *, struct type_description *, enum c_token_type);
unsigned int count_specifiers(struct type_description *, enum c_token_type);
struct type_description * create_empty_type_description(struct memory_pool_collection *);
unsigned int is_integral_type(struct memory_pool_collection *, struct type_description *);
unsigned int is_floating_type(struct memory_pool_collection *, struct type_description *);
unsigned int is_arithmetic_type(struct memory_pool_collection *, struct type_description *);
unsigned int is_scalar_type(struct memory_pool_collection *, struct type_description *);
struct c_lexer_token * get_struct_or_union_or_enum_tag_token(struct parser_node *);
struct parser_node * get_struct_declaration_list(struct parser_node *);
struct parser_node * get_enumerator_list(struct parser_node *);
void print_error_with_types(struct c_lexer_state *, struct type_description *, struct type_description *, struct parser_node *, const char *);
void print_error_with_type(struct c_lexer_state *, struct type_description *, struct parser_node *, const char *);
struct type_description * get_type_description_from_suffix(struct memory_pool_collection *, unsigned char *);
struct parser_node * get_nth_parameter_declaration_from_parameter_type_list(struct parser_node *, unsigned int);
struct parser_node * get_parameter_type_list_from_abstract_declarator(struct parser_node *);
unsigned int is_function_variadic(struct parser_node *);

struct parser_node * specifier_qualifier_list(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct specifier_or_qualifier_list_item_id *);
struct parser_node * type_qualifier_list_rest(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct specifier_or_qualifier_list_item_id *);
struct parser_node * type_qualifier_list(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct specifier_or_qualifier_list_item_id *);
struct parser_node * declaration_specifiers(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct specifier_or_qualifier_list_item_id *);
struct parser_node * simple_storage_class_specifier(struct parser_state *, struct simple_storage_class_specifier_id *);
struct parser_node * type_qualifier(struct parser_state *, struct simple_type_qualifier_id *);
struct parser_node * type_specifier(struct parser_state *, struct aggregate_type_specifier_id *);

struct parser_node * get_typedef_identifier_node(struct parser_state *, struct scope_guid_id *, struct identifier_id *);
struct parser_node * get_non_typedef_identifier_node(struct parser_state *, struct scope_guid_id *, struct identifier_id *);
struct parser_node * get_any_identifier_node(struct parser_state *, struct identifier_id *);

struct parser_node * declarator(struct parser_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id *, struct identifier_from_declarator *, struct any_statement_list_item_id *, unsigned int, unsigned int*);
struct parser_node * direct_declarator(struct parser_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id *, struct identifier_from_declarator *, struct any_statement_list_item_id *, unsigned int, unsigned int*);
struct parser_node * direct_declarator_rest(struct parser_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id *, struct identifier_from_declarator *, struct any_statement_list_item_id *, unsigned int, unsigned int*);
struct parser_node * abstract_declarator(struct parser_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id *);
struct parser_node * direct_abstract_declarator_rest(struct parser_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id *);
struct parser_node * direct_abstract_declarator(struct parser_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id *);
struct parser_node * pointer(struct parser_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id *);
struct parser_node * init_declarator(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct any_statement_list_item_id, struct any_statement_list_item_id *);

struct parser_node * struct_declarator(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct struct_or_union_member_id *);
struct parser_node * struct_declarator_list_rest(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct struct_or_union_member_list_item_id, struct struct_or_union_member_list_item_id *);
struct parser_node * struct_declarator_list(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct struct_or_union_member_list_item_id, struct struct_or_union_member_list_item_id *);
struct parser_node * init_declarator_list_rest(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * init_declarator_list(struct parser_state *, struct specifier_or_qualifier_list_item_id, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * parameter_declaration(struct parser_state *, struct general_type_id *, struct simple_declaration_id *, unsigned int, unsigned int*, unsigned int *);
struct parser_node * parameter_list_rest(struct parser_state *, struct general_type_list_item_id, struct general_type_list_item_id *, struct any_statement_list_item_id, struct any_statement_list_item_id *, unsigned int, unsigned int*);
struct parser_node * parameter_list(struct parser_state *, struct general_type_list_item_id *, struct any_statement_list_item_id *, unsigned int, unsigned int*);
struct parser_node * struct_declaration(struct parser_state *, struct struct_or_union_member_list_item_id, struct struct_or_union_member_list_item_id *);
struct parser_node * struct_declaration_list_rest(struct parser_state *, struct struct_or_union_member_list_item_id, struct struct_or_union_member_list_item_id *);
struct parser_node * struct_declaration_list(struct parser_state *, struct struct_or_union_member_list_item_id, struct struct_or_union_member_list_item_id *);
struct parser_node * declaration(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * struct_or_union(struct parser_state *, enum asm_unscoped_tag_specifier_kind *);
struct parser_node * struct_or_union_specifier(struct parser_state *, struct scoped_tag_specifier_id *);
struct parser_node * struct_or_union(struct parser_state *, enum asm_unscoped_tag_specifier_kind *);
struct parser_node * declaration_list_rest(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id*);
struct parser_node * primary_expression(struct parser_state *, struct primary_expression_id *);
struct parser_node * declaration_list(struct parser_state *, struct any_statement_list_item_id , struct any_statement_list_item_id*);
struct parser_node * argument_expression_list_rest(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * argument_expression_list(struct parser_state *, struct any_statement_list_item_id *);
struct parser_node * postfix_expression_rest(struct parser_state *, struct postfix_expression_part_list_item_id, struct postfix_expression_part_list_item_id *);
struct parser_node * postfix_expression(struct parser_state *, struct postfix_expression_id *);
struct parser_node * unary_expression(struct parser_state *, struct unary_expression_id *);
struct parser_node * cast_expression(struct parser_state *, struct cast_expression_id *);
struct parser_node * multiplicative_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * multiplicative_expression(struct parser_state *, struct expression_id *);
struct parser_node * additive_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * additive_expression(struct parser_state *, struct expression_id *);
struct parser_node * shift_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * shift_expression(struct parser_state *, struct expression_id *);
struct parser_node * relational_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * relational_expression(struct parser_state *, struct expression_id *);
struct parser_node * equality_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * equality_expression(struct parser_state *, struct expression_id *);
struct parser_node * and_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * and_expression(struct parser_state *, struct expression_id *);
struct parser_node * exclusive_or_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * exclusive_or_expression(struct parser_state *, struct expression_id *);
struct parser_node * inclusive_or_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * inclusive_or_expression(struct parser_state *, struct expression_id *);
struct parser_node * logical_and_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * logical_and_expression(struct parser_state *, struct expression_id *);
struct parser_node * logical_or_expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * logical_or_expression(struct parser_state *, struct expression_id *);
struct parser_node * compound_statement(struct parser_state *, struct function_definition_helper *, unsigned int, struct any_statement_list_item_id *);
struct parser_node * conditional_expression(struct parser_state *, struct expression_id *);
struct parser_node * expression(struct parser_state *, struct expression_id *);
struct parser_node * expression_rest(struct parser_state *, struct expression_id, struct expression_id *);
struct parser_node * expression_statement(struct parser_state *, struct expression_id *);
struct parser_node * assignment_expression(struct parser_state *, struct expression_id *);
struct parser_node * enumerator(struct parser_state *, struct enum_member_id *, struct scoped_tag_specifier_id);
struct parser_node * enumerator_list_rest(struct parser_state *, struct enum_member_list_item_id, struct enum_member_list_item_id *, struct scoped_tag_specifier_id);
struct parser_node * enumerator_list(struct parser_state *, struct enum_member_list_item_id, struct enum_member_list_item_id *, struct scoped_tag_specifier_id);
struct parser_node * enum_specifier(struct parser_state *, struct scoped_tag_specifier_id *);
struct parser_node * initializer_list_rest(struct parser_state *, struct initializer_list_item_id, struct initializer_list_item_id *);
struct parser_node * initializer_list(struct parser_state *, struct initializer_id *);
struct parser_node * initializer(struct parser_state *, struct initializer_id *);
struct parser_node * parameter_type_list(struct parser_state *, struct parameter_list_id *, struct any_statement_list_item_id *, unsigned int, unsigned int*);
struct parser_node * labeled_statement(struct parser_state *, struct labeled_statement_id *);
struct parser_node * selection_statement(struct parser_state *, struct code_statement_id *);
struct parser_node * iteration_statement(struct parser_state *, struct code_statement_id *);
struct parser_node * jump_statement(struct parser_state *, struct jump_statement_id *);
struct parser_node * type_name(struct parser_state *, struct general_type_id *);
struct parser_node * p_accept(enum c_token_type, struct parser_state *);
struct parser_node * expect(enum c_token_type, struct parser_state *);
struct parser_node * make_epsilon(struct parser_state *);
struct parser_node * unary_operator(struct parser_state *);
struct parser_node * constant_expression(struct parser_state *, struct expression_id *);
struct parser_node * statement(struct parser_state *, unsigned int, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * statement_list_rest(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * statement_list(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * identifier_list_rest(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * identifier_list(struct parser_state *, struct any_statement_list_item_id *, unsigned int, unsigned int *);
struct parser_node * function_definition(struct parser_state *, struct function_definition_id *);
struct parser_node * external_declaration(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * translation_unit_rest(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id *);
struct parser_node * translation_unit(struct parser_state *, struct any_statement_list_item_id *);
struct parser_node * string_literal_list_rest(struct parser_state *, struct unsigned_char_list *);
struct parser_node * string_literal_list(struct parser_state *, struct primary_expression_id *);



unsigned int get_scope_of_closest_tag_declaration(struct parser_state *, struct scope_guid_id *, struct scope_guid_id, struct identifier_id);
unsigned int get_scope_of_closest_identifier_declaration(struct parser_state *, struct scope_guid_id *, struct scope_guid_id, struct identifier_id, enum asm_identifier_namespace_entry_kind, enum asm_identifier_namespace_entry_kind);

void print_parser_error(struct unsigned_char_list *, struct parser_state *, struct parser_error);
void print_parser_error_description(struct unsigned_char_list *, struct parser_state *, enum parser_error_type);
void add_parser_error(struct parser_state *, enum parser_error_type);
void increment_scope_depth(struct parser_state *, enum scope_level_type);
void decrement_scope_depth(struct parser_state *);
struct any_statement_list_item_id add_scoped_any_statement(struct parser_state *, struct any_statement_list_item_id, struct any_statement_list_item_id);
struct scope_guid_id get_current_scope_guid(struct parser_state *);

unsigned int specifier_or_qualifier_list_item_id_contains_typedef(struct parser_state *, struct specifier_or_qualifier_list_item_id);

void add_named_struct_or_union_definition(struct parser_state *, struct struct_or_union_member_list_item_id, struct scope_guid_id, struct identifier_id, enum asm_unscoped_tag_specifier_kind );

void add_named_enum_definition(struct parser_state *, struct enum_member_list_item_id, struct scope_guid_id, struct identifier_id);

void add_anonymous_struct_or_union_definition(struct parser_state *, struct struct_or_union_member_list_item_id, struct scope_guid_id, struct anonymous_entry_id, enum asm_unscoped_tag_specifier_kind);
void add_anonymous_enum_definition(struct parser_state *, struct enum_member_list_item_id, struct scope_guid_id, struct anonymous_entry_id);

#endif
