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

#include "../recc-implementation/compiler_interface_phase_3.h"

void register_kernel_objects(struct build_state *);

void register_kernel_objects(struct build_state * state){

        register_entity(state, "kernel/kernel_symbols.l1", ENTITY_TYPE_L1_FILE);
        add_entity_attribute(state, "kernel/kernel_symbols.l1", "page_align_permission_regions", "true");
        add_entity_attribute(state, "kernel/kernel_symbols.l1", "only_metadata", "true");

        register_entity(state, "kernel/kernel_symbols.l0.c", ENTITY_TYPE_L0_FILE);
        add_entity_attribute(state, "kernel/kernel_symbols.l0.c", "variable_name", "data");
        add_entity_attribute(state, "kernel/kernel_symbols.l0.c", "language", "c");
        register_entity(state, "kernel/kernel_symbols.l0.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/kernel_symbols.l0.l2", ENTITY_TYPE_L2_FILE);
        add_entity_attribute(state, "kernel/kernel_symbols.l0.l2", "relocation", "END"); /*  Put this object on the end so it doesn't affect previous symbol offsets */

	register_dependency(state, "kernel/kernel_symbols.l0.c", "kernel/kernel_symbols.l1");
	register_dependency(state, "kernel/kernel_symbols.l0.i", "kernel/kernel_symbols.l0.c");
	register_dependency(state, "kernel/kernel_symbols.l0.l2", "kernel/kernel_symbols.l0.i");

	/*  Kernel files */
	register_entity(state, "kernel/kernel.sym", ENTITY_TYPE_SYMBOL_FILE);
	register_entity(state, "kernel/kernel_symbols.sym", ENTITY_TYPE_SYMBOL_FILE);
        register_entity(state, "kernel/kernel.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/kernel.l0.js", ENTITY_TYPE_L0_FILE);
        add_entity_attribute(state, "test/kernel.l0.js", "variable_name", "jsonpCallback");
        add_entity_attribute(state, "test/kernel.l0.js", "language", "jsonp");
        register_entity(state, "emulators/c/kernel.l0.c", ENTITY_TYPE_L0_FILE);
        add_entity_attribute(state, "emulators/c/kernel.l0.c", "variable_name", "data");
        add_entity_attribute(state, "emulators/c/kernel.l0.c", "language", "c");
        register_entity(state, "emulators/python/op_cpu_package/python_l0_module.py", ENTITY_TYPE_L0_FILE);
        add_entity_attribute(state, "emulators/python/op_cpu_package/python_l0_module.py", "variable_name", "OpCPUData");
        add_entity_attribute(state, "emulators/python/op_cpu_package/python_l0_module.py", "language", "python");
        register_entity(state, "emulators/java/OpCPU/KernelData.java", ENTITY_TYPE_L0_FILE);
        add_entity_attribute(state, "emulators/java/OpCPU/KernelData.java", "variable_name", "KernelData");
        add_entity_attribute(state, "emulators/java/OpCPU/KernelData.java", "language", "java");
        register_dependency(state, "kernel/kernel.sym", "kernel/kernel.l1");
        register_dependency(state, "kernel/kernel_symbols.sym", "kernel/kernel_symbols.l1");
        register_dependency(state, "test/kernel.l0.js", "kernel/kernel.l1");
        register_dependency(state, "emulators/c/kernel.l0.c", "kernel/kernel.l1");
        register_dependency(state, "emulators/python/op_cpu_package/python_l0_module.py", "kernel/kernel.l1");
        register_dependency(state, "emulators/java/OpCPU/KernelData.java", "kernel/kernel.l1");

	register_entity(state, "kernel/l2/kernel_interface.l2", ENTITY_TYPE_L2_FILE);
	add_entity_attribute(state, "kernel/l2/kernel_interface.l2", "terminal", "true");
	register_entity(state, "kernel/l2/putchar_nobusy.l2", ENTITY_TYPE_L2_FILE);
	add_entity_attribute(state, "kernel/l2/putchar_nobusy.l2", "terminal", "true");
	register_entity(state, "kernel/l2/getchar_nobusy.l2", ENTITY_TYPE_L2_FILE);
	add_entity_attribute(state, "kernel/l2/getchar_nobusy.l2", "terminal", "true");
	register_entity(state, "kernel/l2/putchar_busy.l2", ENTITY_TYPE_L2_FILE);
	add_entity_attribute(state, "kernel/l2/putchar_busy.l2", "terminal", "true");

        register_entity(state, "kernel/filesystem.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/filesystem.c", "terminal", "true");
        register_entity(state, "kernel/putchar.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/putchar.c", "terminal", "true");
        register_entity(state, "kernel/main.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/main.c", "terminal", "true");
        register_entity(state, "kernel/queue.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/queue.c", "terminal", "true");
        register_entity(state, "kernel/kernel_state.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/kernel_state.c", "terminal", "true");
        register_entity(state, "kernel/kernel_impl.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/kernel_impl.c", "terminal", "true");
        register_entity(state, "kernel/level_1_page_table_entries.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/level_1_page_table_entries.c", "terminal", "true");
        register_entity(state, "kernel/user_proc.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/user_proc.c", "terminal", "true");
        register_entity(state, "kernel/printf_busy.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "kernel/printf_busy.c", "terminal", "true");

        register_entity(state, "kernel/filesystem.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/putchar.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/main.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/queue.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/kernel_state.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/kernel_impl.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/level_1_page_table_entries.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/user_proc.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "kernel/printf_busy.i", ENTITY_TYPE_PREPROCESSED_FILE);

        register_dependency(state, "kernel/filesystem.i",                   "kernel/filesystem.c");
        register_dependency(state, "kernel/putchar.i",                      "kernel/putchar.c");
        register_dependency(state, "kernel/main.i",                         "kernel/main.c");
        register_dependency(state, "kernel/queue.i",                        "kernel/queue.c");
        register_dependency(state, "kernel/kernel_state.i",                 "kernel/kernel_state.c");
        register_dependency(state, "kernel/kernel_impl.i",                  "kernel/kernel_impl.c");
        register_dependency(state, "kernel/level_1_page_table_entries.i",   "kernel/level_1_page_table_entries.c");
        register_dependency(state, "kernel/user_proc.i",                    "kernel/user_proc.c");
        register_dependency(state, "kernel/printf_busy.i",                  "kernel/printf_busy.c");


        register_dependency(state, "kernel/filesystem.l2",                    "kernel/filesystem.i");
        register_dependency(state, "kernel/putchar.l2",                       "kernel/putchar.i");
        register_dependency(state, "kernel/main.l2",                          "kernel/main.i");
        register_dependency(state, "kernel/queue.l2",                         "kernel/queue.i");
        register_dependency(state, "kernel/kernel_state.l2",                  "kernel/kernel_state.i");
        register_dependency(state, "kernel/kernel_impl.l2",                   "kernel/kernel_impl.i");
        register_dependency(state, "kernel/level_1_page_table_entries.l2",    "kernel/level_1_page_table_entries.i");
        register_dependency(state, "kernel/user_proc.l2",                     "kernel/user_proc.i");
        register_dependency(state, "kernel/printf_busy.l2",                   "kernel/printf_busy.i");

        register_entity(state, "kernel/filesystem.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "kernel/putchar.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "kernel/main.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "kernel/queue.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "kernel/kernel_state.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "kernel/kernel_impl.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "kernel/level_1_page_table_entries.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "kernel/user_proc.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "kernel/printf_busy.l2", ENTITY_TYPE_L2_FILE);


	register_dependency(state, "kernel/kernel.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "kernel/kernel.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "kernel/kernel.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/libdata-structures.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/memory_pool_collection.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/code_generator.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/preprocessor.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/parser.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/heap_memory_pool.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/lexer.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/io.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/regex_engine.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/type_engine.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/replace_tool.l2");
	register_dependency(state, "kernel/kernel.l1", "recc-implementation/binary_exponential_buffer.l2");
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
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/libdata-structures.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/memory_pool_collection.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/code_generator.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/preprocessor.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/parser.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/heap_memory_pool.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/lexer.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/io.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/regex_engine.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/type_engine.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/replace_tool.l2");
	register_dependency(state, "kernel/kernel_symbols.l1", "recc-implementation/binary_exponential_buffer.l2");
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
