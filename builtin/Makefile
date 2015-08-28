BUILTIN_SOURCES=$(wildcard builtin/*.c)

BUILTIN_OBJECTS=$(BUILTIN_SOURCES:.c=.o)

build-builtins: recc
	$(VALGRIND) ./recc builtin/build-script builtin/new-build-script
%.o: %.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C89_FLAGS)

builtin/recc.o: builtin/recc.c compiler_interface.h
	@$(HOSTCC) -c builtin/recc.c -o builtin/recc.o $(CUSTOM_C89_FLAGS)

clean-builtins:
	@rm -f builtin/*.i builtin/*.o builtin/*.l2
