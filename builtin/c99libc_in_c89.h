#ifndef __BUILTIN_C99LIBC_IN_C89__
#define __BUILTIN_C99LIBC_IN_C89__
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
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include "print_buff_add.h"
#include "print_string.h"
#include "print_decimal.h"
#include "print_hexadecimal.h"
#include "../types/builtin/struct_printing_state.h"

unsigned int c89_vsnprintf(char *, size_t, const char *, va_list);
#endif
