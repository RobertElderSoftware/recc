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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../recc-implementation/type_engine.h"
#include "../recc-implementation/memory_pool_collection.h"
#include "../generated/struct_c_lexer_state_ptr_list.h"
#include "../generated/struct_parser_state_ptr_list.h"
#include "../generated/struct_unsigned_char_list_ptr_list.h"
#include "../recc-implementation/lexer.h"
#include "../recc-implementation/parser.h"

#define COMPARE_TYPES 0x1
#define ERROR_CHECK 0x2

enum ID_TYPE {
	TEST_1_PRIMARY_EXPRESSION,
	TEST_1_IDENTIFIER,
	TEST_1_TYPE_QUALIFIER,
	TEST_1_TYPE_SPECIFIER,
	TEST_1_STORAGE_CLASS_SPECIFIER,
	TEST_1_DECLARATION_SPECIFIERS,
	TEST_1_SPECIFIER_QUALIFIER_LIST,
	TEST_1_TYPE_QUALIFIER_LIST,
	TEST_1_ABSTRACT_DECLARATOR,
	TEST_1_DECLARATOR,
	TEST_1_STRUCT_OR_UNION_SPECIFIER,
	TEST_1_STRUCT_DECLARATOR,
	TEST_1_INIT_DECLARATOR,
	TEST_1_PARAMETER_LIST,
	TEST_1_STRUCT_DECLARATION,
	TEST_1_STRUCT_DECLARATION_LIST,
	TEST_1_DECLARATION,
	TEST_1_DECLARATION_LIST,
	TEST_1_ARGUMENT_EXPRESSION_LIST,
	TEST_1_ARGUMENT_EXPRESSION_LIST_REST,
	TEST_1_POSTFIX_EXPRESSION_REST,
	TEST_1_POSTFIX_EXPRESSION,
	TEST_1_UNARY_EXPRESSION,
	TEST_1_CAST_EXPRESSION,
	TEST_1_MULTIPLICATIVE_EXPRESSION_REST,
	TEST_1_MULTIPLICATIVE_EXPRESSION,
	TEST_1_ADDITIVE_EXPRESSION_REST,
	TEST_1_ADDITIVE_EXPRESSION,
	TEST_1_SHIFT_EXPRESSION_REST,
	TEST_1_SHIFT_EXPRESSION,
	TEST_1_RELATIONAL_EXPRESSION_REST,
	TEST_1_RELATIONAL_EXPRESSION,
	TEST_1_EQUALITY_EXPRESSION_REST,
	TEST_1_EQUALITY_EXPRESSION,
	TEST_1_AND_EXPRESSION_REST,
	TEST_1_AND_EXPRESSION,
	TEST_1_EXCLUSIVE_OR_EXPRESSION_REST,
	TEST_1_EXCLUSIVE_OR_EXPRESSION,
	TEST_1_INCLUSIVE_OR_EXPRESSION_REST,
	TEST_1_INCLUSIVE_OR_EXPRESSION,
	TEST_1_LOGICAL_AND_EXPRESSION_REST,
	TEST_1_LOGICAL_AND_EXPRESSION,
	TEST_1_LOGICAL_OR_EXPRESSION_REST,
	TEST_1_LOGICAL_OR_EXPRESSION,
	TEST_1_COMPOUND_STATEMENT,
	TEST_1_CONDITIONAL_EXPRESSION,
	TEST_1_EXPRESSION,
	TEST_1_EXPRESSION_REST,
	TEST_1_ENUMERATOR,
	TEST_1_ENUMERATOR_LIST_REST,
	TEST_1_ENUMERATOR_LIST,
	TEST_1_ENUM_SPECIFIER,
	TEST_1_DIRECT_DECLARATOR_REST,
	TEST_1_DIRECT_DECLARATOR,
	TEST_1_INITIALIZER_LIST_REST,
	TEST_1_INITIALIZER
};

void test_1(struct memory_pool_collection *);
void test_type_id(const char * str, struct memory_pool_collection *, enum ID_TYPE);
void test_parser_error(const char * str, struct memory_pool_collection *, enum ID_TYPE, enum parser_error_type);
void grammar_rule_test(const char * str, struct memory_pool_collection *, enum ID_TYPE, unsigned int error_check, enum parser_error_type);
struct parser_state * setup_parser(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);

struct parser_state * setup_parser(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * parser_state = malloc(sizeof(struct parser_state));
	struct unsigned_char_list * lexer_output = malloc(sizeof(struct unsigned_char_list));
	struct unsigned_char_list * generated_code = malloc(sizeof(struct unsigned_char_list));
	struct c_lexer_state * c_lexer_state = malloc(sizeof(struct c_lexer_state));
	/*  Keep track of to free later */
	struct_c_lexer_state_ptr_list_add_end(lexers, c_lexer_state);
	struct_parser_state_ptr_list_add_end(parsers, parser_state);
	struct_unsigned_char_list_ptr_list_add_end(chrs, lexer_output);
	struct_unsigned_char_list_ptr_list_add_end(chrs, generated_code);
	/*  Initialize */
	unsigned_char_list_create(generated_code);
	unsigned_char_list_create(lexer_output);
	create_c_lexer_state(c_lexer_state, lexer_output, m, (unsigned char *)"No File.", (unsigned char *)str, strlen(str));
	assert(!lex_c(c_lexer_state));
	create_parser_state(parser_state, m, c_lexer_state, generated_code, (unsigned char *)str, engine);
	return parser_state;
}

void test_type_id(const char * str, struct memory_pool_collection * m, enum ID_TYPE rule_type){
	grammar_rule_test(str, m, rule_type, COMPARE_TYPES, (enum parser_error_type)0);
}

void test_parser_error(const char * str, struct memory_pool_collection * m, enum ID_TYPE rule_type, enum parser_error_type error_type){
	grammar_rule_test(str, m, rule_type, ERROR_CHECK, error_type);
}

void grammar_rule_test(const char * str, struct memory_pool_collection * m, enum ID_TYPE type, unsigned int checks, enum parser_error_type error_type){
	struct unsigned_char_list l;
	unsigned char * c;
	struct struct_unsigned_char_list_ptr_list chrs;
	struct struct_c_lexer_state_ptr_list lexers;
	struct struct_parser_state_ptr_list parsers;
	struct type_engine_state engine;
	unsigned int i;
	struct parser_state * p1;
	struct parser_state * p2;
	struct parser_node * n1;
	struct parser_node * n2;
	unsigned int id1;
	unsigned int id2;

	struct_parser_state_ptr_list_create(&parsers);
	struct_c_lexer_state_ptr_list_create(&lexers);
	struct_unsigned_char_list_ptr_list_create(&chrs);

	create_type_engine_state(&engine, m);

	p1 = setup_parser(str, &engine, m, &chrs, &lexers, &parsers);
	p2 = setup_parser(str, &engine, m, &chrs, &lexers, &parsers);

	unsigned_char_list_create(&l);
	buffered_printf(&l, "--------------------------\n");
	switch(type){
		case TEST_1_TYPE_SPECIFIER:{
			n1 = type_specifier(p1, &id1);
			n2 = type_specifier(p2, &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing type_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_all_type_specifiers(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_TYPE_QUALIFIER:{
			n1 = type_qualifier(p1, &id1);
			n2 = type_qualifier(p2, &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing type_qualifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_type_qualifier(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STORAGE_CLASS_SPECIFIER:{
			n1 = storage_class_specifier(p1, &id1);
			n2 = storage_class_specifier(p2, &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing storage_class_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_storage_class_specifier(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STRUCT_OR_UNION_SPECIFIER:{
			n1 = struct_or_union_specifier(p1, &id1, 0/*  For testing, both have same anonymous id */);
			n2 = struct_or_union_specifier(p2, &id2, 0/*  For testing, both have same anonymous id */);
			if(n1 && n2){
				buffered_printf(&l, "Testing struct_or_union_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_scoped_struct_or_union_specifier(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DECLARATION_SPECIFIERS:{
			n1 = declaration_specifiers(p1, get_type_engine_id_for_specifier_or_qualifier_list_begin(p1->type_engine), &id1);
			n2 = declaration_specifiers(p2, get_type_engine_id_for_specifier_or_qualifier_list_begin(p2->type_engine), &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing declaration_specifiers rule with input '%s'.\nSuccessfully identified:\n", str);
				print_specifier_or_qualifier_list(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_SPECIFIER_QUALIFIER_LIST:{
			n1 = specifier_qualifier_list(p1, get_type_engine_id_for_specifier_or_qualifier_list_begin(p1->type_engine), &id1);
			n2 = specifier_qualifier_list(p2, get_type_engine_id_for_specifier_or_qualifier_list_begin(p2->type_engine), &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing specifier_qualifier_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_specifier_or_qualifier_list(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_TYPE_QUALIFIER_LIST:{
			n1 = type_qualifier_list(p1, get_type_engine_id_for_specifier_or_qualifier_list_begin(p1->type_engine), &id1);
			n2 = type_qualifier_list(p2, get_type_engine_id_for_specifier_or_qualifier_list_begin(p2->type_engine), &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing type_qualifier_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_specifier_or_qualifier_list(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DECLARATOR:{
			struct identifier_from_declarator ifd1;
			struct identifier_from_declarator ifd2;
			ifd1.initialized = 0;
			ifd2.initialized = 0;
			n1 = declarator(p1, get_type_engine_id_for_declarator_part_list_begin(p1->type_engine), &id1, &ifd1);
			n2 = declarator(p2, get_type_engine_id_for_declarator_part_list_begin(p2->type_engine), &id2, &ifd2);
			if(n1 && n2){
				buffered_printf(&l, "Testing declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				print_declarator_in_spiral_rule_order(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
				print_declarator_in_visual_order(&engine, &l, id1, TYPE_ENGINE_NORMAL, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_ABSTRACT_DECLARATOR:{
			n1 = abstract_declarator(p1, get_type_engine_id_for_declarator_part_list_begin(p1->type_engine), &id1);
			n2 = abstract_declarator(p2, get_type_engine_id_for_declarator_part_list_begin(p2->type_engine), &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing abstract_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				print_declarator_in_spiral_rule_order(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
				print_declarator_in_visual_order(&engine, &l, id1, TYPE_ENGINE_NORMAL, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STRUCT_DECLARATOR:{
			struct identifier_from_declarator ifd1;
			struct identifier_from_declarator ifd2;
			ifd1.initialized = 0;
			ifd2.initialized = 0;
			n1 = struct_declarator(p1, &id1, &ifd1);
			n2 = struct_declarator(p2, &id2, &ifd2);
			if(n1 && n2){
				buffered_printf(&l, "Testing struct_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				print_bitfield_or_declarator(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_INIT_DECLARATOR:{
			struct identifier_from_declarator ifd1;
			struct identifier_from_declarator ifd2;
			ifd1.initialized = 0;
			ifd2.initialized = 0;
			n1 = init_declarator(p1, &id1, &ifd1);
			n2 = init_declarator(p2, &id2, &ifd2);
			if(n1 && n2){
				buffered_printf(&l, "Testing init_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				print_bitfield_or_declarator(&engine, &l, id1, TYPE_ENGINE_NORMAL);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_PARAMETER_LIST:{
			n1 = parameter_list(p1, &id1);
			n2 = parameter_list(p2, &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing parameter_list rule with input '%s'.\nSuccessfully identified:\n", str);
				buffered_printf(&l, "(");
				print_general_type_list(&engine, (unsigned char *)", ", &l, id1, TYPE_ENGINE_NORMAL, 0);
				buffered_printf(&l, ")\n");
			}
			break;
		}case TEST_1_STRUCT_DECLARATION:{
			n1 = struct_declaration(p1, get_type_engine_id_for_general_type_list_begin(&engine), &id1, create_declaration_namespace(&engine));
			n2 = struct_declaration(p2, get_type_engine_id_for_general_type_list_begin(&engine), &id2, create_declaration_namespace(&engine));
			if(n1 && n2){
				buffered_printf(&l, "Testing struct_declaration rule with input '%s'.\nSuccessfully identified:\n", str);
				print_general_type_list(&engine, (unsigned char *)";\n", &l, id1, TYPE_ENGINE_NORMAL, 0);
				buffered_printf(&l, ";\n");
			}
			break;
		}case TEST_1_STRUCT_DECLARATION_LIST:{
			n1 = struct_declaration_list(p1, get_type_engine_id_for_general_type_list_begin(&engine), &id1, create_declaration_namespace(&engine));
			n2 = struct_declaration_list(p2, get_type_engine_id_for_general_type_list_begin(&engine), &id2, create_declaration_namespace(&engine));
			if(n1 && n2){
				buffered_printf(&l, "Testing struct_declaration_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_general_type_list(&engine, (unsigned char *)";\n", &l, id1, TYPE_ENGINE_NORMAL, 0);
				buffered_printf(&l, ";\n");
			}
			break;
		}case TEST_1_DECLARATION:{
			n1 = declaration(p1, get_type_engine_id_for_general_type_list_begin(&engine), &id1);
			n2 = declaration(p2, get_type_engine_id_for_general_type_list_begin(&engine), &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing declaration rule with input '%s'.\nSuccessfully identified:\n", str);
				print_general_type_list(&engine, (unsigned char *)";\n", &l, id1, TYPE_ENGINE_NORMAL, 0);
				buffered_printf(&l, ";\n");
			}
			break;
		}case TEST_1_DECLARATION_LIST:{
			n1 = declaration_list(p1, (struct namespace_object *)0, get_type_engine_id_for_general_type_list_begin(&engine), &id1);
			n2 = declaration_list(p2, (struct namespace_object *)0, get_type_engine_id_for_general_type_list_begin(&engine), &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing declaration_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_general_type_list(&engine, (unsigned char *)";\n", &l, id1, TYPE_ENGINE_NORMAL, 0);
				buffered_printf(&l, ";\n");
			}
			break;
		}case TEST_1_IDENTIFIER:{
			n1 = get_identifier_node(p1, &id1);
			n2 = get_identifier_node(p2, &id2);
			if(n1 && n2){
				buffered_printf(&l, "Testing identifier rule with input '%s'.\nSuccessfully identified.\n", str);
			}
			break;
		}case TEST_1_PRIMARY_EXPRESSION:{
			primary_expression(p1);
			primary_expression(p2);
			buffered_printf(&l, "Testing primary_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_ARGUMENT_EXPRESSION_LIST:{
			argument_expression_list(p1);
			argument_expression_list(p2);
			buffered_printf(&l, "Testing argument_expression_list rule with input '%s'.\n", str);
			break;
		}case TEST_1_ARGUMENT_EXPRESSION_LIST_REST:{
			argument_expression_list_rest(p1);
			argument_expression_list_rest(p2);
			buffered_printf(&l, "Testing argument_expression_list_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_POSTFIX_EXPRESSION_REST:{
			postfix_expression_rest(p1);
			postfix_expression_rest(p2);
			buffered_printf(&l, "Testing postfix_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_POSTFIX_EXPRESSION:{
			postfix_expression(p1);
			postfix_expression(p2);
			buffered_printf(&l, "Testing postfix_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_UNARY_EXPRESSION:{
			unary_expression(p1);
			unary_expression(p2);
			buffered_printf(&l, "Testing unary_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_CAST_EXPRESSION:{
			cast_expression(p1);
			cast_expression(p2);
			buffered_printf(&l, "Testing cast_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_MULTIPLICATIVE_EXPRESSION_REST:{
			multiplicative_expression_rest(p1);
			multiplicative_expression_rest(p2);
			buffered_printf(&l, "Testing multiplicative_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_MULTIPLICATIVE_EXPRESSION:{
			multiplicative_expression(p1);
			multiplicative_expression(p2);
			buffered_printf(&l, "Testing multiplicative_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_ADDITIVE_EXPRESSION_REST:{
			additive_expression_rest(p1);
			additive_expression_rest(p2);
			buffered_printf(&l, "Testing additive_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_ADDITIVE_EXPRESSION:{
			additive_expression(p1);
			additive_expression(p2);
			buffered_printf(&l, "Testing additive_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_SHIFT_EXPRESSION_REST:{
			shift_expression_rest(p1);
			shift_expression_rest(p2);
			buffered_printf(&l, "Testing shift_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_SHIFT_EXPRESSION:{
			shift_expression(p1);
			shift_expression(p2);
			buffered_printf(&l, "Testing shift_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_RELATIONAL_EXPRESSION_REST:{
			relational_expression_rest(p1);
			relational_expression_rest(p2);
			buffered_printf(&l, "Testing relational_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_RELATIONAL_EXPRESSION:{
			relational_expression(p1);
			relational_expression(p2);
			buffered_printf(&l, "Testing relational_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_EQUALITY_EXPRESSION_REST:{
			equality_expression_rest(p1);
			equality_expression_rest(p2);
			buffered_printf(&l, "Testing equality_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_EQUALITY_EXPRESSION:{
			equality_expression(p1);
			equality_expression(p2);
			buffered_printf(&l, "Testing equality_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_AND_EXPRESSION_REST:{
			and_expression_rest(p1);
			and_expression_rest(p2);
			buffered_printf(&l, "Testing and_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_AND_EXPRESSION:{
			and_expression(p1);
			and_expression(p2);
			buffered_printf(&l, "Testing and_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_EXCLUSIVE_OR_EXPRESSION_REST:{
			exclusive_or_expression_rest(p1);
			exclusive_or_expression_rest(p2);
			buffered_printf(&l, "Testing exclusive_or_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_EXCLUSIVE_OR_EXPRESSION:{
			exclusive_or_expression(p1);
			exclusive_or_expression(p2);
			buffered_printf(&l, "Testing exclusive_or_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_INCLUSIVE_OR_EXPRESSION_REST:{
			inclusive_or_expression_rest(p1);
			inclusive_or_expression_rest(p2);
			buffered_printf(&l, "Testing inclusive_or_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_INCLUSIVE_OR_EXPRESSION:{
			inclusive_or_expression(p1);
			inclusive_or_expression(p2);
			buffered_printf(&l, "Testing inclusive_or_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_LOGICAL_AND_EXPRESSION_REST:{
			logical_and_expression_rest(p1);
			logical_and_expression_rest(p2);
			buffered_printf(&l, "Testing logical_and_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_LOGICAL_AND_EXPRESSION:{
			logical_and_expression(p1);
			logical_and_expression(p2);
			buffered_printf(&l, "Testing logical_and_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_LOGICAL_OR_EXPRESSION_REST:{
			logical_or_expression_rest(p1);
			logical_or_expression_rest(p2);
			buffered_printf(&l, "Testing logical_or_expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_LOGICAL_OR_EXPRESSION:{
			logical_or_expression(p1);
			logical_or_expression(p2);
			buffered_printf(&l, "Testing logical_or_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_COMPOUND_STATEMENT:{
			compound_statement(p1, (struct parser_node *)0, 0);
			compound_statement(p2, (struct parser_node *)0, 0);
			buffered_printf(&l, "Testing compound_statement rule with input '%s'.\n", str);
			break;
		}case TEST_1_CONDITIONAL_EXPRESSION:{
			conditional_expression(p1);
			conditional_expression(p2);
			buffered_printf(&l, "Testing conditional_expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_EXPRESSION_REST:{
			expression_rest(p1);
			expression_rest(p2);
			buffered_printf(&l, "Testing expression_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_EXPRESSION:{
			expression(p1);
			expression(p2);
			buffered_printf(&l, "Testing expression rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUMERATOR:{
			enumerator(p1);
			enumerator(p2);
			buffered_printf(&l, "Testing enumerator rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUMERATOR_LIST_REST:{
			enumerator_list_rest(p1);
			enumerator_list_rest(p2);
			buffered_printf(&l, "Testing enumerator_list_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUMERATOR_LIST:{
			enumerator_list(p1);
			enumerator_list(p2);
			buffered_printf(&l, "Testing enumerator_list rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUM_SPECIFIER:{
			enum_specifier(p1);
			enum_specifier(p2);
			buffered_printf(&l, "Testing enum_specifier rule with input '%s'.\n", str);
			break;
		}case TEST_1_DIRECT_DECLARATOR_REST:{
			unsigned int list_id1;
			unsigned int list_id2;
			struct identifier_from_declarator ifd1;
			struct identifier_from_declarator ifd2;
			direct_declarator_rest(p1, get_type_engine_id_for_declarator_part_list_begin(&engine), &list_id1, &ifd1);
			direct_declarator_rest(p2, get_type_engine_id_for_declarator_part_list_begin(&engine), &list_id2, &ifd2);
			buffered_printf(&l, "Testing direct_declarator_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_DIRECT_DECLARATOR:{
			unsigned int list_id1;
			unsigned int list_id2;
			struct identifier_from_declarator ifd1;
			struct identifier_from_declarator ifd2;
			ifd1.initialized = 0;
			ifd2.initialized = 0;
			direct_declarator(p1, get_type_engine_id_for_declarator_part_list_begin(&engine), &list_id1, &ifd1);
			direct_declarator(p2, get_type_engine_id_for_declarator_part_list_begin(&engine), &list_id2, &ifd2);
			buffered_printf(&l, "Testing direct_declarator rule with input '%s'.\n", str);
			break;
		}case TEST_1_INITIALIZER_LIST_REST:{
			initializer_list_rest(p1);
			initializer_list_rest(p2);
			buffered_printf(&l, "Testing initializer_list_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_INITIALIZER:{
			initializer(p1);
			initializer(p2);
			buffered_printf(&l, "Testing initializer rule with input '%s'.\n", str);
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}

	if(COMPARE_TYPES & checks){
		assert(id1 == id2);
		assert(n1 && n2);
	}

	if(ERROR_CHECK & checks){
		unsigned int found = 0;
		unsigned int i;
		assert(struct_parser_error_list_size(&p1->parser_errors));
		/*  Make sure at least one of the parser errors was the one we expected. */
		for(i = 0; i < struct_parser_error_list_size(&p1->parser_errors); i++){
			struct parser_error e = struct_parser_error_list_get(&p1->parser_errors, i);
			buffered_printf(&l, "***\n");
			print_parser_error(&l, p1, e);
			buffered_printf(&l, "***\n");
			if(e.type == error_type){
				found = 1;
				break;
			}
		}
		assert(found);
	}

	buffered_printf(&l, "--------------------------\n");
	c = unsigned_char_list_data(&l);
	while(*c){
		putchar(*c);
		c++;
	}
	unsigned_char_list_destroy(&l);

	/*  Clean up testing resources. */
	for(i = 0; i < struct_parser_state_ptr_list_size(&parsers); i++){
		destroy_parser_state(struct_parser_state_ptr_list_get(&parsers, i));
		free(struct_parser_state_ptr_list_get(&parsers, i));
	}
	struct_parser_state_ptr_list_destroy(&parsers);

	for(i = 0; i < struct_c_lexer_state_ptr_list_size(&lexers); i++){
		destroy_c_lexer_state(struct_c_lexer_state_ptr_list_get(&lexers, i));
		free(struct_c_lexer_state_ptr_list_get(&lexers, i));
	}
	struct_c_lexer_state_ptr_list_destroy(&lexers);

	for(i = 0; i < struct_unsigned_char_list_ptr_list_size(&chrs); i++){
		unsigned_char_list_destroy(struct_unsigned_char_list_ptr_list_get(&chrs, i));
		free(struct_unsigned_char_list_ptr_list_get(&chrs, i));
	}
	struct_unsigned_char_list_ptr_list_destroy(&chrs);

	destroy_type_engine_state(&engine);
}

void test_1(struct memory_pool_collection * m){
	test_type_id("int ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("signed ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("unsigned ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("float ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("long ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("double ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("void ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("char ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("short ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("struct foo ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("union boo ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("struct goo {int i;}", m, TEST_1_TYPE_SPECIFIER);

	test_type_id("struct goo {int i;}", m, TEST_1_STRUCT_OR_UNION_SPECIFIER);
	test_type_id("struct {int i;}", m, TEST_1_STRUCT_OR_UNION_SPECIFIER);
	test_type_id("union {struct asdf * f; int j;}", m, TEST_1_STRUCT_OR_UNION_SPECIFIER);

	test_type_id("volatile ", m, TEST_1_TYPE_QUALIFIER);
	test_type_id("const ", m, TEST_1_TYPE_QUALIFIER);

	test_type_id("typedef ", m, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_type_id("extern ", m, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_type_id("auto ", m, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_type_id("register ", m, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_type_id("static ", m, TEST_1_STORAGE_CLASS_SPECIFIER);

	test_type_id("foo ", m, TEST_1_IDENTIFIER);
	test_type_id("shor ", m, TEST_1_IDENTIFIER);
	test_type_id("short_ ", m, TEST_1_IDENTIFIER);
	test_type_id("shortt ", m, TEST_1_IDENTIFIER);
	test_type_id("shorttt ", m, TEST_1_IDENTIFIER);
	test_type_id("_ ", m, TEST_1_IDENTIFIER);
	test_type_id("_1 ", m, TEST_1_IDENTIFIER);

	test_type_id("const int ", m, TEST_1_DECLARATION_SPECIFIERS);
	test_type_id("static const int ", m, TEST_1_DECLARATION_SPECIFIERS);
	test_type_id("extern const int ", m, TEST_1_DECLARATION_SPECIFIERS);
	test_type_id("extern int ", m, TEST_1_DECLARATION_SPECIFIERS);
	test_type_id("const double ", m, TEST_1_SPECIFIER_QUALIFIER_LIST);
	test_type_id("const long long ", m, TEST_1_SPECIFIER_QUALIFIER_LIST);
	test_type_id("const int ", m, TEST_1_SPECIFIER_QUALIFIER_LIST);
	test_type_id("const const const const const ", m, TEST_1_TYPE_QUALIFIER_LIST);
	test_type_id("const volatile const ", m, TEST_1_TYPE_QUALIFIER_LIST);
	test_type_id("const ", m, TEST_1_TYPE_QUALIFIER_LIST);
	test_type_id("volatile ", m, TEST_1_TYPE_QUALIFIER_LIST);

	test_type_id("(******const a[4][5])(void)", m, TEST_1_DECLARATOR);
	test_type_id("(*a)(void)", m, TEST_1_DECLARATOR);
	test_type_id("*(*a)[3]", m, TEST_1_DECLARATOR);
	test_type_id("*(* const volatile*a)[3][]", m, TEST_1_DECLARATOR);
	test_type_id("(*(*a)(void))(void)", m, TEST_1_DECLARATOR);
	test_type_id("(*(*a)(void))[3]", m, TEST_1_DECLARATOR);
	test_type_id("(*(*a)[3])(void)", m, TEST_1_DECLARATOR);
	test_type_id("(*((*a))[3])(void)", m, TEST_1_DECLARATOR);
	test_type_id("(*((*a)[3]))(void)", m, TEST_1_DECLARATOR);
	test_type_id("(*(*a[3]))(void)", m, TEST_1_DECLARATOR);

	test_type_id("(******const [4][5])(void)", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("(*)(void)", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("*(*)[3]", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("*(* const volatile*)[3][]", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("(*(*)(void))(void)", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("(*(*)(void))[3]", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("(*(*)[3])(void)", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("(*((*))[3])(void)", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("(*((*)[3]))(void)", m, TEST_1_ABSTRACT_DECLARATOR);
	test_type_id("(*(*[3]))(void)", m, TEST_1_ABSTRACT_DECLARATOR);

	test_type_id("(*(*a[3]))(void)", m, TEST_1_INIT_DECLARATOR);
	test_type_id("*fff", m, TEST_1_INIT_DECLARATOR);
	test_type_id("fff[9]", m, TEST_1_INIT_DECLARATOR);
	test_type_id("fff(void)", m, TEST_1_INIT_DECLARATOR);
	test_type_id("fff()", m, TEST_1_INIT_DECLARATOR);
	test_type_id("fff(a,b,c)", m, TEST_1_INIT_DECLARATOR);
	test_type_id("abc:3", m, TEST_1_STRUCT_DECLARATOR);
	test_type_id(":44", m, TEST_1_STRUCT_DECLARATOR);

	test_type_id("int i, int j[3], void (*k)(double)", m, TEST_1_PARAMETER_LIST);
	test_type_id("void ", m, TEST_1_PARAMETER_LIST);
	test_type_id("int, int ", m, TEST_1_PARAMETER_LIST);

	test_type_id("double asdf;", m, TEST_1_STRUCT_DECLARATION);
	test_type_id("int i, j[4], *k;", m, TEST_1_STRUCT_DECLARATION);
	test_type_id("long i;", m, TEST_1_STRUCT_DECLARATION);

	test_type_id("long i;", m, TEST_1_STRUCT_DECLARATION_LIST);
	test_type_id("double i; int j; float arr[2], k(), *t;", m, TEST_1_STRUCT_DECLARATION_LIST);
	test_type_id("int i; float t;", m, TEST_1_STRUCT_DECLARATION_LIST);
	test_type_id("int i; char * f, d(), n[3]; long long t;", m, TEST_1_STRUCT_DECLARATION_LIST);

	test_type_id("char f = 'a', d(), n[3];", m, TEST_1_DECLARATION);

	test_type_id("struct foo {int i;}; struct boo {int i;};", m, TEST_1_DECLARATION_LIST);

	test_parser_error("struct foo {int i;};\nstruct foo {int i;};", m, TEST_1_DECLARATION_LIST, PARSER_ERROR_STRUCT_OR_UNION_REDEFINITION);
	test_parser_error("struct foo {int i;", m, TEST_1_DECLARATION_LIST, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_BRACE);
	test_parser_error("struct foo {}", m, TEST_1_DECLARATION_LIST, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_NO_DECLARATIONS);

	test_parser_error("(123", m, TEST_1_PRIMARY_EXPRESSION, PARSER_ERROR_PRIMARY_EXPRESSION_MISSING_CLOSE_PAREN);
	test_parser_error("a = ;", m, TEST_1_INIT_DECLARATOR, PARSER_ERROR_INIT_DECLARATOR_MISSING_INITIALIZER);

	test_parser_error("1," , m, TEST_1_ARGUMENT_EXPRESSION_LIST, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_MISSING_ARGUMENT);
	test_parser_error(",3,,", m, TEST_1_ARGUMENT_EXPRESSION_LIST_REST, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_REST_MISSING_ARGUMENT);
	test_parser_error(",4,,", m, TEST_1_ARGUMENT_EXPRESSION_LIST_REST, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_REST_MISSING_ASSIGNMENT_EXPRESSION);

	test_parser_error("[1][;", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_NO_REST);
	test_parser_error("[2;", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_EXPRESSION_INCOMPLETE);
	test_parser_error("[;", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_ARRAY_EXPRESSION_MISSING);
	test_parser_error("()(", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_0_ARGS_NO_REST);
	test_parser_error("(1)(", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_SOME_ARGS_NO_REST);
	test_parser_error("(1", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_SOME_ARGS_NO_CLOSE);
	test_parser_error("(", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_0_ARGS_NO_CLOSE);
	test_parser_error(".i.", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_DOT_NO_REST);
	test_parser_error(".", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_DOT_NO_IDENTIFIER);
	test_parser_error("->a->", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_PTR_NO_REST);
	test_parser_error("->", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_PTR_NO_IDENTIFIER);
	test_parser_error("--(", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_DEC_NO_REST);
	test_parser_error("++(", m, TEST_1_POSTFIX_EXPRESSION_REST, PARSER_ERROR_POSTFIX_EXPRESSION_REST_INC_NO_REST);

	test_parser_error("i(", m, TEST_1_POSTFIX_EXPRESSION, PARSER_ERROR_POSTFIX_EXPRESSION_NO_REST);
	test_parser_error("++(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_AFTER_INC);
	test_parser_error("--(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_AFTER_DEC);
	test_parser_error("+(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST);
	test_parser_error("sizeof(int(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_MISSING_PAREN);
	test_parser_error("sizeof(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_MISSING_TYPE);
	test_parser_error("sizeof ", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_BAD_SIZEOF);

	test_parser_error("(int))", m, TEST_1_CAST_EXPRESSION, PARSER_ERROR_CAST_EXPRESSION_MISSING_CAST);
	test_parser_error("(int(", m, TEST_1_CAST_EXPRESSION, PARSER_ERROR_CAST_EXPRESSION_MISSING_CLOSE_PAREN);
	test_parser_error("()", m, TEST_1_CAST_EXPRESSION, PARSER_ERROR_CAST_EXPRESSION_MISSING_TYPE_NAME);

	test_parser_error("*(", m, TEST_1_MULTIPLICATIVE_EXPRESSION_REST, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("*3*", m, TEST_1_MULTIPLICATIVE_EXPRESSION_REST, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_NO_REST);
	test_parser_error("3*", m, TEST_1_MULTIPLICATIVE_EXPRESSION, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_NO_REST);

	test_parser_error("+(", m, TEST_1_ADDITIVE_EXPRESSION_REST, PARSER_ERROR_ADDITIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("+3+", m, TEST_1_ADDITIVE_EXPRESSION_REST, PARSER_ERROR_ADDITIVE_EXPRESSION_REST_NO_REST);
	test_parser_error("3+", m, TEST_1_ADDITIVE_EXPRESSION, PARSER_ERROR_ADDITIVE_EXPRESSION_NO_REST);

	test_parser_error(">>(", m, TEST_1_SHIFT_EXPRESSION_REST, PARSER_ERROR_SHIFT_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error(">>3>>", m, TEST_1_SHIFT_EXPRESSION_REST, PARSER_ERROR_SHIFT_EXPRESSION_REST_NO_REST);
	test_parser_error("3>>", m, TEST_1_SHIFT_EXPRESSION, PARSER_ERROR_SHIFT_EXPRESSION_NO_REST);

	test_parser_error(">(", m, TEST_1_RELATIONAL_EXPRESSION_REST, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error(">3>", m, TEST_1_RELATIONAL_EXPRESSION_REST, PARSER_ERROR_RELATIONAL_EXPRESSION_REST_NO_REST);
	test_parser_error("3>", m, TEST_1_RELATIONAL_EXPRESSION, PARSER_ERROR_RELATIONAL_EXPRESSION_NO_REST);

	test_parser_error("==(", m, TEST_1_EQUALITY_EXPRESSION_REST, PARSER_ERROR_EQUALITY_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("==3==", m, TEST_1_EQUALITY_EXPRESSION_REST, PARSER_ERROR_EQUALITY_EXPRESSION_REST_NO_REST);
	test_parser_error("3==", m, TEST_1_EQUALITY_EXPRESSION, PARSER_ERROR_EQUALITY_EXPRESSION_NO_REST);

	test_parser_error("&(", m, TEST_1_AND_EXPRESSION_REST, PARSER_ERROR_AND_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("&3&", m, TEST_1_AND_EXPRESSION_REST, PARSER_ERROR_AND_EXPRESSION_REST_NO_REST);
	test_parser_error("3&", m, TEST_1_AND_EXPRESSION, PARSER_ERROR_AND_EXPRESSION_NO_REST);

	test_parser_error("^(", m, TEST_1_EXCLUSIVE_OR_EXPRESSION_REST, PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("^3^", m, TEST_1_EXCLUSIVE_OR_EXPRESSION_REST, PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_REST_NO_REST);
	test_parser_error("3^", m, TEST_1_EXCLUSIVE_OR_EXPRESSION, PARSER_ERROR_EXCLUSIVE_OR_EXPRESSION_NO_REST);

	test_parser_error("|(", m, TEST_1_INCLUSIVE_OR_EXPRESSION_REST, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("|3|", m, TEST_1_INCLUSIVE_OR_EXPRESSION_REST, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_REST_NO_REST);
	test_parser_error("3|", m, TEST_1_INCLUSIVE_OR_EXPRESSION, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_NO_REST);

	test_parser_error("&&(", m, TEST_1_LOGICAL_AND_EXPRESSION_REST, PARSER_ERROR_LOGICAL_AND_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("&&3&&", m, TEST_1_LOGICAL_AND_EXPRESSION_REST, PARSER_ERROR_LOGICAL_AND_EXPRESSION_REST_NO_REST);
	test_parser_error("3&&", m, TEST_1_LOGICAL_AND_EXPRESSION, PARSER_ERROR_LOGICAL_AND_EXPRESSION_NO_REST);

	test_parser_error("||(", m, TEST_1_LOGICAL_OR_EXPRESSION_REST, PARSER_ERROR_LOGICAL_OR_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("||3||", m, TEST_1_LOGICAL_OR_EXPRESSION_REST, PARSER_ERROR_LOGICAL_OR_EXPRESSION_REST_NO_REST);
	test_parser_error("3||", m, TEST_1_LOGICAL_OR_EXPRESSION, PARSER_ERROR_LOGICAL_OR_EXPRESSION_NO_REST);

	test_parser_error("{a = b;", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_1);
	test_parser_error("{int i; a = b;", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_2);
	test_parser_error("{int i;", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_3);
	test_parser_error("{", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_4);

	test_parser_error("1 ? 0 :", m, TEST_1_CONDITIONAL_EXPRESSION, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_FALSE);
	test_parser_error("1 ? 0 ", m, TEST_1_CONDITIONAL_EXPRESSION, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_COLON);
	test_parser_error("1 ? ", m, TEST_1_CONDITIONAL_EXPRESSION, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_TRUE);

	test_parser_error(", 1 ,", m, TEST_1_EXPRESSION_REST, PARSER_ERROR_EXPRESSION_REST_NO_REST);
	test_parser_error(",( ", m, TEST_1_EXPRESSION_REST, PARSER_ERROR_EXPRESSION_REST_NO_ASSIGNMENT_EXPRESSION);
	test_parser_error("2,* ", m, TEST_1_EXPRESSION, PARSER_ERROR_EXPRESSION_NO_REST);

	test_parser_error("abc = )", m, TEST_1_ENUMERATOR, PARSER_ERROR_ENUMERATOR_MISSING_CONSTANT_EXPRESSION);

	test_parser_error(", a , abc = )", m, TEST_1_ENUMERATOR_LIST_REST, PARSER_ERROR_ENUMERATOR_LIST_REST_MISSING_REST);
	test_parser_error(", abc = )", m, TEST_1_ENUMERATOR_LIST_REST, PARSER_ERROR_ENUMERATOR_LIST_REST_MISSING_ENUMERATOR);

	test_parser_error("a , abc = )", m, TEST_1_ENUMERATOR_LIST, PARSER_ERROR_ENUMERATOR_LIST_MISSING_REST);

	test_parser_error("enum { a ", m, TEST_1_ENUM_SPECIFIER, PARSER_ERROR_ENUM_SPECIFIER_MISSING_CLOSE_BRACE_ANON);
	test_parser_error("enum { ) ", m, TEST_1_ENUM_SPECIFIER, PARSER_ERROR_ENUM_SPECIFIER_MISSING_ENUMERATOR_LIST_ANON);
	test_parser_error("enum a { a ", m, TEST_1_ENUM_SPECIFIER, PARSER_ERROR_ENUM_SPECIFIER_MISSING_CLOSE_BRACE_NAMED);
	test_parser_error("enum a { ) ", m, TEST_1_ENUM_SPECIFIER, PARSER_ERROR_ENUM_SPECIFIER_MISSING_ENUMERATOR_LIST_NAMED);
	test_parser_error("enum ) ", m, TEST_1_ENUM_SPECIFIER, PARSER_ERROR_ENUM_SPECIFIER_MISSING_OPEN_BRACE_OR_IDENTIFIER);


	test_parser_error("[3][", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_ARRAY_SIZED);
	test_parser_error("[3", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_AFTER_CONSTANT);
	test_parser_error("[][", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_ARRAY_UNSIZED);
	test_parser_error("[", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_OR_CONSTANT);
	test_parser_error("(int)(", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_PARAM_FUNCTION);
	test_parser_error("(int ", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_PARAM);
	test_parser_error("(a, b)( ", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_IDENTIFIER_FUNCTION);
	test_parser_error("(a, b ", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_IDENTIFIER_LIST);
	test_parser_error("()( ", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_REST_AFTER_KR_FUNCTION);
	test_parser_error("( ", m, TEST_1_DIRECT_DECLARATOR_REST, PARSER_ERROR_DIRECT_DECLARATOR_REST_MISSING_CLOSE_PAREN);


	test_parser_error("a ( ", m, TEST_1_DIRECT_DECLARATOR, PARSER_ERROR_DIRECT_DECLARATOR_MISSING_REST_AFTER_IDENTIFIER);
	test_parser_error("(a) (", m, TEST_1_DIRECT_DECLARATOR, PARSER_ERROR_DIRECT_DECLARATOR_MISSING_REST_AFTER_CLOSE_PAREN);
	test_parser_error("( a ", m, TEST_1_DIRECT_DECLARATOR, PARSER_ERROR_DIRECT_DECLARATOR_MISSING_CLOSE_PAREN);

	test_parser_error("{f ", m, TEST_1_INITIALIZER, PARSER_ERROR_INITIALIZER_MISSING_COMMA_OR_CLOSE_BRACE);
	test_parser_error("{}", m, TEST_1_INITIALIZER, PARSER_ERROR_INITIALIZER_MISSING_INITIALIZER_LIST);
}

int main(void){
	struct memory_pool_collection m;
	memory_pool_collection_create(&m);
	test_1(&m);
	memory_pool_collection_destroy(&m);
	return 0;
}
