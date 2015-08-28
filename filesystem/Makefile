filesystem/filesystem_compiler.h: bootstrap-datatypes libc/filesystem.h
	@touch filesystem/filesystem_compiler.h

filesystem/filesystem_compiler.o: filesystem/filesystem_compiler.h filesystem/filesystem_compiler.c
	@$(HOSTCC) -c filesystem/filesystem_compiler.c -o filesystem/filesystem_compiler.o $(CUSTOM_C89_FLAGS)

filesystem/recc.o: filesystem/recc.c compiler_interface.h
	@$(HOSTCC) -c filesystem/recc.c -o filesystem/recc.o $(CUSTOM_C89_FLAGS)

clean-filesystem:
	@rm -f filesystem/recc.o filesystem/filesystem_compiler filesystem/filesystem_compiler.o filesystem/filesystem_impl.l2 filesystem/filesystem.o
