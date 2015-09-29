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

#include "../../recc-implementation/compiler_interface.h"

void build_tests(void);

void register_libc_objects(struct build_state *);
void register_data_structures_objects(struct build_state *);
void register_builtin_objects(struct build_state *);
void register_compiler_objects(struct build_state *);
void register_kernel_objects(struct build_state *);

void build_tests(void){
	struct build_state * state = create_build_state();
	register_libc_objects(state);
	register_builtin_objects(state);
	register_data_structures_objects(state);
	register_compiler_objects(state);
	register_kernel_objects(state);

	construct_build_target(state, "emulators/python/op_cpu_package/python_l0_module.py");

	destroy_build_state(state);
}

int main(void){
	build_tests();
	return 0;
}
