run-java-emulator: emulators/java/OpCPU/KernelData.class emulators/java/OpCPU/LinuxEmulatorExample.class emulators/java/OpCPU/OpCPU.class emulators/java/OpCPU/OpCPUDataInterface.class
	@java -cp .:emulators/java OpCPU/LinuxEmulatorExample OpCPU/KernelData OpCPU/OpCPU OpCPU/OpCPUDataInterface

emulators/java/OpCPU/LinuxEmulatorExample.class: emulators/java/OpCPU/LinuxEmulatorExample.java
	@javac -cp .:emulators/java -Xlint emulators/java/OpCPU/LinuxEmulatorExample.java

emulators/java/OpCPU/OpCPU.class: emulators/java/OpCPU/OpCPU.java
	@javac -cp .:emulators/java -Xlint emulators/java/OpCPU/OpCPU.java

emulators/java/OpCPU/KernelData.class: emulators/java/OpCPU/KernelData.java
	@javac -cp .:emulators/java -Xlint emulators/java/OpCPU/KernelData.java

emulators/java/OpCPU/OpCPUDataInterface.class: emulators/java/OpCPU/OpCPUDataInterface.java
	@javac -cp .:emulators/java -Xlint emulators/java/OpCPU/OpCPUDataInterface.java

emulators/java/build_java_example: emulators/java/build_java_example.o $(COMPILER_OBJECTS)
	@$(HOSTCC) -o emulators/java/build_java_example emulators/java/build_java_example.o $(COMPILER_OBJECTS)  $(CUSTOM_C89_FLAGS)

emulators/java/build_java_example.o: emulators/java/build_java_example.c compiler_interface.h
	@$(HOSTCC) -c emulators/java/build_java_example.c -o emulators/java/build_java_example.o $(CUSTOM_C89_FLAGS)

emulators/java/OpCPU/KernelData.java: emulators/java/build_java_example
	@./emulators/java/build_java_example

clean-java-emulator:
	@rm -f emulators/java/build_java_example.o emulators/java/build_java_example emulators/java/OpCPU/*.class emulators/java/OpCPU/KernelData.java 
