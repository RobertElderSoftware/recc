#ifndef __TYPES_PARSER_enum_type_class_H__
#define __TYPES_PARSER_enum_type_class_H__
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

enum type_class{
	TYPE_CLASS_CHAR,
	TYPE_CLASS_SHORT,
	TYPE_CLASS_INT,
	TYPE_CLASS_LONG,
	TYPE_CLASS_LONG_LONG,
	TYPE_CLASS_FLOAT,
	TYPE_CLASS_DOUBLE,
	TYPE_CLASS_LONG_DOUBLE,
	TYPE_CLASS_VOID,
	TYPE_CLASS_POINTER,
	TYPE_CLASS_ARRAY,
	TYPE_CLASS_FUNCTION,
	TYPE_CLASS_ENUM,
	TYPE_CLASS_STRUCT,
	TYPE_CLASS_UNION
};

#endif
