#ifndef PRIVATE_KERNEL_INTERFACE_H_
#define PRIVATE_KERNEL_INTERFACE_H_
/*
    Copyright 2019 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/

#ifndef KERNEL_STATE_H_
#include "kernel_state.h"
#endif
#ifndef QUEUE_H_DEFINED_
#include "queue.h"
#endif
#ifndef USER_PROC_H_
#include "user_proc.h"
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif

void schedule_next_task(void);
void save_current_task(struct task_queue *, enum process_state);
void save_current_task_as_ready(void);
void add_task_to_ready_queue(struct process_control_block *);

void set_irq_handler(void (*)(void));
void set_timer_period(unsigned int);
void set_level_2_page_pointer(unsigned int *);
void or_into_flags_register(unsigned int);
void deassert_bits_in_flags_register(unsigned int);
unsigned int read_flags_register(void);
unsigned int timer_interrupt_enable(void);
unsigned int uart1_out_interrupt_enable(void);
unsigned int uart1_in_interrupt_enable(void);
unsigned int page_fault_exception_interrupt_enable(void);
unsigned int init_task_stack(unsigned int **, void (*)(void));
unsigned int scheduler(void);
unsigned int k_release_processor(void);
void k_task_exit(void);
void k_kernel_exit(void);
void k_kernel_init(void);
void irq_handler(void);
void k_irq_handler(void);
void k_block_on_event(enum kernel_event);
void k_send_message(struct kernel_message *, unsigned int, struct kernel_message *);
void k_receive_message(struct kernel_message *);
void k_reply_message(struct kernel_message *, unsigned int);

#endif
