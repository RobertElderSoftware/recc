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

#include <stdio.h>

int output(void);

int output(void){
	unsigned int i = 0;
	const char * str = "Hello World!";
	while(i < 12){
		putchar(str[i]);
		i++;
	}
	putchar(" "[0]);
	putchar("abc"[2]);
	putchar("foo"[1]);
	putchar("fun"[2]);
	putchar("dedfun"[0]);
	return 0;
}


int main(void){
	output();
	return 0;
}
