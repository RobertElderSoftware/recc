#ifndef __TYPES_PARSER_struct_parser_state_H__
#define __TYPES_PARSER_struct_parser_state_H__
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

struct parser_state{
	struct memory_pool_collection * memory_pool_collection;
	struct c_lexer_state * c_lexer_state;
	struct scope_level * top_scope;
	struct parser_node * top_node;
	struct unsigned_char_list * buffered_output;
	struct struct_parser_operation_stack operation_stack;
	struct unsigned_char_ptr_to_struct_constant_description_ptr_map constant_map;
	unsigned char * buff;
	unsigned int tokens_position;
	unsigned int line_number;
	unsigned int current_scope_depth;
	unsigned int pad;
};

#endif
