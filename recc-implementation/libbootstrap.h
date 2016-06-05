#ifndef LIBBOOTSTRAP_H_DEFINED_
#define LIBBOOTSTRAP_H_DEFINED_
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
#include "../recc-implementation/binary_exponential_buffer.h"
#include "../recc-implementation/replace_tool.h"
#include <stdio.h>
#include <stdarg.h>

struct bootstrap_state{
	struct binary_exponential_buffer object_makefile;
	struct binary_exponential_buffer library_dependency;
	struct binary_exponential_buffer file_dependencies;
	struct binary_exponential_buffer * library_data_structures_filename;
	struct binary_exponential_buffer * object_data_structures_filename;
	struct binary_exponential_buffer * file_dependencies_data_structures_filename;
};

enum generated_file_type{
	GENERATED_FILE_TYPE_MAP,
	GENERATED_FILE_TYPE_LIST,
	GENERATED_FILE_TYPE_BINARY_SEARCH,
	GENERATED_FILE_TYPE_MERGE_SORT,
	GENERATED_FILE_TYPE_KEY_VALUE_PAIR,
	GENERATED_FILE_TYPE_MEMORY_POOL
};

enum templated_file_type{
	TEMPLATED_FILE_TYPE_C_SOURCE,
	TEMPLATED_FILE_TYPE_I_FILE,
	TEMPLATED_FILE_TYPE_I_TO_L2,
	TEMPLATED_FILE_TYPE_C_TO_O
};

struct templated_file{
	enum templated_file_type type;
	unsigned int is_header;
	struct binary_exponential_buffer in_file;
	struct binary_exponential_buffer out_file;
	struct binary_exponential_buffer header_files;
	unsigned char * relative_location;
	unsigned char * type_1_literal;
	unsigned char * type_2_literal;
	unsigned char * type_name;
};

struct files_for_type{
	struct binary_exponential_buffer replacement_rules;
	struct binary_exponential_buffer files;
	struct binary_exponential_buffer literal_type_names;
	struct replacement_context * filename_context;
};

int do_bootstrap(void);
int add_file_to_binary_exponential_buffer(struct binary_exponential_buffer *, unsigned char *);
void add_string_to_binary_exponential_buffer(void *, struct binary_exponential_buffer *);
void add_string_to_binary_exponential_buffer_with_null_terminator(void *, struct binary_exponential_buffer *);
void add_buffer_to_binary_exponential_buffer(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
int output_binary_exponential_buffer_to_file(struct binary_exponential_buffer *, unsigned char *);
void add_endif(struct binary_exponential_buffer *);
void add_include_guard(struct binary_exponential_buffer *, unsigned char *);
void add_include(struct binary_exponential_buffer *, unsigned char *, unsigned char *);
void add_filepath_macro(struct binary_exponential_buffer *, unsigned char *);
void add_define_include_guard(struct binary_exponential_buffer *, unsigned char *);
void bootstrap_state_destroy(struct bootstrap_state *);
void bootstrap_state_create(struct bootstrap_state *);
void bootstrap_state_output_makefiles(struct bootstrap_state *, const char *);
void add_build_rules(struct bootstrap_state *, const char *, const char *);
void add_char_ptr_to_list(void *, struct binary_exponential_buffer *);
void build_header_section(struct binary_exponential_buffer *, struct binary_exponential_buffer *, unsigned char *);
void make_build_rules(struct bootstrap_state *, const char *, const char *);
void create_map_files(struct bootstrap_state *, struct binary_exponential_buffer, struct binary_exponential_buffer);
void create_list_files(struct bootstrap_state * b, struct binary_exponential_buffer, struct binary_exponential_buffer);
void create_memory_pool_files(struct bootstrap_state * b, struct binary_exponential_buffer, struct binary_exponential_buffer);
void create_merge_sort_files(struct bootstrap_state * b, struct binary_exponential_buffer, struct binary_exponential_buffer);
void create_binary_search_files(struct bootstrap_state * b, struct binary_exponential_buffer, struct binary_exponential_buffer);
void create_key_value_pair_files(struct bootstrap_state * b, struct binary_exponential_buffer, struct binary_exponential_buffer);
struct files_for_type * make_generated_files(struct bootstrap_state *, struct files_for_type *);
void cleanup_files_for_type(struct files_for_type *);
void make_type_replacement_rules(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void make_generated_file(unsigned char *, unsigned char *, unsigned char *, struct binary_exponential_buffer *, struct binary_exponential_buffer *, unsigned int);

struct files_for_type * make_files_for_type(struct bootstrap_state *, enum generated_file_type, struct binary_exponential_buffer, struct binary_exponential_buffer, struct binary_exponential_buffer);

#endif
