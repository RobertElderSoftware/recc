HOSTCC=gcc
CLANG_C89_FLAGS=-g -std=c89 -ferror-limit=10 -W -Wextra -Wall -Werror -Weverything -pedantic -Wno-switch-enum -Wno-covered-switch-default -Wno-format-nonliteral -Wno-loop-analysis -Wno-disabled-macro-expansion -Wno-unused-macros
CLANG_C99_FLAGS=-g -std=c99 -ferror-limit=10 -W -Wextra -Wall -Werror -Weverything -pedantic -Wno-switch-enum -Wno-covered-switch-default -Wno-format-nonliteral -Wno-loop-analysis -Wno-disabled-macro-expansion -Wno-unused-macros
CLANGPP_FLAGS=-x c++ -std=c++11 -g -ferror-limit=10 -W -Wextra -Weverything -Wall -pedantic -Wno-switch-enum -Wno-covered-switch-default -Wno-format-nonliteral -Wno-loop-analysis 
GCC_C89_FLAGS=-g -std=c89 -W -Wextra -Wall -Werror -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
GCC_C99_FLAGS=-g -std=c99 -W -Wextra -Wall -Werror -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
GPP_FLAGS=-g -W -Wextra -Wall -Werror -pedantic
CUSTOM_C89_FLAGS=$(GCC_C89_FLAGS)
CUSTOM_C99_FLAGS=$(GCC_C99_FLAGS)
VALGRIND=#valgrind -q --leak-check=full --show-reachable=yes --track-origins=yes --log-file=out

COMPILER_OBJECTS=data-structures/binary_exponential_buffer.o memory_pool_collection.o heap_memory_pool.o compiler_interface.o lexer.o preloader.o parser.o code_generator.o preprocessor.o linker.o io.o data-structures/replace_tool.o filesystem/filesystem_compiler.o data-structures/libgenerated-data-structures.a libc/filesystem.o test/recc.o data-structures/recc.o libc/recc.o filesystem/recc.o kernel/recc.o builtin/recc.o recc.o 

help:
	@echo "What would you like to make?"
	@echo ""
	@echo "1) make bootstrap-datatypes  - (IMPORTANT) Must be run before anything else in a fresh project to create initial Makefiles."
	@echo "2) make build_kernel         - Builds an executable 'build_kernel', that can build the kernel."
	@echo "2) make build_tests          - Builds an executable 'build_tests', that can build all tests."
	@echo "3) make kernel/kernel.l1     - Builds the kernel image (but doesn't try to run it)."
	@echo "4) make run-c-emulator       - Attempts to run the kernel in the C emulator."
	@echo "5) make run-java-emulator    - Attempts to run the kernel in the Java emulator."
	@echo "6) make run-python-emulator  - Attempts to run the kernel in the Python emulator."
	@echo "7) make test                 - Attempts to run unit tests in chrome (requires that testing API is set up)."
	@echo "8) make kernel               - Attempts to run the kernel in chrome (requires that testing API is set up)."

test: run-tests

build_kernel: build_kernel.o $(COMPILER_OBJECTS)
	@$(HOSTCC) -o build_kernel build_kernel.o $(COMPILER_OBJECTS) $(CUSTOM_C89_FLAGS)

build_tests: build_tests.o $(COMPILER_OBJECTS)
	@$(HOSTCC) -o build_tests build_tests.o $(COMPILER_OBJECTS) $(CUSTOM_C89_FLAGS)

build_kernel.o: build_kernel.c compiler_interface.h
	@$(HOSTCC) -c build_kernel.c -o build_kernel.o $(CUSTOM_C89_FLAGS)

build_tests.o: build_tests.c compiler_interface.h
	@$(HOSTCC) -c build_tests.c -o build_tests.o $(CUSTOM_C89_FLAGS)

data-structures/recc.o: data-structures/recc.c
	@$(HOSTCC) -c data-structures/recc.c -o data-structures/recc.o $(CUSTOM_C89_FLAGS)

recc.o: recc.c
	@$(HOSTCC) -c recc.c -o recc.o $(CUSTOM_C89_FLAGS)

kernel: build-and-test-kernel

compiler_interface.o: compiler_interface.c compiler_interface.h
	@$(HOSTCC) -c compiler_interface.c $(CUSTOM_C89_FLAGS)

lexer.h: bootstrap-datatypes io.h
	@touch lexer.h

lexer.o: lexer.h lexer.c
	@$(HOSTCC) -c lexer.c $(CUSTOM_C89_FLAGS)

parser.h: bootstrap-datatypes io.h core_data_types.h lexer.h
	@touch parser.h

parser.o: parser.h parser.c
	@$(HOSTCC) -c parser.c $(CUSTOM_C89_FLAGS)

code_generator.o: code_generator.h code_generator.c
	@$(HOSTCC) -c code_generator.c $(CUSTOM_C89_FLAGS)

code_generator.h: bootstrap-datatypes parser.h linker.h lexer.h
	@touch code_generator.h

linker.h: bootstrap-datatypes core_data_types.h io.h lexer.h parser.h
	@touch linker.h

linker.o: linker.c linker.h
	@$(HOSTCC) -c linker.c $(CUSTOM_C89_FLAGS)

io.h: bootstrap-datatypes
	@touch io.h

compiler_interface.h: bootstrap-datatypes
	@touch compiler_interface.h

io.o: io.c io.h data-structures/unsigned_char_list.o
	@$(HOSTCC) -c io.c $(CUSTOM_C89_FLAGS)

preloader.h: io.h
	@touch preloader.h

preloader.o: preloader.c preloader.h
	@$(HOSTCC) -c preloader.c $(CUSTOM_C89_FLAGS)

preprocessor.h: io.h
	@touch preprocessor.h

preprocessor.o: preprocessor.c preprocessor.h
	@$(HOSTCC) -c preprocessor.c -o preprocessor.o $(CUSTOM_C89_FLAGS)

memory_pool_collection.h: bootstrap-datatypes
	@touch memory_pool_collection.h

memory_pool_collection.o: memory_pool_collection.c memory_pool_collection.h
	@$(HOSTCC) -c memory_pool_collection.c -o memory_pool_collection.o $(CUSTOM_C89_FLAGS)

heap_memory_pool.h: bootstrap-datatypes
	@touch heap_memory_pool.h

heap_memory_pool.o: heap_memory_pool.c heap_memory_pool.h
	@$(HOSTCC) -c heap_memory_pool.c -o heap_memory_pool.o $(CUSTOM_C89_FLAGS)

clean: clean-data-structures clean-tests clean-builtins clean-stdlib clean-kernel clean-emulators clean-filesystem clean-brainfuck-cpp
	@rm -f build_tests build_kernel filesystem_compiler *.o *.l2 *.i

include demos/brainfuck-cpp/Makefile
include filesystem/Makefile
include kernel/Makefile
include data-structures/Makefile
include test/Makefile
include builtin/Makefile
include libc/Makefile
include emulators/Makefile
