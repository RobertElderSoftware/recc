ifeq ("$(wildcard config.mak)","")
$(error config.mak does not exist... Run './configure' first.)
all:
	@touch /dev/null
else
include config.mak


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

test: run-tests

COMPILER_OBJECTS=libc/filesystem.o test/recc.o data-structures/recc.o libc/recc.o kernel/recc.o builtin/recc.o recc-implementation/recc.o recc-implementation/librecc.a

include builtin/Makefile
include demos/brainfuck-cpp/Makefile
include kernel/Makefile
include data-structures/Makefile
include recc-implementation/Makefile
include test/Makefile
include libc/Makefile
include emulators/Makefile

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


clean: clean-recc-implementation clean-data-structures clean-tests clean-builtins clean-stdlib clean-kernel clean-emulators clean-brainfuck-cpp
	@rm -f logs/*

endif
