#ifndef __TYPES_DATA_STRUCTURES_struct_parser_node_memory_pool_H__
#define __TYPES_DATA_STRUCTURES_struct_parser_node_memory_pool_H__
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

#ifndef __TYPES_DATA_STRUCTURES_struct_void_ptr_list_H__
#include "struct_void_ptr_list.h"
#endif
#ifndef __TYPES_PARSER_struct_parser_node_H__
#include "../../types/parser/struct_parser_node.h"
#endif

struct struct_parser_node_memory_pool{
	void ** freed_pointers;  /* Pointers that we can just hand out next time we get a memory request */
	struct void_ptr_list block_pointers;  /* An array of pointers to blocks of object_size * 2^n */
	unsigned int object_size; /*  The size of the object (in bytes) for which we're providing this service. */
	unsigned int objects_allocated;  /*  The total number of outstanding non-freed objects */
	unsigned int next_preallocation;  /*  If we're asked to allocate more memory, and we find that objects_allocated is next_allocation, allocate another block */
	unsigned int objects_to_preallocate;  /*  The number of objects we'll allocate next time we allocate another block */
	unsigned int num_freed_pointers;  /*  The number of pointers in freed_pointers that we can just hand out for a malloc */
	unsigned int pooling_active;
};

#endif
