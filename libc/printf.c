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

#include <assert.h>
#include <stdarg.h>

char printfbuffer[1048576];

int putchar(int);

int printf(const char *, ...);

void print_hexadecimal(char **, unsigned int);

void print_decimal(char **, int);

void print_string(char **, const char *);

void print_hexadecimal(char ** buf, unsigned int d){
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
	digits[10] = 'A';
	digits[11] = 'B';
	digits[12] = 'C';
	digits[13] = 'D';
	digits[14] = 'E';
	digits[15] = 'F';
	if(d == 0){
		(*(*buf)) = '0';
		(*buf) = (*buf) + 1;
	}

	current = d;
	while(base){
		digit = current / base;
		if(digit){
			(*(*buf)) = (char)digits[digit];
			(*buf) = (*buf) + 1;
			leading_zero = 0;
		}else{
			if(!leading_zero){
				(*(*buf)) = (char)digits[digit];
				(*buf) = (*buf) + 1;
			}
		}
		a = base * digit;
		current = current - a;
		base = base / 16;
	}
}

void print_string(char ** buf, const char * c){
	while(*c){
		(*(*buf)) = *c;
		c++;
		(*buf) = (*buf) + 1;
	}
}

int printf(const char * fmt, ...){
	va_list arglist;
	char *result = &printfbuffer[0];
	int rtn;
	va_start(arglist, fmt);
	rtn = vsprintf(&printfbuffer[0], fmt, arglist);
	va_end( arglist );
	while(*result) {
		putchar(*result);
		result++;
	}
	return rtn;
}

int vsprintf(char * buf, const char * fmt, va_list va){
	unsigned int i = 0;
	while(fmt[i]){
		if(fmt[i] == '%'){
			if(fmt[i+1] == 'c'){
				*buf = (char)va_arg(va, char);
				buf++;
				i++;
			}else if(fmt[i+1] == 's'){
				print_string(&buf, va_arg(va, char *));
				i++;
			}else if((fmt[i+1] == 'd') || (fmt[i+1] == 'i')){
				print_decimal(&buf, va_arg(va, int));
				i++;
			}else if((fmt[i+1] == 'p') || (fmt[i+1] == 'P')){
				*buf = '0';
				buf++;
				*buf = 'x';
				buf++;
				print_hexadecimal(&buf, va_arg(va, unsigned int));
				i++;
			}else if((fmt[i+1] == 'x') || (fmt[i+1] == 'X')){
				print_hexadecimal(&buf, va_arg(va, unsigned int));
				i++;
			}else if(fmt[i+1] == '\0'){
				*buf = '%';
				buf++;
				i++;
			}else{
				*buf = (char)fmt[i+1];
				buf++;
				i++;
			}
		}else{
			*buf = (char)fmt[i];
			buf++;
		}
		i++;
	}
	*buf = '\0';
	buf++;
	return 0;
}

int sprintf(char * buf, const char * fmt, ...){
	va_list arglist;
	int rtn;
	va_start(arglist, fmt);
	rtn = vsprintf(buf, fmt, arglist);
	va_end(arglist);
	return rtn;
}
