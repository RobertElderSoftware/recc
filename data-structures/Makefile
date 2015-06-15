-include data-structures/library-data-structures
-include data-structures/object-data-structures
-include data-structures/file-dependencies-data-structures
-include data-structures/clean-data-structures

data-structures/bootstrap.c: data-structures/replace_tool.h

bootstrap-datatypes: data-structures/bootstrap.c data-structures/replace_tool.o data-structures/binary_exponential_buffer.o 
	@$(HOSTCC) data-structures/binary_exponential_buffer.o data-structures/bootstrap.c data-structures/replace_tool.o -o data-structures/bootstrap-datatypes  $(CUSTOM_FLAGS)
	$(VALGRIND) data-structures/bootstrap-datatypes

data-structures/replace_tool.o: data-structures/binary_exponential_buffer.h data-structures/replace_tool.h data-structures/replace_tool.c
	@$(HOSTCC) -c data-structures/replace_tool.c -o data-structures/replace_tool.o $(CUSTOM_FLAGS)

data-structures/binary_exponential_buffer.o: data-structures/binary_exponential_buffer.h data-structures/binary_exponential_buffer.c
	@$(HOSTCC) -c data-structures/binary_exponential_buffer.c -o data-structures/binary_exponential_buffer.o $(CUSTOM_FLAGS)

data-structures/memory_pooler.h: data-structures/struct_memory_pooler_ptr_list.h
data-structures/memory_pooler.o: data-structures/memory_pooler.h data-structures/memory_pooler.c
	@$(HOSTCC) -c data-structures/memory_pooler.c -o data-structures/memory_pooler.o $(CUSTOM_FLAGS)

clean-data-structures: clean-generated-files
	@rm -f data-structures/*.o data-structures/libgenerated-data-structures.a data-structures/bootstrap-datatypes
