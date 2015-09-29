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
#include "../types/builtin/struct_printing_state.h"
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
