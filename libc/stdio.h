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
#ifndef __STDIO_H_DEFINED__
#define __STDIO_H_DEFINED__

#include "common.h"
#include <stdarg.h>

#define EOF 255   /*  TODO: shouldn't be -1*/

int vsprintf (char *, const char *, va_list);
int printf(const char *, ...);
int putchar(int);

struct file_descriptor{
	unsigned int position;
	unsigned int inode_index;
	unsigned int current_block_index;
};

typedef struct file_descriptor FILE;

FILE * fopen(const char *, const char *);
int fclose(FILE *);
int getc(FILE *);
int fputc(int, FILE *);

#include "filesystem.h"

#endif
