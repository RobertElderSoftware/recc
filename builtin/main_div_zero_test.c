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

void irq_handler(void);
void set_irq_handler(void);
int main_div_zero_test(void);
int main(void);

void irq_handler(void){
	/*  Won't ever be executed if compiled with GCC or Clang, but will be executed inside One Page CPU. */
	printf("Floating point exception (core dumped)\n"); /*  Must match string from host, otherwise test will fail. */
}

void set_irq_handler(void){
	/*  Won't ever be executed if compiled with GCC or Clang, but will be executed inside One Page CPU. */
	void (**irq_handler_fcn_location)(void) = (void (**)(void))0x300020;
	*irq_handler_fcn_location = irq_handler;
}

int main_div_zero_test(void){
	set_irq_handler();
	return main();
}
