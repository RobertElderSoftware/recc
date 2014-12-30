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
#ifndef __PRIVATE_KERNEL_INTERFACE_H__
#define __PRIVATE_KERNEL_INTERFACE_H__

#include "kernel_state.h"
#include "queue.h"
#include "user_proc.h"
#include <stdio.h>

void schedule_next_task(void);
void save_current_task(struct task_queue *, enum process_state);
void save_current_task_as_ready(void);
void add_task_to_ready_queue(struct process_control_block *);

void set_irq_handler(void (*)(void));
unsigned int timer_interrupt_enable(void);
unsigned int uart1_out_interrupt_enable(void);
unsigned int uart1_in_interrupt_enable(void);
unsigned int init_task_stack(unsigned int *, void (*)(void));
unsigned int scheduler(void);
unsigned int k_release_processor(void);
void k_task_exit(void);
void k_kernel_init(void);
void irq_handler(void);
void k_irq_handler(unsigned int);
void k_block_on_event(enum kernel_event);
void k_send_message(struct kernel_message *, unsigned int, struct kernel_message *);
void k_receive_message(struct kernel_message *);
void k_reply_message(struct kernel_message *, unsigned int);

#endif
