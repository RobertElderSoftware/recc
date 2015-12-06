#ifndef __TYPES_LINKER_struct_linker_object_H__
#define __TYPES_LINKER_struct_linker_object_H__
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

#ifndef __TYPES_LEXER_struct_asm_lexer_state_H__
#include "../../types/lexer/struct_asm_lexer_state.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_map_H__
#include "../../types/data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_struct_asm_instruction_ptr_list_H__
#include "../../types/data-structures/struct_struct_asm_instruction_ptr_list.h"
#endif

struct linker_object{ /*  Represents everything that comes from one translation unit (or assembly file) */
	struct asm_lexer_state * asm_lexer_state;
	struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map internal_symbols;
	struct struct_asm_instruction_ptr_list instructions;
	unsigned int is_relocatable;
	unsigned int current_line;
	unsigned int linker_object_post_linking_offset; /* The offset to the instruction of this symbol after considering instructions re-written by the linker */
	unsigned int pad;
};

#endif
