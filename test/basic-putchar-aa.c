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

struct km {
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int source_id;
};

void foo2(unsigned int, struct km, unsigned int);
void foo2(unsigned int g, struct km m, unsigned int h){
	putchar((int)m.source_id);
	putchar((int)g);
	putchar((int)h);
}

int main(void){
	struct km m;
	putchar('a');
	putchar('a');
	putchar('a');
	putchar('a');
	putchar('a');
	putchar('a');
	putchar('a');
	putchar('a');
	putchar('a');
	m.source_id = 'z';
	m.a = 'a';
	m.b = 'b';
	m.c = 'c';
	foo2('g', m, 'h');
	return 0;
}
