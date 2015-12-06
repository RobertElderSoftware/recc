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

#include "../../compiler_interface.h"

void build_tests(void);

int main(void){
	struct build_state * state = create_build_state();
	register_build_target(state, "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h",  BUILD_TARGET_C_FILE);
	register_build_target(state, "demos/brainfuck-cpp/result.i",  BUILD_TARGET_PREPROCESSED_FILE);
        register_dependency(state, "demos/brainfuck-cpp/result.i", "demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h");
	construct_build_target(state, "demos/brainfuck-cpp/result.i");
	destroy_build_state(state);
	return 0;
}
