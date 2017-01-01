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

#include "filesystem.h"

static int unsigned_char_ptr_cmp(unsigned char *, unsigned char *);

static int unsigned_char_ptr_cmp(unsigned char * a, unsigned char * b){
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}else{
		return 0;
	}
}

void initialize_root_directory(void){
	/*  This needs to be the first inode allocated */
	unsigned int root_inode = allocate_inode();
	assert(root_inode == 0);
	inodes[root_inode].first_block = 0;
	inodes[root_inode].first_block_initialized = 0;
	inodes[root_inode].metadata_block = 0;
	inodes[root_inode].is_directory = 1;

	add_directory_entry_pointing_at_inode(0, (unsigned char *)".", 0);
	add_directory_entry_pointing_at_inode(0, (unsigned char *)"..", 0);
}

void initialize_filesystem_datastructures(void){
	unsigned int i;
	/*  To start off with, all inodes and blocks are free */
	for(i = 0; i < NUM_BLOCKS; i++){
		free_blocks[i] = i;
	}
	for(i = 0; i < NUM_INODES; i++){
		free_inodes[i] = i;
	}
	/*  The first block or inode to be allocated will be the one at the top of the stack */
	free_blocks_stack_top = 0;
	free_inodes_stack_top = 0;
	initialize_root_directory();
}

unsigned int allocate_block(void){
	unsigned int rtn = free_blocks[free_blocks_stack_top];
	assert(free_blocks_stack_top < NUM_BLOCKS);
	free_blocks_stack_top++;
	return rtn;
}

unsigned int allocate_inode(void){
	unsigned int rtn = free_inodes[free_inodes_stack_top];
	assert(free_inodes_stack_top < NUM_INODES);
	free_inodes_stack_top++;
	return rtn;
}

unsigned int get_directory_entry_inode(unsigned char * entry_name, unsigned int parent_inode, unsigned int is_directory){
	unsigned int current_block;
	struct directory_entry * current_directory_entry;
	assert(inodes[parent_inode].is_directory);
	assert(inodes[parent_inode].first_block_initialized);

	current_block = inodes[parent_inode].first_block;
	current_directory_entry = (struct directory_entry *)&blocks[current_block].data[0];

	while(unsigned_strcmp(entry_name, current_directory_entry->file_name) && blocks[current_block].header.next_block_initialized){
		current_block = blocks[current_block].header.next_block;
		current_directory_entry = (struct directory_entry *)&blocks[current_block].data[0];
	}

	/*  Did we end on the entry we're looking for? */
	if(!unsigned_strcmp(entry_name, current_directory_entry->file_name)){
		unsigned int rtn_inode = current_directory_entry->inode;
		if(is_directory){
			assert(inodes[rtn_inode].is_directory);
		}else{
			assert(!inodes[rtn_inode].is_directory);
		}
		return rtn_inode;
	}else{
		printf("Failed to find %s\n", entry_name);
		assert(0 && "Did not find entry.");
		return 0;
	}
}

unsigned int get_directory_inode_from_path_parts(struct memory_pool_collection * m, struct unsigned_char_ptr_list * path_components){
	unsigned int current_inode = 0; /*  0 is root inode */
	unsigned char * root_component = unsigned_char_ptr_list_get(path_components, 0);
	unsigned int i;
	/*  The first component represents the root inode, which is constant */
	unsigned_char_ptr_list_remove_all(path_components, root_component, unsigned_char_ptr_cmp);
	heap_memory_pool_free(m, root_component);

	for(i = 0; i < unsigned_char_ptr_list_size(path_components); i++){
		current_inode = get_directory_entry_inode(unsigned_char_ptr_list_get(path_components, i), current_inode, 1);
	}
	return current_inode;
}

unsigned int create_new_directory_entry_block_pointing_at_inode(unsigned int previous, unsigned char * file_name, unsigned int inode){
	unsigned int rtn = allocate_block();
	struct directory_entry * entry;
	unsigned int namelen = (unsigned int)strlen((const char *)file_name);
	unsigned int i;
	assert(namelen + 1 < DATA_WORDS_PER_BLOCK * sizeof(unsigned int));
	blocks[rtn].header.next_block = 0;
	blocks[rtn].header.next_block_initialized = 0;
	blocks[rtn].header.previous_block = previous;
	blocks[rtn].header.previous_block_initialized = 1;
	blocks[rtn].header.bytes_used = sizeof(unsigned int) + namelen + 1;
	entry = (struct directory_entry *)&blocks[rtn].data[0];
	entry->inode = inode;
	for(i = 0; i < namelen; i++){
		entry->file_name[i] = file_name[i];
	}
	entry->file_name[i] = 0; /*  Null termination */
	return rtn;
}

void set_file_size_given_inode(unsigned int inode, unsigned int target_size){
	assert(!inodes[inode].is_directory);
	if(inodes[inode].first_block_initialized){
		assert(0 && "Not implemented.");
	}else{
		unsigned int block_index;
		inodes[inode].first_block = allocate_block();
		inodes[inode].first_block_initialized = 1;

		blocks[inodes[inode].first_block].header.previous_block = 0;
		blocks[inodes[inode].first_block].header.previous_block_initialized = 0;

		blocks[inodes[inode].first_block].header.next_block = 0;
		blocks[inodes[inode].first_block].header.next_block_initialized = 0;
		block_index = inodes[inode].first_block;

		while(target_size > 0){
			unsigned int bytes_to_expand;
			unsigned int new_block_index;
			if(target_size <= (DATA_WORDS_PER_BLOCK * sizeof(unsigned int))){
				bytes_to_expand = target_size;
			}else{
				bytes_to_expand = (DATA_WORDS_PER_BLOCK * sizeof(unsigned int));
			}
			blocks[block_index].header.bytes_used = bytes_to_expand;
			target_size -= bytes_to_expand;
			/*  Check if more blocks need to be allocated */
			if(target_size > 0){
				new_block_index = allocate_block();
				blocks[block_index].header.next_block = new_block_index;
				blocks[block_index].header.next_block_initialized = 1;
				blocks[new_block_index].header.next_block = 0;
				blocks[new_block_index].header.next_block_initialized = 0;
				blocks[new_block_index].header.previous_block = block_index;
				blocks[new_block_index].header.previous_block_initialized = 1;
				block_index = new_block_index;
			}
		}
	}
}

unsigned int add_directory_entry(unsigned int parent, unsigned char * filename, unsigned int is_directory){
	unsigned int new_inode = allocate_inode();
	inodes[new_inode].is_directory = is_directory;
	inodes[new_inode].first_block = 0;
	inodes[new_inode].first_block_initialized = 0;
	inodes[new_inode].metadata_block = 0;
	inodes[new_inode].metadata_block_initialized = 0;
	return add_directory_entry_pointing_at_inode(parent, filename, new_inode);
}

unsigned int add_directory_entry_pointing_at_inode(unsigned int parent_inode, unsigned char * filename, unsigned int inode){
	unsigned int new_file_entry_block;
	if(inodes[parent_inode].first_block_initialized){
		/*  Add the new entry onto the end */
		unsigned int current_block = inodes[parent_inode].first_block;
		while(blocks[current_block].header.next_block_initialized){
			current_block = blocks[current_block].header.next_block;
		}
		new_file_entry_block = create_new_directory_entry_block_pointing_at_inode(current_block, filename, inode);
		blocks[current_block].header.next_block = new_file_entry_block;
		blocks[current_block].header.next_block_initialized = 1;
		printf("Added new file %s on end for inode %d\n", filename, parent_inode);
	}else{
		new_file_entry_block = create_new_directory_entry_block_pointing_at_inode(0, filename, inode);
		inodes[parent_inode].first_block = new_file_entry_block;
		inodes[parent_inode].first_block_initialized = 1;
		printf("Added first file %s to inode %d\n", filename, parent_inode);
	}
	/*  Return the inode for the file or directory that we just created */
	return ((struct directory_entry *)(&blocks[new_file_entry_block].data[0]))->inode;
}

unsigned int create_file_given_parent_inode(unsigned int parent_inode, unsigned char * filename, unsigned int is_directory){
	unsigned int new_file_inode = add_directory_entry(parent_inode, filename, is_directory);
	if(is_directory){
		add_directory_entry_pointing_at_inode(new_file_inode, (unsigned char *)".", new_file_inode);
		add_directory_entry_pointing_at_inode(new_file_inode, (unsigned char *)"..", parent_inode);
	}
	return new_file_inode;
}

void create_directory(unsigned char * d, struct memory_pool_collection * m){
	struct unsigned_char_ptr_list components;
	unsigned int parent_directory_inode;
	unsigned char * directory_name;
	unsigned int i;
	unsigned_char_ptr_list_create(&components);
	resolve_path_components(d, &components, m);
	/*  Last path component is directory to create */
	directory_name = unsigned_char_ptr_list_get(&components, unsigned_char_ptr_list_size(&components) -1);
	unsigned_char_ptr_list_remove_all(&components, directory_name, unsigned_char_ptr_cmp);
	/*  Last component is new directory to create */
	parent_directory_inode = get_directory_inode_from_path_parts(m, &components);
	printf("Creating directory %s with full path %s\n", directory_name, d);
	create_file_given_parent_inode(parent_directory_inode, directory_name, 1);
	for(i = 0; i < unsigned_char_ptr_list_size(&components);i++){
		heap_memory_pool_free(m, unsigned_char_ptr_list_get(&components, i));
	}
	unsigned_char_ptr_list_destroy(&components);
	heap_memory_pool_free(m, directory_name);
}
