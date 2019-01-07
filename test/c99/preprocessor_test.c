/*
    Copyright 2019 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/

#define stringify(a) # a

#define stringify_indirect(a) stringify(a)

#define eval(...)  eval1(eval1(eval1(__VA_ARGS__)))
#define eval1(...) eval2(eval2(eval2(__VA_ARGS__)))
#define eval2(...) __VA_ARGS__

#define empty()
#define defer(id) id empty()

#define loop() ?    defer(loop_indirect) () ()
#define loop_indirect() loop

/*  Test to make sure that a token that represents a function macro name will only get disabled if you attempt to call the function */
#define recur4(C, T, E) C-T-E #C #T #E
#define recur3(X) [ X ] #X
#define recur2(C, X) recur4(C(X), recur4(C(X), ,),) |C| #C #X
#define recur1(F, X) F(recur3, X) #F #X

/*  Test another case involving token disabling where a token is passed down into multiple places */
#define cur4(C, T, E) C-T-E #C #T #E
#define cur3(X) [ X (2)
#define f(C) C(C)
#define cur2(C, X) f(C) cur4(C(X), cur4(C(X), ,),) |C
#define cur1(F, X) F(cur3, X) #F #X

/*  Make sure you can get the last argument of an empty list sent to vaargs: */
#define empty_va_list(a,...) a
#define empty_va_list_eval(a) empty_va_list a

/*  Make sure joining with empty token lists works: */
#define empty_join_test(a,b) a##b

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
	ignore_spaces_print(stringify_indirect(__LINE__));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect( eval(loop()) ));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect( eval(loop_indirect()) ));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect( defer(loop_indirect()) ));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect( defer(loop_indirect) ));
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect( defer(loop) ));
	ignore_spaces_print("Crazy token disabling test case:\n");
	ignore_spaces_print(stringify_indirect(recur1(recur2, recur1(recur2, 1))));
	ignore_spaces_print("Another crazy token disabling test case:\n");
	/*  TODO: This case is currently failing for recc due to token disabling not being implemented correctly. */
	/*ignore_spaces_print(stringify_indirect(cur1(cur2, cur1(cur2, 1))));*/
	ignore_spaces_print("\n");
	ignore_spaces_print(stringify_indirect( defer(eval(loop_indirect()))() ));
	ignore_spaces_print("Va list case:\n");
	/* Currently causes warnings in clang
	ignore_spaces_print(stringify_indirect(empty_va_list_eval(())));
	*/
	ignore_spaces_print("Concat empty left:\n");
	ignore_spaces_print(stringify_indirect(empty_join_test(, fdasfdasfdsa)));
	ignore_spaces_print("Concat empty right:\n");
	ignore_spaces_print(stringify_indirect(empty_join_test(fjdksajflsdk,)));
	ignore_spaces_print("Concat both empty:\n");
	ignore_spaces_print(stringify_indirect(empty_join_test(,)));
	return 0;
}
