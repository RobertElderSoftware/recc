#ifndef KERNEL_STATE_H_
#define KERNEL_STATE_H_
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

#ifndef QUEUE_H_DEFINED_
#include "queue.h"
#endif

#include "../emulators/c/op-cpu.h"

#define MAX_LEVEL_1_PAGE_TABLE_MAPPINGS 4194304
#define MAX_LEVEL_2_PAGE_TABLE_MAPPINGS 2048

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

extern unsigned int num_level_1_page_table_mappings_used;
extern unsigned int num_level_2_page_table_mappings_used;

extern unsigned int level_1_page_table_mappings[MAX_LEVEL_1_PAGE_TABLE_MAPPINGS];
extern unsigned int level_2_page_table_mappings[MAX_LEVEL_2_PAGE_TABLE_MAPPINGS];

#endif
