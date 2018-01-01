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
#include "../recc-implementation/asm.h"
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
	TEST_1_STRUCT_DECLARATOR_LIST_REST,
	TEST_1_INIT_DECLARATOR,
	TEST_1_PARAMETER_LIST,
	TEST_1_PARAMETER_TYPE_LIST,
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
	TEST_1_ASSIGNMENT_EXPRESSION,
	TEST_1_EXPRESSION,
	TEST_1_EXPRESSION_REST,
	TEST_1_ENUMERATOR,
	TEST_1_ENUMERATOR_LIST_REST,
	TEST_1_ENUMERATOR_LIST,
	TEST_1_ENUM_SPECIFIER,
	TEST_1_DIRECT_DECLARATOR_REST,
	TEST_1_DIRECT_DECLARATOR,
	TEST_1_INITIALIZER_LIST_REST,
	TEST_1_INITIALIZER,
	TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST,
	TEST_1_DIRECT_ABSTRACT_DECLARATOR,
	TEST_1_LABELED_STATEMENT,
	TEST_1_SELECTION_STATEMENT,
	TEST_1_ITERATION_STATEMENT,
	TEST_1_JUMP_STATEMENT,
	TEST_1_TRANSLATION_UNIT,
	TEST_1_TYPEDEF
};

void flush_buffer(struct unsigned_char_list *);
void test_1(struct memory_pool_collection *);
void test_type_id(const char * str, struct memory_pool_collection *, enum ID_TYPE);
void test_type_id_dcl(const char *, unsigned int, const char *, struct memory_pool_collection *, enum ID_TYPE);
void test_parser_error(const char * str, struct memory_pool_collection *, enum ID_TYPE, enum parser_error_type);
void test_parser_error_dcl(const char *, unsigned int, const char *, struct memory_pool_collection *, enum ID_TYPE, enum parser_error_type);
void grammar_rule_test(const char * str, struct memory_pool_collection *, enum ID_TYPE, unsigned int error_check, enum parser_error_type, unsigned int);
struct parser_state * setup_parser(const char *, struct asm_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);

struct parser_state * setup_parser(const char * str, struct asm_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * parser_state = (struct parser_state *)malloc(sizeof(struct parser_state));
	struct unsigned_char_list * lexer_output = (struct unsigned_char_list *)malloc(sizeof(struct unsigned_char_list));
	struct unsigned_char_list * generated_code = (struct unsigned_char_list *)malloc(sizeof(struct unsigned_char_list));
	struct c_lexer_state * c_lexer_state = (struct c_lexer_state *)malloc(sizeof(struct c_lexer_state));
	/*  Keep track of to free later */
	struct_c_lexer_state_ptr_list_add_end(lexers, c_lexer_state);
	struct_parser_state_ptr_list_add_end(parsers, parser_state);
	struct_unsigned_char_list_ptr_list_add_end(chrs, lexer_output);
	struct_unsigned_char_list_ptr_list_add_end(chrs, generated_code);
	/*  Initialize */
	unsigned_char_list_create(generated_code);
	unsigned_char_list_create(lexer_output);
	create_c_lexer_state(c_lexer_state, lexer_output, m, (unsigned char *)"No File.", (unsigned char *)str, (unsigned int)strlen(str));
	assert(!lex_c(c_lexer_state));
	create_parser_state(parser_state, m, c_lexer_state, generated_code, (unsigned char *)str, engine);
	increment_scope_depth(parser_state, SCOPE_LEVEL_TYPE_FILE_SCOPE);
	return parser_state;
}

void test_type_id(const char * str, struct memory_pool_collection * m, enum ID_TYPE rule_type){
	grammar_rule_test(str, m, rule_type, COMPARE_TYPES, (enum parser_error_type)0, 0);
}

void test_type_id_dcl(const char * dcl, unsigned int num, const char * str, struct memory_pool_collection * m, enum ID_TYPE rule_type){
	/*  In order to correctly parse identifiers, the syntax model needs associated declarations to be created first. */
	struct unsigned_char_list all;
	unsigned_char_list_create(&all);
	buffered_printf(&all, "%s%s", dcl, str);
	grammar_rule_test((const char *)unsigned_char_list_data(&all), m, rule_type, COMPARE_TYPES, (enum parser_error_type)0, num);
	unsigned_char_list_destroy(&all);
}

void test_parser_error_dcl(const char * dcl, unsigned int num, const char * str, struct memory_pool_collection * m, enum ID_TYPE rule_type, enum parser_error_type error_type){
	/*  In order to correctly parse identifiers, the syntax model needs associated declarations to be created first. */
	struct unsigned_char_list all;
	unsigned_char_list_create(&all);
	buffered_printf(&all, "%s%s", dcl, str);
	grammar_rule_test((const char *)unsigned_char_list_data(&all), m, rule_type, ERROR_CHECK, error_type, num);
	unsigned_char_list_destroy(&all);
}

void test_parser_error(const char * str, struct memory_pool_collection * m, enum ID_TYPE rule_type, enum parser_error_type error_type){
	grammar_rule_test(str, m, rule_type, ERROR_CHECK, error_type, 0);
}

void flush_buffer(struct unsigned_char_list * l){
	unsigned char * c = unsigned_char_list_data(l);
	unsigned int size = unsigned_char_list_size(l);
	unsigned int i;
	for(i = 0; i < size; i++){
		putchar(c[i]);
	}
	unsigned_char_list_destroy(l);
	unsigned_char_list_create(l);
}

void grammar_rule_test(const char * str, struct memory_pool_collection * m, enum ID_TYPE type, unsigned int checks, enum parser_error_type error_type, unsigned int num_declarations){
	struct unsigned_char_list l;
	struct struct_unsigned_char_list_ptr_list chrs;
	struct struct_c_lexer_state_ptr_list lexers;
	struct struct_parser_state_ptr_list parsers;
	struct asm_state engine;
	unsigned int i;
	struct parser_state * p1;
	struct parser_node * n1;

	struct_parser_state_ptr_list_create(&parsers);
	struct_c_lexer_state_ptr_list_create(&lexers);
	struct_unsigned_char_list_ptr_list_create(&chrs);

	create_asm_state(&engine, m);

	unsigned_char_list_create(&l);
	buffered_printf(&l, "--------------------------\n");

	buffered_printf(&l, "About to run a test with the following input: '%s'\n", str);
	flush_buffer(&l);

	p1 = setup_parser(str, &engine, m, &chrs, &lexers, &parsers);

	for(i = 0; i < num_declarations; i++){
		struct any_statement_list_item_id start = make_any_statement_list_item_list_start(&engine);
		struct any_statement_list_item_id result;
		struct parser_node * d = declaration(p1, start, &result);
		assert(d);
	}

	switch(type){
		case TEST_1_TYPE_SPECIFIER:{
			struct aggregate_type_specifier_id id1;
			n1 = type_specifier(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing type_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_aggregate_type_specifier(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_TYPE_QUALIFIER:{
			struct simple_type_qualifier_id id1;
			n1 = type_qualifier(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing type_qualifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_simple_type_qualifier(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STORAGE_CLASS_SPECIFIER:{
			struct simple_storage_class_specifier_id id1;
			n1 = simple_storage_class_specifier(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing simple_storage_class_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_simple_storage_class_specifier(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STRUCT_OR_UNION_SPECIFIER:{
			struct scoped_tag_specifier_id id1;
			n1 = struct_or_union_specifier(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing struct_or_union_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_scoped_tag_specifier(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DECLARATION_SPECIFIERS:{
			struct specifier_or_qualifier_list_item_id id1;
			n1 = declaration_specifiers(p1, make_specifier_or_qualifier_list_item_list_start(p1->syntax_model), &id1);
			if(n1){
				buffered_printf(&l, "Testing declaration_specifiers rule with input '%s'.\nSuccessfully identified:\n", str);
				print_specifier_or_qualifier_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_SPECIFIER_QUALIFIER_LIST:{
			struct specifier_or_qualifier_list_item_id id1;
			n1 = specifier_qualifier_list(p1, make_specifier_or_qualifier_list_item_list_start(p1->syntax_model), &id1);
			if(n1){
				buffered_printf(&l, "Testing specifier_qualifier_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_specifier_or_qualifier_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_TYPE_QUALIFIER_LIST:{
			struct specifier_or_qualifier_list_item_id id1;
			n1 = type_qualifier_list(p1, make_specifier_or_qualifier_list_item_list_start(p1->syntax_model), &id1);
			if(n1){
				buffered_printf(&l, "Testing type_qualifier_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_specifier_or_qualifier_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DECLARATOR:{
			struct any_statement_list_item_id aid1;
			struct declarator_part_list_item_id id1;
			struct identifier_from_declarator ifd1;
			unsigned int is_function_definition = 0;
			unsigned int parameter_list_count = 0;
			ifd1.initialized = 0;
			n1 = declarator(p1, make_declarator_part_list_item_list_start(p1->syntax_model), &id1, &ifd1, &aid1, is_function_definition, &parameter_list_count);
			if(n1){
				buffered_printf(&l, "Testing declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				buffered_printf(&l, "Logical view:  ");
				print_declarator_part_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
				buffered_printf(&l, "Identifier:  ");
				print_identifier(&engine, 0, &l, ifd1.id);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_ABSTRACT_DECLARATOR:{
			struct declarator_part_list_item_id id1;
			n1 = abstract_declarator(p1, make_declarator_part_list_item_list_start(p1->syntax_model), &id1);
			if(n1){
				buffered_printf(&l, "Testing abstract_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				print_declarator_part_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STRUCT_DECLARATOR:{
			struct struct_or_union_member_id id1;
			n1 = struct_declarator(p1, make_specifier_or_qualifier_list_item_list_start(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing struct_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STRUCT_DECLARATOR_LIST_REST:{
			struct struct_or_union_member_list_item_id id1;
			struct struct_or_union_member_list_item_id member_list_start = make_struct_or_union_member_list_item_list_start(&engine);
			struct specifier_or_qualifier_list_item_id specifier_list_start = make_specifier_or_qualifier_list_item_list_start(&engine);
			n1 = struct_declarator_list_rest(p1, specifier_list_start, member_list_start, &id1);
			if(n1){
				buffered_printf(&l, "Testing struct_declarator_list_rest rule with input '%s'.\nSuccessfully identified:\n", str);
			}
			break;
		}case TEST_1_INIT_DECLARATOR:{
			struct any_statement_list_item_id result_statement_list;

			n1 = init_declarator(p1, make_specifier_or_qualifier_list_item_list_start(&engine), make_any_statement_list_item_list_start(&engine), &result_statement_list);
			if(n1){
				buffered_printf(&l, "Testing init_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_PARAMETER_LIST:{
			struct general_type_list_item_id id1;
			struct any_statement_list_item_id aid1;
			unsigned int is_function_definition = 0;
			unsigned int parameter_list_count = 0;
			n1 = parameter_list(p1, &id1, &aid1, is_function_definition, &parameter_list_count);
			if(n1){
				buffered_printf(&l, "Testing parameter_list rule with input '%s'.\nSuccessfully identified:\n", str);
				buffered_printf(&l, "(");
				print_general_type_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, ")\n");
			}
			break;
		}case TEST_1_PARAMETER_TYPE_LIST:{
			struct parameter_list_id id1;
			struct any_statement_list_item_id aid1;
			unsigned int is_function_definition = 0;
			unsigned int parameter_list_count = 0;
			n1 = parameter_type_list(p1, &id1, &aid1, is_function_definition, &parameter_list_count);
			if(n1){
				buffered_printf(&l, "Testing parameter_type_list rule with input '%s'.\nSuccessfully identified:\n", str);
				buffered_printf(&l, "(");
				print_parameter_list(&engine, 0, &l, id1);
				buffered_printf(&l, ")\n");
			}
			break;
		}case TEST_1_STRUCT_DECLARATION:{
			struct struct_or_union_member_list_item_id id1;
			struct struct_or_union_member_list_item_id member_list_start = make_struct_or_union_member_list_item_list_start(&engine);
			n1 = struct_declaration(p1, member_list_start, &id1);
			if(n1){
				buffered_printf(&l, "Testing struct_declaration rule with input '%s'.\nSuccessfully identified:\n", str);
				print_struct_or_union_member_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STRUCT_DECLARATION_LIST:{
			struct struct_or_union_member_list_item_id id1;
			struct struct_or_union_member_list_item_id member_list_start = make_struct_or_union_member_list_item_list_start(&engine);
			n1 = struct_declaration_list(p1, member_list_start, &id1);
			if(n1){
				buffered_printf(&l, "Testing struct_declaration_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_struct_or_union_member_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DECLARATION:{
			struct any_statement_list_item_id id1;
			n1 = declaration(p1, make_any_statement_list_item_list_start(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing declaration rule with input '%s'.\nSuccessfully identified:\n", str);
				print_any_statement_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_TYPEDEF:{
			struct any_statement_list_item_id id1;
			n1 = declaration(p1, make_any_statement_list_item_list_start(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing typedef declaration rule with input '%s'.\nSuccessfully identified:\n", str);
				print_any_statement_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
				print_all_identifier_namespace_entrys(&engine, 0, &l);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DECLARATION_LIST:{
			struct any_statement_list_item_id id1;
			n1 = declaration_list(p1, make_any_statement_list_item_list_start(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing declaration_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_any_statement_list_item(&engine, 0, &l, id1, 0);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_IDENTIFIER:{
			struct identifier_id id1;
			n1 = get_any_identifier_node(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing identifier rule with input '%s'.\nSuccessfully identified.\n", str);
				print_identifier(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_PRIMARY_EXPRESSION:{
			struct primary_expression_id id1;
			n1 = primary_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing primary_expression rule with input '%s'.\n", str);
				print_primary_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_ARGUMENT_EXPRESSION_LIST:{
			struct any_statement_list_item_id id1;
			argument_expression_list(p1, &id1);
			buffered_printf(&l, "Testing argument_expression_list rule with input '%s'.\n", str);
			break;
		}case TEST_1_ARGUMENT_EXPRESSION_LIST_REST:{
			struct any_statement_list_item_id id1;
			n1 = argument_expression_list_rest(p1, make_any_statement_list_item_list_start(&engine), &id1);
			buffered_printf(&l, "Testing argument_expression_list_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_POSTFIX_EXPRESSION_REST:{
			struct postfix_expression_part_list_item_id id1;
			n1 = postfix_expression_rest(p1, make_postfix_expression_part_list_item_list_start(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing postfix_expression_rest rule with input '%s'.\n", str);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_POSTFIX_EXPRESSION:{
			struct postfix_expression_id id1;
			n1 = postfix_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing postfix_expression rule with input '%s'.\n", str);
				print_postfix_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_UNARY_EXPRESSION:{
			struct unary_expression_id id1;
			n1 = unary_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing unary_expression rule with input '%s'.\n", str);
				print_unary_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_CAST_EXPRESSION:{
			struct cast_expression_id id1;
			n1 = cast_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing cast_expression rule with input '%s'.\n", str);
				print_cast_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_MULTIPLICATIVE_EXPRESSION:{
			struct expression_id id1;
			n1 = multiplicative_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing multiplicative_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_MULTIPLICATIVE_EXPRESSION_REST:{
			struct expression_id id1;
			n1 = multiplicative_expression_rest(p1, make_expression_nullary_expression(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing multiplicative_expression_rest rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_ADDITIVE_EXPRESSION:{
			struct expression_id id1;
			n1 = additive_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing additive_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_ADDITIVE_EXPRESSION_REST:{
			struct expression_id id1;
			n1 = additive_expression_rest(p1, make_expression_nullary_expression(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing additive_expression_rest rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_SHIFT_EXPRESSION:{
			struct expression_id id1;
			n1 = shift_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing shift_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_SHIFT_EXPRESSION_REST:{
			struct expression_id id1;
			n1 = shift_expression_rest(p1, make_expression_nullary_expression(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing shift_expression_rest rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_RELATIONAL_EXPRESSION:{
			struct expression_id id1;
			n1 = relational_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing relational_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_RELATIONAL_EXPRESSION_REST:{
			struct expression_id id1;
			n1 = relational_expression_rest(p1, make_expression_nullary_expression(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing relational_expression_rest rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_EQUALITY_EXPRESSION:{
			struct expression_id id1;
			n1 = equality_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing equality_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_EQUALITY_EXPRESSION_REST:{
			struct expression_id id1;
			n1 = equality_expression_rest(p1, make_expression_nullary_expression(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing equality_expression_rest rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_AND_EXPRESSION:{
			struct expression_id id1;
			n1 = and_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing and_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_AND_EXPRESSION_REST:{
			struct expression_id id1;
			n1 = and_expression_rest(p1, make_expression_nullary_expression(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing and_expression_rest rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_EXCLUSIVE_OR_EXPRESSION:{
			struct expression_id id1;
			n1 = exclusive_or_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing exclusive_or_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_EXCLUSIVE_OR_EXPRESSION_REST:{
			struct expression_id id1;
			n1 = exclusive_or_expression_rest(p1, make_expression_nullary_expression(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing exclusive_or_expression_rest rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_INCLUSIVE_OR_EXPRESSION:{
			struct expression_id id1;
			n1 = inclusive_or_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing inclusive_or_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_LOGICAL_AND_EXPRESSION:{
			struct expression_id id1;
			n1 = logical_and_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing logical_and_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_LOGICAL_OR_EXPRESSION:{
			struct expression_id id1;
			n1 = logical_or_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing logical_or_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_COMPOUND_STATEMENT:{
			struct any_statement_list_item_id id1;
			struct function_definition_helper h;
			h.for_function_definition = 0;
			compound_statement(p1, &h, 0, &id1);
			buffered_printf(&l, "Testing compound_statement rule with input '%s'.\n", str);
			break;
		}case TEST_1_CONDITIONAL_EXPRESSION:{
			struct expression_id id1;
			n1 = conditional_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing conditional_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_ASSIGNMENT_EXPRESSION:{
			struct expression_id id1;
			n1 = assignment_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing assignment_expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_EXPRESSION:{
			struct expression_id id1;
			n1 = expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing expression rule with input '%s'.\n", str);
				print_expression(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_ENUMERATOR:{
			struct enum_member_id id1;
			struct anonymous_entry_id anonymous_entry_id = make_anonymous_entry_anonymous_entry(&engine);
			struct unscoped_enum_specifier_id anonymous_id = make_unscoped_enum_specifier_anonymous(&engine, anonymous_entry_id);
			struct unscoped_tag_specifier_id anonymous_unscoped_tag_specifier_id = make_unscoped_tag_specifier_enum(&engine, anonymous_id);
			struct scoped_tag_specifier_id scoped_tag_specifier_id = make_scoped_tag_specifier_scoped_tag_specifier(&engine, get_current_scope_guid(p1), anonymous_unscoped_tag_specifier_id);

			enumerator(p1, &id1, scoped_tag_specifier_id);
			buffered_printf(&l, "Testing enumerator rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUMERATOR_LIST_REST:{
			struct enum_member_list_item_id id1;
			struct anonymous_entry_id anonymous_entry_id = make_anonymous_entry_anonymous_entry(&engine);
			struct unscoped_enum_specifier_id anonymous_id = make_unscoped_enum_specifier_anonymous(&engine, anonymous_entry_id);
			struct unscoped_tag_specifier_id anonymous_unscoped_tag_specifier_id = make_unscoped_tag_specifier_enum(&engine, anonymous_id);
			struct scoped_tag_specifier_id scoped_tag_specifier_id = make_scoped_tag_specifier_scoped_tag_specifier(&engine, get_current_scope_guid(p1), anonymous_unscoped_tag_specifier_id);
			enumerator_list_rest(p1, make_enum_member_list_item_list_start(&engine), &id1, scoped_tag_specifier_id);
			buffered_printf(&l, "Testing enumerator_list_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUMERATOR_LIST:{
			struct enum_member_list_item_id id1;
			struct anonymous_entry_id anonymous_entry_id = make_anonymous_entry_anonymous_entry(&engine);
			struct unscoped_enum_specifier_id anonymous_id = make_unscoped_enum_specifier_anonymous(&engine, anonymous_entry_id);
			struct unscoped_tag_specifier_id anonymous_unscoped_tag_specifier_id = make_unscoped_tag_specifier_enum(&engine, anonymous_id);
			struct scoped_tag_specifier_id scoped_tag_specifier_id = make_scoped_tag_specifier_scoped_tag_specifier(&engine, get_current_scope_guid(p1), anonymous_unscoped_tag_specifier_id);
			enumerator_list(p1, make_enum_member_list_item_list_start(&engine), &id1, scoped_tag_specifier_id);
			buffered_printf(&l, "Testing enumerator_list rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUM_SPECIFIER:{
			struct scoped_tag_specifier_id id1;
			n1 = enum_specifier(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing enum_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
				print_scoped_tag_specifier(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DIRECT_DECLARATOR_REST:{
			struct any_statement_list_item_id aid1;
			struct declarator_part_list_item_id id1;
			struct identifier_from_declarator ifd1;
			unsigned int is_function_definition = 0;
			unsigned int parameter_list_count = 0;
			direct_declarator_rest(p1, make_declarator_part_list_item_list_start(&engine), &id1, &ifd1, &aid1, is_function_definition, &parameter_list_count);
			buffered_printf(&l, "Testing direct_declarator_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_DIRECT_DECLARATOR:{
			struct any_statement_list_item_id aid1;
			struct declarator_part_list_item_id id1;
			struct identifier_from_declarator ifd1;
			unsigned int is_function_definition = 0;
			unsigned int parameter_list_count = 0;
			ifd1.initialized = 0;
			direct_declarator(p1, make_declarator_part_list_item_list_start(&engine), &id1, &ifd1, &aid1, is_function_definition, &parameter_list_count);
			buffered_printf(&l, "Testing direct_declarator rule with input '%s'.\n", str);
			break;
		}case TEST_1_INITIALIZER_LIST_REST:{
			struct initializer_list_item_id initializer_list_id = make_initializer_list_item_list_start(&engine);
			initializer_list_rest(p1, initializer_list_id, &initializer_list_id);
			buffered_printf(&l, "Testing initializer_list_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_INITIALIZER:{
			struct initializer_id id1;
			initializer(p1, &id1);
			buffered_printf(&l, "Testing initializer rule with input '%s'.\n", str);
			break;
		}case TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST:{
			struct declarator_part_list_item_id id1;
			direct_abstract_declarator_rest(p1, make_declarator_part_list_item_list_start(&engine), &id1);
			buffered_printf(&l, "Testing direct_abstract_declarator_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_DIRECT_ABSTRACT_DECLARATOR:{
			struct declarator_part_list_item_id id1;
			direct_abstract_declarator(p1, make_declarator_part_list_item_list_start(&engine), &id1);
			buffered_printf(&l, "Testing direct_abstract_declarator rule with input '%s'.\n", str);
			break;
		}case TEST_1_LABELED_STATEMENT:{
			struct labeled_statement_id labeled_statement_id;
			labeled_statement(p1, &labeled_statement_id);
			buffered_printf(&l, "Testing labeled_statement rule with input '%s'.\n", str);
			break;
		}case TEST_1_SELECTION_STATEMENT:{
			struct code_statement_id code_statement_id;
			selection_statement(p1, &code_statement_id);
			buffered_printf(&l, "Testing selection_statement rule with input '%s'.\n", str);
			break;
		}case TEST_1_ITERATION_STATEMENT:{
			struct code_statement_id code_statement_id;
			iteration_statement(p1, &code_statement_id);
			buffered_printf(&l, "Testing iteration_statement rule with input '%s'.\n", str);
			break;
		}case TEST_1_JUMP_STATEMENT:{
			struct jump_statement_id id1;
			jump_statement(p1, &id1);
			buffered_printf(&l, "Testing jump_statement rule with input '%s'.\n", str);
			break;
		}case TEST_1_TRANSLATION_UNIT:{
			struct any_statement_list_item_id id1;
			struct any_statement_list_item_id result_list;
			n1 = translation_unit(p1, &id1);
			result_list = add_scoped_any_statement(p1, make_any_statement_list_item_list_start(&engine), id1);
			if(n1){
				buffered_printf(&l, "Testing translation_unit rule with input '%s'.\nSuccessfully identified:\n", str);
				print_any_statement_list_item(&engine, 0, &l, result_list, 0);
				buffered_printf(&l, "\n");
				print_all_identifier_namespace_entrys(&engine, 0, &l);
				buffered_printf(&l, "\n");
				print_all_named_tag_definitions(&engine, 0, &l);
				buffered_printf(&l, "\n");
				print_all_anonymous_tag_definitions(&engine, 0, &l);
				buffered_printf(&l, "\n");
				print_all_tag_definitions(&engine, 0, &l);
				buffered_printf(&l, "\n");
				print_all_named_tag_predeclarations(&engine, 0, &l);
				buffered_printf(&l, "\n");
				print_all_scope_guids(&engine, 0, &l);
				buffered_printf(&l, "\n");
			}
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}

	if((ERROR_CHECK & checks)){
		unsigned int found = 0;
		unsigned int j;
		assert(struct_parser_error_list_size(&p1->parser_errors));
		/*  Make sure at least one of the parser errors was the one we expected. */
		buffered_printf(&l, "BEGIN list of errors that were found:\n");
		for(j = 0; j < struct_parser_error_list_size(&p1->parser_errors); j++){
			struct parser_error e = struct_parser_error_list_get(&p1->parser_errors, j);
			buffered_printf(&l, "***\n");
			print_parser_error(&l, p1, e);
			buffered_printf(&l, "***\n");
			if(e.type == error_type){
				found = 1;
			}
		}
		buffered_printf(&l, "END list of errors that were found\n");
		flush_buffer(&l);
		if(!found){
			buffered_printf(&l, "Did not find expected error.\n");
			flush_buffer(&l);
			assert(0);
		}
	}else{
		unsigned int j;
		if(struct_parser_error_list_size(&p1->parser_errors)){
			buffered_printf(&l, "Unexpected parsing errors:\n");
		}
		for(j = 0; j < struct_parser_error_list_size(&p1->parser_errors); j++){
			struct parser_error e = struct_parser_error_list_get(&p1->parser_errors, j);
			buffered_printf(&l, "***\n");
			print_parser_error(&l, p1, e);
			buffered_printf(&l, "***\n");
		}
		flush_buffer(&l);
		assert(!struct_parser_error_list_size(&p1->parser_errors));
	}
	if(!(ERROR_CHECK & checks)){
		if(!n1){
			unsigned int j;
			for(j = 0; j < struct_parser_error_list_size(&p1->parser_errors); j++){
				struct parser_error e = struct_parser_error_list_get(&p1->parser_errors, j);
				buffered_printf(&l, "***\n");
				print_parser_error(&l, p1, e);
				buffered_printf(&l, "***\n");
			}
			flush_buffer(&l);
			assert(0);
		}
	}

	buffered_printf(&l, "--------------------------\n");


	/*  Clean up testing resources. */
	for(i = 0; i < struct_parser_state_ptr_list_size(&parsers); i++){
		struct parser_state * parser_state = struct_parser_state_ptr_list_get(&parsers, i);
		decrement_scope_depth(parser_state);
		destroy_parser_state(parser_state);
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

	destroy_asm_state(&engine);
	flush_buffer(&l);
	unsigned_char_list_destroy(&l);
}

void test_1(struct memory_pool_collection * m){
	/*  Simple type specifiers */
	test_type_id("int ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("signed ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("unsigned ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("float ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("long ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("double ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("void ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("char ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("short ", m, TEST_1_TYPE_SPECIFIER);

	/*  Simple type qualifiers */
	test_type_id("volatile ", m, TEST_1_TYPE_QUALIFIER);
	test_type_id("const ", m, TEST_1_TYPE_QUALIFIER);

	/*  Simple storage class specifiers */
	test_type_id("typedef ", m, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_type_id("extern ", m, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_type_id("auto ", m, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_type_id("register ", m, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_type_id("static ", m, TEST_1_STORAGE_CLASS_SPECIFIER);


	/*  Simple Specifiers lists */
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

	/*  Identifiers */
	test_type_id("foo ", m, TEST_1_IDENTIFIER);
	test_type_id("shor ", m, TEST_1_IDENTIFIER);
	test_type_id("short_ ", m, TEST_1_IDENTIFIER);
	test_type_id("shortt ", m, TEST_1_IDENTIFIER);
	test_type_id("shorttt ", m, TEST_1_IDENTIFIER);
	test_type_id("_ ", m, TEST_1_IDENTIFIER);
	test_type_id("_1 ", m, TEST_1_IDENTIFIER);

	/*  Simple Declarators */
	test_type_id("a", m, TEST_1_DECLARATOR);
	test_type_id("*a", m, TEST_1_DECLARATOR);
	test_type_id("* const a", m, TEST_1_DECLARATOR);
	test_type_id("* volatile a", m, TEST_1_DECLARATOR);
	test_type_id("* const volatile a", m, TEST_1_DECLARATOR);

	/*  Simple decimal integer constants */
	test_type_id("37", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("1", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("9", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("123", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("2147483647", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("2147483648", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("4294967295", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("4294967296", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("9223372036854775807", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("9223372036854775808", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("18446744073709551615", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("18446744073709551616", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("170141183460469231731687303715884105727", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("170141183460469231731687303715884105728", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("340282366920938463463374607431768211455", m, TEST_1_PRIMARY_EXPRESSION);
	test_parser_error("340282366920938463463374607431768211456", m, TEST_1_PRIMARY_EXPRESSION, PARSER_ERROR_CONSTANT_OVERFLOW);
	test_parser_error("999340282366920938463463374607431768211456", m, TEST_1_PRIMARY_EXPRESSION, PARSER_ERROR_CONSTANT_OVERFLOW);

	/*  Simple octal integer constants */
	test_type_id("0", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0123", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("01234567", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("017777777777", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("020000000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("037777777777", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("040000000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0777777777777777777777", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0100000000000000000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("01777777777777777777777", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("02000000000000000000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("01777777777777777777777777777777777777777777", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("02000000000000000000000000000000000000000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("03777777777777777777777777777777777777777777", m, TEST_1_PRIMARY_EXPRESSION);
	test_parser_error("04000000000000000000000000000000000000000000", m, TEST_1_PRIMARY_EXPRESSION, PARSER_ERROR_CONSTANT_OVERFLOW);

	/*  Simple hex integer constants */
	test_type_id("0x0", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x123", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0xabcdef", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0xABCDEF", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x123456789", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x7FFFFFFF", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x80000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0xFFFFFFFF", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x100000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x7FFFFFFFFFFFFFFF", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x8000000000000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0xFFFFFFFFFFFFFFFF", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x10000000000000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0x80000000000000000000000000000000", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", m, TEST_1_PRIMARY_EXPRESSION);
	test_parser_error("0x100000000000000000000000000000000", m, TEST_1_PRIMARY_EXPRESSION, PARSER_ERROR_CONSTANT_OVERFLOW);

	/*  Simple identifier expressions */
	test_type_id_dcl("int abc;", 1, "abc ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("int abc;", 1, "abc ", m, TEST_1_UNARY_EXPRESSION);
	test_type_id_dcl("int abc;", 1, "abc ", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id_dcl("int abc;", 1, "abc ", m, TEST_1_POSTFIX_EXPRESSION);



	/*  Simple character constants */
	test_type_id("'a'", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("'b'", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("'c'", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("'d'", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("'\\n'", m, TEST_1_PRIMARY_EXPRESSION);


	test_type_id_dcl("int a;", 1, "a ", m, TEST_1_MULTIPLICATIVE_EXPRESSION);
	test_type_id_dcl("int a,b,c,d,g,f;", 1, "a*b*c* d % g / f ", m, TEST_1_MULTIPLICATIVE_EXPRESSION);

	test_type_id_dcl("int a;", 1, "a ", m, TEST_1_ADDITIVE_EXPRESSION);

	test_type_id_dcl("int a, b, c, d;", 1, "a-b-c-d ", m, TEST_1_ADDITIVE_EXPRESSION);
	test_type_id_dcl("int a, c, d;", 1, "a-3-c+d ", m, TEST_1_ADDITIVE_EXPRESSION);

	test_type_id("10 << 9 >> 123 ", m, TEST_1_SHIFT_EXPRESSION);

	test_type_id_dcl("int a,b,c,d,e,f;", 1, "a > b < c >= d <= e > f ", m, TEST_1_RELATIONAL_EXPRESSION);

	test_type_id_dcl("int a,b,c;", 1, "a == b != c ", m, TEST_1_EQUALITY_EXPRESSION);

	test_type_id_dcl("int a,b,c;", 1, "a & b & c ", m, TEST_1_AND_EXPRESSION);

	test_type_id_dcl("int a,b,c;", 1, "a ^ b ^ c ", m, TEST_1_EXCLUSIVE_OR_EXPRESSION);

	test_type_id_dcl("int a,b,c;", 1, "a | b | c ", m, TEST_1_INCLUSIVE_OR_EXPRESSION);

	test_type_id_dcl("int a,b,c;", 1, "a && b && c ", m, TEST_1_LOGICAL_AND_EXPRESSION);

	test_type_id_dcl("int a,b,c;", 1, "a || b || c ", m, TEST_1_LOGICAL_OR_EXPRESSION);

	test_type_id_dcl("int a,b,c;", 1, "a ? b : c ", m, TEST_1_CONDITIONAL_EXPRESSION);
	test_type_id_dcl("int a,b,c;", 1, "a ? b : c ? a ? b : c : a ? b : c ", m, TEST_1_CONDITIONAL_EXPRESSION);

	test_type_id_dcl("int a,b,c,d,e,f,g,h,i,j,k,l;", 1, "a = b += c -= d *= e /= f %= g <<= h >> i &= j ^= k |= l ", m, TEST_1_ASSIGNMENT_EXPRESSION);

	test_type_id("123u ", m, TEST_1_EXPRESSION);

	test_type_id_dcl("int u,v,x,y;", 1, "u,v,x,y ", m, TEST_1_EXPRESSION);

	test_type_id_dcl("int a;", 1, "&!~-+++--a", m, TEST_1_UNARY_EXPRESSION);

	test_type_id_dcl("void * p;", 1, "(void *)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("void * p;", 1, "(void **)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("void * p;", 1, "(void const **)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("void * p;", 1, "(void * const *)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("void * p;", 1, "(void * * const)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("void * p;", 1, "(void (*)(void))p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("void * p;", 1, "(void ((*)(int))(void))p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("void * p;", 1, "(void ((*(int)))(void))p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id_dcl("void * p;", 1, "(void [123][456])p ", m, TEST_1_CAST_EXPRESSION);

	test_type_id("int i; ", m, TEST_1_DECLARATION);
	test_type_id("int i = 4; ", m, TEST_1_DECLARATION);
	test_type_id_dcl("int a;", 1, "int i = a, *j, arr[45]; ", m, TEST_1_DECLARATION);
	test_type_id("int i; int j; int k; ", m, TEST_1_DECLARATION_LIST);

	test_type_id("int i; int j; int k; ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int i; char * f, d(), n[3]; long long t;", m, TEST_1_STRUCT_DECLARATION_LIST);

	test_type_id("struct foo ", m, TEST_1_TYPE_SPECIFIER);

	test_type_id("typedef int a; ", m, TEST_1_TYPEDEF);
	test_type_id("typedef int arr[3]; ", m, TEST_1_TYPEDEF);
	test_type_id("typedef int (*arr)(void), num[3], l; ", m, TEST_1_TYPEDEF);

	test_type_id("typedef int a; int k; a b; int i;", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("struct foo {int abc; int k; double d;};", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct {int abc; int k[3],f; double d;};", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("union {int a; long d;};", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("enum e {a, b, c};", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("enum {a, b = 3, c};", m, TEST_1_TRANSLATION_UNIT);

	/* c11 style anonymous inner tags */
	test_type_id("struct { struct {int i;};};", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main(int i, double d, long l) { int j; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main(struct s {int i; } a) { int j; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main(void (*a)(struct s {int i; })) { int j; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() int foo; { int j; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main(a) int foo; { int j; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main(a, b, c) int foo; long l; { int j; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("struct foo ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("union boo ", m, TEST_1_TYPE_SPECIFIER);
	test_type_id("struct goo {int i;}", m, TEST_1_TYPE_SPECIFIER);

	test_type_id("struct goo {int i;}", m, TEST_1_STRUCT_OR_UNION_SPECIFIER);
	test_type_id("struct {int i;}", m, TEST_1_STRUCT_OR_UNION_SPECIFIER);
	test_type_id("union {struct asdf * f; int j;}", m, TEST_1_STRUCT_OR_UNION_SPECIFIER);

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

	test_type_id_dcl("int d, n;", 1, "char f = 'a', d(), n[3];", m, TEST_1_DECLARATION);

	test_type_id("struct foo {int i;}; struct boo {int i;};", m, TEST_1_DECLARATION_LIST);

	test_type_id("struct foo {int i;};\nstruct foo {int i;};", m, TEST_1_DECLARATION_LIST);
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

	test_parser_error_dcl("int i;", 1, "i(", m, TEST_1_POSTFIX_EXPRESSION, PARSER_ERROR_POSTFIX_EXPRESSION_NO_REST);
	test_parser_error("++(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_AFTER_INC);
	test_parser_error("--(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_AFTER_DEC);
	test_parser_error("+(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_MISSING_CAST);
	test_parser_error("sizeof(int(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_MISSING_PAREN);
	test_parser_error("sizeof(", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_MISSING_TYPE);
	test_parser_error("sizeof ", m, TEST_1_UNARY_EXPRESSION, PARSER_ERROR_UNARY_EXPRESSION_BAD_SIZEOF);

	test_parser_error("(int))", m, TEST_1_CAST_EXPRESSION, PARSER_ERROR_CAST_EXPRESSION_MISSING_CAST);
	test_parser_error("(int(", m, TEST_1_CAST_EXPRESSION, PARSER_ERROR_CAST_EXPRESSION_MISSING_CLOSE_PAREN);
	test_parser_error("()", m, TEST_1_CAST_EXPRESSION, PARSER_ERROR_CAST_EXPRESSION_MISSING_TYPE_NAME);

	test_parser_error("/(", m, TEST_1_MULTIPLICATIVE_EXPRESSION_REST, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_EXPECTED_EXPRESSION);
	test_parser_error("/3/", m, TEST_1_MULTIPLICATIVE_EXPRESSION_REST, PARSER_ERROR_MULTIPLICATIVE_EXPRESSION_REST_NO_REST);

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

	test_parser_error("3|", m, TEST_1_INCLUSIVE_OR_EXPRESSION, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_NO_REST);

	test_parser_error("3&&", m, TEST_1_LOGICAL_AND_EXPRESSION, PARSER_ERROR_LOGICAL_AND_EXPRESSION_NO_REST);

	test_parser_error("3||", m, TEST_1_LOGICAL_OR_EXPRESSION, PARSER_ERROR_LOGICAL_OR_EXPRESSION_NO_REST);

	test_parser_error_dcl("int a,b;", 1, "{a = b;", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_1);
	test_parser_error("{int a; int b; a = b;", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_2);
	test_parser_error("{int i;", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_3);
	test_parser_error("{", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_4);

	test_parser_error("1 ? 0 :", m, TEST_1_CONDITIONAL_EXPRESSION, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_FALSE);
	test_parser_error("1 ? 0 ", m, TEST_1_CONDITIONAL_EXPRESSION, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_COLON);
	test_parser_error("1 ? ", m, TEST_1_CONDITIONAL_EXPRESSION, PARSER_ERROR_CONDITIONAL_EXPRESSION_MISSING_TRUE);

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

	test_parser_error_dcl("int f;", 1, "{f ", m, TEST_1_INITIALIZER, PARSER_ERROR_INITIALIZER_MISSING_COMMA_OR_CLOSE_BRACE);
	test_parser_error("{}", m, TEST_1_INITIALIZER, PARSER_ERROR_INITIALIZER_MISSING_INITIALIZER_LIST);

	test_parser_error("a:", m, TEST_1_STRUCT_DECLARATOR, PARSER_ERROR_STRUCT_DECLARATOR_MISSING_CONSTANT_AFTER_DECLARATOR);
	test_parser_error(":", m, TEST_1_STRUCT_DECLARATOR, PARSER_ERROR_STRUCT_DECLARATOR_MISSING_CONSTANT_NO_DECLARATOR);

	test_parser_error(", ) ", m, TEST_1_STRUCT_DECLARATOR_LIST_REST, PARSER_ERROR_STRUCT_DECLARATOR_LIST_REST_MISSING_STRUCT_DECLARATOR);
	test_parser_error(", a , ) ", m, TEST_1_STRUCT_DECLARATOR_LIST_REST, PARSER_ERROR_STRUCT_DECLARATOR_LIST_REST_MISSING_REST);

	test_parser_error("int, double, ", m, TEST_1_PARAMETER_TYPE_LIST, PARSER_ERROR_PARAMETER_TYPE_LIST_MISSING_ELLIPSIS);

	test_parser_error("struct { }", m, TEST_1_STRUCT_OR_UNION_SPECIFIER, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_STRUCT_DECLARATION_LIST);
	test_parser_error("struct { int i; ", m, TEST_1_STRUCT_OR_UNION_SPECIFIER, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_CLOSE_BRACE);



	test_parser_error("()(", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_NO_PARAM);
	test_parser_error("(int)(", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_WITH_PARAM);
	test_parser_error("(int ", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_PAREN_AFTER_PARAMS);
	test_parser_error("[](", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_NO_SIZE);
	test_parser_error("[3](", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_REST_AFTER_CLOSE_SIZED);
	test_parser_error("[3 ", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_AFTER_SIZED);
	test_parser_error("[ ", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR_REST, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_REST_MISSING_CLOSE_BRACKET_OR_CONSTANT);
	test_parser_error("()( ", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_NO_PARAM);
	test_parser_error("(int)( ", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_WITH_PARAM);
	test_parser_error("(int ", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_PAREN_AFTER_PARAMS);
	test_parser_error("(*)( ", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_ABSTRACT_DECLARATOR);
	test_parser_error("(* ", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_PAREN_AFTER_ABSTRACT_DECLARATOR);
	test_parser_error("[](", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_NO_SIZE);
	test_parser_error("[3](", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_REST_AFTER_CLOSE_SIZED);
	test_parser_error("[3", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_BRACKET_AFTER_SIZED);
	test_parser_error("[", m, TEST_1_DIRECT_ABSTRACT_DECLARATOR, PARSER_ERROR_DIRECT_ABSTRACT_DECLARATOR_MISSING_CLOSE_BRACKET_OR_CONSTANT);


	test_parser_error("asdf: ", m, TEST_1_LABELED_STATEMENT, PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_LABEL);
	test_parser_error("case 3: ", m, TEST_1_LABELED_STATEMENT, PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_CASE);
	test_parser_error("case 3 ", m, TEST_1_LABELED_STATEMENT, PARSER_ERROR_LABELED_STATEMENT_MISSING_COLON_AFTER_CASE);
	test_parser_error("case  ", m, TEST_1_LABELED_STATEMENT, PARSER_ERROR_LABELED_STATEMENT_MISSING_CONSTANT_AFTER_CASE);
	test_parser_error("default: ", m, TEST_1_LABELED_STATEMENT, PARSER_ERROR_LABELED_STATEMENT_MISSING_STATEMENT_AFTER_DEFAULT);
	test_parser_error("default ", m, TEST_1_LABELED_STATEMENT, PARSER_ERROR_LABELED_STATEMENT_MISSING_COLON_AFTER_DEFAULT);

	test_parser_error_dcl("int a;", 1, "if(1) a; else ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_ELSE);
	test_parser_error("if(1) else ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_IF);
	test_parser_error("if(1 else ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_IF);
	test_parser_error("if() else ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_EXPRESSION_AFTER_IF);
	test_parser_error("if) else ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_OPEN_PAREN_AFTER_IF);
	test_parser_error("switch (2)  ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_SWITCH);
	test_parser_error("switch (2  ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_SWITCH);
	test_parser_error("switch ()  ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_EXPRESSION_AFTER_SWITCH);
	test_parser_error("switch )  ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_OPEN_PAREN_AFTER_SWITCH);


	test_parser_error("while(1) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_WHILE_CLOSE_PAREN);
	test_parser_error("while(1 ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_WHILE);
	test_parser_error("while() ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_AFTER_WHILE);
	test_parser_error("while )", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_WHILE);
	test_parser_error_dcl("int i;", 1, "do i++; while (i) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_SEMICOLON_AFTER_DO_WHILE);
	test_parser_error_dcl("int i;", 1, "do i++; while (i ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_DO_WHILE);
	test_parser_error_dcl("int i;", 1, "do i++; while ( ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_AFTER_DO_WHILE);
	test_parser_error_dcl("int i;", 1, "do i++; while ) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_DO_WHILE);
	test_parser_error_dcl("int i;", 1, "do i++; ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_WHILE_AFTER_DO_WHILE);
	test_parser_error("do  ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_DO_WHILE);
	test_parser_error("for(;;) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_SMALL_FOR);
	test_parser_error_dcl("int i,a;", 1, "for(i=0;i<a;i++) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_BIG_FOR);
	test_parser_error_dcl("int i,a;", 1, "for(i=0;i<a;i++ ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_FOR);
	test_parser_error_dcl("int i,a;", 1, "for(i=0;i<a; ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_OR_CLOSE_PAREN_AFTER_FOR);
	test_parser_error_dcl("int i;", 1, "for(i=0;: ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_STATEMENT_2_AFTER_FOR);
	test_parser_error("for(:", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_STATEMENT_1_AFTER_FOR);
	test_parser_error("for)", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_FOR);
	test_parser_error("goto a ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_GOTO);
	test_parser_error("goto ;", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_IDENTIFIER_AFTER_GOTO);
	test_parser_error("continue ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_CONTINUE);
	test_parser_error("break ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_BREAK);
	test_parser_error_dcl("int abc;", 1, "return abc ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_RETURN);
	test_parser_error("return ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AND_EXPRESSION_AFTER_RETURN);


	test_type_id("int main() { \"abc\"; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() {\"asdf\\x66 \\0hithere\"; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() {\"\"; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {\"a\"; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {'a'; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {'\\x62'; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {'\\n'; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { int f,i,a,b,c; while(1 + f() ){ ++i++; --i.a.c.d; i->b; a[f]; f(a, b, c);} } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { int i; } double d;", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { int a; +a; -a; &a; *a; !a; ~a; sizeof a; sizeof (a); sizeof(unsigned int);  } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { int a; do{ int i; }while(a); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int a; do a++; while(a); } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { int i; for(i = 0; i < 10; i++) i++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; for(i = 0; i < 10; i++) { i++; } } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; for(i = 0; i < 10; ) i++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; for(i = 0; i < 10; ) { i++; } } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { int a; if(a) a++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int a; if(a) a++; else a--; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { int a,i; switch(a) i++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i,g,foo; case 1: i++; default: g--; abc: foo++; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("typedef int abc; int main() { (abc)1; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char * c = \"a\"\"b\"\"c\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char * c = \"a\"  \"b\" /*lol*/ \"c\";", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("const char * c = L\"a\"  u\"b\" /*lol*/ u8\"c\" U\"fun\";", m, TEST_1_TRANSLATION_UNIT);
	/*  Dangling else */
	test_type_id("int main() { int a,b,s,s2; if(a) if(b) s; else s2; }", m, TEST_1_TRANSLATION_UNIT);

	/*  This case is special because it will show if the 'sizeof(foo)' references the typedefed char or unsigned int type.  */
	test_type_id("typedef char foo; int main() { unsigned int a = sizeof(foo), foo = sizeof(foo), b = sizeof(foo); }", m, TEST_1_TRANSLATION_UNIT);

	/*  Verify incomplete structure type declaration */
	test_type_id("struct abc {int a;}; int main(void) { struct abc; struct abc * p; }", m, TEST_1_TRANSLATION_UNIT);

	/*  Struct declared in function parameter. */
	test_type_id("struct abc {int a;}; int main(struct abc {long l;} g) { struct abc p; }", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("struct abc {int a;}; int main(struct abc {long l;} g) { struct abc p; }", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { struct boo{ struct foo * f; }; struct foo{ int i; }; struct boo b; }", m, TEST_1_TRANSLATION_UNIT);

	test_type_id(" struct foo{ int j; }; int main(void){ struct foo; struct boo{ struct foo * f; }; struct foo{ int i; }; struct boo b; (void)b.f->i; return 0; } ", m, TEST_1_TRANSLATION_UNIT);


	test_type_id(" int main(void){ struct foo{ struct foo * f; }; struct foo f; (void)f; return 0; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("struct foo{ int i; }; int main(void){ typedef struct foo str; struct foo; str f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("struct foo{ int i; }; int main(void){ typedef struct foo str; struct foo; typedef str dir; dir f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("struct foo{ int j; }; int main(void){ struct boo{ struct foo f; struct foo {int i;}g; }; } ", m, TEST_1_TRANSLATION_UNIT);

	/*  Test that the inner var references the global variable. */
	test_type_id("int var = 1; int main(void){ int a = var; int var = a; return 0; }", m, TEST_1_TRANSLATION_UNIT);


	/*  Anonymous structures and unions */
	test_type_id("union { int i; }; ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct { int i; }; ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct { int i; }; ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct { int i; union {int abc;} f;}; ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct { int i; }; union {int abc;}; ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo; int main() {(struct foo *)0;}; ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo; int main(int argc){ struct foo * f = (struct foo *)0; struct foo{ int i; }; *f; } struct foo{ int j; }; int foo() {return 0;}", m, TEST_1_TRANSLATION_UNIT);

	/*  Re-define tag as different type:  */
	test_type_id("struct foo; union foo;", m, TEST_1_TRANSLATION_UNIT);


	/*  enums */
	test_type_id("enum e {a};", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("enum e {a,b};", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("enum {a,b};", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("enum e {a,b}; int main(){ enum e {b,c};} ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("enum e {a,b=34}; ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const typedef enum e {a,b=34} * f[2][3]; ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main(void){ enum e {a,b}; int f = a; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main(void){ enum e {a,b=a}; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main(void){ enum e {a,b,c=a*b}; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main(void){ struct abc { enum e {a,b=sizeof(a)} g; }; }", m, TEST_1_TRANSLATION_UNIT);

	/*  Declarators, examples from http://blog.robertelder.org/building-a-c-compiler-type-system-the-formidable-declarator/ */
	test_type_id("int x = x;", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main(void) { int a; int b = a; int c = 3, f, *k = & a, r(); return 0; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("void (*f(int a))(char){ }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("void ((*f)(int a))(char){ }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int * array_of_3_pointer[3]; int * (*(get_array_of_3_pointer(double d)))[3]{ return &array_of_3_pointer; } int * (*(*(get_fcn_ptr_that_returns_array_of_3_pointer)(char c))(double))[3]{ return get_array_of_3_pointer; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int a(){ int foo(int *, double **); (void *)0x99; sizeof(struct foo [20]); }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int a(){ int * j; int (* k); int * l[2]; int (* m)[2]; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("void take_int_ptr(int (*)); void take_int_ptr(int  * ); void take_int_ptr(int (*i)){ (void)i; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int *((((((((*j)))))))); int * (*((((((((*(((((*foo)))))(char)))))))))(double))[3]; int (i); int (arr[4])[3];", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("void take_fcn_ptr(void (void)); void take_fcn_ptr(void (*)(void)); void take_fcn_ptr(void (*fun)(void)){ (void)fun; }", m, TEST_1_TRANSLATION_UNIT);

	/*  initializers: */
	test_type_id("void main() { int arr[4] = {0,1,2,3}; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("void main() { int arr[] = {0,1,2,3}; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("void main() { int a = 2; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("void main() { int arr[2][3] = {{1,2,3},{1,2,4}}; }", m, TEST_1_TRANSLATION_UNIT);

	/* These cases are currently not supported and will result in parse errors: */  
	/* test_type_id("int f(());", m, TEST_1_TRANSLATION_UNIT); */
	/* test_type_id("int f((((((((((((((((((((((((((((((((((()))))))))))))))))))))))))))))))))));", m, TEST_1_TRANSLATION_UNIT); */
	test_type_id("int f(int ());", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int f(int (*g)(void (*)(int), int)){}", m, TEST_1_TRANSLATION_UNIT);
	/*  Make sure identifiers that aren't parameters don't get declared. */
	test_type_id("void foo(int a, void (*b)(char c)) {int c;}", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("void foo(enum e {x,y,z} a, struct abc {enum e m;} b){ }", m, TEST_1_TRANSLATION_UNIT);

	/*  Typedefs and structs, examples from http://blog.robertelder.org/magical-world-of-structs-typedefs-scoping/ */
	test_type_id("struct foo{ int i; }; int main(void){ struct foo a; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo; struct foo{ int i; }; int main(void){ struct foo a; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo; int main(void){ struct foo a; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct boo{ struct foo * f; }; struct foo{ int i; }; int main(void){ struct boo b; (void)b.f->i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int j; }; struct boo{ struct foo * f; }; struct foo{ int i; }; int main(void){ struct boo b; (void)b.f->i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main(void){ struct boo{ struct foo * f; }; struct foo{ int i; }; struct boo b; (void)b.f->i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int j; }; int main(void){ struct boo{ struct foo * f; }; struct foo{ int i; }; struct boo b; (void)b.f->i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int j; }; int main(void){ struct foo; struct boo{ struct foo * f; }; struct foo{ int i; }; struct boo b; (void)b.f->i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int i; }; int main(void){ struct boo{ struct foo * f; }; struct foo{ int i; }; struct boo b; (void)b.f->i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int j; }; int main(void){ struct foo ty; struct boo{ struct foo * f; }; struct foo{ int i; }; struct boo b; (void)b.f->i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int j; }; int main(void){ struct boo{ struct foo {int k;} abc; struct foo * f; }; struct foo{ int i; }; struct boo b; (void)b.f->i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ union foo * i; }; int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ struct boo * b; }; int main(void){ { struct boo{ int i; }; struct foo f1; (void)f1.b->i; } { struct boo{ int j; }; struct foo f2; (void)f2.b->j; } return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ struct boo * b; }; int main(void){ { struct foo; struct boo{ int i; }; struct foo f1; (void)f1.b->i; } { struct foo; struct boo{ int j; }; struct foo f2; (void)f2.b->j; } return 0; }", m, TEST_1_TRANSLATION_UNIT);
	/* This case is not currently supported due to requirements of c99 style mixture of declarations and statements. */
	/*test_type_id("int main(void){ struct foo; { struct foo f1; } struct foo{ int i; }; return 0; }", m, TEST_1_TRANSLATION_UNIT);*/
	test_type_id("int main(void){ struct foo{ struct foo * f; }; struct foo f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int i; }; int main(void){ struct foo; typedef struct foo str; str f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	/* This case is not currently supported due to requirements of c99 style mixture of declarations and statements. */
	/*test_type_id("struct foo{ int i; }; int main(void){ (struct foo*)0; typedef struct foo str; str f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);*/
	test_type_id("struct foo{ int i; }; int main(void){ typedef struct foo str; struct foo; str f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int i; }; int main(void){ typedef struct foo abc; typedef struct foo def; abc g; def h; (void)g; (void)h; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int i; }; int main(void){ typedef struct foo str; struct foo; typedef str dir; dir f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int i; }; typedef struct foo str; int main(void){ str; struct foo f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo; typedef struct foo str; str{ int i; }; int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int foo(struct boo {int a;}); int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main(void){ struct foo {int i;} (*a[3])(int (int (int (int)))); (void)a; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int i; }; int main(void){ const struct foo; struct foo f; (void)f; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("typedef struct foo {int i;} koo(struct foo); int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_parser_error("typedef struct foo { typedef struct boo {int i;}; }; int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_NO_DECLARATIONS);
	test_type_id("typedef struct foo str; int main(void){ struct foo{ int i; }; str f; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int j; }; int main(void){ struct boo{ struct foo f; struct foo {int i;}g; }; struct boo b; (void)b.f.j; (void)b.g.i; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int j; }; int main(void){ struct boo{ struct foo; struct foo g; }; struct boo b; (void)b.g.j; return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo {int i;}; int main(void){ struct foo {struct foo * f; int j;}; struct foo a; return a.f->j; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("union foo {int i;}; int main(void){ struct foo * f; struct foo {int k;}; struct foo a; return a.k; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ void (*f)(struct foo); }; int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int foo(struct foo {int i;} a, int (* b)(struct foo {double j;} c)){ struct foo lol; } int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("typedef char foo; int main(void) { unsigned int a = sizeof(foo), foo=sizeof(foo), b=sizeof(foo); return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int i; }; typedef struct foo type1; typedef type1 type2; typedef type2 type1; int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo{ int i; }; typedef struct foo type1; int main(void){ struct foo { int j;}; typedef struct foo type2; typedef type1 type2; /* Error here. */ return 0; }", m, TEST_1_TRANSLATION_UNIT);


	/*  Simple jump statements: */
	test_type_id("void main() { return; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { return 0 ; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { forever: goto forever; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { for(;;) continue; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { for(;;) break; }", m, TEST_1_TRANSLATION_UNIT);


	/*  Examples from http://blog.robertelder.org/switch-statements-statement-expressions/  */
	test_type_id("int main() { int i = 8; switch(i){ case 0:{ break; }case 1:{ break; }case 2:{ break; }default:{ } }  }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i = 2; switch(i){ i++; default:{ } i++; case 0:{ case 3: i; } if(i < 10){ case 1:{ break; } for(i=0; i < 10; i++){ case 2:; } } } }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i = 3; if(i == 0) goto label_0; if(i == 1) goto label_1; if(i == 2) goto label_2; if(i == 3) goto label_3; /*  Otherwise, go to default label */ goto label_default; { i++; label_default:{ } i++; label_0:{ label_3: i; } if(i < 10){ label_1:{ goto break_from_switch; } for(i=0; i < 10; i++){ label_2:; } } } break_from_switch:; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {int total_bytes = 2; int n = (total_bytes + 3) / 4; int * to; int * from; switch (total_bytes % 4) { case 0: do { *to = *from++; case 3:      *to = *from++; case 2:      *to = *from++; case 1:      *to = *from++; } while (--n > 0); }} ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; switch(0) i++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; switch(0) switch(0) switch(0) switch(0) switch(0) switch(0); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; switch(0) case 0:; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i,j,k; switch(0) case 0: for(i = 0; i < 10; i++) case 1: for(j = 0; j < 10; j++) case 2: for(k = 0; k < 10; k++); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i,j,k; switch(0){ case 0:{ for(i = 0; i < 10; i++){ case 1:{ while(j){ case 2:{ for(k = 0; k < 10; k++){ } } j++; } } } }case 3:{ /*...*/ }default:{ /*...*/ } } } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; switch(i) default: case 0: case 1: case 2: case 3:; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; switch(0){ case 1:{ break; }case 2:{ break; }case 3: case 4: case 5:{ break; } }  } ", m, TEST_1_TRANSLATION_UNIT);
	/*  The blog post has other examples that use statement expressions, but this compiler doesn't currently support statement expressions. */


	/*   Stuff that involves sizeof */
	test_type_id("int main() { int i; int j = sizeof(i); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i; int j = sizeof(int); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i = sizeof(struct {int i;}); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i = sizeof(struct {int i; int arr[sizeof(union {double d;})]; }); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { struct abc { int i:sizeof(int);}; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { int i = sizeof(sizeof(sizeof(sizeof(1)))); } ", m, TEST_1_TRANSLATION_UNIT);

	/*   K & R style function definitions: */
	test_type_id("int a() { } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int a(a,b,c) { } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int a(a,b,c) int a; int b; int c; { return c; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int a(a,b,c) int a; { return c; } ", m, TEST_1_TRANSLATION_UNIT);

	/*  Recursive function definitions: */
	test_type_id(" void r(void) { r(); } ", m, TEST_1_TRANSLATION_UNIT);

	/*   Uses of typenames: */
	test_type_id("int a() { typedef int a; int j = sizeof(a); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("typedef struct foo {int i;} a; int main(void){ a * b; b->i = 0; return 0; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("typedef int node; static node one = 1; int main(void){ typedef struct foo {int i;} node; node n; n.i = one; return n.i; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("typedef int type1; typedef int type1; typedef type1 type2; typedef type2 type1; typedef double type1; int main(void){ return 0; }", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct foo {int i;}; typedef struct foo type1; int main(void){ struct foo {int j;}; typedef struct foo type2; typedef struct foo type3; typedef type3 type2; typedef type1 type2; return 0; }", m, TEST_1_TRANSLATION_UNIT);


	/* C11 struct/union */
	test_type_id("struct lol { union { struct { unsigned int c:23, b:8, a:1; } f; float g; }; };", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct lol { union { int i; }; } s;", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("struct lol { union { union { int i; }; }; } s;", m, TEST_1_TRANSLATION_UNIT);

	/* Character constants */
	test_type_id("const char str = \"\\x00\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\\x09\\x0A\\x0B\\x0C\\x0D\\x0E\\x0F\\x10\\x11\\x12\\x13\\x14\\x15\\x16\\x17\\x18\\x19\\x1A\\x1B\\x1C\\x1D\\x1E\\x1F\\x20\\x21\\x22\\x23\\x24\\x25\\x26\\x27\\x28\\x29\\x2A\\x2B\\x2C\\x2D\\x2E\\x2F\\x30\\x31\\x32\\x33\\x34\\x35\\x36\\x37\\x38\\x39\\x3A\\x3B\\x3C\\x3D\\x3E\\x3F\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char str = \"\\x40\\x41\\x42\\x43\\x44\\x45\\x46\\x47\\x48\\x49\\x4A\\x4B\\x4C\\x4D\\x4E\\x4F\\x50\\x51\\x52\\x53\\x54\\x55\\x56\\x57\\x58\\x59\\x5A\\x5B\\x5C\\x5D\\x5E\\x5F\\x60\\x61\\x62\\x63\\x64\\x65\\x66\\x67\\x68\\x69\\x6A\\x6B\\x6C\\x6D\\x6E\\x6F\\x70\\x71\\x72\\x73\\x74\\x75\\x76\\x77\\x78\\x79\\x7A\\x7B\\x7C\\x7D\\x7E\\x7F\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char str = \"\\x80\\x81\\x82\\x83\\x84\\x85\\x86\\x87\\x88\\x89\\x8A\\x8B\\x8C\\x8D\\x8E\\x8F\\x90\\x91\\x92\\x93\\x94\\x95\\x96\\x97\\x98\\x99\\x9A\\x9B\\x9C\\x9D\\x9E\\x9F\\xA0\\xA1\\xA2\\xA3\\xA4\\xA5\\xA6\\xA7\\xA8\\xA9\\xAA\\xAB\\xAC\\xAD\\xAE\\xAF\\xB0\\xB1\\xB2\\xB3\\xB4\\xB5\\xB6\\xB7\\xB8\\xB9\\xBA\\xBB\\xBC\\xBD\\xBE\\xBF\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char str = \"\\xC0\\xC1\\xC2\\xC3\\xC4\\xC5\\xC6\\xC7\\xC8\\xC9\\xCA\\xCB\\xCC\\xCD\\xCE\\xCF\\xD0\\xD1\\xD2\\xD3\\xD4\\xD5\\xD6\\xD7\\xD8\\xD9\\xDA\\xDB\\xDC\\xDD\\xDE\\xDF\\xE0\\xE1\\xE2\\xE3\\xE4\\xE5\\xE6\\xE7\\xE8\\xE9\\xEA\\xEB\\xEC\\xED\\xEE\\xEF\\xF0\\xF1\\xF2\\xF3\\xF4\\xF5\\xF6\\xF7\\xF8\\xF9\\xFA\\xFB\\xFC\\xFD\\xFE\\xFF\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char str = \"\\x0\\x1\\x2\\x3\\x4\\x5\\x6\\x7\\x8\\x9\\xA\\xB\\xC\\xD\\xE\\xF\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char str = \"\\'\\\"\\?\\\\\\a\\b\\f\\n\\r\\t\\v\";", m, TEST_1_TRANSLATION_UNIT);

	/*  Octal constants 3 0 pad: */
	test_type_id("const char str = \"\\000\\001\\002\\003\\004\\005\\006\\007\\010\\011\\012\\013\\014\\015\\016\\017\\020\\021\\022\\023\\024\\025\\026\\027\\030\\031\\032\\033\\034\\035\\036\\037\\040\\041\\042\\043\\044\\045\\046\\047\\050\\051\\052\\053\\054\\055\\056\\057\\060\\061\\062\\063\\064\\065\\066\\067\\070\\071\\072\\073\\074\\075\\076\\077\\100\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char str = \"\\101\\102\\103\\104\\105\\106\\107\\110\\111\\112\\113\\114\\115\\116\\117\\120\\121\\122\\123\\124\\125\\126\\127\\130\\131\\132\\133\\134\\135\\136\\137\\140\\141\\142\\143\\144\\145\\146\\147\\150\\151\\152\\153\\154\\155\\156\\157\\160\\161\\162\\163\\164\\165\\166\\167\\170\\171\\172\\173\\174\\175\\176\\177\\200\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char str = \"\\201\\202\\203\\204\\205\\206\\207\\210\\211\\212\\213\\214\\215\\216\\217\\220\\221\\222\\223\\224\\225\\226\\227\\230\\231\\232\\233\\234\\235\\236\\237\\240\\241\\242\\243\\244\\245\\246\\247\\250\\251\\252\\253\\254\\255\\256\\257\\260\\261\\262\\263\\264\\265\\266\\267\\270\\271\\272\\273\\274\\275\\276\\277\\300\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char str = \"\\301\\302\\303\\304\\305\\306\\307\\310\\311\\312\\313\\314\\315\\316\\317\\320\\321\\322\\323\\324\\325\\326\\327\\330\\331\\332\\333\\334\\335\\336\\337\\340\\341\\342\\343\\344\\345\\346\\347\\350\\351\\352\\353\\354\\355\\356\\357\\360\\361\\362\\363\\364\\365\\366\\367\\370\\371\\372\\373\\374\\375\\376\\377\";", m, TEST_1_TRANSLATION_UNIT);

	/*  Octal constants 2 0 pad: */
	test_type_id("const char str = \"\\00\\01\\02\\03\\04\\05\\06\\07\\10\\11\\12\\13\\14\\15\\16\\17\\20\\21\\22\\23\\24\\25\\26\\27\\30\\31\\32\\33\\34\\35\\36\\37\\40\\41\\42\\43\\44\\45\\46\\47\\50\\51\\52\\53\\54\\55\\56\\57\\60\\61\\62\\63\\64\\65\\66\\67\\70\\71\\72\\73\\74\\75\\76\\77\";", m, TEST_1_TRANSLATION_UNIT);
	/*  Octal constants 1 0 pad: */
	test_type_id("const char str = \"\\0\\1\\2\\3\\4\\5\\6\\7\";", m, TEST_1_TRANSLATION_UNIT);
}



int main(void){
	struct memory_pool_collection m;
	memory_pool_collection_create(&m);
	heap_memory_pool_create(&m);

	struct_regex_computation_node_memory_pool_create(&m);
	struct_regex_parser_node_memory_pool_create(&m);
	struct_c_lexer_token_memory_pool_create(&m);
	struct_parser_node_memory_pool_create(&m);
	c_token_matcher_create(&m);

	test_1(&m);
	heap_memory_pool_destroy(&m);

	struct_c_lexer_token_memory_pool_destroy(&m);
	struct_parser_node_memory_pool_destroy(&m);

	c_token_matcher_destroy(&m);

	struct_regex_parser_node_memory_pool_destroy(&m);
	struct_regex_computation_node_memory_pool_destroy(&m);

	memory_pool_collection_destroy(&m);

	return 0;
}
