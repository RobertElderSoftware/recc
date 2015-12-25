/*
    Copyright 2015 Robert Elder Software Inc.
    
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

int foo1(int, int, int, int, int, int, int, int, int, int, int, int);
int foo2(int, int, int, int, int, int, int, int, int, int, int, int);
int foo3(int, int, int, int, int, int, int, int, int, int, int, int);

int foo1(int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t){
	putchar(i);
	putchar(j);
	putchar(k);
	putchar(l);
	putchar(m);
	putchar(n);
	putchar(o);
	putchar(p);
	putchar(q);
	putchar(r);
	putchar(s);
	putchar(t);
	return 0;
}

int foo2(int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t){
	foo1(i, j, k, l, m, n, o, p, q, r, s, t);
	return 0;
}

int foo3(int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t){
	foo2(i, j, k, l, m, n, o, p, q, r, s, t);
	return 0;
}

int main(void){
	foo3('h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!');
	return 0;
}
