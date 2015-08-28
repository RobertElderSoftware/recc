#ifndef __TYPES_LEXER_struct_asm_lexer_state_H__
#define __TYPES_LEXER_struct_asm_lexer_state_H__
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

#ifndef __TYPES_LEXER_struct_common_lexer_state_H__
#include "struct_common_lexer_state.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_struct_asm_lexer_token_ptr_list_H__
#include "../../types/data-structures/struct_struct_asm_lexer_token_ptr_list.h"
#endif

struct asm_lexer_state{
	struct struct_asm_lexer_token_ptr_list tokens;
	struct common_lexer_state c;
};

#endif
