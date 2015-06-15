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
#include "filesystem.h"

FILE * fopen(const char * filepath, const char * mode){
	FILE * f = malloc(sizeof(FILE));
	struct unsigned_char_ptr_list components;
	unsigned int parent_directory_inode;
	unsigned int existing_file_inode;
	unsigned char * file_name;
	(void)mode; /*  TODO: inspect mode */
	unsigned_char_ptr_list_create(&components);
	resolve_path_components((unsigned char *)filepath, &components);
	file_name = unsigned_char_ptr_list_get(&components, unsigned_char_ptr_list_size(&components) -1);
	unsigned_char_ptr_list_remove_all(&components, file_name);
	parent_directory_inode = get_directory_inode_from_path_parts(&components);
	existing_file_inode = get_directory_entry_inode(file_name, parent_directory_inode, 0);
	f->inode_index = existing_file_inode;
	f->position = 0;
	assert(inodes[f->inode_index].first_block_initialized);
	f->current_block_index = inodes[f->inode_index].first_block;
	unsigned_char_ptr_list_destroy(&components);
	return f;
}

int fclose(FILE *f){
	free(f);
}

int getc(FILE * f){
	char * data_ptr;
	char data_rtn;
	unsigned int index_in_block = f->position % (DATA_WORDS_PER_BLOCK * sizeof(unsigned int));
	if(index_in_block == blocks[f->current_block_index].header.bytes_used){
		return EOF;
	}
	data_ptr = (char *)&(blocks[f->current_block_index].data[0]);
	if(index_in_block == (blocks[f->current_block_index].header.bytes_used -1)){
		/*  Go to next block */
		if(blocks[f->current_block_index].header.next_block_initialized){
			f->current_block_index = blocks[f->current_block_index].header.next_block;
		}
	}
	f->position = f->position + 1;
	data_rtn = data_ptr[index_in_block];
	return data_rtn;
}

int fputc(int c, FILE * f){
	assert(0);
	(void)f;
	(void)c;
}
