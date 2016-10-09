ifeq ("$(wildcard recc-implementation/bootstrap_phase_1)","")
include recc-implementation/Phase1-Makefile
clean: clean-phase-1
	@:
else
ifeq ("$(wildcard recc-implementation/bootstrap_phase_2)","")
include recc-implementation/Phase2-Makefile
include recc-implementation/Phase1-Makefile
include recc-implementation/library-data-structures
include recc-implementation/object-data-structures
include recc-implementation/file-dependencies-data-structures
clean: clean-phase-1 clean-phase-2
	@:
else
include recc-implementation/Phase2-Makefile
include recc-implementation/Phase1-Makefile

RECC_IMPLEMENTATION_OBJECTS=recc-implementation/type_engine.o recc-implementation/regex_engine.o recc-implementation/compiler_interface_common.o recc-implementation/compiler_interface_c_compiler_targets.o recc-implementation/compiler_interface_header_targets.o recc-implementation/compiler_interface_phase_3.o recc-implementation/lexer.o recc-implementation/parser.o recc-implementation/code_generator.o recc-implementation/linker.o recc-implementation/io.o recc-implementation/l0_generator.o recc-implementation/preprocessor.o recc-implementation/memory_pool_collection.o recc-implementation/heap_memory_pool.o $(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.o recc-implementation/libbootstrap.o recc-implementation/l2_parser.o recc-implementation/replace_tool.o recc-implementation/binary_exponential_buffer.o $(COMPARISON_FUNCTION_OBJECTS)

recc-implementation/librecc.a: $(RECC_IMPLEMENTATION_OBJECTS) $(DATA_STRUCTURES_OBJECT_FILES) $(BUILTIN_OBJECTS)
	@ar -rcs recc-implementation/librecc.a $(RECC_IMPLEMENTATION_OBJECTS) $(DATA_STRUCTURES_OBJECT_FILES) $(BUILTIN_OBJECTS)

$(CROSS_BUILD_PREFIX)recc-implementation/recc.o: $(CROSS_BUILD_PREFIX)recc-implementation/recc.c
	@$(HOSTCC) -c $(CROSS_BUILD_PREFIX)recc-implementation/recc.c -o $(CROSS_BUILD_PREFIX)recc-implementation/recc.o $(CUSTOM_C89_FLAGS)

recc-implementation/compiler_interface_phase_3.o: recc-implementation/compiler_interface_phase_3.c recc-implementation/compiler_interface_phase_3.h
	@$(HOSTCC) -c recc-implementation/compiler_interface_phase_3.c -o recc-implementation/compiler_interface_phase_3.o $(CUSTOM_C89_FLAGS)

recc-implementation/compiler_interface_c_compiler_targets.o: recc-implementation/compiler_interface_c_compiler_targets.c recc-implementation/compiler_interface_c_compiler_targets.h
	@$(HOSTCC) -c recc-implementation/compiler_interface_c_compiler_targets.c -o recc-implementation/compiler_interface_c_compiler_targets.o $(CUSTOM_C89_FLAGS)

recc-implementation/lexer.h: recc-implementation/io.h
	@touch recc-implementation/lexer.h

recc-implementation/regex_engine.o: recc-implementation/regex_engine.h recc-implementation/regex_engine.c
	@$(HOSTCC) -c recc-implementation/regex_engine.c -o recc-implementation/regex_engine.o $(CUSTOM_C89_FLAGS)

recc-implementation/type_engine.o: recc-implementation/type_engine.h recc-implementation/type_engine.c
	@$(HOSTCC) -c recc-implementation/type_engine.c -o recc-implementation/type_engine.o $(CUSTOM_C89_FLAGS)

recc-implementation/lexer.o: recc-implementation/lexer.h recc-implementation/lexer.c
	@$(HOSTCC) -c recc-implementation/lexer.c -o recc-implementation/lexer.o $(CUSTOM_C89_FLAGS)

recc-implementation/parser.h: recc-implementation/io.h recc-implementation/lexer.h
	@touch recc-implementation/parser.h

recc-implementation/parser.o: recc-implementation/parser.h recc-implementation/parser.c
	@$(HOSTCC) -c recc-implementation/parser.c -o recc-implementation/parser.o $(CUSTOM_C89_FLAGS)

recc-implementation/code_generator.o: recc-implementation/code_generator.h recc-implementation/code_generator.c
	@$(HOSTCC) -c recc-implementation/code_generator.c -o recc-implementation/code_generator.o $(CUSTOM_C89_FLAGS)

recc-implementation/code_generator.h: recc-implementation/parser.h recc-implementation/linker.h recc-implementation/lexer.h
	@touch recc-implementation/code_generator.h

recc-implementation/linker.h: recc-implementation/io.h recc-implementation/lexer.h recc-implementation/parser.h
	@touch recc-implementation/linker.h

recc-implementation/linker.o: recc-implementation/linker.c recc-implementation/linker.h
	@$(HOSTCC) -c recc-implementation/linker.c -o recc-implementation/linker.o $(CUSTOM_C89_FLAGS)

recc-implementation/io.h:
	@touch recc-implementation/io.h

recc-implementation/compiler_interface_phase_3.h:
	@touch recc-implementation/compiler_interface_phase_3.h

recc-implementation/io.o: recc-implementation/io.c recc-implementation/io.h
	@$(HOSTCC) -c recc-implementation/io.c -o recc-implementation/io.o $(CUSTOM_C89_FLAGS)

recc-implementation/l0_generator.h: recc-implementation/io.h
	@touch recc-implementation/l0_generator.h

recc-implementation/l0_generator.o: recc-implementation/l0_generator.c recc-implementation/l0_generator.h
	@$(HOSTCC) -c recc-implementation/l0_generator.c -o recc-implementation/l0_generator.o $(CUSTOM_C89_FLAGS)

recc-implementation/preprocessor.h: recc-implementation/io.h
	@touch recc-implementation/preprocessor.h

recc-implementation/preprocessor.o: recc-implementation/preprocessor.c recc-implementation/preprocessor.h
	@$(HOSTCC) -c recc-implementation/preprocessor.c -o recc-implementation/preprocessor.o $(CUSTOM_C89_FLAGS)

recc-implementation/memory_pool_collection.h:
	@touch recc-implementation/memory_pool_collection.h

recc-implementation/memory_pool_collection.c: recc-implementation/memory_pool_collection.h
	@touch recc-implementation/memory_pool_collection.c

recc-implementation/memory_pool_collection.o: recc-implementation/memory_pool_collection.c recc-implementation/memory_pool_collection.h
	@$(HOSTCC) -c recc-implementation/memory_pool_collection.c -o recc-implementation/memory_pool_collection.o $(CUSTOM_C89_FLAGS)

recc-implementation/heap_memory_pool.h:
	@touch recc-implementation/heap_memory_pool.h

recc-implementation/heap_memory_pool.o: recc-implementation/heap_memory_pool.c recc-implementation/heap_memory_pool.h
	@$(HOSTCC) -c recc-implementation/heap_memory_pool.c -o recc-implementation/heap_memory_pool.o $(CUSTOM_C89_FLAGS)

recc-implementation/filesystem_compiler.h: libc/filesystem.h
	@touch recc-implementation/filesystem_compiler.h

$(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.o: recc-implementation/filesystem_compiler.h $(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.c
	@$(HOSTCC) -c $(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.c -o $(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.o $(CUSTOM_C89_FLAGS)

recc-implementation/l2_parser.o: recc-implementation/l2_parser.h recc-implementation/l2_parser.c
	@$(HOSTCC) -c recc-implementation/l2_parser.c -o recc-implementation/l2_parser.o $(CUSTOM_C89_FLAGS)

clean-recc-implementation: clean-phase-1 clean-phase-2
	@rm -f recc-implementation/librecc.a recc-implementation/filesystem_impl.l2 

endif
endif
