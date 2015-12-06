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

void register_kernel_objects(struct build_state *);

void register_kernel_objects(struct build_state * state){

	/*  Kernel files */
	register_build_target(state, "kernel/kernel.sym", BUILD_TARGET_SYMBOL_FILE);
	register_build_target(state, "kernel/kernel.l1", BUILD_TARGET_L1_FILE);
        register_l0_file(state, "test/kernel.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "emulators/c/kernel.l0.c", "data", "c");
        register_l0_file(state, "emulators/python/op_cpu_package/python_l0_module.py", "OpCPUData", "python");
        register_l0_file(state, "emulators/java/OpCPU/KernelData.java", "KernelData", "java");
        register_dependency(state, "kernel/kernel.sym", "kernel/kernel.l1");
        register_dependency(state, "test/kernel.l0.js", "kernel/kernel.l1");
        register_dependency(state, "emulators/c/kernel.l0.c", "kernel/kernel.l1");
        register_dependency(state, "emulators/python/op_cpu_package/python_l0_module.py", "kernel/kernel.l1");
        register_dependency(state, "emulators/java/OpCPU/KernelData.java", "kernel/kernel.l1");

	register_build_target(state, "kernel/l2/kernel_interface.l2", BUILD_TARGET_L2_FILE);
	register_build_target(state, "kernel/l2/putchar_nobusy.l2", BUILD_TARGET_L2_FILE);
	register_build_target(state, "kernel/l2/getchar_nobusy.l2", BUILD_TARGET_L2_FILE);
        register_c_to_l2(state, "kernel/filesystem");
        register_c_to_l2(state, "kernel/putchar");
        register_c_to_l2(state, "kernel/main");
        register_c_to_l2(state, "kernel/queue");
        register_c_to_l2(state, "kernel/kernel_state");
        register_c_to_l2(state, "kernel/kernel_impl");
        register_c_to_l2(state, "kernel/user_proc");

	register_dependency(state, "kernel/kernel.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "kernel/kernel.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "kernel/kernel.l1", "builtin/l2/stack.l2");
	register_dependency(state, "kernel/kernel.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "kernel/kernel.l1", "data-structures/libdata-structures.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/memory_pool_collection.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/code_generator.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/preprocessor.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/parser.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/heap_memory_pool.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/lexer.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/io.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/getcwd.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/string.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/filesystem.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/filesystem_impl.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/fopen.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/assert.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/printf.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/malloc.l2");

	register_dependency(state, "kernel/kernel.l1", "kernel/filesystem.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/putchar.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/main.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/queue.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/kernel_state.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/kernel_impl.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/user_proc.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/l2/kernel_interface.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/l2/putchar_nobusy.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/l2/getchar_nobusy.l2");
}
