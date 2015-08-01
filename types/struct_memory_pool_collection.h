#ifndef __TYPES_struct_memory_pool_collection_H__
#define __TYPES_struct_memory_pool_collection_H__
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

struct memory_pool_collection{
	struct struct_asm_lexer_token_memory_pool * struct_asm_lexer_token_pool;
	struct struct_build_script_lexer_token_memory_pool * struct_build_script_lexer_token_pool;
	struct struct_c_lexer_token_memory_pool * struct_c_lexer_token_pool;
	struct struct_parser_node_memory_pool * struct_parser_node_pool;
	struct struct_type_description_memory_pool * struct_type_description_pool;
	struct struct_asm_instruction_memory_pool * struct_asm_instruction_pool;
	struct struct_linker_symbol_memory_pool * struct_linker_symbol_pool;
	struct heap_memory_pool * heap_pool; /*  Used for pooling memory objects of arbitrary size */
};

#endif
