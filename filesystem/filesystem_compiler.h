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
#ifndef __FILESYSTEM_COMPILER_H__
#define __FILESYSTEM_COMPILER_H__

#include <assert.h>
#include <stdio.h>
#include "../libc/filesystem.h"
#include "../data-structures/unsigned_char_ptr_to_unsigned_char_ptr_map.h"
#include "../data-structures/unsigned_char_list.h"
#include "../io.h"

void output_filesystem_impl(void);
void create_directories(struct unsigned_char_ptr_list *);
void create_files(struct unsigned_char_ptr_to_unsigned_char_ptr_map *);
void load_file(unsigned char *, FILE *);
void load_file_given_new_inode(unsigned int, FILE *);

#endif
