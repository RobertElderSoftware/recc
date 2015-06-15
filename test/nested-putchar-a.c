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

int foo1(void);
int foo2(void);
int foo3(void);

int foo1(void){
	putchar('a');
	return 0;
}

int foo2(void){
	foo1();
	putchar('a');
	return 0;
}

int foo3(void){
	foo2();
	putchar('a');
	return 0;
}

int main(void){
	foo3();
	return 0;
}
