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

#include "printf_busy.h"

int printf_busy(const char * fmt, ...){
	va_list arglist;
	int rtn = 0;
	va_start(arglist, fmt);
	rtn = vprintf_busy(fmt, arglist);
	va_end( arglist );
	return rtn;
}

int vprintf_busy(const char * fmt, va_list va){
	/*  Keep trying to print safely using the stack as temporary storage: */
	char try1[4];
	char *result1 = &try1[0];
	unsigned int chars_required = vsnprintf(&try1[0], 4, fmt, va);
	if(chars_required < 4){
		while(*result1){
			putchar_busy(*result1);
			result1++;
		}
	}else{
		char try2[16];
		unsigned int chars_required = vsnprintf(&try2[0], 16, fmt, va);
		char * result2 = &try2[0];
		if(chars_required < 16){
			while(*result2){
				putchar_busy(*result2);
				result2++;
			}
		}else{
			char try3[200];
			unsigned int chars_required = vsnprintf(&try3[0], 200, fmt, va);
			char * result3 = &try3[0];
			if(chars_required < 200){
				while(*result3){
					putchar_busy(*result3);
					result3++;
				}
			}else{
				printf_busy("Fail busy print...");
			}
		}
	}
	return 0;
}
