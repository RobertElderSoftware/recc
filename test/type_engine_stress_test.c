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
#include "../data-structures/struct_c_lexer_state_ptr_list.h"
#include "../data-structures/struct_parser_state_ptr_list.h"
#include "../data-structures/struct_unsigned_char_list_ptr_list.h"
#include "../recc-implementation/lexer.h"
#include "../recc-implementation/parser.h"

enum ID_TYPE {
	TEST_1_TYPE_QUALIFIER,
	TEST_1_TYPE_SPECIFIER,
	TEST_1_STORAGE_CLASS_SPECIFIER,
	TEST_1_DECLARATION_SPECIFIERS,
	TEST_1_SPECIFIER_QUALIFIER_LIST,
	TEST_1_TYPE_QUALIFIER_LIST,
	TEST_1_ABSTRACT_DECLARATOR,
	TEST_1_DECLARATOR,
	TEST_1_STRUCT_DECLARATOR,
	TEST_1_INIT_DECLARATOR,
	TEST_1_PARAMETER_LIST,
	TEST_1_STRUCT_DECLARATION,
	TEST_1_STRUCT_DECLARATION_LIST,
	TEST_1_DECLARATION
};

void test_grammar_rule(const char * str, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *, enum ID_TYPE);
struct parser_state * setup_parser(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_type_specifier(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_type_qualifier(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_storage_class_specifier(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_declaration_specifiers(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_type_qualifier_list(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_specifier_qualifier_list(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_declarator(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_abstract_declarator(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_struct_declarator(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_init_declarator(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_parameter_list(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_identifier(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
void test_1(struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_struct_declaration(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_struct_declaration_list(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);
unsigned int test_declaration(const char *, struct type_engine_state *, struct memory_pool_collection *, struct struct_unsigned_char_list_ptr_list *, struct struct_c_lexer_state_ptr_list *, struct struct_parser_state_ptr_list *);

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


unsigned int test_type_specifier(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(type_specifier(p1, &id1));
	assert(type_specifier(p2, &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_type_qualifier(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(type_qualifier(p1, &id1));
	assert(type_qualifier(p2, &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_storage_class_specifier(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(storage_class_specifier(p1, &id1));
	assert(storage_class_specifier(p2, &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_identifier(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(get_identifier_node(p1, &id1));
	assert(get_identifier_node(p2, &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_declaration_specifiers(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(declaration_specifiers(p1, get_type_engine_id_for_specifier_or_qualifier_list_begin(p1->type_engine), &id1));
	assert(declaration_specifiers(p2, get_type_engine_id_for_specifier_or_qualifier_list_begin(p2->type_engine), &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_specifier_qualifier_list(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(specifier_qualifier_list(p1, get_type_engine_id_for_specifier_or_qualifier_list_begin(p1->type_engine), &id1));
	assert(specifier_qualifier_list(p2, get_type_engine_id_for_specifier_or_qualifier_list_begin(p2->type_engine), &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_type_qualifier_list(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(type_qualifier_list(p1, get_type_engine_id_for_specifier_or_qualifier_list_begin(p1->type_engine), &id1));
	assert(type_qualifier_list(p2, get_type_engine_id_for_specifier_or_qualifier_list_begin(p2->type_engine), &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_declarator(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(declarator(p1, get_type_engine_id_for_declarator_part_list_begin(p1->type_engine), &id1));
	assert(declarator(p2, get_type_engine_id_for_declarator_part_list_begin(p2->type_engine), &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_abstract_declarator(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(abstract_declarator(p1, get_type_engine_id_for_declarator_part_list_begin(p1->type_engine), &id1));
	assert(abstract_declarator(p2, get_type_engine_id_for_declarator_part_list_begin(p2->type_engine), &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_init_declarator(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(init_declarator(p1, &id1));
	assert(init_declarator(p2, &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_struct_declarator(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(struct_declarator(p1, &id1));
	assert(struct_declarator(p2, &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_parameter_list(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(parameter_list(p1, &id1));
	assert(parameter_list(p2, &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_struct_declaration(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(struct_declaration(p1, get_type_engine_id_for_specifier_or_qualifier_list_begin(engine), &id1));
	assert(struct_declaration(p2, get_type_engine_id_for_specifier_or_qualifier_list_begin(engine), &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_struct_declaration_list(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(struct_declaration_list(p1, get_type_engine_id_for_specifier_or_qualifier_list_begin(engine), &id1));
	assert(struct_declaration_list(p2, get_type_engine_id_for_specifier_or_qualifier_list_begin(engine), &id2));
	assert(id1 == id2);
	return id1;
}

unsigned int test_declaration(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	struct parser_state * p1 = setup_parser(str, engine, m, chrs, lexers, parsers);
	struct parser_state * p2 = setup_parser(str, engine, m, chrs, lexers, parsers);
	unsigned int id1;
	unsigned int id2;
	/*  Try parsing: */
	assert(declaration(p1, &id1));
	assert(declaration(p2, &id2));
	assert(id1 == id2);
	return id1;
}

void test_grammar_rule(const char * str, struct type_engine_state * engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers, enum ID_TYPE type){
	struct unsigned_char_list l;
	unsigned char * c;
	unsigned_char_list_create(&l);
	buffered_printf(&l, "--------------------------\n");
	switch(type){
		case TEST_1_TYPE_SPECIFIER:{
			unsigned int n = test_type_specifier(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing type_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
			print_type_specifier_details(engine, &l, unsigned_int_list_get(&engine->all_type_specifiers, n), TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_TYPE_QUALIFIER:{
			unsigned int n = test_type_qualifier(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing type_qualifier rule with input '%s'.\nSuccessfully identified:\n", str);
			print_type_qualifier(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_STORAGE_CLASS_SPECIFIER:{
			unsigned int n = test_storage_class_specifier(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing storage_class_specifier rule with input '%s'.\nSuccessfully identified:\n", str);
			print_storage_class_specifier(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_DECLARATION_SPECIFIERS:{
			unsigned int n = test_declaration_specifiers(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing declaration_specifiers rule with input '%s'.\nSuccessfully identified:\n", str);
			print_specifier_or_qualifier_list(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_SPECIFIER_QUALIFIER_LIST:{
			unsigned int n = test_specifier_qualifier_list(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing specifier_qualifier_list rule with input '%s'.\nSuccessfully identified:\n", str);
			print_specifier_or_qualifier_list(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_TYPE_QUALIFIER_LIST:{
			unsigned int n = test_type_qualifier_list(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing type_qualifier_list rule with input '%s'.\nSuccessfully identified:\n", str);
			print_specifier_or_qualifier_list(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_DECLARATOR:{
			unsigned int n = test_declarator(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing declarator rule with input '%s'.\nSuccessfully identified:\n", str);
			print_declarator_in_spiral_rule_order(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			print_declarator_in_visual_order(engine, &l, n, TYPE_ENGINE_NORMAL, 0);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_ABSTRACT_DECLARATOR:{
			unsigned int n = test_abstract_declarator(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing abstract_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
			print_declarator_in_spiral_rule_order(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			print_declarator_in_visual_order(engine, &l, n, TYPE_ENGINE_NORMAL, 0);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_STRUCT_DECLARATOR:{
			unsigned int n = test_struct_declarator(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing struct_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
			print_bitfield_or_declarator(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_INIT_DECLARATOR:{
			unsigned int n = test_init_declarator(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing init_declarator rule with input '%s'.\nSuccessfully identified:\n", str);
			print_bitfield_or_declarator(engine, &l, n, TYPE_ENGINE_NORMAL);
			buffered_printf(&l, "\n");
			break;
		}case TEST_1_PARAMETER_LIST:{
			unsigned int n = test_parameter_list(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing parameter_list rule with input '%s'.\nSuccessfully identified:\n", str);
			buffered_printf(&l, "(");
			print_general_type_list(engine, (unsigned char *)", ", &l, n, TYPE_ENGINE_NORMAL, 0);
			buffered_printf(&l, ")\n");
			break;
		}case TEST_1_STRUCT_DECLARATION:{
			unsigned int n = test_struct_declaration(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing struct_declaration rule with input '%s'.\nSuccessfully identified:\n", str);
			print_general_type_list(engine, (unsigned char *)";\n", &l, n, TYPE_ENGINE_NORMAL, 0);
			buffered_printf(&l, ";\n");
			break;
		}case TEST_1_STRUCT_DECLARATION_LIST:{
			unsigned int n = test_struct_declaration_list(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing struct_declaration_list rule with input '%s'.\nSuccessfully identified:\n", str);
			print_general_type_list(engine, (unsigned char *)";\n", &l, n, TYPE_ENGINE_NORMAL, 0);
			buffered_printf(&l, ";\n");
			break;
		}case TEST_1_DECLARATION:{
			unsigned int n = test_declaration(str, engine, m, chrs, lexers, parsers);
			buffered_printf(&l, "Testing declaration rule with input '%s'.\nSuccessfully identified:\n", str);
			print_general_type_list(engine, (unsigned char *)";\n", &l, n, TYPE_ENGINE_NORMAL, 0);
			buffered_printf(&l, ";\n");
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
	buffered_printf(&l, "--------------------------\n");
	c = unsigned_char_list_data(&l);
	while(*c){
		putchar(*c);
		c++;
	}
	unsigned_char_list_destroy(&l);
}

void test_1(struct type_engine_state * type_engine, struct memory_pool_collection * m, struct struct_unsigned_char_list_ptr_list * chrs, struct struct_c_lexer_state_ptr_list * lexers, struct struct_parser_state_ptr_list * parsers){
	test_grammar_rule("int ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);
	test_grammar_rule("signed ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);
	test_grammar_rule("unsigned ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);
	test_grammar_rule("float ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);
	test_grammar_rule("long ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);
	test_grammar_rule("double ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);
	test_grammar_rule("void ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);
	test_grammar_rule("char ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);
	test_grammar_rule("short ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_SPECIFIER);

	test_grammar_rule("volatile ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_QUALIFIER);
	test_grammar_rule("const ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_QUALIFIER);

	test_grammar_rule("typedef ", type_engine, m, chrs, lexers, parsers, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_grammar_rule("extern ", type_engine, m, chrs, lexers, parsers, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_grammar_rule("auto ", type_engine, m, chrs, lexers, parsers, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_grammar_rule("register ", type_engine, m, chrs, lexers, parsers, TEST_1_STORAGE_CLASS_SPECIFIER);
	test_grammar_rule("static ", type_engine, m, chrs, lexers, parsers, TEST_1_STORAGE_CLASS_SPECIFIER);

	test_identifier("foo ", type_engine, m, chrs, lexers, parsers);
	test_identifier("shor ", type_engine, m, chrs, lexers, parsers);
	test_identifier("short_ ", type_engine, m, chrs, lexers, parsers);
	test_identifier("shortt ", type_engine, m, chrs, lexers, parsers);
	test_identifier("shorttt ", type_engine, m, chrs, lexers, parsers);
	test_identifier("_ ", type_engine, m, chrs, lexers, parsers);
	test_identifier("_1 ", type_engine, m, chrs, lexers, parsers);

	test_grammar_rule("const int ", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATION_SPECIFIERS);
	test_grammar_rule("static const int ", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATION_SPECIFIERS);
	test_grammar_rule("extern const int ", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATION_SPECIFIERS);
	test_grammar_rule("extern int ", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATION_SPECIFIERS);
	test_grammar_rule("const double ", type_engine, m, chrs, lexers, parsers, TEST_1_SPECIFIER_QUALIFIER_LIST);
	test_grammar_rule("const long long ", type_engine, m, chrs, lexers, parsers, TEST_1_SPECIFIER_QUALIFIER_LIST);
	test_grammar_rule("const int ", type_engine, m, chrs, lexers, parsers, TEST_1_SPECIFIER_QUALIFIER_LIST);
	test_grammar_rule("const const const const const ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_QUALIFIER_LIST);
	test_grammar_rule("const volatile const ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_QUALIFIER_LIST);
	test_grammar_rule("const ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_QUALIFIER_LIST);
	test_grammar_rule("volatile ", type_engine, m, chrs, lexers, parsers, TEST_1_TYPE_QUALIFIER_LIST);

	test_grammar_rule("(******const a[4][5])(void)", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("(*a)(void)", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("*(*a)[3]", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("*(* const volatile*a)[3][]", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("(*(*a)(void))(void)", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("(*(*a)(void))[3]", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("(*(*a)[3])(void)", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("(*((*a))[3])(void)", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("(*((*a)[3]))(void)", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);
	test_grammar_rule("(*(*a[3]))(void)", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATOR);

	test_grammar_rule("(******const [4][5])(void)", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("(*)(void)", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("*(*)[3]", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("*(* const volatile*)[3][]", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("(*(*)(void))(void)", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("(*(*)(void))[3]", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("(*(*)[3])(void)", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("(*((*))[3])(void)", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("(*((*)[3]))(void)", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);
	test_grammar_rule("(*(*[3]))(void)", type_engine, m, chrs, lexers, parsers, TEST_1_ABSTRACT_DECLARATOR);

	test_grammar_rule("(*(*a[3]))(void)", type_engine, m, chrs, lexers, parsers, TEST_1_INIT_DECLARATOR);
	test_grammar_rule("*fff", type_engine, m, chrs, lexers, parsers, TEST_1_INIT_DECLARATOR);
	test_grammar_rule("fff[9]", type_engine, m, chrs, lexers, parsers, TEST_1_INIT_DECLARATOR);
	test_grammar_rule("fff(void)", type_engine, m, chrs, lexers, parsers, TEST_1_INIT_DECLARATOR);
	test_grammar_rule("fff()", type_engine, m, chrs, lexers, parsers, TEST_1_INIT_DECLARATOR);
	test_grammar_rule("fff(a,b,c)", type_engine, m, chrs, lexers, parsers, TEST_1_INIT_DECLARATOR);
	test_grammar_rule("abc:3", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATOR);
	test_grammar_rule(":44", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATOR);

	test_grammar_rule("int i, int k[3], void (*k)(double)", type_engine, m, chrs, lexers, parsers, TEST_1_PARAMETER_LIST);
	test_grammar_rule("void ", type_engine, m, chrs, lexers, parsers, TEST_1_PARAMETER_LIST);
	test_grammar_rule("int, int ", type_engine, m, chrs, lexers, parsers, TEST_1_PARAMETER_LIST);

	test_grammar_rule("double asdf;", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATION);
	test_grammar_rule("int i, j[4], *k;", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATION);
	test_grammar_rule("long i;", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATION);

	test_grammar_rule("long i;", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATION_LIST);
	test_grammar_rule("double i; int i; float arr[2], j(), *t;", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATION_LIST);
	test_grammar_rule("int i; float t;", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATION_LIST);
	test_grammar_rule("int i; char * f, d(), n[3]; long long t;", type_engine, m, chrs, lexers, parsers, TEST_1_STRUCT_DECLARATION_LIST);

	test_grammar_rule("char f = 'a', d(), n[3];", type_engine, m, chrs, lexers, parsers, TEST_1_DECLARATION);
}

int main(void){
	struct struct_unsigned_char_list_ptr_list character_lists;
	struct struct_c_lexer_state_ptr_list lexers;
	struct struct_parser_state_ptr_list parsers;
	struct memory_pool_collection m;
	struct type_engine_state type_engine;
	unsigned int i;
	struct_parser_state_ptr_list_create(&parsers);
	struct_c_lexer_state_ptr_list_create(&lexers);
	struct_unsigned_char_list_ptr_list_create(&character_lists);

	memory_pool_collection_create(&m);
	create_type_engine_state(&type_engine, &m);
	test_1(&type_engine, &m, &character_lists, &lexers, &parsers);

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

	for(i = 0; i < struct_unsigned_char_list_ptr_list_size(&character_lists); i++){
		unsigned_char_list_destroy(struct_unsigned_char_list_ptr_list_get(&character_lists, i));
		free(struct_unsigned_char_list_ptr_list_get(&character_lists, i));
	}
	struct_unsigned_char_list_ptr_list_destroy(&character_lists);

	destroy_type_engine_state(&type_engine);
	memory_pool_collection_destroy(&m);
	return 0;
}
