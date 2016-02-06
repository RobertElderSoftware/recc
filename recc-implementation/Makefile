recc-implementation/librecc.a: recc-implementation/regex_engine.o recc-implementation/compiler_interface.o recc-implementation/lexer.o recc-implementation/parser.o recc-implementation/code_generator.o recc-implementation/linker.o recc-implementation/io.o recc-implementation/l0_generator.o recc-implementation/preprocessor.o recc-implementation/memory_pool_collection.o recc-implementation/heap_memory_pool.o recc-implementation/filesystem_compiler.o recc-implementation/l2_parser.o $(DATA_STRUCTURES_OBJECT_FILES) $(BUILTIN_OBJECTS)
	@ar -rcs recc-implementation/librecc.a recc-implementation/regex_engine.o recc-implementation/compiler_interface.o recc-implementation/lexer.o recc-implementation/parser.o recc-implementation/code_generator.o recc-implementation/linker.o recc-implementation/io.o recc-implementation/l0_generator.o recc-implementation/preprocessor.o recc-implementation/memory_pool_collection.o recc-implementation/heap_memory_pool.o recc-implementation/filesystem_compiler.o recc-implementation/l2_parser.o $(DATA_STRUCTURES_OBJECT_FILES) $(BUILTIN_OBJECTS)
recc-implementation/recc.o: recc-implementation/recc.c
	@$(HOSTCC) -c recc-implementation/recc.c -o recc-implementation/recc.o $(CUSTOM_C89_FLAGS)

recc-implementation/compiler_interface.o: recc-implementation/compiler_interface.c recc-implementation/compiler_interface.h
	@$(HOSTCC) -c recc-implementation/compiler_interface.c -o recc-implementation/compiler_interface.o $(CUSTOM_C89_FLAGS)

recc-implementation/lexer.h: data-structures/bootstrap recc-implementation/io.h
	@touch recc-implementation/lexer.h

recc-implementation/regex_engine.o: data-structures/bootstrap recc-implementation/regex_engine.h recc-implementation/regex_engine.c
	@$(HOSTCC) -c recc-implementation/regex_engine.c -o recc-implementation/regex_engine.o $(CUSTOM_C89_FLAGS)

recc-implementation/lexer.o: recc-implementation/lexer.h recc-implementation/lexer.c
	@$(HOSTCC) -c recc-implementation/lexer.c -o recc-implementation/lexer.o $(CUSTOM_C89_FLAGS)

recc-implementation/parser.h: data-structures/bootstrap recc-implementation/io.h recc-implementation/lexer.h
	@touch recc-implementation/parser.h

recc-implementation/parser.o: recc-implementation/parser.h recc-implementation/parser.c
	@$(HOSTCC) -c recc-implementation/parser.c -o recc-implementation/parser.o $(CUSTOM_C89_FLAGS)

recc-implementation/code_generator.o: recc-implementation/code_generator.h recc-implementation/code_generator.c
	@$(HOSTCC) -c recc-implementation/code_generator.c -o recc-implementation/code_generator.o $(CUSTOM_C89_FLAGS)

recc-implementation/code_generator.h: data-structures/bootstrap recc-implementation/parser.h recc-implementation/linker.h recc-implementation/lexer.h
	@touch recc-implementation/code_generator.h

recc-implementation/linker.h: data-structures/bootstrap recc-implementation/io.h recc-implementation/lexer.h recc-implementation/parser.h
	@touch recc-implementation/linker.h

recc-implementation/linker.o: recc-implementation/linker.c recc-implementation/linker.h
	@$(HOSTCC) -c recc-implementation/linker.c -o recc-implementation/linker.o $(CUSTOM_C89_FLAGS)

recc-implementation/io.h: data-structures/bootstrap
	@touch recc-implementation/io.h

recc-implementation/compiler_interface.h: data-structures/bootstrap
	@touch recc-implementation/compiler_interface.h

recc-implementation/io.o: recc-implementation/io.c recc-implementation/io.h data-structures/bootstrap
	@$(HOSTCC) -c recc-implementation/io.c -o recc-implementation/io.o $(CUSTOM_C89_FLAGS)

recc-implementation/l0_generator.h: recc-implementation/io.h
	@touch recc-implementation/l0_generator.h

recc-implementation/l0_generator.o: recc-implementation/l0_generator.c recc-implementation/l0_generator.h
	@$(HOSTCC) -c recc-implementation/l0_generator.c -o recc-implementation/l0_generator.o $(CUSTOM_C89_FLAGS)

recc-implementation/preprocessor.h: recc-implementation/io.h
	@touch recc-implementation/preprocessor.h

recc-implementation/preprocessor.o: recc-implementation/preprocessor.c recc-implementation/preprocessor.h
	@$(HOSTCC) -c recc-implementation/preprocessor.c -o recc-implementation/preprocessor.o $(CUSTOM_C89_FLAGS)

recc-implementation/memory_pool_collection.h: data-structures/bootstrap
	@touch recc-implementation/memory_pool_collection.h

recc-implementation/memory_pool_collection.c: recc-implementation/memory_pool_collection.h
	@touch recc-implementation/memory_pool_collection.c

recc-implementation/memory_pool_collection.o: recc-implementation/memory_pool_collection.c recc-implementation/memory_pool_collection.h
	@$(HOSTCC) -c recc-implementation/memory_pool_collection.c -o recc-implementation/memory_pool_collection.o $(CUSTOM_C89_FLAGS)

recc-implementation/heap_memory_pool.h: data-structures/bootstrap
	@touch recc-implementation/heap_memory_pool.h

recc-implementation/heap_memory_pool.o: recc-implementation/heap_memory_pool.c recc-implementation/heap_memory_pool.h
	@$(HOSTCC) -c recc-implementation/heap_memory_pool.c -o recc-implementation/heap_memory_pool.o $(CUSTOM_C89_FLAGS)

recc-implementation/filesystem_compiler.h: data-structures/bootstrap libc/filesystem.h
	@touch recc-implementation/filesystem_compiler.h

recc-implementation/filesystem_compiler.o: recc-implementation/filesystem_compiler.h recc-implementation/filesystem_compiler.c
	@$(HOSTCC) -c recc-implementation/filesystem_compiler.c -o recc-implementation/filesystem_compiler.o $(CUSTOM_C89_FLAGS)

recc-implementation/l2_parser.o: recc-implementation/l2_parser.h recc-implementation/l2_parser.c
	@$(HOSTCC) -c recc-implementation/l2_parser.c -o recc-implementation/l2_parser.o $(CUSTOM_C89_FLAGS)

clean-recc-implementation:
	@rm -f recc-implementation/librecc.a recc-implementation/filesystem_impl.l2 recc-implementation/*.o recc-implementation/*.l2 recc-implementation/*.i
