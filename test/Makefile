include test/c89/Makefile
include test/c99/Makefile

ifeq ($(origin SPECIFIC_TESTS),undefined)
  ALL_SOURCES=$(C99_SOURCES) $(C89_SOURCES)
  ALL_TEST_NAMES=$(basename $(ALL_SOURCES))
else
  ALL_SOURCES=$(addsuffix .c, $(SPECIFIC_TESTS))
  ALL_TEST_NAMES=$(SPECIFIC_TESTS)
endif

ALL_OBJECTS=$(ALL_SOURCES:.c=.o)
ALL_EXECUTABLES=$(ALL_SOURCES:.c=.exec)
ALL_HOST_CC_OUTPUT=$(ALL_SOURCES:.c=.output)
ALL_DIFFS=$(ALL_SOURCES:.c=.diff)

RECC_PREPROCESSED= $(ALL_SOURCES:.c=.i)
RECC_L2= $(ALL_SOURCES:.c=.l2)
RECC_L1= $(ALL_SOURCES:.c=.l1)
RECC_L0= $(ALL_SOURCES:.c=.l0.js)
JS_OUTPUT= $(ALL_SOURCES:.c=.jsoutput)

REQUIRED_BUILTIN_OBJECTS=builtin/print_buff_add.o builtin/extern_test.o builtin/print_bits.o builtin/c99libc_in_c89.o builtin/print_hexadecimal.o builtin/print_string.o builtin/print_decimal.o builtin/random.o builtin/signed_division.o builtin/signed_greater_than.o builtin/signed_greater_than_or_equal_to.o builtin/signed_less_than.o builtin/signed_less_than_or_equal_to.o builtin/signed_modulo.o

#  Can change based on make file variable values
remove-previous-test-list:
	rm -f test/tests_to_run

test/build_tests: test/build_tests.o $(COMPILER_OBJECTS)
	@$(HOSTCC) -o test/build_tests test/build_tests.o $(COMPILER_OBJECTS) $(CUSTOM_C89_FLAGS)

test/build_tests.o: test/build_tests.c recc-implementation/compiler_interface.h
	@$(HOSTCC) -c test/build_tests.c -o test/build_tests.o $(CUSTOM_C89_FLAGS)

test/tests_to_run: remove-previous-test-list
	@echo "$(ALL_TEST_NAMES)" > test/tests_to_run

run-tests: $(ALL_DIFFS)

remove-previous-custom-list:
	@rm -f test/custom_build_targets.c

test/custom_build_targets.c: remove-previous-custom-list
	@echo "$(addsuffix \\n, $(ALL_TEST_NAMES))" | tr -d ' ' | sed '/^$$/d' | sed 's/.*/\tconstruct_build_target(state, "\0.l0.js");/' > test/custom_build_targets.c

test/build_tests.c: test/build_target_template.c test/custom_build_targets.c
	@cat test/build_target_template.c | sed -e '/GETS_REPLACED_WITH_BUILD_TARGETS/r test/custom_build_targets.c' -e '/GETS_REPLACED_WITH_BUILD_TARGETS/d' > test/build_tests.c

test/recc.o: test/recc.c
	@$(HOSTCC) -c test/recc.c -o test/recc.o $(CUSTOM_C89_FLAGS)

remove-old-js-and-diffs:
	@rm -f $(JS_OUTPUT) $(ALL_DIFFS)

run-build-tests: $(ALL_HOST_CC_OUTPUT) $(SOURCES) test/build_tests
	$(VALGRIND) test/build_tests

$(JS_OUTPUT): js-output 

js-output: run-build-tests test/tests_to_run
#  The use of this data dir will make the browser tests blocking if there is already a chrome window open.
	@google-chrome "http://localhost/emulators/javascript/" --user-data-dir="emulators/javascript/chrome/data-dir" > /dev/null 2>&1

$(ALL_EXECUTABLES): $(REQUIRED_BUILTIN_OBJECTS) $(COMPILER_OBJECTS)

%.output: %.exec
	@./$< > $@ 2>&1 || true #  This commands never errors, even if the gcc executable does.

%.diff: %.output %.jsoutput
	@diff $< $(word 2,$^) > $@ || >&2 echo "\033[0;31mTest Failure for "$<" \033[0m"

clean-tests:
	@rm -f test/build_tests test/custom_build_targets.c test/build_tests.c test/tests_to_run test/*.o $(ALL_OBJECTS) $(RECC_PREPROCESSED) $(RECC_L2) $(RECC_L1) $(RECC_L0) $(ALL_EXECUTABLES) $(ALL_HOST_CC_OUTPUT) $(JS_OUTPUT) $(ALL_DIFFS)
