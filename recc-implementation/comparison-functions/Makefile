COMPARISON_FUNCTION_SOURCES=$(wildcard recc-implementation/comparison-functions/*.c)
COMPARISON_FUNCTION_OBJECTS=$(COMPARISON_FUNCTION_SOURCES:.c=.o)

recc-implementation/comparison-functions/%.o: recc-implementation/comparison-functions/%.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_C89_FLAGS)
