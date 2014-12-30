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
int putchar(int);

int foo1(int);
int foo2(int);
int foo3(int);

int foo1(int i){
	(void)i;
	putchar('a');
	return 0;
}

int foo2(int i){
	foo1(i);
	putchar('a');
	return 0;
}

int foo3(int i){
	foo2(i);
	putchar('a');
	return 0;
}

int main(void){
	foo3(97);
	return 0;
}
