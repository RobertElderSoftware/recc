HOSTCC=gcc
CLANG_C89_FLAGS=-g -std=c89 -ferror-limit=10 -W -Wextra -Wall -Werror -Weverything -pedantic -Wno-switch-enum -Wno-covered-switch-default -Wno-format-nonliteral -Wno-loop-analysis -Wno-disabled-macro-expansion -Wno-unused-macros
CLANG_C99_FLAGS=-g -std=c99 -ferror-limit=10 -W -Wextra -Wall -Werror -Weverything -pedantic -Wno-switch-enum -Wno-covered-switch-default -Wno-format-nonliteral -Wno-loop-analysis -Wno-disabled-macro-expansion -Wno-unused-macros
CLANGPP_FLAGS=-x c++ -std=c++11 -g -ferror-limit=10 -W -Wextra -Weverything -Wall -pedantic -Wno-switch-enum -Wno-covered-switch-default -Wno-format-nonliteral -Wno-loop-analysis -Wno-implicit-fallthrough -Wno-constant-logical-operand
GCC_C89_FLAGS=-g -std=c89 -W -Wextra -Wall -Werror -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
GCC_C99_FLAGS=-g -std=c99 -W -Wextra -Wall -Werror -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
GPP_FLAGS=-g -W -Wextra -Wall -Werror -pedantic
CUSTOM_C89_FLAGS=$(GCC_C89_FLAGS)
CUSTOM_C99_FLAGS=$(GCC_C99_FLAGS)
VALGRIND=#valgrind -q --leak-check=full --show-reachable=yes --track-origins=yes --log-file=out

COMPILER_OBJECTS=libc/filesystem.o test/recc.o data-structures/recc.o libc/recc.o kernel/recc.o builtin/recc.o recc-implementation/recc.o recc-implementation/librecc.a

help:
	@echo "What would you like to make?"
	@echo ""
	@echo "1) make data-structures/bootstrap - (IMPORTANT) Must be run before anything else in a fresh project to create initial Makefiles."
	@echo "2) make kernel/build_kernel       - Builds an executable 'kernel/build_kernel', that can build the kernel."
	@echo "2) make test/build_tests          - Builds an executable 'test/build_tests', that can build all tests."
	@echo "3) make kernel/kernel.l1          - Builds the kernel image (but doesn't try to run it)."
	@echo "4) make run-c-emulator            - Attempts to run the kernel in the C emulator."
	@echo "5) make run-java-emulator         - Attempts to run the kernel in the Java emulator."
	@echo "6) make run-python-emulator       - Attempts to run the kernel in the Python emulator."
	@echo "7) make test                      - Attempts to run unit tests in chrome (requires that testing API is set up)."
	@echo "8) make kernel                    - Attempts to run the kernel in chrome (requires that testing API is set up)."

test: run-tests

kernel: build-and-test-kernel

clean: clean-recc-implementation clean-data-structures clean-tests clean-builtins clean-stdlib clean-kernel clean-emulators clean-brainfuck-cpp


include demos/brainfuck-cpp/Makefile
include kernel/Makefile
include data-structures/Makefile
include recc-implementation/Makefile
include test/Makefile
include builtin/Makefile
include libc/Makefile
include emulators/Makefile
