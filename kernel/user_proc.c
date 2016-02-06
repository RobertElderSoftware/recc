/*
    Copyright 2016 Robert Elder Software Inc.
    
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
#include "user_proc.h"

void user_proc_1(void){
	while(1){
		/*
		printf("You're currently running a very simple microkernel that was built\n");
		printf("for the purposes of demonstrating the 'One Page CPU' design, and\n");
		printf("cross compiler collection.  This microkernel implements inter-process\n");
		printf("communication, premptive context switching, interrupt based I/O, along\n");
		printf("with a very simple timer that counts simulated clock ticks.\n");
		printf("\nSome single-character commands include:\n\n");
		printf("t -  Prints the number of simulated clock ticks since kernel start.\n");
		printf("s -  Prints the stack pointer values of each task.\n");
		printf("p -  Prints the priority of each task.\n");
		printf("q -  Quit.\n");
		*/
		task_exit();
	}
}

void user_proc_2(void){
	struct kernel_message m;
        m.message_type = OUTPUT_CHARACTER;
        m.data = 'M';
	while(1){
        	release_processor();
	}
}

void clock_tick_notifier(void){
	struct kernel_message clock_server_message;
	struct kernel_message clock_server_reply;
	clock_server_message.message_type = CLOCK_TICK_NOTIFY;
	clock_server_message.data = 1;
	while(1){
		block_on_event(CLOCK_TICK_EVENT);
		send_message(&clock_server_message, 4, &clock_server_reply);
		switch(clock_server_reply.message_type){
			case MESSAGE_ACKNOWLEDGED:{
				break;
			}default:{
				assert(0 && "Unknown message type.\n");
			}
		}
	}
}

unsigned int num_ticks = 0;

void clock_server(void){
	struct kernel_message message_to_reply;
	struct kernel_message received_message;
	message_to_reply.message_type = MESSAGE_ACKNOWLEDGED;
	while(1){
		receive_message(&received_message);
		switch(received_message.message_type){
			case CLOCK_TICK_NOTIFY:{
				num_ticks++;
				break;
			}default:{
				assert(0 && "Unknown message type.\n");
			}
		}
		reply_message(&message_to_reply, received_message.source_id);
	}
}

void uart1_out_ready_notifier(void){
	struct kernel_message output_server_message;
	struct kernel_message output_server_reply;
	output_server_message.message_type = UART1_OUT_READY_NOTIFY;
	output_server_message.data = 1;
	while(1){
		block_on_event(UART1_OUT_READY);
		send_message(&output_server_message, 6, &output_server_reply);
		switch(output_server_reply.message_type){
			case MESSAGE_ACKNOWLEDGED:{
				break;
			}default:{
				assert(0 && "Unknown message type.\n");
			}
		}
	}
}

void uart1_out_server(void){
	struct message_queue output_requests;
	struct kernel_message message_to_reply;
	struct kernel_message received_message;
	struct kernel_message poped_message;
	unsigned int can_output = 1; /* Initially, it is ok to send a character when the CPU starts, because we haven't got an interrupt yet */
	message_to_reply.message_type = MESSAGE_ACKNOWLEDGED;
	message_queue_init(&output_requests, MAX_NUM_PROCESSES);
	while(1){
		receive_message(&received_message);
		switch(received_message.message_type){
			case UART1_OUT_READY_NOTIFY:{
				if(message_queue_current_count(&output_requests)){
					poped_message = message_queue_pop_start(&output_requests);
					putchar_nobusy(poped_message.data);
					can_output = 0;
					reply_message(&message_to_reply, poped_message.source_id);
				}else{
					can_output = 1;
				}
				reply_message(&message_to_reply, received_message.source_id);
				break;
			}case OUTPUT_CHARACTER:{
				if(can_output){
					putchar_nobusy(received_message.data);
					reply_message(&message_to_reply, received_message.source_id);
					can_output = 0;
				}else{
					message_queue_push_end(&output_requests, received_message);
				}
				break;
			}default:{
				assert(0 && "Unknown message type.\n");
			}
		}
	}
}

void uart1_in_ready_notifier(void){
	struct kernel_message input_server_message;
	struct kernel_message input_server_reply;
	input_server_message.message_type = UART1_IN_READY_NOTIFY;
	input_server_message.data = 1;
	while(1){
		block_on_event(UART1_IN_READY);
		send_message(&input_server_message, 8, &input_server_reply);
		switch(input_server_reply.message_type){
			case MESSAGE_ACKNOWLEDGED:{
				break;
			}default:{
				assert(0 && "Unknown message type.\n");
			}
		}
	}
}

void uart1_in_server(void){
	struct kernel_message message_to_reply;
	struct kernel_message received_message;
	struct kernel_message output_server_message;
	struct kernel_message output_server_reply;
	message_to_reply.message_type = MESSAGE_ACKNOWLEDGED;
	output_server_message.message_type = OUTPUT_CHARACTER;
	while(1){
		receive_message(&received_message);
		switch(received_message.message_type){
			case UART1_IN_READY_NOTIFY:{
				output_server_message.data = getchar_nobusy();
				/*  Send the character to output */
				send_message(&output_server_message, 6, &output_server_reply);
				switch(output_server_reply.message_type){
					case MESSAGE_ACKNOWLEDGED:{
						break;
					}default:{
						assert(0 && "Unknown message type.\n");
					}
				}
				/*  Let the command server know what is being typed */
				send_message(&output_server_message, 9, &output_server_reply);
				switch(output_server_reply.message_type){
					case MESSAGE_ACKNOWLEDGED:{
						break;
					}default:{
						assert(0 && "Unknown message type.\n");
					}
				}
				reply_message(&message_to_reply, received_message.source_id);
				break;
			}default:{
				assert(0 && "Unknown message type.\n");
			}
		}
	}
}

void command_server(void){
	struct kernel_message received_message;
	struct kernel_message input_server_reply;
	input_server_reply.message_type = MESSAGE_ACKNOWLEDGED;
	while(1){
		receive_message(&received_message);
		switch(received_message.message_type){
			case OUTPUT_CHARACTER:{
				switch(received_message.data){
					case 116:{/* letter 't' */
						printf("\n%d\n", num_ticks);
						break;
					}case 115:{/* letter 's' */
						unsigned int i;
						printf("\n");
						for(i = 0; i < 9; i++){
							printf("Task %d SP: 0x%X\n", i, pcbs[i].stack_pointer);
						}
						break;
					}case 112:{/* letter 'p' */
						unsigned int i;
						printf("\n");
						for(i = 0; i < 9; i++){
							printf("Task %d Priority: %d\n", i, pcbs[i].priority);
						}
						break;
					}case 113:{/* letter 'q' */
						kernel_exit();
						break;
					}default:{
						printf("\n");
						printf("Unknown command.");
					}
				}
				break;
			}default:{
				assert(0 && "Unknown message type.\n");
			}
		}
		reply_message(&input_server_reply, received_message.source_id);
	}
}
