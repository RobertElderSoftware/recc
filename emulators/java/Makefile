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

emulators/java/OpCPU/KernelData.java: preloader
	@$(CUSTOM_PRELOADER) KernelData kernel/kernel.l1 emulators/java/OpCPU/KernelData.java java

clean-java-emulator:
	@rm -f emulators/java/OpCPU/*.class emulators/java/OpCPU/KernelData.java 
