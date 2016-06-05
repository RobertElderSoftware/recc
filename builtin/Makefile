BUILTIN_SOURCES=$(wildcard builtin/*.c)

BUILTIN_OBJECTS=$(BUILTIN_SOURCES:.c=.o)

builtin/%.o: builtin/%.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C89_FLAGS)

builtin/recc.o: builtin/recc.c recc-implementation/compiler_interface_phase_3.h
	@$(HOSTCC) -c builtin/recc.c -o builtin/recc.o $(CUSTOM_C89_FLAGS)

clean-builtins:
	@rm -f builtin/*.i builtin/*.o builtin/*.l2
