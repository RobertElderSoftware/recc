.PRECIOUS: %.o
.PRECIOUS: %.gcc
.PRECIOUS: %.gccoutput

# File names
SOURCES = $(wildcard test/*.c)
BUILTIN_SOURCES= $(wildcard builtin/*.c)
OTHER_OBJECTS=code_generator.o preprocessor.o parser.o io.o lexer.o data-structures/binary_exponential_buffer.o data-structures/memory_pooler.o data-structures/libgenerated-data-structures.a 
BUILTIN_OBJECTS= $(BUILTIN_SOURCES:.c=.o)
PREPROCESSED= $(SOURCES:.c=.i)
ASSEMBLY= $(SOURCES:.c=.l2)
CLEAN_ASSEMBLY= $(SOURCES:.c=.l1)
GCC_EXECUTABLES= $(SOURCES:.c=.gcc)
GCC_OUTPUT= $(GCC_EXECUTABLES:.gcc=.gccoutput)
JS_OUTPUT= $(CLEAN_ASSEMBLY:.l1=.jsoutput)
JS_PRELOADER= $(CLEAN_ASSEMBLY:.l1=.l0.js)
OUTPUT_DIFFS= $(JS_OUTPUT:.jsoutput=.diff)

# Main target
run-tests: $(OUTPUT_DIFFS)


remove-old-js-and-diffs:
	@rm -f $(JS_OUTPUT) $(OUTPUT_DIFFS)

run-build-script: $(GCC_OUTPUT) $(SOURCES) $(CUSTOM_CC) build-builtins build-stdlib
	$(VALGRIND) $(CUSTOM_CC) test/build-script

$(CLEAN_ASSEMBLY): run-build-script

%.l0.js: %.l1 preloader
	@$(VALGRIND) $(CUSTOM_PRELOADER) jsonpCallback $< $@ jsonp

# To obtain gcc objects of executables
%.o: %.c
	@$(HOSTCC) -c $< -o $@ $(CUSTOM_FLAGS)

$(GCC_EXECUTABLES): $(BUILTIN_OBJECTS) $(OTHER_OBJECTS)

%.gcc: %.o
	@$(HOSTCC) $< $(BUILTIN_OBJECTS) $(OTHER_OBJECTS) -o $@ $(CUSTOM_FLAGS)

# To obtain gcc output
%.gccoutput: %.gcc recc
	@./$< > $@

# To obtain js vm output
js-output: $(CLEAN_ASSEMBLY) $(JS_PRELOADER)
#  The use of this data dir will make the browser tests blocking if there is already a chrome window open.
	@google-chrome "http://localhost/emulators/javascript/" --user-data-dir="chrome/data-dir" > /dev/null 2>&1

$(JS_OUTPUT): js-output

# To obtain js vm output
%.diff: %.gccoutput %.jsoutput
	@diff $< $(word 2,$^) > $@ || echo "\033[0;31mTest Failure for "$<" \033[0m"

# To remove generated files
clean-tests:
	@rm -f test/*.l2 test/*.i test/*.o $(PREPROCESSED) $(ASSEMBLY) $(CLEAN_ASSEMBLY) $(GCC_EXECUTABLES) $(GCC_OUTPUT) $(JS_PRELOADER) $(JS_OUTPUT) $(OUTPUT_DIFFS)
