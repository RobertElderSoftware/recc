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


struct binary_exponential_buffer object_makefile;
struct binary_exponential_buffer library_dependency;
struct binary_exponential_buffer library_buildcommand;
struct binary_exponential_buffer clean_files;
struct binary_exponential_buffer file_dependencies;

int add_file_to_buffer(struct binary_exponential_buffer *, unsigned char *);
void create_map_type(char *, char *);
void create_list_type(char *);
void create_stack_type(char *);
void add_string_literal_to_buffer(unsigned char *, struct binary_exponential_buffer *, unsigned int);
struct binary_exponential_buffer create_identifier_friendly_type(unsigned char *);
int output_buffer_to_file(struct binary_exponential_buffer *, unsigned char *);
void output_buffer_to_c_file(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void output_buffer_to_h_file(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void add_object_build_rule(struct binary_exponential_buffer *);
void add_library_build_rule(struct binary_exponential_buffer *);
void add_library_dependency_rule(struct binary_exponential_buffer *);
void add_clean_rule(struct binary_exponential_buffer *);
void add_file_dependency_rule(struct binary_exponential_buffer *);

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
	unsigned char * data = binary_exponential_buffer_data(buffer);
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
	struct binary_exponential_buffer c_file = binary_exponential_buffer_copy(filename);
	add_string_literal_to_buffer((unsigned char *)".c",&c_file, 1);
	output_buffer_to_file(buffer, (unsigned char *)binary_exponential_buffer_data(&c_file));
	binary_exponential_buffer_destroy(&c_file);
}

void output_buffer_to_h_file(struct binary_exponential_buffer * buffer, struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer h_file = binary_exponential_buffer_copy(filename);
	add_string_literal_to_buffer((unsigned char *)".h",&h_file, 1);
	output_buffer_to_file(buffer, (unsigned char *)binary_exponential_buffer_data(&h_file));
	binary_exponential_buffer_destroy(&h_file);
}

void add_object_build_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer h_file = binary_exponential_buffer_copy(filename);
	struct binary_exponential_buffer c_file = binary_exponential_buffer_copy(filename);
	struct binary_exponential_buffer o_file = binary_exponential_buffer_copy(filename);
	add_string_literal_to_buffer((unsigned char *)".h",&h_file, 1);
	add_string_literal_to_buffer((unsigned char *)".c",&c_file, 1);
	add_string_literal_to_buffer((unsigned char *)".o",&o_file, 1);

	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&o_file), &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)": ", &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&h_file), &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)" ", &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&c_file), &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)"\n\t@$(HOSTCC) -c ", &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&c_file), &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)" -o ", &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&o_file), &object_makefile, 0);
	add_string_literal_to_buffer((unsigned char *)" $(CUSTOM_FLAGS)\n\n", &object_makefile, 0);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
	binary_exponential_buffer_destroy(&o_file);
}

void add_library_build_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer o_file = binary_exponential_buffer_copy(filename);
	add_string_literal_to_buffer((unsigned char *)".o",&o_file, 1);

	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&o_file), &library_buildcommand, 0);
	add_string_literal_to_buffer((unsigned char *)" ", &library_buildcommand, 0);

	binary_exponential_buffer_destroy(&o_file);
}

void add_library_dependency_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer o_file = binary_exponential_buffer_copy(filename);
	add_string_literal_to_buffer((unsigned char *)".o",&o_file, 1);

	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&o_file), &library_dependency, 0);
	add_string_literal_to_buffer((unsigned char *)" ", &library_dependency, 0);

	binary_exponential_buffer_destroy(&o_file);
}

void add_clean_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer c_file = binary_exponential_buffer_copy(filename);
	struct binary_exponential_buffer h_file = binary_exponential_buffer_copy(filename);
	add_string_literal_to_buffer((unsigned char *)".c",&c_file, 1);
	add_string_literal_to_buffer((unsigned char *)".h",&h_file, 1);

	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&c_file), &clean_files, 0);
	add_string_literal_to_buffer((unsigned char *)" ", &clean_files, 0);
	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&h_file), &clean_files, 0);
	add_string_literal_to_buffer((unsigned char *)" ", &clean_files, 0);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
}

void add_file_dependency_rule(struct binary_exponential_buffer * filename){
	struct binary_exponential_buffer c_file = binary_exponential_buffer_copy(filename);
	struct binary_exponential_buffer h_file = binary_exponential_buffer_copy(filename);
	add_string_literal_to_buffer((unsigned char *)".c",&c_file, 1);
	add_string_literal_to_buffer((unsigned char *)".h",&h_file, 1);

	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&c_file), &file_dependencies, 0);
	add_string_literal_to_buffer((unsigned char *)" ", &file_dependencies, 0);
	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&h_file), &file_dependencies, 0);
	add_string_literal_to_buffer((unsigned char *)" ", &file_dependencies, 0);

	binary_exponential_buffer_destroy(&h_file);
	binary_exponential_buffer_destroy(&c_file);
}

void add_string_literal_to_buffer(unsigned char * str, struct binary_exponential_buffer * b, unsigned int null_terminate){
	unsigned int i = 0;
	while(*str){
		binary_exponential_buffer_increment(b, 1);
		((unsigned char *)binary_exponential_buffer_data(b))[binary_exponential_buffer_size(b) -1] = *str;
		i++;
		str++;
	}
	if(null_terminate){
		binary_exponential_buffer_increment(b, 1);
		((unsigned char *)binary_exponential_buffer_data(b))[binary_exponential_buffer_size(b) -1] = 0;
	}
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

	binary_exponential_buffer_increment(&searches, 2);
	((unsigned char **)binary_exponential_buffer_data(&searches))[0] = (unsigned char *)" ";
	((unsigned char **)binary_exponential_buffer_data(&searches))[1] = (unsigned char *)"*";

	binary_exponential_buffer_increment(&replaces, 2);
	((unsigned char **)binary_exponential_buffer_data(&replaces))[0] = (unsigned char *)"_";
	((unsigned char **)binary_exponential_buffer_data(&replaces))[1] = (unsigned char *)"ptr";

	add_string_literal_to_buffer(type, &in_characters, 0);
	
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	return out_characters;
}

void create_stack_type(char * t1){
	struct binary_exponential_buffer searches;
	struct binary_exponential_buffer replaces;
	struct binary_exponential_buffer generic_filename;
	struct binary_exponential_buffer specific_filename;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;
	struct binary_exponential_buffer t1_identifier = create_identifier_friendly_type((unsigned char *)t1);

	binary_exponential_buffer_increment(&t1_identifier, 1);
	((unsigned char *)binary_exponential_buffer_data(&t1_identifier))[binary_exponential_buffer_size(&t1_identifier) - 1] = 0;

	binary_exponential_buffer_create(&specific_filename, sizeof(unsigned char *));
	binary_exponential_buffer_create(&generic_filename, sizeof(unsigned char *));
	binary_exponential_buffer_create(&searches, sizeof(unsigned char *));
	binary_exponential_buffer_create(&replaces, sizeof(unsigned char *));
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_string_literal_to_buffer((unsigned char *)"data-structures/GENERIC_TYPE0_IDENTIFIER_FRIENDLY_stack", &generic_filename, 0);

	binary_exponential_buffer_increment(&searches, 1);
	((unsigned char **)binary_exponential_buffer_data(&searches))[0] = (unsigned char *)"GENERIC_TYPE0_IDENTIFIER_FRIENDLY";
	binary_exponential_buffer_increment(&replaces, 1);
	((unsigned char **)binary_exponential_buffer_data(&replaces))[0] = (unsigned char *)binary_exponential_buffer_data(&t1_identifier);
	/*  Get our specific filename */
	do_string_replacements(&searches, &replaces, &generic_filename, &specific_filename);

	binary_exponential_buffer_increment(&searches, 1);
	((unsigned char **)binary_exponential_buffer_data(&searches))[1] = (unsigned char *)"GENERIC_TYPE0_LITERAL";

	binary_exponential_buffer_increment(&replaces, 1);
	((unsigned char **)binary_exponential_buffer_data(&replaces))[1] = (unsigned char *)t1;

	add_file_to_buffer(&in_characters, (unsigned char *)"data-structures/generic.h.stack");
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_h_file(&out_characters, &specific_filename);
	/*  Clear input and output buffers. */
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_file_to_buffer(&in_characters, (unsigned char *)"data-structures/generic.c.stack");
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_c_file(&out_characters, &specific_filename);

	add_object_build_rule(&specific_filename);
	add_library_build_rule(&specific_filename);
	add_library_dependency_rule(&specific_filename);
	add_clean_rule(&specific_filename);
	add_file_dependency_rule(&specific_filename);

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_destroy(&t1_identifier);
	binary_exponential_buffer_destroy(&generic_filename);
	binary_exponential_buffer_destroy(&specific_filename);
}

void create_list_type(char * t1){
	struct binary_exponential_buffer searches;
	struct binary_exponential_buffer replaces;
	struct binary_exponential_buffer generic_filename;
	struct binary_exponential_buffer specific_filename;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;
	struct binary_exponential_buffer t1_identifier = create_identifier_friendly_type((unsigned char *)t1);

	binary_exponential_buffer_increment(&t1_identifier, 1);
	((unsigned char *)binary_exponential_buffer_data(&t1_identifier))[binary_exponential_buffer_size(&t1_identifier) - 1] = 0;

	binary_exponential_buffer_create(&specific_filename, sizeof(unsigned char *));
	binary_exponential_buffer_create(&generic_filename, sizeof(unsigned char *));
	binary_exponential_buffer_create(&searches, sizeof(unsigned char *));
	binary_exponential_buffer_create(&replaces, sizeof(unsigned char *));
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_string_literal_to_buffer((unsigned char *)"data-structures/GENERIC_TYPE0_IDENTIFIER_FRIENDLY_list", &generic_filename, 0);

	binary_exponential_buffer_increment(&searches, 1);
	((unsigned char **)binary_exponential_buffer_data(&searches))[0] = (unsigned char *)"GENERIC_TYPE0_IDENTIFIER_FRIENDLY";
	binary_exponential_buffer_increment(&replaces, 1);
	((unsigned char **)binary_exponential_buffer_data(&replaces))[0] = (unsigned char *)binary_exponential_buffer_data(&t1_identifier);
	/*  Get our specific filename */
	do_string_replacements(&searches, &replaces, &generic_filename, &specific_filename);

	binary_exponential_buffer_increment(&searches, 1);
	((unsigned char **)binary_exponential_buffer_data(&searches))[1] = (unsigned char *)"GENERIC_TYPE0_LITERAL";

	binary_exponential_buffer_increment(&replaces, 1);
	((unsigned char **)binary_exponential_buffer_data(&replaces))[1] = (unsigned char *)t1;

	add_file_to_buffer(&in_characters, (unsigned char *)"data-structures/generic.h.list");
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_h_file(&out_characters, &specific_filename);
	/*  Clear input and output buffers. */
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_file_to_buffer(&in_characters, (unsigned char *)"data-structures/generic.c.list");
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_c_file(&out_characters, &specific_filename);

	add_object_build_rule(&specific_filename);
	add_library_build_rule(&specific_filename);
	add_library_dependency_rule(&specific_filename);
	add_clean_rule(&specific_filename);
	add_file_dependency_rule(&specific_filename);

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_destroy(&t1_identifier);
	binary_exponential_buffer_destroy(&generic_filename);
	binary_exponential_buffer_destroy(&specific_filename);
}

void create_map_type(char * t1, char * t2){
	struct binary_exponential_buffer searches;
	struct binary_exponential_buffer replaces;
	struct binary_exponential_buffer generic_filename;
	struct binary_exponential_buffer specific_filename;
	struct binary_exponential_buffer in_characters;
	struct binary_exponential_buffer out_characters;
	struct binary_exponential_buffer t1_identifier = create_identifier_friendly_type((unsigned char *)t1);
	struct binary_exponential_buffer t2_identifier = create_identifier_friendly_type((unsigned char *)t2);

	binary_exponential_buffer_increment(&t1_identifier, 1);
	((unsigned char *)binary_exponential_buffer_data(&t1_identifier))[binary_exponential_buffer_size(&t1_identifier) - 1] = 0;
	binary_exponential_buffer_increment(&t2_identifier, 1);
	((unsigned char *)binary_exponential_buffer_data(&t2_identifier))[binary_exponential_buffer_size(&t2_identifier) - 1] = 0;

	binary_exponential_buffer_create(&specific_filename, sizeof(unsigned char *));
	binary_exponential_buffer_create(&generic_filename, sizeof(unsigned char *));
	binary_exponential_buffer_create(&searches, sizeof(unsigned char *));
	binary_exponential_buffer_create(&replaces, sizeof(unsigned char *));
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_string_literal_to_buffer((unsigned char *)"data-structures/GENERIC_TYPE0_IDENTIFIER_FRIENDLY_to_GENERIC_TYPE1_IDENTIFIER_FRIENDLY_map", &generic_filename, 0);

	binary_exponential_buffer_increment(&searches, 2);
	((unsigned char **)binary_exponential_buffer_data(&searches))[0] = (unsigned char *)"GENERIC_TYPE0_IDENTIFIER_FRIENDLY";
	((unsigned char **)binary_exponential_buffer_data(&searches))[1] = (unsigned char *)"GENERIC_TYPE1_IDENTIFIER_FRIENDLY";
	binary_exponential_buffer_increment(&replaces, 2);
	((unsigned char **)binary_exponential_buffer_data(&replaces))[0] = (unsigned char *)binary_exponential_buffer_data(&t1_identifier);
	((unsigned char **)binary_exponential_buffer_data(&replaces))[1] = (unsigned char *)binary_exponential_buffer_data(&t2_identifier);
	/*  Get our specific filename */
	do_string_replacements(&searches, &replaces, &generic_filename, &specific_filename);

	binary_exponential_buffer_increment(&searches, 2);
	((unsigned char **)binary_exponential_buffer_data(&searches))[2] = (unsigned char *)"GENERIC_TYPE0_LITERAL";
	((unsigned char **)binary_exponential_buffer_data(&searches))[3] = (unsigned char *)"GENERIC_TYPE1_LITERAL";

	binary_exponential_buffer_increment(&replaces, 2);
	((unsigned char **)binary_exponential_buffer_data(&replaces))[2] = (unsigned char *)t1;
	((unsigned char **)binary_exponential_buffer_data(&replaces))[3] = (unsigned char *)t2;

	add_file_to_buffer(&in_characters, (unsigned char *)"data-structures/generic.h.map");
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_h_file(&out_characters, &specific_filename);
	/*  Clear input and output buffers. */
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_create(&in_characters, sizeof(unsigned char));
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_create(&out_characters, sizeof(unsigned char));

	add_file_to_buffer(&in_characters, (unsigned char *)"data-structures/generic.c.map");
	do_string_replacements(&searches, &replaces, &in_characters, &out_characters);
	output_buffer_to_c_file(&out_characters, &specific_filename);

	add_object_build_rule(&specific_filename);
	add_library_dependency_rule(&specific_filename);
	add_library_build_rule(&specific_filename);
	add_clean_rule(&specific_filename);
	add_file_dependency_rule(&specific_filename);

	binary_exponential_buffer_destroy(&searches);
	binary_exponential_buffer_destroy(&replaces);
	binary_exponential_buffer_destroy(&in_characters);
	binary_exponential_buffer_destroy(&out_characters);
	binary_exponential_buffer_destroy(&t1_identifier);
	binary_exponential_buffer_destroy(&t2_identifier);
	binary_exponential_buffer_destroy(&generic_filename);
	binary_exponential_buffer_destroy(&specific_filename);
}

int main(void){
	binary_exponential_buffer_create(&object_makefile, sizeof(unsigned char));
	binary_exponential_buffer_create(&library_dependency, sizeof(unsigned char));
	binary_exponential_buffer_create(&library_buildcommand, sizeof(unsigned char));
	binary_exponential_buffer_create(&clean_files, sizeof(unsigned char));
	binary_exponential_buffer_create(&file_dependencies, sizeof(unsigned char));

	add_string_literal_to_buffer((unsigned char *)"clean-generated-files:\n\t@rm -f ", &clean_files, 0);
	add_string_literal_to_buffer((unsigned char *)"data-structures/libgenerated-data-structures.a: ", &library_dependency, 0);
	add_string_literal_to_buffer((unsigned char *)"\n\t@ar -rcs data-structures/libgenerated-data-structures.a ", &library_buildcommand, 0);

	/*  To create additional datatypes, you can simply add a line below and the build process should take care of the rest */
	create_map_type("unsigned char *", "struct linker_symbol *");
	create_map_type("unsigned char *", "unsigned char *");
	create_map_type("unsigned char *", "struct constant_description *");
	create_list_type("char");
	create_list_type("char *");
	create_list_type("unsigned char *");
	create_list_type("unsigned char");
	create_list_type("unsigned int");
	create_list_type("void *");
	create_list_type("struct memory_pooler *");
	create_list_type("struct c_lexer_token *");
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
	create_stack_type("unsigned int");
	create_stack_type("struct parser_operation");

	add_string_literal_to_buffer((unsigned char *)": bootstrap-datatypes", &file_dependencies, 0);
	add_string_literal_to_buffer((unsigned char *)"", &library_buildcommand, 1);
	add_string_literal_to_buffer((unsigned char *)binary_exponential_buffer_data(&library_buildcommand), &library_dependency, 0);

	output_buffer_to_file(&library_dependency, (unsigned char *)"data-structures/library-data-structures");
	output_buffer_to_file(&object_makefile, (unsigned char *)"data-structures/object-data-structures");
	output_buffer_to_file(&clean_files, (unsigned char *)"data-structures/clean-data-structures");
	output_buffer_to_file(&file_dependencies, (unsigned char *)"data-structures/file-dependencies-data-structures");

	binary_exponential_buffer_destroy(&object_makefile);
	binary_exponential_buffer_destroy(&library_dependency);
	binary_exponential_buffer_destroy(&library_buildcommand);
	binary_exponential_buffer_destroy(&clean_files);
	binary_exponential_buffer_destroy(&file_dependencies);

	return 0;
}
