#ifndef TYPES_LINKER_struct_l2_item_h__
#define TYPES_LINKER_struct_l2_item_h__
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

#ifndef TYPES_LEXER_struct_l2_lexer_token_H__
#include "../lexer/struct_l2_lexer_token.h"
#endif

struct l2_item{
	struct l2_lexer_token * op_token;
	struct l2_lexer_token * rx_token;
	struct l2_lexer_token * ry_token;
	struct l2_lexer_token * rz_token;
	struct l2_lexer_token * identifier_token;
	struct l2_lexer_token * number_token;
	unsigned int number_token_is_negative;
	unsigned int post_linking_offset; /* The offset (in # of 4 byte words) of this instruction after we've considered instructions that need to be re-written by the linker */
	unsigned int pre_linking_offset; /* The offset (in # of 4 byte words) of this instruction before moving symbols around in a linker object */
	unsigned int pad;
};

#endif
