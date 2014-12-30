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
#include "kernel_state.h"
#include "queue.h"

struct task_queue ready_queue_p0;
struct task_queue ready_queue_p1;
struct task_queue ready_queue_p2;
struct task_queue ready_queue;
struct task_queue zombie_queue;
struct task_queue blocked_on_clock_tick_queue;
struct task_queue blocked_on_uart1_out_ready_queue;
struct task_queue blocked_on_uart1_in_ready_queue;
struct task_queue blocked_on_send_queue;
struct task_queue blocked_on_receive_queue;
struct task_queue blocked_on_reply_queue;

unsigned int g_current_sp;
unsigned int g_kernel_sp;

struct process_control_block pcbs[MAX_NUM_PROCESSES];

unsigned int kernel_stack[STACK_SIZE];     /*  8K Space for our kernel stack (1 int item takes up 8 bytes in memory) */
unsigned int kernel_stack_start;     /*  Relies on adjacent items beging layed out adjacent in memory */

unsigned int user_proc_1_stack[STACK_SIZE];
unsigned int user_proc_1_stack_start;

unsigned int user_proc_2_stack[STACK_SIZE];
unsigned int user_proc_2_stack_start;

unsigned int user_proc_3_stack[STACK_SIZE];
unsigned int user_proc_3_stack_start;

unsigned int user_proc_4_stack[STACK_SIZE];
unsigned int user_proc_4_stack_start;

unsigned int user_proc_5_stack[STACK_SIZE];
unsigned int user_proc_5_stack_start;

unsigned int user_proc_6_stack[STACK_SIZE];
unsigned int user_proc_6_stack_start;

unsigned int user_proc_7_stack[STACK_SIZE];
unsigned int user_proc_7_stack_start;

unsigned int user_proc_8_stack[STACK_SIZE];
unsigned int user_proc_8_stack_start;

unsigned int user_proc_9_stack[STACK_SIZE];
unsigned int user_proc_9_stack_start;
