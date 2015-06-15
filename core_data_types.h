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
#define __CORE_DATA_TYPES__H__DEFINED__

/*  Data types required to be defined/declared by our data structures */
struct type_description;
struct parser_operation;
struct normalized_specifier;
struct normalized_declarator;
struct normalized_declaration_element;
struct memory_pooler;
struct memory_pooler_collection;
struct c_lexer_token;
struct build_script_lexer_token;
struct asm_lexer_token;
struct linker_symbol;
struct asm_instruction;
struct unsigned_char_list;
struct linker_object;
struct asm_lexer_state;
struct namespace_object;
struct scope_level;
struct switch_frame;
struct constant_description;
struct constant_initializer_level;
struct type_traversal;
struct macro_parameter;
struct macro_definition;
struct struct_c_lexer_token_ptr_list;
struct if_branch;
struct struct_unsigned_char_list;
struct c_lexer_state;

enum parser_operation_type{
	INCREMENT_NEWLINE_COUNT,
	PROCESS_CONSTANT,
	ACCEPT_IDENTIFIER,
	ADVANCE_TOKEN_POSITION,
	ADVANCE_PARSER_POSITION,
	INCREMENT_SCOPE_DEPTH,
	DECREMENT_SCOPE_DEPTH,
	RECORD_EXTERN_DECLARATION,
	SAVE_DECLARATION_ELEMENTS,
	NAMESPACE_MODIFICATION,
	CREATE_PARSER_NODE_TREE,
	SET_NEXT_NAMESPACE_OBJECT,
	SET_PREVIOUS_NAMESPACE_OBJECT,
	SET_CURRENT_FUNCTION
};

struct parser_operation{
	void * data;
	enum parser_operation_type type;
	unsigned int pad;
};

#define BITS_PER_BRANCH_DIST  9u
#define BITS_PER_LITERAL     16u
#define BITS_PER_OP_CODE      5u
#define OP_CODE_OFFSET       27u
#define BITS_PER_REGISTER     9u
#define ra_OFFSET            18u
#define rb_OFFSET            (ra_OFFSET - BITS_PER_REGISTER)
#define rc_OFFSET            (rb_OFFSET - BITS_PER_REGISTER)

#define UNSHIFTED_OP_CODE_MASK   (0xFFFFFFFF >> (32u - BITS_PER_OP_CODE))
#define OP_CODE_MASK             (UNSHIFTED_OP_CODE_MASK << OP_CODE_OFFSET)
#define UNSHIFTED_REGISTER_MASK  (0xFFFFFFFF >> (32u - BITS_PER_REGISTER))
#define ra_MASK                  (UNSHIFTED_REGISTER_MASK << ra_OFFSET)
#define rb_MASK                  (UNSHIFTED_REGISTER_MASK << rb_OFFSET)
#define rc_MASK                  (UNSHIFTED_REGISTER_MASK << rc_OFFSET)
#define LITERAL_MASK             (0xFFFFFFFF >> (32u - BITS_PER_LITERAL))
#define BRANCH_DISTANCE_MASK     (0xFFFFFFFF >> (32u - BITS_PER_BRANCH_DIST))

#define BRANCH_DISTANCE_SIGN_BIT 0x100

#define MAX_LL_CONSTANT 0xFFFF
#define MAX_BRANCH_POS 255
#define MAX_BRANCH_NEG 256

#endif
