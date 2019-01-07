#ifndef REPLACE_TOOL_H_DEFINED_
#define REPLACE_TOOL_H_DEFINED_
/*
    Copyright 2019 Robert Elder Software Inc.
    
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

#include "../recc-implementation/binary_exponential_buffer.h"

struct replacement_rule{
	struct binary_exponential_buffer search;
	struct binary_exponential_buffer replace;
};

struct replacement_context{
	struct binary_exponential_buffer rules;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;
};

void replacement_context_associate_replacement(struct replacement_context *, struct replacement_rule *);
struct replacement_context * replacement_context_create(void);
void replacement_context_destroy(struct replacement_context *);
void reset_candidates(struct binary_exponential_buffer *);
unsigned int count_candidates(struct binary_exponential_buffer *);
unsigned int get_length(unsigned char *);
void do_string_replacements(struct replacement_context *);
struct replacement_rule * replacement_rule_create(void);
void replacement_rule_destroy(struct replacement_rule *);
#endif
