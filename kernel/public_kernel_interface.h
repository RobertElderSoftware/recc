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
#ifndef __PUBLIC_KERNEL_INTERFACE_H__
#define __PUBLIC_KERNEL_INTERFACE_H__

#include "kernel_state.h"
#include "queue.h"

unsigned int release_processor(void);
void task_exit(void);
void kernel_init(void);
void kernel_exit(void);
void block_on_event(enum kernel_event);
void send_message(struct kernel_message *, unsigned int, struct kernel_message *);
void receive_message(struct kernel_message *);
void reply_message(struct kernel_message *, unsigned int);

#endif
