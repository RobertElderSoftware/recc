#ifndef __LINKER_H__
#define __LINKER_H__
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
#ifndef __ASSERT_H_DEFINED__
#include <assert.h>
#endif
#ifndef __IO_H__
#include "io.h"
#endif
#ifndef __LEXER_H__
#include "lexer.h"
#endif
#ifndef __PARSER_H__
#include "parser.h"
#endif
#ifndef __LINKER_H__
#include "linker.h"
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "../data-structures/unsigned_char_list.h"
#endif
#ifndef __unsigned_char_ptr_list__H__DEFINED__
#include "../data-structures/unsigned_char_ptr_list.h"
#endif
#ifndef __struct_unsigned_char_list_ptr_list__H__DEFINED__
#include "../data-structures/struct_unsigned_char_list_ptr_list.h"
#endif
#ifndef __struct_asm_instruction_ptr_list__H__DEFINED__
#include "../data-structures/struct_asm_instruction_ptr_list.h"
#endif
#ifndef __struct_asm_lexer_state_ptr_list__H__DEFINED__
#include "../data-structures/struct_asm_lexer_state_ptr_list.h"
#endif
#ifndef __struct_linker_object_ptr_list__H__DEFINED__
#include "../data-structures/struct_linker_object_ptr_list.h"
#endif
#ifndef __unsigned_char_ptr_to_struct_linker_symbol_ptr_map__H__DEFINED__
#include "../data-structures/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h"
#endif
#ifndef __struct_linker_object_ptr_merge_sort__H__DEFINED__
#include "../data-structures/struct_linker_object_ptr_merge_sort.h"
#endif
#ifndef __struct_asm_instruction_memory_pool__H__DEFINED__
#include "../data-structures/struct_asm_instruction_memory_pool.h"
#endif
#ifndef __struct_linker_symbol_memory_pool__H__DEFINED__
#include "../data-structures/struct_linker_symbol_memory_pool.h"
#endif
#ifndef __TYPES_LINKER_struct_linker_symbol_h__
#include "../types/linker/struct_linker_symbol.h"
#endif
#ifndef __TYPES_LINKER_struct_asm_instruction_h__
#include "../types/linker/struct_asm_instruction.h"
#endif
#ifndef __OP_CPU_H__
#include "../emulators/c/op-cpu.h"
#endif
#ifndef __TYPES_LINKER_struct_linker_state_h__
#include "../types/linker/struct_linker_state.h"
#endif

int do_link(struct memory_pool_collection *, struct unsigned_char_ptr_list *, unsigned char *, unsigned char *, enum build_target_type);
struct linker_object * process_assembly(struct linker_state *, struct asm_lexer_state *);
void set_symbol_instruction_index(struct linker_state *, struct linker_object *, struct asm_lexer_token *, unsigned int);
void add_internal_linker_symbol(struct linker_state *, struct linker_object *, struct asm_lexer_token *, unsigned int, unsigned int);
void add_external_linker_symbol(struct linker_state *, struct linker_object *, struct asm_lexer_token *, unsigned int, unsigned int);
void output_artifacts(struct linker_state *, struct unsigned_char_list *, struct linker_object *, struct unsigned_char_list *, unsigned char *);
unsigned int get_relative_symbol_offset(struct linker_state *, struct linker_object *, struct linker_symbol *);
unsigned int get_absolute_symbol_offset(struct linker_state *, unsigned char *, struct linker_object *);
void verify_symbol_declaration(struct linker_state *, struct linker_object *, struct asm_lexer_token *);
void linker_state_destroy(struct linker_state *);
void linker_state_create(struct linker_state *, struct memory_pool_collection *, struct unsigned_char_ptr_list *, unsigned char *, unsigned char *, enum build_target_type);
void do_link_to_l1(struct linker_state *);
void do_link_to_l2(struct linker_state *);
void set_all_post_linking_offsets(struct linker_state *);

#endif
