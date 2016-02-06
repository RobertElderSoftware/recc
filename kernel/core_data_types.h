#ifndef CORE_DATA_TYPES_H
#define CORE_DATA_TYPES_H
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
