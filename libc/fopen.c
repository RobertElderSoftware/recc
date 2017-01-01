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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "filesystem.h"

#define BUFFER_LEN 1000
static int unsigned_char_ptr_cmp_fopen(unsigned char *, unsigned char *);

static int unsigned_char_ptr_cmp_fopen(unsigned char * a, unsigned char * b){
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}else{
		return 0;
	}
}

FILE * fopen(const char * filepath, const char * mode){
	FILE * f = malloc(sizeof(FILE));
	struct unsigned_char_ptr_list components;
	unsigned int parent_directory_inode;
	unsigned int existing_file_inode;
	unsigned char * file_name;
	struct unsigned_char_list full_path_char_list;
	unsigned int i;
	char cwd[BUFFER_LEN];
	struct memory_pool_collection mpc;
	memory_pool_collection_create(&mpc);
	heap_memory_pool_create(&mpc);
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
	resolve_path_components((unsigned char *)unsigned_char_list_data(&full_path_char_list), &components, &mpc);
	file_name = unsigned_char_ptr_list_get(&components, unsigned_char_ptr_list_size(&components) -1);
	unsigned_char_ptr_list_remove_all(&components, file_name, unsigned_char_ptr_cmp_fopen);
	heap_memory_pool_free(&mpc, file_name);
	parent_directory_inode = get_directory_inode_from_path_parts(&mpc, &components);
	existing_file_inode = get_directory_entry_inode(file_name, parent_directory_inode, 0);
	f->inode_index = existing_file_inode;
	f->position = 0;
	assert(inodes[f->inode_index].first_block_initialized);
	f->current_block_index = inodes[f->inode_index].first_block;
	for(i = 0; i < unsigned_char_ptr_list_size(&components); i++){
		heap_memory_pool_free(&mpc, unsigned_char_ptr_list_get(&components, i));
	}
	unsigned_char_ptr_list_destroy(&components);
	unsigned_char_list_destroy(&full_path_char_list);

	heap_memory_pool_destroy(&mpc);
	memory_pool_collection_destroy(&mpc);

	return f;
}

int fclose(FILE *f){
	free(f);
}

size_t fwrite(const void * src, size_t item_size, size_t num_items, FILE * f){
	(void)src;
	(void)item_size;
	(void)num_items;
	(void)f;
	assert(0 && "Not implemented.");
}

size_t fread(void * dst, size_t item_size, size_t num_items, FILE * f){
	unsigned char * dst_char_ptr = dst;
	unsigned int index_in_block;
	unsigned int bytes_read = 0;
	unsigned int bytes_used_current_block = blocks[f->current_block_index].header.bytes_used;
	char * data_ptr = (char *)&(blocks[f->current_block_index].data[0]);
	assert(item_size == 1 && "Only considering item size 1 for now.");
	while(bytes_read < (item_size * num_items)){
		index_in_block = f->position % (DATA_WORDS_PER_BLOCK * sizeof(unsigned int));
		if(index_in_block == bytes_used_current_block){ /* index is past end of file */
			break;
		}
		if(index_in_block == (bytes_used_current_block -1)){
			/*  Go to next block */
			if(blocks[f->current_block_index].header.next_block_initialized){
				f->current_block_index = blocks[f->current_block_index].header.next_block;
				data_ptr = (char *)&(blocks[f->current_block_index].data[0]);
				bytes_used_current_block = blocks[f->current_block_index].header.bytes_used;
				f->position = f->position + 1;
				continue;
			}
		}
		f->position = f->position + 1;
		dst_char_ptr[bytes_read] = data_ptr[index_in_block];
		bytes_read++;
	}
	return bytes_read;
}

int getc(FILE * f){
	char * data_ptr;
	char data_rtn;
	unsigned int index_in_block = f->position % (DATA_WORDS_PER_BLOCK * sizeof(unsigned int));
	if(index_in_block == blocks[f->current_block_index].header.bytes_used){ /* index is past end of file */
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
