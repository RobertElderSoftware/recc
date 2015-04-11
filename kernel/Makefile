run-kernel:
	@google-chrome "http://localhost/emulators/javascript/?kernel_mode=1" --user-data-dir="chrome/data-dir" > /dev/null 2>&1

kernel/kernel.l1: $(CUSTOM_CC) build-builtins build-stdlib
	$(VALGRIND) $(CUSTOM_CC) kernel/build-script

build-and-test-kernel: kernel/kernel.l1 preloader
	cp kernel/kernel.l1 test/
	@$(CUSTOM_PRELOADER) jsonpCallback test/kernel.l1 test/kernel.l0.js jsonp
	@google-chrome "http://localhost/emulators/javascript/?kernel_mode=1" --user-data-dir="chrome/data-dir" > /dev/null 2>&1

clean-kernel:
	@rm -f kernel/main.l1 kernel/main.l2 kernel/kernel.l1 kernel/user_proc.l2 kernel/user_proc.i kernel/kernel_impl.i kernel/kernel_impl.l2 kernel/kernel_state.l2 kernel/kernel_state.i kernel/kernel.sym kernel/queue.i kernel/queue.l2 test/kernel.l1 kernel/main.i kernel/putchar.l2 kernel/putchar.i kernel/assert.l2 kernel/assert.i
