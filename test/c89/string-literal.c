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

int output(void);

int output(void){
	unsigned int i = 0;
	const char * str = "Hello World!";
	const char * a = "Hello World!\0foo!\0boo!\0cfjksdafloo!\0fkfsdajkoo!\0fofdasjko!\0foofkds!\0foofds!\0foofdjk!\0foo!\0foo!\0foo!\0foo!\0";
	const char * b = "\x61\x62\t\n\r";
	const char * c = "a""b";
	const char * d = "a" "b";
	const char * e = "a" /* foo */ "b";
	const char * f = "a"/* foo */ "b";
	const char * g = "a" /* foo */"b";
	const char * h = "a"
	"b";
	while(i < 12){
		putchar(str[i]);
		i++;
	}
	putchar(" "[0]);
	putchar("abc"[2]);
	putchar("foo"[1]);
	putchar("fun"[2]);
	putchar("dedfun"[0]);
	printf("%s", a);
	printf("%s", &a[10]);
	printf("%s", &a[20]);
	printf("%s\n", &a[30]);
	printf("%s\n", &a[40]);
	printf("%s\n", &a[50]);
	printf("%s\n", b);
	printf("%s\n", c);
	printf("%s\n", d);
	printf("%s\n", e);
	printf("%s\n", f);
	printf("%s\n", g);
	printf("%s\n", h);
	printf("foo '""used_flags[index] = (num_blocks - i)""' in file ""\"libc/malloc.c\"""  on line ""120"".\n");
	return 0;
}


int main(void){
	output();
	return 0;
}
