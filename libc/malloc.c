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

#define WORD_SIZE sizeof(unsigned int)
#define WORDS_MEMORY_AVAILABLE 0x1000000  /*  Change this value to increase or decrease the amount of memory available */
#define BYTES_MEMORY_AVAILABLE (WORDS_MEMORY_AVAILABLE * WORD_SIZE)
#define BYTES_PER_BLOCK 0x4000

unsigned int free_mem[WORDS_MEMORY_AVAILABLE];
unsigned int malloc_inited = 0;

unsigned int heap_size;
unsigned int num_blocks;
unsigned int * first_block_addr;
unsigned int * used_flags;

void * malloc(size_t);

unsigned int get_block_index_from_pointer(void * ptr){
	if(!((((unsigned int)ptr) - (unsigned int)first_block_addr) % BYTES_PER_BLOCK == 0)){
		printf("Invalid pointer: %p\n", ptr);
		assert(0);
	}
	return (((unsigned int)ptr) - (unsigned int)first_block_addr) / BYTES_PER_BLOCK;
}

void malloc_init(void){
	unsigned int i;
	/*  Total number of bytes malloc has to work with */
	heap_size = BYTES_MEMORY_AVAILABLE;
	/*  Num of 1kb blocks we can give to the user */
	num_blocks = heap_size / (BYTES_PER_BLOCK + WORD_SIZE); /*  How many blocks and flags can we have? */
	/*  The location where we store the 'allocated' flags */
	used_flags = &free_mem[0];
	/*  The address of the first block */
	first_block_addr = &free_mem[num_blocks];
	/*  All unused in the beginning  */
	for(i = 0; i < num_blocks; i++){
		used_flags[i] = 0;
	}
}

void * malloc(size_t num_bytes){
	if(num_bytes){
		unsigned int i;
		unsigned int blocks_needed = ((num_bytes % BYTES_PER_BLOCK) == 0 ? 0 : 1) + (num_bytes / BYTES_PER_BLOCK);
		/*printf("malloc size %d, need %d blocks\n", num_bytes, blocks_needed);*/
		if(!malloc_inited){
			malloc_init();
			malloc_inited = 1;
		}

		/*  Find a contiguous region of n blocks of BYTES_PER_BLOCK that are free */
		for(i = 0; i < (num_blocks + (blocks_needed -1)); i++){
			unsigned int j;
			unsigned int ok_to_allocate = 1;
			for(j = 0; j < blocks_needed; j++){
				if(used_flags[i + j]){
					/*  There was a block allocated, try again */
					ok_to_allocate = 0;
					i+=j; /*  Avoid making this check n^2 */
					break;
				}
			}
			if(ok_to_allocate){
				void * rtn;
				for(j = 0; j < blocks_needed; j++){
					used_flags[i + j] = blocks_needed - j; /*  Value indicates number of blocks ahead that are allocated */
				}
				rtn = (void *)(((unsigned int)first_block_addr) + (i * BYTES_PER_BLOCK));
				/*printf("finish malloc %p\n", rtn);*/
				return rtn;
			}
		}
		assert(0 && "Out of memory!\n");
	}else{
		return (void*)0;
	}
}

void * realloc(void * ptr, size_t num_bytes){
	if(ptr){
		unsigned int old_index = get_block_index_from_pointer(ptr);
		unsigned int old_num_blocks = used_flags[old_index];
		unsigned int old_region_size = old_num_blocks * BYTES_PER_BLOCK;
		assert(old_num_blocks);

		/*printf("old used flag value: %d.\n", old_num_blocks);*/
		/*printf("realloc size %d region (%p) to %d size region.\n", old_region_size, ptr, num_bytes);*/
		if(num_bytes > old_region_size){
			void * new_ptr = malloc(num_bytes);
			unsigned int i;
			unsigned int * src = (unsigned int *)ptr;
			unsigned int * dst = (unsigned int *)new_ptr;
			unsigned int new_index = get_block_index_from_pointer(new_ptr);
			unsigned int new_num_blocks = used_flags[new_index];
			unsigned int min_num_blocks = old_num_blocks < new_num_blocks ? old_num_blocks : new_num_blocks;

			for(i = 0; i < ((min_num_blocks * BYTES_PER_BLOCK) / WORD_SIZE); i++){
				dst[i] = src[i];
			}
			free(ptr);
			/*printf("Complete realloc return new ptr %p\n", new_ptr);*/
			return new_ptr;
		}else{
			/*printf("Complete realloc return ptr %p\n", ptr);*/
			return ptr;
		}
	}else{
		return malloc(num_bytes);
	}
}

void free(void * ptr){
	if(ptr){
		unsigned int index;
		/*printf("Free region %p\n", ptr);*/
		if(!malloc_inited){
			printf("Malloc not inited.\n");
			return;
		}

		index = get_block_index_from_pointer(ptr);
		if(index < num_blocks){
			if(used_flags[index]){
				unsigned int num_blocks = used_flags[index];
				unsigned int i;
				for(i = 0; i < num_blocks; i++){
					assert(used_flags[index] = (num_blocks - i));				
					used_flags[index] = 0;/*  Deallocate */
				}
			}else{
				assert(0 && "Index %d is not allocated.\n", index);
			}
		}else{
			assert(0 && "Unknown index for deallocation: %d\n", index);
		}
	}
}
