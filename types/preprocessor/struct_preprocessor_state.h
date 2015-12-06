#ifndef __TYPES_PREPROCESSOR_struct_preprocessor_state_H__
#define __TYPES_PREPROCESSOR_struct_preprocessor_state_H__
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

struct preprocessor_state{
	struct struct_unsigned_char_list_ptr_list tokenizable_input_buffers;  /*  The characters underlying all of our tokens */
	struct struct_preprocessor_if_branch_ptr_list preprocessor_if_branches;
	struct struct_preprocessor_file_context_ptr_list file_contexts;
	struct memory_pool_collection * memory_pool_collection;
	struct struct_c_lexer_state_ptr_list c_lexer_states;
	struct c_lexer_token * comma_token;
	struct c_lexer_token * space_token;
	struct struct_c_lexer_token_ptr_list created_tokens;
	struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_map special_macros;
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map macro_map;
	struct unsigned_char_ptr_to_struct_macro_definition_ptr_map disabled_macros;
	struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map disabled_tokens;
};

#endif
