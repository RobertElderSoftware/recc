emulators/linux-emulator-example.o: emulators/linux-emulator-example.c emulators/op-cpu.h
	@$(HOSTCC) -c emulators/linux-emulator-example.c -o emulators/linux-emulator-example.o $(CUSTOM_FLAGS)

emulators/op-cpu.o: emulators/op-cpu.c emulators/op-cpu.h
	@$(HOSTCC) -c emulators/op-cpu.c -o emulators/op-cpu.o $(CUSTOM_FLAGS)

emulators/linux-emulator-example: emulators/op-cpu.o emulators/linux-emulator-example.o
	@$(HOSTCC) emulators/op-cpu.o emulators/linux-emulator-example.o -o emulators/linux-emulator-example

clean-emulators:
	@rm -f emulators/linux-emulator-example emulators/*.o
