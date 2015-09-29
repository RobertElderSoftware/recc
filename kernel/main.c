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
#include "public_kernel_interface.h"
#include "kernel_state.h"

int putchar(int);

int main(void){
	/*  Need to set the kernel stack pointer before we can make any kernel calls */
	g_kernel_sp = &kernel_stack[STACK_SIZE -1];
	kernel_init(); /*  This method will block until the kernel exits */
	return 0;
}
