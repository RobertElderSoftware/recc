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
#ifndef __PREPROCESSOR_H__
#define __PREPROCESSOR_H__ 

#include <assert.h>
#include "lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lexer.h"
#include "io.h"
#include "data-structures/unsigned_char_list.h"
#include "data-structures/struct_if_branch_ptr_list.h"
#include "data-structures/struct_c_lexer_token_ptr_list.h"
#include "data-structures/struct_struct_c_lexer_token_ptr_list_ptr_list.h"
#include "data-structures/unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#include "data-structures/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h"
#include "data-structures/unsigned_char_ptr_to_struct_macro_definition_ptr_map.h"
#include "data-structures/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.h"
#include "data-structures/struct_c_lexer_token_ptr_to_unsigned_char_ptr_map.h"
#include "data-structures/struct_unsigned_char_list_ptr_list.h"
#include "data-structures/struct_c_lexer_state_ptr_list.h"

struct if_branch{
	unsigned int active;
};

struct preprocessor_state{
	struct struct_unsigned_char_list_ptr_list tokenizable_input_buffers;  /*  The characters underlying all of our tokens */
	struct struct_if_branch_ptr_list if_branches;
	struct unsigned_char_ptr_list full_filenames;
	struct memory_pooler_collection * memory_pooler_collection;
	struct struct_c_lexer_state_ptr_list c_lexer_states;
	struct c_lexer_token * comma_token;
};

enum macro_definition_type{
	OBJECT_MACRO,
	FUNCTION_MACRO
};

struct macro_parameter{
	unsigned int position_index;
	unsigned int is_variadic;
};

struct macro_definition{
	struct struct_c_lexer_token_ptr_list definition_tokens;
	struct unsigned_char_ptr_to_struct_macro_parameter_ptr_map function_macro_parameters;
	enum macro_definition_type type;
	unsigned int pad;
};

enum search_state{
	MACRO_SEARCH,
	COMPLETE_DIRECTIVE,
	EXPECT_DIRECTIVE,
	BEGIN_LINE,
	NONE
};

enum directive_type{
	DEFINE_DIRECTIVE,
	INCLUDE_DIRECTIVE,
	IF_DIRECTIVE,
	IFDEF_DIRECTIVE,
	IFNDEF_DIRECTIVE,
	ENDIF_DIRECTIVE,
	ERROR_DIRECTIVE,
	PRAGMA_DIRECTIVE
};

void process_tokens(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *, enum search_state, unsigned int);
void process_line_continuators(struct unsigned_char_list *, struct unsigned_char_list *);
void free_define_map(struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *);
int get_preprocessed_output_from_file(struct preprocessor_state *, unsigned char *, struct struct_c_lexer_token_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *);

struct c_lexer_token * read_until_next_token(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *);
unsigned int is_non_whitespace_inline_token(struct c_lexer_token *);
void release_working_tokens(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *);
void process_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *);
enum directive_type get_directive_type(struct c_lexer_token *);
void process_define_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *);
void process_include_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *);
void process_ifndef_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *);
unsigned int process_identifier_if_macro(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *);
void split_tokens_into_parameters(struct struct_c_lexer_token_ptr_list *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *);
void perform_argument_prescan_all(struct preprocessor_state *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *);
void fully_expand_macros(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *, struct struct_c_lexer_token_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *);
void evaluate_function_macro_body(struct preprocessor_state *, struct macro_definition *, struct struct_c_lexer_token_ptr_list *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *, struct struct_struct_c_lexer_token_ptr_list_ptr_list *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct unsigned_char_ptr_to_struct_macro_definition_ptr_map *, struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map *);
struct preprocessor_state * create_preprocessor_state(struct memory_pooler_collection *);
void destroy_preprocessor_state(struct preprocessor_state *);
void process_endif_directive(struct preprocessor_state *, struct struct_c_lexer_token_ptr_list *);
unsigned int is_in_active_branch(struct preprocessor_state *);
unsigned int always_processed_directive(enum directive_type);
unsigned char * convert_filename_to_directory(unsigned char *);
void removes_quotes_from_string_literal(unsigned char *);
struct unsigned_char_list * add_tokenizable_input_buffer(struct preprocessor_state *);
struct c_lexer_state * add_c_lexer_state(struct preprocessor_state *);
unsigned char * get_current_file(struct preprocessor_state *);
int do_preprocess(struct memory_pooler_collection *, unsigned char *, unsigned char *);

#endif
