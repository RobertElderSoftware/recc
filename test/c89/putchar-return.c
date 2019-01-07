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

#include <stdio.h>

int foo2(int);
int output(int,int,int,int,int,int);

int foo2(int i){
	return i;
}

struct mystruct;

struct coolstruct;

struct coolstruct {
	int t;
	int d;
	int e;
	int f;
	int g;
	int arr[4];
	int h;
};

struct awesome{
	struct coolstruct k;
	int a;
	int b;
	int c;
	int d;
};



struct mystruct {
	int arr[3];
	int j;
	struct awesome b;
	struct awesome a;
	int nn;
	struct awesome arr2[2][3];
	struct awesome c;
	struct mouse{
		int ch;
	}mou;
};
static struct mystruct g;

struct mystruct;

int output(int k, int j, int a,int b,int c,int d){
	putchar(k);
	putchar(j);
	putchar(a);
	putchar(b);
	putchar(c);
	putchar(d);
	return 0;
}

void output_mystruct(struct mystruct *);

void output_mystruct(struct mystruct * p){
	putchar(p->arr2[0][0].a);
	putchar(p->arr2[0][1].b);
	putchar(p->arr2[0][2].a);
	putchar(p->arr2[1][0].b);
	putchar(p->arr2[1][1].a);
	putchar(p->arr2[1][2].b);


	putchar((*p).arr2[0][0].a);
	putchar((*p).arr2[0][1].b);
	putchar((*p).arr2[0][2].a);
	putchar((*p).arr2[1][0].b);
	putchar((*p).arr2[1][1].a);
	putchar((*p).arr2[1][2].b);
}

int main(void){
	struct mystruct s;
	struct coolstruct cc;
	struct coolstruct * pnt;
	struct mystruct * ppp;
	struct coolstruct * ttt;
	struct coolstruct cools[4];
	struct mystruct new_s;
	(void)g;
	cc.h = '5';
	ppp = &s;
	pnt = &cc;
	ttt = &s.a.k;
	ttt->t = 'X';
	cools[0].t = 'a';
	cools[1].t = 'b';
	cools[2].t = 'c';
	cools[3].t = 'd';

	s.arr2[0][0].a = 'z';
	s.arr2[0][1].b = 'x';
	s.arr2[0][2].a = 'c';
	s.arr2[1][0].b = 'v';
	s.arr2[1][1].a = 'b';
	s.arr2[1][2].b = 'n';

	putchar(s.arr2[0][0].a);
	putchar(s.arr2[0][1].b);
	putchar(s.arr2[0][2].a);
	putchar(s.arr2[1][0].b);
	putchar(s.arr2[1][1].a);
	putchar(s.arr2[1][2].b);
	output_mystruct(&s);
	{
		struct mystruct {signed int nee; };
		struct mystruct d;
		s.mou.ch = 'Y';
		s.arr[0] = 'x';
		s.arr[1] = 'y';
		s.a.k.arr[0] = '0';
		s.a.k.arr[1] = '1';
		s.a.k.arr[2] = '2';
		s.a.k.arr[3] = '3';
		s.a.k.d = 'd';
		s.a.k.e = 'e';
		s.a.k.f = 'f';
		s.a.k.g = 'g';
		s.a.k.h = 'h';
		s.a.a = 'D';
		s.a.b = '<';
		s.a.c = '_';
		s.a.d = '3';
		s.nn = '5';
		s.j = 'L';
		d.nee = '3';
		putchar(d.nee);
	}
	{
		struct money {int eee;}m;
		struct mystruct {signed int j; };
		struct mystruct d;
		d.j = '2';
		m.eee = '1';
		putchar(d.j);
		putchar(m.eee);
	}
	new_s = s;

	putchar(new_s.mou.ch);
	putchar(new_s.arr[0]);
	putchar(new_s.arr[1]);
	putchar(new_s.j);
	putchar(new_s.a.a);
	putchar(new_s.a.b);
	putchar(new_s.a.c);
	putchar(new_s.a.d);
	putchar(new_s.a.k.arr[0]);
	putchar(new_s.a.k.arr[1]);
	putchar(new_s.a.k.arr[2]);
	putchar(new_s.a.k.arr[3]);


	putchar(s.mou.ch);
	putchar(s.arr[0]);
	putchar(s.arr[1]);
	putchar(s.j);
	putchar(s.a.a);
	putchar(s.a.b);
	putchar(s.a.c);
	putchar(s.a.d);
	putchar(s.a.k.arr[0]);
	putchar(s.a.k.arr[1]);
	putchar(s.a.k.arr[2]);
	putchar(s.a.k.arr[3]);
	putchar(ppp->a.k.arr[3]);
	putchar(ppp->a.k.d);
	putchar(ttt->d);
	putchar(ttt->arr[3]);
	putchar(s.a.k.d);
	putchar(s.a.k.e);
	putchar(s.a.k.f);
	putchar(s.a.k.g);
	putchar(s.a.k.h);
	putchar(pnt->h);
	putchar(s.a.k.t);
	putchar(cools[0].t);
	putchar(cools[1].t);
	putchar(cools[2].t);
	putchar(cools[3].t);
	output('a', foo2('b'), foo2(foo2('a')),foo2(foo2(foo2('b'))),foo2(foo2(foo2('c'))),foo2(foo2(foo2(foo2('d')))));
	return 0;
}
