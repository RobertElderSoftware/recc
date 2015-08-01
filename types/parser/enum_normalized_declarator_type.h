#ifndef __TYPES_PARSER_enum_normalized_declarator_type_H__
#define __TYPES_PARSER_enum_normalized_declarator_type_H__
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

enum normalized_declarator_type{
        NORMALIZED_DECLARATOR,
        NORMALIZED_ABSTRACT_DECLARATOR,
        NORMALIZED_STRUCT_DECLARATOR,
        NORMALIZED_INIT_DECLARATOR,
        NORMALIZED_ENUMERATOR
};

#endif
