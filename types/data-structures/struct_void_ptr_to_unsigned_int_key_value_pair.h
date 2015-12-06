#ifndef __TYPES_DATA_STRUCTURES_struct_void_ptr_to_unsigned_int_key_value_pair_H__
#define __TYPES_DATA_STRUCTURES_struct_void_ptr_to_unsigned_int_key_value_pair_H__
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

struct void_ptr_to_unsigned_int_key_value_pair{
	void * key;
	unsigned int value;
	unsigned int pad;
};

#endif
