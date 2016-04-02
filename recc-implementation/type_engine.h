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

enum type_engine_print_type{
	TYPE_ENGINE_DEBUG,
	TYPE_ENGINE_NORMAL
};

enum type_engine_type_specifier_kind {
	TYPE_ENGINE_SIMPLE_SPECIFIER,
	TYPE_ENGINE_STRUCT_OR_UNION_SPECIFIER,
	TYPE_ENGINE_ENUM_SPECIFIER,
	TYPE_ENGINE_TYPENAME_SPECIFIER
};

enum type_engine_specifier_or_qualifier_kind {
	TYPE_ENGINE_STORAGE_CLASS_SPECIFIER,
	TYPE_ENGINE_SIMPLE_QUALIFIER,
	TYPE_ENGINE_TYPE_SPECIFIER
};

enum type_engine_specifier_or_qualifier_list_item_kind {
	TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER,
	TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START
};

enum type_engine_declarator_part_list_item_kind {
	TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART,
	TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_LIST_START
};

enum type_engine_general_type_list_item_kind {
	TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE,
	TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_LIST_START
};

enum type_engine_declarator_part_kind {
	TYPE_ENGINE_FUNCTION_DECLARATOR_PART,
	TYPE_ENGINE_POINTER_DECLARATOR_PART,
	TYPE_ENGINE_ARRAY_DECLARATOR_PART
};

enum type_engine_parameter_list_kind {
	TYPE_ENGINE_PARAMETER_LIST_VARIADIC,
	TYPE_ENGINE_PARAMETER_LIST_NON_VARIADIC
};

enum type_engine_pointer_part_kind{
	TYPE_ENGINE_QUALIFIED_POINTER,
	TYPE_ENGINE_UNQUALIFIED_POINTER
};

enum type_engine_function_part_kind {
	TYPE_ENGINE_FUNCTION_K_AND_R_C,
	TYPE_ENGINE_FUNCTION_PROTOTYPE
};

enum type_engine_array_part_kind{
	TYPE_ENGINE_CONSTANT_EXPRESSION_ARRAY,
	TYPE_ENGINE_FLEXIBLE_ARRAY
	/*  Others in the future... */
};

enum type_engine_bitfield_or_declarator_kind{
	TYPE_ENGINE_ONLY_DECLARATOR,
	TYPE_ENGINE_ONLY_BITFIELD,
	TYPE_ENGINE_BITFIELD_AND_DECLARATOR
};

#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef struct_c_lexer_token_memory_pool_H_DEFINED_
#include "../data-structures/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef struct_parser_node_memory_pool_H_DEFINED_
#include "../data-structures/struct_parser_node_memory_pool.h"
#endif
#ifndef TYPES_PARSER_enum_type_class_H_
#include "../types/parser/enum_type_class.h"
#endif
#ifndef TYPES_PARSER_struct_type_description_H_
#include "../types/parser/struct_type_description.h"
#endif
#ifndef TYPES_PARSER_struct_type_description_reference_H_
#include "../types/parser/struct_type_description_reference.h"
#endif
#ifndef TYPES_struct_memory_pool_collection_H_
#include "../types/recc-implementation/struct_memory_pool_collection.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../data-structures/unsigned_char_list.h"
#endif
#ifndef struct_c_lexer_token_list_H_DEFINED_
#include "../data-structures/struct_c_lexer_token_list.h"
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
#ifndef IO_H_
#include "io.h"
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef TYPES_PARSER_enum_scope_type_H_
#include "../types/parser/enum_scope_type.h"
#endif
#ifndef struct_parser_node_ptr_list_H_DEFINED_
#include "../data-structures/struct_parser_node_ptr_list.h"
#endif
#ifndef unsigned_int_list_H_DEFINED_
#include "../data-structures/unsigned_int_list.h"
#endif
#ifndef unsigned_char_ptr_to_struct_namespace_object_ptr_map_H_DEFINED_
#include "../data-structures/unsigned_char_ptr_to_struct_namespace_object_ptr_map.h"
#endif
#ifndef struct_c_lexer_token_to_unsigned_int_map_H_DEFINED_
#include "../data-structures/struct_c_lexer_token_to_unsigned_int_map.h"
#endif
#ifndef unsigned_int_to_unsigned_int_map_H_DEFINED_
#include "../data-structures/unsigned_int_to_unsigned_int_map.h"
#endif
#ifndef struct_parser_node_ptr_to_unsigned_int_map_H_DEFINED_
#include "../data-structures/struct_parser_node_ptr_to_unsigned_int_map.h"
#endif
#ifndef TYPES_struct_memory_pool_collection_H_
#include "../types/recc-implementation/struct_memory_pool_collection.h"
#endif
#ifndef TYPES_TYPE_ENGINE_struct_type_engine_state_H_
#include "../types/type_engine/struct_type_engine_state.h"
#endif
#ifndef TYPES_TYPE_ENGINE_enum_type_engine_operation_type_H_
#include "../types/type_engine/enum_type_engine_operation_type.h"
#endif
#ifndef TYPES_PARSER_enum_add_or_remove_H_
#include "../types/parser/enum_add_or_remove.h"
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
struct scope_level * create_empty_scope(struct scope_level *);
void destroy_empty_scope(struct scope_level *);

void create_type_engine_state(struct type_engine_state *, struct memory_pool_collection *);
void destroy_type_engine_state(struct type_engine_state *);
void undo_type_engine_operation(struct type_engine_state *, unsigned int);
void backtrack_type_engine(struct type_engine_state *, unsigned int);
unsigned int type_engine_checkpoint(struct type_engine_state *);

void increment_scope_depth(struct type_engine_state *);
void decrement_scope_depth(struct type_engine_state *);
void manage_new_scope(struct scope_level *, unsigned int, enum add_or_remove);
void remove_scope(struct scope_level *);
void add_scope(struct scope_level *);
unsigned int get_type_engine_id_for_simple_specifier(struct type_engine_state *, enum c_token_type);
unsigned int get_type_engine_id_for_identifier(struct type_engine_state *, struct c_lexer_token *);
unsigned int get_type_engine_id_for_simple_qualifier(struct type_engine_state *, enum c_token_type);
unsigned int get_type_engine_id_for_storage_class_specifier(struct type_engine_state *, enum c_token_type);
unsigned int get_type_engine_id_for_specifier_or_qualifier_list_begin(struct type_engine_state *);
unsigned int get_type_engine_id_for_declarator_part_list_begin(struct type_engine_state *);
unsigned int get_type_engine_id_for_all_specifier(struct type_engine_state *, enum type_engine_type_specifier_kind, unsigned int);
unsigned int get_type_engine_id_for_specifier_or_qualifiers(struct type_engine_state *, enum type_engine_specifier_or_qualifier_kind, unsigned int);
unsigned int get_type_engine_id_for_parameter_list(struct type_engine_state *, enum type_engine_parameter_list_kind, unsigned int);
unsigned int get_type_engine_specifier_qualifier_list_id(struct type_engine_state *, enum type_engine_specifier_or_qualifier_list_item_kind, unsigned int, unsigned int);
unsigned int get_type_engine_id_for_constant_parser_node(struct type_engine_state *, struct parser_node *);
unsigned int get_type_engine_id_for_declarator_part(struct type_engine_state * state, enum type_engine_declarator_part_kind, unsigned int);
unsigned int get_type_engine_declarator_part_list_item_id(struct type_engine_state * state, enum type_engine_declarator_part_list_item_kind, unsigned int, unsigned int);
unsigned int add_pointer_and_direct_declarators_to_list(struct type_engine_state *, unsigned int, unsigned int, unsigned int);
void add_declarators_to_list(struct type_engine_state *, unsigned int *, unsigned int, unsigned int);
unsigned int get_type_engine_id_for_pointer_part(struct type_engine_state *, enum type_engine_pointer_part_kind, unsigned int);
unsigned int get_type_engine_id_for_array_part(struct type_engine_state *, enum type_engine_array_part_kind, unsigned int);
unsigned int get_type_engine_id_for_function_part(struct type_engine_state *, enum type_engine_function_part_kind, unsigned int);
unsigned int get_type_engine_id_for_bitfield_or_declarator(struct type_engine_state *, enum type_engine_bitfield_or_declarator_kind, unsigned int);
unsigned int get_type_engine_id_for_general_type(struct type_engine_state *, unsigned int, unsigned int);
unsigned int get_type_engine_general_type_list_item_id(struct type_engine_state *, enum type_engine_general_type_list_item_kind, unsigned int, unsigned int);
unsigned int get_type_engine_id_for_general_type_list_begin(struct type_engine_state *);
void print_general_type_list(struct type_engine_state *, unsigned char *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type, unsigned int);
void print_general_type_list_item(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_pointer_part(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_function_part(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_array_part(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_type_specifier_details(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_type_specifier_or_qualifier_list_item(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_declarator_part_list_item(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_declarator_in_spiral_rule_order(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_bitfield_or_declarator(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_type_engine_state(struct type_engine_state *, struct unsigned_char_list *);
void print_type_qualifier(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_storage_class_specifier(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_specifier_or_qualifier_list(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type);
void print_declarator_in_visual_order(struct type_engine_state *, struct unsigned_char_list *, unsigned int, enum type_engine_print_type, unsigned int);

#endif
