filesystem/filesystem_impl.l2: filesystem/filesystem_compiler
	@$(VALGRIND) filesystem/filesystem_compiler

filesystem/filesystem_compiler.h: bootstrap-datatypes libc/filesystem.h
	@touch filesystem/filesystem_compiler.h

filesystem/filesystem_compiler.o: filesystem/filesystem_compiler.h filesystem/filesystem_compiler.c
	@$(HOSTCC) -c filesystem/filesystem_compiler.c -o filesystem/filesystem_compiler.o $(CUSTOM_FLAGS)

filesystem/filesystem_compiler: libc/filesystem.o data-structures/binary_exponential_buffer.o filesystem/filesystem_compiler.o io.o data-structures/libgenerated-data-structures.a
	@$(HOSTCC) -o filesystem/filesystem_compiler libc/filesystem.o data-structures/binary_exponential_buffer.o io.o filesystem/filesystem_compiler.o data-structures/libgenerated-data-structures.a $(CUSTOM_FLAGS)

clean-filesystem:
	@rm -f filesystem/filesystem_compiler filesystem/filesystem_compiler.o filesystem/filesystem_impl.l2 filesystem/filesystem.o
