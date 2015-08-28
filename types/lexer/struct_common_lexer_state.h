#ifndef __TYPES_LEXER_struct_common_lexer_state_H__
#define __TYPES_LEXER_struct_common_lexer_state_H__
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

#ifndef __TYPES_struct_memory_pool_collection_H__
#include "../struct_memory_pool_collection.h"
#endif
#ifndef __TYPES_DATA_STRUCTURES_struct_unsigned_char_list_H__
#include "../data-structures/struct_unsigned_char_list.h"
#endif

struct common_lexer_state {
	unsigned char * buf;
	unsigned char * filename;
	struct memory_pool_collection * memory_pool_collection;
	struct unsigned_char_list * buffered_output;
	unsigned int position;
	unsigned int current_line;
	unsigned int buffer_size;
	unsigned int pad;
};

#endif
