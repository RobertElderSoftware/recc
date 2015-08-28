#ifndef COMPILER_INTERFACE_H__
#define COMPILER_INTERFACE_H__
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
#ifndef __PREPROCESSOR_H__
#include "preprocessor.h"
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
#ifndef __GENERATE_CODE__ 
#include "code_generator.h"
#endif
#ifndef __unsigned_char_ptr_list__H__DEFINED__
#include "data-structures/unsigned_char_ptr_list.h"
#endif
#ifndef __unsigned_char_ptr_to_struct_build_target_ptr_map__H__DEFINED__
#include "data-structures/unsigned_char_ptr_to_struct_build_target_ptr_map.h"
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
#ifndef __struct_parser_node_memory_pool__H__DEFINED__
#include "data-structures/struct_parser_node_memory_pool.h"
#endif
#ifndef __struct_type_description_memory_pool__H__DEFINED__
#include "data-structures/struct_type_description_memory_pool.h"
#endif
#ifndef __MEMORY_POOL_COLLECTION_H__
#include "memory_pool_collection.h"
#endif
#ifndef __TYPES_struct_build_target_H__
#include "types/struct_build_target.h"
#endif
#ifndef __TYPES_struct_build_state_H__
#include "types/struct_build_state.h"
#endif
#ifndef __PRELOADER_H__
#include "preloader.h"
#endif
#ifndef __FILESYSTEM_COMPILER_H__
#include "filesystem/filesystem_compiler.h"
#endif

struct build_state * create_build_state(void);
void destroy_build_state(struct build_state *);
const char * register_build_target(struct build_state *, const char *, enum build_target_type);
const char * register_dependency(struct build_state *, const char *, const char *);
void construct_build_target(struct build_state *, const char *);
void register_c_to_l1(struct build_state *, const char *);
void register_c_to_l2(struct build_state *, const char *);
void register_l0_file(struct build_state *, const char *, const char *, const char *);

#endif
