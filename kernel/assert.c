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

#include "assert.h"

void assert(unsigned int i, const char * str){
	unsigned char * c = (unsigned char *)str;
	const char * msg = "ASSERTION FAILURE!!! :  ";
	unsigned char * d = (unsigned char *)msg;
	if(!i){
		while(*d){
			putchar_nobusy(*d);
			d = d + 1;
		}
		while(*c){
			putchar_nobusy(*c);
			c = c + 1;
		}
		while(1){ /*Hang forever*/ }
	}
}
