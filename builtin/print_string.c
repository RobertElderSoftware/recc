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

#include "print_buff_add.h"
#include "../generated/struct_printing_state.h"
#include "print_string.h"

void print_string(struct printing_state * ps, const char * c){
	if(c){
		while(*c){
			print_buff_add(ps, *c);
			c++;
		}
	}else{
		print_buff_add(ps, '(');
		print_buff_add(ps, 'n');
		print_buff_add(ps, 'u');
		print_buff_add(ps, 'l');
		print_buff_add(ps, 'l');
		print_buff_add(ps, ')');
	}
}
