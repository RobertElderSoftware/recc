#ifndef STDIO_H_DEFINED__
#define STDIO_H_DEFINED__
/*
    Copyright 2015 Robert Elder Software Inc.
    
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

#ifndef COMMON_H_DEFINED__
#include "common.h"
#endif
#ifndef STDARG_H_DEFINED__
#include <stdarg.h>
#endif
#ifndef PUTCHAR_H_DEFINED__
#include <putchar.h>
#endif
#ifndef SIZE_T_H_DEFINED__
#include "size_t.h"
#endif

#define EOF 255   /*  TODO: should be -1*/

int vsnprintf(char *, size_t, const char *, va_list);
int snprintf(char *, size_t, const char *, ...);
int vsprintf (char *, const char *, va_list);
int printf(const char *, ...);
int vprintf(const char *, ...);

struct file_descriptor{
	unsigned int position;
	unsigned int inode_index;
	unsigned int current_block_index;
};

typedef struct file_descriptor FILE;

FILE * fopen(const char *, const char *);
size_t fread(void *, size_t, size_t, FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
int fclose(FILE *);
int getc(FILE *);
int fputc(int, FILE *);

#ifndef STDLIB_FILESYSTEM_H__
#include "filesystem.h"
#endif

#endif
