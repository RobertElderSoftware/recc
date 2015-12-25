#ifndef TYPES_L0_GENERATOR_struct_l0_generator_state_H__
#define TYPES_L0_GENERATOR_struct_l0_generator_state_H__
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

#ifndef TYPES_PARSER_struct_l2_parser_state_H__
#include "../parser/struct_l2_parser_state.h"
#endif
#ifndef TYPES_LEXER_struct_l2_lexer_state_H__
#include "../lexer/struct_l2_lexer_state.h"
#endif
#ifndef TYPES_L0_GENERATOR_enum_l0_language_type_H__
#include "../l0_generator/enum_l0_language_type.h"
#endif

struct l0_generator_state {
	struct l2_lexer_state l2_lexer_state;
	struct l2_parser_state l2_parser_state;
	struct memory_pool_collection * memory_pool_collection;
};

#endif
