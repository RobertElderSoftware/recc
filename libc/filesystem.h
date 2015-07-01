#ifndef __STDLIB_FILESYSTEM_H__
#define __STDLIB_FILESYSTEM_H__
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

#define DATA_WORDS_PER_BLOCK 1024
#define NUM_INODES 1024
#define NUM_BLOCKS 1024
#define TIMES_4 4096 /*  TODO: get rid of this when multiplicative constants are implemented */

#ifndef __unsigned_char_ptr_to_unsigned_char_ptr_map__H__DEFINED__
#include "../data-structures/unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "../data-structures/unsigned_char_list.h"
#endif
#ifndef __IO_H__
#include "../io.h"
#endif
#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif
#ifndef __STDLIB_H_DEFINED__
#include <stdlib.h>
#endif


struct directory_entry{
	unsigned int inode;
	unsigned char file_name[TIMES_4];
};

struct filesystem_block_header{
	unsigned int previous_block;
	unsigned int previous_block_initialized;
	unsigned int next_block;
	unsigned int next_block_initialized;
	unsigned int bytes_used;
};

struct filesystem_block{
	struct filesystem_block_header header;
	unsigned int data[DATA_WORDS_PER_BLOCK];
};

struct filesystem_inode{
	unsigned int first_block;
	unsigned int first_block_initialized;
	unsigned int metadata_block; /*  For future use in efficiently navigating file */
	unsigned int metadata_block_initialized;
	unsigned int is_directory;
};

extern struct filesystem_block blocks[NUM_BLOCKS];
extern struct filesystem_inode inodes[NUM_INODES];
extern unsigned int free_blocks_stack_top;
extern unsigned int free_inodes_stack_top;
extern unsigned int free_blocks[NUM_BLOCKS];
extern unsigned int free_inodes[NUM_INODES];

void initialize_filesystem_datastructures(void);
void initialize_root_directory(void);
unsigned int allocate_inode(void);
unsigned int allocate_block(void);
void create_directory(unsigned char *);
unsigned int get_directory_inode_from_path_parts(struct unsigned_char_ptr_list *);
unsigned int get_directory_entry_inode(unsigned char *, unsigned int, unsigned int);
unsigned int create_file_given_parent_inode(unsigned int, unsigned char *, unsigned int);
unsigned int create_new_directory_entry_block_pointing_at_inode(unsigned int, unsigned char *, unsigned int);
unsigned int create_new_directory_entry_block(unsigned int, unsigned char *, unsigned int);
void set_file_size_given_inode(unsigned int, unsigned int);
unsigned int open_existing_file_given_parent_inode(unsigned int, unsigned char *);
unsigned int add_directory_entry_pointing_at_inode(unsigned int, unsigned char *, unsigned int);
unsigned int add_directory_entry(unsigned int, unsigned char *, unsigned int);

#endif
