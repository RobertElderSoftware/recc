emulators/c/linux-emulator-example: emulators/c/op-cpu.o emulators/c/linux-emulator-example.o emulators/c/linux-emulator-example-data.o
	@$(HOSTCC) emulators/c/op-cpu.o emulators/c/linux-emulator-example.o emulators/c/linux-emulator-example-data.o -o emulators/c/linux-emulator-example -lpthread

emulators/c/linux-emulator-example.o: emulators/c/linux-emulator-example.c emulators/c/op-cpu.h
	@$(HOSTCC) -c emulators/c/linux-emulator-example.c -o emulators/c/linux-emulator-example.o $(CUSTOM_FLAGS)

emulators/c/op-cpu.o: emulators/c/op-cpu.c emulators/c/op-cpu.h
	@$(HOSTCC) -c emulators/c/op-cpu.c -o emulators/c/op-cpu.o $(CUSTOM_FLAGS)

emulators/c/linux-emulator-example-data.l0.c: preloader
	@$(CUSTOM_PRELOADER) data kernel/kernel.l1 emulators/c/linux-emulator-example-data.l0.c c

emulators/c/linux-emulator-example-data.o: emulators/c/linux-emulator-example-data.l0.c
	@$(HOSTCC) -c emulators/c/linux-emulator-example-data.l0.c -o emulators/c/linux-emulator-example-data.o $(CUSTOM_FLAGS)

run-c-emulator: emulators/c/linux-emulator-example
	@emulators/c/linux-emulator-example

clean-c-emulator:
	@rm -f emulators/c/*.o emulators/c/linux-emulator-example-data.l0.c emulators/c/linux-emulator-example
