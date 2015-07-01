#ifndef __STDARG_H_DEFINED__
#define __STDARG_H_DEFINED__
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
#define va_list unsigned int *
#define va_start(a,b) a = (unsigned int*)&b
#define va_arg(a,b) *((b*)(a=(unsigned int *)((unsigned int)a - sizeof(unsigned int))))
#define va_end(a) 
#endif
