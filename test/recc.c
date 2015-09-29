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

void register_test_objects(struct build_state *);

void register_test_objects(struct build_state * state){
	/*  Test cases */
        register_c_to_l1(state, "test/c89/basic-operations");
        register_c_to_l1(state, "test/c89/basic-putchar-aa");
        register_c_to_l1(state, "test/c89/div-zero");
        register_c_to_l1(state, "test/c89/preprocessor_test");
        register_c_to_l1(state, "test/c89/filesystem");
        register_c_to_l1(state, "test/c89/basic-putchar-a");
        register_c_to_l1(state, "test/c89/basic-putchar-b");
        register_c_to_l1(state, "test/c89/empty-main-return-zero");
        register_c_to_l1(state, "test/c89/for-loop");
        register_c_to_l1(state, "test/c89/ignore-local");
        register_c_to_l1(state, "test/c89/many-types");
        register_c_to_l1(state, "test/c89/nested-putchar-a");
        register_c_to_l1(state, "test/c89/nested-putchar-a-param-ignored");
        register_c_to_l1(state, "test/c89/nested-putchar-a-param-used");
        register_c_to_l1(state, "test/c89/print");
        register_c_to_l1(state, "test/c89/putchar-return");
        register_c_to_l1(state, "test/c89/string-literal");
        register_c_to_l1(state, "test/c89/use-array");
        register_c_to_l1(state, "test/c89/use-local");
        register_c_to_l1(state, "test/c89/use-reference");
        register_c_to_l1(state, "test/c89/va_list_call");
        register_c_to_l1(state, "test/c89/while-loop");
        register_c_to_l1(state, "test/c89/typedef");
        register_c_to_l1(state, "test/c89/exotic-declarators");
        register_c_to_l1(state, "test/c89/constants");
        register_c_to_l1(state, "test/c89/malloc-test");
        register_c_to_l1(state, "test/c89/main_with_parameters");

        register_c_to_l1(state, "test/c99/preprocessor_test");
        register_c_to_l1(state, "test/c99/snprintf_test");

        register_l0_file(state, "test/c89/basic-operations.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/basic-putchar-aa.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/div-zero.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/preprocessor_test.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/filesystem.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/basic-putchar-a.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/basic-putchar-b.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/empty-main-return-zero.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/for-loop.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/ignore-local.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/many-types.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/nested-putchar-a.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/nested-putchar-a-param-ignored.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/nested-putchar-a-param-used.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/print.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/putchar-return.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/string-literal.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/use-array.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/use-local.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/use-reference.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/va_list_call.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/while-loop.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/typedef.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/exotic-declarators.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/constants.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/malloc-test.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c89/main_with_parameters.l0.js", "jsonpCallback", "jsonp");

        register_l0_file(state, "test/c99/preprocessor_test.l0.js", "jsonpCallback", "jsonp");
        register_l0_file(state, "test/c99/snprintf_test.l0.js", "jsonpCallback", "jsonp");

        register_dependency(state, "test/c89/basic-operations.l0.js", "test/c89/basic-operations.l1");
        register_dependency(state, "test/c89/basic-putchar-aa.l0.js", "test/c89/basic-putchar-aa.l1");
        register_dependency(state, "test/c89/div-zero.l0.js", "test/c89/div-zero.l1");
        register_dependency(state, "test/c89/preprocessor_test.l0.js", "test/c89/preprocessor_test.l1");
        register_dependency(state, "test/c89/filesystem.l0.js", "test/c89/filesystem.l1");
        register_dependency(state, "test/c89/basic-putchar-a.l0.js", "test/c89/basic-putchar-a.l1");
        register_dependency(state, "test/c89/basic-putchar-b.l0.js", "test/c89/basic-putchar-b.l1");
        register_dependency(state, "test/c89/empty-main-return-zero.l0.js", "test/c89/empty-main-return-zero.l1");
        register_dependency(state, "test/c89/for-loop.l0.js", "test/c89/for-loop.l1");
        register_dependency(state, "test/c89/ignore-local.l0.js", "test/c89/ignore-local.l1");
        register_dependency(state, "test/c89/many-types.l0.js", "test/c89/many-types.l1");
        register_dependency(state, "test/c89/nested-putchar-a.l0.js", "test/c89/nested-putchar-a.l1");
        register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l0.js", "test/c89/nested-putchar-a-param-ignored.l1");
        register_dependency(state, "test/c89/nested-putchar-a-param-used.l0.js", "test/c89/nested-putchar-a-param-used.l1");
        register_dependency(state, "test/c89/print.l0.js", "test/c89/print.l1");
        register_dependency(state, "test/c89/putchar-return.l0.js", "test/c89/putchar-return.l1");
        register_dependency(state, "test/c89/string-literal.l0.js", "test/c89/string-literal.l1");
        register_dependency(state, "test/c89/use-array.l0.js", "test/c89/use-array.l1");
        register_dependency(state, "test/c89/use-local.l0.js", "test/c89/use-local.l1");
        register_dependency(state, "test/c89/use-reference.l0.js", "test/c89/use-reference.l1");
        register_dependency(state, "test/c89/va_list_call.l0.js", "test/c89/va_list_call.l1");
        register_dependency(state, "test/c89/while-loop.l0.js", "test/c89/while-loop.l1");
        register_dependency(state, "test/c89/typedef.l0.js", "test/c89/typedef.l1");
        register_dependency(state, "test/c89/exotic-declarators.l0.js", "test/c89/exotic-declarators.l1");
        register_dependency(state, "test/c89/constants.l0.js", "test/c89/constants.l1");
        register_dependency(state, "test/c89/malloc-test.l0.js", "test/c89/malloc-test.l1");
        register_dependency(state, "test/c89/main_with_parameters.l0.js", "test/c89/main_with_parameters.l1");

        register_dependency(state, "test/c99/preprocessor_test.l0.js", "test/c99/preprocessor_test.l1");
        register_dependency(state, "test/c99/snprintf_test.l0.js", "test/c99/snprintf_test.l1");

	/* Link final l1 files */
	register_dependency(state, "test/c89/filesystem.l1", "data-structures/libdata-structures.l2");
	register_dependency(state, "test/c89/filesystem.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/memory_pool_collection.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/code_generator.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/preprocessor.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/parser.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/heap_memory_pool.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/lexer.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/io.l2");
	register_dependency(state, "test/c89/filesystem.l1", "libc/libc.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/filesystem_impl.l2");
	register_dependency(state, "test/c89/filesystem.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/filesystem.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/filesystem.l1", "builtin/l2/mainstub.l2");

	register_dependency(state, "test/c89/basic-operations.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/basic-putchar-aa.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/basic-putchar-aa.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/basic-putchar-aa.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/basic-putchar-aa.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/basic-putchar-aa.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/div-zero.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/div-zero.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/div-zero.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/div-zero.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/div-zero.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/div-zero.l1", "builtin/main_div_zero_test.l2");
	register_dependency(state, "test/c89/div-zero.l1", "builtin/l2/mainstub_div_zero_test.l2");
	register_dependency(state, "test/c89/div-zero.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/preprocessor_test.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/basic-putchar-a.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/basic-putchar-a.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/basic-putchar-a.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/basic-putchar-a.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/basic-putchar-a.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/basic-putchar-b.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/basic-putchar-b.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/basic-putchar-b.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/basic-putchar-b.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/basic-putchar-b.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/empty-main-return-zero.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/empty-main-return-zero.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/empty-main-return-zero.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/empty-main-return-zero.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/empty-main-return-zero.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/for-loop.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/for-loop.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/for-loop.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/for-loop.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/for-loop.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/ignore-local.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/ignore-local.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/ignore-local.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/ignore-local.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/ignore-local.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/many-types.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/many-types.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/many-types.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/many-types.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/many-types.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/nested-putchar-a.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/nested-putchar-a.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/nested-putchar-a.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/nested-putchar-a.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/nested-putchar-a.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "builtin/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/print.l1", "data-structures/unsigned_int_binary_search.l2");
	register_dependency(state, "test/c89/print.l1", "libc/malloc.l2");
	register_dependency(state, "test/c89/print.l1", "data-structures/unsigned_int_merge_sort.l2");
	register_dependency(state, "test/c89/print.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/print.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/print.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/print.l1", "builtin/extern_test.l2");
	register_dependency(state, "test/c89/print.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/print.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/print.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/print.l1", "libc/printf.l2");

	register_dependency(state, "test/c89/putchar-return.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/putchar-return.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/putchar-return.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/putchar-return.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/putchar-return.l1", "builtin/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/string-literal.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/string-literal.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/string-literal.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/string-literal.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/string-literal.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/string-literal.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/string-literal.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/use-array.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/use-array.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/use-array.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/use-array.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/use-array.l1", "builtin/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/use-local.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/use-local.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/use-local.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/use-local.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/use-local.l1", "builtin/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/use-reference.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/use-reference.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/use-reference.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/use-reference.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/use-reference.l1", "builtin/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/va_list_call.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/va_list_call.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/va_list_call.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/va_list_call.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/va_list_call.l1", "builtin/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/while-loop.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/while-loop.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/while-loop.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/while-loop.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/while-loop.l1", "builtin/signed_less_than.l2");
	register_dependency(state, "test/c89/while-loop.l1", "builtin/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/typedef.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/typedef.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/typedef.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/typedef.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/typedef.l1", "builtin/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/exotic-declarators.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/exotic-declarators.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/exotic-declarators.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/exotic-declarators.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/exotic-declarators.l1", "builtin/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/constants.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/constants.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/constants.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/constants.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/constants.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/constants.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/constants.l1", "builtin/l2/mainstub.l2");

	register_dependency(state, "test/c89/malloc-test.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "libc/malloc.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "builtin/l2/mainstub.l2");

	register_dependency(state, "test/c89/main_with_parameters.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "builtin/main_argv_indirect.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "builtin/l2/mainstub_argv_indirect.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "libc/printf.l2");

	register_dependency(state, "test/c99/preprocessor_test.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "libc/assert.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "libc/printf.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "libc/malloc.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "libc/putchar.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "builtin/l2/mainstub.l2");

	register_dependency(state, "test/c99/snprintf_test.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "libc/assert.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "libc/printf.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "libc/malloc.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "libc/putchar.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "builtin/l2/stack.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "builtin/l2/mainstub.l2");
}
