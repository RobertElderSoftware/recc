-include data-structures/library-data-structures
-include data-structures/object-data-structures
-include data-structures/file-dependencies-data-structures
-include data-structures/clean-data-structures

data-structures/recc.o: data-structures/recc.c
	@$(HOSTCC) -c data-structures/recc.c -o data-structures/recc.o $(CUSTOM_C89_FLAGS)

data-structures/replace_tool.h: data-structures/binary_exponential_buffer.h
	@touch data-structures/replace_tool.h

data-structures/bootstrap.c: data-structures/replace_tool.h
	@touch data-structures/bootstrap.c

data-structures/bootstrap-datatypes: data-structures/bootstrap.c data-structures/replace_tool.o data-structures/binary_exponential_buffer.o 
	@$(HOSTCC) data-structures/binary_exponential_buffer.o data-structures/bootstrap.c data-structures/replace_tool.o -o data-structures/bootstrap-datatypes  $(CUSTOM_C89_FLAGS)
	$(VALGRIND) data-structures/bootstrap-datatypes

clean-data-structures: clean-generated-files
	@rm -f data-structures/*.o data-structures/libgenerated-data-structures.a data-structures/bootstrap-datatypes
