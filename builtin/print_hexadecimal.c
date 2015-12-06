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

#include "print_buff_add.h"
#include "print_hexadecimal.h"
#include "../types/builtin/struct_printing_state.h"

void print_hexadecimal(struct printing_state * ps, unsigned int d, unsigned int letter_offset){
	unsigned int leading_zero = 1;
	unsigned int base = 0x10000000;
	unsigned int current;
	unsigned int a;
	unsigned int digit = 0;
	unsigned int digits[16];
	digits[0] = '0';
	digits[1] = '1';
	digits[2] = '2';
	digits[3] = '3';
	digits[4] = '4';
	digits[5] = '5';
	digits[6] = '6';
	digits[7] = '7';
	digits[8] = '8';
	digits[9] = '9';
	digits[10] = 'A' + letter_offset;
	digits[11] = 'B' + letter_offset;
	digits[12] = 'C' + letter_offset;
	digits[13] = 'D' + letter_offset;
	digits[14] = 'E' + letter_offset;
	digits[15] = 'F' + letter_offset;
	if(d == 0){
		print_buff_add(ps, '0');
	}

	current = d;
	while(base){
		digit = current / base;
		if(digit){
			print_buff_add(ps, (char)digits[digit]);
			leading_zero = 0;
		}else{
			if(!leading_zero){
				print_buff_add(ps, (char)digits[digit]);
			}
		}
		a = base * digit;
		current = current - a;
		base = base / 16;
	}
}
