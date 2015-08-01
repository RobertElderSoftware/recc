#ifndef __LEXER_H__
#define __LEXER_H__
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

#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif
#ifndef __STDLIB_H_DEFINED__
#include <stdlib.h>
#endif
#ifndef __IO_H__
#include "io.h"
#endif
#ifndef __struct_c_lexer_token_ptr_list__H__DEFINED__
#include "data-structures/struct_c_lexer_token_ptr_list.h"
#endif
#ifndef __struct_build_script_lexer_token_ptr_list__H__DEFINED__
#include "data-structures/struct_build_script_lexer_token_ptr_list.h"
#endif
#ifndef __struct_asm_lexer_token_ptr_list__H__DEFINED__
#include "data-structures/struct_asm_lexer_token_ptr_list.h"
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "data-structures/unsigned_char_list.h"
#endif
#ifndef __struct_c_lexer_token_memory_pool__H__DEFINED__
#include "data-structures/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef __struct_build_script_lexer_token_memory_pool__H__DEFINED__
#include "data-structures/struct_build_script_lexer_token_memory_pool.h"
#endif
#ifndef __struct_asm_lexer_token_memory_pool__H__DEFINED__
#include "data-structures/struct_asm_lexer_token_memory_pool.h"
#endif

struct common_lexer_state {
	unsigned char * buf;
	unsigned char * filename;
	struct memory_pool_collection * memory_pool_collection;
	struct unsigned_char_list * buffered_output;
	unsigned int position;
	unsigned int current_line;
	unsigned int buffer_size;
	unsigned int pad;
};

struct c_lexer_state{
	struct struct_c_lexer_token_ptr_list tokens;
	struct common_lexer_state c;
};

struct build_script_lexer_state{
	struct struct_build_script_lexer_token_ptr_list tokens;
	struct common_lexer_state c;
};

struct asm_lexer_state{
	struct struct_asm_lexer_token_ptr_list tokens;
	struct common_lexer_state c;
};

int lex_c(struct c_lexer_state *);
int lex_build_script(struct build_script_lexer_state *, unsigned char *, unsigned char *, unsigned int);
int lex_asm(struct asm_lexer_state *, unsigned char *, unsigned char *, unsigned int);
void create_c_lexer_state(struct c_lexer_state *, struct unsigned_char_list *, struct memory_pool_collection *, unsigned char *, unsigned char *, unsigned int);
void destroy_c_lexer_state(struct c_lexer_state *);
void destroy_build_script_lexer_state(struct build_script_lexer_state *);
void destroy_asm_lexer_state(struct asm_lexer_state *);
const char ** get_c_token_type_names(void);
const char ** get_build_script_token_type_names(void);
const char ** get_asm_token_type_names(void);
unsigned int count_newlines_in_comment(struct c_lexer_token *);

#endif
