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
#include <unistd.h>
#include "filesystem.h"

#define BUFFER_LEN 1000

FILE * fopen(const char * filepath, const char * mode){
	FILE * f = malloc(sizeof(FILE));
	struct unsigned_char_ptr_list components;
	unsigned int parent_directory_inode;
	unsigned int existing_file_inode;
	unsigned char * file_name;
	struct unsigned_char_list full_path_char_list;
	unsigned int i;
	char cwd[BUFFER_LEN];
	unsigned_char_list_create(&full_path_char_list);
	(void)mode; /*  TODO: inspect mode */

	assert(filepath);

	if(*filepath != '/'){ /*  If the file was not an absolute path */
		/*  Get current working directory. */
		getcwd(cwd, BUFFER_LEN);

		/*  Add current working directory to target file path */
		for(i = 0; ; i++){
			if(!cwd[i]){
				break;
			}
			unsigned_char_list_add_end(&full_path_char_list, cwd[i]);
		}
		if(unsigned_char_list_get(&full_path_char_list, unsigned_char_list_size(&full_path_char_list) -1) != '/'){
			unsigned_char_list_add_end(&full_path_char_list, '/');
		}
	}

	while(*filepath){
		unsigned_char_list_add_end(&full_path_char_list, *filepath);
		filepath++;
	}

	unsigned_char_list_add_end(&full_path_char_list, 0); /* Null terminator */

	unsigned_char_ptr_list_create(&components);
	resolve_path_components((unsigned char *)unsigned_char_list_data(&full_path_char_list), &components);
	file_name = unsigned_char_ptr_list_get(&components, unsigned_char_ptr_list_size(&components) -1);
	unsigned_char_ptr_list_remove_all(&components, file_name);
	parent_directory_inode = get_directory_inode_from_path_parts(&components);
	existing_file_inode = get_directory_entry_inode(file_name, parent_directory_inode, 0);
	f->inode_index = existing_file_inode;
	f->position = 0;
	assert(inodes[f->inode_index].first_block_initialized);
	f->current_block_index = inodes[f->inode_index].first_block;
	unsigned_char_ptr_list_destroy(&components);
	unsigned_char_list_destroy(&full_path_char_list);
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
