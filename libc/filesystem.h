#ifndef STDLIB_FILESYSTEM_H_
#define STDLIB_FILESYSTEM_H_
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

#define DATA_WORDS_PER_BLOCK 1024
#define NUM_INODES 1024
#define NUM_BLOCKS 1024
#define TIMES_4 4096 /*  TODO: get rid of this when multiplicative constants are implemented */

#ifndef unsigned_char_ptr_to_unsigned_char_ptr_map_H_DEFINED_
#include "../generated/unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../generated/unsigned_char_list.h"
#endif
#ifndef IO_H_
#include "../recc-implementation/io.h"
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef STDLIB_H_DEFINED_
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
void create_directory(unsigned char *, struct memory_pool_collection *);
unsigned int get_directory_inode_from_path_parts(struct memory_pool_collection *, struct unsigned_char_ptr_list *);
unsigned int get_directory_entry_inode(unsigned char *, unsigned int, unsigned int);
unsigned int create_file_given_parent_inode(unsigned int, unsigned char *, unsigned int);
unsigned int create_new_directory_entry_block_pointing_at_inode(unsigned int, unsigned char *, unsigned int);
unsigned int create_new_directory_entry_block(unsigned int, unsigned char *, unsigned int);
void set_file_size_given_inode(unsigned int, unsigned int);
unsigned int open_existing_file_given_parent_inode(unsigned int, unsigned char *);
unsigned int add_directory_entry_pointing_at_inode(unsigned int, unsigned char *, unsigned int);
unsigned int add_directory_entry(unsigned int, unsigned char *, unsigned int);

#endif
