demos/brainfuck-cpp/build_brainfuck_cpp_demo: demos/brainfuck-cpp/build_brainfuck_cpp_demo.o $(COMPILER_OBJECTS)
	@$(HOSTCC) -o demos/brainfuck-cpp/build_brainfuck_cpp_demo demos/brainfuck-cpp/build_brainfuck_cpp_demo.o $(COMPILER_OBJECTS) $(CUSTOM_C89_FLAGS)

demos/brainfuck-cpp/build_brainfuck_cpp_demo.o: demos/brainfuck-cpp/build_brainfuck_cpp_demo.c
	@$(HOSTCC) -c demos/brainfuck-cpp/build_brainfuck_cpp_demo.c -o demos/brainfuck-cpp/build_brainfuck_cpp_demo.o $(CUSTOM_C89_FLAGS)

clean-brainfuck-cpp:
	@rm -f demos/brainfuck-cpp/build_brainfuck_cpp_demo demos/brainfuck-cpp/build_brainfuck_cpp_demo.o demos/brainfuck-cpp/result.i
