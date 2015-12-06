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

#include "../recc-implementation/compiler_interface.h"

void register_libc_objects(struct build_state *);

void register_libc_objects(struct build_state * state){
	/*  Standard Library Objects */
        register_c_to_l2(state, "libc/string");
        register_c_to_l2(state, "libc/printf");
        register_c_to_l2(state, "libc/malloc");
        register_c_to_l2(state, "libc/assert");
        register_c_to_l2(state, "libc/fopen");
        register_c_to_l2(state, "libc/filesystem");
        register_c_to_l2(state, "libc/getcwd");
        register_c_to_l2(state, "libc/putchar");

	register_build_target(state, "libc/libc.l2", BUILD_TARGET_L2_FILE);

        register_dependency(state, "libc/libc.l2", "libc/string.l2");
        register_dependency(state, "libc/libc.l2", "libc/printf.l2");
        register_dependency(state, "libc/libc.l2", "libc/malloc.l2");
        register_dependency(state, "libc/libc.l2", "libc/assert.l2");
        register_dependency(state, "libc/libc.l2", "libc/fopen.l2");
        register_dependency(state, "libc/libc.l2", "libc/getcwd.l2");
        register_dependency(state, "libc/libc.l2", "libc/filesystem.l2");
        register_dependency(state, "libc/libc.l2", "libc/putchar.l2");
}
