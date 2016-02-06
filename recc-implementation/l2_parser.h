#ifndef L2_PARSER_H_
#define L2_PARSER_H_
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
#ifndef STDARG_H_DEFINED_
#include <stdarg.h>
#endif
#ifndef LEXER_H_
#include "lexer.h"
#endif
#ifndef TYPES_PARSER_struct_l2_parser_state_H_
#include "../types/parser/struct_l2_parser_state.h"
#endif
#ifndef struct_l2_lexer_token_ptr_list_H_DEFINED_
#include "../data-structures/struct_l2_lexer_token_ptr_list.h"
#endif
#ifndef struct_l2_parser_operation_stack_H_DEFINED_
#include "../data-structures/struct_l2_parser_operation_stack.h"
#endif
#ifndef struct_l2_parser_node_memory_pool_H_DEFINED_
#include "../data-structures/struct_l2_parser_node_memory_pool.h"
#endif

int parse_l2(struct l2_parser_state *);
void create_l2_parser_state(struct l2_parser_state *, struct memory_pool_collection *, struct l2_lexer_state *, struct unsigned_char_list *, unsigned char *);
void destroy_l2_parser_state(struct l2_parser_state *);
const char ** get_l2_node_type_names(void);


#endif
