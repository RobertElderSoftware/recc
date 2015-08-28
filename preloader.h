#ifndef __PRELOADER_H__
#define __PRELOADER_H__
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

#ifndef __IO_H__
#include "io.h"
#endif
#ifndef __CORE_DATA_TYPES__H__DEFINED__
#include "core_data_types.h"
#endif
#ifndef __unsigned_char_list__H__DEFINED__
#include "data-structures/unsigned_char_list.h"
#endif
#ifndef __struct_preloader_instruction_list__H__DEFINED__
#include "data-structures/struct_preloader_instruction_list.h"
#endif
#ifndef __STDIO_H_DEFINED__
#include <stdio.h>
#endif
#ifndef __STDLIB_H_DEFINED__
#include <stdlib.h>
#endif
#ifndef __ASSERT_H_DEFINED__
#include <assert.h>
#endif

struct preloader_state;

enum language_type {
	C_LANGUAGE_TYPE,
	JSONP_LANGUAGE_TYPE,
	PYTHON_LANGUAGE_TYPE,
	JAVA_LANGUAGE_TYPE
};

struct preloader_state * preloader_state_create(unsigned char *, unsigned char *, unsigned char *, unsigned char *);
void preloader_state_destroy(struct preloader_state *);

#endif
