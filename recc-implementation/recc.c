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

#include "compiler_interface.h"

void register_compiler_objects(struct build_state *);

void register_compiler_objects(struct build_state * state){
	/*  Implemention of emulated ext like filesystem */
	register_build_target(state, "recc-implementation/filesystem_impl.l2",         BUILD_TARGET_FILESYSTEM_IMPLEMENTATION);
	/*  Parts of the compiler */
	register_c_to_l2(state, "recc-implementation/io");
	register_c_to_l2(state, "recc-implementation/preprocessor");
	register_c_to_l2(state, "recc-implementation/code_generator");
	register_c_to_l2(state, "recc-implementation/parser");
	register_c_to_l2(state, "recc-implementation/linker");
	register_c_to_l2(state, "recc-implementation/lexer");
	register_c_to_l2(state, "recc-implementation/heap_memory_pool");
	register_c_to_l2(state, "recc-implementation/memory_pool_collection");
}
