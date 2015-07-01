#ifndef __CORE_DATA_TYPES_H
#define __CORE_DATA_TYPES_H
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

#define STACK_SIZE 100000
#define MAX_NUM_PROCESSES 10

enum process_state {BLOCKED_ON_SEND, BLOCKED_ON_RECEIVE, BLOCKED_ON_REPLY, BLOCKED_ON_CLOCK_TICK, BLOCKED_ON_UART1_IN_READY, BLOCKED_ON_UART1_OUT_READY, READY, ACTIVE, ZOMBIE};

enum kernel_event {CLOCK_TICK_EVENT, UART1_OUT_READY, UART1_IN_READY};

enum kernel_message_type {UART1_IN_READY_NOTIFY, UART1_OUT_READY_NOTIFY, CLOCK_TICK_NOTIFY, MESSAGE_ACKNOWLEDGED, OUTPUT_CHARACTER};

struct kernel_message;

struct kernel_message{
	enum kernel_message_type message_type;
	unsigned int data;
	unsigned int source_id;
};

#endif
