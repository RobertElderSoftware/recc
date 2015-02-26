/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#ifndef __LINKER_H__
#define __LINKER_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "core_data_types.h"
#include "io.h"
#include "lexer.h"
#include "parser.h"
#include "linker.h"
#include "code_generator.h"
#include "data-structures/unsigned_char_list.h"
#include "data-structures/unsigned_char_ptr_list.h"
#include "data-structures/struct_unsigned_char_list_ptr_list.h"
#include "data-structures/struct_asm_instruction_ptr_list.h"
#include "data-structures/struct_asm_lexer_state_ptr_list.h"
#include "data-structures/struct_linker_object_ptr_list.h"
#include "data-structures/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h"

int do_link(struct memory_pooler_collection *, struct unsigned_char_ptr_list *, unsigned char *, unsigned char *);

struct linker_symbol{
	unsigned int is_implemented;
	unsigned int is_required;
	unsigned int is_external;
	unsigned int instruction_index; /* The index of the instruction in its parent linker object. */
};

struct asm_instruction{
	struct asm_lexer_token * op_token;
	struct asm_lexer_token * rx_token;
	struct asm_lexer_token * ry_token;
	struct asm_lexer_token * rz_token;
	struct asm_lexer_token * rw_token;
	struct asm_lexer_token * identifier_token;
	struct asm_lexer_token * number_token;
	unsigned int number_token_is_negative;
	unsigned int post_linking_offset; /* The offset (in # of 4 byte words) of this instruction after we've considered instructions that need to be re-written by the linker */
};

struct linker_object{ /*  Represents everything that comes from one translation unit (or assembly file) */
	struct asm_lexer_state * asm_lexer_state;
	struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map symbols;
	struct struct_asm_instruction_ptr_list instructions;
	unsigned int is_relocatable;
	unsigned int current_line;
	unsigned int linker_object_post_linking_offset; /* The offset to the instruction of this symbol after considering instructions re-written by the linker */
	unsigned int pad;
};

struct linker_object * process_assembly(struct asm_lexer_state *);
void set_symbol_instruction_index(struct linker_object *, struct asm_lexer_token *, unsigned int);
void add_linker_symbol(struct linker_object *, struct asm_lexer_token *, unsigned int, unsigned int, unsigned int);
void output_artifacts(struct unsigned_char_list *, struct linker_object *, struct struct_linker_object_ptr_list *, struct unsigned_char_list *);
unsigned int get_absolute_symbol_offset(unsigned char *, struct linker_object *, struct struct_linker_object_ptr_list *);
void verify_symbol_declaration(struct linker_object *, struct asm_lexer_token *);

#endif
