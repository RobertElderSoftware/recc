#ifndef REPLACE_TOOL__H__DEFINED__
#define REPLACE_TOOL__H__DEFINED__
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

#include "binary_exponential_buffer.h"

void validate_replacement_strings(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void reset_candidates(struct binary_exponential_buffer *);
unsigned int count_candidates(struct binary_exponential_buffer *);
unsigned int get_length(unsigned char *);
unsigned int set_lengths(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void do_string_replacements(struct binary_exponential_buffer *, struct binary_exponential_buffer *, struct binary_exponential_buffer *, struct binary_exponential_buffer *);
#endif

