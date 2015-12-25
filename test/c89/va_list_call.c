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

#include <stdarg.h>
#include <stdio.h>

void print(unsigned int arg1, ...);

void print(unsigned int arg1, ...){
	va_list v;

	va_start(v, arg1);
	putchar((int)va_arg(v, unsigned int));
	putchar((int)va_arg(v, unsigned int));
	putchar((int)va_arg(v, unsigned int));
	putchar((int)va_arg(v, unsigned int));
	va_end(v);
}
 
int main(void){
	print(97, 98, 99, 100, 101);
	return 0;
}
