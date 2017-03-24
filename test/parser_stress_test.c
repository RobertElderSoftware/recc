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

void test_1(struct memory_pool_collection *);
void test_type_id(const char * str, struct memory_pool_collection *, enum ID_TYPE);
void test_parser_error(const char * str, struct memory_pool_collection *, enum ID_TYPE, enum parser_error_type);
void grammar_rule_test(const char * str, struct memory_pool_collection *, enum ID_TYPE, unsigned int error_check, enum parser_error_type);
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
	struct asm_state engine;
	unsigned int i;
	struct parser_state * p1;
	struct parser_node * n1;

	struct_parser_state_ptr_list_create(&parsers);
	struct_c_lexer_state_ptr_list_create(&lexers);
	struct_unsigned_char_list_ptr_list_create(&chrs);

	create_asm_state(&engine, m);

	p1 = setup_parser(str, &engine, m, &chrs, &lexers, &parsers);

	unsigned_char_list_create(&l);
	buffered_printf(&l, "--------------------------\n");
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
				print_specifier_or_qualifier_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_SPECIFIER_QUALIFIER_LIST:{
			struct specifier_or_qualifier_list_item_id id1;
			n1 = specifier_qualifier_list(p1, make_specifier_or_qualifier_list_item_list_start(p1->syntax_model), &id1);
			if(n1){
				buffered_printf(&l, "Testing specifier_qualifier_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_specifier_or_qualifier_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_TYPE_QUALIFIER_LIST:{
			struct specifier_or_qualifier_list_item_id id1;
			n1 = type_qualifier_list(p1, make_specifier_or_qualifier_list_item_list_start(p1->syntax_model), &id1);
			if(n1){
				buffered_printf(&l, "Testing type_qualifier_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_specifier_or_qualifier_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DECLARATOR:{
			struct any_statement_list_item_id aid1;
			struct declarator_part_list_item_id id1;
			struct identifier_from_declarator ifd1;
			ifd1.initialized = 0;
			n1 = declarator(p1, make_declarator_part_list_item_list_start(p1->syntax_model), &id1, &ifd1, &aid1, 0);
			if(n1){
				buffered_printf(&l, "Testing declarator rule with input '%s'.\nSuccessfully identified:\n", str);
				buffered_printf(&l, "Logical view:  ");
				print_declarator_part_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
				buffered_printf(&l, "Vsual view:  ");
				/*print_declarator_in_visual_order(&engine, &l, id1, 0);*/
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
				/*print_declarator_in_spiral_rule_order(&engine, &l, id1);*/
				buffered_printf(&l, "\n");
				/*print_declarator_in_visual_order(&engine, &l, id1, 0);*/
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
			n1 = parameter_list(p1, &id1, &aid1, 0);
			if(n1){
				buffered_printf(&l, "Testing parameter_list rule with input '%s'.\nSuccessfully identified:\n", str);
				buffered_printf(&l, "(");
				print_general_type_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, ")\n");
			}
			break;
		}case TEST_1_PARAMETER_TYPE_LIST:{
			struct parameter_list_id id1;
			struct any_statement_list_item_id aid1;
			n1 = parameter_type_list(p1, &id1, &aid1, 0);
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
				print_struct_or_union_member_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_STRUCT_DECLARATION_LIST:{
			struct struct_or_union_member_list_item_id id1;
			struct struct_or_union_member_list_item_id member_list_start = make_struct_or_union_member_list_item_list_start(&engine);
			n1 = struct_declaration_list(p1, member_list_start, &id1);
			if(n1){
				buffered_printf(&l, "Testing struct_declaration_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_struct_or_union_member_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, "\n");
			}
			break;
		}case TEST_1_DECLARATION:{
			struct any_statement_list_item_id id1;
			n1 = declaration(p1, make_any_statement_list_item_list_start(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing declaration rule with input '%s'.\nSuccessfully identified:\n", str);
				print_any_statement_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, ";\n");
			}
			break;
		}case TEST_1_TYPEDEF:{
			struct any_statement_list_item_id id1;
			n1 = declaration(p1, make_any_statement_list_item_list_start(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing typedef declaration rule with input '%s'.\nSuccessfully identified:\n", str);
				print_any_statement_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, "All typedef entries:\n", str);
				print_all_identifier_namespace_entrys(&engine, 0, &l);
				buffered_printf(&l, ";\n");
			}
			break;
		}case TEST_1_DECLARATION_LIST:{
			struct any_statement_list_item_id id1;
			n1 = declaration_list(p1, make_any_statement_list_item_list_start(&engine), &id1);
			if(n1){
				buffered_printf(&l, "Testing declaration_list rule with input '%s'.\nSuccessfully identified:\n", str);
				print_any_statement_list_item(&engine, 0, &l, id1);
				buffered_printf(&l, ";\n");
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
		}case TEST_1_ADDITIVE_EXPRESSION:{
			struct expression_id id1;
			n1 = additive_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing additive_expression rule with input '%s'.\n", str);
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
		}case TEST_1_RELATIONAL_EXPRESSION:{
			struct expression_id id1;
			n1 = relational_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing relational_expression rule with input '%s'.\n", str);
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
		}case TEST_1_AND_EXPRESSION:{
			struct expression_id id1;
			n1 = and_expression(p1, &id1);
			if(n1){
				buffered_printf(&l, "Testing and_expression rule with input '%s'.\n", str);
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
			compound_statement(p1, 0, &id1);
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
			enumerator(p1, &id1);
			buffered_printf(&l, "Testing enumerator rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUMERATOR_LIST_REST:{
			struct enum_member_list_item_id id1;
			enumerator_list_rest(p1, make_enum_member_list_item_list_start(&engine), &id1);
			buffered_printf(&l, "Testing enumerator_list_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_ENUMERATOR_LIST:{
			struct enum_member_list_item_id id1;
			enumerator_list(p1, make_enum_member_list_item_list_start(&engine), &id1);
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
			direct_declarator_rest(p1, make_declarator_part_list_item_list_start(&engine), &id1, &ifd1, &aid1, 0);
			buffered_printf(&l, "Testing direct_declarator_rest rule with input '%s'.\n", str);
			break;
		}case TEST_1_DIRECT_DECLARATOR:{
			struct any_statement_list_item_id aid1;
			struct declarator_part_list_item_id id1;
			struct identifier_from_declarator ifd1;
			ifd1.initialized = 0;
			direct_declarator(p1, make_declarator_part_list_item_list_start(&engine), &id1, &ifd1, &aid1, 0);
			buffered_printf(&l, "Testing direct_declarator rule with input '%s'.\n", str);
			break;
		}case TEST_1_INITIALIZER_LIST_REST:{
			initializer_list_rest(p1);
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
				print_any_statement_list_item(&engine, 0, &l, result_list);
				buffered_printf(&l, "All identifier namespace entries:\n", str);
				print_all_identifier_namespace_entrys(&engine, 0, &l);
				buffered_printf(&l, "All named tag entries:\n", str);
				print_all_named_tag_definitions(&engine, 0, &l);
				buffered_printf(&l, "All anonymous tag entries:\n", str);
				print_all_anonymous_tag_definitions(&engine, 0, &l);
				buffered_printf(&l, "All named tag predeclaration entries:\n", str);
				print_all_named_tag_predeclarations(&engine, 0, &l);
				buffered_printf(&l, "\n");
			}
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}

	if(ERROR_CHECK & checks){
		unsigned int found = 0;
		unsigned int j;
		assert(struct_parser_error_list_size(&p1->parser_errors));
		/*  Make sure at least one of the parser errors was the one we expected. */
		for(j = 0; j < struct_parser_error_list_size(&p1->parser_errors); j++){
			struct parser_error e = struct_parser_error_list_get(&p1->parser_errors, j);
			buffered_printf(&l, "***\n");
			print_parser_error(&l, p1, e);
			buffered_printf(&l, "***\n");
			if(e.type == error_type){
				found = 1;
				break;
			}
		}
		assert(found);
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
	test_type_id("3777777777777777777777777777777777777777777", m, TEST_1_PRIMARY_EXPRESSION);
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
	test_type_id("0x100000000000000000000000000000000", m, TEST_1_PRIMARY_EXPRESSION);

	/*  Simple identifier expressions */
	test_type_id("abc ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("abc ", m, TEST_1_UNARY_EXPRESSION);
	test_type_id("abc ", m, TEST_1_PRIMARY_EXPRESSION);
	test_type_id("abc ", m, TEST_1_POSTFIX_EXPRESSION);


	test_type_id("a ", m, TEST_1_MULTIPLICATIVE_EXPRESSION);
	test_type_id("a*b*c* d % g / f ", m, TEST_1_MULTIPLICATIVE_EXPRESSION);

	test_type_id("a ", m, TEST_1_ADDITIVE_EXPRESSION);

	test_type_id("a-b-c-d ", m, TEST_1_ADDITIVE_EXPRESSION);
	test_type_id("a-3-c+d ", m, TEST_1_ADDITIVE_EXPRESSION);

	test_type_id("10 << 9 >> 123 ", m, TEST_1_SHIFT_EXPRESSION);

	test_type_id("a > b < c >= d <= e > f ", m, TEST_1_RELATIONAL_EXPRESSION);

	test_type_id("a == b != c ", m, TEST_1_EQUALITY_EXPRESSION);

	test_type_id("a & b & c ", m, TEST_1_AND_EXPRESSION);

	test_type_id("a ^ b ^ c ", m, TEST_1_EXCLUSIVE_OR_EXPRESSION);

	test_type_id("a | b | c ", m, TEST_1_INCLUSIVE_OR_EXPRESSION);

	test_type_id("a && b && c ", m, TEST_1_LOGICAL_AND_EXPRESSION);

	test_type_id("a || b || c ", m, TEST_1_LOGICAL_OR_EXPRESSION);

	test_type_id("a ? b : c ", m, TEST_1_CONDITIONAL_EXPRESSION);
	test_type_id("a ? b : c ? a ? b : c : a ? b : c ", m, TEST_1_CONDITIONAL_EXPRESSION);

	test_type_id("a = b += c -= d *= e /= f %= g <<= h >> i &= j ^= k |= l ", m, TEST_1_ASSIGNMENT_EXPRESSION);

	test_type_id("123u ", m, TEST_1_EXPRESSION);

	test_type_id("u,v,x,y ", m, TEST_1_EXPRESSION);

	test_type_id("&!~-+++--a", m, TEST_1_UNARY_EXPRESSION);

	test_type_id("(void *)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("(void **)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("(void const **)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("(void * const *)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("(void * * const)p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("(void (*)(void))p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("(void ((*)(int))(void))p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("(void ((*(int)))(void))p ", m, TEST_1_CAST_EXPRESSION);
	test_type_id("(void [123][456])p ", m, TEST_1_CAST_EXPRESSION);

	test_type_id("int i; ", m, TEST_1_DECLARATION);
	test_type_id("int i = 4; ", m, TEST_1_DECLARATION);
	test_type_id("int i = a, *j, arr[45]; ", m, TEST_1_DECLARATION);
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
	test_type_id("/*c11 style anonymous inner tags*/\nstruct { struct {int i;};};", m, TEST_1_TRANSLATION_UNIT);

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

	/*
	test_type_id("char f = 'a', d(), n[3];", m, TEST_1_DECLARATION);
	*/

	test_type_id("struct foo {int i;}; struct boo {int i;};", m, TEST_1_DECLARATION_LIST);

	test_parser_error("struct foo {int i;};\nstruct foo {int i;};", m, TEST_1_DECLARATION_LIST, PARSER_ERROR_STRUCT_OR_UNION_REDEFINITION);
	test_parser_error("struct foo {int i;", m, TEST_1_DECLARATION_LIST, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_MISSING_BRACE);
	test_parser_error("struct foo {}", m, TEST_1_DECLARATION_LIST, PARSER_ERROR_STRUCT_OR_UNION_SPECIFIER_NO_DECLARATIONS);

	test_parser_error("(123", m, TEST_1_PRIMARY_EXPRESSION, PARSER_ERROR_PRIMARY_EXPRESSION_MISSING_CLOSE_PAREN);
	test_parser_error("a = ;", m, TEST_1_INIT_DECLARATOR, PARSER_ERROR_INIT_DECLARATOR_MISSING_INITIALIZER);

	test_parser_error("1," , m, TEST_1_ARGUMENT_EXPRESSION_LIST, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_MISSING_ARGUMENT);
	test_parser_error(",3,,", m, TEST_1_ARGUMENT_EXPRESSION_LIST_REST, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_REST_MISSING_ARGUMENT);
	test_parser_error(",4,,", m, TEST_1_ARGUMENT_EXPRESSION_LIST_REST, PARSER_ERROR_ARGUMENT_EXPRESSION_LIST_REST_MISSING_ASSIGNMENT_EXPRESSION);

	/*
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
	*/

	test_parser_error("3|", m, TEST_1_INCLUSIVE_OR_EXPRESSION, PARSER_ERROR_INCLUSIVE_OR_EXPRESSION_NO_REST);

	test_parser_error("3&&", m, TEST_1_LOGICAL_AND_EXPRESSION, PARSER_ERROR_LOGICAL_AND_EXPRESSION_NO_REST);

	test_parser_error("3||", m, TEST_1_LOGICAL_OR_EXPRESSION, PARSER_ERROR_LOGICAL_OR_EXPRESSION_NO_REST);

	test_parser_error("{a = b;", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_1);
	test_parser_error("{int i; a = b;", m, TEST_1_COMPOUND_STATEMENT, PARSER_ERROR_COMPOUND_STATEMENT_MISSING_BRACE_2);
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

	test_parser_error("{f ", m, TEST_1_INITIALIZER, PARSER_ERROR_INITIALIZER_MISSING_COMMA_OR_CLOSE_BRACE);
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

	test_parser_error("if(1) a; else ", m, TEST_1_SELECTION_STATEMENT, PARSER_ERROR_SELECTION_STATEMENT_MISSING_STATEMENT_AFTER_ELSE);
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
	test_parser_error("do i++; while (i) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_SEMICOLON_AFTER_DO_WHILE);
	test_parser_error("do i++; while (i ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_DO_WHILE);
	test_parser_error("do i++; while ( ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_AFTER_DO_WHILE);
	test_parser_error("do i++; while ) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_DO_WHILE);
	test_parser_error("do i++; ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_WHILE_AFTER_DO_WHILE);
	test_parser_error("do  ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_DO_WHILE);
	test_parser_error("for(;;) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_SMALL_FOR);
	test_parser_error("for(i=0;i<a;i++) ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_STATEMENT_AFTER_BIG_FOR);
	test_parser_error("for(i=0;i<a;i++ ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_CLOSE_PAREN_AFTER_FOR);
	test_parser_error("for(i=0;i<a; ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_OR_CLOSE_PAREN_AFTER_FOR);
	test_parser_error("for(i=0;: ", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_STATEMENT_2_AFTER_FOR);
	test_parser_error("for(:", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_EXPRESSION_STATEMENT_1_AFTER_FOR);
	test_parser_error("for)", m, TEST_1_ITERATION_STATEMENT, PARSER_ERROR_ITERATION_STATEMENT_MISSING_OPEN_PAREN_AFTER_FOR);
	test_parser_error("goto a ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_GOTO);
	test_parser_error("goto ;", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_IDENTIFIER_AFTER_GOTO);
	test_parser_error("continue ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_CONTINUE);
	test_parser_error("break ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_BREAK);
	test_parser_error("return abc ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AFTER_RETURN);
	test_parser_error("return ", m, TEST_1_JUMP_STATEMENT, PARSER_ERROR_JUMP_STATEMENT_MISSING_SEMICOLON_AND_EXPRESSION_AFTER_RETURN);


	test_type_id("int main() { \"abc\"; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() {\"asdf\\x66 \\0hithere\"; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() {\"\"; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {\"a\"; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {'a'; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {'\\x62'; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() {'\\n'; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { while(1 + f() ){ ++i++; --i.a.c.d; i->b; a[f]; f(a, b, c);} } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { int i; } double d;", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { +a; -a; &a; *a; !a; ~a; sizeof a; sizeof (a); sizeof(unsigned int);  } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { do{ int i; }while(a); } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { do a++; while(a); } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { for(i = 0; i < 10; i++) i++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { for(i = 0; i < 10; i++) { i++; } } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { for(i = 0; i < 10; ) i++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { for(i = 0; i < 10; ) { i++; } } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { if(a) a++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { if(a) a++; else a--; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("int main() { switch(a) i++; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("int main() { case 1: i++; default: g--; abc: foo++; } ", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("typedef int abc; int main() { (abc)1; } ", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char * c = \"a\"\"b\"\"c\";", m, TEST_1_TRANSLATION_UNIT);
	test_type_id("const char * c = \"a\"  \"b\" /*lol*/ \"c\";", m, TEST_1_TRANSLATION_UNIT);

	test_type_id("const char * c = L\"a\"  u\"b\" /*lol*/ u8\"c\" U\"fun\";", m, TEST_1_TRANSLATION_UNIT);
	/*  Dangling else */
	test_type_id("int main() { if(a) if(b) s; else s2; }", m, TEST_1_TRANSLATION_UNIT);

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

	test_type_id(" struct foo{ int j; }; int main(void){ struct boo{ struct foo f; struct foo {int i;}g; }; } ", m, TEST_1_TRANSLATION_UNIT);
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
