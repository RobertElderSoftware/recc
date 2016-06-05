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
#include "libbootstrap.h"
#include <stdio.h>
#include <stdarg.h>

/*  An executable is created from this file that takes care of creating all of the 
    specific .c and .h files from generic template .c and .h files.  This provides basic
    data structures for types like map, list that are statically typed.
*/


struct binary_exponential_buffer create_identifier_friendly_type(unsigned char *);
void add_object_build_rule(struct bootstrap_state * state, unsigned char *, unsigned char *);
void add_library_dependency_rule(struct bootstrap_state * state, unsigned char *, unsigned char *);
void add_file_dependency_rule(struct bootstrap_state * state, unsigned char *, unsigned char *);
struct binary_exponential_buffer * make_file_path(unsigned char *);
unsigned char * deep_copy_string(unsigned char *);
const char * get_file_postfix(enum generated_file_type);
const char * get_generic_filename_str(enum generated_file_type);
void add_templated_file(struct files_for_type *, const char *, const char *, const char *, const char *, const char *, const char *, struct binary_exponential_buffer, unsigned int, enum templated_file_type);
void add_generated_file(struct files_for_type *, const char *, const char *, const char *, const char *, enum templated_file_type);

static const char * templates_location = "recc-implementation/templates/";
static const char * generated_location = "generated/";

const char * get_file_postfix(enum generated_file_type type){
	switch(type){
		case GENERATED_FILE_TYPE_MAP:{
			return "map";
		}case GENERATED_FILE_TYPE_LIST:{
			return "list";
		}case GENERATED_FILE_TYPE_BINARY_SEARCH:{
			return "binary_search";
		}case GENERATED_FILE_TYPE_MERGE_SORT:{
			return "merge_sort";
		}case GENERATED_FILE_TYPE_KEY_VALUE_PAIR:{
			return "key_value_pair";
		}case GENERATED_FILE_TYPE_MEMORY_POOL:{
			return "memory_pool";
		}default:{
			assert(0);
			return (const char *)0;
		}
	}
}

const char * get_generic_filename_str(enum generated_file_type type){
	switch(type){
		case GENERATED_FILE_TYPE_MAP:{
			return "T0_IDENTIFIER_to_T1_IDENTIFIER_";
		}case GENERATED_FILE_TYPE_LIST:{
			return "T0_IDENTIFIER_";
		}case GENERATED_FILE_TYPE_BINARY_SEARCH:{
			return "T0_IDENTIFIER_";
		}case GENERATED_FILE_TYPE_MERGE_SORT:{
			return "T0_IDENTIFIER_";
		}case GENERATED_FILE_TYPE_KEY_VALUE_PAIR:{
			return "T0_IDENTIFIER_to_T1_IDENTIFIER_";
		}case GENERATED_FILE_TYPE_MEMORY_POOL:{
			return "T0_IDENTIFIER_";
		}default:{
			assert(0);
			return (const char *)0;
		}
	}
}

void add_templated_file(struct files_for_type * files_for_type, const char * source_file_prefix, const char * outfile_prefix, const char * file_extension, const char * file_postfix, const char * generic_filename_str, const char * relative_location, struct binary_exponential_buffer header_files, unsigned int is_header, enum templated_file_type type){

	/*  Build the filename of the input and output file we want to generate later. */
	unsigned int i;
	unsigned int num_literal_types = binary_exponential_buffer_size(&files_for_type->literal_type_names);
	unsigned char ** types = (unsigned char **)binary_exponential_buffer_data(&files_for_type->literal_type_names);
	struct templated_file * f = (struct templated_file *)malloc(sizeof(struct templated_file));
	struct replacement_context * filename_context = replacement_context_create();
	f->type = type;
	f->is_header = is_header;
	f->header_files = header_files;
	f->relative_location = (unsigned char *)relative_location;
	f->type_name = (unsigned char *)file_postfix;
	f->type_1_literal = types[0];
	f->type_2_literal = (num_literal_types > 1) ? types[1] : (unsigned char *)0;
	binary_exponential_buffer_create(&f->in_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&f->out_file, sizeof(unsigned char));

	add_string_to_binary_exponential_buffer((void*)templates_location, &f->in_file);
	add_string_to_binary_exponential_buffer((void*)source_file_prefix, &f->in_file);
	add_string_to_binary_exponential_buffer_with_null_terminator((void*)file_postfix, &f->in_file);

	add_string_to_binary_exponential_buffer((void*)generated_location, &filename_context->in_characters);
	add_string_to_binary_exponential_buffer((void*)outfile_prefix, &filename_context->in_characters);
	add_string_to_binary_exponential_buffer((void*)generic_filename_str, &filename_context->in_characters);
	add_string_to_binary_exponential_buffer((void*)file_postfix, &filename_context->in_characters);
	add_string_to_binary_exponential_buffer_with_null_terminator((void*)file_extension, &filename_context->in_characters);

	for(i = 0; i < binary_exponential_buffer_size(&files_for_type->replacement_rules); i++){
		struct replacement_rule ** d = (struct replacement_rule **)binary_exponential_buffer_data(&files_for_type->replacement_rules);
		replacement_context_associate_replacement(filename_context, d[i]);
	}
	do_string_replacements(filename_context);

	add_buffer_to_binary_exponential_buffer(&filename_context->out_characters, &f->out_file);

	/*  Add the new file generation rule */
	binary_exponential_buffer_increment(&files_for_type->files, 1);
	((struct templated_file **)binary_exponential_buffer_data(&files_for_type->files))[binary_exponential_buffer_size(&files_for_type->files) -1] = f;
	replacement_context_destroy(filename_context);
}


void add_generated_file(struct files_for_type * files_for_type, const char * source_file_extension, const char * outfile_extension, const char * file_postfix, const char * generic_filename_str, enum templated_file_type type){
	unsigned int i;
	unsigned int num_literal_types = binary_exponential_buffer_size(&files_for_type->literal_type_names);
	unsigned char ** types = (unsigned char **)binary_exponential_buffer_data(&files_for_type->literal_type_names);
	struct templated_file * f = (struct templated_file *)malloc(sizeof(struct templated_file));
	struct replacement_context * in_filename_context = replacement_context_create();
	struct replacement_context * out_filename_context = replacement_context_create();
	f->type = type;
	f->is_header = 0;
	f->header_files = make_beb_list(0);
	f->relative_location = (unsigned char *)"";
	f->type_name = (unsigned char *)file_postfix;
	f->type_1_literal = types[0];
	f->type_2_literal = (num_literal_types > 1) ? types[1] : (unsigned char *)0;
	binary_exponential_buffer_create(&f->in_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&f->out_file, sizeof(unsigned char));

	add_string_to_binary_exponential_buffer((void*)generated_location, &in_filename_context->in_characters);
	add_string_to_binary_exponential_buffer((void*)generic_filename_str, &in_filename_context->in_characters);
	add_string_to_binary_exponential_buffer((void*)file_postfix, &in_filename_context->in_characters);
	add_string_to_binary_exponential_buffer_with_null_terminator((void*)source_file_extension, &in_filename_context->in_characters);

	add_string_to_binary_exponential_buffer((void*)generated_location, &out_filename_context->in_characters);
	add_string_to_binary_exponential_buffer((void*)generic_filename_str, &out_filename_context->in_characters);
	add_string_to_binary_exponential_buffer((void*)file_postfix, &out_filename_context->in_characters);
	add_string_to_binary_exponential_buffer_with_null_terminator((void*)outfile_extension, &out_filename_context->in_characters);

	for(i = 0; i < binary_exponential_buffer_size(&files_for_type->replacement_rules); i++){
		struct replacement_rule ** d = (struct replacement_rule **)binary_exponential_buffer_data(&files_for_type->replacement_rules);
		replacement_context_associate_replacement(in_filename_context, d[i]);
		replacement_context_associate_replacement(out_filename_context, d[i]);
	}
	do_string_replacements(in_filename_context);
	do_string_replacements(out_filename_context);

	add_buffer_to_binary_exponential_buffer(&in_filename_context->out_characters, &f->in_file);
	add_buffer_to_binary_exponential_buffer(&out_filename_context->out_characters, &f->out_file);

	/*  Add the new file generation rule */
	binary_exponential_buffer_increment(&files_for_type->files, 1);
	((struct templated_file **)binary_exponential_buffer_data(&files_for_type->files))[binary_exponential_buffer_size(&files_for_type->files) -1] = f;
	replacement_context_destroy(in_filename_context);
	replacement_context_destroy(out_filename_context);
}

struct files_for_type * make_files_for_type(struct bootstrap_state * b, enum generated_file_type type, struct binary_exponential_buffer literal_type_names, struct binary_exponential_buffer type_header_files, struct binary_exponential_buffer algorithm_header_files){
	struct files_for_type * files_for_type = (struct files_for_type *)malloc(sizeof(struct files_for_type));

	const char * generic_filename_str = get_generic_filename_str(type);
	const char * file_postfix = get_file_postfix(type);
	binary_exponential_buffer_create(&files_for_type->files, sizeof(struct templated_file *));
	binary_exponential_buffer_create(&files_for_type->replacement_rules, sizeof(struct replacement_rule *));
	files_for_type->literal_type_names = literal_type_names;

	(void)b;
	make_type_replacement_rules(&files_for_type->replacement_rules, &files_for_type->literal_type_names);

	switch(type){
		case GENERATED_FILE_TYPE_MAP:{
			binary_exponential_buffer_increment(&algorithm_header_files, 1);
			((unsigned char **)binary_exponential_buffer_data(&algorithm_header_files))[binary_exponential_buffer_size(&algorithm_header_files) -1] = (unsigned char *)"recc-implementation/binary_exponential_buffer.h";
			binary_exponential_buffer_increment(&type_header_files, 1);
			((unsigned char **)binary_exponential_buffer_data(&type_header_files))[binary_exponential_buffer_size(&type_header_files) -1] = (unsigned char *)"recc-implementation/binary_exponential_buffer.h";
			add_templated_file(files_for_type, "generic.h.header.", "struct_", ".h", file_postfix, generic_filename_str, "../", type_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_templated_file(files_for_type, "generic.h.", "", ".h", file_postfix, generic_filename_str, "../" , algorithm_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_templated_file(files_for_type, "generic.c.", "", ".c", file_postfix, generic_filename_str, "../", make_beb_list(0), 0, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_generated_file(files_for_type, ".c", ".i", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_FILE);
			add_generated_file(files_for_type, ".i", ".l2", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_TO_L2);
			add_generated_file(files_for_type, ".c", ".o", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_C_TO_O);

			break;
		}case GENERATED_FILE_TYPE_LIST:{
			binary_exponential_buffer_increment(&algorithm_header_files, 1);
			((unsigned char **)binary_exponential_buffer_data(&algorithm_header_files))[binary_exponential_buffer_size(&algorithm_header_files) -1] = (unsigned char *)"recc-implementation/binary_exponential_buffer.h";
			binary_exponential_buffer_increment(&type_header_files, 1);
			((unsigned char **)binary_exponential_buffer_data(&type_header_files))[binary_exponential_buffer_size(&type_header_files) -1] = (unsigned char *)"recc-implementation/binary_exponential_buffer.h";
			add_templated_file(files_for_type, "generic.h.header.", "struct_", ".h", file_postfix, generic_filename_str, "../", type_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_templated_file(files_for_type, "generic.h.", "", ".h", file_postfix, generic_filename_str, "../" , algorithm_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_templated_file(files_for_type, "generic.c.", "", ".c", file_postfix, generic_filename_str, "../", make_beb_list(0), 0, TEMPLATED_FILE_TYPE_C_SOURCE);

			add_generated_file(files_for_type, ".c", ".i", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_FILE);
			add_generated_file(files_for_type, ".i", ".l2", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_TO_L2);
			add_generated_file(files_for_type, ".c", ".o", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_C_TO_O);
			break;
		}case GENERATED_FILE_TYPE_BINARY_SEARCH:{
			add_templated_file(files_for_type, "generic.h.", "", ".h", file_postfix, generic_filename_str, "../" , algorithm_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_templated_file(files_for_type, "generic.c.", "", ".c", file_postfix, generic_filename_str, "../", make_beb_list(0), 0, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_generated_file(files_for_type, ".c", ".i", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_FILE);
			add_generated_file(files_for_type, ".i", ".l2", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_TO_L2);
			add_generated_file(files_for_type, ".c", ".o", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_C_TO_O);

			break;
		}case GENERATED_FILE_TYPE_MERGE_SORT:{
			add_templated_file(files_for_type, "generic.h.", "", ".h", file_postfix, generic_filename_str, "../" , algorithm_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_templated_file(files_for_type, "generic.c.", "", ".c", file_postfix, generic_filename_str, "../", make_beb_list(0), 0, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_generated_file(files_for_type, ".c", ".i", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_FILE);
			add_generated_file(files_for_type, ".i", ".l2", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_TO_L2);
			add_generated_file(files_for_type, ".c", ".o", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_C_TO_O);

			break;
		}case GENERATED_FILE_TYPE_KEY_VALUE_PAIR:{
			add_templated_file(files_for_type, "generic.h.header.", "struct_", ".h", file_postfix, generic_filename_str, "../", type_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			break;
		}case GENERATED_FILE_TYPE_MEMORY_POOL:{
			binary_exponential_buffer_increment(&algorithm_header_files, 1);
			((unsigned char **)binary_exponential_buffer_data(&algorithm_header_files))[binary_exponential_buffer_size(&algorithm_header_files) -1] = (unsigned char *)"recc-implementation/binary_exponential_buffer.h";
			add_templated_file(files_for_type, "generic.h.header.", "struct_", ".h", file_postfix, generic_filename_str, "../", type_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_templated_file(files_for_type, "generic.h.", "", ".h", file_postfix, generic_filename_str, "../" , algorithm_header_files, 1, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_templated_file(files_for_type, "generic.c.", "", ".c", file_postfix, generic_filename_str, "../", make_beb_list(0), 0, TEMPLATED_FILE_TYPE_C_SOURCE);
			add_generated_file(files_for_type, ".c", ".i", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_FILE);
			add_generated_file(files_for_type, ".i", ".l2", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_I_TO_L2);
			add_generated_file(files_for_type, ".c", ".o", file_postfix, generic_filename_str, TEMPLATED_FILE_TYPE_C_TO_O);

			break;
		}default:{
			assert(0);
		}
	}
	return files_for_type;
}

void bootstrap_state_create(struct bootstrap_state * state){
	state->library_data_structures_filename = make_file_path((unsigned char*)"library-data-structures");
	state->object_data_structures_filename = make_file_path((unsigned char*)"object-data-structures");
	state->file_dependencies_data_structures_filename = make_file_path((unsigned char*)"file-dependencies-data-structures");

	binary_exponential_buffer_create(&state->object_makefile, sizeof(unsigned char));
	binary_exponential_buffer_create(&state->library_dependency, sizeof(unsigned char));
	binary_exponential_buffer_create(&state->file_dependencies, sizeof(unsigned char));

	add_string_to_binary_exponential_buffer((void*)"DATA_STRUCTURES_OBJECT_FILES=", &state->library_dependency);
}

void bootstrap_state_destroy(struct bootstrap_state * state){
	binary_exponential_buffer_destroy(state->library_data_structures_filename);
	binary_exponential_buffer_destroy(state->object_data_structures_filename);
	binary_exponential_buffer_destroy(state->file_dependencies_data_structures_filename);

	free(state->library_data_structures_filename);
	free(state->object_data_structures_filename);
	free(state->file_dependencies_data_structures_filename);

	binary_exponential_buffer_destroy(&state->object_makefile);
	binary_exponential_buffer_destroy(&state->library_dependency);
	binary_exponential_buffer_destroy(&state->file_dependencies);
}

void bootstrap_state_output_makefiles(struct bootstrap_state * state, const char * current_exec){
	add_string_to_binary_exponential_buffer((void*)": ", &state->file_dependencies);
	add_string_to_binary_exponential_buffer((void*)current_exec, &state->file_dependencies);


	output_binary_exponential_buffer_to_file(&state->library_dependency, (unsigned char *)binary_exponential_buffer_data(state->library_data_structures_filename));
	output_binary_exponential_buffer_to_file(&state->object_makefile, (unsigned char *)binary_exponential_buffer_data(state->object_data_structures_filename));
	output_binary_exponential_buffer_to_file(&state->file_dependencies, (unsigned char *)binary_exponential_buffer_data(state->file_dependencies_data_structures_filename));
}

void add_unsigned_int_to_binary_exponential_buffer(unsigned int, struct binary_exponential_buffer *);

void add_unsigned_int_to_binary_exponential_buffer(unsigned int unsigned_d, struct binary_exponential_buffer * buffer){
	unsigned int leading_zero = 1;
	unsigned int base = 1000000000;
	unsigned int digit = 0;
	if(unsigned_d == 0){
                binary_exponential_buffer_increment(buffer, 1);
                ((unsigned char *)binary_exponential_buffer_data(buffer))[binary_exponential_buffer_size(buffer) -1] = (unsigned char)'0';
	}
	while(base){
		digit = unsigned_d / base;
		if(digit){
			binary_exponential_buffer_increment(buffer, 1);
			((unsigned char *)binary_exponential_buffer_data(buffer))[binary_exponential_buffer_size(buffer) -1] = (unsigned char)('0' + (int)digit);
			leading_zero = 0;
		}else{
			if(!leading_zero){
				binary_exponential_buffer_increment(buffer, 1);
				((unsigned char *)binary_exponential_buffer_data(buffer))[binary_exponential_buffer_size(buffer) -1] = (unsigned char)('0' + (int)digit);
			}
		}
		unsigned_d = unsigned_d - (base * digit);
		base /= 10;
	}
}

int add_file_to_binary_exponential_buffer(struct binary_exponential_buffer * buffer, unsigned char * in_file){
        FILE *f = NULL;
        int c = 0;
        if(!(f = fopen((char *)in_file, "rb"))){
                printf("Failed to open file %s for read.\n", in_file);
                return 1;
        }

        while (c != EOF) {
                c = getc(f);
                if(c == EOF)
                        break;
                binary_exponential_buffer_increment(buffer, 1);
                ((unsigned char *)binary_exponential_buffer_data(buffer))[binary_exponential_buffer_size(buffer) -1] = (unsigned char)c;
        }
        fclose(f);
        return 0;
}

int output_binary_exponential_buffer_to_file(struct binary_exponential_buffer * buffer, unsigned char * out_file){
	FILE *f = NULL;
	unsigned char * data = (unsigned char *)binary_exponential_buffer_data(buffer);
	unsigned int size = binary_exponential_buffer_size(buffer);
	unsigned int i;
	if(!(f = fopen((char *)out_file, "w"))){
		printf("Failed to open file %s for write.\n", out_file);
		return 1;
	}

	for(i = 0; i < size; i++){
		fputc ( data[i] , f);
	}
	fclose(f);
	return 0;
}

void add_object_build_rule(struct bootstrap_state * state, unsigned char * outfile, unsigned char * infile){
	add_string_to_binary_exponential_buffer(outfile, &state->object_makefile);
	add_string_to_binary_exponential_buffer((void*)": ", &state->object_makefile);
	add_string_to_binary_exponential_buffer(infile, &state->object_makefile);
	add_string_to_binary_exponential_buffer((void*)"\n\t@$(HOSTCC) -c ", &state->object_makefile);
	add_string_to_binary_exponential_buffer(infile, &state->object_makefile);
	add_string_to_binary_exponential_buffer((void*)" -o ", &state->object_makefile);
	add_string_to_binary_exponential_buffer(outfile, &state->object_makefile);
	add_string_to_binary_exponential_buffer((void*)" $(CUSTOM_C89_FLAGS)\n\n", &state->object_makefile);
}

void add_library_dependency_rule(struct bootstrap_state * state, unsigned char * outfile, unsigned char * infile){
	add_string_to_binary_exponential_buffer(outfile, &state->library_dependency);
	add_string_to_binary_exponential_buffer((void*)" ", &state->library_dependency);
	(void)infile;
}

void add_file_dependency_rule(struct bootstrap_state * state, unsigned char * outfile, unsigned char * infile){
	add_string_to_binary_exponential_buffer(infile, &state->file_dependencies);
	add_string_to_binary_exponential_buffer((void*)" ", &state->file_dependencies);
	(void)outfile;
}

void add_string_to_binary_exponential_buffer(void * dest, struct binary_exponential_buffer * b){
	unsigned int i = 0;
	unsigned char * str = (unsigned char *)dest;
	while(*str){
		binary_exponential_buffer_increment(b, 1);
		((unsigned char *)binary_exponential_buffer_data(b))[binary_exponential_buffer_size(b) -1] = *str;
		i++;
		str++;
	}
}

void add_buffer_to_binary_exponential_buffer(struct binary_exponential_buffer * src, struct binary_exponential_buffer * dest){
	unsigned int src_size = binary_exponential_buffer_size(src);
	unsigned int i = 0;
	assert(binary_exponential_buffer_element_size(src) == sizeof(unsigned char) && binary_exponential_buffer_element_size(dest) == sizeof(unsigned char));
	while(i < src_size){
		unsigned char c = ((unsigned char *)binary_exponential_buffer_data(src))[i];
		binary_exponential_buffer_increment(dest, 1);
		((unsigned char *)binary_exponential_buffer_data(dest))[binary_exponential_buffer_size(dest) -1] = c;
		i++;
	}
}

void add_string_to_binary_exponential_buffer_with_null_terminator(void * dest, struct binary_exponential_buffer * b){
	add_string_to_binary_exponential_buffer(dest, b);
	binary_exponential_buffer_increment(b, 1);
	((unsigned char *)binary_exponential_buffer_data(b))[binary_exponential_buffer_size(b) -1] = 0;
}

void add_char_ptr_to_list(void * ptr, struct binary_exponential_buffer * b){
	binary_exponential_buffer_increment(b, 1);
	((unsigned char **)binary_exponential_buffer_data(b))[binary_exponential_buffer_size(b) -1] = (unsigned char *)ptr;
}

struct binary_exponential_buffer create_identifier_friendly_type(unsigned char * type){
	struct binary_exponential_buffer rtn;
	struct replacement_context * r = replacement_context_create();
	struct replacement_rule * space_rule = replacement_rule_create();
	struct replacement_rule * ptr_rule = replacement_rule_create();
	replacement_context_associate_replacement(r, space_rule);
	replacement_context_associate_replacement(r, ptr_rule);

	add_string_to_binary_exponential_buffer_with_null_terminator(type, &r->in_characters);

	add_string_to_binary_exponential_buffer_with_null_terminator((void*)" ", &space_rule->search);
	add_string_to_binary_exponential_buffer_with_null_terminator((void*)"_", &space_rule->replace);
	add_string_to_binary_exponential_buffer_with_null_terminator((void*)"*", &ptr_rule->search);
	add_string_to_binary_exponential_buffer_with_null_terminator((void*)"ptr", &ptr_rule->replace);

	do_string_replacements(r);

	replacement_rule_destroy(space_rule);
	replacement_rule_destroy(ptr_rule);

	rtn = binary_exponential_buffer_copy(&r->out_characters);
	replacement_context_destroy(r);
	return rtn;
}

unsigned char * deep_copy_string(unsigned char * str){
	unsigned int i = 0;
	unsigned int num_chars = 0;
	unsigned char * c = str; 
	unsigned char * rtn;
	while(*c){
		c++;
		num_chars++;
	}
	num_chars++; /*  For null terminator */
	rtn = (unsigned char *)malloc(num_chars*sizeof(unsigned char));
	c = str; 
	while(*c){
		rtn[i] = *c;
		i++;
		c++;
	}
	rtn[i] = '\0';
	return rtn;
}

void build_header_section(struct binary_exponential_buffer * files, struct binary_exponential_buffer * output, unsigned char * relative_location){
	unsigned int i;
	/*  Build the list of extra includes that this file requires */
	add_string_to_binary_exponential_buffer((void*)"\n", output);
	for(i = 0; i < binary_exponential_buffer_size(files); i++){
		unsigned char * dependency = ((unsigned char **)binary_exponential_buffer_data(files))[i];
		printf("It has dependency %s.\n", dependency);
		add_include_guard(output, dependency);
		add_include(output, relative_location, dependency);
		add_endif(output);
	}
	add_string_to_binary_exponential_buffer_with_null_terminator((void*)"", output);
}

void make_type_replacement_rules(struct binary_exponential_buffer * rules, struct binary_exponential_buffer * types){
	unsigned int i;
	unsigned int num_types = binary_exponential_buffer_size(types);
	unsigned char ** t = (unsigned char **)binary_exponential_buffer_data(types);
	for(i = 0; i < num_types; i++){
		unsigned char * type = t[i];
		struct replacement_rule * identifier_rule = replacement_rule_create();
		struct replacement_rule * literal_rule = replacement_rule_create();
		struct binary_exponential_buffer type_identifier = create_identifier_friendly_type(type);
		/*  Replacement rule for identifier friendly version */
		add_string_to_binary_exponential_buffer((void*)"T", &identifier_rule->search);
		add_unsigned_int_to_binary_exponential_buffer(i, &identifier_rule->search);
		add_string_to_binary_exponential_buffer_with_null_terminator((void*)"_IDENTIFIER", &identifier_rule->search);
		add_string_to_binary_exponential_buffer_with_null_terminator((void*)binary_exponential_buffer_data(&type_identifier), &identifier_rule->replace);
		/*  Replacement rule for identifier literal version */
		add_string_to_binary_exponential_buffer((void*)"T", &literal_rule->search);
		add_unsigned_int_to_binary_exponential_buffer(i, &literal_rule->search);
		add_string_to_binary_exponential_buffer_with_null_terminator((void*)"_LITERAL", &literal_rule->search);
		add_string_to_binary_exponential_buffer_with_null_terminator(type, &literal_rule->replace);
		binary_exponential_buffer_destroy(&type_identifier);
		binary_exponential_buffer_increment(rules, 1);
		((struct replacement_rule **)binary_exponential_buffer_data(rules))[binary_exponential_buffer_size(rules) -1] = literal_rule;
		binary_exponential_buffer_increment(rules, 1);
		((struct replacement_rule **)binary_exponential_buffer_data(rules))[binary_exponential_buffer_size(rules) -1] = identifier_rule;
	}
}

void make_generated_file(unsigned char * infile, unsigned char * outfile, unsigned char * relative_header_location, struct binary_exponential_buffer * header_files, struct binary_exponential_buffer * replacement_rules, unsigned int is_header){
	struct replacement_context * r = replacement_context_create();
	struct replacement_rule * header_rule = replacement_rule_create();
	unsigned int i;
	replacement_context_associate_replacement(r, header_rule);

	for(i = 0; i < binary_exponential_buffer_size(replacement_rules); i++){
		struct replacement_rule ** d = (struct replacement_rule **)binary_exponential_buffer_data(replacement_rules);
		replacement_context_associate_replacement(r, d[i]);
	}

	if(is_header){
		add_include_guard(&r->in_characters, outfile);
		add_define_include_guard(&r->in_characters, outfile);
	}
	if(infile){ /*  If there is no source file, assume it's just a generated header file. */
		add_file_to_binary_exponential_buffer(&r->in_characters, infile);
	}else{
		add_string_to_binary_exponential_buffer((unsigned char *)"/*GETS_REPLACED_WITH_INCLUDES*/", &r->in_characters);
	}
	if(is_header){
		add_endif(&r->in_characters);
	}

	build_header_section(header_files, &header_rule->replace, relative_header_location);
	add_string_to_binary_exponential_buffer_with_null_terminator((unsigned char *)"/*GETS_REPLACED_WITH_INCLUDES*/", &header_rule->search);
	do_string_replacements(r);
	output_binary_exponential_buffer_to_file(&r->out_characters, outfile);
	replacement_rule_destroy(header_rule);
	replacement_context_destroy(r);
}

struct files_for_type * make_generated_files(struct bootstrap_state * b, struct files_for_type * files_for_type){
	unsigned int i;
	struct templated_file ** data = (struct templated_file **)binary_exponential_buffer_data(&files_for_type->files);
	unsigned int num_files = binary_exponential_buffer_size(&files_for_type->files);
	(void)b;
	for(i = 0; i < num_files; i++){
		struct templated_file * f = data[i];
		switch(f->type){
			case TEMPLATED_FILE_TYPE_C_SOURCE:{
				struct binary_exponential_buffer file_literal_type_names;
				struct binary_exponential_buffer file_replacement_rules;
				unsigned int j;
				binary_exponential_buffer_create(&file_literal_type_names, sizeof(unsigned char *));
				binary_exponential_buffer_create(&file_replacement_rules, sizeof(struct replacement_rule *));

				binary_exponential_buffer_increment(&file_literal_type_names, 1);
				((unsigned char **)binary_exponential_buffer_data(&file_literal_type_names))[binary_exponential_buffer_size(&file_literal_type_names) -1] = f->type_1_literal;
				if(f->type_2_literal){
					binary_exponential_buffer_increment(&file_literal_type_names, 1);
					((unsigned char **)binary_exponential_buffer_data(&file_literal_type_names))[binary_exponential_buffer_size(&file_literal_type_names) -1] = f->type_2_literal;
				}

				make_type_replacement_rules(&file_replacement_rules, &file_literal_type_names);

				make_generated_file((unsigned char *)binary_exponential_buffer_data(&f->in_file), (unsigned char *)binary_exponential_buffer_data(&f->out_file), f->relative_location, &f->header_files, &file_replacement_rules, f->is_header);


				for(j = 0; j < binary_exponential_buffer_size(&file_replacement_rules); j++){
					struct replacement_rule ** d = (struct replacement_rule **)binary_exponential_buffer_data(&file_replacement_rules);
					replacement_rule_destroy(d[j]);
				}
				binary_exponential_buffer_destroy(&file_literal_type_names);
				binary_exponential_buffer_destroy(&file_replacement_rules);
				break;
			}case TEMPLATED_FILE_TYPE_C_TO_O:{
				make_build_rules(b, (const char *)binary_exponential_buffer_data(&f->out_file), (const char *)binary_exponential_buffer_data(&f->in_file));
				break;
			}default:{
				/*  Do nothing, used for later stages of compillation. */
			}
		}
	}
	return files_for_type;
}

void cleanup_files_for_type(struct files_for_type * files_for_type){
	unsigned int i;
	struct templated_file ** data = (struct templated_file **)binary_exponential_buffer_data(&files_for_type->files);
	unsigned int num_files = binary_exponential_buffer_size(&files_for_type->files);
	for(i = 0; i < num_files; i++){
		struct templated_file * f = data[i];
		binary_exponential_buffer_destroy(&f->in_file);
		binary_exponential_buffer_destroy(&f->out_file);
		binary_exponential_buffer_destroy(&f->header_files);
		free(f);
	}

	binary_exponential_buffer_destroy(&files_for_type->files);
	binary_exponential_buffer_destroy(&files_for_type->literal_type_names);
	for(i = 0; i < binary_exponential_buffer_size(&files_for_type->replacement_rules); i++){
		struct replacement_rule ** d = (struct replacement_rule **)binary_exponential_buffer_data(&files_for_type->replacement_rules);
		replacement_rule_destroy(d[i]);
	}

	binary_exponential_buffer_destroy(&files_for_type->replacement_rules);
	free(files_for_type);
}

void make_build_rules(struct bootstrap_state * state, const char * outfile, const char * infile){
	add_object_build_rule(state, (unsigned char *)outfile, (unsigned char *)infile);
	add_library_dependency_rule(state, (unsigned char *)outfile, (unsigned char *)infile);
	add_file_dependency_rule(state, (unsigned char *)outfile, (unsigned char *)infile);
}

struct binary_exponential_buffer * make_file_path(unsigned char * str){
	struct binary_exponential_buffer * b = (struct binary_exponential_buffer *)malloc(sizeof(struct binary_exponential_buffer));
	binary_exponential_buffer_create(b, sizeof(unsigned char));
	add_string_to_binary_exponential_buffer((void*)"recc-implementation/", b);
	add_string_to_binary_exponential_buffer_with_null_terminator(str, b);
	return b;
}

void add_include_guard(struct binary_exponential_buffer * output, unsigned char * filename){
	/*  Make the include guard based on the file path name. */
	add_string_to_binary_exponential_buffer((void *)"#ifndef ", output);
	add_filepath_macro(output, filename);
	add_string_to_binary_exponential_buffer((void *)"\n", output);
}

void add_define_include_guard(struct binary_exponential_buffer * output, unsigned char * filename){
	/*  Make the include guard based on the file path name. */
	add_string_to_binary_exponential_buffer((void *)"#define ", output);
	add_filepath_macro(output, filename);
	add_string_to_binary_exponential_buffer((void *)"\n", output);
}

void add_include(struct binary_exponential_buffer * output, unsigned char * prefix, unsigned char * filename){
	add_string_to_binary_exponential_buffer((void *)"#include \"", output);
	add_string_to_binary_exponential_buffer((void *)prefix, output);
	add_string_to_binary_exponential_buffer((void *)filename, output);
	add_string_to_binary_exponential_buffer((void *)"\"\n", output);
}

void add_filepath_macro(struct binary_exponential_buffer * output, unsigned char * filename){
	unsigned char * c = filename;
	while(*c) {
		if(*c == '/'){
			add_string_to_binary_exponential_buffer((void *)"_DIR_", output);
		}else if(*c == '.'){
			add_string_to_binary_exponential_buffer((void *)"_DOT_", output);
		}else if(*c == '-'){
			add_string_to_binary_exponential_buffer((void *)"_", output);
		}else{
			unsigned char * bu;
			unsigned int indx;
			indx = binary_exponential_buffer_size(output);
			binary_exponential_buffer_increment(output, 1);
			bu = (unsigned char *)binary_exponential_buffer_data(output);
			bu[indx] = *c;
		}
		c++;
	}
}

void add_endif(struct binary_exponential_buffer * output){
	unsigned char * c = (unsigned char *)"#endif\n";
	add_string_to_binary_exponential_buffer((void *)c, output);
}
