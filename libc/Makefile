libc/filesystem.h:
	@touch libc/filesystem.h

libc/filesystem.o: libc/filesystem.c libc/filesystem.h
	@$(HOSTCC) -c libc/filesystem.c -o libc/filesystem.o $(CUSTOM_C89_FLAGS)

libc/recc.o: libc/recc.c recc-implementation/compiler_interface_phase_3.h
	@$(HOSTCC) -c libc/recc.c -o libc/recc.o $(CUSTOM_C89_FLAGS)

clean-stdlib:
	@rm -f libc/*.o libc/*.i libc/*.l2
