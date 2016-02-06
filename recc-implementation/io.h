#ifndef IO_H_
#define IO_H_
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

#ifndef STDLIB_H_DEFINED_
#include <stdlib.h>
#endif
#ifndef STDIO_H_DEFINED_
#include <stdio.h>
#endif
#ifndef STDARG_H_DEFINED_
#include <stdarg.h>
#endif
#ifndef ASSERT_H_DEFINED_
#include <assert.h>
#endif
#ifndef STRING_H_DEFINED_
#include <string.h>
#endif
#ifndef unsigned_char_list_H_DEFINED_
#include "../data-structures/unsigned_char_list.h"
#endif
#ifndef unsigned_char_ptr_list_H_DEFINED_
#include "../data-structures/unsigned_char_ptr_list.h"
#endif
#ifndef MEMORY_POOL_COLLECTION_H_
#include "memory_pool_collection.h"
#endif
#ifndef HEAP_MEMORY_POOL_H_
#include "heap_memory_pool.h"
#endif
#ifndef TYPES_BUILTIN_struct_printing_state_H_
#include "../types/builtin/struct_printing_state.h"
#endif
#ifndef BUILTIN_C99LIBC_IN_C89_
#include "../builtin/c99libc_in_c89.h"
#endif

extern unsigned char * g_format_buffer;  /* TODO: get rid of this.  This is a hack that must exist until snprintf can be implemented */
extern unsigned int g_format_buffer_uses;

void g_format_buffer_use(void);
void g_format_buffer_release(void);
int add_file_to_buffer(struct unsigned_char_list *, char *);
int output_buffer_to_file(struct unsigned_char_list *, char *);
void buffered_printf(struct unsigned_char_list * list, const char* format, ...);
unsigned char * get_sprintf_buffer(void);
unsigned char * copy_string(unsigned char *, unsigned char *, struct memory_pool_collection *);
void copy_string_into_buffer(unsigned char *, unsigned char *, unsigned char *);
unsigned char * get_null_terminator(unsigned char *);
void add_string_to_buffer(struct unsigned_char_list *, unsigned char *, unsigned char *);
int unsigned_strcmp(unsigned char *, unsigned char *);
unsigned char * copy_null_terminated_string(unsigned char *, struct memory_pool_collection *);
void resolve_path_components(unsigned char *, struct unsigned_char_ptr_list *, struct memory_pool_collection *);
unsigned char * vcreate_formatted_string(struct memory_pool_collection *, unsigned int, const char *, va_list, va_list);
unsigned char * create_formatted_string(struct memory_pool_collection *, unsigned int, const char *, ...);
void buffered_puts(struct unsigned_char_list *, const char *);

#endif
