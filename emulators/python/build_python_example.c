#include "../../compiler_interface.h"

void build_tests(void);

void register_libc_objects(struct build_state *);
void register_data_structures_objects(struct build_state *);
void register_builtin_objects(struct build_state *);
void register_compiler_objects(struct build_state *);
void register_filesystem_objects(struct build_state *);
void register_kernel_objects(struct build_state *);

void build_tests(void){
	struct build_state * state = create_build_state();
	register_libc_objects(state);
	register_builtin_objects(state);
	register_data_structures_objects(state);
	register_compiler_objects(state);
	register_filesystem_objects(state);
	register_kernel_objects(state);

	construct_build_target(state, "emulators/python/op_cpu_package/python_l0_module.py");

	destroy_build_state(state);
}

int main(void){
	build_tests();
	return 0;
}
