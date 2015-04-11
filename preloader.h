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
#ifndef __PRELOADER_H__
#define __PRELOADER_H__

#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct preloader_state;

enum language_type {
	C_LANGUAGE_TYPE,
	JSONP_LANGUAGE_TYPE,
	PYTHON_LANGUAGE_TYPE,
	JAVA_LANGUAGE_TYPE
};

struct preloader_state * preloader_state_create(char *, char *, char *, enum language_type);
void preloader_state_destroy(struct preloader_state *);

#endif
