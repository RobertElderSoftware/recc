CUSTOM_CC=./recc
CUSTOM_PRELOADER=./preloader
HOSTCC=gcc
CLANG_FLAGS=-g -ferror-limit=3 -W -Wextra -Wall -Werror -Weverything -pedantic -Wno-switch-enum -Wno-covered-switch-default -Wno-format-nonliteral
GCC_FLAGS=-g -std=c89 -W -Wextra -Wall -Werror -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
GPP_FLAGS=-g -W -Wextra -Wall -Werror -pedantic
CUSTOM_FLAGS=$(GCC_FLAGS)
VALGRIND=#valgrind -q --leak-check=full --show-reachable=yes --track-origins=yes

test: run-tests

kernel: build-and-test-kernel

recc: data-structures/binary_exponential_buffer.o data-structures/memory_pooler.o recc.o lexer.o parser.o code_generator.o preprocessor.o linker.o io.o data-structures/replace_tool.o data-structures/libgenerated-data-structures.a filesystem/filesystem_impl.l2
	@$(HOSTCC) -o recc recc.o lexer.o parser.o code_generator.o preprocessor.o linker.o io.o data-structures/binary_exponential_buffer.o data-structures/memory_pooler.o data-structures/replace_tool.o data-structures/libgenerated-data-structures.a $(CUSTOM_FLAGS)

recc.o: recc.c parser.h lexer.h linker.h preprocessor.h code_generator.h
	@$(HOSTCC) -c recc.c $(CUSTOM_FLAGS)

lexer.h: bootstrap-datatypes io.h

lexer.o: lexer.h lexer.c
	@$(HOSTCC) -c lexer.c $(CUSTOM_FLAGS)

parser.h: bootstrap-datatypes io.h core_data_types.h lexer.h

parser.o: parser.h parser.c
	@$(HOSTCC) -c parser.c $(CUSTOM_FLAGS)

code_generator.o: code_generator.h code_generator.c
	@$(HOSTCC) -c code_generator.c $(CUSTOM_FLAGS)

code_generator.h: bootstrap-datatypes parser.h linker.h lexer.h

linker.h: bootstrap-datatypes core_data_types.h io.h lexer.h parser.h

linker.o: linker.c linker.h
	@$(HOSTCC) -c linker.c $(CUSTOM_FLAGS)

io.h: bootstrap-datatypes

io.o: io.c io.h data-structures/unsigned_char_list.o
	@$(HOSTCC) -c io.c $(CUSTOM_FLAGS)

preloader.h: io.h
	@touch preloader.h

preloader: preloader.c preloader.h
	@$(HOSTCC) preloader.c -o preloader $(CUSTOM_FLAGS)

preprocessor.h: io.h
	@touch preprocessor.h

preprocessor.o: preprocessor.c preprocessor.h
	@$(HOSTCC) -c preprocessor.c -o preprocessor.o $(CUSTOM_FLAGS)

clean: clean-data-structures clean-tests clean-builtins clean-stdlib clean-kernel clean-emulators clean-filesystem
	@rm -f recc preloader filesystem_compiler *.o

include filesystem/Makefile
include kernel/Makefile
include data-structures/Makefile
include test/Makefile
include builtin/Makefile
include libc/Makefile
include emulators/Makefile
