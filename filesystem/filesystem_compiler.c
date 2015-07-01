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

unsigned char printable_character(unsigned char c);
unsigned char printable_character(unsigned char c){
	if(c > 31 && c < 125){
		return c;
	}else{
		return ' ';
	}
}

void output_filesystem_impl(void){
	FILE *f = NULL;
	const char * out_file = "filesystem/filesystem_impl.l2";
	unsigned int i;
	if(!(f = fopen(out_file, "w"))){
		printf("Failed to open file %s for write.\n", out_file);
		return;
	}

	fprintf(f, "OFFSET RELOCATABLE;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_free_blocks_stack_top;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_free_inodes_stack_top;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_blocks;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_inodes;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_free_blocks;\n");
	fprintf(f, "IMPLEMENTS, REQUIRES EXTERNAL globalvar_free_inodes;\n");
	fprintf(f, "globalvar_free_blocks_stack_top:\ndw 0x%X\n", free_blocks_stack_top);
	fprintf(f, "globalvar_free_inodes_stack_top:\ndw 0x%X\n", free_inodes_stack_top);
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
			fprintf(f, "dw 0x%X;  Block %d entry header\n", block_header_int_ptr[j], i);
		}
		for(j = 0; j < num_ints_data; j++){
			fprintf(f, "dw 0x%X;  %c%c%c%c\n", block_data_int_ptr[j], printable_character(((unsigned char *)&block_data_int_ptr[j])[3]), printable_character(((unsigned char *)&block_data_int_ptr[j])[2]),printable_character(((unsigned char *)&block_data_int_ptr[j])[1]),printable_character(((unsigned char *)&block_data_int_ptr[j])[0]));
		}
		fprintf(f, "sw 0x%X; Unused region in block %d\n", DATA_WORDS_PER_BLOCK - num_ints_data, i);
	}
	fprintf(f, "sw 0x%X;  Space for remaining (%d) unused blocks\n", (unsigned int)((NUM_BLOCKS - free_blocks_stack_top) * sizeof(struct filesystem_block)), NUM_BLOCKS - free_blocks_stack_top);
	fprintf(f, "globalvar_inodes:\n");
	for(i = 0; i < free_inodes_stack_top; i++){
		unsigned int j;
		unsigned int * inode_int_ptr = (unsigned int*)&inodes[i];
		unsigned int num_ints_inode = sizeof(struct filesystem_inode) / sizeof(unsigned int);
		assert(sizeof(struct filesystem_inode) % sizeof(unsigned int) == 0);
		for(j = 0; j < num_ints_inode; j++){
			fprintf(f, "dw 0x%X;  Inode %d\n", inode_int_ptr[j], i);
		}
	}
	fprintf(f, "sw 0x%X;  Space for remaining (%d) unused inodes\n", (unsigned int)((NUM_INODES - free_inodes_stack_top) * sizeof(struct filesystem_inode)), NUM_INODES - free_inodes_stack_top);
	fprintf(f, "globalvar_free_blocks:\n");
	for(i = 0; i < NUM_BLOCKS; i++){
		fprintf(f, "dw 0x%X;\n", free_blocks[i]);
	}
	fprintf(f, "globalvar_free_inodes:\n");
	for(i = 0; i < NUM_INODES; i++){
		fprintf(f, "dw 0x%X;\n", free_inodes[i]);
	}

	fclose(f);
	return;
}

void load_file_given_new_inode(unsigned int inode, FILE * source_file){
	char c;
	unsigned int source_file_size = (unsigned int)ftell(source_file);
	unsigned int current_block;
	unsigned int position_in_block = 0;
	assert(!inodes[inode].first_block_initialized);
	fseek(source_file, 0L, SEEK_SET);
	set_file_size_given_inode(inode, source_file_size);
	current_block = inodes[inode].first_block;

	/*  Load our file into the new filesystem */
	while (!feof(source_file)) {
		c = (char)getc(source_file);
		if(c == EOF)
			break;
		((char *)&blocks[current_block].data[0])[position_in_block] = c;
		if(position_in_block == (DATA_WORDS_PER_BLOCK * sizeof(unsigned int)) -1){
			position_in_block = 0;
			assert(blocks[current_block].header.next_block_initialized);
			current_block = blocks[current_block].header.next_block;
		}else{
			position_in_block++;
		}
	}
}

void load_file(unsigned char * d, FILE * f){
	struct unsigned_char_ptr_list components;
	unsigned int parent_directory_inode;
	unsigned int new_file_inode;
	unsigned char * file_name;
	unsigned int i;
	unsigned_char_ptr_list_create(&components);
	resolve_path_components(d, &components);
	/*  Last path component is file to create */
	file_name = unsigned_char_ptr_list_get(&components, unsigned_char_ptr_list_size(&components) -1);
	unsigned_char_ptr_list_remove_all(&components, file_name);
	/*  Last component is new file to create */
	parent_directory_inode = get_directory_inode_from_path_parts(&components);
	printf("Creating file %s with full path %s\n", file_name, d);
	new_file_inode = create_file_given_parent_inode(parent_directory_inode, file_name, 0);
	load_file_given_new_inode(new_file_inode, f);
	for(i = 0; i < unsigned_char_ptr_list_size(&components);i++){
		free(unsigned_char_ptr_list_get(&components, i));
	}
	unsigned_char_ptr_list_destroy(&components);
	free(file_name);
}



void create_directories(struct unsigned_char_ptr_list * directories){
	unsigned int i;
	for(i = 0; i < unsigned_char_ptr_list_size(directories); i++){
		unsigned char * dir = copy_null_terminated_string(unsigned_char_ptr_list_get(directories, i));
		unsigned char * null_byte = get_null_terminator(dir);
		assert(*(null_byte -1) == '/' && "Directory should end with '/'.");
		create_directory(dir);
		free(dir);
	}
}

void create_files(struct unsigned_char_ptr_to_unsigned_char_ptr_map * files){
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
		load_file(destination_file, f);
		fclose(f);
	}
	unsigned_char_ptr_list_destroy(&file_keys);
}

#define NUM_FILES 145

int main(int argc, char ** argv){
	unsigned char * root_dir;
	struct unsigned_char_ptr_to_unsigned_char_ptr_map files;
	struct unsigned_char_ptr_to_unsigned_char_ptr_map directories;
	struct unsigned_char_ptr_list directory_keys;
	struct unsigned_char_list tmp;

	const char * filesystem_files[NUM_FILES][2] = {
		{"code_generator.c", "/code_generator.c"},
		{"code_generator.h", "/code_generator.h"},
		{"core_data_types.h", "/core_data_types.h"},
		{"io.c", "/io.c"},
		{"io.h", "/io.h"},
		{"lexer.c", "/lexer.c"},
		{"lexer.h", "/lexer.h"},
		{"linker.c", "/linker.c"},
		{"linker.h", "/linker.h"},
		{"parser.c", "/parser.c"},
		{"parser.h", "/parser.h"},
		{"preloader.c", "/preloader.c"},
		{"preloader.h", "/preloader.h"},
		{"preprocessor.c", "/preprocessor.c"},
		{"preprocessor.h", "/preprocessor.h"},
		{"recc.c", "/recc.c"},
		{"data-structures/binary_exponential_buffer.h", "/data-structures/binary_exponential_buffer.h"},
		{"data-structures/char_list.h", "/data-structures/char_list.h"},
		{"data-structures/char_ptr_list.h", "/data-structures/char_ptr_list.h"},
		{"data-structures/memory_pooler.h", "/data-structures/memory_pooler.h"},
		{"data-structures/replace_tool.h", "/data-structures/replace_tool.h"},
		{"data-structures/struct_asm_instruction_ptr_list.h", "/data-structures/struct_asm_instruction_ptr_list.h"},
		{"data-structures/struct_asm_lexer_state_ptr_list.h", "/data-structures/struct_asm_lexer_state_ptr_list.h"},
		{"data-structures/struct_asm_lexer_token_ptr_list.h", "/data-structures/struct_asm_lexer_token_ptr_list.h"},
		{"data-structures/struct_build_script_lexer_token_ptr_list.h", "/data-structures/struct_build_script_lexer_token_ptr_list.h"},
		{"data-structures/struct_c_lexer_state_ptr_list.h", "/data-structures/struct_c_lexer_state_ptr_list.h"},
		{"data-structures/struct_c_lexer_token_ptr_compare.h", "/data-structures/struct_c_lexer_token_ptr_compare.h"},
		{"data-structures/struct_c_lexer_token_ptr_list.h", "/data-structures/struct_c_lexer_token_ptr_list.h"},
		{"data-structures/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.h", "/data-structures/struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_map.h"},
		{"data-structures/struct_c_lexer_token_ptr_to_unsigned_char_ptr_map.h", "/data-structures/struct_c_lexer_token_ptr_to_unsigned_char_ptr_map.h"},
		{"data-structures/struct_constant_description_ptr_list.h", "/data-structures/struct_constant_description_ptr_list.h"},
		{"data-structures/struct_constant_initializer_level_ptr_list.h", "/data-structures/struct_constant_initializer_level_ptr_list.h"},
		{"data-structures/struct_if_branch_ptr_list.h", "/data-structures/struct_if_branch_ptr_list.h"},
		{"data-structures/struct_linker_object_ptr_list.h", "/data-structures/struct_linker_object_ptr_list.h"},
		{"data-structures/struct_linker_object_ptr_merge_sort.h", "/data-structures/struct_linker_object_ptr_merge_sort.h"},
		{"data-structures/struct_linker_symbol_ptr_list.h", "/data-structures/struct_linker_symbol_ptr_list.h"},
		{"data-structures/struct_macro_definition_ptr_list.h", "/data-structures/struct_macro_definition_ptr_list.h"},
		{"data-structures/struct_macro_parameter_ptr_list.h", "/data-structures/struct_macro_parameter_ptr_list.h"},
		{"data-structures/struct_memory_pooler_ptr_list.h", "/data-structures/struct_memory_pooler_ptr_list.h"},
		{"data-structures/struct_namespace_object_ptr_list.h", "/data-structures/struct_namespace_object_ptr_list.h"},
		{"data-structures/struct_normalized_declaration_element_ptr_list.h", "/data-structures/struct_normalized_declaration_element_ptr_list.h"},
		{"data-structures/struct_normalized_declarator_ptr_list.h", "/data-structures/struct_normalized_declarator_ptr_list.h"},
		{"data-structures/struct_normalized_specifier_ptr_list.h", "/data-structures/struct_normalized_specifier_ptr_list.h"},
		{"data-structures/struct_parser_operation_stack.h", "/data-structures/struct_parser_operation_stack.h"},
		{"data-structures/struct_preprocessor_file_context_ptr_list.h", "/data-structures/struct_preprocessor_file_context_ptr_list.h"},
		{"data-structures/struct_scope_level_ptr_list.h", "/data-structures/struct_scope_level_ptr_list.h"},
		{"data-structures/struct_special_macro_definition_ptr_list.h", "/data-structures/struct_special_macro_definition_ptr_list.h"},
		{"data-structures/struct_struct_c_lexer_token_ptr_list_ptr_list.h", "/data-structures/struct_struct_c_lexer_token_ptr_list_ptr_list.h"},
		{"data-structures/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_ptr_binary_search.h", "/data-structures/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_ptr_binary_search.h"},
		{"data-structures/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_ptr_merge_sort.h", "/data-structures/struct_struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair_ptr_merge_sort.h"},
		{"data-structures/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair_ptr_binary_search.h", "/data-structures/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair_ptr_binary_search.h"},
		{"data-structures/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair_ptr_merge_sort.h", "/data-structures/struct_struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair_ptr_merge_sort.h"},
		{"data-structures/struct_struct_unsigned_char_list_ptr_list.h", "/data-structures/struct_struct_unsigned_char_list_ptr_list.h"},
		{"data-structures/struct_switch_frame_ptr_list.h", "/data-structures/struct_switch_frame_ptr_list.h"},
		{"data-structures/struct_type_description_ptr_list.h", "/data-structures/struct_type_description_ptr_list.h"},
		{"data-structures/struct_type_traversal_ptr_list.h", "/data-structures/struct_type_traversal_ptr_list.h"},
		{"data-structures/struct_unsigned_char_list_ptr_list.h", "/data-structures/struct_unsigned_char_list_ptr_list.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_ptr_binary_search.h", "/data-structures/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_ptr_binary_search.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_ptr_merge_sort.h", "/data-structures/struct_unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair_ptr_merge_sort.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_ptr_binary_search.h", "/data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_ptr_binary_search.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_ptr_merge_sort.h", "/data-structures/struct_unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair_ptr_merge_sort.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_ptr_binary_search.h", "/data-structures/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_ptr_binary_search.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_ptr_merge_sort.h", "/data-structures/struct_unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair_ptr_merge_sort.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_ptr_binary_search.h", "/data-structures/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_ptr_binary_search.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_ptr_merge_sort.h", "/data-structures/struct_unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair_ptr_merge_sort.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_ptr_binary_search.h", "/data-structures/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_ptr_binary_search.h"},
		{"data-structures/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_ptr_merge_sort.h", "/data-structures/struct_unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair_ptr_merge_sort.h"},
		{"data-structures/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_ptr_binary_search.h", "/data-structures/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_ptr_binary_search.h"},
		{"data-structures/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_ptr_merge_sort.h", "/data-structures/struct_unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair_ptr_merge_sort.h"},
		{"data-structures/unsigned_char_list.h", "/data-structures/unsigned_char_list.h"},
		{"data-structures/unsigned_char_ptr_compare.h", "/data-structures/unsigned_char_ptr_compare.h"},
		{"data-structures/unsigned_char_ptr_list.h", "/data-structures/unsigned_char_ptr_list.h"},
		{"data-structures/unsigned_char_ptr_to_struct_constant_description_ptr_map.h", "/data-structures/unsigned_char_ptr_to_struct_constant_description_ptr_map.h"},
		{"data-structures/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h", "/data-structures/unsigned_char_ptr_to_struct_linker_symbol_ptr_map.h"},
		{"data-structures/unsigned_char_ptr_to_struct_macro_definition_ptr_map.h", "/data-structures/unsigned_char_ptr_to_struct_macro_definition_ptr_map.h"},
		{"data-structures/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h", "/data-structures/unsigned_char_ptr_to_struct_macro_parameter_ptr_map.h"},
		{"data-structures/unsigned_char_ptr_to_struct_special_macro_definition_ptr_map.h", "/data-structures/unsigned_char_ptr_to_struct_special_macro_definition_ptr_map.h"},
		{"data-structures/unsigned_char_ptr_to_unsigned_char_ptr_map.h", "/data-structures/unsigned_char_ptr_to_unsigned_char_ptr_map.h"},
		{"data-structures/unsigned_int_binary_search.h", "/data-structures/unsigned_int_binary_search.h"},
		{"data-structures/unsigned_int_list.h", "/data-structures/unsigned_int_list.h"},
		{"data-structures/unsigned_int_merge_sort.h", "/data-structures/unsigned_int_merge_sort.h"},
		{"data-structures/unsigned_int_ptr_list.h", "/data-structures/unsigned_int_ptr_list.h"},
		{"data-structures/unsigned_int_stack.h", "/data-structures/unsigned_int_stack.h"},
		{"data-structures/void_ptr_list.h", "/data-structures/void_ptr_list.h"},
		{"test/basic-operations.c", "/test/basic-operations.c"},
		{"test/basic-putchar-aa.c", "/test/basic-putchar-aa.c"},
		{"test/basic-putchar-a.c", "/test/basic-putchar-a.c"},
		{"test/basic-putchar-b.c", "/test/basic-putchar-b.c"},
		{"test/constants.c", "/test/constants.c"},
		{"test/empty-main-return-zero.c", "/test/empty-main-return-zero.c"},
		{"test/exotic-declarators.c", "/test/exotic-declarators.c"},
		{"test/filesystem.c", "/test/filesystem.c"},
		{"test/for-loop.c", "/test/for-loop.c"},
		{"test/ignore-local.c", "/test/ignore-local.c"},
		{"test/main_with_parameters.c", "/test/main_with_parameters.c"},
		{"test/malloc-test.c", "/test/malloc-test.c"},
		{"test/many-types.c", "/test/many-types.c"},
		{"test/nested-putchar-a.c", "/test/nested-putchar-a.c"},
		{"test/nested-putchar-a-param-ignored.c", "/test/nested-putchar-a-param-ignored.c"},
		{"test/nested-putchar-a-param-used.c", "/test/nested-putchar-a-param-used.c"},
		{"test/preprocessor.c", "/test/preprocessor.c"},
		{"test/print.c", "/test/print.c"},
		{"test/putchar-return.c", "/test/putchar-return.c"},
		{"test/string-literal.c", "/test/string-literal.c"},
		{"test/typedef.c", "/test/typedef.c"},
		{"test/use-array.c", "/test/use-array.c"},
		{"test/use-local.c", "/test/use-local.c"},
		{"test/use-reference.c", "/test/use-reference.c"},
		{"test/va_list_call.c", "/test/va_list_call.c"},
		{"test/while-loop.c", "/test/while-loop.c"},
		{"libc/assert.c", "/libc/assert.c"},
		{"libc/assert.h", "/libc/assert.h"},
		{"libc/build-script", "/libc/build-script"},
		{"libc/common.h", "/libc/common.h"},
		{"libc/filesystem.c", "/libc/filesystem.c"},
		{"libc/filesystem.h", "/libc/filesystem.h"},
		{"libc/fopen.c", "/libc/fopen.c"},
		{"libc/getcwd.c", "/libc/getcwd.c"},
		{"libc/Makefile", "/libc/Makefile"},
		{"libc/malloc.c", "/libc/malloc.c"},
		{"libc/printf.c", "/libc/printf.c"},
		{"libc/putchar.h", "/libc/putchar.h"},
		{"libc/l2/putchar.l2", "/libc/l2/putchar.l2"},
		{"libc/size_t.h", "/libc/size_t.h"},
		{"libc/stdarg.h", "/libc/stdarg.h"},
		{"libc/stdio.h", "/libc/stdio.h"},
		{"libc/stdlib.h", "/libc/stdlib.h"},
		{"libc/string.c", "/libc/string.c"},
		{"libc/string.h", "/libc/string.h"},
		{"libc/unistd.h", "/libc/unistd.h"},
		{"kernel/core_data_types.h", "/kernel/core_data_types.h"},
		{"kernel/kernel_state.h", "/kernel/kernel_state.h"},
		{"kernel/private_kernel_interface.h", "/kernel/private_kernel_interface.h"},
		{"kernel/public_kernel_interface.h", "/kernel/public_kernel_interface.h"},
		{"kernel/queue.h", "/kernel/queue.h"},
		{"kernel/user_proc.h", "/kernel/user_proc.h"},
		{"kernel/filesystem.c", "/kernel/filesystem.c"},
		{"kernel/kernel_impl.c", "/kernel/kernel_impl.c"},
		{"kernel/kernel_state.c", "/kernel/kernel_state.c"},
		{"kernel/main.c", "/kernel/main.c"},
		{"kernel/putchar.c", "/kernel/putchar.c"},
		{"kernel/queue.c", "/kernel/queue.c"},
		{"kernel/user_proc.c", "/kernel/user_proc.c"},
		{"filesystem/filesystem_compiler.c", "/filesystem/filesystem_compiler.c"},
		{"filesystem/filesystem_compiler.h", "/filesystem/filesystem_compiler.h"}
	};
	unsigned int i;
	initialize_filesystem_datastructures();
	assert(argc == 1 && "Invoked with the wrong number of arguments.");
	unsigned_char_ptr_to_unsigned_char_ptr_map_create(&files);
	unsigned_char_ptr_to_unsigned_char_ptr_map_create(&directories);
	unsigned_char_list_create(&tmp);
	for(i = 0; i < NUM_FILES; i++){
		unsigned int j = 0;
		if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&files, (unsigned char *)filesystem_files[i][1])){
			assert(0 && "Duplicate file in target filesystem.");
		}else{
			unsigned_char_ptr_to_unsigned_char_ptr_map_put(&files, (unsigned char *)&filesystem_files[i][1][0], (unsigned char *)&filesystem_files[i][0][0]);
		}
		while(filesystem_files[i][1][j]){
			unsigned_char_list_add_end(&tmp, (unsigned char)filesystem_files[i][1][j]);
			if(filesystem_files[i][1][j] == '/'){
				unsigned char * s = copy_string(unsigned_char_list_data(&tmp), ((unsigned char *)unsigned_char_list_data(&tmp)) + j);
				if(unsigned_char_ptr_to_unsigned_char_ptr_map_exists(&directories, s)){
					free(s);
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
	free(root_dir);

	directory_keys = unsigned_char_ptr_to_unsigned_char_ptr_map_keys(&directories);

	create_directories(&directory_keys);

	for(i = 0; i < unsigned_char_ptr_list_size(&directory_keys); i++){
		unsigned char * dir = unsigned_char_ptr_list_get(&directory_keys, i);
		printf("Unique Directory: %s\n", dir);
		free(dir);
	}

	create_files(&files);

	(void)argc;
	(void)argv;
	unsigned_char_list_destroy(&tmp);
	unsigned_char_ptr_list_destroy(&directory_keys);
	unsigned_char_ptr_to_unsigned_char_ptr_map_destroy(&files);
	unsigned_char_ptr_to_unsigned_char_ptr_map_destroy(&directories);
	output_filesystem_impl();  /*  Create l2 file the represents filesystem state */
	return 0;
}
