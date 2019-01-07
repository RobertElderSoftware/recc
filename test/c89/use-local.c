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

int output(void);

struct foo{
	int arr[2];
	int k;
};

void loo(struct foo);

void loo(struct foo f){
	f.arr[0] = 'e';
	f.arr[1] = 'f';
	f.k = 'g';
	(void)f;
}

struct foo makefoo(void);

struct foo makefoo(void){
	struct foo f;
	f.arr[0] = 'e';
	f.arr[1] = 'f';
	f.k = 'g';
	return f;
}

int output(void){
	struct foo foost;
	struct foo newfoo;
	int a1;
	int a = 'a';
	int b = 'b';
	int c = 'c';
	int d = 'd';
	int b2;
	int e = 'e', foo = 'f';
	int f = 'f';
	int g = 'g', boo = 'b', goo = 'g';
	int h = 'h';
	int i = 'i';
	int c3;
	putchar(a);
	putchar(b);
	a1 = '1';
	putchar(c);
	putchar(d);
	putchar(e);
	putchar(f);
	b2 = '2';
	putchar(g);
	putchar(h);
	putchar(i);
	putchar(foo);
	putchar(boo);
	putchar(goo);
	c3 = '3' - 1;
	putchar(a1);
	putchar(b2);
	putchar(c3);
	foost.arr[0] = 'a';
	foost.arr[1] = 'b';
	foost.k = 'b';
	putchar(foost.arr[0]);
	putchar(foost.arr[1]);
	putchar(foost.k);
	loo(foost);
	putchar(foost.arr[0]);
	putchar(foost.arr[1]);
	putchar(foost.k);
	newfoo = makefoo();
	putchar(newfoo.arr[0]);
	putchar(newfoo.arr[1]);
	putchar(newfoo.k);
	return 0;
}


int main(void){
	output();
	return 0;
}
