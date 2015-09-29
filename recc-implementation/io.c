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
#include "io.h"

unsigned char * vcreate_formatted_string(struct memory_pool_collection * m, unsigned int estimated_bytes_needed, const char * format, va_list try1, va_list try2){
	/*  Use the estimate, otherwise allocate more memory. */
	unsigned int chars_actually_required;
	unsigned char * rtn = (unsigned char *)heap_memory_pool_malloc(m->heap_pool, estimated_bytes_needed);
	chars_actually_required = c89_vsnprintf((char *)rtn, estimated_bytes_needed, format, try1);
	if(chars_actually_required < estimated_bytes_needed){
		return rtn;
	}else{
		heap_memory_pool_free(m->heap_pool, rtn);
		rtn = vcreate_formatted_string(m, chars_actually_required + 1, format, try2, try1);
		return rtn;
	}
}

unsigned char * create_formatted_string(struct memory_pool_collection * m, unsigned int estimated_bytes_needed, const char * format, ...){
	va_list try1;
	va_list try2;
	unsigned char * rtn;
	va_start(try1, format);
	va_start(try2, format);
	rtn = vcreate_formatted_string(m, estimated_bytes_needed, format, try1, try2);
	va_end(try2);
	va_end(try1);
	return rtn;
}

void buffered_printf(struct unsigned_char_list * list, const char* format, ...){
	va_list try1;
	va_list try2;
	unsigned int estimated_bytes_needed = 20;  /*  Includes space for a null terminator which we need to take off */
	unsigned int chars_actually_needed; /*  Characters not including null terminator */
	unsigned int size_before = binary_exponential_buffer_size(&list->buffer);
	va_start(try1, format);
	va_start(try2, format);
	/*  Assume 20 is enough for our buffer */
	binary_exponential_buffer_increment(&list->buffer, estimated_bytes_needed);
	chars_actually_needed = c89_vsnprintf((char *)&((unsigned char *)binary_exponential_buffer_data(&list->buffer))[size_before], estimated_bytes_needed, format, try1);
	if(chars_actually_needed < estimated_bytes_needed){
		/*  There was enough space to write the string and we have to remove the null terminator and extra space */
		binary_exponential_buffer_decrement(&list->buffer, estimated_bytes_needed - chars_actually_needed);
	}else{
		/*  There was not enough space to write the string the first time, but now we know how much we need */
		binary_exponential_buffer_increment(&list->buffer, (chars_actually_needed + 1) - estimated_bytes_needed);
		c89_vsnprintf((char *)&((unsigned char *)binary_exponential_buffer_data(&list->buffer))[size_before], chars_actually_needed + 1, format, try2);
		binary_exponential_buffer_decrement(&list->buffer, 1); /*  Remove null terminator */
	}
	va_end(try1);
	va_end(try2);
}

int add_file_to_buffer(struct unsigned_char_list * buffer, char * in_file){
	FILE *f = NULL;
	unsigned int bytes_this_chunk = 0;
	unsigned int bytes_next_chunk = 1;
	unsigned int bytes_just_read = 0;
	unsigned int current_buffer_size;
	if(!(f = fopen(in_file, "rb"))){
		printf("Failed to open file %s for read.\n", in_file);
		assert(0 && "Failed to open file.");
		return 1;
	}

	while (bytes_just_read == bytes_this_chunk) {
		unsigned char * start_write_position;
		bytes_this_chunk = bytes_next_chunk;
		current_buffer_size = binary_exponential_buffer_size(&buffer->buffer);
		binary_exponential_buffer_increment(&buffer->buffer, bytes_this_chunk);
		start_write_position = &(((unsigned char *)binary_exponential_buffer_data(&buffer->buffer))[current_buffer_size * buffer->buffer.element_size]);
		bytes_just_read = (unsigned int)fread(start_write_position, sizeof(unsigned char), bytes_this_chunk, f);
		bytes_next_chunk = bytes_this_chunk * 2; /* Increment by two each time */
	}
	/*  Last increment was likely a bit larger than it needed to be */
	binary_exponential_buffer_decrement(&buffer->buffer, bytes_this_chunk - bytes_just_read);
	fclose(f);
	return 0;
}

int output_buffer_to_file(struct unsigned_char_list * buffer, char * out_file){
	FILE *f = NULL;
	unsigned char * data = unsigned_char_list_data(buffer);
	unsigned int size = unsigned_char_list_size(buffer);
	if(!(f = fopen(out_file, "w"))){
		printf("Failed to open file %s for write.\n", out_file);
		return 1;
	}

	fwrite(data, sizeof(unsigned char), size, f);
	fclose(f);
	return 0;
}

unsigned char * get_null_terminator(unsigned char * c){
	while(*c) {
		c++;
	}
	return c;
}

unsigned char * copy_null_terminated_string(unsigned char * start, struct memory_pool_collection * m){
	unsigned int length = (unsigned int)strlen((char *)start);
	unsigned char * rtn = (unsigned char *)heap_memory_pool_malloc(m->heap_pool, ((sizeof(unsigned char) * length) + 1));
	unsigned int i = 0;
	(void)m;
	for(i = 0; i < length; i++){
		rtn[i] = start[i];
	}
	rtn[length] = 0;
	return rtn;
}

unsigned char * copy_string(unsigned char * start, unsigned char * end, struct memory_pool_collection * m){
	unsigned int length = (unsigned int)(end - start) + 1;
	unsigned char * string;
	(void)m;
	string = (unsigned char *)heap_memory_pool_malloc(m->heap_pool, (sizeof(unsigned char) * length) + 1);
	string[length] = 0; /* Null termination */
	memcpy(string, start, length);
	return string;
}

void add_string_to_buffer(struct unsigned_char_list * buffer, unsigned char * start, unsigned char * end){
	unsigned char * i;
	for(i = start; i < (end + 1); i++){
		buffered_printf(buffer,"%c", *i);
	}
}

int unsigned_strcmp(unsigned char * a, unsigned char * b){
	return strcmp((const char *)a,(const char *)b);
}

void resolve_path_components(unsigned char * path, struct unsigned_char_ptr_list * path_components, struct memory_pool_collection * m){
	/*  Split up a path like /a/b/abc/d into 'a', 'b', 'abc', 'd' */
	unsigned int j = 0;
	struct unsigned_char_list tmp;
	unsigned_char_list_create(&tmp);
	while(path[j]){
		if(path[j] == '/'){
			if(unsigned_char_list_size(&tmp)){
				unsigned int len = unsigned_char_list_size(&tmp);
				unsigned char * s = copy_string(unsigned_char_list_data(&tmp), ((unsigned char *)unsigned_char_list_data(&tmp)) + (len -1), m);
				unsigned_char_ptr_list_add_end(path_components, s);
			}else{
				unsigned char * s = (unsigned char *)heap_memory_pool_malloc(m->heap_pool, 1);
				s[0] = 0; /* empty string */
				unsigned_char_ptr_list_add_end(path_components, s);
			}
			unsigned_char_list_destroy(&tmp);
			unsigned_char_list_create(&tmp);
		}else{
			unsigned_char_list_add_end(&tmp, path[j]);
		}
		j++;
	}
	if(unsigned_char_list_size(&tmp)){
		unsigned int len = unsigned_char_list_size(&tmp);
		unsigned char * s = copy_string(unsigned_char_list_data(&tmp), ((unsigned char *)unsigned_char_list_data(&tmp)) + (len-1), m);
		unsigned_char_ptr_list_add_end(path_components, s);
	}
	unsigned_char_list_destroy(&tmp);
}
