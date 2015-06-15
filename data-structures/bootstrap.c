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
#include "replace_tool.h"
#include <stdio.h>

/*  An executable is created from this file that takes care of creating all of the 
    specific .c and .h files from generic template .c and .h files.  This provides basic
    data structures for types like map, list, or stack that are statically typed.
*/

static struct binary_exponential_buffer object_makefile;
static struct binary_exponential_buffer library_dependency;
static struct binary_exponential_buffer library_buildcommand;
static struct binary_exponential_buffer clean_files;
static struct binary_exponential_buffer file_dependencies;
static struct binary_exponential_buffer gitignores;

int add_file_to_buffer(struct binary_exponential_buffer *, unsigned char *);
void create_map_type(const char *, const char *);
void create_list_type(const char *);
void create_stack_type(const char *);
void add_string_to_buffer(void *, struct binary_exponential_buffer *);
void add_buffer_to_buffer(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void add_string_to_buffer_with_null_terminator(void *, struct binary_exponential_buffer *);
void add_char_ptr_to_list(void *, struct binary_exponential_buffer *);
struct binary_exponential_buffer create_identifier_friendly_type(unsigned char *);
int output_buffer_to_file(struct binary_exponential_buffer *, unsigned char *);
void output_buffer_to_c_file(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void output_buffer_to_h_file(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void add_object_build_rule(struct binary_exponential_buffer *);
void add_library_build_rule(struct binary_exponential_buffer *);
void add_library_dependency_rule(struct binary_exponential_buffer *);
void add_clean_rule(struct binary_exponential_buffer *);
void add_file_dependency_rule(struct binary_exponential_buffer *);
void add_gitignores(struct binary_exponential_buffer *);
struct binary_exponential_buffer * make_file_path(unsigned char *);

static const char * artifacts_location = "data-structures/";

int add_file_to_buffer(struct binary_exponential_buffer * buffer, unsigned char * in_file){
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

int output_buffer_to_file(struct binary_exponential_buffer * buffer, unsigned char * out_file){
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

void output_buffer_to_c_file(struct binary_exponential_buffer * buffer, struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer c_file;
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &c_file);
	add_buffer_to_buffer(filename, &c_file);
	add_string_to_buffer_with_null_terminator((void*)".c", &c_file);
	output_buffer_to_file(buffer, (unsigned char *)binary_exponential_buffer_data(&c_file));
	binary_exponential_buffer_destroy(&c_file);
}

void output_buffer_to_h_file(struct binary_exponential_buffer * buffer, struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer h_file;
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &h_file);
	add_buffer_to_buffer(filename, &h_file);
	add_string_to_buffer_with_null_terminator((void*)".h", &h_file);
	output_buffer_to_file(buffer, (unsigned char *)binary_exponential_buffer_data(&h_file));
	binary_exponential_buffer_destroy(&h_file);
}

void add_object_build_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer h_file;
	struct binary_exponential_buffer c_file;
	struct binary_exponential_buffer o_file;
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&o_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &h_file);
	add_string_to_buffer((unsigned char *)artifacts_location, &c_file);
	add_string_to_buffer((unsigned char *)artifacts_location, &o_file);
	add_buffer_to_buffer(filename, &h_file);
	add_buffer_to_buffer(filename, &c_file);
	add_buffer_to_buffer(filename, &o_file);
	add_string_to_buffer_with_null_terminator((void*)".h",&h_file);
	add_string_to_buffer_with_null_terminator((void*)".c",&c_file);
	add_string_to_buffer_with_null_terminator((void*)".o",&o_file);

	add_string_to_buffer(binary_exponential_buffer_data(&o_file), &object_makefile);
	add_string_to_buffer((void*)": ", &object_makefile);
	add_string_to_buffer(binary_exponential_buffer_data(&h_file), &object_makefile);
	add_string_to_buffer((void*)" ", &object_makefile);
	add_string_to_buffer(binary_exponential_buffer_data(&c_file), &object_makefile);
	add_string_to_buffer((void*)"\n\t@$(HOSTCC) -c ", &object_makefile);
	add_string_to_buffer(binary_exponential_buffer_data(&c_file), &object_makefile);
	add_string_to_buffer((void*)" -o ", &object_makefile);
	add_string_to_buffer(binary_exponential_buffer_data(&o_file), &object_makefile);
	add_string_to_buffer((void*)" $(CUSTOM_FLAGS)\n\n", &object_makefile);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
	binary_exponential_buffer_destroy(&o_file);
}

void add_library_build_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer o_file;
	binary_exponential_buffer_create(&o_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &o_file);
	add_buffer_to_buffer(filename, &o_file);
	add_string_to_buffer_with_null_terminator((void*)".o", &o_file);

	add_string_to_buffer(binary_exponential_buffer_data(&o_file), &library_buildcommand);
	add_string_to_buffer((void*)" ", &library_buildcommand);

	binary_exponential_buffer_destroy(&o_file);
}

void add_library_dependency_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer o_file;
	binary_exponential_buffer_create(&o_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &o_file);
	add_buffer_to_buffer(filename, &o_file);
	add_string_to_buffer_with_null_terminator((void*)".o", &o_file);

	add_string_to_buffer(binary_exponential_buffer_data(&o_file), &library_dependency);
	add_string_to_buffer((void*)" ", &library_dependency);

	binary_exponential_buffer_destroy(&o_file);
}

void add_clean_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer c_file;
	struct binary_exponential_buffer h_file;
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &c_file);
	add_string_to_buffer((unsigned char *)artifacts_location, &h_file);
	add_buffer_to_buffer(filename, &c_file);
	add_buffer_to_buffer(filename, &h_file);
	add_string_to_buffer_with_null_terminator((void*)".c", &c_file);
	add_string_to_buffer_with_null_terminator((void*)".h", &h_file);

	add_string_to_buffer(binary_exponential_buffer_data(&c_file), &clean_files);
	add_string_to_buffer((void*)" ", &clean_files);
	add_string_to_buffer(binary_exponential_buffer_data(&h_file), &clean_files);
	add_string_to_buffer((void*)" ", &clean_files);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
}

void add_file_dependency_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer c_file;
	struct binary_exponential_buffer h_file;
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &c_file);
	add_string_to_buffer((unsigned char *)artifacts_location, &h_file);
	add_buffer_to_buffer(filename, &c_file);
	add_buffer_to_buffer(filename, &h_file);
	add_string_to_buffer_with_null_terminator((void*)".c", &c_file);
	add_string_to_buffer_with_null_terminator((void*)".h", &h_file);

	add_string_to_buffer(binary_exponential_buffer_data(&c_file), &file_dependencies);
	add_string_to_buffer((void*)" ", &file_dependencies);
	add_string_to_buffer(binary_exponential_buffer_data(&h_file), &file_dependencies);
	add_string_to_buffer((void*)" ", &file_dependencies);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
}

void add_gitignores(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer c_file;
	struct binary_exponential_buffer h_file;
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	/* Don't include full artifacts location because .gitignore is relative */
	add_string_to_buffer((unsigned char *)"/", &c_file);
	add_string_to_buffer((unsigned char *)"/", &h_file);
	add_buffer_to_buffer(filename, &c_file);
	add_buffer_to_buffer(filename, &h_file);
	add_string_to_buffer_with_null_terminator((void*)".c", &c_file);
	add_string_to_buffer_with_null_terminator((void*)".h", &h_file);

	add_string_to_buffer(binary_exponential_buffer_data(&c_file), &gitignores);
	add_string_to_buffer((void*)"\n", &gitignores);
	add_string_to_buffer(binary_exponential_buffer_data(&h_file), &gitignores);
	add_string_to_buffer((void*)"\n", &gitignores);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
}

void add_string_to_buffer(void * dest, struct binary_exponential_buffer * b){
	unsigned int i = 0;
	unsigned char * str = (unsigned char *)dest;
	while(*str){
		binary_exponential_buffer_increment(b, 1);
		((unsigned char *)binary_exponential_buffer_data(b))[binary_exponential_buffer_size(b) -1] = *str;
		i++;
		str++;
	}
}

void add_buffer_to_buffer(struct binary_exponential_buffer * src, struct binary_exponential_buffer * dest){
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

void add_string_to_buffer_with_null_terminator(void * dest, struct binary_exponential_buffer * b){
	add_string_to_buffer(dest, b);
	binary_exponential_buffer_increment(b, 1);
	((unsigned char *)binary_exponential_buffer_data(b))[binary_exponential_buffer_size(b) -1] = 0;
}

void add_char_ptr_to_list(void * ptr, struct binary_exponential_buffer * b){
	binary_exponential_buffer_increment(b, 1);
	((unsigned char **)binary_exponential_buffer_data(b))[binary_exponential_buffer_size(b) -1] = ptr;
}

struct binary_exponential_buffer create_identifier_friendly_type(unsigned char * type){
	struct binary_exponential_buffer searches;
	struct binary_exponential_buffer replaces;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;

	binary_exponential_buffer_create(&searches, sizeof(unsigned char *));
	binary_exponential_buffer_create(&replaces, sizeof(unsigned char *));
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_char_ptr_to_list((void*)" ", &searches);
	add_char_ptr_to_list((void*)"*", &searches);

	add_char_ptr_to_list((void*)"_", &replaces);
	add_char_ptr_to_list((void*)"ptr", &replaces);

	add_string_to_buffer_with_null_terminator(type, &in_characters);
	
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	return out_characters;
}

void create_stack_type(const char * t0){
	struct binary_exponential_buffer * stack_h_filename = make_file_path((unsigned char*)"generic.h.stack");
	struct binary_exponential_buffer * stack_c_filename = make_file_path((unsigned char*)"generic.c.stack");
	struct binary_exponential_buffer searches;
	struct binary_exponential_buffer replaces;
	struct binary_exponential_buffer generic_filename;
	struct binary_exponential_buffer specific_filename;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;
	struct binary_exponential_buffer t0_identifier = create_identifier_friendly_type((unsigned char *)t0);

	binary_exponential_buffer_create(&specific_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&generic_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&searches, sizeof(unsigned char *));
	binary_exponential_buffer_create(&replaces, sizeof(unsigned char *));
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_string_to_buffer((void*)"T0_IDENTIFIER_stack", &generic_filename);

	add_char_ptr_to_list((void*)"T0_IDENTIFIER", &searches);
	add_char_ptr_to_list(binary_exponential_buffer_data(&t0_identifier), &replaces);
	/*  Get our specific filename */
	do_string_replacements(&searches, &replaces, &generic_filename, &specific_filename);

	add_char_ptr_to_list((void*)"T0_LITERAL", &searches);
	add_char_ptr_to_list((void*)t0, &replaces);

	add_file_to_buffer(&in_characters, (unsigned char *)binary_exponential_buffer_data(stack_h_filename));
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_h_file(&out_characters, &specific_filename);
	/*  Clear input and output buffers. */
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_file_to_buffer(&in_characters, (unsigned char *)binary_exponential_buffer_data(stack_c_filename));
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_c_file(&out_characters, &specific_filename);

	add_object_build_rule(&specific_filename);
	add_library_build_rule(&specific_filename);
	add_library_dependency_rule(&specific_filename);
	add_clean_rule(&specific_filename);
	add_file_dependency_rule(&specific_filename);
	add_gitignores(&specific_filename);

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_destroy(&t0_identifier);
	binary_exponential_buffer_destroy(&generic_filename);
	binary_exponential_buffer_destroy(&specific_filename);

	binary_exponential_buffer_destroy(stack_h_filename);
	binary_exponential_buffer_destroy(stack_c_filename);
	free(stack_h_filename);
	free(stack_c_filename);
}

void create_list_type(const char * t0){
	struct binary_exponential_buffer * list_h_filename = make_file_path((unsigned char*)"generic.h.list");
	struct binary_exponential_buffer * list_c_filename = make_file_path((unsigned char*)"generic.c.list");
	struct binary_exponential_buffer searches;
	struct binary_exponential_buffer replaces;
	struct binary_exponential_buffer generic_filename;
	struct binary_exponential_buffer specific_filename;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;
	struct binary_exponential_buffer t0_identifier = create_identifier_friendly_type((unsigned char *)t0);

	binary_exponential_buffer_create(&specific_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&generic_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&searches, sizeof(unsigned char *));
	binary_exponential_buffer_create(&replaces, sizeof(unsigned char *));
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_string_to_buffer((void*)"T0_IDENTIFIER_list", &generic_filename);

	add_char_ptr_to_list((void*)"T0_IDENTIFIER", &searches);
	add_char_ptr_to_list(binary_exponential_buffer_data(&t0_identifier), &replaces);
	/*  Get our specific filename */
	do_string_replacements(&searches, &replaces, &generic_filename, &specific_filename);

	add_char_ptr_to_list((void*)"T0_LITERAL", &searches);
	add_char_ptr_to_list((void*)t0, &replaces);

	add_file_to_buffer(&in_characters, (unsigned char *)binary_exponential_buffer_data(list_h_filename));
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_h_file(&out_characters, &specific_filename);
	/*  Clear input and output buffers. */
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_file_to_buffer(&in_characters, (unsigned char *)binary_exponential_buffer_data(list_c_filename));
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_c_file(&out_characters, &specific_filename);

	add_object_build_rule(&specific_filename);
	add_library_build_rule(&specific_filename);
	add_library_dependency_rule(&specific_filename);
	add_clean_rule(&specific_filename);
	add_file_dependency_rule(&specific_filename);
	add_gitignores(&specific_filename);

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_destroy(&t0_identifier);
	binary_exponential_buffer_destroy(&generic_filename);
	binary_exponential_buffer_destroy(&specific_filename);

	binary_exponential_buffer_destroy(list_h_filename);
	binary_exponential_buffer_destroy(list_c_filename);
	free(list_h_filename);
	free(list_c_filename);
}

void create_map_type(const char * t0, const char * t1){
	struct binary_exponential_buffer * map_h_filename = make_file_path((unsigned char*)"generic.h.map");
	struct binary_exponential_buffer * map_c_filename = make_file_path((unsigned char*)"generic.c.map");
	struct binary_exponential_buffer searches;
	struct binary_exponential_buffer replaces;
	struct binary_exponential_buffer generic_filename;
	struct binary_exponential_buffer specific_filename;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;
	struct binary_exponential_buffer t0_identifier = create_identifier_friendly_type((unsigned char *)t0);
	struct binary_exponential_buffer t1_identifier = create_identifier_friendly_type((unsigned char *)t1);

	binary_exponential_buffer_create(&specific_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&generic_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&searches, sizeof(unsigned char *));
	binary_exponential_buffer_create(&replaces, sizeof(unsigned char *));
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_string_to_buffer((void*)"T0_IDENTIFIER_to_T1_IDENTIFIER_map", &generic_filename);

	add_char_ptr_to_list((void*)"T0_IDENTIFIER", &searches);
	add_char_ptr_to_list((void*)"T1_IDENTIFIER", &searches);
	add_char_ptr_to_list(binary_exponential_buffer_data(&t0_identifier), &replaces);
	add_char_ptr_to_list(binary_exponential_buffer_data(&t1_identifier), &replaces);
	/*  Get our specific filename */
	do_string_replacements(&searches, &replaces, &generic_filename, &specific_filename);

	add_char_ptr_to_list((void*)"T0_LITERAL", &searches);
	add_char_ptr_to_list((void*)"T1_LITERAL", &searches);
	add_char_ptr_to_list((void*)t0, &replaces);
	add_char_ptr_to_list((void*)t1, &replaces);

	add_file_to_buffer(&in_characters, (unsigned char *)binary_exponential_buffer_data(map_h_filename));
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_h_file(&out_characters, &specific_filename);
	/*  Clear input and output buffers. */
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_file_to_buffer(&in_characters, (unsigned char *)binary_exponential_buffer_data(map_c_filename));
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_c_file(&out_characters, &specific_filename);

	add_object_build_rule(&specific_filename);
	add_library_dependency_rule(&specific_filename);
	add_library_build_rule(&specific_filename);
	add_clean_rule(&specific_filename);
	add_file_dependency_rule(&specific_filename);
	add_gitignores(&specific_filename);

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_destroy(&t0_identifier);
	binary_exponential_buffer_destroy(&t1_identifier);
	binary_exponential_buffer_destroy(&generic_filename);
	binary_exponential_buffer_destroy(&specific_filename);

	binary_exponential_buffer_destroy(map_h_filename);
	binary_exponential_buffer_destroy(map_c_filename);
	free(map_h_filename);
	free(map_c_filename);
}


struct binary_exponential_buffer * make_file_path(unsigned char * str){
	struct binary_exponential_buffer * b = malloc(sizeof(struct binary_exponential_buffer));
	binary_exponential_buffer_create(b, sizeof(unsigned char));
	add_string_to_buffer((void*)artifacts_location, b);
	add_string_to_buffer_with_null_terminator(str, b);
	return b;
}

int main(void){
	struct binary_exponential_buffer * library_data_structures_filename = make_file_path((unsigned char*)"library-data-structures");
	struct binary_exponential_buffer * object_data_structures_filename = make_file_path((unsigned char*)"object-data-structures");
	struct binary_exponential_buffer * clean_data_structures_filename = make_file_path((unsigned char*)"clean-data-structures");
	struct binary_exponential_buffer * file_dependencies_data_structures_filename = make_file_path((unsigned char*)"file-dependencies-data-structures");
	struct binary_exponential_buffer * gitignores_filename = make_file_path((unsigned char*)".gitignore");
	binary_exponential_buffer_create(&object_makefile, sizeof(unsigned char));
	binary_exponential_buffer_create(&library_dependency, sizeof(unsigned char));
	binary_exponential_buffer_create(&library_buildcommand, sizeof(unsigned char));
	binary_exponential_buffer_create(&clean_files, sizeof(unsigned char));
	binary_exponential_buffer_create(&file_dependencies, sizeof(unsigned char));
	binary_exponential_buffer_create(&gitignores, sizeof(unsigned char));

	add_string_to_buffer((unsigned char *)"/*.o\n", &gitignores);
	add_string_to_buffer((unsigned char *)"/bootstrap-datatypes\n", &gitignores);
	add_string_to_buffer((unsigned char *)"/object-data-structures\n", &gitignores);
	add_string_to_buffer((unsigned char *)"/clean-data-structures\n", &gitignores);
	add_string_to_buffer((unsigned char *)"/file-dependencies-data-structures\n", &gitignores);
	add_string_to_buffer((unsigned char *)"/library-data-structures\n", &gitignores);
	add_string_to_buffer((unsigned char *)"/libgenerated-data-structures.a\n", &gitignores);

	add_string_to_buffer((void*)"clean-generated-files:\n\t@rm -f ", &clean_files);
	add_string_to_buffer((unsigned char *)artifacts_location, &library_dependency);
	add_string_to_buffer((void*)"libgenerated-data-structures.a: ", &library_dependency);

	add_string_to_buffer((void*)"\n\t@ar -rcs ", &library_buildcommand);
	add_string_to_buffer((unsigned char *)artifacts_location, &library_buildcommand);
	add_string_to_buffer((void*)"libgenerated-data-structures.a ", &library_buildcommand);

	/*  To create additional datatypes, you can simply add a line below and the build process should take care of the rest */
	create_map_type("unsigned char *", "struct linker_symbol *");
	create_map_type("unsigned char *", "unsigned char *");
	create_map_type("unsigned char *", "struct constant_description *");
	create_map_type("unsigned char *", "struct macro_parameter *");
	create_map_type("unsigned char *", "struct macro_definition *");
	create_map_type("struct c_lexer_token *", "struct c_lexer_token *");
	create_map_type("struct c_lexer_token *", "unsigned char *");
	create_list_type("char");
	create_list_type("char *");
	create_list_type("unsigned char *");
	create_list_type("unsigned char");
	create_list_type("unsigned int");
	create_list_type("unsigned int *");
	create_list_type("void *");
	create_list_type("struct macro_definition *");
	create_list_type("struct macro_parameter *");
	create_list_type("struct memory_pooler *");
	create_list_type("struct c_lexer_token *");
	create_list_type("struct c_lexer_state *");
	create_list_type("struct struct_c_lexer_token_ptr_list *");
	create_list_type("struct struct_unsigned_char_list *");
	create_list_type("struct asm_lexer_token *");
	create_list_type("struct build_script_lexer_token *");
	create_list_type("struct normalized_specifier *");
	create_list_type("struct normalized_declarator *");
	create_list_type("struct normalized_declaration_element *");
	create_list_type("struct namespace_object *");
	create_list_type("struct scope_level *");
	create_list_type("struct switch_frame *");
	create_list_type("struct unsigned_char_list *");
	create_list_type("struct asm_instruction *");
	create_list_type("struct asm_lexer_state *");
	create_list_type("struct linker_object *");
	create_list_type("struct linker_symbol *");
	create_list_type("struct type_description *");
	create_list_type("struct constant_description *");
	create_list_type("struct constant_initializer_level *");
	create_list_type("struct type_traversal *");
	create_list_type("struct if_branch *");
	create_stack_type("unsigned int");
	create_stack_type("struct parser_operation");

	add_string_to_buffer((void*)": bootstrap-datatypes", &file_dependencies);
	add_string_to_buffer_with_null_terminator((void*)"", &library_buildcommand);
	add_string_to_buffer(binary_exponential_buffer_data(&library_buildcommand), &library_dependency);

	output_buffer_to_file(&library_dependency, (unsigned char *)binary_exponential_buffer_data(library_data_structures_filename));
	output_buffer_to_file(&object_makefile, (unsigned char *)binary_exponential_buffer_data(object_data_structures_filename));
	output_buffer_to_file(&clean_files, (unsigned char *)binary_exponential_buffer_data(clean_data_structures_filename));
	output_buffer_to_file(&file_dependencies, (unsigned char *)binary_exponential_buffer_data(file_dependencies_data_structures_filename));
	output_buffer_to_file(&gitignores, (unsigned char *)binary_exponential_buffer_data(gitignores_filename));

	binary_exponential_buffer_destroy(&object_makefile);
	binary_exponential_buffer_destroy(&library_dependency);
	binary_exponential_buffer_destroy(&library_buildcommand);
	binary_exponential_buffer_destroy(&clean_files);
	binary_exponential_buffer_destroy(&file_dependencies);
	binary_exponential_buffer_destroy(&gitignores);

	binary_exponential_buffer_destroy(library_data_structures_filename);
	binary_exponential_buffer_destroy(object_data_structures_filename);
	binary_exponential_buffer_destroy(clean_data_structures_filename);
	binary_exponential_buffer_destroy(file_dependencies_data_structures_filename);
	binary_exponential_buffer_destroy(gitignores_filename);
	free(library_data_structures_filename);
	free(object_data_structures_filename);
	free(clean_data_structures_filename);
	free(file_dependencies_data_structures_filename);
	free(gitignores_filename);

	return 0;
}
