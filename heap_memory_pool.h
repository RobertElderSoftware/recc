#ifndef __HEAP_MEMORY_POOL_H__
#define __HEAP_MEMORY_POOL_H__
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
#include "data-structures/void_ptr_memory_pool.h"
#endif
#ifndef __TYPES_struct_heap_memory_pool_H__
#include "types/struct_heap_memory_pool.h"
#endif

void heap_memory_pool_create(struct heap_memory_pool *);
void heap_memory_pool_destroy(struct heap_memory_pool *);
void * heap_memory_pool_malloc(struct heap_memory_pool *, unsigned int);
void heap_memory_pool_free(struct heap_memory_pool *, void *);

#endif
