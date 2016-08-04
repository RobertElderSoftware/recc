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

/*

Takes input files on host machine and transforms them into the l2 files required
for a filesystem hosting those same files as viewed from any program linking them.

*/
#include "filesystem_compiler.h"

unsigned int free_blocks_stack_top;
unsigned int free_inodes_stack_top;
struct filesystem_block blocks[NUM_BLOCKS];
struct filesystem_inode inodes[NUM_INODES];
unsigned int free_blocks[NUM_BLOCKS];
unsigned int free_inodes[NUM_INODES];

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

unsigned char printable_character(unsigned char c);
unsigned char printable_character(unsigned char c){
	if(c > 31 && c < 125){
		return c;
	}else{
		return ' ';
	}
}

void output_filesystem_impl(unsigned char * out_file){
	FILE *f = NULL;
	unsigned int i;
	if(!(f = fopen((char*)out_file, "w"))){
		printf("Failed to open file %s for write.\n", out_file);
		return;
	}

	fprintf(f, "OFFSET RELOCATABLE;\n");
	fprintf(f, "VARIABLE globalvar_free_blocks_stack_top _end_globalvar_free_blocks_stack_top;\n");
	fprintf(f, "VARIABLE globalvar_free_inodes_stack_top _end_globalvar_free_inodes_stack_top;\n");
	fprintf(f, "VARIABLE globalvar_blocks _end_globalvar_blocks;\n");
	fprintf(f, "VARIABLE globalvar_inodes _end_globalvar_inodes;\n");
	fprintf(f, "VARIABLE globalvar_free_blocks _end_globalvar_free_blocks;\n");
	fprintf(f, "VARIABLE globalvar_free_inodes _end_globalvar_free_inodes;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_free_blocks_stack_top;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL _end_globalvar_free_blocks_stack_top;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_free_inodes_stack_top;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL _end_globalvar_free_inodes_stack_top;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_blocks;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL _end_globalvar_blocks;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_inodes;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL _end_globalvar_inodes;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_free_blocks;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL _end_globalvar_free_blocks;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_free_inodes;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL _end_globalvar_free_inodes;\n");
	fprintf(f, "globalvar_free_blocks_stack_top:\nDW 0x%X\n_end_globalvar_free_blocks_stack_top:\n", free_blocks_stack_top);
	fprintf(f, "globalvar_free_inodes_stack_top:\nDW 0x%X\n_end_globalvar_free_inodes_stack_top:\n", free_inodes_stack_top);
	fprintf(f, "globalvar_blocks:\n");
	for(i = 0; i < free_blocks_stack_top; i++){
		unsigned int j;
		unsigned int * block_header_int_ptr = (unsigned int*)&blocks[i].header;
		unsigned int * block_data_int_ptr = (unsigned int*)&blocks[i].data[0];
		unsigned int block_header_size = sizeof(struct filesystem_block_header);
		unsigned int num_ints_header = block_header_size / sizeof(unsigned int);
		unsigned int num_ints_data = (blocks[i].header.bytes_used / sizeof(unsigned int)) + ((blocks[i].header.bytes_used % sizeof(unsigned int) == 0) ? 0 : 1);
		assert(block_header_size % sizeof(unsigned int) == 0);
		for(j = 0; j < num_ints_header; j++){
			fprintf(f, "DW 0x%X;  Block %d entry header\n", block_header_int_ptr[j], i);
		}
		for(j = 0; j < num_ints_data; j++){
			fprintf(f, "DW 0x%X;  %c%c%c%c\n", block_data_int_ptr[j], printable_character(((unsigned char *)&block_data_int_ptr[j])[3]), printable_character(((unsigned char *)&block_data_int_ptr[j])[2]),printable_character(((unsigned char *)&block_data_int_ptr[j])[1]),printable_character(((unsigned char *)&block_data_int_ptr[j])[0]));
		}
		fprintf(f, "SW 0x%X; Unused region in block %d\n", DATA_WORDS_PER_BLOCK - num_ints_data, i);
	}
	fprintf(f, "SW 0x%X;  Space for remaining (%d) unused blocks\n", (unsigned int)((NUM_BLOCKS - free_blocks_stack_top) * sizeof(struct filesystem_block)), NUM_BLOCKS - free_blocks_stack_top);
	fprintf(f, "_end_globalvar_blocks:\n");
	fprintf(f, "globalvar_inodes:\n");
	for(i = 0; i < free_inodes_stack_top; i++){
		unsigned int j;
		unsigned int * inode_int_ptr = (unsigned int*)&inodes[i];
		unsigned int num_ints_inode = sizeof(struct filesystem_inode) / sizeof(unsigned int);
		assert(sizeof(struct filesystem_inode) % sizeof(unsigned int) == 0);
		for(j = 0; j < num_ints_inode; j++){
			fprintf(f, "DW 0x%X;  Inode %d\n", inode_int_ptr[j], i);
		}
	}
	fprintf(f, "SW 0x%X;  Space for remaining (%d) unused inodes\n", (unsigned int)((NUM_INODES - free_inodes_stack_top) * sizeof(struct filesystem_inode)), NUM_INODES - free_inodes_stack_top);
	fprintf(f, "_end_globalvar_inodes:\n");
	fprintf(f, "globalvar_free_blocks:\n");
	for(i = 0; i < NUM_BLOCKS; i++){
		fprintf(f, "DW 0x%X;\n", free_blocks[i]);
	}
	fprintf(f, "_end_globalvar_free_blocks:\n");
	fprintf(f, "globalvar_free_inodes:\n");
	for(i = 0; i < NUM_INODES; i++){
		fprintf(f, "DW 0x%X;\n", free_inodes[i]);
	}
	fprintf(f, "_end_globalvar_free_inodes:\n");

	fclose(f);
	return;
}

void load_file_given_new_inode(unsigned int inode, FILE * source_file){
	char c;
	unsigned int source_file_size = (unsigned int)ftell(source_file);
	unsigned int current_block;
	unsigned int position_in_block = 0;
	unsigned int more_writes_safe = 1;
	assert(!inodes[inode].first_block_initialized);
	fseek(source_file, 0L, SEEK_SET);
	set_file_size_given_inode(inode, source_file_size);
	current_block = inodes[inode].first_block;

	/*  Load our file into the new filesystem */
	while (!feof(source_file)) {
		c = (char)getc(source_file);
		if(c == EOF)
			break;
		assert(more_writes_safe); /*  Necessary if the filesize mod block size == 0*/
		((char *)&blocks[current_block].data[0])[position_in_block] = c;
		if(position_in_block == (DATA_WORDS_PER_BLOCK * sizeof(unsigned int)) -1){
			position_in_block = 0;
			more_writes_safe = blocks[current_block].header.next_block_initialized;
			current_block = blocks[current_block].header.next_block;
		}else{
			position_in_block++;
		}
	}
}

void load_file(unsigned char * d, FILE * f, struct memory_pool_collection * m){
	struct unsigned_char_ptr_list components;
	unsigned int parent_directory_inode;
	unsigned int new_file_inode;
	unsigned char * file_name;
	unsigned int i;
	unsigned_char_ptr_list_create(&components);
	resolve_path_components(d, &components, m);
	/*  Last path component is file to create */
	file_name = unsigned_char_ptr_list_get(&components, unsigned_char_ptr_list_size(&components) -1);
	unsigned_char_ptr_list_remove_all(&components, file_name, unsigned_char_ptr_cmp);
	/*  Last component is new file to create */
	parent_directory_inode = get_directory_inode_from_path_parts(m, &components);
	printf("Creating file %s with full path %s\n", file_name, d);
	new_file_inode = create_file_given_parent_inode(parent_directory_inode, file_name, 0);
	load_file_given_new_inode(new_file_inode, f);
	for(i = 0; i < unsigned_char_ptr_list_size(&components);i++){
		heap_memory_pool_free(m->heap_pool, unsigned_char_ptr_list_get(&components, i));
	}
	unsigned_char_ptr_list_destroy(&components);
	heap_memory_pool_free(m->heap_pool, file_name);
}



void create_directories(struct unsigned_char_ptr_list * directories, struct memory_pool_collection * m){
	unsigned int i;
	for(i = 0; i < unsigned_char_ptr_list_size(directories); i++){
		unsigned char * dir = copy_null_terminated_string(unsigned_char_ptr_list_get(directories, i), m);
		unsigned char * null_byte = get_null_terminator(dir);
		assert(*(null_byte -1) == '/' && "Directory should end with '/'.");
		create_directory(dir, m);
		heap_memory_pool_free(m->heap_pool, dir);
	}
}

void create_files(struct unsigned_char_ptr_to_unsigned_char_ptr_map * files, struct memory_pool_collection * m){
	struct unsigned_char_ptr_list file_keys = unsigned_char_ptr_to_unsigned_char_ptr_map_keys(files);
	unsigned int i;
	for(i = 0; i < unsigned_char_ptr_list_size(&file_keys); i++){
		unsigned char * destination_file = unsigned_char_ptr_list_get(&file_keys, i);
		unsigned char * source_file = unsigned_char_ptr_to_unsigned_char_ptr_map_get(files, unsigned_char_ptr_list_get(&file_keys, i));
		FILE * f;
		if(!(f = fopen((char *)source_file, "rb"))){
			printf("Failed to open file %s\n", source_file);
			assert(0);
		}
		fseek(f, 0L, SEEK_END);
		printf("Loading file %s with size %i into %s\n", source_file, (unsigned int)ftell(f), destination_file);
		load_file(destination_file, f, m);
		fclose(f);
	}
	unsigned_char_ptr_list_destroy(&file_keys);
}

#define NUM_FILES 253

void create_filesystem_impl(unsigned char * out_file){
	unsigned char * root_dir;
	struct unsigned_char_ptr_to_unsigned_char_ptr_map files;
	struct unsigned_char_ptr_to_unsigned_char_ptr_map directories;
	struct unsigned_char_ptr_list directory_keys;
	struct unsigned_char_list tmp;
	struct memory_pool_collection mpc;

	const char * filesystem_files[NUM_FILES][2] = {
		{"./generated/void_ptr_memory_pool.h", "/./generated/void_ptr_memory_pool.h"},
		{"./generated/void_ptr_to_unsigned_int_map.h", "/./generated/void_ptr_to_unsigned_int_map.h"},
		{"./generated/struct_constant_description_ptr_list.h", "/./generated/struct_constant_description_ptr_list.h"},
		{"./generated/struct_type_description_memory_pool.h", "/./generated/struct_type_description_memory_pool.h"},
		{"./generated/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_binary_search.h", "/./generated/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_binary_search.h"},
		{"./generated/unsigned_char_ptr_to_unsigned_char_ptr_map.h", "/./generated/unsigned_char_ptr_to_unsigned_char_ptr_map.h"},
		{"./generated/struct_normalized_declarator_ptr_list.h", "/./generated/struct_normalized_declarator_ptr_list.h"},
		{"./generated/struct_c_lexer_token_ptr_to_unsigned_char_ptr_map.h", "/./generated/struct_c_lexer_token_ptr_to_unsigned_char_ptr_map.h"},
		{"./generated/struct_macro_definition_ptr_list.h", "/./generated/struct_macro_definition_ptr_list.h"},
		{"./generated/unsigned_char_ptr_to_struct_macro_definition_ptr_map.h", "/./generated/unsigned_char_ptr_to_struct_macro_definition_ptr_map.h"},
		{"./generated/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_binary_search.h", "/./generated/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_binary_search.h"},
		{"./generated/struct_namespace_object_ptr_list.h", "/./generated/struct_namespace_object_ptr_list.h"},
		{"./generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_binary_search.h", "/./generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_binary_search.h"},
		{"./generated/struct_macro_parameter_ptr_list.h", "/./generated/struct_macro_parameter_ptr_list.h"},
		{"./generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_binary_search.h", "/./generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_binary_search.h"},
		{"./generated/unsigned_char_ptr_to_struct_special_macro_definition_ptr_map.h", "/./generated/unsigned_char_ptr_to_struct_special_macro_definition_ptr_map.h"},
		{"./generated/struct_constant_initializer_level_ptr_list.h", "/./generated/struct_constant_initializer_level_ptr_list.h"},
		{"./generated/struct_normalized_declaration_element_ptr_list.h", "/./generated/struct_normalized_declaration_element_ptr_list.h"},
		{"./generated/unsigned_int_list.h", "/./generated/unsigned_int_list.h"},
		{"./recc-implementation/replace_tool.h", "/./recc-implementation/replace_tool.h"},
		{"./generated/struct_preprocessor_file_context_ptr_list.h", "/./generated/struct_preprocessor_file_context_ptr_list.h"},
		{"./generated/struct_switch_frame_ptr_list.h", "/./generated/struct_switch_frame_ptr_list.h"},
		{"./generated/unsigned_int_binary_search.h", "/./generated/unsigned_int_binary_search.h"},
		{"./generated/struct_void_ptr_to_unsigned_int_key_value_pair_binary_search.h", "/./generated/struct_void_ptr_to_unsigned_int_key_value_pair_binary_search.h"},
		{"./generated/struct_l2_lexer_token_ptr_list.h", "/./generated/struct_l2_lexer_token_ptr_list.h"},
		{"./recc-implementation/binary_exponential_buffer.h", "/./recc-implementation/binary_exponential_buffer.h"},
		{"./generated/struct_linker_symbol_ptr_list.h", "/./generated/struct_linker_symbol_ptr_list.h"},
		{"./generated/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h", "/./generated/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h"},
		{"./generated/struct_type_traversal_ptr_list.h", "/./generated/struct_type_traversal_ptr_list.h"},
		{"./generated/char_list.h", "/./generated/char_list.h"},
		{"./generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_binary_search.h", "/./generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_binary_search.h"},
		{"./generated/struct_scope_level_ptr_list.h", "/./generated/struct_scope_level_ptr_list.h"},
		{"./generated/struct_l2_item_ptr_list.h", "/./generated/struct_l2_item_ptr_list.h"},
		{"./generated/struct_c_lexer_state_ptr_list.h", "/./generated/struct_c_lexer_state_ptr_list.h"},
		{"./generated/struct_unsigned_char_list_ptr_list.h", "/./generated/struct_unsigned_char_list_ptr_list.h"},
		{"./generated/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h", "/./generated/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h"},
		{"./generated/struct_l2_lexer_state_ptr_list.h", "/./generated/struct_l2_lexer_state_ptr_list.h"},
		{"./generated/unsigned_int_merge_sort.h", "/./generated/unsigned_int_merge_sort.h"},
		{"./generated/struct_c_lexer_token_memory_pool.h", "/./generated/struct_c_lexer_token_memory_pool.h"},
		{"./generated/unsigned_char_ptr_list.h", "/./generated/unsigned_char_ptr_list.h"},
		{"./generated/struct_parser_node_memory_pool.h", "/./generated/struct_parser_node_memory_pool.h"},
		{"./generated/unsigned_char_ptr_to_struct_namespace_object_ptr_map.h", "/./generated/unsigned_char_ptr_to_struct_namespace_object_ptr_map.h"},
		{"./generated/struct_normalized_specifier_ptr_list.h", "/./generated/struct_normalized_specifier_ptr_list.h"},
		{"./generated/struct_linker_symbol_memory_pool.h", "/./generated/struct_linker_symbol_memory_pool.h"},
		{"./generated/unsigned_char_list.h", "/./generated/unsigned_char_list.h"},
		{"./generated/struct_l2_lexer_token_memory_pool.h", "/./generated/struct_l2_lexer_token_memory_pool.h"},
		{"./generated/struct_linker_file_ptr_merge_sort.h", "/./generated/struct_linker_file_ptr_merge_sort.h"},
		{"./generated/struct_special_macro_definition_ptr_list.h", "/./generated/struct_special_macro_definition_ptr_list.h"},
		{"./generated/struct_parser_operation_list.h", "/./generated/struct_parser_operation_list.h"},
		{"./generated/struct_struct_c_lexer_token_ptr_list_ptr_list.h", "/./generated/struct_struct_c_lexer_token_ptr_list_ptr_list.h"},
		{"./generated/struct_linker_file_ptr_list.h", "/./generated/struct_linker_file_ptr_list.h"},
		{"./generated/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_binary_search.h", "/./generated/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_binary_search.h"},
		{"./generated/struct_c_lexer_token_ptr_list.h", "/./generated/struct_c_lexer_token_ptr_list.h"},
		{"./generated/char_ptr_list.h", "/./generated/char_ptr_list.h"},
		{"./generated/void_ptr_list.h", "/./generated/void_ptr_list.h"},
		{"./generated/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair_binary_search.h", "/./generated/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair_binary_search.h"},
		{"./generated/struct_preprocessor_if_branch_ptr_list.h", "/./generated/struct_preprocessor_if_branch_ptr_list.h"},
		{"./generated/struct_l2_item_memory_pool.h", "/./generated/struct_l2_item_memory_pool.h"},
		{"./generated/unsigned_int_ptr_list.h", "/./generated/unsigned_int_ptr_list.h"},
		{"./generated/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.h", "/./generated/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.h"},
		{"./generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_binary_search.h", "/./generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_binary_search.h"},
		{"./generated/struct_type_description_reference_list.h", "/./generated/struct_type_description_reference_list.h"},
		{"./generated/unsigned_char_ptr_to_struct_constant_description_ptr_map.h", "/./generated/unsigned_char_ptr_to_struct_constant_description_ptr_map.h"},
		{"./generated/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_binary_search.h", "/./generated/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_binary_search.h"},
		{"./recc-implementation/parser.h", "/./recc-implementation/parser.h"},
		{"./recc-implementation/linker.h", "/./recc-implementation/linker.h"},
		{"./recc-implementation/filesystem_compiler.h", "/./recc-implementation/filesystem_compiler.h"},
		{"./recc-implementation/heap_memory_pool.h", "/./recc-implementation/heap_memory_pool.h"},
		{"./recc-implementation/io.h", "/./recc-implementation/io.h"},
		{"./kernel/private_kernel_interface.h", "/./kernel/private_kernel_interface.h"},
		{"./kernel/user_proc.h", "/./kernel/user_proc.h"},
		{"./kernel/public_kernel_interface.h", "/./kernel/public_kernel_interface.h"},
		{"./kernel/kernel_state.h", "/./kernel/kernel_state.h"},
		{"./kernel/core_data_types.h", "/./kernel/core_data_types.h"},
		{"./kernel/queue.h", "/./kernel/queue.h"},
		{"./libc/size_t.h", "/./libc/size_t.h"},
		{"./libc/common.h", "/./libc/common.h"},
		{"./libc/putchar.h", "/./libc/putchar.h"},
		{"./libc/stdio.h", "/./libc/stdio.h"},
		{"./libc/unistd.h", "/./libc/unistd.h"},
		{"./libc/string.h", "/./libc/string.h"},
		{"./libc/stdarg.h", "/./libc/stdarg.h"},
		{"./libc/stdlib.h", "/./libc/stdlib.h"},
		{"./libc/assert.h", "/./libc/assert.h"},
		{"./libc/filesystem.h", "/./libc/filesystem.h"},
		{"./recc-implementation/lexer.h", "/./recc-implementation/lexer.h"},
		{"./recc-implementation/l0_generator.h", "/./recc-implementation/l0_generator.h"},
		{"./recc-implementation/memory_pool_collection.h", "/./recc-implementation/memory_pool_collection.h"},
		{"./emulators/javascript/index.php", "/./emulators/javascript/index.php"},
		{"./emulators/c/op-cpu.h", "/./emulators/c/op-cpu.h"},
		{"./test/extras/includetest2.h", "/test/extras/includetest2.h"},
		{"./test/extras/includetest1.h", "/test/extras/includetest1.h"},
		{"kernel/l2/putchar_busy.l2", "/kernel/l2/putchar_busy.l2"},
		{"./recc-implementation/preprocessor.h", "/./recc-implementation/preprocessor.h"},
		{"./recc-implementation/code_generator.h", "/./recc-implementation/code_generator.h"},
		{"recc-implementation/code_generator.c", "/recc-implementation/code_generator.c"},
		{"recc-implementation/io.c", "/recc-implementation/io.c"},
		{"recc-implementation/lexer.c", "/recc-implementation/lexer.c"},
		{"recc-implementation/linker.c", "/recc-implementation/linker.c"},
		{"recc-implementation/parser.c", "/recc-implementation/parser.c"},
		{"recc-implementation/l0_generator.c", "/recc-implementation/l0_generator.c"},
		{"recc-implementation/preprocessor.c", "/recc-implementation/preprocessor.c"},
		{"test/c89/basic-operations.c", "/test/c89/basic-operations.c"},
		{"test/c89/basic-putchar-aa.c", "/test/c89/basic-putchar-aa.c"},
		{"test/c89/basic-putchar-a.c", "/test/c89/basic-putchar-a.c"},
		{"test/c89/basic-putchar-b.c", "/test/c89/basic-putchar-b.c"},
		{"test/c89/constants.c", "/test/c89/constants.c"},
		{"test/c89/empty-main-return-zero.c", "/test/c89/empty-main-return-zero.c"},
		{"test/c89/exotic-declarators.c", "/test/c89/exotic-declarators.c"},
		{"test/c89/filesystem.c", "/test/c89/filesystem.c"},
		{"test/c89/for-loop.c", "/test/c89/for-loop.c"},
		{"test/c89/ignore-local.c", "/test/c89/ignore-local.c"},
		{"test/c89/main_with_parameters.c", "/test/c89/main_with_parameters.c"},
		{"test/c89/malloc-test.c", "/test/c89/malloc-test.c"},
		{"test/c89/many-types.c", "/test/c89/many-types.c"},
		{"test/c89/nested-putchar-a.c", "/test/c89/nested-putchar-a.c"},
		{"test/c89/nested-putchar-a-param-ignored.c", "/test/c89/nested-putchar-a-param-ignored.c"},
		{"test/c89/nested-putchar-a-param-used.c", "/test/c89/nested-putchar-a-param-used.c"},
		{"test/c89/preprocessor_test.c", "/test/c89/preprocessor_test.c"},
		{"test/c89/print.c", "/test/c89/print.c"},
		{"test/c89/putchar-return.c", "/test/c89/putchar-return.c"},
		{"test/c89/string-literal.c", "/test/c89/string-literal.c"},
		{"test/c89/typedef.c", "/test/c89/typedef.c"},
		{"test/c89/use-array.c", "/test/c89/use-array.c"},
		{"test/c89/use-local.c", "/test/c89/use-local.c"},
		{"test/c89/use-reference.c", "/test/c89/use-reference.c"},
		{"test/c89/va_list_call.c", "/test/c89/va_list_call.c"},
		{"test/c89/while-loop.c", "/test/c89/while-loop.c"},
		{"libc/assert.c", "/libc/assert.c"},
		{"libc/filesystem.c", "/libc/filesystem.c"},
		{"libc/fopen.c", "/libc/fopen.c"},
		{"libc/getcwd.c", "/libc/getcwd.c"},
		{"libc/Makefile", "/libc/Makefile"},
		{"libc/malloc.c", "/libc/malloc.c"},
		{"libc/printf.c", "/libc/printf.c"},
		{"libc/string.c", "/libc/string.c"},
		{"kernel/filesystem.c", "/kernel/filesystem.c"},
		{"kernel/kernel_impl.c", "/kernel/kernel_impl.c"},
		{"kernel/kernel_state.c", "/kernel/kernel_state.c"},
		{"kernel/main.c", "/kernel/main.c"},
		{"kernel/putchar.c", "/kernel/putchar.c"},
		{"kernel/queue.c", "/kernel/queue.c"},
		{"kernel/user_proc.c", "/kernel/user_proc.c"},
		{"recc-implementation/filesystem_compiler.c", "/recc-implementation/filesystem_compiler.c"},
		{"generated/struct_struct_linker_symbol_ptr_list.h","/generated/struct_struct_linker_symbol_ptr_list.h"},
		{"generated/struct_struct_preprocessor_macro_level_ptr_list.h","/generated/struct_struct_preprocessor_macro_level_ptr_list.h"},
		{"generated/struct_void_ptr_list.h","/generated/struct_void_ptr_list.h"},
		{"generated/struct_struct_constant_description_ptr_list.h","/generated/struct_struct_constant_description_ptr_list.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h","/generated/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair.h"},
		{"generated/struct_unsigned_int_ptr_list.h","/generated/struct_unsigned_int_ptr_list.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_map.h","/generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_map.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_map.h","/generated/struct_unsigned_char_ptr_to_struct_constant_description_ptr_map.h"},
		{"generated/struct_struct_c_lexer_token_ptr_list.h","/generated/struct_struct_c_lexer_token_ptr_list.h"},
		{"generated/struct_struct_namespace_object_ptr_list.h","/generated/struct_struct_namespace_object_ptr_list.h"},
		{"generated/struct_struct_l2_item_ptr_list.h","/generated/struct_struct_l2_item_ptr_list.h"},
		{"generated/struct_struct_c_lexer_state_ptr_list.h","/generated/struct_struct_c_lexer_state_ptr_list.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair.h"},
		{"generated/struct_struct_normalized_declaration_element_ptr_list.h","/generated/struct_struct_normalized_declaration_element_ptr_list.h"},
		{"generated/struct_char_list.h","/generated/struct_char_list.h"},
		{"generated/struct_struct_macro_definition_ptr_list.h","/generated/struct_struct_macro_definition_ptr_list.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h","/generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair.h"},
		{"generated/struct_struct_l2_lexer_state_ptr_list.h","/generated/struct_struct_l2_lexer_state_ptr_list.h"},
		{"generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_map.h","/generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_map.h"},
		{"generated/struct_char_ptr_list.h","/generated/struct_char_ptr_list.h"},
		{"generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair.h"},
		{"generated/struct_struct_normalized_declarator_ptr_list.h","/generated/struct_struct_normalized_declarator_ptr_list.h"},
		{"generated/struct_struct_struct_c_lexer_token_ptr_list_ptr_list.h","/generated/struct_struct_struct_c_lexer_token_ptr_list_ptr_list.h"},
		{"generated/struct_struct_preprocessor_if_branch_ptr_list.h","/generated/struct_struct_preprocessor_if_branch_ptr_list.h"},
		{"generated/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.h","/generated/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_struct_entity_ptr_key_value_pair.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_linker_file_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_struct_linker_file_ptr_key_value_pair.h"},
		{"generated/struct_struct_scope_level_ptr_list.h","/generated/struct_struct_scope_level_ptr_list.h"},
		{"generated/struct_void_ptr_to_unsigned_int_key_value_pair.h","/generated/struct_void_ptr_to_unsigned_int_key_value_pair.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_map.h","/generated/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_map.h"},
		{"generated/struct_unsigned_char_list.h","/generated/struct_unsigned_char_list.h"},
		{"generated/struct_struct_unsigned_char_list_ptr_list.h","/generated/struct_struct_unsigned_char_list_ptr_list.h"},
		{"generated/struct_unsigned_char_ptr_list.h","/generated/struct_unsigned_char_ptr_list.h"},
		{"generated/struct_struct_entity_ptr_list.h","/generated/struct_struct_entity_ptr_list.h"},
		{"generated/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair.h","/generated/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair.h"},
		{"generated/struct_struct_special_macro_definition_ptr_list.h","/generated/struct_struct_special_macro_definition_ptr_list.h"},
		{"generated/struct_struct_macro_parameter_ptr_list.h","/generated/struct_struct_macro_parameter_ptr_list.h"},
		{"generated/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair.h","/generated/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_entity_ptr_map.h","/generated/struct_unsigned_char_ptr_to_struct_entity_ptr_map.h"},
		{"generated/struct_unsigned_int_list.h","/generated/struct_unsigned_int_list.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair.h"},
		{"generated/struct_struct_linker_file_ptr_list.h","/generated/struct_struct_linker_file_ptr_list.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_map.h","/generated/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_map.h"},
		{"generated/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_map.h","/generated/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_map.h"},
		{"generated/struct_struct_preprocessor_file_context_ptr_list.h","/generated/struct_struct_preprocessor_file_context_ptr_list.h"},
		{"generated/struct_struct_type_traversal_ptr_list.h","/generated/struct_struct_type_traversal_ptr_list.h"},
		{"generated/struct_void_ptr_to_unsigned_int_map.h","/generated/struct_void_ptr_to_unsigned_int_map.h"},
		{"generated/struct_struct_type_description_reference_list.h","/generated/struct_struct_type_description_reference_list.h"},
		{"generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair.h","/generated/struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair.h"},
		{"generated/struct_struct_constant_initializer_level_ptr_list.h","/generated/struct_struct_constant_initializer_level_ptr_list.h"},
		{"generated/struct_struct_normalized_specifier_ptr_list.h","/generated/struct_struct_normalized_specifier_ptr_list.h"},
		{"generated/struct_struct_switch_frame_ptr_list.h","/generated/struct_struct_switch_frame_ptr_list.h"},
		{"generated/struct_struct_l2_lexer_token_ptr_list.h","/generated/struct_struct_l2_lexer_token_ptr_list.h"},
		{"types/lexer/struct_common_lexer_state.h","/types/lexer/struct_common_lexer_state.h"},
		{"types/lexer/struct_l2_lexer_token.h","/types/lexer/struct_l2_lexer_token.h"},
		{"types/lexer/enum_c_token_type.h","/types/lexer/enum_c_token_type.h"},
		{"types/lexer/enum_l2_token_type.h","/types/lexer/enum_l2_token_type.h"},
		{"types/lexer/struct_c_lexer_token.h","/types/lexer/struct_c_lexer_token.h"},
		{"types/lexer/struct_c_lexer_state.h","/types/lexer/struct_c_lexer_state.h"},
		{"types/lexer/struct_l2_lexer_state.h","/types/lexer/struct_l2_lexer_state.h"},
		{"types/recc-implementation/struct_heap_memory_pool.h","/types/recc-implementation/struct_heap_memory_pool.h"},
		{"types/preprocessor/struct_macro_definition.h","/types/preprocessor/struct_macro_definition.h"},
		{"types/preprocessor/struct_preprocessor_file_context.h","/types/preprocessor/struct_preprocessor_file_context.h"},
		{"types/preprocessor/struct_preprocessor_macro_level.h","/types/preprocessor/struct_preprocessor_macro_level.h"},
		{"types/preprocessor/enum_macro_definition_type.h","/types/preprocessor/enum_macro_definition_type.h"},
		{"types/preprocessor/struct_preprocessor_if_branch.h","/types/preprocessor/struct_preprocessor_if_branch.h"},
		{"types/preprocessor/struct_special_macro_definition.h","/types/preprocessor/struct_special_macro_definition.h"},
		{"types/preprocessor/struct_preprocessor_state.h","/types/preprocessor/struct_preprocessor_state.h"},
		{"types/preprocessor/struct_macro_parameter.h","/types/preprocessor/struct_macro_parameter.h"},
		{"types/preprocessor/enum_special_macro_type.h","/types/preprocessor/enum_special_macro_type.h"},
		{"types/code_generator/struct_switch_frame.h","/types/code_generator/struct_switch_frame.h"},
		{"types/code_generator/struct_compile_time_constant.h","/types/code_generator/struct_compile_time_constant.h"},
		{"types/code_generator/struct_constant_initializer_level.h","/types/code_generator/struct_constant_initializer_level.h"},
		{"types/code_generator/struct_type_traversal.h","/types/code_generator/struct_type_traversal.h"},
		{"types/code_generator/enum_copy_method.h","/types/code_generator/enum_copy_method.h"},
		{"types/code_generator/struct_code_gen_state.h","/types/code_generator/struct_code_gen_state.h"},
		{"types/linker/struct_l2_item.h","/types/linker/struct_l2_item.h"},
		{"types/linker/struct_linker_file.h","/types/linker/struct_linker_file.h"},
		{"types/linker/struct_linker_symbol.h","/types/linker/struct_linker_symbol.h"},
		{"types/recc-implementation/enum_entity_type.h","/types/recc-implementation/enum_entity_type.h"},
		{"types/parser/enum_normalized_declarator_type.h","/types/parser/enum_normalized_declarator_type.h"},
		{"types/parser/enum_add_or_remove.h","/types/parser/enum_add_or_remove.h"},
		{"types/type_engine/struct_scope_level.h","/types/type_engine/struct_scope_level.h"},
		{"types/parser/enum_normalized_specifier_type.h","/types/parser/enum_normalized_specifier_type.h"},
		{"types/parser/enum_object_location.h","/types/parser/enum_object_location.h"},
		{"types/parser/struct_normalized_declaration_element.h","/types/parser/struct_normalized_declaration_element.h"},
		{"types/parser/struct_namespace_object.h","/types/parser/struct_namespace_object.h"},
		{"types/parser/struct_first_and_last_namespace_object.h","/types/parser/struct_first_and_last_namespace_object.h"},
		{"types/parser/struct_constant_description.h","/types/parser/struct_constant_description.h"},
		{"types/parser/struct_current_function_change.h","/types/parser/struct_current_function_change.h"},
		{"types/parser/struct_normalized_declarator.h","/types/parser/struct_normalized_declarator.h"},
		{"types/parser/enum_node_type.h","/types/parser/enum_node_type.h"},
		{"types/parser/struct_type_description.h","/types/parser/struct_type_description.h"},
		{"types/parser/struct_namespace_object_change.h","/types/parser/struct_namespace_object_change.h"},
		{"types/parser/struct_namespace_modification.h","/types/parser/struct_namespace_modification.h"},
		{"types/parser/struct_parser_node.h","/types/parser/struct_parser_node.h"},
		{"types/parser/struct_normalized_declaration_set.h","/types/parser/struct_normalized_declaration_set.h"},
		{"types/parser/enum_declaration_or_definition.h","/types/parser/enum_declaration_or_definition.h"},
		{"types/parser/struct_normalized_specifier.h","/types/parser/struct_normalized_specifier.h"},
		{"types/parser/enum_type_class.h","/types/parser/enum_type_class.h"},
		{"types/parser/enum_value_type.h","/types/parser/enum_value_type.h"},
		{"types/parser/enum_normalized_declaration_type.h","/types/parser/enum_normalized_declaration_type.h"},
		{"types/parser/struct_parser_state.h","/types/parser/struct_parser_state.h"},
		{"types/parser/enum_scope_type.h","/types/parser/enum_scope_type.h"},
		{"types/recc-implementation/struct_memory_pool_collection.h","/types/recc-implementation/struct_memory_pool_collection.h"},
		{"emulators/c/struct_l0_item.h","/emulators/c/struct_l0_item.h"}
	};
	unsigned int i;
	memory_pool_collection_create(&mpc);
	initialize_filesystem_datastructures();
	unsigned_char_ptr_to_unsigned_char_ptr_map_create(&files, struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_compare);
	unsigned_char_ptr_to_unsigned_char_ptr_map_create(&directories, struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_compare);
	unsigned_char_list_create(&tmp);
	for(i = 0; i < NUM_FILES; i++){
		unsigned int j = 0;
		if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&files, (unsigned char *)filesystem_files[i][1])){
			printf("Duplicate file: %s\n", filesystem_files[i][1]);
			assert(0 && "Duplicate file in target filesystem.");
		}else{
			unsigned_char_ptr_to_unsigned_char_ptr_map_put(&files, (unsigned char *)&filesystem_files[i][1][0], (unsigned char *)&filesystem_files[i][0][0]);
		}
		while(filesystem_files[i][1][j]){
			unsigned_char_list_add_end(&tmp, (unsigned char)filesystem_files[i][1][j]);
			if(filesystem_files[i][1][j] == '/'){
				unsigned char * s = copy_string(unsigned_char_list_data(&tmp), ((unsigned char *)unsigned_char_list_data(&tmp)) + j, &mpc);
				if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&directories, s)){
					heap_memory_pool_free(mpc.heap_pool, s);
				}else{
					unsigned_char_ptr_to_unsigned_char_ptr_map_put(&directories, s, s);
				}
			}
			j++;
		}
		unsigned_char_list_destroy(&tmp);
		unsigned_char_list_create(&tmp);
	}

	/*  Remove the root directory because its inode has already been created */
	root_dir = unsigned_char_ptr_to_unsigned_char_ptr_map_get(&directories, (unsigned char *)"/");
	unsigned_char_ptr_to_unsigned_char_ptr_map_remove(&directories, (unsigned char *)"/");
	heap_memory_pool_free(mpc.heap_pool, root_dir);

	directory_keys = unsigned_char_ptr_to_unsigned_char_ptr_map_keys(&directories);

	create_directories(&directory_keys, &mpc);

	for(i = 0; i < unsigned_char_ptr_list_size(&directory_keys); i++){
		unsigned char * dir = unsigned_char_ptr_list_get(&directory_keys, i);
		printf("Unique Directory: %s\n", dir);
		heap_memory_pool_free(mpc.heap_pool, dir);
	}

	create_files(&files, &mpc);

	unsigned_char_list_destroy(&tmp);
	unsigned_char_ptr_list_destroy(&directory_keys);
	unsigned_char_ptr_to_unsigned_char_ptr_map_destroy(&files);
	unsigned_char_ptr_to_unsigned_char_ptr_map_destroy(&directories);
	memory_pool_collection_destroy(&mpc);
	output_filesystem_impl(out_file);  /*  Create l2 file the represents filesystem state */
}
