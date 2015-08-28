#ifndef __TYPES_PARSER_struct_constant_description_H__
#define __TYPES_PARSER_struct_constant_description_H__
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


#ifndef __TYPES_PARSER_struct_type_description_H__
#include "struct_type_description.h"
#endif

struct constant_description{
	struct type_description * type_description;
	unsigned char * str;
	unsigned int * native_data; /* Byte array of the data used in the program */
	unsigned int size_in_bytes;
	unsigned int num_references;
	enum c_token_type type;
	unsigned int pad;
};

#endif
