#ifndef __KERNEL_STATE_H__
#define __KERNEL_STATE_H__
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

struct process_control_block{
	enum process_state state;
	unsigned int * stack_pointer;
	unsigned int pid;
	unsigned int priority;
	struct message_queue messages;
	struct kernel_message * reply_message;
	struct kernel_message * recieve_message;
};

extern struct task_queue ready_queue_p0;
extern struct task_queue ready_queue_p1;
extern struct task_queue ready_queue_p2;
extern struct task_queue ready_queue;
extern struct task_queue zombie_queue;
extern struct task_queue blocked_on_clock_tick_queue;
extern struct task_queue blocked_on_uart1_out_ready_queue;
extern struct task_queue blocked_on_uart1_in_ready_queue;

extern unsigned int * g_kernel_sp;
extern unsigned int * g_current_sp;
extern unsigned int kernel_stack[STACK_SIZE];
extern struct process_control_block pcbs[MAX_NUM_PROCESSES];
extern unsigned int user_proc_1_stack[STACK_SIZE];
extern unsigned int user_proc_2_stack[STACK_SIZE];
extern unsigned int user_proc_3_stack[STACK_SIZE];
extern unsigned int user_proc_4_stack[STACK_SIZE];
extern unsigned int user_proc_5_stack[STACK_SIZE];
extern unsigned int user_proc_6_stack[STACK_SIZE];
extern unsigned int user_proc_7_stack[STACK_SIZE];
extern unsigned int user_proc_8_stack[STACK_SIZE];
extern unsigned int user_proc_9_stack[STACK_SIZE];

#endif
