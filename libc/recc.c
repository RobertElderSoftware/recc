#include "../compiler_interface.h"

void register_libc_objects(struct build_state *);

void register_libc_objects(struct build_state * state){
	/*  Standard Library Objects */
	register_build_target(state, "libc/l2/putchar.l2", BUILD_TARGET_L2_FILE);

        register_c_to_l2(state, "libc/string");
        register_c_to_l2(state, "libc/printf");
        register_c_to_l2(state, "libc/malloc");
        register_c_to_l2(state, "libc/assert");
        register_c_to_l2(state, "libc/fopen");
        register_c_to_l2(state, "libc/filesystem");
        register_c_to_l2(state, "libc/getcwd");
}
