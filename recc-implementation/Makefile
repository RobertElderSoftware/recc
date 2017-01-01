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

recc-implementation/asm.o: recc-implementation/asm.h recc-implementation/asm.c
	@$(HOSTCC) -c recc-implementation/asm.c -o recc-implementation/asm.o $(CUSTOM_C89_FLAGS)

recc-implementation/asm_functions.o: recc-implementation/asm_functions.h recc-implementation/asm_functions.c
	@$(HOSTCC) -c recc-implementation/asm_functions.c -o recc-implementation/asm_functions.o $(CUSTOM_C89_FLAGS)

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

recc-implementation/filesystem_compiler.h: libc/filesystem.h
	@touch recc-implementation/filesystem_compiler.h

$(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.o: recc-implementation/filesystem_compiler.h $(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.c
	@$(HOSTCC) -c $(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.c -o $(CROSS_BUILD_PREFIX)recc-implementation/filesystem_compiler.o $(CUSTOM_C89_FLAGS)

recc-implementation/l2_parser.o: recc-implementation/l2_parser.h recc-implementation/l2_parser.c
	@$(HOSTCC) -c recc-implementation/l2_parser.c -o recc-implementation/l2_parser.o $(CUSTOM_C89_FLAGS)

clean-recc-implementation: clean-phase-1 clean-phase-2
	@rm -f recc-implementation/filesystem_impl.l2 

endif
endif
