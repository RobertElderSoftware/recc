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
#include "stdlib.h"
#include "stdio.h"

extern unsigned int free_mem_start;
extern unsigned int free_mem_end;

void * malloc(size_t);

void * malloc(size_t){
	unsigned int heap_size = free_mem_end - free_mem_start;
	/*printf("Size is %d byes\n", (free_mem_end - free_mem_start));*/
}
