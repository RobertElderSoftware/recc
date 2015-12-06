RECC_BUILTIN=builtin/print_buff_add.o builtin/extern_test.o builtin/print_bits.o builtin/c99libc_in_c89.o builtin/print_hexadecimal.o builtin/print_string.o builtin/print_decimal.o builtin/random.o builtin/signed_division.o builtin/signed_greater_than.o builtin/signed_greater_than_or_equal_to.o builtin/signed_less_than.o builtin/signed_less_than_or_equal_to.o builtin/signed_modulo.o

recc-implementation/librecc.a: recc-implementation/compiler_interface.o recc-implementation/lexer.o recc-implementation/parser.o recc-implementation/code_generator.o recc-implementation/linker.o recc-implementation/io.o recc-implementation/preloader.o recc-implementation/preprocessor.o recc-implementation/memory_pool_collection.o recc-implementation/heap_memory_pool.o recc-implementation/filesystem_compiler.o $(DATA_STRUCTURES_OBJECT_FILES) $(RECC_BUILTIN)
	@ar -rcs recc-implementation/librecc.a recc-implementation/compiler_interface.o recc-implementation/lexer.o recc-implementation/parser.o recc-implementation/code_generator.o recc-implementation/linker.o recc-implementation/io.o recc-implementation/preloader.o recc-implementation/preprocessor.o recc-implementation/memory_pool_collection.o recc-implementation/heap_memory_pool.o recc-implementation/filesystem_compiler.o $(DATA_STRUCTURES_OBJECT_FILES) $(RECC_BUILTIN)
recc-implementation/recc.o: recc-implementation/recc.c
	@$(HOSTCC) -c recc-implementation/recc.c -o recc-implementation/recc.o $(CUSTOM_C89_FLAGS)

recc-implementation/compiler_interface.o: recc-implementation/compiler_interface.c recc-implementation/compiler_interface.h
	@$(HOSTCC) -c recc-implementation/compiler_interface.c -o recc-implementation/compiler_interface.o $(CUSTOM_C89_FLAGS)

recc-implementation/lexer.h: data-structures/bootstrap-datatypes recc-implementation/io.h
	@touch recc-implementation/lexer.h

recc-implementation/lexer.o: recc-implementation/lexer.h recc-implementation/lexer.c
	@$(HOSTCC) -c recc-implementation/lexer.c -o recc-implementation/lexer.o $(CUSTOM_C89_FLAGS)

recc-implementation/parser.h: data-structures/bootstrap-datatypes recc-implementation/io.h recc-implementation/lexer.h
	@touch recc-implementation/parser.h

recc-implementation/parser.o: recc-implementation/parser.h recc-implementation/parser.c
	@$(HOSTCC) -c recc-implementation/parser.c -o recc-implementation/parser.o $(CUSTOM_C89_FLAGS)

recc-implementation/code_generator.o: recc-implementation/code_generator.h recc-implementation/code_generator.c
	@$(HOSTCC) -c recc-implementation/code_generator.c -o recc-implementation/code_generator.o $(CUSTOM_C89_FLAGS)

recc-implementation/code_generator.h: data-structures/bootstrap-datatypes recc-implementation/parser.h recc-implementation/linker.h recc-implementation/lexer.h
	@touch recc-implementation/code_generator.h

recc-implementation/linker.h: data-structures/bootstrap-datatypes recc-implementation/io.h recc-implementation/lexer.h recc-implementation/parser.h
	@touch recc-implementation/linker.h

recc-implementation/linker.o: recc-implementation/linker.c recc-implementation/linker.h
	@$(HOSTCC) -c recc-implementation/linker.c -o recc-implementation/linker.o $(CUSTOM_C89_FLAGS)

recc-implementation/io.h: data-structures/bootstrap-datatypes
	@touch recc-implementation/io.h

recc-implementation/compiler_interface.h: data-structures/bootstrap-datatypes
	@touch recc-implementation/compiler_interface.h

recc-implementation/io.o: recc-implementation/io.c recc-implementation/io.h data-structures/bootstrap-datatypes
	@$(HOSTCC) -c recc-implementation/io.c -o recc-implementation/io.o $(CUSTOM_C89_FLAGS)

recc-implementation/preloader.h: recc-implementation/io.h
	@touch recc-implementation/preloader.h

recc-implementation/preloader.o: recc-implementation/preloader.c recc-implementation/preloader.h
	@$(HOSTCC) -c recc-implementation/preloader.c -o recc-implementation/preloader.o $(CUSTOM_C89_FLAGS)

recc-implementation/preprocessor.h: recc-implementation/io.h
	@touch recc-implementation/preprocessor.h

recc-implementation/preprocessor.o: recc-implementation/preprocessor.c recc-implementation/preprocessor.h
	@$(HOSTCC) -c recc-implementation/preprocessor.c -o recc-implementation/preprocessor.o $(CUSTOM_C89_FLAGS)

recc-implementation/memory_pool_collection.h: data-structures/bootstrap-datatypes
	@touch recc-implementation/memory_pool_collection.h

recc-implementation/memory_pool_collection.c: recc-implementation/memory_pool_collection.h
	@touch recc-implementation/memory_pool_collection.c

recc-implementation/memory_pool_collection.o: recc-implementation/memory_pool_collection.c recc-implementation/memory_pool_collection.h
	@$(HOSTCC) -c recc-implementation/memory_pool_collection.c -o recc-implementation/memory_pool_collection.o $(CUSTOM_C89_FLAGS)

recc-implementation/heap_memory_pool.h: data-structures/bootstrap-datatypes
	@touch recc-implementation/heap_memory_pool.h

recc-implementation/heap_memory_pool.o: recc-implementation/heap_memory_pool.c recc-implementation/heap_memory_pool.h
	@$(HOSTCC) -c recc-implementation/heap_memory_pool.c -o recc-implementation/heap_memory_pool.o $(CUSTOM_C89_FLAGS)

recc-implementation/filesystem_compiler.h: data-structures/bootstrap-datatypes libc/filesystem.h
	@touch recc-implementation/filesystem_compiler.h

recc-implementation/filesystem_compiler.o: recc-implementation/filesystem_compiler.h recc-implementation/filesystem_compiler.c
	@$(HOSTCC) -c recc-implementation/filesystem_compiler.c -o recc-implementation/filesystem_compiler.o $(CUSTOM_C89_FLAGS)

clean-recc-implementation:
	@rm -f recc-implementation/librecc.a recc-implementation/filesystem_impl.l2 recc-implementation/*.o recc-implementation/*.l2 recc-implementation/*.i
