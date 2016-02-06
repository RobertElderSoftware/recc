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
