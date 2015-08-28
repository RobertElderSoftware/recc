run-kernel:
	@google-chrome "http://localhost/emulators/javascript/?kernel_mode=1" --user-data-dir="chrome/data-dir" > /dev/null 2>&1

kernel/kernel.l1: build_kernel
	$(VALGRIND) ./build_kernel

build-and-test-kernel: kernel/kernel.l1
	@google-chrome "http://localhost/emulators/javascript/?kernel_mode=1" --user-data-dir="chrome/data-dir" > /dev/null 2>&1

kernel/recc.o: kernel/recc.c compiler_interface.h
	@$(HOSTCC) -c kernel/recc.c -o kernel/recc.o $(CUSTOM_C89_FLAGS)

clean-kernel:
	@rm -f kernel/recc.o kernel/main.l1 kernel/main.l2 kernel/kernel.l1 kernel/user_proc.l2 kernel/user_proc.i kernel/kernel_impl.i kernel/kernel_impl.l2 kernel/kernel_state.l2 kernel/kernel_state.i kernel/kernel.sym kernel/queue.i kernel/queue.l2 test/kernel.l1 kernel/main.i kernel/putchar.l2 kernel/putchar.i test/kernel.l0.* recc.o
