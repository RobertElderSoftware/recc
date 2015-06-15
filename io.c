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
	unsigned int i = 0;
	va_start( arglist, format );
	vsprintf_hook(format, arglist);
	while(g_format_buffer[i]){
		unsigned_char_list_add_end(list, (unsigned char)g_format_buffer[i]);
		i++;
	}
	va_end(arglist);
}

int add_file_to_buffer(struct unsigned_char_list * buffer, char * in_file){
	FILE *f = NULL;
	int c = 0;
	if(!(f = fopen(in_file, "rb"))){
		printf("Failed to open file %s for read.\n", in_file);
		return 1;
	}

	while (c != EOF) {
		c = getc(f);
		if(c == EOF)
			break;
		unsigned_char_list_add_end(buffer, (unsigned char)c);
	}
	fclose(f);
	return 0;
}

int output_buffer_to_file(struct unsigned_char_list * buffer, char * out_file){
	FILE *f = NULL;
	unsigned char * data = unsigned_char_list_data(buffer);
	unsigned int size = unsigned_char_list_size(buffer);
	unsigned int i;
	if(!(f = fopen(out_file, "w"))){
		printf("Failed to open file %s for write.\n", out_file);
		return 1;
	}

	for(i = 0; i < size; i++){
		fputc ( data[i] , f);
	}
	fclose(f);
	return 0;
}

void copy_string_into_buffer(unsigned char * from, unsigned char * to, unsigned char * dst){
	/* Copies characters between and including from and to.  dst is assumed to be large enough */
	unsigned char * i;
	unsigned int j = 0;
	for(i = from; i < (to + 1); i++){
		dst[j] = *i;
		j = j + 1;
	}
}

unsigned char * get_null_terminator(unsigned char * c){
	while(*c) {
		c++;
	}
	return c;
}

unsigned char * copy_null_terminated_string(unsigned char * start){
	unsigned int length = (unsigned int)strlen((char *)start);
	unsigned char * rtn = (unsigned char *)malloc((sizeof(unsigned char) * length) + 1);
	unsigned int i = 0;
	for(i = 0; i < length; i++){
		rtn[i] = start[i];
	}
	rtn[length] = 0;
	return rtn;
}

unsigned char * copy_string(unsigned char * start, unsigned char * end){
	unsigned int length = (unsigned int)(end - start) + 1;
	unsigned char * string;
	string = (unsigned char *)malloc((sizeof(unsigned char) * length) + 1);
	string[length] = 0; /* Null termination */
	copy_string_into_buffer(start, end, string);
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

void resolve_path_components(unsigned char * path, struct unsigned_char_ptr_list * path_components){
	/*  Split up a path like /a/b/abc/d into 'a', 'b', 'abc', 'd' */
	unsigned int j = 0;
	struct unsigned_char_list tmp;
	unsigned_char_list_create(&tmp);
	while(path[j]){
		if(path[j] == '/'){
			if(unsigned_char_list_size(&tmp)){
				unsigned int len = unsigned_char_list_size(&tmp);
				unsigned char * s = copy_string(unsigned_char_list_data(&tmp), ((unsigned char *)unsigned_char_list_data(&tmp)) + (len -1));
				unsigned_char_ptr_list_add_end(path_components, s);
			}else{
				unsigned char * s = malloc(1);
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
		unsigned char * s = copy_string(unsigned_char_list_data(&tmp), ((unsigned char *)unsigned_char_list_data(&tmp)) + (len-1));
		unsigned_char_ptr_list_add_end(path_components, s);
	}
	unsigned_char_list_destroy(&tmp);
}
