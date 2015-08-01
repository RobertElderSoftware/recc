/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
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
