#ifndef __TYPES_PARSER_enum_parser_operation_type_H__
#define __TYPES_PARSER_enum_parser_operation_type_H__
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

#endif
