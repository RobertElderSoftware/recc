/*
    Copyright 2019 Robert Elder Software Inc.
    
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
#include "../../recc-implementation/regex_engine.h"
#include "../../recc-implementation/lexer.h"
#include "../../recc-implementation/memory_pool_collection.h"
#include "../../generated/struct_type_description_memory_pool.h"
#include "../../generated/struct_linker_symbol_memory_pool.h"
#include "../../generated/struct_c_lexer_token_memory_pool.h"
#include "../../generated/struct_parser_node_memory_pool.h"
#include "../../generated/struct_l2_parser_node_memory_pool.h"
#include "../../generated/struct_l2_lexer_token_memory_pool.h"
#include "../../generated/struct_l2_item_memory_pool.h"
#include "../../generated/struct_c_lexer_token_memory_pool.h"


enum example_tokens {
	EX_SPACE,
	EX_NEWLINE,
	EX_COMMENT,
	EX_AUTO,
	EX_BREAK,
	EX_CASE,
	EX_CHAR,
	EX_CONST,
	EX_CONTINUE,
	EX_DEFAULT,
	EX_DO,
	EX_DOUBLE,
	EX_ELSE,
	EX_ENUM,
	EX_EXTERN,
	EX_FLOAT,
	EX_FOR,
	EX_GOTO,
	EX_IF,
	EX_INT,
	EX_LONG,
	EX_REGISTER,
	EX_RETURN,
	EX_SHORT,
	EX_SIGNED,
	EX_SIZEOF,
	EX_STATIC,
	EX_STRUCT,
	EX_SWITCH,
	EX_TYPEDEF,
	EX_UNION,
	EX_UNSIGNED,
	EX_VOID,
	EX_VOLATILE,
	EX_WHILE,
	EX_IDENTIFIER,
	EX_CONSTANT_HEX,
	EX_CONSTANT_EXPONENT,
	EX_CONSTANT_FLOAT_SMALL,
	EX_CONSTANT_FLOAT_LARGE,
	EX_CONSTANT_DECIMAL,
	EX_CONSTANT_CHARACTER,
	EX_STRING_LITERAL,
	EX_ELLIPSIS,
	EX_RIGHT_ASSIGN,
	EX_LEFT_ASSIGN,
	EX_ADD_ASSIGN,
	EX_SUB_ASSIGN,
	EX_MUL_ASSIGN,
	EX_DIV_ASSIGN,
	EX_MOD_ASSIGN,
	EX_AND_ASSIGN,
	EX_XOR_ASSIGN,
	EX_OR_ASSIGN,
	EX_RIGHT_OP,
	EX_LEFT_OP,
	EX_INC_OP,
	EX_DEC_OP,
	EX_PTR_OP,
	EX_AND_OP,
	EX_OR_OP,
	EX_LE_OP,
	EX_GE_OP,
	EX_EQ_OP,
	EX_NE_OP,
	EX_SEMICOLON_CHAR,
	EX_OPEN_BRACE_CHAR,
	EX_CLOSE_BRACE_CHAR,
	EX_OPEN_SQUARE_BRACKET_CHAR,
	EX_CLOSE_SQUARE_BRACKET_CHAR,
	EX_COMMA_CHAR,
	EX_COLON_CHAR,
	EX_EQUALS_CHAR,
	EX_OPEN_PAREN_CHAR,
	EX_CLOSE_PAREN_CHAR,
	EX_DOT_CHAR,
	EX_AMPERSAND_CHAR,
	EX_EXCLAMATION_MARK_CHAR,
	EX_TILDE_CHAR,
	EX_MINUS_CHAR,
	EX_PLUS_CHAR,
	EX_MULTIPLY_CHAR,
	EX_DIVIDE_CHAR,
	EX_PERCENT_CHAR,
	EX_OPEN_ANGLE_BRACKET_CHAR,
	EX_CLOSE_ANGLE_BRACKET_CHAR,
	EX_CARET_CHAR,
	EX_PIPE_CHAR,
	EX_QUESTION_MARK_CHAR,
	EX_NUMBER_SIGN_CHAR
};


#define NUM_EXAMPLE_TOKENS 90

static const char * example_token_names[NUM_EXAMPLE_TOKENS] = {
	"EX_SPACE",
	"EX_NEWLINE",
	"EX_COMMENT",
	"EX_AUTO",
	"EX_BREAK",
	"EX_CASE",
	"EX_CHAR",
	"EX_CONST",
	"EX_CONTINUE",
	"EX_DEFAULT",
	"EX_DO",
	"EX_DOUBLE",
	"EX_ELSE",
	"EX_ENUM",
	"EX_EXTERN",
	"EX_FLOAT",
	"EX_FOR",
	"EX_GOTO",
	"EX_IF",
	"EX_INT",
	"EX_LONG",
	"EX_REGISTER",
	"EX_RETURN",
	"EX_SHORT",
	"EX_SIGNED",
	"EX_SIZEOF",
	"EX_STATIC",
	"EX_STRUCT",
	"EX_SWITCH",
	"EX_TYPEDEF",
	"EX_UNION",
	"EX_UNSIGNED",
	"EX_VOID",
	"EX_VOLATILE",
	"EX_WHILE",
	"EX_IDENTIFIER",
	"EX_CONSTANT_HEX",
	"EX_CONSTANT_EXPONENT",
	"EX_CONSTANT_FLOAT_SMALL",
	"EX_CONSTANT_FLOAT_LARGE",
	"EX_CONSTANT_DECIMAL",
	"EX_CONSTANT_CHARACTER",
	"EX_STRING_LITERAL",
	"EX_ELLIPSIS",
	"EX_RIGHT_ASSIGN",
	"EX_LEFT_ASSIGN",
	"EX_ADD_ASSIGN",
	"EX_SUB_ASSIGN",
	"EX_MUL_ASSIGN",
	"EX_DIV_ASSIGN",
	"EX_MOD_ASSIGN",
	"EX_AND_ASSIGN",
	"EX_XOR_ASSIGN",
	"EX_OR_ASSIGN",
	"EX_RIGHT_OP",
	"EX_LEFT_OP",
	"EX_INC_OP",
	"EX_DEC_OP",
	"EX_PTR_OP",
	"EX_AND_OP",
	"EX_OR_OP",
	"EX_LE_OP",
	"EX_GE_OP",
	"EX_EQ_OP",
	"EX_NE_OP",
	"EX_SEMICOLON_CHAR",
	"EX_OPEN_BRACE_CHAR",
	"EX_CLOSE_BRACE_CHAR",
	"EX_OPEN_SQUARE_BRACKET_CHAR",
	"EX_CLOSE_SQUARE_BRACKET_CHAR",
	"EX_COMMA_CHAR",
	"EX_COLON_CHAR",
	"EX_EQUALS_CHAR",
	"EX_OPEN_PAREN_CHAR",
	"EX_CLOSE_PAREN_CHAR",
	"EX_DOT_CHAR",
	"EX_AMPERSAND_CHAR",
	"EX_EXCLAMATION_MARK_CHAR",
	"EX_TILDE_CHAR",
	"EX_MINUS_CHAR",
	"EX_PLUS_CHAR",
	"EX_MULTIPLY_CHAR",
	"EX_DIVIDE_CHAR",
	"EX_PERCENT_CHAR",
	"EX_OPEN_ANGLE_BRACKET_CHAR",
	"EX_CLOSE_ANGLE_BRACKET_CHAR",
	"EX_CARET_CHAR",
	"EX_PIPE_CHAR",
	"EX_QUESTION_MARK_CHAR",
	"EX_NUMBER_SIGN_CHAR"
};

void test_1(struct memory_pool_collection *);
void test_2(struct memory_pool_collection *);
void test_3(struct memory_pool_collection *);
void test_4(struct memory_pool_collection *);
void test_5(struct memory_pool_collection *);
void test_6(struct memory_pool_collection *);
void test_7(struct memory_pool_collection *);
void test_8(struct memory_pool_collection *);
void mini_lexer_test(struct memory_pool_collection *);
void test_regex_compile_error(struct memory_pool_collection *, const char *, enum regex_compiler_error_type);
void add_lexer_test_regex(struct memory_pool_collection *, struct regex_compiler_state ***, const char *, enum example_tokens, unsigned int *);

int do_test_case(struct memory_pool_collection *, unsigned int, const char **, unsigned int *, unsigned int *, const char *);
void cleanup_test_case(unsigned int, struct regex_compiler_state **, struct regex_matcher_state *);

void cleanup_test_case(unsigned int num_regexes, struct regex_compiler_state ** compiler_states, struct regex_matcher_state * matcher){
	unsigned int i;
	for(i = 0; i < num_regexes; i++){
		regex_compiler_state_destroy(compiler_states[i]);
		free((void*)compiler_states[i]);
	}
	free((void*)compiler_states);
	regex_matcher_state_destroy(matcher);
}


void test_regex_compile_error(struct memory_pool_collection * m, const char * c, enum regex_compiler_error_type type){
	struct regex_compiler_state compiler_state;
	regex_compiler_state_create(&compiler_state, m, (unsigned char *)c, 0);
	regex_compiler_state_compile(&compiler_state);
	if(compiler_state.errors){
		printf("Correctly detect an error from compiling this regex.\n");
		if(type == compiler_state.errors->type){
			printf("Error was expected type.\n");
		}else{
			printf("Error was not expected type for regex '%s'.\n", c);
			assert(0);
		}
	}else{
		printf("Expected a regex compile error here for regex '%s'.\n", c);
		assert(0);
	}
	regex_compiler_state_destroy(&compiler_state);
}

int do_test_case(struct memory_pool_collection * m, unsigned int num_regexes, const char ** regexes, unsigned int * should_match, unsigned int * match_length, const char * subject){
	struct regex_compiler_state ** compiler_states = (struct regex_compiler_state **)malloc(sizeof(struct regex_compiler_state *) * num_regexes);
	struct regex_matcher_state matcher;
	unsigned int i;

	for(i = 0; i < num_regexes; i++){
		compiler_states[i] = (struct regex_compiler_state *)malloc(sizeof(struct regex_compiler_state));

		printf("Performing test case with regex \"%s\"\n", regexes[i]);
		regex_compiler_state_create(compiler_states[i], m, (unsigned char *)regexes[i], i);
		regex_compiler_state_compile(compiler_states[i]);
		if(compiler_states[i]->errors){
			assert(0 && "Should not encounter errors here.");
		}
	}

	regex_matcher_state_create(&matcher, compiler_states, num_regexes);
	regex_matcher_state_run(&matcher, (unsigned char *)subject, (unsigned int)strlen(subject));

	printf("Checking match results against text '%s':\n", subject);
	/*  For each regex */
	for(i = 0; i < num_regexes; i++){
		unsigned int j;
		/*  If there is a match in the list of matches... */
		for(j = 0; j < matcher.num_unique_matches_found; j++){
			unsigned match_id = matcher.actual_matches[j]->match_node->match_id;
			if(match_id == i){ /*  If this match belongs to the regex we want to check for */
				if(should_match[match_id]){
					unsigned int k;
					if(!(matcher.actual_matches[j]->length == match_length[match_id])){
						printf("Expect match length to be %u, but it was %u: ", match_length[match_id], matcher.actual_matches[j]->length);
						cleanup_test_case(num_regexes, compiler_states, &matcher);
						return 1;
					}
					printf("Pattern '%s' correctly matched text '", regexes[match_id]);
					for(k = 0; k < matcher.actual_matches[j]->length; k++){
						printf("%c",((unsigned char *)subject)[k]);
					}
					printf("'\n");
				}else{
					printf("Pattern '%s' matched, but shouldn't have.\n", regexes[match_id]);
					cleanup_test_case(num_regexes, compiler_states, &matcher);
					return 1;
				}
				break; /*  Terminate early */
			}
		}
		if(j == matcher.num_unique_matches_found){ /*  Did not find any match for this 'i' value */
			if(should_match[i]){
				printf("Pattern '%s' did not match, but was expected to.\n", regexes[i]);
				cleanup_test_case(num_regexes, compiler_states, &matcher);
				return 1;
			}else{
				printf("Pattern '%s' did not match, as expected.\n", regexes[i]);
			}
		}
	}

	cleanup_test_case(num_regexes, compiler_states, &matcher);
	return 0;
}

static const char * regexes1[4] =      {"b?a?a?a?baaa", "baaa", "foo", "ba[^a-zA-Z0-9_]"};
static unsigned int does_match1[4] =   {             1,      1,     0,                 0};
static unsigned int match_length1[4] = {             4,      4,     0,                 0};

void test_1(struct memory_pool_collection * m){
	assert(!do_test_case(m, 4, regexes1, does_match1, match_length1, "baaa"));
}

/*
static const char * regexes2[3] =      {"b?", "", "a"};
static unsigned int does_match2[3] =   {   1,  1,   0};
static unsigned int match_length2[3] = {   0,  0,   0};

void test_2(struct memory_pool_collection * m){
	assert(!do_test_case(m, 3, regexes2, does_match2, match_length2, ""));
}
*/

static const char * regexes3[8] =      {"[\\x00-\\xfF]", "[a-f]", "[a]", "[k]", "[k-zt-u]", "[k-za-u]+",      "[.]+",      ".+"};
static unsigned int does_match3[8] =   {              1,       1,     1,     0,          0,           1,           1,         1};
static unsigned int match_length3[8] = {              1,       1,     1,     0,          0,           3,           3,         3};

void test_3(struct memory_pool_collection * m){
	assert(!do_test_case(m, 8, regexes3, does_match3, match_length3, "abc"));
}

static const char * regexes4[2] =      {"\\n\\t\\(\\)\\[\\]\\+\\?\\*\\.\\-\\^\\|\\\\x", "[\\n\\t\\(\\)\\[\\]\\+\\?\\*\\.\\-\\^\\|\\\\x]+"};
static unsigned int does_match4[2] =   {                                               1,                                                   1};
static unsigned int match_length4[2] = {                                              15,                                                  15};

void test_4(struct memory_pool_collection * m){
	assert(!do_test_case(m, 2, regexes4, does_match4, match_length4, "\n\t()[]+?*.-^|\\x"));
}

static const char * regexes5[1] =       {"<[^ ]+ [^ ]+ *= *\"[^\"]*\" *>"};
static unsigned int does_match5[1] =    {                               1};
static unsigned int does_not_match5[1] ={                               0};
static unsigned int match_length5[1] =  {                              15};

void test_5(struct memory_pool_collection * m){
	assert(!do_test_case(m, 1, regexes5, does_match5, match_length5, "<abc foo=\"lol\">"));
	assert(!do_test_case(m, 1, regexes5, does_not_match5, match_length5, "<abc foo\"lol\">"));
	assert(!do_test_case(m, 1, regexes5, does_not_match5, match_length5, "<abc foo=\"lol\""));
	assert(!do_test_case(m, 1, regexes5, does_not_match5, match_length5, "<abcfoo=\"lol\">"));
	assert(!do_test_case(m, 1, regexes5, does_not_match5, match_length5, "<abc foo=lol\">"));
}

/*  Asymptotic complexity test:  This will take 2^100 time if we used backtracking. (We don't) */
static const char * regexes6[1] =      {"a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?a?"};
static unsigned int does_match6[1] =   {                                1};
static unsigned int match_length6[1] = {                              100};

void test_6(struct memory_pool_collection * m){
	assert(!do_test_case(m, 1, regexes6, does_match6, match_length6, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
}

static const char * regexes7[8] =      {"(dog|foo)", "(foo|cat|dog)", "(foo|dog|cat)", "(|dog)", "(dog|)", "(dogen|def)",      "(abc|def)",      "(abc|dogen)"};
static unsigned int does_match7[8] =   {          1,               1,               1,     1,          1,              0,                0,                  0};
static unsigned int match_length7[8] = {          3,               3,               3,     3,          3,              0,                0,                  0};

void test_7(struct memory_pool_collection * m){
	assert(!do_test_case(m, 8, regexes7, does_match7, match_length7, "dogasdf"));
}

static const char * regexes8[10] =      {"aaa(lol)", "aaa(lol)f", "aaa(lol)f?", "aaa(lol)f+", "aaa(lol)f.", "aaa((l)ol)f", "aaa(l(o)l)f","aaa(lol(f))f", "aaa((l))o", "aaa((l))"};
static unsigned int does_match8[10] =   {         1,           1,            1,            1,            1,              1,            1,             0,           1,          1};
static unsigned int match_length8[10] = {         6,           7,            7,            7,            8,              7,            7,             0,           5,          4};

void test_8(struct memory_pool_collection * m){
	assert(!do_test_case(m, 10, regexes8, does_match8, match_length8, "aaalolfk"));
	assert(!do_test_case(m, 10, regexes8, does_match8, match_length8, "aaalolf2345"));
}

void add_lexer_test_regex(struct memory_pool_collection * m, struct regex_compiler_state *** compiler_states, const char * c, enum example_tokens t, unsigned int * count){
	struct regex_compiler_state * comp = (struct regex_compiler_state *)malloc(sizeof(struct regex_compiler_state));
	regex_compiler_state_create(comp, m, (unsigned char *)c, (unsigned int)t);
	regex_compiler_state_compile(comp);
	assert(!comp->errors);
	*count = *count + 1;
	*compiler_states = (struct regex_compiler_state **)realloc((void*)*compiler_states, (*count) * sizeof(struct regex_compiler_state *));
	(*compiler_states)[*count -1] = comp;
}

static const char * sample_c = "int main(void) { /* A comment* */ unsigned int abc = 0; unsigned int def = 0; return abc==def; } /* A comment \n* \n */ auto break case char const continue default do double else enum extern float for goto if int long register return short signed sizeof static struct switch typedef union unsigned void volatile while foobar 0xfabe 4E2 .99 4.4f 33 'a' \"abc\" \"def\" ... >>= <<= += -= *= /= %= &= ^= |= >> << ++ -- -> && || <= >= == != ; { } [ ] , : = ( ) . & ! ~ - + * / % < > ^ | ? #";

#define NUM_EXPECTED_TYPES 113

static enum example_tokens expected_types[NUM_EXPECTED_TYPES] = {EX_INT, EX_IDENTIFIER, EX_OPEN_PAREN_CHAR, EX_VOID, EX_CLOSE_PAREN_CHAR, EX_OPEN_BRACE_CHAR, EX_COMMENT, EX_UNSIGNED, EX_INT, EX_IDENTIFIER, EX_EQUALS_CHAR, EX_CONSTANT_DECIMAL, EX_SEMICOLON_CHAR, EX_UNSIGNED, EX_INT, EX_IDENTIFIER, EX_EQUALS_CHAR, EX_CONSTANT_DECIMAL, EX_SEMICOLON_CHAR, EX_RETURN, EX_IDENTIFIER, EX_EQ_OP, EX_IDENTIFIER, EX_SEMICOLON_CHAR, EX_CLOSE_BRACE_CHAR, EX_COMMENT, EX_AUTO, EX_BREAK, EX_CASE, EX_CHAR, EX_CONST, EX_CONTINUE, EX_DEFAULT, EX_DO, EX_DOUBLE, EX_ELSE, EX_ENUM, EX_EXTERN, EX_FLOAT, EX_FOR, EX_GOTO, EX_IF, EX_INT, EX_LONG, EX_REGISTER, EX_RETURN, EX_SHORT, EX_SIGNED, EX_SIZEOF, EX_STATIC, EX_STRUCT, EX_SWITCH, EX_TYPEDEF, EX_UNION, EX_UNSIGNED, EX_VOID, EX_VOLATILE, EX_WHILE, EX_IDENTIFIER, EX_CONSTANT_HEX, EX_CONSTANT_EXPONENT, EX_CONSTANT_FLOAT_SMALL, EX_CONSTANT_FLOAT_LARGE, EX_CONSTANT_DECIMAL, EX_CONSTANT_CHARACTER, EX_STRING_LITERAL, EX_ELLIPSIS, EX_RIGHT_ASSIGN, EX_LEFT_ASSIGN, EX_ADD_ASSIGN, EX_SUB_ASSIGN, EX_MUL_ASSIGN, EX_DIV_ASSIGN, EX_MOD_ASSIGN, EX_AND_ASSIGN, EX_XOR_ASSIGN, EX_OR_ASSIGN, EX_RIGHT_OP, EX_LEFT_OP, EX_INC_OP, EX_DEC_OP, EX_PTR_OP, EX_AND_OP, EX_OR_OP, EX_LE_OP, EX_GE_OP, EX_EQ_OP, EX_NE_OP, EX_SEMICOLON_CHAR, EX_OPEN_BRACE_CHAR, EX_CLOSE_BRACE_CHAR, EX_OPEN_SQUARE_BRACKET_CHAR, EX_CLOSE_SQUARE_BRACKET_CHAR, EX_COMMA_CHAR, EX_COLON_CHAR, EX_EQUALS_CHAR, EX_OPEN_PAREN_CHAR, EX_CLOSE_PAREN_CHAR, EX_DOT_CHAR, EX_AMPERSAND_CHAR, EX_EXCLAMATION_MARK_CHAR, EX_TILDE_CHAR, EX_MINUS_CHAR, EX_PLUS_CHAR, EX_MULTIPLY_CHAR, EX_DIVIDE_CHAR, EX_PERCENT_CHAR, EX_OPEN_ANGLE_BRACKET_CHAR, EX_CLOSE_ANGLE_BRACKET_CHAR, EX_CARET_CHAR, EX_PIPE_CHAR, EX_QUESTION_MARK_CHAR, EX_NUMBER_SIGN_CHAR};

void mini_lexer_test(struct memory_pool_collection * m){
	unsigned int offset = 0;
	unsigned int len = (unsigned int)strlen(sample_c);
	unsigned int num_regexes = 0;
	unsigned int i;
	unsigned int current_index = 0;
	struct regex_matcher_state matcher;
	struct regex_compiler_state ** compiler_states = (struct regex_compiler_state **)0;

#define NON_IDENTIFIER_CHAR "[^a-zA-Z0-9_]"
#define C_COMMENT "(/\\*([^\\*]|\\*[^/])*\\*/)"
#define NEWLINE "(\\n)"
#define SPACE "(\\t| )+"
#define DECIMAL "([0-9])"
#define EXPONENT "([Ee][\\+\\-]?" DECIMAL "+)"
#define FLOAT "([fFlL])"
#define INTEGER "([uUlL]*)"
#define CHARACTER "([^']+|\\\\')"
#define STRING "\"(" CHARACTER ")*\""
#define CRAZY_STRING STRING "(((" NEWLINE "|" SPACE "|" C_COMMENT ")*)?" STRING ")*"
#define LARGE_FLOAT DECIMAL "+\\." DECIMAL "*" EXPONENT "?" FLOAT "?"
#define SMALL_FLOAT "\\." DECIMAL "+" EXPONENT "?" FLOAT "?"

	add_lexer_test_regex(m, &compiler_states, SPACE,                             EX_SPACE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, NEWLINE,                           EX_NEWLINE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, C_COMMENT,                         EX_COMMENT, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "auto" NON_IDENTIFIER_CHAR,         EX_AUTO, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "break" NON_IDENTIFIER_CHAR,        EX_BREAK, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "case" NON_IDENTIFIER_CHAR,         EX_CASE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "char" NON_IDENTIFIER_CHAR,         EX_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "const" NON_IDENTIFIER_CHAR,        EX_CONST, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "continue" NON_IDENTIFIER_CHAR,     EX_CONTINUE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "default" NON_IDENTIFIER_CHAR,      EX_DEFAULT, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "do" NON_IDENTIFIER_CHAR,           EX_DO, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "double" NON_IDENTIFIER_CHAR,       EX_DOUBLE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "else" NON_IDENTIFIER_CHAR,         EX_ELSE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "enum" NON_IDENTIFIER_CHAR,         EX_ENUM, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "extern" NON_IDENTIFIER_CHAR,       EX_EXTERN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "float" NON_IDENTIFIER_CHAR,        EX_FLOAT, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "for" NON_IDENTIFIER_CHAR,          EX_FOR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "goto" NON_IDENTIFIER_CHAR,         EX_GOTO, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "if" NON_IDENTIFIER_CHAR,           EX_IF, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "int" NON_IDENTIFIER_CHAR,          EX_INT, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "long" NON_IDENTIFIER_CHAR,         EX_LONG, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "register" NON_IDENTIFIER_CHAR,     EX_REGISTER, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "return" NON_IDENTIFIER_CHAR,       EX_RETURN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "short" NON_IDENTIFIER_CHAR,        EX_SHORT, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "signed" NON_IDENTIFIER_CHAR,       EX_SIGNED, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "sizeof" NON_IDENTIFIER_CHAR,       EX_SIZEOF, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "static" NON_IDENTIFIER_CHAR,       EX_STATIC, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "struct" NON_IDENTIFIER_CHAR,       EX_STRUCT, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "switch" NON_IDENTIFIER_CHAR,       EX_SWITCH, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "typedef" NON_IDENTIFIER_CHAR,      EX_TYPEDEF, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "union" NON_IDENTIFIER_CHAR,        EX_UNION, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "unsigned" NON_IDENTIFIER_CHAR,     EX_UNSIGNED, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "void" NON_IDENTIFIER_CHAR,         EX_VOID, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "volatile" NON_IDENTIFIER_CHAR,     EX_VOLATILE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "while" NON_IDENTIFIER_CHAR,        EX_WHILE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "[a-zA-Z_][a-zA-Z_0-9]+",          EX_IDENTIFIER, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "0[xX][a-fA-f0-9]+" INTEGER,       EX_CONSTANT_HEX, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, DECIMAL "+" EXPONENT FLOAT "?",    EX_CONSTANT_EXPONENT, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, SMALL_FLOAT,                       EX_CONSTANT_FLOAT_SMALL, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, LARGE_FLOAT,                       EX_CONSTANT_FLOAT_LARGE, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "[0-9]+",                          EX_CONSTANT_DECIMAL, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "'" CHARACTER "'",                 EX_CONSTANT_CHARACTER, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, CRAZY_STRING,                      EX_STRING_LITERAL, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\.\\.\\.",                       EX_ELLIPSIS, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, ">>=",                             EX_RIGHT_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "<<=",                             EX_LEFT_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\+=",                            EX_ADD_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\-=",                            EX_SUB_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\*=",                            EX_MUL_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "/=",                              EX_DIV_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "%=",                              EX_MOD_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "&=",                              EX_AND_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\^=",                            EX_XOR_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\|=",                            EX_OR_ASSIGN, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, ">>",                              EX_RIGHT_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "<<",                              EX_LEFT_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\+\\+",                          EX_INC_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\-\\-",                          EX_DEC_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\->",                            EX_PTR_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "&&",                              EX_AND_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\|\\|",                          EX_OR_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "<=",                              EX_LE_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, ">=",                              EX_GE_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "==",                              EX_EQ_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "!=",                              EX_NE_OP, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, ";",                               EX_SEMICOLON_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "{",                               EX_OPEN_BRACE_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "}",                               EX_CLOSE_BRACE_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\[",                             EX_OPEN_SQUARE_BRACKET_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\]",                             EX_CLOSE_SQUARE_BRACKET_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, ",",                               EX_COMMA_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, ":",                               EX_COLON_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "=",                               EX_EQUALS_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\(",                             EX_OPEN_PAREN_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\)",                             EX_CLOSE_PAREN_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\.",                             EX_DOT_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "&",                               EX_AMPERSAND_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "!",                               EX_EXCLAMATION_MARK_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "~",                               EX_TILDE_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\-",                             EX_MINUS_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\+",                             EX_PLUS_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\*",                             EX_MULTIPLY_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "/",                               EX_DIVIDE_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "%",                               EX_PERCENT_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "<",                               EX_OPEN_ANGLE_BRACKET_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, ">",                               EX_CLOSE_ANGLE_BRACKET_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\^",                             EX_CARET_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\|",                             EX_PIPE_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "\\?",                             EX_QUESTION_MARK_CHAR, &num_regexes);
	add_lexer_test_regex(m, &compiler_states, "#",                               EX_NUMBER_SIGN_CHAR, &num_regexes);

	regex_matcher_state_create(&matcher, compiler_states, num_regexes);

	while(offset < len){
		unsigned int match_len;
		unsigned int match_id;
		unsigned int match_index = 0;
		unsigned int j;
		regex_matcher_state_run(&matcher, (unsigned char *)&sample_c[offset], len);
		assert(matcher.num_unique_matches_found);
		/*  Pick the longest match if there are multiple */
		for(j = 1; j < matcher.num_unique_matches_found; j++){
			if(matcher.actual_matches[j]->length > matcher.actual_matches[match_index]->length){
				match_index = j;
			}
		}
		match_id = matcher.actual_matches[match_index]->match_node->match_id;
		match_len = matcher.actual_matches[match_index]->length;
		/*  Keywords tokens don't include the last character */
		switch(match_id){
			case EX_AUTO:; case EX_BREAK:; case EX_CASE:; case EX_CHAR:; case EX_CONST:;
			case EX_CONTINUE:; case EX_DEFAULT:; case EX_DO:; case EX_DOUBLE:; case EX_ELSE:;
			case EX_ENUM:; case EX_EXTERN:; case EX_FLOAT:; case EX_FOR:; case EX_GOTO:;
			case EX_IF:; case EX_INT:; case EX_LONG:; case EX_REGISTER:; case EX_RETURN:;
			case EX_SHORT:; case EX_SIGNED:; case EX_SIZEOF:; case EX_STATIC:; case EX_STRUCT:;
			case EX_SWITCH:; case EX_TYPEDEF:; case EX_UNION:; case EX_UNSIGNED:; case EX_VOID:; case EX_VOLATILE:; case EX_WHILE:{
				match_len = match_len -1;
			}
		}
		printf("Match is type %s: '", example_token_names[match_id]);
		for(j = 0; j < match_len; j++){
			printf("%c", sample_c[offset + j]);
		}
		printf("'.\n");
		offset += match_len;

		if(match_id != EX_SPACE){ /*  Ignore spaces in our check */
			if(match_id == expected_types[current_index]){
				printf("%s matches.\n", example_token_names[expected_types[current_index]]);
			}else{
				printf("Expected %s but got %s.\n", example_token_names[expected_types[current_index]], example_token_names[match_id]);
				assert(0);
			}
			current_index++;
		}
	}

	for(i = 0; i < num_regexes; i++){
		regex_compiler_state_destroy(compiler_states[i]);
		free((void*)compiler_states[i]);
	}
	free((void*)compiler_states);
	regex_matcher_state_destroy(&matcher);
}

int main(void){
	struct memory_pool_collection m;
	memory_pool_collection_create(&m);

	struct_regex_computation_node_memory_pool_create(&m);
	struct_regex_parser_node_memory_pool_create(&m);

	test_regex_compile_error(&m, "\\x",   REGEX_ERROR_EXPECTED_HEX_DIGIT);
	test_regex_compile_error(&m, "\\xk",  REGEX_ERROR_EXPECTED_HEX_DIGIT);
	test_regex_compile_error(&m, "\\x3z", REGEX_ERROR_EXPECTED_HEX_DIGIT);
	test_regex_compile_error(&m, "(",    REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST);
	test_regex_compile_error(&m, "(a|",    REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST);
	test_regex_compile_error(&m, ")",    REGEX_ERROR_INCOMPLETE_PARSING);
	test_regex_compile_error(&m, "abc(",    REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST);
	test_regex_compile_error(&m, "abc(abc(",    REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST);
	test_regex_compile_error(&m, "abc(abc(|abc(",    REGEX_ERROR_EXPECTED_INFIX_EXPRESSION_REST);
	test_regex_compile_error(&m, "[a-]",    REGEX_ERROR_INVALID_CHARACTER_RANGE);
	test_regex_compile_error(&m, "[a--a]",    REGEX_ERROR_INVALID_CHARACTER_RANGE);
	test_regex_compile_error(&m, "[-a]",    REGEX_ERROR_INVALID_CHARACTER_CLASS);
	test_regex_compile_error(&m, "[-]",    REGEX_ERROR_INVALID_CHARACTER_CLASS);
	test_regex_compile_error(&m, "[a-b-c]",    REGEX_ERROR_INVALID_CHARACTER_CLASS);
	test_regex_compile_error(&m, "a?+",    REGEX_ERROR_INCOMPLETE_PARSING);
	test_regex_compile_error(&m, "a?*",    REGEX_ERROR_INCOMPLETE_PARSING);
	test_regex_compile_error(&m, "a+*",    REGEX_ERROR_INCOMPLETE_PARSING);
	test_regex_compile_error(&m, "a**",    REGEX_ERROR_INCOMPLETE_PARSING);
	test_regex_compile_error(&m, "a??",    REGEX_ERROR_INCOMPLETE_PARSING);
	test_1(&m);
	/*test_2(&m);*/
	test_3(&m);
	test_4(&m);
	test_5(&m);
	test_6(&m);
	test_7(&m);
	test_8(&m);
	mini_lexer_test(&m);

	struct_regex_parser_node_memory_pool_destroy(&m);
	struct_regex_computation_node_memory_pool_destroy(&m);

	memory_pool_collection_destroy(&m);



	return 0;
}
