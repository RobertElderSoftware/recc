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

int foo1(int);
int foo2(int);
int foo3(int);

int foo1(int i){
	(void)i;
	putchar('a');
	return 0;
}

int foo2(int i){
	foo1(i);
	putchar('a');
	return 0;
}

int foo3(int i){
	foo2(i);
	putchar('a');
	return 0;
}

int main(void){
	foo3(97);
	return 0;
}
