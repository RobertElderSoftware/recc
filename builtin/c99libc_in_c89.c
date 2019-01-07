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

#include "c99libc_in_c89.h"

unsigned int c89_vsnprintf(char * buf, size_t n, const char * fmt, va_list va){
	unsigned int i = 0;
	struct printing_state ps;
	unsigned int zero_pads = 0;
	unsigned int zero_padding_start = 0;
	ps.buffer = buf;
	ps.chars_required = 0;
	ps.buffer_size = (unsigned int)n;
	while(fmt[i]){
		char c = fmt[i];
		if(c == '%'){
			/*  TODO:  This currently ignores flags and always assumes left padding is zero padding. */
			/*  Consume the format numbers to get to get the number of spaces to left pad. */
			while(fmt[i+1] >= '0' && fmt[i+1] <= '9'){
				zero_pads = (zero_pads * 10) + ((unsigned int)fmt[i+1] - (unsigned int)'0');
				i++;
			}

			zero_padding_start = ps.chars_required;

			switch(fmt[i+1]){
				case 'c':{
					print_buff_add(&ps, (char)va_arg(va, int));
					break;
				}case 's':{
					print_string(&ps, va_arg(va, char *));
					break;
				}case 'd':{
					print_decimal(&ps, va_arg(va, int));
					break;
				}case 'i':{
					print_decimal(&ps, va_arg(va, int));
					break;
				}case 'u':{
					print_unsigned_decimal(&ps, va_arg(va, unsigned int));
					break;
				}case 'p':{
					print_buff_add(&ps, '0');
					print_buff_add(&ps, 'x');
					print_hexadecimal(&ps, va_arg(va, unsigned int), 32);
					break;
				}case 'X':{
					print_hexadecimal(&ps, va_arg(va, unsigned int), 0);
					break;
				}case 'x':{
					print_hexadecimal(&ps, va_arg(va, unsigned int), 32);
					break;
				}case '\0':{
					print_buff_add(&ps, '%');
					break;
				}default:{
					print_buff_add(&ps, (char)fmt[i+1]);
				}
			}

			if(zero_pads){
				print_buff_left_zero_pad(&ps, zero_padding_start, zero_pads);
			}
			zero_pads = 0; /*  Reset zero padding thing */
			i++;
		}else{
			print_buff_add(&ps, c);
		}
		i++;
	}
	print_buff_add(&ps, '\0');
	return ps.chars_required -1;
}
