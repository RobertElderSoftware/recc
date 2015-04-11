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
#ifndef __USER_PROC_H__
#define __USER_PROC_H__

#include "queue.h"
#include "public_kernel_interface.h"
#include "assert.h"
#include <stdio.h>

void user_proc_1(void);
void user_proc_2(void);
void clock_tick_notifier(void);
void clock_server(void);
void uart1_out_ready_notifier(void);
void uart1_out_server(void);
void uart1_in_ready_notifier(void);
void uart1_in_server(void);
void command_server(void);

int putchar_nobusy(int);
int getchar_nobusy(void);
void putchar_init(void);

extern struct process_control_block * pcb_ptrs[MAX_NUM_PROCESSES];

#endif
