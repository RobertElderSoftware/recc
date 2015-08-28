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

unsigned char * g_format_buffer = 0;  /* TODO: get rid of this.  This is a hack that must exist until snprintf can be implemented */
unsigned int g_format_buffer_uses = 0;

unsigned char * get_sprintf_buffer(void){
	return g_format_buffer;
}

void g_format_buffer_use(void){
	if(!g_format_buffer_uses){
		g_format_buffer = (unsigned char *)malloc(1024 * 1024);
	}
	g_format_buffer_uses++;
}

void g_format_buffer_release(void){
	assert(g_format_buffer_uses);
	g_format_buffer_uses--;
	if(!g_format_buffer_uses){
		free(g_format_buffer);
	}
}

void vsprintf_hook(const char* format, va_list v){
	/* TODO:  this is not safe because sprintf is inherently unsafe.  Need to implement snprintf, since its not c89 */
	vsprintf((char*)g_format_buffer, format, v);
}

void sprintf_hook(const char* format, ...){
	va_list arglist;
	va_start( arglist, format );
	vsprintf_hook(format, arglist);
	va_end(arglist);
}

void buffered_printf(struct unsigned_char_list * list, const char* format, ...){
	va_list arglist;
	unsigned int length;
	unsigned char * dst;
	unsigned int size_before;
	va_start(arglist, format);
	vsprintf_hook(format, arglist);
	length = (unsigned int)strlen((char *)g_format_buffer);
	size_before = binary_exponential_buffer_size(&list->buffer);
	binary_exponential_buffer_increment(&list->buffer, length);
	dst = (unsigned char *)binary_exponential_buffer_data(&list->buffer);
	memcpy(&dst[size_before], g_format_buffer, length);
	va_end(arglist);
}

int add_file_to_buffer(struct unsigned_char_list * buffer, char * in_file){
	FILE *f = NULL;
	unsigned int bytes_this_chunk = 0;
	unsigned int bytes_next_chunk = 1;
	unsigned int bytes_just_read = 0;
	unsigned int current_buffer_size;
	if(!(f = fopen(in_file, "rb"))){
		printf("Failed to open file %s for read.\n", in_file);
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
