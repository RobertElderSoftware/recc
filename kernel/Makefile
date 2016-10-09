kernel/build_kernel.o: kernel/build_kernel.c recc-implementation/compiler_interface_phase_3.h
	@$(HOSTCC) -c kernel/build_kernel.c -o kernel/build_kernel.o $(CUSTOM_C89_FLAGS)

kernel/build_kernel: kernel/build_kernel.o $(COMPILER_OBJECTS)
	@$(HOSTCC) -o kernel/build_kernel kernel/build_kernel.o $(COMPILER_OBJECTS) $(CUSTOM_C89_FLAGS)

kernel/kernel.l1: kernel/build_kernel
	$(VALGRIND) ./kernel/build_kernel
	@if [ "`diff kernel/kernel.sym kernel/kernel_symbols.sym | wc -l`" -eq 5 ]; then echo 'Kernel images are same.'; else echo 'Kernel images differ'; exit 1; fi

build-and-test-kernel: kernel/kernel.l1
	@echo "test/kernel" > test/tests_to_run
	@chromium-browser "http://localhost/emulators/javascript/?kernel_mode=1" --load-extension=emulators/javascript/chrome/chrome-plugin/ --user-data-dir="emulators/javascript/chrome/data-dir" > /dev/null 2>&1

$(CROSS_BUILD_PREFIX)kernel/recc.o: $(CROSS_BUILD_PREFIX)kernel/recc.c recc-implementation/compiler_interface_phase_3.h
	@$(HOSTCC) -c $(CROSS_BUILD_PREFIX)kernel/recc.c -o $(CROSS_BUILD_PREFIX)kernel/recc.o $(CUSTOM_C89_FLAGS)

kernel: build-and-test-kernel

clean-kernel:
	@rm -f kernel/build_kernel kernel/*.o kernel/*.l2 kernel/*.i kernel/*.l1 kernel/*.sym kernel/kernel_symbols.l0.c test/kernel.jsoutput test/kernel.l0.js
