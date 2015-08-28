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
	const char * a = "Hello World!\0foo!\0boo!\0cfjksdafloo!\0fkfsdajkoo!\0fofdasjko!\0foofkds!\0foofds!\0foofdjk!\0foo!\0foo!\0foo!\0foo!\0";
	const char * b = "\x61\x62\t\n\r";
	const char * c = "a""b";
	const char * d = "a" "b";
	const char * e = "a" /* foo */ "b";
	const char * f = "a"/* foo */ "b";
	const char * g = "a" /* foo */"b";
	const char * h = "a"
	"b";
	while(i < 12){
		putchar(str[i]);
		i++;
	}
	putchar(" "[0]);
	putchar("abc"[2]);
	putchar("foo"[1]);
	putchar("fun"[2]);
	putchar("dedfun"[0]);
	printf("%s", a);
	printf("%s", &a[10]);
	printf("%s", &a[20]);
	printf("%s\n", &a[30]);
	printf("%s\n", &a[40]);
	printf("%s\n", &a[50]);
	printf("%s\n", b);
	printf("%s\n", c);
	printf("%s\n", d);
	printf("%s\n", e);
	printf("%s\n", f);
	printf("%s\n", g);
	printf("%s\n", h);
	printf("foo '""used_flags[index] = (num_blocks - i)""' in file ""\"libc/malloc.c\"""  on line ""120"".\n");
	return 0;
}


int main(void){
	output();
	return 0;
}
