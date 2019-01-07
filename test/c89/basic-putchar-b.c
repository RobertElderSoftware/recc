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

void fun(void);

void fun(void){
	return;
}

static int arr[3][3] = {
	{98,99,100},
	{101,102,103},
	{104,105,106}
};

int main(void){
	putchar('b');
	putchar(arr[0][0]);
	putchar(arr[0][1]);
	putchar(arr[0][2]);
	putchar(arr[1][0]);
	putchar(arr[1][1]);
	putchar(arr[1][2]);
	putchar(arr[2][0]);
	putchar(arr[2][1]);
	putchar(arr[2][2]);
	fun();
	return 0;
}
