#ifndef __TYPES_LINKER_struct_linker_state_h__
#define __TYPES_LINKER_struct_linker_state_h__
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

#ifndef __TYPES_enum_build_target_type_H__
#include "../../types/recc-implementation/enum_build_target_type.h"
#endif

struct linker_state{
	struct struct_unsigned_char_list_ptr_list input_file_buffers;
	struct struct_linker_object_ptr_list linker_objects;
	struct struct_linker_object_ptr_list reordered_linker_objects;
	struct struct_asm_lexer_state_ptr_list lexer_states;
	struct unsigned_char_list file_output;
	struct unsigned_char_list symbol_output;
	struct unsigned_char_list asm_lexer_output;
	struct memory_pool_collection * memory_pool_collection;
	struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map external_symbols;
	struct unsigned_char_ptr_list * in_files;
	unsigned char * symbol_file;
	unsigned char * out_file;
	unsigned int starting_offset;
	enum build_target_type build_target_type;
};

#endif
