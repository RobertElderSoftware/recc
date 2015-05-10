/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#ifndef __PARSER_H__
#define __PARSER_H__

#include "io.h"
#include "core_data_types.h"
#include "lexer.h"
#include "data-structures/unsigned_int_stack.h"
#include "data-structures/struct_type_description_ptr_list.h"
#include "data-structures/char_ptr_list.h"
#include "data-structures/struct_parser_operation_stack.h"
#include "data-structures/memory_pooler.h"
#include "data-structures/binary_exponential_buffer.h"
#include "data-structures/struct_normalized_specifier_ptr_list.h"
#include "data-structures/struct_normalized_declarator_ptr_list.h"
#include "data-structures/struct_normalized_declaration_element_ptr_list.h"
#include "data-structures/struct_namespace_object_ptr_list.h"
#include "data-structures/unsigned_char_list.h"
#include "data-structures/unsigned_char_ptr_list.h"
#include "data-structures/unsigned_char_ptr_to_struct_constant_description_ptr_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

enum node_type{
	TERMINAL,
	EPSILON,
	PRIMARY_EXPRESSION,
	POSTFIX_EXPRESSION,
	POSTFIX_EXPRESSION_REST,
	UNARY_EXPRESSION,
	UNARY_OPERATOR,
	CAST_EXPRESSION,
	MULTIPLICATIVE_EXPRESSION,
	MULTIPLICATIVE_EXPRESSION_REST,
	ADDITIVE_EXPRESSION,
	ADDITIVE_EXPRESSION_REST,
	SHIFT_EXPRESSION,
	SHIFT_EXPRESSION_REST,
	RELATIONAL_EXPRESSION,
	RELATIONAL_EXPRESSION_REST,
	EQUALITY_EXPRESSION,
	EQUALITY_EXPRESSION_REST,
	AND_EXPRESSION,
	AND_EXPRESSION_REST,
	EXCLUSIVE_OR_EXPRESSION,
	EXCLUSIVE_OR_EXPRESSION_REST,
	INCLUSIVE_OR_EXPRESSION,
	INCLUSIVE_OR_EXPRESSION_REST,
	LOGICAL_AND_EXPRESSION,
	LOGICAL_AND_EXPRESSION_REST,
	LOGICAL_OR_EXPRESSION,
	LOGICAL_OR_EXPRESSION_REST,
	CONDITIONAL_EXPRESSION,
	ASSIGNMENT_EXPRESSION,
	ASSIGNMENT_OPERATOR,
	EXPRESSION_REST,
	EXPRESSION,
	CONSTANT_EXPRESSION,
	EXPRESSION_STATEMENT,
	LABELED_STATEMENT,
	SELECTION_STATEMENT,
	ITERATION_STATEMENT,
	JUMP_STATEMENT,
	STATEMENT,
	STATEMENT_LIST,
	STATEMENT_LIST_REST,
	COMPOUND_STATEMENT,
	TYPE_QUALIFIER,
	TYPE_QUALIFIER_LIST,
	TYPE_QUALIFIER_LIST_REST,
	STORAGE_CLASS_SPECIFIER,
	TYPE_SPECIFIER,
	ENUM_SPECIFIER,
	DECLARATION_SPECIFIERS,
	DECLARATION,
	DECLARATION_LIST,
	DECLARATION_LIST_REST,
	DIRECT_DECLARATOR_REST,
	DIRECT_DECLARATOR,
	DIRECT_ABSTRACT_DECLARATOR,
	DIRECT_ABSTRACT_DECLARATOR_REST,
	TYPE_NAME,
	ABSTRACT_DECLARATOR,
	DECLARATOR,
	INIT_DECLARATOR,
	INIT_DECLARATOR_LIST,
	INIT_DECLARATOR_LIST_REST,
	INITIALIZER,
	INITIALIZER_LIST,
	INITIALIZER_LIST_REST,
	IDENTIFIER_LIST,
	IDENTIFIER_LIST_REST,
	POINTER,
	STRUCT_OR_UNION,
	STRUCT_DECLARATOR,
	STRUCT_DECLARATOR_LIST,
	STRUCT_DECLARATOR_LIST_REST,
	STRUCT_DECLARATION,
	STRUCT_DECLARATION_LIST,
	STRUCT_DECLARATION_LIST_REST,
	STRUCT_OR_UNION_SPECIFIER,
	SPECIFIER_QUALIFIER_LIST,
	PARAMETER_DECLARATION,
	ARGUMENT_EXPRESSION_LIST,
	ARGUMENT_EXPRESSION_LIST_REST,
	PARAMETER_LIST,
	PARAMETER_LIST_REST,
	PARAMETER_TYPE_LIST,
	ENUMERATOR_LIST,
	ENUMERATOR_LIST_REST,
	ENUMERATOR,
	TRANSLATION_UNIT,
	TRANSLATION_UNIT_REST,
	EXTERNAL_DECLARATION,
	FUNCTION_DEFINITION
};

enum type_class{
	TYPE_CLASS_CHAR,
	TYPE_CLASS_SHORT,
	TYPE_CLASS_INT,
	TYPE_CLASS_LONG,
	TYPE_CLASS_LONG_LONG,
	TYPE_CLASS_FLOAT,
	TYPE_CLASS_DOUBLE,
	TYPE_CLASS_LONG_DOUBLE,
	TYPE_CLASS_VOID,
	TYPE_CLASS_POINTER,
	TYPE_CLASS_ARRAY,
	TYPE_CLASS_FUNCTION,
	TYPE_CLASS_ENUM,
	TYPE_CLASS_STRUCT,
	TYPE_CLASS_UNION
};

enum scope_type{
	IDENTIFIER_NAMESPACE,
	TAG_NAMESPACE,
	LABEL_NAMESPACE
};

enum add_or_remove{
	ADD,
	REMOVE
};

enum declaration_or_definition{
	FROM_DECLARATION,
	FROM_DEFINITION
};

enum normalized_declaration_type{
	NORMALIZED_DECLARATION,
	NORMALIZED_FUNCTION_DEFINITION,
	NORMALIZED_PARAMETER_DECLARATION,
	NORMALIZED_STRUCT_DECLARATION,
	NORMALIZED_K_AND_R_C_IDENTIFIER_DECLARATOR_PAIR,
	NORMALIZED_ENUMERATOR_LIST
};

enum normalized_declarator_type{
	NORMALIZED_DECLARATOR,
	NORMALIZED_ABSTRACT_DECLARATOR,
	NORMALIZED_STRUCT_DECLARATOR,
	NORMALIZED_INIT_DECLARATOR,
	NORMALIZED_ENUMERATOR
};

enum normalized_specifier_type{
	NORMALIZED_TYPE_SPECIFIER,
	NORMALIZED_TYPE_QUALIFIER,
	NORMALIZED_STORAGE_CLASS_SPECIFIER
};

enum object_location{
	LOCATION_STRUCT,
	ENUM_IDENTIFIER,
	GLOBAL,
	LOCAL,
	PARAMETER
};


enum value_type{
	LVALUE,
	RVALUE
};

struct parser_node;

struct parser_node{
	struct parser_node * next;
	struct parser_node * first_child;
	struct c_lexer_token * c_lexer_token;
	enum node_type type;
	unsigned int pad;
};

struct k_and_r_c_identifier_description{
	/*
	   TODO: review if this struct can be removed...
	   Describes the identifier and possible declarator
	   in a K&R C style function definition. Ex:
	   int foo(a,b,c) int a, int (*c)(int); {return 0;}
	   For a, this gives:
	   identifier: a, declaration_specifiers: int, declarator: a
	   For b, this gives:
	   identifier: b, declaration_specifiers: null, declarator: null
	   For c, this gives:
	   identifier: c, declaration_specifiers: int, declarator: (*c)(int)
	*/
	struct parser_node * identifier;  /*  The identifier in the identifier list of the function declaration */
	struct parser_node * declaration_specifiers;
	struct parser_node * declarator;
};

struct normalized_declaration_set{
	/*  This is essentially a wrapper for a parser_node that
	    happens to one of the common methods of declaring things.
	    The important point is that in the normalized_declaration_set,
	    the parser_node can have a declarator list with more than one
	    declarator in whatever declarator list it uses.
	    For example, int a, b, f(); has an init_declarator_list that
	    declares three different symbols.
	*/
	struct parser_node * set; /* a declaration, struct_declaration, parameter_declaration etc. */
	struct normalized_declaration_set * parent_normalized_declaration_set; /* Used to point to the main enum, struct or union declaration from an inner declaration. */
	struct struct_normalized_specifier_ptr_list * normalized_specifiers; /*  This entire list of specifiers gets applied to */
	struct struct_normalized_declarator_ptr_list * normalized_declarators; /*  each of these declarators */
	enum normalized_declaration_type type;
	unsigned int pad;
};

struct normalized_declaration_element{
	/*  In the normalized_declaration_element, the normalized_declarator can
	    ONLY HAVE ONE declarator in whatever declarator list it
	    uses to describe the declaration.
	*/
	struct normalized_declaration_set * normalized_declaration_set; /* the declaration set (declarator list) this element belongs to */
	struct normalized_declarator * normalized_declarator;
	struct struct_normalized_specifier_ptr_list * normalized_specifiers;
};

struct normalized_declarator{
	/*  
	    Used for abstracting away struct_declarator_list, init_declarator_list etc.
	*/
	struct parser_node * declarator;
	enum normalized_declarator_type type;
	unsigned int pad;
};

struct normalized_specifier{
	/*  
	    Used for abstracting away declaration_specifiers, type_specifier_list,
	    and qualifier_specifier_list
	*/
	struct parser_node * specifier;
	enum normalized_specifier_type type;
	unsigned int pad;
};

struct namespace_object{
	struct struct_namespace_object_ptr_list * children;
	struct struct_normalized_declaration_element_ptr_list elements; /* All of the declaration elements related to one named symbol */
	struct scope_level * scope_level;
	struct namespace_object * previous; /* The previous declaration element (previous param, or previous local stack varable) */
	struct namespace_object * next; /* The previous declaration element (previous param, or previous local stack varable) */
	enum object_location object_location;
	unsigned int first_load;
	int offset;
	unsigned int pad;
};

struct first_and_last_namespace_object{
	struct namespace_object * first; /* The first namespace occupying namespace object resulting from a set of declarations  */
	struct namespace_object * last; /* The last ...   */
};

struct namespace_object_change {
	struct namespace_object * target; /* The namespace object for which we're going to change one of its pointers */
	struct namespace_object * old_obj;
	struct namespace_object * new_obj;
};

struct current_function_change {
	struct scope_level * target; /* The scope namespace for which we're going to change the current function */
	struct namespace_object * old_obj;
	struct namespace_object * new_obj;
};

struct namespace_modification{
	struct normalized_declaration_element * new_element;
	struct namespace_object * object;
	struct scope_level * scope_level;
	struct struct_namespace_object_ptr_list * scope_namespace;
	enum scope_type scope_type;
	enum object_location object_location;
	int offset;
	int pad;
};

struct scope_level;

struct scope_level{
	struct scope_level * parent_scope;
	struct scope_level ** scopes;
	struct unsigned_char_ptr_list evaluate_labels; /*  For continue statements  */
	struct unsigned_char_ptr_list end_labels;      /*  For break statements  */
	struct namespace_object * current_function;
	struct namespace_object * first_local_object;
	struct struct_namespace_object_ptr_list tag_namespace;
	struct struct_namespace_object_ptr_list label_namespace;
	struct struct_namespace_object_ptr_list identifier_namespace;
	unsigned int num_sub_scopes;
	unsigned int pad;
};

struct constant_description{
	struct type_description * type_description;
	unsigned char * str;
	unsigned int * native_data; /* Byte array of the data used in the program */
	unsigned int size_in_bytes;
	unsigned int num_references;
	enum c_token_type type;
	unsigned int pad;
};

struct type_description{
	struct normalized_declaration_element * source_element; /* Needed for anonymous struct/union/enum */
	struct normalized_declarator * declarator;
	struct struct_normalized_specifier_ptr_list * specifiers;
	struct parser_node * context;
	struct scope_level * source_scope_level;
	enum value_type value_type;
	unsigned int pad;
};

struct parser_state{
	struct memory_pooler_collection * memory_pooler_collection;
	struct c_lexer_state * c_lexer_state;
	struct scope_level * top_scope;
	struct parser_node * top_node;
	struct type_description * unsigned_int_description;
	struct unsigned_char_list * buffered_output;
	struct struct_parser_operation_stack operation_stack;
	struct unsigned_char_ptr_to_struct_constant_description_ptr_map constant_map;
	unsigned char * buff;
	unsigned int tokens_position;
	unsigned int line_number;
	unsigned int current_scope_depth;
	unsigned int pad;
};

void * push_operation(struct parser_state *, enum parser_operation_type, void *);
void pop_operation(struct parser_state *);
void backtrack(struct parser_state *, unsigned int);

struct parser_node * get_identifier_from_declarator(struct parser_node *);
unsigned char * copy_asm_token(struct asm_lexer_token *);
unsigned char * copy_build_script_token(struct build_script_lexer_token *);
int parse(struct c_lexer_state *, struct parser_state *, unsigned char *);
void destroy_parser_state(struct parser_state *);
const char ** get_node_type_names(void);
struct namespace_object * get_namespace_object_from_scope_namespace_using_string(enum scope_type, struct struct_namespace_object_ptr_list *, unsigned char *);
struct constant_description * find_constant(struct parser_state *, unsigned char *);
int is_pointer(struct parser_node *);
int is_function(struct parser_node *);
int is_array(struct parser_node *);
struct parser_node * create_abstract_declarator_from_normalized_declarator(struct normalized_declarator *);
struct parser_node * get_constant_expression_from_abstract_declarator(struct parser_node *);
struct type_description * create_type_description_from_normalized_declaration_element(struct normalized_declaration_element *, struct parser_node *, struct scope_level *, enum value_type);
void destroy_type_description(struct type_description *);
void print_normalized_declaration_element(struct unsigned_char_list *, struct normalized_declaration_element *, unsigned int);
void print_normalized_declaration_declarator_and_specifiers(struct unsigned_char_list *, struct normalized_declarator *, struct struct_normalized_specifier_ptr_list *, struct parser_node *, unsigned int);

struct parser_node * copy_parser_node_children_only(struct parser_node *);
struct parser_node * copy_parser_node_tree_and_c_lexer_tokens(struct parser_node *);
struct parser_node * destroy_parser_node_tree_and_c_lexer_tokens(struct parser_node *);
struct type_description * copy_type_description(struct type_description *);
int type_description_cmp(struct type_description *, struct type_description *);
struct type_description * create_type_description_from_normalized_declarator_and_specifiers(struct normalized_declarator *, struct struct_normalized_specifier_ptr_list *, struct parser_node *, struct scope_level *, enum value_type, struct normalized_declaration_element *);
struct parser_node * convert_abstract_declarator_to_function_type(struct parser_node *);
void convert_to_untypedefed_type_description(struct type_description *);
struct type_description * create_address_type_description_from_type_description(struct type_description *);
struct type_description * create_dereferenced_pointer_type_description_from_type_description(struct type_description *);
struct type_description * create_dereferenced_array_type_description_from_type_description(struct type_description *);
int is_type_description_a_function(struct type_description *);
int is_type_description_a_function_pointer(struct type_description *);
struct type_description * get_current_function_return_type_description(struct type_description *);
struct c_lexer_token * get_identifier_token_from_normalized_declarator(struct normalized_declarator *);

struct type_description * create_type_description_from_type_name(struct parser_state *, struct parser_node *);
unsigned int is_signed(struct type_description * t);
void print_node_context(struct c_lexer_state *, struct parser_node *);
struct parser_node * get_struct_or_union_or_enum_specifier(struct struct_normalized_specifier_ptr_list *);
int is_struct(struct parser_node *);
int is_union(struct parser_node *);
unsigned int contains_struct_or_union_or_enum_definition(struct namespace_object *);
int is_enum(struct parser_node *);
unsigned int get_enum_value(struct normalized_declaration_element *);
unsigned int convert_decimal_constant(unsigned char *);

enum type_class determine_type_class(struct type_description *);
unsigned int void_type_size(enum value_type);
unsigned int arithmetic_type_size(struct type_description *, enum value_type);
unsigned int pointer_type_size(struct type_description *, enum value_type);
unsigned int enum_type_size(struct type_description *, enum value_type);
unsigned int get_ceil_modulo(unsigned int, unsigned int);
void remove_enum(struct type_description *);
void remove_specifier(struct type_description *, unsigned int, enum c_token_type);
struct type_description * add_specifier(struct type_description *, enum c_token_type);
unsigned int count_specifiers(struct type_description *, enum c_token_type);
struct type_description * create_empty_type_description(void);
unsigned int is_integral_type(struct type_description *);
unsigned int is_floating_type(struct type_description *);
unsigned int is_arithmetic_type(struct type_description *);
unsigned int is_scalar_type(struct type_description *);
struct c_lexer_token * get_struct_or_union_or_enum_tag_token(struct parser_node *);
struct parser_node * get_struct_declaration_list(struct parser_node *);
struct normalized_declaration_set * create_normalized_declaration_set_from_parser_node(struct parser_node *, struct normalized_declaration_set *);
struct struct_normalized_declaration_element_ptr_list * create_normalized_declaration_element_list(struct normalized_declaration_set *);
void destroy_normalized_declaration_element_list(struct struct_normalized_declaration_element_ptr_list*);
struct parser_node * get_enumerator_list(struct parser_node *);
struct namespace_object * get_namespace_object_from_closest_namespace(unsigned char *, enum scope_type, struct scope_level *, unsigned int);
unsigned char * make_up_identifier(struct normalized_declaration_element *);
struct normalized_declarator * make_array_brackets(void);
void print_error_with_types(struct c_lexer_state *, struct type_description *, struct type_description *, struct parser_node *, const char *);
void print_error_with_type(struct c_lexer_state *, struct type_description *, struct parser_node *, const char *);

#endif
