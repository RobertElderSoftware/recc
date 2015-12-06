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

#include "c99libc_in_c89.h"

unsigned int c89_vsnprintf(char * buf, size_t n, const char * fmt, va_list va){
	unsigned int i = 0;
	struct printing_state ps;
	ps.buffer = buf;
	ps.chars_required = 0;
	ps.buffer_size = (unsigned int)n;
	while(fmt[i]){
		if(fmt[i] == '%'){
			/*  For now, just consume the format numbers to get to the format specifier */
			while(fmt[i+1] >= '0' && fmt[i+1] <= '9'){
				i++;
			}

			if(fmt[i+1] == 'c'){
				print_buff_add(&ps, (char)va_arg(va, int));
				i++;
			}else if(fmt[i+1] == 's'){
				print_string(&ps, va_arg(va, char *));
				i++;
			}else if((fmt[i+1] == 'd') || (fmt[i+1] == 'i')){
				print_decimal(&ps, va_arg(va, int));
				i++;
			}else if((fmt[i+1] == 'p')){
				print_buff_add(&ps, '0');
				print_buff_add(&ps, 'x');
				print_hexadecimal(&ps, va_arg(va, unsigned int), 32);
				i++;
			}else if((fmt[i+1] == 'X')){
				print_hexadecimal(&ps, va_arg(va, unsigned int), 0);
				i++;
			}else if((fmt[i+1] == 'x')){
				print_hexadecimal(&ps, va_arg(va, unsigned int), 32);
				i++;
			}else if(fmt[i+1] == '\0'){
				print_buff_add(&ps, '%');
				i++;
			}else{
				print_buff_add(&ps, (char)fmt[i+1]);
				i++;
			}
		}else{
			print_buff_add(&ps, (char)fmt[i]);
		}
		i++;
	}
	print_buff_add(&ps, '\0');
	return ps.chars_required -1;
}
