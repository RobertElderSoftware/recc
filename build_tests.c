#include "compiler_interface.h"

void build_tests(void);

void register_libc_objects(struct build_state *);
void register_data_structures_objects(struct build_state *);
void register_builtin_objects(struct build_state *);
void register_compiler_objects(struct build_state *);
void register_filesystem_objects(struct build_state *);
void register_kernel_objects(struct build_state *);
void register_test_objects(struct build_state *);

int main(void){
	struct build_state * state = create_build_state();
	register_libc_objects(state);
	register_builtin_objects(state);
	register_data_structures_objects(state);
	register_compiler_objects(state);
	register_filesystem_objects(state);
	register_kernel_objects(state);
	register_test_objects(state);

	/* Tests */
	construct_build_target(state, "test/c89/preprocessor_test.l0.js");
	construct_build_target(state, "test/c89/filesystem.l0.js");
	construct_build_target(state, "test/c89/basic-operations.l0.js");
	construct_build_target(state, "test/c89/basic-putchar-aa.l0.js");
	construct_build_target(state, "test/c89/div-zero.l0.js");
	construct_build_target(state, "test/c89/basic-putchar-a.l0.js");
	construct_build_target(state, "test/c89/basic-putchar-b.l0.js");
	construct_build_target(state, "test/c89/empty-main-return-zero.l0.js");
	construct_build_target(state, "test/c89/for-loop.l0.js");
	construct_build_target(state, "test/c89/ignore-local.l0.js");
	construct_build_target(state, "test/c89/many-types.l0.js");
	construct_build_target(state, "test/c89/nested-putchar-a.l0.js");
	construct_build_target(state, "test/c89/nested-putchar-a-param-ignored.l0.js");
	construct_build_target(state, "test/c89/nested-putchar-a-param-used.l0.js");
	construct_build_target(state, "test/c89/print.l0.js");
	construct_build_target(state, "test/c89/putchar-return.l0.js");
	construct_build_target(state, "test/c89/string-literal.l0.js");
	construct_build_target(state, "test/c89/use-array.l0.js");
	construct_build_target(state, "test/c89/use-local.l0.js");
	construct_build_target(state, "test/c89/use-reference.l0.js");
	construct_build_target(state, "test/c89/va_list_call.l0.js");
	construct_build_target(state, "test/c89/while-loop.l0.js");
	construct_build_target(state, "test/c89/typedef.l0.js");
	construct_build_target(state, "test/c89/exotic-declarators.l0.js");
	construct_build_target(state, "test/c89/constants.l0.js");
	construct_build_target(state, "test/c89/malloc-test.l0.js");
	construct_build_target(state, "test/c89/main_with_parameters.l0.js");

	construct_build_target(state, "test/c99/preprocessor_test.l0.js");

	destroy_build_state(state);
	return 0;
}
