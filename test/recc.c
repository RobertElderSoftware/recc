/*
    Copyright 2019 Robert Elder Software Inc.
    
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

void register_test_objects(struct build_state *);

void register_test_objects(struct build_state * state){
	/*  Some required functions for tests */
	register_entity(state, "test/extras/random.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/extras/random.c", "terminal", "true");
	register_entity(state, "test/extras/extern_test.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/extras/extern_test.c", "terminal", "true");
	register_entity(state, "test/extras/main_argv_indirect.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/extras/main_argv_indirect.c", "terminal", "true");
	register_entity(state, "test/extras/main_div_zero_test.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/extras/main_div_zero_test.c", "terminal", "true");

	register_entity(state, "test/extras/random.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "test/extras/extern_test.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "test/extras/main_argv_indirect.i", ENTITY_TYPE_PREPROCESSED_FILE);
	register_entity(state, "test/extras/main_div_zero_test.i", ENTITY_TYPE_PREPROCESSED_FILE);

	register_entity(state, "test/extras/random.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "test/extras/extern_test.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "test/extras/main_argv_indirect.l2", ENTITY_TYPE_L2_FILE);
	register_entity(state, "test/extras/main_div_zero_test.l2", ENTITY_TYPE_L2_FILE);

	register_dependency(state, "test/extras/random.i", "test/extras/random.c");
	register_dependency(state, "test/extras/extern_test.i", "test/extras/extern_test.c");
	register_dependency(state, "test/extras/main_argv_indirect.i", "test/extras/main_argv_indirect.c");
	register_dependency(state, "test/extras/main_div_zero_test.i", "test/extras/main_div_zero_test.c");

	register_dependency(state, "test/extras/random.l2", "test/extras/random.i");
	register_dependency(state, "test/extras/extern_test.l2", "test/extras/extern_test.i");
	register_dependency(state, "test/extras/main_argv_indirect.l2", "test/extras/main_argv_indirect.i");
	register_dependency(state, "test/extras/main_div_zero_test.l2", "test/extras/main_div_zero_test.i");

	register_entity(state, "test/extras/l2/mainstub_div_zero_test.l2",  ENTITY_TYPE_L2_FILE);
	add_entity_attribute(state, "test/extras/l2/mainstub_div_zero_test.l2", "terminal", "true");
	register_entity(state, "test/extras/l2/mainstub_argv_indirect.l2",  ENTITY_TYPE_L2_FILE);
	add_entity_attribute(state, "test/extras/l2/mainstub_argv_indirect.l2", "terminal", "true");

	/*  Test cases */
        register_entity(state, "test/c89/basic-operations.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/basic-operations.c", "terminal", "true");
        register_entity(state, "test/c89/basic-putchar-aa.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/basic-putchar-aa.c", "terminal", "true");
        register_entity(state, "test/c89/div-zero.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/div-zero.c", "terminal", "true");
        register_entity(state, "test/c89/preprocessor_test.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/preprocessor_test.c", "terminal", "true");
        register_entity(state, "test/c89/filesystem.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/filesystem.c", "terminal", "true");
        register_entity(state, "test/c89/basic-putchar-a.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/basic-putchar-a.c", "terminal", "true");
        register_entity(state, "test/c89/basic-putchar-b.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/basic-putchar-b.c", "terminal", "true");
        register_entity(state, "test/c89/empty-main-return-zero.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/empty-main-return-zero.c", "terminal", "true");
        register_entity(state, "test/c89/for-loop.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/for-loop.c", "terminal", "true");
        register_entity(state, "test/c89/ignore-local.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/ignore-local.c", "terminal", "true");
        register_entity(state, "test/c89/many-types.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/many-types.c", "terminal", "true");
        register_entity(state, "test/c89/nested-putchar-a.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/nested-putchar-a.c", "terminal", "true");
        register_entity(state, "test/c89/nested-putchar-a-param-ignored.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/nested-putchar-a-param-ignored.c", "terminal", "true");
        register_entity(state, "test/c89/nested-putchar-a-param-used.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/nested-putchar-a-param-used.c", "terminal", "true");
        register_entity(state, "test/c89/print.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/print.c", "terminal", "true");
        register_entity(state, "test/c89/putchar-return.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/putchar-return.c", "terminal", "true");
        register_entity(state, "test/c89/string-literal.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/string-literal.c", "terminal", "true");
        register_entity(state, "test/c89/use-array.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/use-array.c", "terminal", "true");
        register_entity(state, "test/c89/use-local.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/use-local.c", "terminal", "true");
        register_entity(state, "test/c89/use-reference.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/use-reference.c", "terminal", "true");
        register_entity(state, "test/c89/va_list_call.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/va_list_call.c", "terminal", "true");
        register_entity(state, "test/c89/while-loop.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/while-loop.c", "terminal", "true");
        register_entity(state, "test/c89/typedef.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/typedef.c", "terminal", "true");
        register_entity(state, "test/c89/exotic-declarators.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/exotic-declarators.c", "terminal", "true");
        register_entity(state, "test/c89/constants.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/constants.c", "terminal", "true");
        register_entity(state, "test/c89/malloc-test.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/malloc-test.c", "terminal", "true");
        register_entity(state, "test/c89/main_with_parameters.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/main_with_parameters.c", "terminal", "true");
        register_entity(state, "test/c89/regex_engine_unit_tests.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c89/regex_engine_unit_tests.c", "terminal", "true");

        register_entity(state, "test/c89/basic-operations.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/basic-putchar-aa.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/div-zero.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/preprocessor_test.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/filesystem.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/basic-putchar-a.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/basic-putchar-b.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/empty-main-return-zero.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/for-loop.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/ignore-local.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/many-types.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/nested-putchar-a.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/nested-putchar-a-param-ignored.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/nested-putchar-a-param-used.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/print.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/putchar-return.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/string-literal.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/use-array.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/use-local.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/use-reference.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/va_list_call.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/while-loop.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/typedef.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/exotic-declarators.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/constants.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/malloc-test.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/main_with_parameters.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c89/regex_engine_unit_tests.i", ENTITY_TYPE_PREPROCESSED_FILE);

        register_dependency(state, "test/c89/basic-operations.i", "test/c89/basic-operations.c");
        register_dependency(state, "test/c89/basic-putchar-aa.i", "test/c89/basic-putchar-aa.c");
        register_dependency(state, "test/c89/div-zero.i", "test/c89/div-zero.c");
        register_dependency(state, "test/c89/preprocessor_test.i", "test/c89/preprocessor_test.c");
        register_dependency(state, "test/c89/filesystem.i", "test/c89/filesystem.c");
        register_dependency(state, "test/c89/basic-putchar-a.i", "test/c89/basic-putchar-a.c");
        register_dependency(state, "test/c89/basic-putchar-b.i", "test/c89/basic-putchar-b.c");
        register_dependency(state, "test/c89/empty-main-return-zero.i", "test/c89/empty-main-return-zero.c");
        register_dependency(state, "test/c89/for-loop.i", "test/c89/for-loop.c");
        register_dependency(state, "test/c89/ignore-local.i", "test/c89/ignore-local.c");
        register_dependency(state, "test/c89/many-types.i", "test/c89/many-types.c");
        register_dependency(state, "test/c89/nested-putchar-a.i", "test/c89/nested-putchar-a.c");
        register_dependency(state, "test/c89/nested-putchar-a-param-ignored.i", "test/c89/nested-putchar-a-param-ignored.c");
        register_dependency(state, "test/c89/nested-putchar-a-param-used.i", "test/c89/nested-putchar-a-param-used.c");
        register_dependency(state, "test/c89/print.i", "test/c89/print.c");
        register_dependency(state, "test/c89/putchar-return.i", "test/c89/putchar-return.c");
        register_dependency(state, "test/c89/string-literal.i", "test/c89/string-literal.c");
        register_dependency(state, "test/c89/use-array.i", "test/c89/use-array.c");
        register_dependency(state, "test/c89/use-local.i", "test/c89/use-local.c");
        register_dependency(state, "test/c89/use-reference.i", "test/c89/use-reference.c");
        register_dependency(state, "test/c89/va_list_call.i", "test/c89/va_list_call.c");
        register_dependency(state, "test/c89/while-loop.i", "test/c89/while-loop.c");
        register_dependency(state, "test/c89/typedef.i", "test/c89/typedef.c");
        register_dependency(state, "test/c89/exotic-declarators.i", "test/c89/exotic-declarators.c");
        register_dependency(state, "test/c89/constants.i", "test/c89/constants.c");
        register_dependency(state, "test/c89/malloc-test.i", "test/c89/malloc-test.c");
        register_dependency(state, "test/c89/main_with_parameters.i", "test/c89/main_with_parameters.c");
        register_dependency(state, "test/c89/regex_engine_unit_tests.i", "test/c89/regex_engine_unit_tests.c");

        register_entity(state, "test/c89/basic-operations.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/basic-putchar-aa.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/div-zero.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/preprocessor_test.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/filesystem.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/basic-putchar-a.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/basic-putchar-b.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/empty-main-return-zero.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/for-loop.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/ignore-local.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/many-types.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/nested-putchar-a.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/nested-putchar-a-param-ignored.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/nested-putchar-a-param-used.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/print.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/putchar-return.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/string-literal.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/use-array.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/use-local.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/use-reference.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/va_list_call.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/while-loop.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/typedef.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/exotic-declarators.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/constants.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/malloc-test.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/main_with_parameters.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c89/regex_engine_unit_tests.l2", ENTITY_TYPE_L2_FILE);

        register_dependency(state, "test/c89/basic-operations.l2", "test/c89/basic-operations.i");
        register_dependency(state, "test/c89/basic-putchar-aa.l2", "test/c89/basic-putchar-aa.i");
        register_dependency(state, "test/c89/div-zero.l2", "test/c89/div-zero.i");
        register_dependency(state, "test/c89/preprocessor_test.l2", "test/c89/preprocessor_test.i");
        register_dependency(state, "test/c89/filesystem.l2", "test/c89/filesystem.i");
        register_dependency(state, "test/c89/basic-putchar-a.l2", "test/c89/basic-putchar-a.i");
        register_dependency(state, "test/c89/basic-putchar-b.l2", "test/c89/basic-putchar-b.i");
        register_dependency(state, "test/c89/empty-main-return-zero.l2", "test/c89/empty-main-return-zero.i");
        register_dependency(state, "test/c89/for-loop.l2", "test/c89/for-loop.i");
        register_dependency(state, "test/c89/ignore-local.l2", "test/c89/ignore-local.i");
        register_dependency(state, "test/c89/many-types.l2", "test/c89/many-types.i");
        register_dependency(state, "test/c89/nested-putchar-a.l2", "test/c89/nested-putchar-a.i");
        register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l2", "test/c89/nested-putchar-a-param-ignored.i");
        register_dependency(state, "test/c89/nested-putchar-a-param-used.l2", "test/c89/nested-putchar-a-param-used.i");
        register_dependency(state, "test/c89/print.l2", "test/c89/print.i");
        register_dependency(state, "test/c89/putchar-return.l2", "test/c89/putchar-return.i");
        register_dependency(state, "test/c89/string-literal.l2", "test/c89/string-literal.i");
        register_dependency(state, "test/c89/use-array.l2", "test/c89/use-array.i");
        register_dependency(state, "test/c89/use-local.l2", "test/c89/use-local.i");
        register_dependency(state, "test/c89/use-reference.l2", "test/c89/use-reference.i");
        register_dependency(state, "test/c89/va_list_call.l2", "test/c89/va_list_call.i");
        register_dependency(state, "test/c89/while-loop.l2", "test/c89/while-loop.i");
        register_dependency(state, "test/c89/typedef.l2", "test/c89/typedef.i");
        register_dependency(state, "test/c89/exotic-declarators.l2", "test/c89/exotic-declarators.i");
        register_dependency(state, "test/c89/constants.l2", "test/c89/constants.i");
        register_dependency(state, "test/c89/malloc-test.l2", "test/c89/malloc-test.i");
        register_dependency(state, "test/c89/main_with_parameters.l2", "test/c89/main_with_parameters.i");
        register_dependency(state, "test/c89/regex_engine_unit_tests.l2", "test/c89/regex_engine_unit_tests.i");

        register_entity(state, "test/c89/basic-operations.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/basic-putchar-aa.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/div-zero.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/preprocessor_test.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/filesystem.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/basic-putchar-a.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/basic-putchar-b.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/empty-main-return-zero.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/for-loop.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/ignore-local.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/many-types.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/nested-putchar-a.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/nested-putchar-a-param-ignored.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/nested-putchar-a-param-used.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/print.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/putchar-return.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/string-literal.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/use-array.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/use-local.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/use-reference.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/va_list_call.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/while-loop.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/typedef.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/exotic-declarators.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/constants.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/malloc-test.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/main_with_parameters.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c89/regex_engine_unit_tests.l1", ENTITY_TYPE_L1_FILE);

        register_dependency(state, "test/c89/basic-operations.l1", "test/c89/basic-operations.l2");
        register_dependency(state, "test/c89/basic-putchar-aa.l1", "test/c89/basic-putchar-aa.l2");
        register_dependency(state, "test/c89/div-zero.l1", "test/c89/div-zero.l2");
        register_dependency(state, "test/c89/preprocessor_test.l1", "test/c89/preprocessor_test.l2");
        register_dependency(state, "test/c89/filesystem.l1", "test/c89/filesystem.l2");
        register_dependency(state, "test/c89/basic-putchar-a.l1", "test/c89/basic-putchar-a.l2");
        register_dependency(state, "test/c89/basic-putchar-b.l1", "test/c89/basic-putchar-b.l2");
        register_dependency(state, "test/c89/empty-main-return-zero.l1", "test/c89/empty-main-return-zero.l2");
        register_dependency(state, "test/c89/for-loop.l1", "test/c89/for-loop.l2");
        register_dependency(state, "test/c89/ignore-local.l1", "test/c89/ignore-local.l2");
        register_dependency(state, "test/c89/many-types.l1", "test/c89/many-types.l2");
        register_dependency(state, "test/c89/nested-putchar-a.l1", "test/c89/nested-putchar-a.l2");
        register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "test/c89/nested-putchar-a-param-ignored.l2");
        register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "test/c89/nested-putchar-a-param-used.l2");
        register_dependency(state, "test/c89/print.l1", "test/c89/print.l2");
        register_dependency(state, "test/c89/putchar-return.l1", "test/c89/putchar-return.l2");
        register_dependency(state, "test/c89/string-literal.l1", "test/c89/string-literal.l2");
        register_dependency(state, "test/c89/use-array.l1", "test/c89/use-array.l2");
        register_dependency(state, "test/c89/use-local.l1", "test/c89/use-local.l2");
        register_dependency(state, "test/c89/use-reference.l1", "test/c89/use-reference.l2");
        register_dependency(state, "test/c89/va_list_call.l1", "test/c89/va_list_call.l2");
        register_dependency(state, "test/c89/while-loop.l1", "test/c89/while-loop.l2");
        register_dependency(state, "test/c89/typedef.l1", "test/c89/typedef.l2");
        register_dependency(state, "test/c89/exotic-declarators.l1", "test/c89/exotic-declarators.l2");
        register_dependency(state, "test/c89/constants.l1", "test/c89/constants.l2");
        register_dependency(state, "test/c89/malloc-test.l1", "test/c89/malloc-test.l2");
        register_dependency(state, "test/c89/main_with_parameters.l1", "test/c89/main_with_parameters.l2");
        register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "test/c89/regex_engine_unit_tests.l2");

        register_entity(state, "test/c99/preprocessor_test.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c99/preprocessor_test.c", "terminal", "true");
        register_entity(state, "test/c99/snprintf_test.c", ENTITY_TYPE_C_FILE);
	add_entity_attribute(state, "test/c99/snprintf_test.c", "terminal", "true");

        register_entity(state, "test/c99/preprocessor_test.i", ENTITY_TYPE_PREPROCESSED_FILE);
        register_entity(state, "test/c99/snprintf_test.i", ENTITY_TYPE_PREPROCESSED_FILE);

        register_dependency(state, "test/c99/preprocessor_test.i", "test/c99/preprocessor_test.c");
        register_dependency(state, "test/c99/snprintf_test.i", "test/c99/snprintf_test.c");

        register_entity(state, "test/c99/preprocessor_test.l2", ENTITY_TYPE_L2_FILE);
        register_entity(state, "test/c99/snprintf_test.l2", ENTITY_TYPE_L2_FILE);

        register_dependency(state, "test/c99/preprocessor_test.l2", "test/c99/preprocessor_test.i");
        register_dependency(state, "test/c99/snprintf_test.l2", "test/c99/snprintf_test.i");

        register_entity(state, "test/c99/preprocessor_test.l1", ENTITY_TYPE_L1_FILE);
        register_entity(state, "test/c99/snprintf_test.l1",  ENTITY_TYPE_L1_FILE);

        register_dependency(state, "test/c99/preprocessor_test.l1", "test/c99/preprocessor_test.l2");
        register_dependency(state, "test/c99/snprintf_test.l1", "test/c99/snprintf_test.l2");

        register_entity(state, "test/c89/basic-operations.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/basic-operations.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/basic-operations.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/basic-putchar-aa.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/basic-putchar-aa.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/basic-putchar-aa.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/div-zero.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/div-zero.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/div-zero.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/preprocessor_test.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/preprocessor_test.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/preprocessor_test.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/filesystem.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/filesystem.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/filesystem.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/basic-putchar-a.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/basic-putchar-a.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/basic-putchar-a.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/basic-putchar-b.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/basic-putchar-b.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/basic-putchar-b.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/empty-main-return-zero.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/empty-main-return-zero.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/empty-main-return-zero.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/for-loop.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/for-loop.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/for-loop.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/ignore-local.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/ignore-local.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/ignore-local.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/many-types.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/many-types.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/many-types.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/nested-putchar-a.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/nested-putchar-a.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/nested-putchar-a.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/nested-putchar-a-param-ignored.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/nested-putchar-a-param-ignored.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/nested-putchar-a-param-ignored.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/nested-putchar-a-param-used.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/nested-putchar-a-param-used.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/nested-putchar-a-param-used.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/print.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/print.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/print.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/putchar-return.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/putchar-return.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/putchar-return.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/string-literal.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/string-literal.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/string-literal.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/use-array.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/use-array.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/use-array.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/use-local.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/use-local.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/use-local.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/use-reference.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/use-reference.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/use-reference.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/va_list_call.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/va_list_call.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/va_list_call.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/while-loop.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/while-loop.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/while-loop.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/typedef.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/typedef.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/typedef.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/exotic-declarators.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/exotic-declarators.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/exotic-declarators.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/constants.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/constants.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/constants.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/malloc-test.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/malloc-test.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/malloc-test.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/main_with_parameters.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/main_with_parameters.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/main_with_parameters.l0.js", "language", "jsonp");
        register_entity(state, "test/c89/regex_engine_unit_tests.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c89/regex_engine_unit_tests.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c89/regex_engine_unit_tests.l0.js", "language", "jsonp");

        register_entity(state, "test/c99/preprocessor_test.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c99/preprocessor_test.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c99/preprocessor_test.l0.js", "language", "jsonp");
        register_entity(state, "test/c99/snprintf_test.l0.js", ENTITY_TYPE_L0_FILE);
	add_entity_attribute(state, "test/c99/snprintf_test.l0.js", "variable_name", "jsonpCallback");
	add_entity_attribute(state, "test/c99/snprintf_test.l0.js", "language", "jsonp");

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
        register_dependency(state, "test/c89/regex_engine_unit_tests.l0.js", "test/c89/regex_engine_unit_tests.l1");

        register_dependency(state, "test/c99/preprocessor_test.l0.js", "test/c99/preprocessor_test.l1");
        register_dependency(state, "test/c99/snprintf_test.l0.js", "test/c99/snprintf_test.l1");

	/* Link final l1 files */
	register_dependency(state, "test/c89/filesystem.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/libdata-structures.l2");
	register_dependency(state, "test/c89/filesystem.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/binary_exponential_buffer.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/memory_pool_collection.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/code_generator.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/preprocessor.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/parser.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/lexer.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/io.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/regex_engine.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/asm.l2");
	register_dependency(state, "test/c89/filesystem.l1", "libc/libc.l2");
	register_dependency(state, "test/c89/filesystem.l1", "recc-implementation/filesystem_impl.l2");
	register_dependency(state, "test/c89/filesystem.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/filesystem.l1", "builtin/l2/mainstub.l2");

	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/libdata-structures.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/binary_exponential_buffer.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/memory_pool_collection.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/code_generator.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/preprocessor.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/parser.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/lexer.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/io.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/regex_engine.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/asm.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "libc/libc.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "recc-implementation/filesystem_impl.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/regex_engine_unit_tests.l1", "builtin/l2/mainstub.l2");

	register_dependency(state, "test/c89/basic-operations.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/basic-operations.l1", "libc/printf.l2");

	register_dependency(state, "test/c89/basic-putchar-aa.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/basic-putchar-aa.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/basic-putchar-aa.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/basic-putchar-aa.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/basic-putchar-aa.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/div-zero.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/div-zero.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/div-zero.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/div-zero.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/div-zero.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/div-zero.l1", "test/extras/main_div_zero_test.l2");
	register_dependency(state, "test/c89/div-zero.l1", "test/extras/l2/mainstub_div_zero_test.l2");
	register_dependency(state, "test/c89/div-zero.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/preprocessor_test.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/preprocessor_test.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/basic-putchar-a.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/basic-putchar-a.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/basic-putchar-a.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/basic-putchar-a.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/basic-putchar-a.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/basic-putchar-b.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/basic-putchar-b.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/basic-putchar-b.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/basic-putchar-b.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/basic-putchar-b.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/empty-main-return-zero.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/empty-main-return-zero.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/empty-main-return-zero.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/empty-main-return-zero.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/empty-main-return-zero.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/for-loop.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/for-loop.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/for-loop.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/for-loop.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/for-loop.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/ignore-local.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/ignore-local.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/ignore-local.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/ignore-local.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/ignore-local.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/many-types.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/many-types.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/many-types.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/many-types.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/many-types.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/nested-putchar-a.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/nested-putchar-a.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/nested-putchar-a.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/nested-putchar-a.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/nested-putchar-a.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-ignored.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/nested-putchar-a-param-used.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/print.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/print.l1", "generated/unsigned_int_binary_search.l2");
	register_dependency(state, "test/c89/print.l1", "libc/malloc.l2");
	register_dependency(state, "test/c89/print.l1", "generated/unsigned_int_merge_sort.l2");
	register_dependency(state, "test/c89/print.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/print.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/print.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/print.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/print.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/print.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/print.l1", "test/extras/extern_test.l2");
	register_dependency(state, "test/c89/print.l1", "test/extras/random.l2");

	register_dependency(state, "test/c89/putchar-return.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/putchar-return.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/putchar-return.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/putchar-return.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/putchar-return.l1", "kernel/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/string-literal.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/string-literal.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/string-literal.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/string-literal.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/string-literal.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/string-literal.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/string-literal.l1", "libc/putchar.l2");

	register_dependency(state, "test/c89/use-array.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/use-array.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/use-array.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/use-array.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/use-array.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/use-array.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/use-array.l1", "builtin/libbuiltin.l2");

	register_dependency(state, "test/c89/use-local.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/use-local.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/use-local.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/use-local.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/use-local.l1", "kernel/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/use-reference.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/use-reference.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/use-reference.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/use-reference.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/use-reference.l1", "kernel/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/va_list_call.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/va_list_call.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/va_list_call.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/va_list_call.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/va_list_call.l1", "kernel/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/while-loop.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/while-loop.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/while-loop.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/while-loop.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/while-loop.l1", "builtin/signed_less_than.l2");
	register_dependency(state, "test/c89/while-loop.l1", "kernel/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/typedef.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/typedef.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/typedef.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/typedef.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/typedef.l1", "kernel/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/exotic-declarators.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/exotic-declarators.l1", "builtin/stack.l2");
	register_dependency(state, "test/c89/exotic-declarators.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/exotic-declarators.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/exotic-declarators.l1", "kernel/l2/putchar_busy.l2");

	register_dependency(state, "test/c89/constants.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/constants.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/constants.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/constants.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/constants.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/constants.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/constants.l1", "builtin/l2/mainstub.l2");

	register_dependency(state, "test/c89/malloc-test.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "libc/printf.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "libc/malloc.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "builtin/l2/mainstub.l2");
	register_dependency(state, "test/c89/malloc-test.l1", "test/extras/random.l2");

	register_dependency(state, "test/c89/main_with_parameters.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "test/extras/main_argv_indirect.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "test/extras/l2/mainstub_argv_indirect.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "libc/putchar.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "libc/assert.l2");
	register_dependency(state, "test/c89/main_with_parameters.l1", "libc/printf.l2");

	register_dependency(state, "test/c99/preprocessor_test.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "libc/assert.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "libc/printf.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "libc/malloc.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "libc/putchar.l2");
	register_dependency(state, "test/c99/preprocessor_test.l1", "builtin/l2/mainstub.l2");

	register_dependency(state, "test/c99/snprintf_test.l1", "kernel/l2/putchar_busy.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "builtin/l2/memory_mapped_registers.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "builtin/libbuiltin.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "libc/assert.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "libc/printf.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "libc/malloc.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "libc/putchar.l2");
	register_dependency(state, "test/c99/snprintf_test.l1", "builtin/l2/mainstub.l2");
}
