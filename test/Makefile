.PRECIOUS: %.oc89
.PRECIOUS: %.oc99
.PRECIOUS: %.exec89
.PRECIOUS: %.exec99

C89_SOURCES = $(wildcard test/c89/*.c)
C99_SOURCES = $(wildcard test/c99/*.c)

C89_HOST_CC_OBJECTS= $(C89_SOURCES:.c=.oc89)
C99_HOST_CC_OBJECTS= $(C99_SOURCES:.c=.oc99)

C89_HOST_CC_EXECUTABLES= $(C89_SOURCES:.c=.exec89)
C99_HOST_CC_EXECUTABLES= $(C99_SOURCES:.c=.exec99)

C89_HOST_CC_OUTPUT=$(C89_SOURCES:.c=.c89output)
C99_HOST_CC_OUTPUT=$(C99_SOURCES:.c=.c99output)

C89_DIFF=$(C89_SOURCES:.c=.c89diff)
C99_DIFF=$(C99_SOURCES:.c=.c99diff)

ALL_OBJECTS=$(C89_HOST_CC_OBJECTS) $(C99_HOST_CC_OBJECTS)
ALL_EXECUTABLES=$(C89_HOST_CC_EXECUTABLES) $(C99_HOST_CC_EXECUTABLES)
ALL_HOST_CC_OUTPUT=$(C89_HOST_CC_OUTPUT) $(C99_HOST_CC_OUTPUT)
ALL_SOURCES=$(C89_SOURCES) $(C99_SOURCES)
ALL_DIFFS=$(C89_DIFF) $(C99_DIFF)

RECC_PREPROCESSED= $(ALL_SOURCES:.c=.i)
RECC_L2= $(ALL_SOURCES:.c=.l2)
RECC_L1= $(ALL_SOURCES:.c=.l1)
RECC_L0= $(ALL_SOURCES:.c=.l0.js)
JS_OUTPUT= $(ALL_SOURCES:.c=.jsoutput)

REQUIRED_BUILTIN_OBJECTS=builtin/extern_test.o builtin/print_bits.o builtin/print_decimal.o builtin/random.o builtin/signed_division.o builtin/signed_greater_than.o builtin/signed_greater_than_or_equal_to.o builtin/signed_less_than.o builtin/signed_less_than_or_equal_to.o builtin/signed_modulo.o

# Main target
run-tests: $(ALL_DIFFS)

test/recc.o: test/recc.c
	@$(HOSTCC) -c test/recc.c -o test/recc.o $(CUSTOM_C89_FLAGS)

remove-old-js-and-diffs:
	@rm -f $(JS_OUTPUT) $(ALL_DIFFS)

run-build-tests: $(ALL_HOST_CC_OUTPUT) $(SOURCES) build_tests
	$(VALGRIND) ./build_tests

$(JS_OUTPUT): js-output

js-output: run-build-tests
#  The use of this data dir will make the browser tests blocking if there is already a chrome window open.
	@google-chrome "http://localhost/emulators/javascript/" --user-data-dir="chrome/data-dir" > /dev/null 2>&1

$(ALL_EXECUTABLES): $(REQUIRED_BUILTIN_OBJECTS) $(COMPILER_OBJECTS)

%.oc89: %.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C89_FLAGS)

%.oc99: %.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C99_FLAGS)

%.exec89: %.oc89
	@$(HOSTCC) $< $(REQUIRED_BUILTIN_OBJECTS) $(COMPILER_OBJECTS) -o $@ $(CUSTOM_C89_FLAGS)

%.exec99: %.oc99
	@$(HOSTCC) $< $(REQUIRED_BUILTIN_OBJECTS) $(COMPILER_OBJECTS) -o $@ $(CUSTOM_C99_FLAGS)

%.c89output: %.exec89
	@./$< > $@ 2>&1 || true #  This commands never errors, even if the gcc executable does.

%.c99output: %.exec99
	@./$< > $@ 2>&1 || true #  This commands never errors, even if the gcc executable does.

%.c89diff: %.c89output %.jsoutput
	@diff $< $(word 2,$^) > $@ || >&2 echo "\033[0;31mTest Failure for "$<" \033[0m"

%.c99diff: %.c99output %.jsoutput
	@diff $< $(word 2,$^) > $@ || >&2 echo "\033[0;31mTest Failure for "$<" \033[0m"

clean-tests:
	@rm -f test/recc.o $(ALL_OBJECTS) $(RECC_PREPROCESSED) $(RECC_L2) $(RECC_L1) $(RECC_L0) $(ALL_EXECUTABLES) $(ALL_HOST_CC_OUTPUT) $(JS_OUTPUT) $(ALL_DIFFS)
