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

void register_builtin_objects(struct build_state *);

void register_builtin_objects(struct build_state * state){
	/*  Builtin items */
	register_build_target(state, "builtin/l2/memory_mapped_registers.l2", BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/stack.l2",                   BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/mainstub.l2",                BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/mainstub_div_zero_test.l2",  BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/mainstub_argv_indirect.l2",  BUILD_TARGET_L2_FILE);
	register_build_target(state, "builtin/l2/putchar_busy.l2",            BUILD_TARGET_L2_FILE);

	register_c_to_l2(state, "builtin/signed_division");
	register_c_to_l2(state, "builtin/signed_greater_than_or_equal_to");
	register_c_to_l2(state, "builtin/signed_less_than_or_equal_to");
	register_c_to_l2(state, "builtin/signed_greater_than");
	register_c_to_l2(state, "builtin/signed_less_than");
	register_c_to_l2(state, "builtin/signed_modulo");
	register_c_to_l2(state, "builtin/print_decimal");
	register_c_to_l2(state, "builtin/print_hexadecimal");
	register_c_to_l2(state, "builtin/print_string");
	register_c_to_l2(state, "builtin/print_buff_add");
	register_c_to_l2(state, "builtin/print_bits");
	register_c_to_l2(state, "builtin/random");
	register_c_to_l2(state, "builtin/extern_test");
	register_c_to_l2(state, "builtin/main_argv_indirect");
	register_c_to_l2(state, "builtin/main_div_zero_test");
	register_c_to_l2(state, "builtin/c99libc_in_c89");

	register_build_target(state, "builtin/libbuiltin.l2", BUILD_TARGET_L2_FILE);

        register_dependency(state, "builtin/libbuiltin.l2", "builtin/c99libc_in_c89.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_division.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_greater_than_or_equal_to.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_less_than_or_equal_to.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_greater_than.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_less_than.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/signed_modulo.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_decimal.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_hexadecimal.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_string.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_buff_add.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/print_bits.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/l2/putchar_busy.l2");
        register_dependency(state, "builtin/libbuiltin.l2", "builtin/random.l2");
}
