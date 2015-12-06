#ifndef __TYPES_CODE_GENERATOR_struct_code_gen_state_H__
#define __TYPES_CODE_GENERATOR_struct_code_gen_state_H__
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

struct code_gen_state{
	struct parser_state * parser_state;
	struct memory_pool_collection * memory_pool_collection;
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

#endif
