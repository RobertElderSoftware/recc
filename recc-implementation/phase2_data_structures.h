#ifndef PHASE2_DATA_STRUCTURES
#define PHASE2_DATA_STRUCTURES
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

#include "../recc-implementation/compiler_interface_phase_2.h"

void make_compare(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer, unsigned char *, const char *, const char *);
void make_list(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_kvp(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_map(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer, const char *);
void make_merge_sort(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_binary_search(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void make_memory_pool(struct build_state *, struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void register_generated_aggregate_include(struct build_state *, const char *, ...);
void va_register_generated_aggregate_include(struct build_state *, const char *, va_list);
void register_generated_type(struct build_state *, const char *, const char *, ...);
void va_register_generated_type(struct build_state *, const char *, const char *, va_list);

void register_data_structures_objects(struct build_state *);
#endif
