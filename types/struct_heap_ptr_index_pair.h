#ifndef __TYPES_struct_heap_ptr_index_pair_H__
#define __TYPES_struct_heap_ptr_index_pair_H__
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

/*  This struct is used to associate a pointer to a specific memory pool.  The pointer represents the last
    allocatable pointer for a specific contiguous region inside the memory pool.  The index lets us know which
    pool it came from, since each pool only gives us pointers to blocks of a certain size.

*/

struct heap_ptr_index_pair{
	void * p;               /*  Pointer to last returnable address for a specific contiguous memory region in the pool */
	unsigned int i;         /*  Index of that pool */
};

#endif
