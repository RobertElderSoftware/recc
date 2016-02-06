#ifndef BINARY_EXPONENTIAL_BUFFER_H_DEFINED_
#define BINARY_EXPONENTIAL_BUFFER_H_DEFINED_
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

#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef STDLIB_H_DEFINED_
#include <stdlib.h>
#endif

struct binary_exponential_buffer{
	void * data;
	unsigned int size;
	unsigned int element_size;
	unsigned int next_threshold_size;
	unsigned int pad;
};

void binary_exponential_buffer_increment(struct binary_exponential_buffer *, unsigned int);
void binary_exponential_buffer_decrement(struct binary_exponential_buffer *, unsigned int);
void * binary_exponential_buffer_data(struct binary_exponential_buffer *);
unsigned int binary_exponential_buffer_size(struct binary_exponential_buffer *);
unsigned int binary_exponential_buffer_element_size(struct binary_exponential_buffer *);
void * binary_exponential_buffer_get(struct binary_exponential_buffer *, unsigned int);
void binary_exponential_buffer_destroy(struct binary_exponential_buffer *);
void binary_exponential_buffer_create(struct binary_exponential_buffer *, unsigned int);
struct binary_exponential_buffer binary_exponential_buffer_copy(struct binary_exponential_buffer *);

#endif
