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
#include "private_kernel_interface.h"
#include "assert.h"

unsigned int current_task_id = 0;
unsigned int num_clock_ticks = 0;
unsigned int saved_uart1_out_ready = 0;
unsigned int saved_uart1_in_ready = 0;

struct task_queue * ready_queue_p0_ptr;
struct task_queue * ready_queue_p1_ptr;
struct task_queue * ready_queue_p2_ptr;
struct task_queue * ready_queue_ptr;
struct task_queue * zombie_queue_ptr;
struct task_queue * blocked_on_clock_tick_queue_ptr;
struct task_queue * blocked_on_uart1_out_ready_queue_ptr;
struct task_queue * blocked_on_uart1_in_ready_queue_ptr;

struct process_control_block * pcb_ptrs[MAX_NUM_PROCESSES];

void schedule_next_task(void){
	struct process_control_block * next_task;
	/*  Get the next task */
	if(task_queue_current_count(ready_queue_p0_ptr)){
		next_task = task_queue_pop_start(ready_queue_p0_ptr); 
	}else if(task_queue_current_count(ready_queue_p1_ptr)){
		next_task = task_queue_pop_start(ready_queue_p1_ptr); 
	}else if(task_queue_current_count(ready_queue_p2_ptr)){
		next_task = task_queue_pop_start(ready_queue_p2_ptr); 
	}else{
		next_task = task_queue_pop_start(ready_queue_ptr); 
	}
	next_task->state = ACTIVE;
	/*  Set its stack pointer */
	g_current_sp = next_task->stack_pointer;
	current_task_id = next_task->pid;
}

void unblock_tasks_for_event(enum kernel_event event){
	switch (event){
		case CLOCK_TICK_EVENT:{
			struct process_control_block * unblocked_task;
			if(task_queue_current_count(blocked_on_clock_tick_queue_ptr)){
				unblocked_task = task_queue_pop_start(blocked_on_clock_tick_queue_ptr); 
				add_task_to_ready_queue(unblocked_task);
			}
			break;
		}case UART1_OUT_READY:{
			struct process_control_block * unblocked_task;
			if(task_queue_current_count(blocked_on_uart1_out_ready_queue_ptr) == 0){
				/*  Nothing has blocked on this event yet so save the signal */
				assert(!saved_uart1_out_ready, "There should be no previous saved uart signal.  Expect output problems.");
				saved_uart1_out_ready = 1;
			}else{
				unblocked_task = task_queue_pop_start(blocked_on_uart1_out_ready_queue_ptr); 
				add_task_to_ready_queue(unblocked_task);
			}
			break;
		}case UART1_IN_READY:{
			struct process_control_block * unblocked_task;
			if(task_queue_current_count(blocked_on_uart1_in_ready_queue_ptr) == 0){
				/*  Nothing has blocked on this event yet so save the signal */
				assert(!saved_uart1_in_ready, "There should be no previous saved uart signal.  Expect input problems.");
				saved_uart1_in_ready = 1;
			}else{
				unblocked_task = task_queue_pop_start(blocked_on_uart1_in_ready_queue_ptr); 
				add_task_to_ready_queue(unblocked_task);
			}
			break;
		}default:{
			assert(0, "Unhandled unblock event.\n");
			break;
		}
	}
	scheduler();
}

void add_task_to_ready_queue(struct process_control_block * pcb){
	pcb->state = READY;
	switch(pcb->priority){
		case 0:{
			task_queue_push_end(ready_queue_p0_ptr, pcb); 
			break;
		}case 1:{
			task_queue_push_end(ready_queue_p1_ptr, pcb); 
			break;
		}case 2:{
			task_queue_push_end(ready_queue_p2_ptr, pcb); 
			break;
		}default:{
			task_queue_push_end(ready_queue_ptr, pcb); 
		}
	}
}

void save_current_task_as_ready(void){
	/*  Save the stack pointer of the current task */
	pcb_ptrs[current_task_id]->stack_pointer = g_current_sp;
	/*  Put current task back on ready queue */
	add_task_to_ready_queue(pcb_ptrs[current_task_id]);
}

void save_current_task(struct task_queue * queue, enum process_state state){
	/*  Save the stack pointer of the current task */
	pcb_ptrs[current_task_id]->stack_pointer = g_current_sp;
	/*  Put current task back on queue */
	pcb_ptrs[current_task_id]->state = state;
	task_queue_push_end(queue, pcb_ptrs[current_task_id]); 
}

unsigned int scheduler(void){
	save_current_task_as_ready();
	schedule_next_task();
	return 3;
}

void k_task_exit(void){
	save_current_task(zombie_queue_ptr, ZOMBIE);
	schedule_next_task();
}

unsigned int k_release_processor(void){
	return scheduler();
}

void k_block_on_event(enum kernel_event event){
	switch(event){
		case CLOCK_TICK_EVENT:{
			save_current_task(blocked_on_clock_tick_queue_ptr, BLOCKED_ON_CLOCK_TICK);
			schedule_next_task();
			break;
		}case UART1_OUT_READY:{
			if(saved_uart1_out_ready){
				/*  Use up the saved signal, and don't block the task. */
				saved_uart1_out_ready = 0;
			}else{
				save_current_task(blocked_on_uart1_out_ready_queue_ptr, BLOCKED_ON_UART1_OUT_READY);
				schedule_next_task();
			}
			break;
		}case UART1_IN_READY:{
			if(saved_uart1_in_ready){
				/*  Use up the saved signal, and don't block the task. */
				saved_uart1_in_ready = 0;
			}else{
				save_current_task(blocked_on_uart1_in_ready_queue_ptr, BLOCKED_ON_UART1_IN_READY);
				schedule_next_task();
			}
			break;
		}default:{
			assert(0, "Blocking on unknown event.\n");
		}
	}
}

void k_irq_handler(unsigned int interrupt_id){
	switch(interrupt_id){
		case 16:{/*  Bit 4 was set */
			num_clock_ticks++;
			unblock_tasks_for_event(CLOCK_TICK_EVENT);
			break;
		}case 64:{/*  Bit 6 was set */
			unblock_tasks_for_event(UART1_OUT_READY);
			break;
		}case 256:{/*  Bit 8 was set */
			unblock_tasks_for_event(UART1_IN_READY);
			break;
		}default:{
			break;
		}
	}
	scheduler();
}

void k_send_message(struct kernel_message * message, unsigned int destination_pid, struct kernel_message * reply){
	/*  Remember where to store the reply */
	pcb_ptrs[current_task_id]->reply_message = reply;
	message->source_id = current_task_id;
	if(pcb_ptrs[destination_pid]->state == BLOCKED_ON_SEND){
		/*  The destination is already blocked on our message send */
		pcb_ptrs[current_task_id]->state = BLOCKED_ON_RECEIVE;
		add_task_to_ready_queue(pcb_ptrs[destination_pid]);
		*(pcb_ptrs[destination_pid]->recieve_message) = *message;
	}else{
		/*  The destination has not asked for the message yet */
		message_queue_push_end(pcb_ptrs[destination_pid]->messages_ptr, *message); 
		pcb_ptrs[current_task_id]->state = BLOCKED_ON_REPLY;
	}
	pcb_ptrs[current_task_id]->stack_pointer = g_current_sp;
	schedule_next_task();
}

void k_receive_message(struct kernel_message * message){
	if(message_queue_current_count(pcb_ptrs[current_task_id]->messages_ptr) == 0){
		pcb_ptrs[current_task_id]->state = BLOCKED_ON_SEND;
		pcb_ptrs[current_task_id]->stack_pointer = g_current_sp;
		pcb_ptrs[current_task_id]->recieve_message = message;
	}else{
		struct kernel_message m;
		m = message_queue_pop_start(pcb_ptrs[current_task_id]->messages_ptr);
		*message = m;
		pcb_ptrs[message->source_id]->state = BLOCKED_ON_REPLY;
		save_current_task_as_ready();
	}
	schedule_next_task();
}

void k_reply_message(struct kernel_message * message, unsigned int destination_pid){
	*(pcb_ptrs[destination_pid]->reply_message) = *message;
	/*  Unblock the destination task */
	add_task_to_ready_queue(pcb_ptrs[destination_pid]);
	/*  Save current task and continue */
	save_current_task_as_ready();
	schedule_next_task();
}

void k_kernel_init(void){
	unsigned int i;
	/*  Set up some globals for putchar function */
	putchar_init();
	/*  Optimization, due to the fact that the compiler is not very smart */
	ready_queue_p0_ptr = &ready_queue_p0;
	ready_queue_p1_ptr = &ready_queue_p1;
	ready_queue_p2_ptr = &ready_queue_p2;
	ready_queue_ptr = &ready_queue;
	zombie_queue_ptr = &zombie_queue;
	blocked_on_clock_tick_queue_ptr = &blocked_on_clock_tick_queue;
	blocked_on_uart1_out_ready_queue_ptr = &blocked_on_uart1_out_ready_queue;
	blocked_on_uart1_in_ready_queue_ptr = &blocked_on_uart1_in_ready_queue;

	task_queue_init(ready_queue_p0_ptr, MAX_NUM_PROCESSES);
	task_queue_init(ready_queue_p1_ptr, MAX_NUM_PROCESSES);
	task_queue_init(ready_queue_p2_ptr, MAX_NUM_PROCESSES);
	task_queue_init(ready_queue_ptr, MAX_NUM_PROCESSES);
	task_queue_init(zombie_queue_ptr, MAX_NUM_PROCESSES);
	task_queue_init(blocked_on_clock_tick_queue_ptr, MAX_NUM_PROCESSES);
	task_queue_init(blocked_on_uart1_out_ready_queue_ptr, MAX_NUM_PROCESSES);
	task_queue_init(blocked_on_uart1_in_ready_queue_ptr, MAX_NUM_PROCESSES);

	pcbs[0].state = ACTIVE; /*  Task 0 is not really a task, is the 'int main' that we might want to return to later for graceful exit. */
	for(i = 0; i < MAX_NUM_PROCESSES; i++)
		pcbs[i].pid = i;

	pcbs[0].priority = 5;
	pcbs[1].priority = 5;
	pcbs[2].priority = 5;
	pcbs[3].priority = 2;
	pcbs[4].priority = 3;
	pcbs[5].priority = 0;
	pcbs[6].priority = 1;
	pcbs[7].priority = 0;
	pcbs[8].priority = 1;
	pcbs[9].priority = 3;

	pcbs[1].stack_pointer = (unsigned int)&user_proc_1_stack_start;
	pcbs[2].stack_pointer = (unsigned int)&user_proc_2_stack_start;
	pcbs[3].stack_pointer = (unsigned int)&user_proc_3_stack_start;
	pcbs[4].stack_pointer = (unsigned int)&user_proc_4_stack_start;
	pcbs[5].stack_pointer = (unsigned int)&user_proc_5_stack_start;
	pcbs[6].stack_pointer = (unsigned int)&user_proc_6_stack_start;
	pcbs[7].stack_pointer = (unsigned int)&user_proc_7_stack_start;
	pcbs[8].stack_pointer = (unsigned int)&user_proc_8_stack_start;
	pcbs[9].stack_pointer = (unsigned int)&user_proc_9_stack_start;

	/*  Optimization, due to the fact that the compiler is not very smart */
	for(i = 0; i < MAX_NUM_PROCESSES; i++)
		pcb_ptrs[i] = &pcbs[i];

	/*  Optimization, due to the fact that the compiler is not very smart */
	for(i = 0; i < MAX_NUM_PROCESSES; i++)
		pcb_ptrs[i]->messages_ptr = &pcb_ptrs[i]->messages;

	for(i = 0; i < MAX_NUM_PROCESSES; i++)
		message_queue_init(&pcb_ptrs[i]->messages, MAX_NUM_PROCESSES);

	for(i = 1; i < MAX_NUM_PROCESSES; i++)
		add_task_to_ready_queue(&pcbs[i]);

	/*  We need to set up our tasks so that the stack has the correct PC and FP value for the first time it is scheduled*/
	init_task_stack(&(pcbs[1].stack_pointer), user_proc_1);
	init_task_stack(&(pcbs[2].stack_pointer), user_proc_2);
	init_task_stack(&(pcbs[3].stack_pointer), clock_tick_notifier);
	init_task_stack(&(pcbs[4].stack_pointer), clock_server);
	init_task_stack(&(pcbs[5].stack_pointer), uart1_out_ready_notifier);
	init_task_stack(&(pcbs[6].stack_pointer), uart1_out_server);
	init_task_stack(&(pcbs[7].stack_pointer), uart1_in_ready_notifier);
	init_task_stack(&(pcbs[8].stack_pointer), uart1_in_server);
	init_task_stack(&(pcbs[9].stack_pointer), command_server);

	set_irq_handler(irq_handler);
	timer_interrupt_enable();
	uart1_out_interrupt_enable();
	uart1_in_interrupt_enable();
}
