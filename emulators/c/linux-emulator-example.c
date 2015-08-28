/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
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

extern unsigned char data_start[4];
extern unsigned char data_end[4];
extern unsigned char data[1][5];

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

int main(void){
	struct virtual_machine * vm;
	unsigned int output;
        unsigned char c;
	struct termios * original = terminal_setup();

	vm = vm_create(data_start, data_end, data);
	printf("Kernel image has been loaded. All input is now being handled by the emulator.\n");
	printf("Press 'q' to quit.\n");

	while(!is_halted(vm)){
		if(vm_getc(vm, &output)){
			putchar((int)output);
			fflush(stdout);
		}

		if(read(STDIN_FILENO, &c, 1) > 0){
			vm_putc(vm, c);
		}
		step(vm);
	}

	vm_destroy(vm);
        tcsetattr(STDOUT_FILENO,TCSANOW, original);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH, original);
        free(original);
	return 0;
}
