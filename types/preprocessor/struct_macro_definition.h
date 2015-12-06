#ifndef __TYPES_PREPROCESSOR_struct_macro_definition_H__
#define __TYPES_PREPROCESSOR_struct_macro_definition_H__
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
#ifndef __TYPES_PREPROCESSOR_enum_macro_definition_type_H__
#include "enum_macro_definition_type.h"
#endif
#ifndef __struct_c_lexer_token_ptr_list__H__DEFINED__
#include "../../data-structures/struct_c_lexer_token_ptr_list.h"
#endif
#ifndef __unsigned_char_ptr_to_struct_macro_parameter_ptr_map__H__DEFINED__
#include "../../data-structures/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h"
#endif

struct macro_definition{
	struct struct_c_lexer_token_ptr_list definition_tokens;
	struct unsigned_char_ptr_to_struct_macro_parameter_ptr_map function_macro_parameters;
	enum macro_definition_type type;
	unsigned int pad;
};

#endif
