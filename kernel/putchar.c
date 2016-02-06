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
#include "public_kernel_interface.h"
#include <assert.h>

int putchar(int);
int putchar_nobusy(int);

int putchar(int c){
	struct kernel_message putchar_m;
	struct kernel_message putchar_reply;
	putchar_m.message_type = OUTPUT_CHARACTER;
	putchar_m.data = c;
	send_message(&putchar_m, 6, &putchar_reply);
	switch(putchar_reply.message_type){
		case MESSAGE_ACKNOWLEDGED:{
			break;
		}default:{
			assert(0 && "Unknown message type.\n");
		}
	}
}
