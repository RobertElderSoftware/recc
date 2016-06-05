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
#include "print_buff_add.h"
#include "print_decimal.h"
#include "../generated/struct_printing_state.h"

void print_decimal(struct printing_state * ps, int d){
	unsigned int leading_zero = 1;
	unsigned int base = 1000000000;
	unsigned int digit = 0;
	unsigned int unsigned_d;
	if(d == 0){
		print_buff_add(ps, '0');
	}

	if((int)0 >= d){
		if((unsigned int)d){
			print_buff_add(ps, '-');
		}
		unsigned_d = (unsigned int)-d;
	}else{
		unsigned_d = (unsigned int)d;
	}
	while(base){
		digit = unsigned_d / base;
		if(digit){
			print_buff_add(ps, (char)('0' + (int)digit));
			leading_zero = 0;
		}else{
			if(!leading_zero){
				print_buff_add(ps, (char)('0' + (int)digit));
			}
		}
		unsigned_d = unsigned_d - (base * digit);
		base /= 10;
	}
}


void print_unsigned_decimal(struct printing_state * ps, unsigned int d){
	unsigned int leading_zero = 1;
	unsigned int base = 1000000000;
	unsigned int digit = 0;
	unsigned int unsigned_d = d;
	if(d == 0){
		print_buff_add(ps, '0');
	}

	while(base){
		digit = unsigned_d / base;
		if(digit){
			print_buff_add(ps, (char)('0' + (int)digit));
			leading_zero = 0;
		}else{
			if(!leading_zero){
				print_buff_add(ps, (char)('0' + (int)digit));
			}
		}
		unsigned_d = unsigned_d - (base * digit);
		base /= 10;
	}
}
