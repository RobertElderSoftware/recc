build-stdlib: recc
	$(VALGRIND) ./recc libc/build-script

libc/filesystem.h: bootstrap-datatypes
	@touch libc/filesystem.h

libc/filesystem.o: libc/filesystem.c libc/filesystem.h
	@$(HOSTCC) -c libc/filesystem.c -o libc/filesystem.o $(CUSTOM_FLAGS)

clean-stdlib:
	@rm -f libc/filesystem.l2 libc/filesystem.i libc/malloc.l2 libc/malloc.i libc/printf.i libc/printf.l2 libc/*.o
