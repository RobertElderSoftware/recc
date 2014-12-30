/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#ifndef __MEMORY_POOLER__H__DEFINED__
#define __MEMORY_POOLER__H__DEFINED__

#include <assert.h>
#include "binary_exponential_buffer.h"
#include "void_ptr_list.h"
#include "struct_memory_pooler_ptr_list.h"
#include "unsigned_int_list.h"

struct memory_pooler{
	struct void_ptr_list free_pointers;  /* Pointers that we can just hand out next time we get a memory request */
	struct void_ptr_list block_pointers;  /* An array of pointers to blocks of object_size * 2^n */
	unsigned int object_size; /*  The size of the object (in bytes) for which we're providing this service. */
	unsigned int objects_allocated;  /*  The total number of outstanding non-freed objects */
	unsigned int next_allocation;  /*  If we're asked to allocate more memory, and we find that objects_allocated is next_allocation, allocate another block */
	unsigned int objects_to_allocate;  /*  The number of objects we'll allocate next time we allocate another block */
};

struct memory_pooler_collection{
	/* Each memory pooler pools objects of a given size.  This collection allows us to keep track of the memory pools and the size of the objects they pool */
	struct struct_memory_pooler_ptr_list pools;  /* Pointers to all our memory pools, each for a different size object */
	struct unsigned_int_list sizes;  /* The size of objects in each memory pool */
};

void memory_pooler_create(struct memory_pooler *, unsigned int);
void memory_pooler_destroy(struct memory_pooler *);
void * memory_pooler_malloc(struct memory_pooler *);
void memory_pooler_free(struct memory_pooler *, void *);

void memory_pooler_collection_create(struct memory_pooler_collection *);
void memory_pooler_collection_destroy(struct memory_pooler_collection *);
struct memory_pooler * memory_pooler_collection_get_pool(struct memory_pooler_collection *, unsigned int);


#endif
