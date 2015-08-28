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

void test(int *, int);

void test1(int, int *);

void test1(int b, int * a){
	*a = *a + 1;
	b = b + 1;
	putchar(*a);
	putchar(b);
	(void)(*a)++;
	putchar(*a);
	(void)(*a)++;
	putchar(*a);
	(void)(*a)++;
	putchar(*a);
	(void)(*a)++;
	putchar(*a);
	(void)(*a)++;
	putchar(*a);
	putchar((*a)++);
	putchar((*a)++);
	putchar((*a)++);
	putchar((*a)++);
	putchar((*a)++);
	putchar((*a)++);
}

void test(int * a, int b){
	*a = *a + 1;
	b = b + 1;
	putchar(*a);
	putchar(b);
	(void)(*a)++;
	putchar(*a);
	(void)(*a)++;
	putchar(*a);
	(void)(*a)++;
	putchar(*a);
	(void)(*a)++;
	putchar(*a);
	(void)(*a)++;
	putchar(*a);
	putchar((*a)++);
	putchar((*a)++);
	putchar((*a)++);
	putchar((*a)++);
	putchar((*a)++);
	putchar((*a)++);
	test1(b,a);
}

int output(void){
	int i = 0;
	int j = 97;
	int k = 97;
	i++;
	putchar('a' + (0));
	putchar(j);
	test(&j,k);
	putchar(j);
	return 0;
}

int main(void){
	output();
	return 0;
}
