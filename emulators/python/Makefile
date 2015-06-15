emulators/python/op_cpu_package/python_l0_module.py: preloader
	@$(CUSTOM_PRELOADER) OpCPUData kernel/kernel.l1 emulators/python/op_cpu_package/python_l0_module.py python

run-python-emulator: emulators/python/op_cpu_package/python_l0_module.py
	@python emulators/python/linux-emulator-example.py

clean-python-emulator: 
	@rm -f emulators/python/op_cpu_package/*.pyc emulators/python/op_cpu_package/python_l0_module.py
