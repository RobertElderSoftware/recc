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

#include "c99libc_in_c89.h"

unsigned int c89_vsnprintf(char * buf, size_t n, const char * fmt, va_list va){
	unsigned int i = 0;
	struct printing_state ps;
	ps.buffer = buf;
	ps.chars_required = 0;
	ps.buffer_size = (unsigned int)n;
	while(fmt[i]){
		char c = fmt[i];
		if(c == '%'){
			/*  For now, just consume the format numbers to get to the format specifier */
			while(fmt[i+1] >= '0' && fmt[i+1] <= '9'){
				i++;
			}

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
			i++;
		}else{
			print_buff_add(&ps, c);
		}
		i++;
	}
	print_buff_add(&ps, '\0');
	return ps.chars_required -1;
}
