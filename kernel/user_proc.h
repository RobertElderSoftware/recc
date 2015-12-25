#ifndef USER_PROC_H_
#define USER_PROC_H_
/*
    Copyright 2015 Robert Elder Software Inc.
    
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
#ifndef PUBLIC_KERNEL_INTERFACE_H_
#include "public_kernel_interface.h"
#endif
#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif

int do_compile(void);
void user_proc_1(void);
void user_proc_2(void);
void clock_tick_notifier(void);
void clock_server(void);
void uart1_out_ready_notifier(void);
void uart1_out_server(void);
void uart1_in_ready_notifier(void);
void uart1_in_server(void);
void command_server(void);

int putchar_nobusy(int);
int getchar_nobusy(void);
void putchar_init(void);

extern unsigned char data_image_size[4];
extern unsigned char data[1][5];

#endif
