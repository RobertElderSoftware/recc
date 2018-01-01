.SUFFIXES:
.PHONY: test run-make-first kernel
ifeq ("$(wildcard config.mak)","")
$(error config.mak does not exist... Run './configure' first.)
all:
	@:
else
include config.mak

#  Used for building the compiler against build targets from another version
#  of RECC (used for benchmarking and testing)
#  If undefined or set to the current directory
ifeq ($(origin CROSS_BUILD_PREFIX),undefined)
  CROSS_BUILD_PREFIX_PREREQ=
  CROSS_BUILD_PREFIX=./
else
ifeq ("${CROSS_BUILD_PREFIX}","./")
  CROSS_BUILD_PREFIX_PREREQ=
  CROSS_BUILD_PREFIX=./
else
  CROSS_BUILD_PREFIX_PREREQ=cross-build-prefix-prereq
  CROSS_BUILD_PREFIX=${CROSS_BUILD_PREFIX}
endif
endif

CLANG_C89_FLAGS=-g -std=c89 $(SUPPORTED_CLANG_WARNING_FLAGS)
CLANG_C99_FLAGS=-g -std=c99 $(SUPPORTED_CLANG_WARNING_FLAGS)
CLANGPP_FLAGS=-xc++ -std=c++11 -g ${SUPPORTED_CLANGPP_WARNING_FLAGS}
GCC_C89_FLAGS=-g -std=c89 $(SUPPORTED_GCC_WARNING_FLAGS)
GCC_C99_FLAGS=-g -std=c99 $(SUPPORTED_GCC_WARNING_FLAGS)
GPP_FLAGS=-g -W -Wextra -Wall -pedantic

ifeq ($(origin USE_VALGRIND),undefined)
  VALGRIND=
else
ifeq ("${USE_VALGRIND}","fast")
  VALGRIND=valgrind --leak-check=summary --log-file=logs/valgrind.log
else
ifeq ("${USE_VALGRIND}","full")
  VALGRIND=valgrind --leak-check=full --log-file=logs/valgrind.log --show-reachable=yes --track-origins=yes 
else
$(error Unknown Valgrind configuration: "${USE_VALGRIND}")
endif
endif
endif

ifeq ($(origin USE_COMPILER),undefined)
  HOSTCC=gcc
else
  HOSTCC=${USE_COMPILER}
endif

ifeq ("${HOSTCC}","gcc")
CUSTOM_C89_FLAGS=$(GCC_C89_FLAGS)
CUSTOM_C99_FLAGS=$(GCC_C99_FLAGS)
else
ifeq ("${HOSTCC}","g++")
CUSTOM_C89_FLAGS=$(GPP_FLAGS)  #  Not actually c89, or c99, but there is no such thing as c89 c++
CUSTOM_C99_FLAGS=$(GPP_FLAGS)
else
ifeq ("${HOSTCC}","clang")
CUSTOM_C89_FLAGS=$(CLANG_C89_FLAGS)
CUSTOM_C99_FLAGS=$(CLANG_C99_FLAGS)
else
ifeq ("${HOSTCC}","clang++")
CUSTOM_C89_FLAGS=$(CLANGPP_FLAGS)  #  Not actually c89, or c99, but there is no such thing as c89 c++
CUSTOM_C99_FLAGS=$(CLANGPP_FLAGS)
else
$(error Unknown Compiler: "${HOSTCC}")
endif
endif
endif
endif


REGEX_ENGINE_RECC_IMPLEMENTATION_OBJECTS=recc-implementation/io.o recc-implementation/lexer.o recc-implementation/regex_engine.o recc-implementation/memory_pool_collection.o recc-implementation/binary_exponential_buffer.o
PARSER_RECC_IMPLEMENTATION_OBJECTS=recc-implementation/asm_functions.o recc-implementation/asm.o recc-implementation/regex_engine.o recc-implementation/lexer.o recc-implementation/parser.o recc-implementation/io.o recc-implementation/memory_pool_collection.o recc-implementation/binary_exponential_buffer.o
ALL_RECC_IMPLEMENTATION_OBJECTS=recc-implementation/asm.o recc-implementation/regex_engine.o recc-implementation/compiler_interface_common.o recc-implementation/compiler_interface_c_compiler_targets.o recc-implementation/compiler_interface_header_targets.o recc-implementation/compiler_interface_phase_3.o recc-implementation/lexer.o recc-implementation/parser.o recc-implementation/code_generator.o recc-implementation/linker.o recc-implementation/io.o recc-implementation/l0_generator.o recc-implementation/preprocessor.o recc-implementation/memory_pool_collection.o $(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.o recc-implementation/libbootstrap.o recc-implementation/l2_parser.o recc-implementation/replace_tool.o recc-implementation/binary_exponential_buffer.o

ifeq ("$(wildcard recc-implementation/bootstrap_phase_2)","")
include recc-implementation/Makefile
.DEFAULT:
	#@echo $(MAKECMDGOALS)
	@make -e -s phase1 DEFERRED_GLOALS="$(MAKECMDGOALS)"
kernel:
	@make -e -s phase1 DEFERRED_GLOALS="$(MAKECMDGOALS)"
test:
	@make -e -s phase1 DEFERRED_GLOALS="$(MAKECMDGOALS)"
else

help:
	@echo "What would you like to make?"
	@echo ""
	@echo "1) make kernel/build_kernel       - Builds an executable 'kernel/build_kernel', that can build the kernel."
	@echo "2) make test/build_tests          - Builds an executable 'test/build_tests', that can build all tests."
	@echo "3) make kernel/kernel.l1          - Builds the kernel image (but doesn't try to run it)."
	@echo "4) make run-c-emulator            - Attempts to run the kernel in the C emulator."
	@echo "5) make run-java-emulator         - Attempts to run the kernel in the Java emulator."
	@echo "6) make run-python-emulator       - Attempts to run the kernel in the Python emulator."
	@echo "7) make test                      - Attempts to run unit tests in chrome (requires that testing API is set up)."
	@echo "8) make kernel                    - Attempts to run the kernel in chrome (requires that testing API is set up)."


test: run-tests

COMPILER_OBJECTS=libc/filesystem.o $(CROSS_BUILD_PREFIX)test/recc.o $(CROSS_BUILD_PREFIX)recc-implementation/phase2_data_structures.o recc-implementation/phase2_asm_data_structures.o $(CROSS_BUILD_PREFIX)libc/recc.o $(CROSS_BUILD_PREFIX)kernel/recc.o $(CROSS_BUILD_PREFIX)builtin/build/recc.o $(CROSS_BUILD_PREFIX)recc-implementation/recc.o $(ALL_RECC_IMPLEMENTATION_OBJECTS) $(DATA_STRUCTURES_OBJECT_FILES) $(BUILTIN_OBJECTS)

include builtin/Makefile
include recc-implementation/library-data-structures
include recc-implementation/object-data-structures
include recc-implementation/file-dependencies-data-structures
include demos/brainfuck-cpp/Makefile
include kernel/Makefile
include recc-implementation/Makefile
include test/Makefile
include libc/Makefile
include emulators/Makefile


clean: clean-recc-implementation clean-tests clean-builtins clean-stdlib clean-kernel clean-emulators clean-brainfuck-cpp
	@rm -f logs/*
endif
endif
