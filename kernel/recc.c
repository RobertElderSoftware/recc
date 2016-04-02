/*
    Copyright 2016 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/

#include "../recc-implementation/compiler_interface.h"

void register_kernel_objects(struct build_state *);

void register_kernel_objects(struct build_state * state){

        register_l1_file(state, "kernel/kernel_symbols.l1", 1, 1);
        register_l0_file(state, "kernel/kernel_symbols.l0.c", "data", "c");
        register_build_target(state, "kernel/kernel_symbols.l0.i", BUILD_TARGET_PREPROCESSED_FILE);
        register_l2_file(state, "kernel/kernel_symbols.l0.l2", "END"); /*  Put this object on the end so it doesn't affect previous symbol offsets */

	register_dependency(state, "kernel/kernel_symbols.l0.c", "kernel/kernel_symbols.l1");
	register_dependency(state, "kernel/kernel_symbols.l0.i", "kernel/kernel_symbols.l0.c");
	register_dependency(state, "kernel/kernel_symbols.l0.l2", "kernel/kernel_symbols.l0.i");

	/*  Kernel files */
	register_build_target(state, "kernel/kernel.sym", BUILD_TARGET_SYMBOL_FILE);
	register_build_target(state, "kernel/kernel_symbols.sym", BUILD_TARGET_SYMBOL_FILE);
        register_l1_file(state, "kernel/kernel.l1", 0, 0);
        register_l0_file(state, "test/kernel.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "emulators/c/kernel.l0.c", "data", "c");
        register_l0_file(state, "emulators/python/op_cpu_package/python_l0_module.py", "OpCPUData", "python");
        register_l0_file(state, "emulators/java/OpCPU/KernelData.java", "KernelData", "java");
        register_dependency(state, "kernel/kernel.sym", "kernel/kernel.l1");
        register_dependency(state, "kernel/kernel_symbols.sym", "kernel/kernel_symbols.l1");
        register_dependency(state, "test/kernel.l0.js", "kernel/kernel.l1");
        register_dependency(state, "emulators/c/kernel.l0.c", "kernel/kernel.l1");
        register_dependency(state, "emulators/python/op_cpu_package/python_l0_module.py", "kernel/kernel.l1");
        register_dependency(state, "emulators/java/OpCPU/KernelData.java", "kernel/kernel.l1");

	register_l2_file(state, "kernel/l2/kernel_interface.l2", "RELOCATABLE");
	register_l2_file(state, "kernel/l2/putchar_nobusy.l2", "RELOCATABLE");
	register_l2_file(state, "kernel/l2/getchar_nobusy.l2", "RELOCATABLE");
	register_l2_file(state, "kernel/l2/putchar_busy.l2", "RELOCATABLE");
        register_c_to_l2(state, "kernel/filesystem");
        register_c_to_l2(state, "kernel/putchar");
        register_c_to_l2(state, "kernel/main");
        register_c_to_l2(state, "kernel/queue");
        register_c_to_l2(state, "kernel/kernel_state");
        register_c_to_l2(state, "kernel/kernel_impl");
        register_c_to_l2(state, "kernel/level_1_page_table_entries");
        register_c_to_l2(state, "kernel/user_proc");
        register_c_to_l2(state, "kernel/printf_busy");

	register_dependency(state, "kernel/kernel.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "kernel/kernel.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "kernel/kernel.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "kernel/kernel.l1", "data-structures/libdata-structures.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/memory_pool_collection.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/code_generator.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/preprocessor.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/parser.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/heap_memory_pool.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/lexer.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/io.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/regex_engine.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/type_engine.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/getcwd.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/string.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/filesystem.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/filesystem_impl.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/fopen.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/assert.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/printf.l2");
	register_dependency(state, "kernel/kernel.l1", "libc/malloc.l2");

	register_dependency(state, "kernel/kernel.l1", "kernel/printf_busy.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/filesystem.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/putchar.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/main.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/queue.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/kernel_state.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/kernel_impl.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/level_1_page_table_entries.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/user_proc.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/l2/kernel_interface.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/l2/putchar_nobusy.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/l2/getchar_nobusy.l2");
	register_dependency(state, "kernel/kernel.l1", "kernel/l2/putchar_busy.l2");

	/*  List of symbols and regions in the kernel, to be compiled into the kernel */
	register_dependency(state, "kernel/kernel.l1", "kernel/kernel_symbols.l0.l2");

	register_dependency(state, "kernel/kernel_symbols.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "data-structures/libdata-structures.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/memory_pool_collection.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/code_generator.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/preprocessor.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/parser.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/heap_memory_pool.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/lexer.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/io.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/regex_engine.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/type_engine.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "libc/getcwd.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "libc/string.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "libc/filesystem.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/filesystem_impl.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "libc/fopen.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "libc/assert.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "libc/printf.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "libc/malloc.l2");

	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/printf_busy.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/filesystem.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/putchar.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/main.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/queue.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/kernel_state.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/kernel_impl.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/level_1_page_table_entries.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/user_proc.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/l2/kernel_interface.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/l2/putchar_nobusy.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/l2/getchar_nobusy.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "kernel/l2/putchar_busy.l2");
}
