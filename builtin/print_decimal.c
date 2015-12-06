/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#include <stdio.h>
#include "print_buff_add.h"
#include "print_decimal.h"
#include "../types/builtin/struct_printing_state.h"

void print_decimal(struct printing_state *, int);

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
