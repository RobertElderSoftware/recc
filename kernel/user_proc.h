#ifndef __USER_PROC_H__
#define __USER_PROC_H__
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

#ifndef __QUEUE_H_DEFINED__
#include "queue.h"
#endif
#ifndef __PUBLIC_KERNEL_INTERFACE_H__
#include "public_kernel_interface.h"
#endif
#ifndef __ASSERT_H_DEFINED__
#include <assert.h>
#endif
#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif

int do_compile(void);
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

#endif
