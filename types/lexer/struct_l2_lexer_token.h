#ifndef TYPES_LEXER_struct_l2_lexer_token_H_
#define TYPES_LEXER_struct_l2_lexer_token_H_
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

#ifndef TYPES_LEXER_enum_l2_token_type_H_
#include "enum_l2_token_type.h"
#endif

struct l2_lexer_token{
	unsigned char * first_byte;
	unsigned char * last_byte;
	enum l2_token_type type;
	unsigned int pad;
};

#endif
