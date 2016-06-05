emulators/python/op_cpu_package/python_l0_module.py: emulators/python/build_python_example
	@./emulators/python/build_python_example

run-python-emulator: emulators/python/op_cpu_package/python_l0_module.py
	@python emulators/python/linux-emulator-example.py

emulators/python/build_python_example: emulators/python/build_python_example.o $(COMPILER_OBJECTS)
	@$(HOSTCC) -o emulators/python/build_python_example emulators/python/build_python_example.o $(COMPILER_OBJECTS)  $(CUSTOM_C89_FLAGS)

emulators/python/build_python_example.o: emulators/python/build_python_example.c recc-implementation/compiler_interface_phase_3.h
	@$(HOSTCC) -c emulators/python/build_python_example.c -o emulators/python/build_python_example.o $(CUSTOM_C89_FLAGS)

clean-python-emulator: 
	@rm -f emulators/python/build_python_example emulators/python/build_python_example.o emulators/python/op_cpu_package/*.pyc emulators/python/op_cpu_package/python_l0_module.py
