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

#define HEAD() a
#define CURRY(M, L) M L
#define FST(L) CURRY(HEAD, L)

/*  See if you can call a function from the automatic concatenation: */
#define TEDY() a4jkfjk
#define TED() a
#define CURRY_other(M, L) M L()
#define FST_other(L) CURRY_other(TED, L)

#define other_stringify(a) # a
#define other_stringify_indirect(a) other_stringify(a)

#define test234(x) example x
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

#define test_concat_1(a,b)  a ## b
#define test_concat_2(a ,b)  a## b
#define test_concat_3(  a,b)  a##b
#define test_concat_4(a,b )  a ##b
#define test_concat_5(a,b ,c)  a ##b ##c
#define test_concat_6(a,b ,c)  a##b ##c
#define test_concat_7( a, b ,c)  a##b## c
#define test_concat_8( a)  a##89

#define concat_test_broke_helper1(X) 1
#define concat_test_broke_helper2 1234567
#define concat_test_broke1(C, T) lol ## C(T)
#define concat_test_broke2(C, T) lol ## C

#define random_object_macro1234 345454
#define random_function_macro1234(x) 7453454
#define random_function_macro1(x) 7543534
#define random_function_(x) 753343534
#define put_side_by_side(x,y) x y
#define put_side_by_side_and_try_to_call(x,y) x y(99)
#define put_side_by_side2(x,y,z) x y z
#define put_side_by_side3(x,z) x hghghghg z

/*  Make sure joined items are not argument pre-expanded first */
#define join_test_joiner(X, Y) X##Y
#define join_test_1 4hfu44fu4dk

#include <stdio.h>
#include <stdio.h>

void ignore_spaces_print(const char *);
void ignore_spaces_print(const char * str){
	/*  There are just too many preprocessing whitespaces differences (bugs?) between different compilers. */
	/*  Currently not considering spaces in diffs. */
	const char * c = str;
	while(*c){
		if(*c != ' '){
			printf("%c", *c);
		}
		c++;
	}
}

int main(void){
	a_maro_234
	b_maro_234
	ab_macro343
	foo()
	boo('4')
	koo('t', 'h')
	loo('3')
	ignore_spaces_print(stringify1(  unsigned  ));
	ignore_spaces_print(stringify1(unsigned int));
	ignore_spaces_print(stringify2(unsigned int));
	ignore_spaces_print(stringify1(0 && "assertion message"));
	ignore_spaces_print(stringify2(0 && "assertion message"));
	ignore_spaces_print(stringify2(foo() boo('4') koo('t', 'h') loo('3') 0 && "assertion message"));
	ignore_spaces_print(stringify_indirect(foo() boo('4') koo('t', 'h') loo('3') 0 && "assertion message"));
	ignore_spaces_print(stringify_indirect(stringify_indirect("message")));
	ignore_spaces_print(stringify_indirect(stringify_indirect(stringify1("message"))));
	ignore_spaces_print(stringify_indirect(__LINE__));
	ignore_spaces_print(stringify_indirect(__FILE__));
	ignore_spaces_print("\n");

	ignore_spaces_print(stringify_indirect( test234(test234) (2) ));
	ignore_spaces_print("\n");

	ignore_spaces_print(stringify_indirect(test_concat_1(42,99)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(test_concat_2(42,99)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(test_concat_3(42,99)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(test_concat_4(42,99)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(test_concat_5(42,38, 88)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(test_concat_6(42,38, 88)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(test_concat_7(42,38, 88)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(test_concat_8(42)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke1(asdf, foo)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke1(concat_test_broke_helper1, foo)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke1(concat_test_broke_helper2, foo)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke2(concat_test_broke_helper1, foo)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke2(concat_test_broke_helper2, foo)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke2(concat_test_broke_helper2 a b c, foo)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke2(concat_test_broke_helper2 a b c, foo a)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke2(concat_test_broke_helper1 a b c, foo a)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(concat_test_broke1(concat_test_broke_helper1 a b c, foo a)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side(not_a_macro, random_object_macro1234)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side(random_object_macro1234, not_a_macro)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side(random_function_macro1, not_a_macro)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side(random_function_macro1, random_function_macro1234)));
	ignore_spaces_print("Random identifiers:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side_and_try_to_call(dsfkjfjsld, fjdksjfkd))); /* Concat would make a function */
	ignore_spaces_print("Concat function with number:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side_and_try_to_call(random_function_macro1, 234))); /* Concat would make a function */
	ignore_spaces_print("Concat function with identifier:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side_and_try_to_call(random_function_macr, o1234))); /* Concat would make a function */
	ignore_spaces_print("Concat function with identifier:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side_and_try_to_call(random_function_, macro1234))); /* Concat would make a function */
	ignore_spaces_print("Concat function with identifier:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side(random_function_, macro1234))); /* Concat would make a function */
	ignore_spaces_print("Automatic token concat function with number:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side_and_try_to_call(random_object_macro, 1234)));
	ignore_spaces_print("Automatic token concat function with identifier:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side_and_try_to_call(random_object_macr, o1234)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_object_macro, 1234,random_function_macro1)));
	ignore_spaces_print("What about this case?:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1, random_object_macro,random_function_macro1)));
	ignore_spaces_print("Next is object macro:\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1,random_object_macro,random_function_macro1)));
	ignore_spaces_print("Next is object fcn then obj\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1, random_function_macro1 random_object_macro,random_function_macro1)));
	ignore_spaces_print("Next is fcn then fcn\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1,random_function_macro1 random_function_macro1,random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1,blah notmac,random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1, blah notmac,random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1,blah random_function_macro1,random_function_macro1)));
	ignore_spaces_print("Crazy case with leading space: \n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1, blah random_function_macro1,random_function_macro1)));
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1, random_function_macro1,random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1,random_function_macro1,random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(fjsdkf,fjds,fkdfd)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(fjsdkf,random_function_macro1,fkdfd)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1, fkdfd , random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(fjsdkf random_function_macro1,random_function_macro1,fkdfd)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(fjsdkf,random_function_macro1 random_function_macro1,fkdfd)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(fjsdkf,random_function_macro1 fjsdkf,fkdfd)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1,random_function_macro1,random_function_macro1)));
	ignore_spaces_print("Start with space: \n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2( random_function_macro1,random_function_macro1,random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1, 234,random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1,234,random_function_macro1)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side2(random_function_macro1, abc,random_function_macro1234)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side3(random_function_macro1,random_function_macro1234)));
	ignore_spaces_print("\n");
        ignore_spaces_print(stringify_indirect(put_side_by_side3(random_function_macro1, random_function_macro1234)));
	ignore_spaces_print("\n");
	ignore_spaces_print(other_stringify_indirect(FST(4)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(FST(4)));
	ignore_spaces_print("\n");
	ignore_spaces_print(other_stringify_indirect(FST_other(3)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(FST_other(Y)));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect(join_test_joiner(join_test_1, X)));


	ignore_spaces_print("\n");
	return 0;
}
