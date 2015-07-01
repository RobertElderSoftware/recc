#ifndef __ASSERT_H_DEFINED__
#define __ASSERT_H_DEFINED__
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
#include <stdio.h>
#endif

#define assert_stringify_indirect(a) #a
#define assert_stringify(a) assert_stringify_indirect(a)

#define assert(a) if(!(a)){printf("Assertion failure of expression '"assert_stringify_indirect(a)"' in file "assert_stringify(__FILE__)" on line "assert_stringify(__LINE__)".\n");}

#endif
