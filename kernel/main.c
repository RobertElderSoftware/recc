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
#include "public_kernel_interface.h"
#include "kernel_state.h"

int putchar(int);

int main(void){
	/*  Need to set the kernel stack pointer before we can make any kernel calls */
	g_kernel_sp = &kernel_stack[STACK_SIZE -1];
	kernel_init(); /*  This method will block until the kernel exits */
	return 0;
}
