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

/*
  A linux compatible interface for running .l0.c files in the op-cpu emulator.  Takes care
of non-standard terminal input, and output.

*/

#include "op-cpu.h"
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

extern unsigned char data[][5];

struct termios * terminal_setup(void);

struct termios * terminal_setup(void){
	/*  Change settings to let input be completely processed by the emulator. */
	struct termios * original = malloc(sizeof(struct termios));
        struct termios stdio;
	tcgetattr(STDOUT_FILENO, original);
 
        memcpy(&stdio, original, sizeof(struct termios));
        stdio.c_iflag = 0;
        stdio.c_cflag = 0;
        stdio.c_lflag = 0;
        stdio.c_cc[VMIN]=1;
        stdio.c_cc[VTIME]=0;
        tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
        return original;
}

struct ll_item{
	struct ll_item * next;
	unsigned int c;
	unsigned int pad;
};

int main(void){
	struct virtual_machine * vm;
	unsigned int output;
        unsigned char c;
	struct termios * original = terminal_setup();
	struct ll_item * first_item = (struct ll_item *)0;
	struct ll_item * last_item = (struct ll_item *)0;

	vm = vm_create(data);
	printf("Kernel image has been loaded. All input is now being handled by the emulator (including Ctrl+c).\n");
	printf("Press 'q' to quit.\n");

	while(!is_halted(vm)){
		if(vm_getc(vm, &output)){
			putchar((int)output);
			fflush(stdout);
		}

		if(read(STDIN_FILENO, &c, 1) > 0){
			/*  Add item to the end of the list */
			struct ll_item * new_item = malloc(sizeof(struct ll_item));
			new_item->c = c;
			new_item->next = (struct ll_item *)0;
			if(first_item){
				last_item->next = new_item;
			}else{
				first_item = new_item;
			}
			last_item = new_item;
		}

		if(first_item){
			if(!vm_putc(vm, first_item->c)){
				/*  Take first item off of linked list */
				struct ll_item * tmp_item = first_item;
				first_item = first_item->next;
				free(tmp_item);
				if(!first_item){
					last_item = (struct ll_item *)0;
				}
			}
		}
		step(vm);
	}

	vm_destroy(vm);
        tcsetattr(STDOUT_FILENO,TCSANOW, original);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH, original);
        free(original);
	return 0;
}
