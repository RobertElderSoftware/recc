/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#include <stdarg.h>

int putchar(int);

void print(unsigned int arg1, ...);

void print(unsigned int arg1, ...){
	va_list v;

	va_start(v, arg1);
	putchar((int)va_arg(v, unsigned int));
	putchar((int)va_arg(v, unsigned int));
	putchar((int)va_arg(v, unsigned int));
	putchar((int)va_arg(v, unsigned int));
	va_end(v);
}
 
int main(void){
	print(97, 98, 99, 100, 101);
	return 0;
}
