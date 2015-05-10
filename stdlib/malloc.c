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
#include "stdlib.h"
#include "stdio.h"

extern unsigned int free_mem_start;
extern unsigned int free_mem_end;
unsigned int malloc_inited = 0;

unsigned int heap_size;
unsigned int num_blocks;
unsigned int first_block_addr;
unsigned int * used_flags;

void * malloc(size_t);

void malloc_init(void){
	unsigned int i;
	/*  Total number of bytes malloc has to work with */
	heap_size = free_mem_end - free_mem_start;
	/*  Num of 1kb blocks we can give to the user */
	num_blocks = heap_size / (1024 + 4); /*  How many 1kb blocks and flags can we have? */
	/*  The location where we store the 'allocated' flags */
	used_flags = (unsigned int *)free_mem_start;
	/*  The address of the first 1kb block */
	first_block_addr = free_mem_start + (num_blocks * 4);
	/*  All unused in the beginning  */
	for(i = 0; i < num_blocks; i++){
		used_flags[i] = 0;
	}
}

void * malloc(size_t num_bytes){
	unsigned int i;
	if(!malloc_inited){
		malloc_init();
		malloc_inited = 1;
	}

	if(num_bytes > 1024){
		printf("Cannot allocate greater than 1024 byte block!\n");
	}else{
		for(i = 0; i < num_blocks; i++){
			if(!used_flags[i]){
				used_flags[i] = 1;
				return (void *)(first_block_addr + i * 1024);
			}
		}
	}
	
	printf("Out of memory!\n");
	return (void*)0;
}

void free(void * ptr){
	unsigned int index;
	if(!malloc_inited){
		printf("Malloc not inited.\n");
		return;
	}

	index = (((unsigned int)ptr) - first_block_addr) / 1024;
	if(!(index < num_blocks)){
		printf("Unknown index for deallocation: %d\n", index);
	}else{
		if(used_flags[index]){
			used_flags[index] = 0;/*  Deallocate */
		}else{
			printf("Index %d is not allocated.\n", index);
		}
	}
}
