#ifndef PUBLIC_KERNEL_INTERFACE_H_
#define PUBLIC_KERNEL_INTERFACE_H_
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

#ifndef KERNEL_STATE_H_
#include "kernel_state.h"
#endif
#ifndef QUEUE_H_DEFINED_
#include "queue.h"
#endif

unsigned int release_processor(void);
void task_exit(void);
void kernel_init(void);
void kernel_exit(void);
void block_on_event(enum kernel_event);
void send_message(struct kernel_message *, unsigned int, struct kernel_message *);
void receive_message(struct kernel_message *);
void reply_message(struct kernel_message *, unsigned int);

#endif
