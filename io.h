#ifndef __IO_H__
#define __IO_H__
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

#ifndef __STDLIB_H_DEFINED__
#include <stdlib.h>
#endif
#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif
#ifndef __STDARG_H_DEFINED__
#include <stdarg.h>
#endif
#ifndef __ASSERT_H_DEFINED__
#include <assert.h>
#endif
#ifndef __STRING_H_DEFINED__
#include <string.h>
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "data-structures/unsigned_char_list.h"
#endif
#ifndef __unsigned_char_ptr_list__H__DEFINED__
#include "data-structures/unsigned_char_ptr_list.h"
#endif
#ifndef __MEMORY_POOL_COLLECTION_H__
#include "memory_pool_collection.h"
#endif
#ifndef __HEAP_MEMORY_POOL_H__
#include "heap_memory_pool.h"
#endif

extern unsigned char * g_format_buffer;  /* TODO: get rid of this.  This is a hack that must exist until snprintf can be implemented */
extern unsigned int g_format_buffer_uses;

void g_format_buffer_use(void);
void g_format_buffer_release(void);
int add_file_to_buffer(struct unsigned_char_list *, char *);
int output_buffer_to_file(struct unsigned_char_list *, char *);
void buffered_printf(struct unsigned_char_list * list, const char* format, ...);
void vsprintf_hook(const char*, va_list);
void sprintf_hook(const char*, ...);
unsigned char * get_sprintf_buffer(void);
unsigned char * copy_string(unsigned char *, unsigned char *, struct memory_pool_collection *);
void copy_string_into_buffer(unsigned char *, unsigned char *, unsigned char *);
unsigned char * get_null_terminator(unsigned char *);
void add_string_to_buffer(struct unsigned_char_list *, unsigned char *, unsigned char *);
int unsigned_strcmp(unsigned char *, unsigned char *);
unsigned char * copy_null_terminated_string(unsigned char *, struct memory_pool_collection *);
void resolve_path_components(unsigned char *, struct unsigned_char_ptr_list *, struct memory_pool_collection *);

#endif
