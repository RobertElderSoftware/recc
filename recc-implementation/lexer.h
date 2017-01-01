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
#ifndef generated_DIR_lexer_generated_includes_DOT_h
#include "../generated/lexer_generated_includes.h"
#endif
#ifndef REGEX_ENGINE_H_
#include "regex_engine.h"
#endif



int lex_c(struct c_lexer_state *);
int lex_l2(struct l2_lexer_state *);
void create_c_lexer_state(struct c_lexer_state *, struct unsigned_char_list *, struct memory_pool_collection *, unsigned char *, unsigned char *, unsigned int);
void destroy_c_lexer_state(struct c_lexer_state *);
void destroy_l2_lexer_state(struct l2_lexer_state *);
void create_l2_lexer_state(struct l2_lexer_state * state, struct unsigned_char_list *, struct memory_pool_collection *, unsigned char *, unsigned char *, unsigned int);
const char ** get_c_token_type_names(void);
const char ** get_l2_token_type_names(void);
unsigned int count_newlines_in_comment(struct c_lexer_token *);

void l2_token_matcher_create(struct memory_pool_collection * m);
void c_token_matcher_create(struct memory_pool_collection * m);
void l2_token_matcher_destroy(struct memory_pool_collection * m);
void c_token_matcher_destroy(struct memory_pool_collection * m);


#endif
