#ifndef __TYPES_PARSER_enum_normalized_declaration_type_H__
#define __TYPES_PARSER_enum_normalized_declaration_type_H__
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

enum normalized_declaration_type{
        NORMALIZED_DECLARATION,
        NORMALIZED_FUNCTION_DEFINITION,
        NORMALIZED_PARAMETER_DECLARATION,
        NORMALIZED_STRUCT_DECLARATION,
        NORMALIZED_K_AND_R_C_IDENTIFIER_DECLARATOR_PAIR,
        NORMALIZED_ENUMERATOR_LIST
};

#endif
