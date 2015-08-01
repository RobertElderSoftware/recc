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
#include <stdarg.h>

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
void create_specific_type(const char *, const char *, unsigned int, ...);
void add_string_to_buffer(void *, struct binary_exponential_buffer *);
void add_buffer_to_buffer(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void add_string_to_buffer_with_null_terminator(void *, struct binary_exponential_buffer *);
void add_char_ptr_to_list(void *, struct binary_exponential_buffer *);
struct binary_exponential_buffer create_identifier_friendly_type(unsigned char *);
int output_buffer_to_file(struct binary_exponential_buffer *, unsigned char *);
void output_buffer_to_c_file(struct binary_exponential_buffer *, unsigned char *);
void output_buffer_to_h_file(struct binary_exponential_buffer *, unsigned char *);
void add_object_build_rule(unsigned char *);
void add_library_build_rule(unsigned char *);
void add_library_dependency_rule(unsigned char *);
void add_clean_rule(unsigned char *);
void add_file_dependency_rule(unsigned char *);
void add_gitignores(unsigned char *);
struct binary_exponential_buffer * make_file_path(unsigned char *);
unsigned char * deep_copy_string(unsigned char *);

static const char * artifacts_location = "data-structures/";

void add_unsigned_int_to_buffer(unsigned int, struct binary_exponential_buffer *);

void add_unsigned_int_to_buffer(unsigned int unsigned_d, struct binary_exponential_buffer * buffer){
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

void output_buffer_to_c_file(struct binary_exponential_buffer * buffer, unsigned char * filename){
	struct binary_exponential_buffer c_file;
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &c_file);
	add_string_to_buffer(filename, &c_file);
	add_string_to_buffer_with_null_terminator((void*)".c", &c_file);
	output_buffer_to_file(buffer, (unsigned char *)binary_exponential_buffer_data(&c_file));
	binary_exponential_buffer_destroy(&c_file);
}

void output_buffer_to_h_file(struct binary_exponential_buffer * buffer, unsigned char * filename){
	struct binary_exponential_buffer h_file;
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &h_file);
	add_string_to_buffer(filename, &h_file);
	add_string_to_buffer_with_null_terminator((void*)".h", &h_file);
	output_buffer_to_file(buffer, (unsigned char *)binary_exponential_buffer_data(&h_file));
	binary_exponential_buffer_destroy(&h_file);
}

void add_object_build_rule(unsigned char * filename){
	struct binary_exponential_buffer h_file;
	struct binary_exponential_buffer c_file;
	struct binary_exponential_buffer o_file;
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&o_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &h_file);
	add_string_to_buffer((unsigned char *)artifacts_location, &c_file);
	add_string_to_buffer((unsigned char *)artifacts_location, &o_file);
	add_string_to_buffer(filename, &h_file);
	add_string_to_buffer(filename, &c_file);
	add_string_to_buffer(filename, &o_file);
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

void add_library_build_rule(unsigned char * filename){
	struct binary_exponential_buffer o_file;
	binary_exponential_buffer_create(&o_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &o_file);
	add_string_to_buffer(filename, &o_file);
	add_string_to_buffer_with_null_terminator((void*)".o", &o_file);

	add_string_to_buffer(binary_exponential_buffer_data(&o_file), &library_buildcommand);
	add_string_to_buffer((void*)" ", &library_buildcommand);

	binary_exponential_buffer_destroy(&o_file);
}

void add_library_dependency_rule(unsigned char * filename){
	struct binary_exponential_buffer o_file;
	binary_exponential_buffer_create(&o_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &o_file);
	add_string_to_buffer(filename, &o_file);
	add_string_to_buffer_with_null_terminator((void*)".o", &o_file);

	add_string_to_buffer(binary_exponential_buffer_data(&o_file), &library_dependency);
	add_string_to_buffer((void*)" ", &library_dependency);

	binary_exponential_buffer_destroy(&o_file);
}

void add_clean_rule(unsigned char * filename){
	struct binary_exponential_buffer c_file;
	struct binary_exponential_buffer h_file;
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &c_file);
	add_string_to_buffer((unsigned char *)artifacts_location, &h_file);
	add_string_to_buffer(filename, &c_file);
	add_string_to_buffer(filename, &h_file);
	add_string_to_buffer_with_null_terminator((void*)".c", &c_file);
	add_string_to_buffer_with_null_terminator((void*)".h", &h_file);

	add_string_to_buffer(binary_exponential_buffer_data(&c_file), &clean_files);
	add_string_to_buffer((void*)" ", &clean_files);
	add_string_to_buffer(binary_exponential_buffer_data(&h_file), &clean_files);
	add_string_to_buffer((void*)" ", &clean_files);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
}

void add_file_dependency_rule(unsigned char * filename){
	struct binary_exponential_buffer c_file;
	struct binary_exponential_buffer h_file;
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	add_string_to_buffer((unsigned char *)artifacts_location, &c_file);
	add_string_to_buffer((unsigned char *)artifacts_location, &h_file);
	add_string_to_buffer(filename, &c_file);
	add_string_to_buffer(filename, &h_file);
	add_string_to_buffer_with_null_terminator((void*)".c", &c_file);
	add_string_to_buffer_with_null_terminator((void*)".h", &h_file);

	add_string_to_buffer(binary_exponential_buffer_data(&c_file), &file_dependencies);
	add_string_to_buffer((void*)" ", &file_dependencies);
	add_string_to_buffer(binary_exponential_buffer_data(&h_file), &file_dependencies);
	add_string_to_buffer((void*)" ", &file_dependencies);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
}

void add_gitignores(unsigned char * filename){
	struct binary_exponential_buffer c_file;
	struct binary_exponential_buffer h_file;
	binary_exponential_buffer_create(&c_file, sizeof(unsigned char));
	binary_exponential_buffer_create(&h_file, sizeof(unsigned char));
	/* Don't include full artifacts location because .gitignore is relative */
	add_string_to_buffer((unsigned char *)"/", &c_file);
	add_string_to_buffer((unsigned char *)"/", &h_file);
	add_string_to_buffer(filename, &c_file);
	add_string_to_buffer(filename, &h_file);
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
	rtn = malloc(num_chars*sizeof(unsigned char));
	c = str; 
	while(*c){
		rtn[i] = *c;
		i++;
		c++;
	}
	rtn[i] = '\0';
	return rtn;
}

void create_specific_type(const char * file_postfix, const char * generic_filename_str, unsigned int num_types, ...){
	va_list va;
	struct binary_exponential_buffer list_h_filename;
	struct binary_exponential_buffer list_c_filename;
	struct binary_exponential_buffer searches;
	struct binary_exponential_buffer replaces;
	struct binary_exponential_buffer generic_filename;
	struct binary_exponential_buffer specific_filename;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;
	unsigned int i;
	va_start(va, num_types);

	binary_exponential_buffer_create(&list_h_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&list_c_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&specific_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&generic_filename, sizeof(unsigned char));
	binary_exponential_buffer_create(&searches, sizeof(unsigned char *));
	binary_exponential_buffer_create(&replaces, sizeof(unsigned char *));
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_string_to_buffer((void*)artifacts_location, &list_h_filename);
	add_string_to_buffer((void*)artifacts_location, &list_c_filename);
	add_string_to_buffer((void*)"generic.h.", &list_h_filename);
	add_string_to_buffer((void*)"generic.c.", &list_c_filename);
	add_string_to_buffer_with_null_terminator((void*)file_postfix, &list_h_filename);
	add_string_to_buffer_with_null_terminator((void*)file_postfix, &list_c_filename);

	add_string_to_buffer((void*)generic_filename_str, &generic_filename);
	add_string_to_buffer_with_null_terminator((void*)file_postfix, &generic_filename);

	for(i = 0; i < num_types; i++){
		unsigned char * current_type = va_arg(va, unsigned char *);
		struct binary_exponential_buffer type_identifier = create_identifier_friendly_type(current_type);
		struct binary_exponential_buffer identifier_tag;
		struct binary_exponential_buffer literal_tag;
		binary_exponential_buffer_create(&identifier_tag, sizeof(unsigned char));
		binary_exponential_buffer_create(&literal_tag, sizeof(unsigned char));
		add_string_to_buffer((void*)"T", &identifier_tag);
		add_unsigned_int_to_buffer(i, &identifier_tag);
		add_string_to_buffer_with_null_terminator((void*)"_IDENTIFIER", &identifier_tag);
		add_char_ptr_to_list(deep_copy_string(binary_exponential_buffer_data(&identifier_tag)), &searches);
		add_char_ptr_to_list(deep_copy_string(binary_exponential_buffer_data(&type_identifier)), &replaces);

		add_string_to_buffer((void*)"T", &literal_tag);
		add_unsigned_int_to_buffer(i, &literal_tag);
		add_string_to_buffer_with_null_terminator((void*)"_LITERAL", &literal_tag);
		add_char_ptr_to_list(deep_copy_string(binary_exponential_buffer_data(&literal_tag)), &searches);
		add_char_ptr_to_list(deep_copy_string(current_type), &replaces);
		binary_exponential_buffer_destroy(&identifier_tag);
		binary_exponential_buffer_destroy(&literal_tag);
		binary_exponential_buffer_destroy(&type_identifier);
	}

	/*  Get our specific filename */
	do_string_replacements(&searches, &replaces, &generic_filename, &specific_filename);

	add_file_to_buffer(&in_characters, (unsigned char *)binary_exponential_buffer_data(&list_h_filename));
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_h_file(&out_characters, binary_exponential_buffer_data(&specific_filename));
	/*  Clear input and output buffers. */
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_file_to_buffer(&in_characters, (unsigned char *)binary_exponential_buffer_data(&list_c_filename));
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_c_file(&out_characters, binary_exponential_buffer_data(&specific_filename));

	add_object_build_rule(binary_exponential_buffer_data(&specific_filename));
	add_library_build_rule(binary_exponential_buffer_data(&specific_filename));
	add_library_dependency_rule(binary_exponential_buffer_data(&specific_filename));
	add_clean_rule(binary_exponential_buffer_data(&specific_filename));
	add_file_dependency_rule(binary_exponential_buffer_data(&specific_filename));
	add_gitignores(binary_exponential_buffer_data(&specific_filename));

	for(i = 0; i < binary_exponential_buffer_size(&searches); i++){
		free(((unsigned char **)binary_exponential_buffer_data(&searches))[i]);
	}
	for(i = 0; i < binary_exponential_buffer_size(&replaces); i++){
		free(((unsigned char **)binary_exponential_buffer_data(&replaces))[i]);
	}

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_destroy(&generic_filename);
	binary_exponential_buffer_destroy(&specific_filename);

	binary_exponential_buffer_destroy(&list_h_filename);
	binary_exponential_buffer_destroy(&list_c_filename);
	va_end(va);
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
	/*  (except for map types, you will need to do more steps to make dependencies work ) */
	/*  The convention is that each type will have a file located in the 'data-structures directory' named  */
	/*  generic.c.<something> and generic.h.<something>.  These files are then string replaced to become files named */
	/*  with specific type information based on their types. */
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "void *", "unsigned int");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "unsigned char *", "struct linker_symbol *");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "unsigned char *", "unsigned char *");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "unsigned char *", "struct constant_description *");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "unsigned char *", "struct macro_parameter *");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "unsigned char *", "struct macro_definition *");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "unsigned char *", "struct special_macro_definition *");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "unsigned char *", "struct namespace_object *");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "struct c_lexer_token *", "struct c_lexer_token *");
	create_specific_type("map", "T0_IDENTIFIER_to_T1_IDENTIFIER_", 2, "struct c_lexer_token *", "unsigned char *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "char");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "char *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "unsigned char *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "unsigned char");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "unsigned int");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "unsigned int *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "void *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct special_macro_definition *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct macro_definition *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct macro_parameter *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct memory_pool *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct c_lexer_token *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct c_lexer_state *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct struct_c_lexer_token_ptr_list *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct struct_unsigned_char_list *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct asm_lexer_token *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct build_script_lexer_token *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct normalized_specifier *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct normalized_declarator *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct normalized_declaration_element *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct namespace_object *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct scope_level *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct switch_frame *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct unsigned_char_list *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct asm_instruction *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct asm_lexer_state *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct linker_object *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct linker_symbol *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct type_description *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct constant_description *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct constant_initializer_level *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct type_traversal *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct if_branch *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct preprocessor_file_context *");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct preloader_instruction");
	create_specific_type("list", "T0_IDENTIFIER_", 1, "struct heap_ptr_index_pair");
	create_specific_type("stack", "T0_IDENTIFIER_", 1, "unsigned int");
	create_specific_type("stack", "T0_IDENTIFIER_", 1, "struct parser_operation");

	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct heap_ptr_index_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct linker_object *");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "unsigned int");

	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct void_ptr_to_unsigned_int_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair");
	create_specific_type("merge_sort", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair");

	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct void_ptr_to_unsigned_int_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_unsigned_char_ptr_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_macro_parameter_ptr_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_macro_definition_ptr_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_special_macro_definition_ptr_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct struct_c_lexer_token_ptr_to_struct_c_lexer_token_ptr_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct struct_c_lexer_token_ptr_to_unsigned_char_ptr_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_constant_description_ptr_key_value_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct unsigned_char_ptr_to_struct_linker_symbol_ptr_key_value_pair");

	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "struct heap_ptr_index_pair");
	create_specific_type("binary_search", "T0_IDENTIFIER_", 1, "unsigned int");

	create_specific_type("memory_pool", "T0_IDENTIFIER_", 1, "struct c_lexer_token");
	create_specific_type("memory_pool", "T0_IDENTIFIER_", 1, "struct parser_node");
	create_specific_type("memory_pool", "T0_IDENTIFIER_", 1, "struct build_script_lexer_token");
	create_specific_type("memory_pool", "T0_IDENTIFIER_", 1, "struct asm_lexer_token");
	create_specific_type("memory_pool", "T0_IDENTIFIER_", 1, "struct type_description");
	create_specific_type("memory_pool", "T0_IDENTIFIER_", 1, "struct asm_instruction");
	create_specific_type("memory_pool", "T0_IDENTIFIER_", 1, "struct linker_symbol");
	create_specific_type("memory_pool", "T0_IDENTIFIER_", 1, "void *");

	/*  Comparison functions for maps */
	add_object_build_rule((unsigned char *)"void_ptr_compare");
	add_library_build_rule((unsigned char *)"void_ptr_compare");
	add_library_dependency_rule((unsigned char *)"void_ptr_compare");
	add_file_dependency_rule((unsigned char *)"void_ptr_compare");

	add_object_build_rule((unsigned char *)"unsigned_char_ptr_compare");
	add_library_build_rule((unsigned char *)"unsigned_char_ptr_compare");
	add_library_dependency_rule((unsigned char *)"unsigned_char_ptr_compare");
	add_file_dependency_rule((unsigned char *)"unsigned_char_ptr_compare");

	add_object_build_rule((unsigned char *)"struct_c_lexer_token_ptr_compare");
	add_library_build_rule((unsigned char *)"struct_c_lexer_token_ptr_compare");
	add_library_dependency_rule((unsigned char *)"struct_c_lexer_token_ptr_compare");
	add_file_dependency_rule((unsigned char *)"struct_c_lexer_token_ptr_compare");

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
