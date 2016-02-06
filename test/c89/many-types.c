/*
    Copyright 2016 Robert Elder Software Inc.
    
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

#include <stdio.h>

struct k {
int i;
};

union j {
int k;
};

struct a {
  struct b{
    int i;
  } g;
};

struct ggg {long g;};
extern struct ggg foo;
struct ggg foo;

struct c {
	char i;
	char ff;
	short j;
	float l;
	double m;
	long k;
	long n;
};

extern struct c boo;
struct c boo;

extern char ab[3];
extern float ac[3];
extern short ad[3];
extern int ae[3];
extern long af[3];
extern long ag[3];

extern char ****** ah; 
extern int ****** ai; 

int foo1(int);
int foo1(int foo1param){
	return foo1param;
}

int (*foo2(void))(int i);
int (*foo2(void))(int i){
	return foo1;
}

int (*(*foo3(void))(void))(int i);
int (*(*foo3(void))(void))(int i){
	return foo2;
}

int (*(*(*foo4(void))(void))(void))(int i);
int (*(*(*foo4(void))(void))(void))(int i){
	return foo3;
}

int (*(*(*(*foo5(void))(void))(void))(void))(int i);
int (*(*(*(*foo5(void))(void))(void))(void))(int i){
	return foo4;
}

struct foo1{
	unsigned int i:20;
	unsigned int :1;
};


int no_param_names(int, int, int);
int some_param_names(int a, int b, int);
int all_param_names(int a, int b, int c);

enum en1 {num1};

enum en2 {num2, num3, num4};

enum {anon2, anon3, anon4};

enum eninit {num9 = 1, num99, num999};

static int one;
static int two = 2;

/*int three = 3, c(void), ****const* four = (int ****const*)4;*/

void elip(int a, ...);

void elip(int a, ...){
(void)a;
}


typedef int just_int;

typedef int (*(*(*(*foo12(void))(void))(void))(void))(short i);

int main(void){
	(void)one;
	(void)two;
	putchar(foo5()()()()('Z'));
	putchar('a');
	return 0;
}
