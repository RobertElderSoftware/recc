#include "../compiler_interface.h"

void register_builtin_objects(struct build_state *);

void register_builtin_objects(struct build_state * state){
	/*  Builtin items */
	register_build_target(state, "builtin/l2/memory_mapped_registers.l2", BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/stack.l2",                   BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/mainstub.l2",                BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/mainstub_div_zero_test.l2",  BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/mainstub_argv_indirect.l2",  BUILD_TARGET_L2_FILE);

        register_c_to_l2(state, "builtin/signed_division");
        register_c_to_l2(state, "builtin/signed_greater_than_or_equal_to");
        register_c_to_l2(state, "builtin/signed_less_than_or_equal_to");
        register_c_to_l2(state, "builtin/signed_greater_than");
        register_c_to_l2(state, "builtin/signed_less_than");
        register_c_to_l2(state, "builtin/signed_modulo");
        register_c_to_l2(state, "builtin/print_decimal");
        register_c_to_l2(state, "builtin/print_bits");
        register_c_to_l2(state, "builtin/random");
        register_c_to_l2(state, "builtin/extern_test");
        register_c_to_l2(state, "builtin/main_argv_indirect");
        register_c_to_l2(state, "builtin/main_div_zero_test");
}
