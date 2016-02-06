.PRECIOUS: test/c89/%.o
.PRECIOUS: test/c89/%.exec

C89_SOURCES=$(shell ls test/c89/*.c)

test/c89/%.o: test/c89/%.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C89_FLAGS)

test/c89/%.exec: test/c89/%.o $(TESTING_EXTRAS_OBJECTS) $(COMPILER_OBJECTS)
	@$(HOSTCC) $< $(TESTING_EXTRAS_OBJECTS) $(COMPILER_OBJECTS) -o $@ $(CUSTOM_C89_FLAGS)

clean-c89-tests:
	
