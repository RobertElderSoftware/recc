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
#ifndef __OP_CPU_H__
#define __OP_CPU_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct virtual_machine;

unsigned int is_halted(struct virtual_machine *);
unsigned int vm_putc(struct virtual_machine *, unsigned int);
unsigned int vm_getc(struct virtual_machine *, unsigned int *);
void step(struct virtual_machine *);
	struct virtual_machine * vm_create(unsigned char [4], unsigned char [4], unsigned char [1][5]);
void vm_destroy(struct virtual_machine *);

#endif
