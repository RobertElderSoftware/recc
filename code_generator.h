#ifndef __GENERATE_CODE__ 
#define __GENERATE_CODE__ 
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

#ifndef __STDLIB_H_DEFINED__
#include <stdlib.h>
#endif
#ifndef __STRING_H_DEFINED__
#include <string.h>
#endif
#ifndef __PARSER_H__
#include "parser.h"
#endif
#ifndef __unsigned_int_list__H__DEFINED__
#include "data-structures/unsigned_int_list.h"
#endif
#ifndef __unsigned_int_stack__H__DEFINED__
#include "data-structures/unsigned_int_stack.h"
#endif
#ifndef __struct_type_description_ptr_list__H__DEFINED__
#include "data-structures/struct_type_description_ptr_list.h"
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "data-structures/unsigned_char_list.h"
#endif
#ifndef __unsigned_int_ptr_list__H__DEFINED__
#include "data-structures/unsigned_int_ptr_list.h"
#endif
#ifndef __unsigned_char_ptr_to_struct_linker_symbol_ptr_map__H__DEFINED__
#include "data-structures/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h"
#endif
#ifndef __struct_switch_frame_ptr_list__H__DEFINED__
#include "data-structures/struct_switch_frame_ptr_list.h"
#endif
#ifndef __unsigned_int_stack__H__DEFINED__
#include "data-structures/unsigned_int_stack.h"
#endif
#ifndef __struct_type_description_ptr_list__H__DEFINED__
#include "data-structures/struct_type_description_ptr_list.h"
#endif
#ifndef __struct_constant_initializer_level_ptr_list__H__DEFINED__
#include "data-structures/struct_constant_initializer_level_ptr_list.h"
#endif
#ifndef __struct_type_traversal_ptr_list__H__DEFINED__
#include "data-structures/struct_type_traversal_ptr_list.h"
#endif
#ifndef __char_ptr_list__H__DEFINED__
#include "data-structures/char_ptr_list.h"
#endif
#ifndef __LINKER_H__
#include "linker.h"
#endif
#ifndef __LEXER_H__
#include "lexer.h"
#endif
#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif
#ifndef __ASSERT_H_DEFINED__
#include <assert.h>
#endif
#ifndef __STDARG_H_DEFINED__
#include <stdarg.h>
#endif

enum copy_method{
	ASSIGN_COPY,
	REF_COPY,
	NEW_COPY
};

struct type_traversal{
	struct type_description * type_description;
	struct struct_type_traversal_ptr_list children;
	unsigned int arity; /* For arrays */
	unsigned int arrays_as_pointers; /* For arrays */
	unsigned char * parent_tag_name; /* structs, enums, unions */
	unsigned char * member_name; /* for structs, enums */
	enum type_class type_class;
	unsigned int pad;
};

struct switch_frame{
	unsigned int condition_index;
	unsigned int has_default;
	struct unsigned_int_list values;
};

struct compile_time_constant{
	struct constant_description * constant_description; /* For everything else */
	struct normalized_declaration_element * element;    /*  For enum types */
};

struct constant_initializer_level{
	struct compile_time_constant * constant; /*  Will be set only if this is not an initializer list */
	struct struct_constant_initializer_level_ptr_list children;
};

struct code_gen_state{
	struct parser_state * parser_state;
	struct namespace_object * current_function;
	struct struct_switch_frame_ptr_list switch_frames; /*  Used for keeping track of the case labels under switches */
	struct unsigned_int_list scope_index_list;
	struct struct_type_description_ptr_list type_stack;
	struct unsigned_char_list * buffered_output;
	struct unsigned_char_list * buffered_symbol_table;
	struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map symbols;
	unsigned int condition_index;
	unsigned int next_scope_index;
	unsigned int global_var_ptr_index;
	unsigned int pad;
};

int generate_code(struct parser_state *, struct code_gen_state *);
int do_code_generation(struct memory_pooler_collection *, unsigned char *, unsigned char *);
int destroy_code_gen_state(struct code_gen_state *);
struct compile_time_constant * evaluate_constant_constant_expression(struct code_gen_state *, struct parser_node *);
unsigned int type_size(struct code_gen_state *, struct type_description *, enum value_type, unsigned int, struct scope_level *);
unsigned int struct_type_size(struct code_gen_state *, struct type_description *, enum value_type, struct scope_level *);

#endif