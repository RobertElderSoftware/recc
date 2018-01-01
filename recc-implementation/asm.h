#ifndef ASM_H_
#define ASM_H_
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

#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef generated_DIR_asm_generated_includes_DOT_h
#include "../generated/asm_generated_includes.h"
#endif
#ifndef IO_H_
#include "io.h"
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif


struct scope_level * create_empty_scope(struct asm_state *, struct scope_level *, enum scope_level_type);
void destroy_empty_scope(struct scope_level *);

void create_asm_state(struct asm_state *, struct memory_pool_collection *);
void destroy_asm_state(struct asm_state *);
void undo_asm_operation(struct asm_state *, enum asm_operation_type);
void backtrack_asm(struct asm_state *, unsigned int);
unsigned int asm_checkpoint(struct asm_state *);

void syntax_model_print_char(struct unsigned_char_list *, unsigned char);
void free_string_package_pointers(struct asm_state *);
struct parser_node * associate_parser_node_with_scope_level(struct asm_state *, struct scope_level *, struct parser_node *);
void manage_new_scope(struct asm_state *, struct scope_level *, unsigned int, enum add_or_remove, enum scope_level_type);
struct scope_level * get_parser_scope_level_h(struct scope_level *, unsigned int);
struct scope_level * get_parser_scope_level(struct asm_state *);
struct simple_type_specifier_id get_asm_id_for_simple_type_specifier(struct asm_state *, enum asm_simple_type_specifier_kind);
struct simple_type_qualifier_id get_asm_id_for_simple_qualifier(struct asm_state *, enum asm_simple_type_qualifier_kind);
struct simple_storage_class_specifier_id get_asm_id_for_simple_storage_class_specifier(struct asm_state *, enum asm_simple_storage_class_specifier_kind);
struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_begin(struct asm_state *);
unsigned int get_asm_id_for_aggregate_type_specifier(struct asm_state *, enum asm_aggregate_type_specifier_kind, unsigned int);
struct parser_node_id get_asm_id_for_parser_node(struct asm_state *, struct parser_node *);
struct declarator_part_list_item_id declarator_part_list_begin(struct asm_state *);
struct declarator_part_list_item_id add_pointer_and_direct_declarators_to_list(struct asm_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id, struct declarator_part_list_item_id);
void add_declarators_to_list(struct asm_state *, struct declarator_part_list_item_id *, struct declarator_part_list_item_id, unsigned int);
struct pointer_part_id get_asm_id_for_pointer_part(struct asm_state *, enum asm_pointer_part_kind, struct specifier_or_qualifier_list_item_id);
struct scoped_tag_specifier_id get_asm_id_for_scoped_tag_specifier(struct asm_state *, struct scope_guid_id, struct unscoped_tag_specifier_id);
struct tag_declaration_namespace * create_tag_declaration_namespace(struct asm_state *);
struct identifier_declaration_namespace * create_identifier_declaration_namespace(struct asm_state *);
void add_ordered_general_type_to_identifier_declaration_namespace(struct asm_state *, struct identifier_declaration_namespace *, struct general_type_id);
void add_identifier_id_to_identifier_declaration_namespace(struct asm_state *, struct identifier_declaration_namespace *, struct general_type_id, struct identifier_id);

unsigned int add_struct_or_union_definition(struct asm_state *, struct identifier_declaration_namespace *, struct unscoped_tag_specifier_id, struct scope_level *, enum asm_unscoped_tag_specifier_kind);
unsigned int add_enum_definition(struct asm_state *, struct identifier_declaration_namespace *, struct unscoped_tag_specifier_id, struct scope_level *);
unsigned int consume_next_anonymous_tag_id_in_current_parser_scope(struct asm_state *);
unsigned int add_named_tag_declaration(struct asm_state *, struct identifier_id , struct unscoped_tag_specifier_id, struct scope_level *);


struct aggregate_type_specifier_id aggregate_type_specifier_from_simple_type_specifier_id(struct asm_state *, struct simple_type_specifier_id);
struct aggregate_type_specifier_id aggregate_type_specifier_from_typename_specifier_id(struct asm_state *, unsigned int);
struct aggregate_type_specifier_id aggregate_type_specifier_from_scoped_tag_specifier_id(struct asm_state *, struct scoped_tag_specifier_id);

struct specifier_or_qualifier_id specifier_or_qualifier_from_aggregate_type_specifier_id(struct asm_state *, struct aggregate_type_specifier_id);
struct specifier_or_qualifier_id specifier_or_qualifier_from_type_qualifier_id(struct asm_state *, struct simple_type_qualifier_id);
struct specifier_or_qualifier_id specifier_or_qualifier_from_simple_storage_class_specifier_id(struct asm_state *, struct simple_storage_class_specifier_id);

struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_begin(struct asm_state *);

struct declarator_part_id declarator_part_from_array_part_id(struct asm_state *, struct array_part_id);
struct declarator_part_id declarator_part_from_function_part_id(struct asm_state *, struct function_part_id);
struct declarator_part_id declarator_part_from_pointer_part_id(struct asm_state *, struct pointer_part_id);

struct declarator_part_list_item_id declarator_part_list_begin(struct asm_state *);

struct parameter_list_id get_variadic_parameter_list_id(struct asm_state *, struct general_type_list_item_id);
struct parameter_list_id get_non_variadic_parameter_list_id(struct asm_state *, struct general_type_list_item_id);


struct general_type_id get_asm_id_for_general_type(struct asm_state *, struct specifier_or_qualifier_list_item_id);
struct general_type_list_item_id get_general_type_list_begin(struct asm_state *);

struct unscoped_tag_specifier_id get_struct_specifier_id(struct asm_state *, struct unscoped_struct_specifier_id);
struct unscoped_tag_specifier_id get_union_specifier_id(struct asm_state *, struct unscoped_union_specifier_id);
struct unscoped_tag_specifier_id get_enum_specifier_id(struct asm_state *, struct unscoped_enum_specifier_id);

struct unscoped_struct_specifier_id get_named_struct_specifier_id(struct asm_state *, struct identifier_id);
struct unscoped_struct_specifier_id get_anonymous_struct_specifier_id(struct asm_state *, unsigned int);

struct unscoped_union_specifier_id get_named_union_specifier_id(struct asm_state *, struct identifier_id);
struct unscoped_union_specifier_id get_anonymous_union_specifier_id(struct asm_state *, unsigned int);

struct unscoped_enum_specifier_id get_named_enum_specifier_id(struct asm_state *, struct identifier_id);
struct unscoped_enum_specifier_id get_anonymous_enum_specifier_id(struct asm_state *, unsigned int);

struct function_part_id get_asm_id_for_prototype_function_part(struct asm_state *, unsigned int);
struct function_part_id get_asm_id_for_k_r_function_part(struct asm_state *, unsigned int);

struct array_part_id get_asm_id_for_constant_expression_array_part(struct asm_state *, unsigned int);
struct array_part_id get_asm_id_for_flexible_array_part(struct asm_state *, unsigned int);

enum c_token_type simple_type_qualifier_kind_to_c_token_type(enum asm_simple_type_qualifier_kind);
enum asm_simple_type_qualifier_kind c_token_type_to_simple_type_qualifier_kind(enum c_token_type);

enum c_token_type simple_type_specifier_kind_to_c_token_type(enum asm_simple_type_specifier_kind);
enum asm_simple_type_specifier_kind c_token_type_to_simple_type_specifier_kind(enum c_token_type);

enum c_token_type simple_storage_class_specifier_kind_to_c_token_type(enum asm_simple_storage_class_specifier_kind);
enum asm_simple_storage_class_specifier_kind c_token_type_to_simple_storage_class_specifier_kind(enum c_token_type);

void print_asm_state(struct asm_state *, struct unsigned_char_list *);

void print_terminal_struct_or_union_member_list_item_list_start_prev_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct struct_or_union_member_list_item_id);
void print_terminal_struct_or_union_member_list_item_list_start_item_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct struct_or_union_member_id);
void print_terminal_enum_member_list_item_list_start_prev_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct enum_member_list_item_id);
void print_terminal_enum_member_list_item_list_start_item_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct enum_member_id);
void print_terminal_initializer_list_item_list_start_prev_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct initializer_list_item_id);
void print_terminal_initializer_list_item_list_start_item_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct initializer_id);
void print_terminal_general_type_list_item_list_start_prev_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct general_type_list_item_id);
void print_terminal_general_type_list_item_list_start_item_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct general_type_id);
void print_terminal_postfix_expression_part_list_item_list_start_prev_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct postfix_expression_part_list_item_id);
void print_terminal_postfix_expression_part_list_item_list_start_item_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct postfix_expression_part_id);
void print_terminal_bitfield_no_bitfield_expression_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct expression_id);
void print_terminal_jump_statement_return_no_expression_id(struct asm_state *, unsigned int, struct unsigned_char_list *, unsigned int);
void print_terminal_jump_statement_break_id(struct asm_state *, unsigned int, struct unsigned_char_list *, unsigned int);
void print_terminal_jump_statement_continue_id(struct asm_state *, unsigned int, struct unsigned_char_list *, unsigned int);
void print_terminal_if_statement_just_if_s2(struct asm_state *, unsigned int, struct unsigned_char_list *, struct any_statement_id);
void print_terminal_labeled_statement_goto_label_expression_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct expression_id);
void print_terminal_labeled_statement_default_label_expression_id(struct asm_state *, unsigned int, struct unsigned_char_list *, struct expression_id);
void print_terminal_byte_package_byte_data(struct asm_state *, unsigned int, struct unsigned_char_list *, unsigned char);
void print_terminal_hword_package_hword_data0(struct asm_state *, unsigned int, struct unsigned_char_list *, unsigned char);
void print_terminal_hword_package_hword_data1(struct asm_state *, unsigned int, struct unsigned_char_list *, unsigned char);
void print_terminal_string_package_string_length(struct asm_state *, unsigned int, struct unsigned_char_list *, unsigned int);
void print_terminal_string_package_string_data(struct asm_state *, unsigned int, struct unsigned_char_list *, unsigned char *);
void print_terminal_named_tag_predeclaration_named_tag_type(struct asm_state *, unsigned int, struct unsigned_char_list *, enum asm_unscoped_tag_specifier_kind);
struct identifier_namespace_entry_id make_identifier_namespace_entry(struct asm_state *,enum asm_identifier_namespace_entry_kind,struct identifier_id,struct scope_guid_id,struct general_type_id,unsigned int);
struct identifier_namespace_entry_id make_identifier_namespace_entry_typedef_with_default_order(struct asm_state *, struct identifier_id, struct scope_guid_id, struct general_type_id);
struct identifier_namespace_entry_id make_identifier_namespace_entry_declaration_with_default_order(struct asm_state *, struct identifier_id, struct scope_guid_id, struct general_type_id);
struct named_tag_predeclaration_id make_named_tag_predeclaration_named_with_default_order(struct asm_state *, struct scope_guid_id, struct identifier_id, enum asm_unscoped_tag_specifier_kind);
struct named_tag_definition_id make_named_tag_definition_named_with_default_order(struct asm_state *, struct tag_definition_id, struct scope_guid_id, struct identifier_id);

#endif
