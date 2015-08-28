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
#ifndef __struct_asm_lexer_token_ptr_list__H__DEFINED__
#include "data-structures/struct_asm_lexer_token_ptr_list.h"
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "data-structures/unsigned_char_list.h"
#endif
#ifndef __struct_c_lexer_token_memory_pool__H__DEFINED__
#include "data-structures/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef __struct_asm_lexer_token_memory_pool__H__DEFINED__
#include "data-structures/struct_asm_lexer_token_memory_pool.h"
#endif
#ifndef __TYPES_LEXER_struct_c_lexer_state_H__
#include "types/lexer/struct_c_lexer_state.h"
#endif
#ifndef __TYPES_LEXER_struct_asm_lexer_state_H__
#include "types/lexer/struct_asm_lexer_state.h"
#endif


int lex_c(struct c_lexer_state *);
int lex_asm(struct asm_lexer_state *, unsigned char *, unsigned char *, unsigned int);
void create_c_lexer_state(struct c_lexer_state *, struct unsigned_char_list *, struct memory_pool_collection *, unsigned char *, unsigned char *, unsigned int);
void destroy_c_lexer_state(struct c_lexer_state *);
void destroy_asm_lexer_state(struct asm_lexer_state *);
const char ** get_c_token_type_names(void);
const char ** get_asm_token_type_names(void);
unsigned int count_newlines_in_comment(struct c_lexer_token *);

#endif
