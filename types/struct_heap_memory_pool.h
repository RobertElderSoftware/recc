#ifndef __TYPES_struct_heap_memory_pool_H__
#define __TYPES_struct_heap_memory_pool_H__
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

#ifndef __void_ptr_memory_pool__H__DEFINED__
#include "../data-structures/void_ptr_memory_pool.h"
#endif
#ifndef __void_ptr_to_unsigned_int_map__H__DEFINED__
#include "../data-structures/void_ptr_to_unsigned_int_map.h"
#endif

#define NUM_HEAP_POOLS 20

struct heap_memory_pool{
	struct void_ptr_to_unsigned_int_map contiguous_heap_bock_entries; /*  Stores pointers that can be used to deduce which pool similar pointers came from*/
	struct void_ptr_memory_pool heap_pools[NUM_HEAP_POOLS];
};

#endif
