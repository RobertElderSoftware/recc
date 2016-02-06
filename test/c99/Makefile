.PRECIOUS: test/c99/%.o
.PRECIOUS: test/c99/%.exec

C99_SOURCES=$(shell ls test/c99/*.c)

test/c99/%.o: test/c99/%.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C99_FLAGS)

test/c99/%.exec: test/c99/%.o $(TESTING_EXTRAS_OBJECTS) $(COMPILER_OBJECTS)
	@$(HOSTCC) $< $(TESTING_EXTRAS_OBJECTS) $(COMPILER_OBJECTS) -o $@ $(CUSTOM_C99_FLAGS)

clean-c99-tests:
	
