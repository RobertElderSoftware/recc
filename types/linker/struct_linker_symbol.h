#ifndef __TYPES_LINKER_struct_linker_symbol_h__
#define __TYPES_LINKER_struct_linker_symbol_h__
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

struct linker_symbol{
	unsigned int is_implemented;
	unsigned int is_required;
	unsigned int is_external;
	unsigned int instruction_index; /* The index of the instruction in its parent linker object. */
	unsigned int observed_as_implemented;  /*  indicates if a corresponding label for that linker object has been found in the file */
	unsigned int pad;
	struct linker_object * parent_linker_object;  /*  External symbols need to remember which linker object they belong to. */
};

#endif
