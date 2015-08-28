#ifndef __MEMORY_POOL_COLLECTION_H__
#define __MEMORY_POOL_COLLECTION_H__
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

#ifndef __CORE_DATA_TYPES__H__DEFINED__
#include "core_data_types.h"
#endif
#ifndef __struct_parser_node_memory_pool__H__DEFINED__
#include "data-structures/struct_parser_node_memory_pool.h"
#endif
#ifndef __struct_c_lexer_token_memory_pool__H__DEFINED__
#include "data-structures/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef __struct_asm_lexer_token_memory_pool__H__DEFINED__
#include "data-structures/struct_asm_lexer_token_memory_pool.h"
#endif
#ifndef __struct_type_description_memory_pool__H__DEFINED__
#include "data-structures/struct_type_description_memory_pool.h"
#endif
#ifndef __struct_asm_instruction_memory_pool__H__DEFINED__
#include "data-structures/struct_asm_instruction_memory_pool.h"
#endif
#ifndef __struct_linker_symbol_memory_pool__H__DEFINED__
#include "data-structures/struct_linker_symbol_memory_pool.h"
#endif
#ifndef __HEAP_MEMORY_POOL_H__
#include "heap_memory_pool.h"
#endif

void memory_pool_collection_create(struct memory_pool_collection *);
void memory_pool_collection_destroy(struct memory_pool_collection *);

#endif
