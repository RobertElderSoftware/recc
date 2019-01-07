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
#include "printf_busy.h"
#include "private_kernel_interface.h"
#include "../emulators/c/op-cpu.h"
#include <assert.h>

unsigned int current_task_id = 0;
unsigned int num_clock_ticks = 0;
unsigned int saved_uart1_out_ready = 0;
unsigned int saved_uart1_in_ready = 0;
unsigned int current_timer_period = 0xA000;
unsigned int interrupts_this_period = 0;

void schedule_next_task(void){
	struct process_control_block * next_task;
	/*  Get the next task */
	if(task_queue_current_count(&ready_queue_p0)){
		next_task = task_queue_pop_start(&ready_queue_p0); 
	}else if(task_queue_current_count(&ready_queue_p1)){
		next_task = task_queue_pop_start(&ready_queue_p1); 
	}else if(task_queue_current_count(&ready_queue_p2)){
		next_task = task_queue_pop_start(&ready_queue_p2); 
	}else{
		next_task = task_queue_pop_start(&ready_queue); 
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
			if(task_queue_current_count(&blocked_on_clock_tick_queue)){
				unblocked_task = task_queue_pop_start(&blocked_on_clock_tick_queue); 
				add_task_to_ready_queue(unblocked_task);
			}
			break;
		}case UART1_OUT_READY:{
			struct process_control_block * unblocked_task;
			if(task_queue_current_count(&blocked_on_uart1_out_ready_queue) == 0){
				/*  Nothing has blocked on this event yet so save the signal */
				assert(!saved_uart1_out_ready && "There should be no previous saved uart signal.  Expect output problems.");
				saved_uart1_out_ready = 1;
			}else{
				unblocked_task = task_queue_pop_start(&blocked_on_uart1_out_ready_queue); 
				add_task_to_ready_queue(unblocked_task);
			}
			break;
		}case UART1_IN_READY:{
			struct process_control_block * unblocked_task;
			if(task_queue_current_count(&blocked_on_uart1_in_ready_queue) == 0){
				/*  Nothing has blocked on this event yet so save the signal */
				assert(!saved_uart1_in_ready && "There should be no previous saved uart signal.  Expect input problems.");
				saved_uart1_in_ready = 1;
			}else{
				unblocked_task = task_queue_pop_start(&blocked_on_uart1_in_ready_queue); 
				add_task_to_ready_queue(unblocked_task);
			}
			break;
		}default:{
			assert(0 && "Unhandled unblock event.\n");
			break;
		}
	}
	scheduler();
}

void add_task_to_ready_queue(struct process_control_block * pcb){
	pcb->state = READY;
	switch(pcb->priority){
		case 0:{
			task_queue_push_end(&ready_queue_p0, pcb); 
			break;
		}case 1:{
			task_queue_push_end(&ready_queue_p1, pcb); 
			break;
		}case 2:{
			task_queue_push_end(&ready_queue_p2, pcb); 
			break;
		}default:{
			task_queue_push_end(&ready_queue, pcb); 
		}
	}
}

void save_current_task_as_ready(void){
	/*  Save the stack pointer of the current task */
	pcbs[current_task_id].stack_pointer = g_current_sp;
	/*  Put current task back on ready queue */
	add_task_to_ready_queue(&pcbs[current_task_id]);
}

void save_current_task(struct task_queue * queue, enum process_state state){
	/*  Save the stack pointer of the current task */
	pcbs[current_task_id].stack_pointer = g_current_sp;
	/*  Put current task back on queue */
	pcbs[current_task_id].state = state;
	task_queue_push_end(queue, &pcbs[current_task_id]); 
}

unsigned int scheduler(void){
	save_current_task_as_ready();
	schedule_next_task();
	return 3;
}

void k_task_exit(void){
	save_current_task(&zombie_queue, ZOMBIE);
	schedule_next_task();
}

unsigned int k_release_processor(void){
	return scheduler();
}

void k_block_on_event(enum kernel_event event){
	switch(event){
		case CLOCK_TICK_EVENT:{
			save_current_task(&blocked_on_clock_tick_queue, BLOCKED_ON_CLOCK_TICK);
			schedule_next_task();
			break;
		}case UART1_OUT_READY:{
			if(saved_uart1_out_ready){
				/*  Use up the saved signal, and don't block the task. */
				saved_uart1_out_ready = 0;
			}else{
				save_current_task(&blocked_on_uart1_out_ready_queue, BLOCKED_ON_UART1_OUT_READY);
				schedule_next_task();
			}
			break;
		}case UART1_IN_READY:{
			if(saved_uart1_in_ready){
				/*  Use up the saved signal, and don't block the task. */
				saved_uart1_in_ready = 0;
			}else{
				save_current_task(&blocked_on_uart1_in_ready_queue, BLOCKED_ON_UART1_IN_READY);
				schedule_next_task();
			}
			break;
		}default:{
			assert(0 && "Blocking on unknown event.\n");
		}
	}
}

void handle_page_fault_exception(void){
	unsigned int * pfe_page_pointer = *((unsigned int **)PFE_PAGE_POINTER);
	unsigned int pfe_pc_value = *((unsigned int *)PFE_PC_VALUE);
	unsigned int pfe_access = *((unsigned int *)PFE_ACCESS);
	unsigned int pfe_virtual = *((unsigned int *)PFE_VIRTUAL);

	unsigned int level_2_index = (pfe_virtual & LEVEL_2_PAGE_TABLE_INDEX_MASK) >> (LEVEL_1_PAGE_TABLE_NUM_BITS + OP_CPU_PAGE_SIZE_NUM_BITS);
	unsigned int level_1_index = (pfe_virtual & LEVEL_1_PAGE_TABLE_INDEX_MASK) >> OP_CPU_PAGE_SIZE_NUM_BITS;
	unsigned int offset = (pfe_virtual & PAGE_OFFSET_MASK);
	unsigned int level_2_page_table_entry = pfe_page_pointer[level_2_index];
	printf_busy("A Page fault exception occurred originating from PC location 0x%X, seeking 0x%X access.\n", pfe_pc_value, pfe_access);
	printf_busy("Virtual address 0x%X gives lvl 2 index 0x%X and lvl 1 index 0x%X and offset 0x%X using 0x%X as lvl 2 page table pointer\n", pfe_virtual, level_2_index, level_1_index, offset, level_2_page_table_entry);
	/*  Make sure we have access to this level 2 page table entry, and that it is valid */
	if(level_2_page_table_entry & LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED){
		unsigned int * level_1_page_table_pointer = (unsigned int*)(level_2_page_table_entry & (LEVEL_1_PAGE_TABLE_INDEX_MASK | LEVEL_2_PAGE_TABLE_INDEX_MASK));
		unsigned int level_1_page_table_entry = level_1_page_table_pointer[level_1_index];
		/*  Make sure we have access to this level 1 page table entry, and that it is valid */
		if((level_1_page_table_entry & pfe_access) && (level_1_page_table_entry & LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED)){
			unsigned int linear_address_page = level_1_page_table_entry & (LEVEL_1_PAGE_TABLE_INDEX_MASK | LEVEL_2_PAGE_TABLE_INDEX_MASK);
			printf_busy("Should not occur, no PFE: address was translated to 0x%X\n", linear_address_page + offset);
		}else{
			printf_busy("Page fault is at level 1 with entry as 0x%X\n", level_1_page_table_entry);
		}
	}else{
		printf_busy("Page fault is at level 2 with entry as 0x%X\n", level_2_page_table_entry);
	}
}

void k_irq_handler(void){
	unsigned int flags_register = read_flags_register();
	if(flags_register & PAGE_FAULT_EXCEPTION_ASSERTED_BIT){
		/*  Handle page fault exception */
		handle_page_fault_exception();
		or_into_flags_register(HALTED_BIT); /*  Halt the processor for now */
		/*  De-assert the bit last, so we can detect nested page fault exceptions */
		deassert_bits_in_flags_register(PAGE_FAULT_EXCEPTION_ASSERTED_BIT);
	}else if(flags_register & TIMER1_ASSERTED_BIT){
		deassert_bits_in_flags_register(TIMER1_ASSERTED_BIT);
		num_clock_ticks++;
		unblock_tasks_for_event(CLOCK_TICK_EVENT);
	}else if(flags_register & UART1_OUT_ASSERTED_BIT){
		deassert_bits_in_flags_register(UART1_OUT_ASSERTED_BIT);
		unblock_tasks_for_event(UART1_OUT_READY);
	}else if(flags_register & UART1_IN_ASSERTED_BIT){
		deassert_bits_in_flags_register(UART1_IN_ASSERTED_BIT);
		unblock_tasks_for_event(UART1_IN_READY);
	}else{
		/*  Something really bad happend. */
		/*  Busy print will affect flags, but in this case everything is broken anyway */
		printf_busy("IRQ FAILURE!\n");
		or_into_flags_register(HALTED_BIT); /*  Halt the processor */
	}

	/*  Used as a stress test for the kernel:  Try out all interleavings of interrupts and timer periods */
	if(interrupts_this_period == 20){
		interrupts_this_period = 0;
		current_timer_period++;
		set_timer_period(current_timer_period);
	}
	interrupts_this_period++;
	scheduler();
}

void k_send_message(struct kernel_message * message, unsigned int destination_pid, struct kernel_message * reply){
	/*  Remember where to store the reply */
	pcbs[current_task_id].reply_message = reply;
	message->source_id = current_task_id;
	if(pcbs[destination_pid].state == BLOCKED_ON_SEND){
		/*  The destination is already blocked on our message send */
		pcbs[current_task_id].state = BLOCKED_ON_RECEIVE;
		add_task_to_ready_queue(&pcbs[destination_pid]);
		*(pcbs[destination_pid].recieve_message) = *message;
	}else{
		/*  The destination has not asked for the message yet */
		message_queue_push_end(&pcbs[destination_pid].messages, *message); 
		pcbs[current_task_id].state = BLOCKED_ON_REPLY;
	}
	pcbs[current_task_id].stack_pointer = g_current_sp;
	schedule_next_task();
}

void k_receive_message(struct kernel_message * message){
	if(message_queue_current_count(&pcbs[current_task_id].messages) == 0){
		pcbs[current_task_id].state = BLOCKED_ON_SEND;
		pcbs[current_task_id].stack_pointer = g_current_sp;
		pcbs[current_task_id].recieve_message = message;
	}else{
		struct kernel_message m;
		m = message_queue_pop_start(&pcbs[current_task_id].messages);
		*message = m;
		pcbs[message->source_id].state = BLOCKED_ON_REPLY;
		save_current_task_as_ready();
	}
	schedule_next_task();
}

void k_reply_message(struct kernel_message * message, unsigned int destination_pid){
	*(pcbs[destination_pid].reply_message) = *message;
	/*  Unblock the destination task */
	add_task_to_ready_queue(&pcbs[destination_pid]);
	/*  Save current task and continue */
	save_current_task_as_ready();
	schedule_next_task();
}

void k_kernel_exit(void){
	/*  Don't need to save any state because we're exiting kernel */
	struct process_control_block * next_task = &pcbs[0]; /* Main entry SP is stored in PCB 0 */
	g_current_sp = next_task->stack_pointer;
}

void set_irq_handler(void (*irq_handler_fcn)(void)){
	void (**irq_handler_fcn_location)(void) = (void (**)(void))IRQ_HANDLER;
	*irq_handler_fcn_location = irq_handler_fcn;
}

void set_timer_period(unsigned int period){
	unsigned int * period_location = (unsigned int *)TIMER_PERIOD;
	*period_location = period;
}

void set_level_2_page_pointer(unsigned int * pointer){
	unsigned int ** pointer_location = (unsigned int **)PAGE_POINTER;
	*pointer_location = pointer;
	printf_busy("Page ptr is %p\n", pointer);
}

unsigned int timer_interrupt_enable(void){
	or_into_flags_register(TIMER1_ENABLE_BIT);
}

unsigned int uart1_out_interrupt_enable(void){
	or_into_flags_register(UART1_OUT_ENABLE_BIT);
}

unsigned int uart1_in_interrupt_enable(void){
	or_into_flags_register(UART1_IN_ENABLE_BIT);
}

unsigned int page_fault_exception_interrupt_enable(void){
	or_into_flags_register(PAGEING_ENABLE_BIT);
}

unsigned int * allocate_level_1_page_table(void){
	unsigned pages_in_table = (1 << LEVEL_1_PAGE_TABLE_NUM_BITS);
	if((num_level_1_page_table_mappings_used + pages_in_table) <= MAX_LEVEL_1_PAGE_TABLE_MAPPINGS){
		unsigned int * rtn = &level_1_page_table_mappings[num_level_1_page_table_mappings_used];
		unsigned int i;
		/*  Intitialize entries */
		for(i = num_level_1_page_table_mappings_used; i < (num_level_1_page_table_mappings_used + pages_in_table); i++){
			level_1_page_table_mappings[i] = 0;
		}
		num_level_1_page_table_mappings_used += pages_in_table;
		return rtn;
	}else{
		printf_busy("Level 1 page table allocation failed.\n");
	}
	return (unsigned int *)0;
}

unsigned int * allocate_level_2_page_table(void){
	unsigned pages_in_table = (1 << LEVEL_2_PAGE_TABLE_NUM_BITS);
	if((num_level_2_page_table_mappings_used + pages_in_table) <= MAX_LEVEL_2_PAGE_TABLE_MAPPINGS){
		unsigned int * rtn = &level_2_page_table_mappings[num_level_2_page_table_mappings_used];
		unsigned int i;
		/*  Intitialize entries */
		for(i = num_level_2_page_table_mappings_used; i < (num_level_2_page_table_mappings_used + pages_in_table); i++){
			level_2_page_table_mappings[i] = 0;
		}
		num_level_2_page_table_mappings_used += pages_in_table;
		return rtn;
	}else{
		printf_busy("Level 2 page table allocation failed.\n");
	}
	return (unsigned int *)0;
}

void create_level_1_page_table_entry(unsigned int * level_2_page_table_entry, unsigned int linear_address, unsigned int level_1_index, unsigned int permissions){
	unsigned int * level_1_table_ptr;
	if(!(*level_2_page_table_entry & LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED)){
		unsigned int * new_page_table_ptr = allocate_level_1_page_table();
		*level_2_page_table_entry = ((unsigned int)new_page_table_ptr | LEVEL_2_PAGE_TABLE_ENTRY_INITIALIZED);
		/*printf_busy("Initialized to 0x%X\n", *level_2_page_table_entry);*/
	}
	level_1_table_ptr = (unsigned int *)((LEVEL_2_PAGE_TABLE_INDEX_MASK | LEVEL_1_PAGE_TABLE_INDEX_MASK) & (*level_2_page_table_entry));
	/*  Linear address is expected to be a page aligned address with only the high bits set. */
	level_1_table_ptr[level_1_index] = linear_address | permissions | LEVEL_1_PAGE_TABLE_ENTRY_INITIALIZED;
	/*printf_busy("Intialized l2 pt entry %p that points to l1 pt %p and set l1 pt entry %d to be %X\n", level_2_page_table_entry, level_1_table_ptr, level_1_index, level_1_table_ptr[level_1_index]);*/
}

void create_level_2_page_table_entry(unsigned int linear_address, unsigned int virtual_address, unsigned int permissions, unsigned int * level_2_page_table){
	unsigned int level_2_index = (LEVEL_2_PAGE_TABLE_INDEX_MASK & virtual_address) >> (LEVEL_1_PAGE_TABLE_NUM_BITS + OP_CPU_PAGE_SIZE_NUM_BITS);
	unsigned int level_1_index = (LEVEL_1_PAGE_TABLE_INDEX_MASK & virtual_address) >> OP_CPU_PAGE_SIZE_NUM_BITS;
	unsigned int * level_2_page_table_entry = &level_2_page_table_mappings[level_2_index];
	create_level_1_page_table_entry(level_2_page_table_entry, linear_address, level_1_index, permissions);
}

void set_up_identity_kernel_mapping_for_region(unsigned int start, unsigned int end, unsigned int permissions, unsigned int * level_2_page_table){
	unsigned int current;
	for(current = start; current < end; current += OP_CPU_PAGE_SIZE){
		create_level_2_page_table_entry(current, current, permissions, level_2_page_table);
	}
}

void k_kernel_init(void){
	unsigned int num_l0_items = (data[0][1] << 24) + (data[0][2] << 16) + (data[0][3] << 8) + data[0][4];
	unsigned int i;
	unsigned int * level_2_page_table = allocate_level_2_page_table();
	set_level_2_page_pointer(level_2_page_table);
	printf_busy("Begin kernel initialization...\n");
	for(i = 0; i < num_l0_items; i++){
		/*  Assuming that regions are specified first */
		if(data[i][0] == 5){
			/*  No more regions, looking at symbol definitions now. */
			break;
		}else if(data[i][0] == 0xE){
			unsigned int start;
			unsigned int end;
			unsigned int permission;
			i++;
			if(data[i][0] == 0xA){
				start = (data[i][1] << 24) + (data[i][2] << 16) + (data[i][3] << 8) + data[i][4];
				i++;
			}
			if(data[i][0] == 0xB){
				end = (data[i][1] << 24) + (data[i][2] << 16) + (data[i][3] << 8) + data[i][4];
				i++;
			}
			if(data[i][0] == 0xF){
				permission = data[i][4];
			}
			printf_busy("Start: 0x%X End: 0x%X Permission: 0x%X\n", start, end, permission);
			set_up_identity_kernel_mapping_for_region(start, end, permission, level_2_page_table);
		}
	}
	set_irq_handler(irq_handler); /*  Set before paging is enabled, otherwise a page fault doesn't know where to go */
	page_fault_exception_interrupt_enable();

	task_queue_init(&ready_queue_p0, MAX_NUM_PROCESSES);
	task_queue_init(&ready_queue_p1, MAX_NUM_PROCESSES);
	task_queue_init(&ready_queue_p2, MAX_NUM_PROCESSES);
	task_queue_init(&ready_queue, MAX_NUM_PROCESSES);
	task_queue_init(&zombie_queue, MAX_NUM_PROCESSES);
	task_queue_init(&blocked_on_clock_tick_queue, MAX_NUM_PROCESSES);
	task_queue_init(&blocked_on_uart1_out_ready_queue, MAX_NUM_PROCESSES);
	task_queue_init(&blocked_on_uart1_in_ready_queue, MAX_NUM_PROCESSES);

	pcbs[0].state = ACTIVE; /*  Task 0 is not really a task, it is the 'int main' that we might want to return to later for graceful exit. */

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

	pcbs[0].stack_pointer = g_current_sp; /*  Save SP from entering this method so we can exit kernel gracefully */
	pcbs[1].stack_pointer = &user_proc_1_stack[STACK_SIZE -1];
	pcbs[2].stack_pointer = &user_proc_2_stack[STACK_SIZE -1];
	pcbs[3].stack_pointer = &user_proc_3_stack[STACK_SIZE -1];
	pcbs[4].stack_pointer = &user_proc_4_stack[STACK_SIZE -1];
	pcbs[5].stack_pointer = &user_proc_5_stack[STACK_SIZE -1];
	pcbs[6].stack_pointer = &user_proc_6_stack[STACK_SIZE -1];
	pcbs[7].stack_pointer = &user_proc_7_stack[STACK_SIZE -1];
	pcbs[8].stack_pointer = &user_proc_8_stack[STACK_SIZE -1];
	pcbs[9].stack_pointer = &user_proc_9_stack[STACK_SIZE -1];

	for(i = 0; i < MAX_NUM_PROCESSES; i++)
		message_queue_init(&pcbs[i].messages, MAX_NUM_PROCESSES);

	for(i = 1; i < MAX_NUM_PROCESSES; i++)
		add_task_to_ready_queue(&pcbs[i]);

	/*  We need to set up our tasks so that the stack has the correct PC and FP value for the first time it is scheduled*/
	init_task_stack(&(pcbs[1].stack_pointer), user_proc_1);
	init_task_stack(&(pcbs[2].stack_pointer), do_compile);
	init_task_stack(&(pcbs[3].stack_pointer), clock_tick_notifier);
	init_task_stack(&(pcbs[4].stack_pointer), clock_server);
	init_task_stack(&(pcbs[5].stack_pointer), uart1_out_ready_notifier);
	init_task_stack(&(pcbs[6].stack_pointer), uart1_out_server);
	init_task_stack(&(pcbs[7].stack_pointer), uart1_in_ready_notifier);
	init_task_stack(&(pcbs[8].stack_pointer), uart1_in_server);
	init_task_stack(&(pcbs[9].stack_pointer), command_server);

	set_timer_period(current_timer_period);
	timer_interrupt_enable();
	uart1_out_interrupt_enable();
	uart1_in_interrupt_enable();
	schedule_next_task();
}
