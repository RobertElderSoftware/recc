#ifndef __TYPES_CODE_GENERATOR_struct_compile_time_constant_H__
#define __TYPES_CODE_GENERATOR_struct_compile_time_constant_H__
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


#ifndef __TYPES_PARSER_struct_constant_description_H__
#include "../../types/parser/struct_constant_description.h"
#endif
#ifndef __TYPES_PARSER_struct_normalized_declaration_element_H__
#include "../../types/parser/struct_normalized_declaration_element.h"
#endif

struct compile_time_constant{
	struct constant_description * constant_description; /* For everything else */
	struct normalized_declaration_element * element;    /*  For enum types */
};

#endif
