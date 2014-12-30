/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
int putchar(int);

int printf(const char *, ...);

void print_decimal(int);

void print_hexadecimal(unsigned int);

void print_decimal(int);

void print_hexadecimal(unsigned int d){
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
		putchar('0');
	}

	current = d;
	while(base){
		digit = current / base;
		if(digit){
			putchar(digits[digit]);
			leading_zero = 0;
		}else{
			if(!leading_zero){
				putchar(digits[digit]);
			}
		}
		a = base * digit;
		current = current - a;
		base = base / 16;
	}
}

int printf(const char * fmt, ...){
	va_list v;
	unsigned int i = 0;

	va_start(v, fmt);

	while(fmt[i]){
		if(fmt[i] == '%'){
			if(fmt[i+1] == 'c'){
				putchar(va_arg(v, int));
				i++;
			}else if((fmt[i+1] == 'd') || (fmt[i+1] == 'i')){
				print_decimal(va_arg(v, int));
				i++;
			}else if((fmt[i+1] == 'x') || (fmt[i+1] == 'X')){
				print_hexadecimal(va_arg(v, unsigned int));
				i++;
			}else if(fmt[i+1] == '\0'){
				putchar('%');
				i++;
			}else{
				putchar((unsigned int)fmt[i+1]);
				i++;
			}
		}else{
			putchar(fmt[i]);
		}
		i++;
	}
	va_end(v);
}
