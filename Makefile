CUSTOM_CC=./recc
CUSTOM_PRELOADER=./preloader
HOSTCC=gcc
CLANG_FLAGS=-g -ferror-limit=3 -W -Wextra -Wall -Werror -Weverything -pedantic
GCC_FLAGS=-g -std=c89 -W -Wextra -Wall -Werror -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
CUSTOM_FLAGS=$(GCC_FLAGS)
VALGRIND=#valgrind -q --leak-check=full --show-reachable=yes --track-origins=yes

test: run-tests

kernel: build-and-test-kernel

recc: data-structures/binary_exponential_buffer.o data-structures/memory_pooler.o recc.o lexer.o parser.o code_generator.o preprocessor.o linker.o io.o data-structures/replace_tool.o data-structures/libgenerated-data-structures.a 
	@$(HOSTCC) -o recc recc.o lexer.o parser.o code_generator.o preprocessor.o linker.o io.o data-structures/unsigned_int_list.o data-structures/binary_exponential_buffer.o data-structures/memory_pooler.o data-structures/replace_tool.o data-structures/libgenerated-data-structures.a $(CUSTOM_FLAGS)

recc.o: recc.c parser.h lexer.h linker.h preprocessor.h code_generator.h
	@$(HOSTCC) -c recc.c $(CUSTOM_FLAGS)

lexer.h: bootstrap-datatypes

lexer.o: lexer.h lexer.c
	@$(HOSTCC) -c lexer.c $(CUSTOM_FLAGS)

parser.h: bootstrap-datatypes

parser.o: parser.h parser.c
	@$(HOSTCC) -c parser.c $(CUSTOM_FLAGS)

code_generator.o: code_generator.h code_generator.c parser.h lexer.h
	@$(HOSTCC) -c code_generator.c $(CUSTOM_FLAGS)

code_generator.h: bootstrap-datatypes

linker.h: bootstrap-datatypes

linker.o: linker.c linker.h
	@$(HOSTCC) -c linker.c $(CUSTOM_FLAGS)

io.h: data-structures/unsigned_char_list.h

io.o: io.c io.h data-structures/unsigned_char_list.o
	@$(HOSTCC) -c io.c $(CUSTOM_FLAGS)

preprocessor.o: preprocessor.c bootstrap-datatypes
	@$(HOSTCC) -c preprocessor.c $(CUSTOM_FLAGS)

preloader.h: io.h

preloader: preloader.c preloader.h
	@$(HOSTCC) preloader.c -o preloader $(CUSTOM_FLAGS)

clean: clean-data-structures clean-tests clean-builtins clean-stdlib clean-kernel clean-emulators
	@rm -f recc preloader *.o

include kernel/Makefile
include data-structures/Makefile
include test/Makefile
include builtin/Makefile
include stdlib/Makefile
include emulators/Makefile
