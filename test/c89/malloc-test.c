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
	char * str = (char *)malloc(sizeof(char) * 8);
	char * str2 = (char *)malloc(sizeof(char) * 256);
	char * cpy = str2;
	for(i = 0; i < 256; i++){
		cpy[0] = (char)i;
		cpy++;
	}
	cpy[0] = 0;
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	str[3] = 'd';
	str[4] = 'e';
	str[5] = 'f';
	str[6] = 'g';
	str[7] = 0;
	printf("%s", str2);
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
		free((void*)p[i]);
	}
	free((void*)p);
	free((void*)str);
	free((void*)str2);
	return 0;
}
