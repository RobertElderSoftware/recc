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

void irq_handler(void);
void set_irq_handler(void);
int main_div_zero_test(void);
int main(void);

void irq_handler(void){
	/*  Won't ever be executed if compiled with GCC or Clang, but will be executed inside One Page CPU. */
	printf("Floating point exception (core dumped)\n"); /*  Must match string from host, otherwise test will fail. */
}

void set_irq_handler(void){
	/*  Won't ever be executed if compiled with GCC or Clang, but will be executed inside One Page CPU. */
	void (**irq_handler_fcn_location)(void) = (void (**)(void))0x300020;
	*irq_handler_fcn_location = irq_handler;
}

int main_div_zero_test(void){
	set_irq_handler();
	return main();
}
