#ifndef TYPES_PREPROCESSOR_struct_macro_definition_H__
#define TYPES_PREPROCESSOR_struct_macro_definition_H__
/*
    Copyright 2015 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/
#ifndef TYPES_PREPROCESSOR_enum_macro_definition_type_H__
#include "enum_macro_definition_type.h"
#endif
#ifndef struct_c_lexer_token_ptr_list__H__DEFINED__
#include "../../data-structures/struct_c_lexer_token_ptr_list.h"
#endif
#ifndef unsigned_char_ptr_to_struct_macro_parameter_ptr_map__H__DEFINED__
#include "../../data-structures/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h"
#endif

struct macro_definition{
	struct struct_c_lexer_token_ptr_list definition_tokens;
	struct unsigned_char_ptr_to_struct_macro_parameter_ptr_map function_macro_parameters;
	enum macro_definition_type type;
	unsigned int pad;
};

#endif
