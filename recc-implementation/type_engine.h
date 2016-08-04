#ifndef TYPE_ENGINE_H_
#define TYPE_ENGINE_H_
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
#ifndef generated_DIR_type_engine_generated_includes_DOT_h
#include "../generated/type_engine_generated_includes.h"
#endif
#ifndef IO_H_
#include "io.h"
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif


struct parser_node * destroy_parser_node_tree_and_c_lexer_tokens(struct memory_pool_collection *, struct parser_node *);
struct parser_node * remove_one_pointer_level(struct memory_pool_collection *, struct parser_node *);
struct parser_node * copy_parser_node_children_only(struct memory_pool_collection *, struct parser_node *);
struct parser_node * copy_parser_node_tree_and_c_lexer_tokens(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_abstract_declarator_to_pointer_type(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_abstract_declarator_to_array_type(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_abstract_declarator_to_function_type(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_abstract_declarator_to_address_type_h(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_abstract_declarator_to_address_type(struct memory_pool_collection *, struct parser_node *);
struct parser_node * create_pointer_node(struct memory_pool_collection *);
struct parser_node * add_level_to_pointer(struct memory_pool_collection *, struct parser_node *);
unsigned int is_pointer(struct memory_pool_collection *, struct parser_node *);
unsigned int is_function(struct memory_pool_collection *, struct parser_node *);
struct parser_node * get_nth_parameter_declaration_from_parameter_type_list(struct parser_node *, unsigned int);
struct parser_node * get_parameter_type_list_from_abstract_declarator(struct parser_node *);
struct parser_node * get_constant_expression_from_abstract_declarator(struct parser_node *);
unsigned int is_parameter_type_list_variadic(struct parser_node *);
unsigned int is_function_variadic(struct parser_node *);
unsigned int is_function_k_and_r_c_style(struct parser_node *);
unsigned int is_array(struct memory_pool_collection *, struct parser_node *);
void add_normalized_specifier_to_list(struct memory_pool_collection *, struct parser_node *, struct struct_normalized_specifier_ptr_list *);
struct struct_normalized_specifier_ptr_list * get_normalized_specifier_list(struct memory_pool_collection *, struct parser_node *);
void add_normalized_declarator_to_list(struct memory_pool_collection *, struct parser_node *, struct struct_normalized_declarator_ptr_list *);
struct struct_normalized_declarator_ptr_list * get_normalized_declarator_list(struct memory_pool_collection *, struct parser_node *);
struct parser_node * add_outer_direct_abstract_declarator_to_inner_as_rest(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
void add_pointer_to_end(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
struct parser_node * insert_abstract_declarator(struct memory_pool_collection *, struct parser_node *, struct parser_node *);
void convert_to_untypedefed_type_description(struct memory_pool_collection *, struct type_description_reference);
struct namespace_object * get_namespace_object_from_closest_namespace(unsigned char *, enum scope_type, struct scope_level *, unsigned int, struct memory_pool_collection *);
struct type_description_reference create_type_description_from_normalized_declaration_element(struct memory_pool_collection *, struct normalized_declaration_element *, struct parser_node *, struct scope_level *, enum value_type);
void destroy_type_description(struct memory_pool_collection *, struct type_description_reference);
unsigned int count_specifiers(struct type_description_reference, enum c_token_type);
void remove_specifier(struct memory_pool_collection *, struct type_description_reference, unsigned int, enum c_token_type);
struct parser_node * create_abstract_declarator_from_declarator(struct memory_pool_collection *, struct parser_node *);
struct parser_node * create_abstract_declarator_from_normalized_declarator(struct memory_pool_collection *, struct normalized_declarator *);
struct namespace_object * get_namespace_object_from_scope_namespace_using_string(struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *, unsigned char *);
struct namespace_object * get_namespace_object_from_scope_namespace_using_string(struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *, unsigned char *);
struct c_lexer_token * get_identifier_token_from_declarator(struct parser_node *);
struct parser_node * convert_declarator_to_abstract_declarator(struct memory_pool_collection *, struct parser_node *);
struct parser_node * convert_declarator_to_abstract_declarator_h(struct memory_pool_collection *, struct parser_node *);
struct c_lexer_token * get_identifier_token_from_normalized_declarator(struct normalized_declarator *);
struct parser_node * get_function_definition(struct normalized_declaration_set *);
unsigned int is_bitfield(struct parser_node *);
unsigned int is_struct(struct parser_node *);
unsigned int is_union(struct parser_node *);
unsigned int is_enum(struct parser_node *);
unsigned int is_anonymous_struct_or_union_or_enum(struct parser_node *);
unsigned int contains_struct_or_union_or_enum_definition(struct namespace_object *);
unsigned int contains_function_definition(struct namespace_object *);
unsigned int is_struct_or_union_or_enum_definition(struct parser_node *);
struct parser_node * get_enumerator_list(struct parser_node *);
struct parser_node * get_struct_declaration_list(struct parser_node *);
struct parser_node * get_function_definition_from_namespace_object(struct namespace_object *);
struct c_lexer_token * get_struct_or_union_or_enum_tag_token(struct parser_node *);
struct parser_node * get_struct_or_union_or_enum_specifier(struct struct_normalized_specifier_ptr_list *);
struct normalized_declaration_set * create_normalized_declaration_set_from_parser_node(struct memory_pool_collection *, struct parser_node *, struct normalized_declaration_set *);
struct type_description_reference create_type_description_from_normalized_declarator_and_specifiers(struct memory_pool_collection *, struct normalized_declarator *, struct struct_normalized_specifier_ptr_list *, struct parser_node *, struct scope_level *, enum value_type, struct normalized_declaration_element *);
void destroy_normalized_declaration_element_list(struct memory_pool_collection *, struct struct_normalized_declaration_element_ptr_list *);
struct struct_normalized_declaration_element_ptr_list * create_normalized_declaration_element_list(struct normalized_declaration_set *);
void validate_specifier_token_type(enum c_token_type);
int normalized_specifier_ptr_cmp(struct normalized_specifier *, struct normalized_specifier *);
struct normalized_declarator * make_array_brackets(struct memory_pool_collection *);
unsigned char * get_specifier_string(enum c_token_type);
struct scope_level * create_empty_scope(struct type_engine_state *, struct scope_level *, enum scope_level_type);
void destroy_empty_scope(struct scope_level *);

void create_type_engine_state(struct type_engine_state *, struct memory_pool_collection *);
void destroy_type_engine_state(struct type_engine_state *);
void undo_type_engine_operation(struct type_engine_state *, enum type_engine_operation_type);
void backtrack_type_engine(struct type_engine_state *, unsigned int);
unsigned int type_engine_checkpoint(struct type_engine_state *);

struct parser_node * associate_parser_node_with_scope_level(struct type_engine_state *, struct scope_level *, struct parser_node *);
struct scope_level * increment_scope_depth(struct type_engine_state *, enum scope_level_type);
void decrement_scope_depth(struct type_engine_state *);
void manage_new_scope(struct type_engine_state *, struct scope_level *, unsigned int, enum add_or_remove, enum scope_level_type);
struct scope_level * get_parser_scope_level_h(struct scope_level *, unsigned int);
struct scope_level * get_parser_scope_level(struct type_engine_state *);
struct simple_specifier_id get_type_engine_id_for_simple_specifier(struct type_engine_state *, enum c_token_type);
struct identifier_id get_type_engine_id_for_identifier(struct type_engine_state *, struct c_lexer_token *);
struct type_qualifier_id get_type_engine_id_for_simple_qualifier(struct type_engine_state *, enum c_token_type);
struct storage_class_specifier_id get_type_engine_id_for_storage_class_specifier(struct type_engine_state *, enum c_token_type);
struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_begin(struct type_engine_state *);
unsigned int get_type_engine_id_for_aggregate_specifier(struct type_engine_state *, enum type_engine_aggregate_specifier_kind, unsigned int);
struct specifier_or_qualifier_list_item_id make_specifier_or_qualifier_list_item(struct type_engine_state *, struct specifier_or_qualifier_list_item_id, struct specifier_or_qualifier_id);
struct parser_node_id get_type_engine_id_for_parser_node(struct type_engine_state *, struct parser_node *);
struct declarator_part_list_item_id make_declarator_part_list_item(struct type_engine_state *, struct declarator_part_list_item_id, struct declarator_part_id);
struct declarator_part_list_item_id declarator_part_list_begin(struct type_engine_state *);
struct declarator_part_list_item_id add_pointer_and_direct_declarators_to_list(struct type_engine_state *, struct declarator_part_list_item_id, struct declarator_part_list_item_id, struct declarator_part_list_item_id);
void add_declarators_to_list(struct type_engine_state *, struct declarator_part_list_item_id *, struct declarator_part_list_item_id, unsigned int);
struct pointer_part_id get_type_engine_id_for_pointer_part(struct type_engine_state *, enum type_engine_pointer_part_kind, struct specifier_or_qualifier_list_item_id);
struct scoped_struct_or_union_specifier_id get_type_engine_id_for_scoped_struct_or_union_specifier(struct type_engine_state *, struct scope_level_id, struct struct_or_union_specifier_id);
struct scope_level_id get_type_engine_id_for_scope_level(struct type_engine_state *, struct scope_level *);
struct declaration_namespace * create_declaration_namespace(struct type_engine_state *);
void add_ordered_general_type_to_declaration_namespace(struct type_engine_state *, struct declaration_namespace *, struct general_type_id);
void add_identifier_id_to_declaration_namespace(struct type_engine_state *, struct declaration_namespace *, struct general_type_id, struct identifier_id);
unsigned int add_struct_or_union_definition(struct type_engine_state *, struct declaration_namespace *, struct struct_or_union_specifier_id, struct scope_level *);
unsigned int consume_next_anonymous_tag_id_in_current_parser_scope(struct type_engine_state *);
unsigned int add_named_struct_or_union_declaration(struct type_engine_state *, struct identifier_id , struct struct_or_union_specifier_id, struct scope_level *);
struct scope_level * get_scope_of_closest_struct_or_union_tag_declaration(struct type_engine_state *, struct scope_level *, struct struct_or_union_specifier_id);

void print_general_type_list(struct type_engine_state *, unsigned char *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type, unsigned int);
void print_general_type_list_item(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_pointer_part(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_function_part(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_array_part(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_aggregate_specifiers(struct type_engine_state *, struct unsigned_char_list *, struct aggregate_specifier_id, enum type_engine_print_type);
void print_type_specifier_or_qualifier(struct type_engine_state *, struct unsigned_char_list *, struct specifier_or_qualifier_id, enum type_engine_print_type);
void print_declarator_part(struct type_engine_state *, struct unsigned_char_list *, struct declarator_part_id, enum type_engine_print_type);
void print_declarator_in_spiral_rule_order(struct type_engine_state *, struct unsigned_char_list *, struct declarator_part_list_item_id, enum type_engine_print_type);
void print_bitfield_or_declarator(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_type_engine_state(struct type_engine_state *, struct unsigned_char_list *);
void print_type_qualifier(struct type_engine_state *, struct unsigned_char_list *, struct type_qualifier_id, enum type_engine_print_type);
void print_storage_class_specifier(struct type_engine_state *, struct unsigned_char_list *, struct storage_class_specifier_id, enum type_engine_print_type);
void print_specifier_or_qualifier_list(struct type_engine_state *, struct unsigned_char_list *, struct specifier_or_qualifier_list_item_id, enum type_engine_print_type);
void print_declarator_in_visual_order(struct type_engine_state *, struct unsigned_char_list *, struct declarator_part_list_item_id, enum type_engine_print_type, unsigned int);
void print_struct_specifier(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_union_specifier(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_struct_or_union_specifier(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_scoped_struct_or_union_specifier(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_identifier(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);

struct aggregate_specifier_id aggregate_specifier_from_simple_specifier_id(struct type_engine_state *, struct simple_specifier_id);
struct aggregate_specifier_id aggregate_specifier_from_typename_specifier_id(struct type_engine_state *, unsigned int);
struct aggregate_specifier_id aggregate_specifier_from_enum_specifier_id(struct type_engine_state *, unsigned int);
struct aggregate_specifier_id aggregate_specifier_from_scoped_struct_or_union_specifier_id(struct type_engine_state *, struct scoped_struct_or_union_specifier_id);

struct specifier_or_qualifier_id specifier_or_qualifier_from_aggregate_specifier_id(struct type_engine_state *, struct aggregate_specifier_id);
struct specifier_or_qualifier_id specifier_or_qualifier_from_type_qualifier_id(struct type_engine_state *, struct type_qualifier_id);
struct specifier_or_qualifier_id specifier_or_qualifier_from_storage_class_specifier_id(struct type_engine_state *, struct storage_class_specifier_id);

struct specifier_or_qualifier_list_item_id make_specifier_or_qualifier_list_item(struct type_engine_state *, struct specifier_or_qualifier_list_item_id, struct specifier_or_qualifier_id);
struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_begin(struct type_engine_state *);

struct declarator_part_id declarator_part_from_array_part_id(struct type_engine_state *, struct array_part_id);
struct declarator_part_id declarator_part_from_function_part_id(struct type_engine_state *, struct function_part_id);
struct declarator_part_id declarator_part_from_pointer_part_id(struct type_engine_state *, struct pointer_part_id);

struct declarator_part_list_item_id make_declarator_part_list_item(struct type_engine_state *, struct declarator_part_list_item_id, struct declarator_part_id);
struct declarator_part_list_item_id declarator_part_list_begin(struct type_engine_state *);

struct parameter_list_id get_variadic_parameter_list_id(struct type_engine_state *, struct general_type_list_item_id);
struct parameter_list_id get_non_variadic_parameter_list_id(struct type_engine_state *, struct general_type_list_item_id);

struct bitfield_or_declarator_id bitfield_or_declarator_from_only_bitfield(struct type_engine_state *, unsigned int);
struct bitfield_or_declarator_id bitfield_or_declarator_from_only_declarator(struct type_engine_state *, struct declarator_part_list_item_id);
struct bitfield_or_declarator_id bitfield_or_declarator_from_bitfield_and_declarator(struct type_engine_state *, unsigned int);

struct general_type_id get_type_engine_id_for_general_type(struct type_engine_state *, struct specifier_or_qualifier_list_item_id, struct bitfield_or_declarator_id);
struct general_type_list_item_id make_general_type_list_item(struct type_engine_state *, struct general_type_list_item_id, struct general_type_id);
struct general_type_list_item_id get_general_type_list_begin(struct type_engine_state *);

struct struct_or_union_specifier_id get_struct_specifier_id(struct type_engine_state *, struct struct_specifier_id);
struct struct_or_union_specifier_id get_union_specifier_id(struct type_engine_state *, struct union_specifier_id);

struct struct_specifier_id get_named_struct_specifier_id(struct type_engine_state *, struct identifier_id);
struct struct_specifier_id get_anonymous_struct_specifier_id(struct type_engine_state *, unsigned int);

struct union_specifier_id get_named_union_specifier_id(struct type_engine_state *, struct identifier_id);
struct union_specifier_id get_anonymous_union_specifier_id(struct type_engine_state *, unsigned int);

struct function_part_id get_type_engine_id_for_prototype_function_part(struct type_engine_state *, unsigned int);
struct function_part_id get_type_engine_id_for_k_r_function_part(struct type_engine_state *, unsigned int);

struct array_part_id get_type_engine_id_for_constant_expression_array_part(struct type_engine_state *, unsigned int);
struct array_part_id get_type_engine_id_for_flexible_array_part(struct type_engine_state *, unsigned int);

#endif
