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
