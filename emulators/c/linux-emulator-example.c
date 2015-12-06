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
#include <pthread.h>

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
        return original;
}

static volatile int c = -1;
static pthread_mutex_t c_mutex = PTHREAD_MUTEX_INITIALIZER;

static void* read_stdin(void *arg __attribute__((unused))) {
	while(1) {
		unsigned char a;
		if(read(STDIN_FILENO, &a, 1) == 1) {
			pthread_mutex_lock(&c_mutex);
			c = a;
			pthread_mutex_unlock(&c_mutex);
		}
	}
	return NULL;
}

int main(void){
	struct virtual_machine * vm;
	unsigned int output;
	struct termios * original = terminal_setup();
	pthread_t stdin_thread;

	vm = vm_create(data_start, data_end, data);
	printf("Kernel image has been loaded. All input is now being handled by the emulator.\n");
	printf("Press 'q' to quit.\n");

	pthread_create(&stdin_thread, NULL, read_stdin, NULL);

	while(!is_halted(vm)){
		if(vm_getc(vm, &output)){
			putchar((int)output);
			fflush(stdout);
		}

		if(c != -1) {
			unsigned char a;
			pthread_mutex_lock(&c_mutex);
			a = c;
			c = -1;
			pthread_mutex_unlock(&c_mutex);
			vm_putc(vm, a);
		}
		step(vm);
	}

	vm_destroy(vm);
        tcsetattr(STDOUT_FILENO,TCSANOW, original);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH, original);
        free(original);
	return 0;
}
