#ifndef TYPES_LEXER_struct_c_lexer_state_H_
#define TYPES_LEXER_struct_c_lexer_state_H_
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

#ifndef TYPES_LEXER_struct_common_lexer_state_H_
#include "struct_common_lexer_state.h"
#endif
#ifndef TYPES_DATA_STRUCTURES_struct_struct_c_lexer_token_ptr_list_H_
#include "../../types/data-structures/struct_struct_c_lexer_token_ptr_list.h"
#endif

struct c_lexer_state{
	struct struct_c_lexer_token_ptr_list tokens;
	struct common_lexer_state c;
};

#endif
