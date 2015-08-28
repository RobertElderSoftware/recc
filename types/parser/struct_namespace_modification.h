#ifndef __TYPES_PARSER_struct_namespace_modification_H__
#define __TYPES_PARSER_struct_namespace_modification_H__
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

struct namespace_modification{
	struct normalized_declaration_element * new_element;
	struct namespace_object * object;
	struct scope_level * scope_level;
	struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * scope_namespace;
	unsigned char * identifier_str;
	enum scope_type scope_type;
	enum object_location object_location;
	int offset;
	int pad;
};

#endif
