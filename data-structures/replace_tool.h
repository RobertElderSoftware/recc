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
#ifndef __REPLACE_TOOL__H__DEFINED__
#define __REPLACE_TOOL__H__DEFINED__

#include "binary_exponential_buffer.h"

void validate_replacement_strings(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void reset_candidates(struct binary_exponential_buffer *);
unsigned int count_candidates(struct binary_exponential_buffer *);
unsigned int get_length(unsigned char *);
unsigned int set_lengths(struct binary_exponential_buffer *, struct binary_exponential_buffer *);
void do_string_replacements(struct binary_exponential_buffer *, struct binary_exponential_buffer *, struct binary_exponential_buffer *, struct binary_exponential_buffer *);
#endif

