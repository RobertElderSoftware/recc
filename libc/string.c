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

#include "string.h"

size_t strlen(const char * c){
	unsigned int i = 0;
	while(c[i]){
		i++;
	}
	return i;
}

int strcmp (const char * s1, const char * s2){
	unsigned int i = 0;
	int rtn;
	char abc;
	while(s1[i] && s2[i]){
		char c = s1[i] - s2[i];
		if(c){
			rtn = c;
			return rtn;
		}
		i++;
	}
	abc = s1[i] - s2[i];
	rtn = abc;
	return rtn;
}


void * memcpy(void * dest, const void * src, size_t bytes){
	unsigned int remain = bytes % sizeof(unsigned int);
	void * dest_return = dest;
	/*  If the memory locations are 4 byte aligned */
	if(((unsigned int)src) % sizeof(unsigned int) == 0 && ((unsigned int)dest) % sizeof(unsigned int) == 0){
		unsigned int num_words = bytes / sizeof(unsigned int);
		if(num_words){
			unsigned int * d = (unsigned int *)dest;
			unsigned int * s = (unsigned int *)src;
			unsigned int i;
			for(i = 0; i < num_words; i++){
				*d = *s;
				d++;
				s++;
			}
			dest = d;
			src = s;
		}
	}else{
		remain = bytes;
	}
	if(remain){
		char * d = (char *)dest;
		char * s = (char *)src;
		unsigned int i;
		for(i = 0; i < remain; i++){
			*d = *s;
			d++;
			s++;
		}
	}
	return dest_return;
}
