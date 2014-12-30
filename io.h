/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "data-structures/unsigned_char_list.h"

size_t strlen(const char *);
void g_format_buffer_use(void);
void g_format_buffer_release(void);
int add_file_to_buffer(struct unsigned_char_list *, char *);
int output_buffer_to_file(struct unsigned_char_list *, char *);
void buffered_printf(struct unsigned_char_list * list, const char* format, ...);
void vsprintf_hook(const char*, va_list);
void sprintf_hook(const char*, ...);
unsigned char * get_sprintf_buffer(void);
int strcmp(const char *, const char *);
unsigned char * copy_string(unsigned char *, unsigned char *);
void copy_string_into_buffer(unsigned char *, unsigned char *, unsigned char *);
unsigned char * get_null_terminator(unsigned char *);
void add_string_to_buffer(struct unsigned_char_list *, unsigned char *, unsigned char *);
int unsigned_strcmp(unsigned char *, unsigned char *);

#endif
