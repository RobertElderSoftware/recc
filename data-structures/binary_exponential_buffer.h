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
#ifndef __BINARY_EXPONENTIAL_BUFFER__H__DEFINED__
#define __BINARY_EXPONENTIAL_BUFFER__H__DEFINED__

#include <assert.h>
#include <stdlib.h>

struct binary_exponential_buffer{
	void * data;
	unsigned int size;
	unsigned int element_size;
	unsigned int next_threshold_size;
	unsigned int pad;
};

void binary_exponential_buffer_increment(struct binary_exponential_buffer *, unsigned int);
void binary_exponential_buffer_decrement(struct binary_exponential_buffer *, unsigned int);
void * binary_exponential_buffer_data(struct binary_exponential_buffer *);
unsigned int binary_exponential_buffer_size(struct binary_exponential_buffer *);
void * binary_exponential_buffer_get(struct binary_exponential_buffer *, unsigned int);
void binary_exponential_buffer_destroy(struct binary_exponential_buffer *);
void binary_exponential_buffer_create(struct binary_exponential_buffer *, unsigned int);
struct binary_exponential_buffer binary_exponential_buffer_copy(struct binary_exponential_buffer *);

#endif
