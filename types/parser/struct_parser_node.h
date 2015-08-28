#ifndef __TYPES_PARSER_struct_parser_node_H__
#define __TYPES_PARSER_struct_parser_node_H__
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

#ifndef __TYPES_PARSER_enum_node_type_H__
#include "enum_node_type.h"
#endif
#ifndef __TYPES_LEXER_struct_c_lexer_token_H__
#include "../../types/lexer/struct_c_lexer_token.h"
#endif

struct parser_node;

struct parser_node{
	struct parser_node * next;
	struct parser_node * first_child;
	struct c_lexer_token * c_lexer_token;
	enum node_type type;
	unsigned int pad;
};

#endif
