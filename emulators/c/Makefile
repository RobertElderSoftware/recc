emulators/c/linux-emulator-example: emulators/c/op-cpu.o emulators/c/linux-emulator-example.o emulators/c/linux-emulator-example-data.o
	@$(HOSTCC) emulators/c/op-cpu.o emulators/c/linux-emulator-example.o emulators/c/linux-emulator-example-data.o -o emulators/c/linux-emulator-example

emulators/c/linux-emulator-example.o: emulators/c/linux-emulator-example.c emulators/c/op-cpu.h
	@$(HOSTCC) -c emulators/c/linux-emulator-example.c -o emulators/c/linux-emulator-example.o $(CUSTOM_C89_FLAGS)

emulators/c/op-cpu.o: emulators/c/op-cpu.c emulators/c/op-cpu.h
	@$(HOSTCC) -c emulators/c/op-cpu.c -o emulators/c/op-cpu.o $(CUSTOM_C89_FLAGS)

emulators/c/kernel.l0.c: emulators/c/build_c_example
	$(VALGRIND) ./emulators/c/build_c_example

emulators/c/build_c_example: emulators/c/build_c_example.o $(COMPILER_OBJECTS)
	@$(HOSTCC) $(COMPILER_OBJECTS) emulators/c/build_c_example.o -o emulators/c/build_c_example $(CUSTOM_C89_FLAGS)

emulators/c/build_c_example.o: emulators/c/build_c_example.c recc-implementation/compiler_interface_phase_3.h
	@$(HOSTCC) -c emulators/c/build_c_example.c -o emulators/c/build_c_example.o $(CUSTOM_C89_FLAGS)

emulators/c/linux-emulator-example-data.o: emulators/c/kernel.l0.c
	@$(HOSTCC) -c emulators/c/kernel.l0.c -o emulators/c/linux-emulator-example-data.o $(CUSTOM_C89_FLAGS)

run-c-emulator: emulators/c/linux-emulator-example
	@emulators/c/linux-emulator-example

clean-c-emulator:
	@rm -f emulators/c/kernel.l0.c emulators/c/*.o emulators/c/build_c_example emulators/c/linux-emulator-example
