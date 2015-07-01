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

#define a_maro_234 putchar('a');
#define b_maro_234 putchar('b');
#define ab_macro343 a_maro_234 b_maro_234

#define foo() putchar('c');
#define boo(a) putchar(a);
#define koo(a,b) putchar(b); putchar(a);
#define loo(a) boo(a) koo(a,a)

#define stringify1(a) #a
#define stringify2(a) # a

#define stringify_indirect(a) stringify2(a)

#include <stdio.h>
#include <stdio.h>

int main(void){
	a_maro_234
	b_maro_234
	ab_macro343
	foo()
	boo('4')
	koo('t', 'h')
	loo('3')
	printf(stringify1(  unsigned  ));
	printf(stringify1(unsigned int));
	printf(stringify2(unsigned int));
	printf(stringify1(0 && "assertion message"));
	printf(stringify2(0 && "assertion message"));
	printf(stringify2(foo() boo('4') koo('t', 'h') loo('3') 0 && "assertion message"));
	printf(stringify_indirect(foo() boo('4') koo('t', 'h') loo('3') 0 && "assertion message"));
	printf(stringify_indirect(stringify_indirect("message")));
	printf(stringify_indirect(stringify_indirect(stringify1("message"))));
	printf(stringify_indirect(__LINE__));
	printf(stringify_indirect(__FILE__));
	return 0;
}
