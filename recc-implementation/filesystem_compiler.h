#ifndef __FILESYSTEM_COMPILER_H__
#define __FILESYSTEM_COMPILER_H__
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

#ifndef __ASSERT_H_DEFINED__
#include <assert.h>
#endif
#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif
#ifndef __STDLIB_FILESYSTEM_H__
#include "../libc/filesystem.h"
#endif
#ifndef __unsigned_char_ptr_to_unsigned_char_ptr_map__H__DEFINED__
#include "../data-structures/unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "../data-structures/unsigned_char_list.h"
#endif
#ifndef __IO_H__
#include "../recc-implementation/io.h"
#endif

void output_filesystem_impl(unsigned char *);
void create_directories(struct unsigned_char_ptr_list *, struct memory_pool_collection *);
void create_files(struct unsigned_char_ptr_to_unsigned_char_ptr_map *, struct memory_pool_collection *);
void load_file(unsigned char *, FILE *, struct memory_pool_collection *);
void load_file_given_new_inode(unsigned int, FILE *);
void create_filesystem_impl(unsigned char *);

#endif
