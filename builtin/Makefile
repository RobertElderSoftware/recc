BUILTIN_SOURCES=$(wildcard builtin/*.c)

BUILTIN_OBJECTS=$(BUILTIN_SOURCES:.c=.o)

builtin/%.o: builtin/%.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C89_FLAGS)

builtin/build/recc.o: builtin/build/recc.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C89_FLAGS)

$(CROSS_BUILD_PREFIX)builtin/recc.o: $(CROSS_BUILD_PREFIX)builtin/recc.c recc-implementation/compiler_interface_phase_3.h
	@$(HOSTCC) -c $(CROSS_BUILD_PREFIX)builtin/recc.c -o $(CROSS_BUILD_PREFIX)builtin/recc.o $(CUSTOM_C89_FLAGS)

clean-builtins:
	@rm -f builtin/build/*.o builtin/*.i builtin/*.o builtin/*.l2
