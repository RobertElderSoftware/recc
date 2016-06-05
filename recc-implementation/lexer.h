#ifndef LEXER_H_
#define LEXER_H_
/*
    Copyright 2016 Robert Elder Software Inc.
    
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

#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef STDLIB_H_DEFINED_
#include <stdlib.h>
#endif
#ifndef IO_H_
#include "io.h"
#endif
#ifndef struct_c_lexer_token_ptr_list_H_DEFINED_
#include "../generated/struct_c_lexer_token_ptr_list.h"
#endif
#ifndef struct_l2_lexer_token_ptr_list_H_DEFINED_
#include "../generated/struct_l2_lexer_token_ptr_list.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../generated/unsigned_char_list.h"
#endif
#ifndef struct_c_lexer_token_memory_pool_H_DEFINED_
#include "../generated/struct_c_lexer_token_memory_pool.h"
#endif
#ifndef struct_l2_lexer_token_memory_pool_H_DEFINED_
#include "../generated/struct_l2_lexer_token_memory_pool.h"
#endif
#ifndef TYPES_LEXER_struct_c_lexer_state_H_
#include "../generated/struct_c_lexer_state.h"
#endif
#ifndef TYPES_LEXER_struct_l2_lexer_state_H_
#include "../generated/struct_l2_lexer_state.h"
#endif
#ifndef REGEX_ENGINE_H_
#include "regex_engine.h"
#endif


int lex_c(struct c_lexer_state *);
int lex_asm(struct l2_lexer_state *);
void create_c_lexer_state(struct c_lexer_state *, struct unsigned_char_list *, struct memory_pool_collection *, unsigned char *, unsigned char *, unsigned int);
void destroy_c_lexer_state(struct c_lexer_state *);
void destroy_l2_lexer_state(struct l2_lexer_state *);
void create_l2_lexer_state(struct l2_lexer_state * state, struct unsigned_char_list *, struct memory_pool_collection *, unsigned char *, unsigned char *, unsigned int);
const char ** get_c_token_type_names(void);
const char ** get_l2_token_type_names(void);
unsigned int count_newlines_in_comment(struct c_lexer_token *);

#endif
