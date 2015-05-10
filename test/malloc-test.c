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
#include <stdlib.h>
#include <stdio.h>

unsigned int get_input_space_partitioned_random(void);

int main(void){
	unsigned int num_items = 20;
	unsigned int i;
	unsigned int j;
	unsigned int ** p = (unsigned int **)malloc(sizeof(unsigned int *) * num_items);
	for(i = 0; i < num_items; i++){
		p[i] = (unsigned int*)malloc(sizeof(unsigned int) * num_items);
		for(j = 0; j < num_items; j++){
			p[i][j] = get_input_space_partitioned_random();
		}
	}
	for(i = 0; i < num_items; i++){
		for(j = 0; j < num_items; j++){
			printf("%X\n", p[i][j]);
		}
		free(p[i]);
	}
	free(p);
	return 0;
}
