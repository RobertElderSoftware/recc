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

#include "binary_exponential_buffer.h"

/*
	The purpose of the 'binary exponential buffer' is to offer a data structure that provides
	access to a buffer of contiguous memory that can be increased or decreased without causing an
	inordinately large number of memory allocations/deallocations.  When more memory is required,
	the actual size of the physical buffer is doubled, and adding any further elements will
	only increment the size count, until the buffer needs to be resized again.
	This is done so that adding n elements to the buffer only requires O(log n) kernel calls to request 
	more memory.
	
	Note that Changing the number of items in the buffer can cause the actual location of the bytes to
	change.

*/

struct binary_exponential_buffer make_beb_list(unsigned int n, ...){
	va_list va;
	struct binary_exponential_buffer b;
	unsigned int i;
	va_start(va, n);
	binary_exponential_buffer_create(&b, sizeof(unsigned char *));
	for(i = 0; i < n; i++){
		unsigned char * c = va_arg(va, unsigned char *);
		binary_exponential_buffer_increment(&b, 1);
		((unsigned char **)binary_exponential_buffer_data(&b))[binary_exponential_buffer_size(&b) -1] = c;
	}
	va_end(va);
	return b;
}

void binary_exponential_buffer_increment(struct binary_exponential_buffer * b, unsigned int n){
	b->size = b->size + n;
	while(b->size >= (b->next_threshold_size / 2)){
		b->next_threshold_size *= 2;
		b->data = realloc(b->data, b->element_size * b->next_threshold_size);
		assert(b->data);
	}
	assert(b->size < b->next_threshold_size);
}

void binary_exponential_buffer_decrement(struct binary_exponential_buffer * b, unsigned int n){
	assert(b->size >= n);
	b->size = b->size - n;
	if(b->size < (b->next_threshold_size / 4)){
		b->next_threshold_size /= 2;
		b->data = realloc(b->data, b->element_size * b->next_threshold_size);
		assert(b->data);
	}
}

unsigned int binary_exponential_buffer_size(struct binary_exponential_buffer * b){
	return b->size;
}

unsigned int binary_exponential_buffer_element_size(struct binary_exponential_buffer * b){
	return b->element_size;
}

void * binary_exponential_buffer_data(struct binary_exponential_buffer * b){
	return b->data;
}

void * binary_exponential_buffer_get(struct binary_exponential_buffer * b, unsigned int n){
	assert(n < b->size);
	return &(((char *)b->data)[n * b->element_size]);
}

void binary_exponential_buffer_destroy(struct binary_exponential_buffer * b){
	free(b->data);
	b->data = (void *)0;
	b->size = 0;
	b->element_size = 0;
	b->next_threshold_size = 1;
}

void binary_exponential_buffer_create(struct binary_exponential_buffer * b, unsigned int n){
	b->size = 0;
	b->element_size = n;
	b->next_threshold_size = 1;
	b->data = (void*)0;
}

struct binary_exponential_buffer binary_exponential_buffer_copy(struct binary_exponential_buffer * b){
	struct binary_exponential_buffer copy;
	unsigned int i;
	copy.data = malloc(b->element_size * b->next_threshold_size);
	copy.size = b->size;
	copy.element_size = b->element_size;
	copy.next_threshold_size = b->next_threshold_size;
	assert(copy.data || !b->size);
	for(i = 0; i < (b->element_size * b->size); i++){
		((char*)copy.data)[i] = ((char*)b->data)[i];
	}
	return copy;
}
