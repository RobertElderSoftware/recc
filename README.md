# Contributions

DO NOT CREATE PULL REQUESTS FOR THIS PROJECT.  ANY PULL REQUESTS YOU CREATE WILL NOT BE MERGED IN.

Contributing to this project is not currently permitted.

# What Can This Project Do?

Project home: http://recc.robertelder.org/

-  Describes a tiny CPU spec (the 'One-Page CPU') that can be implemented in less than 500 lines of code.
-  The CPU supports 14 instructions, context switching, virtual memory, interrupts.
-  Minimal microkernel (written in C) that runs on the One-Page CPU.
-  An compiler for a (non-trivial subset) of ANSI C89 that targets the One-Page CPU.
-  The compiler includes a linker.
-  Emulators for the One-Page CPU written in 4 different languages: C89, Python, Java, Javascript
-  Ability to statically compile files from host filesystem into One-Page CPU executables, accessed through 'fopen' (file reading only).
-  Entire project only depends on C standard library.

# Warning

If you want to use this project for something, your best bet is to file a bug asking how you could do it, instead of trying to figure out how to this software.  This way, I get feedback that someone actually wants to use it for something too.

This project is unlikely to be interesting or usable by anyone who is not among the most hard-core programmers.  If you enjoy sitting at home, writing code on a Friday night then this project is for you!  Many aspects of the compiler are undocumented and I change them constantly.  This is often because I haven't yet figured out what the best approach is, and I want to avoid creating wasteful documentation that will be wrong shortly in the future.  As I become more confident about what the best approach is, I will add documentation explaining how it works.  If you want to know how something works, the best place to look is in the Makefiles to see how things are being built.

This project is in a constant state of flux and there is no guarantee provided in terms of backward compatibility (nobody really uses it for anything serious anyway).  The eventual goal will be to move toward standardizing certain things, but for now I tend to use this project as a playground to try out completely new ways of doing things.  Sometimes this works out, and sometimes it doesn't.


# License

See LICENSE.txt

You can also find the source code available in a number of other licenses:  http://recc.robertelder.org/other-licenses/

# Developer Set Up

It is expected that you'll be developing on linux.  No other platform is currently supported.

The installation steps will depend on whether you plan to do development work on the compiler, or if you want to simply use the compiler to compile code.

If you want to do full testing of cross compiled code in the browser, it would be helpful to know the testing process.

In order to test the compiler on real C code using the Javascript emulator, the following steps take place:

- 1)  For tests, and executable called 'build_tests' is created.  This executable is a 'compiler' with the test build process hard coded into it.
- 2)  The recc executable is used to preprocess .i files from all of the .c files in the 'test' directory.
- 3)  The recc executable is used to build .l2 files from all of the .i files in the 'test' directory.
- 4)  The recc executable is used to build .l1 files for each test in the 'test' directory.
- 5)  .l0.js files are created for each test in the 'test' directory.
- 6)  Once all .l0.js files have been built, chrome is envoked directly from the makefile, and chrome is instructed to browse to a url served by localhost, which serves files in the test-api-endpoint directory.  This page contains javascript that interacts with the test API to obtain a list of all available test cases, then run each test and submit the result in the API.  The test result from running the compiled code in the javascript emulator is saved into the 'test' directory.
- 7)  Each of the .c files in the 'test' are compiled using (your host compiler) into standard desktop executables.
- 8)  The executables created using gcc are then run, and output is saved to the 'test' directory.
- 9)  The saved output from javascript and gcc are diff'ed.  If the diff is not empty, the test failed.

To set up the testing API, set up a web server and point it to the root directory of this project.  The API uses php as a scripting language, so you'll need to make sure this is set up to work with your web server.  You may need to adjust permissions on the 'test' directory since php will need to write files that contain test results here.

Additionally, you'll find that when you do test directly from the makefile using chrome, chrome does not automatically close when the tests are complete.  To solve this problem, a chrome plugin has been created that automatically closes the chrome window when javascript detects that all the test have finished running.  This plugin is located in 'chrome/chrome-plugin'.  Note that when chrome is run from the make file that a data dir is specified that resides in the project folder so that you'll be running a separate chrome profile from the one you use for personal use.  This is necessary so that the chrome session doesn't interfere with any casual browsing you're doing while you're developing, and avoids stacking your testing chrome session as a new tab in your already open windows.  Make sure you install the chrome plugin in the chrome instance that runs from the makefile.

Additionally, you will likely find that you need to set more open file premissions to allow php to write to test results to the 'test' directory:

chmod o+rwx test

# The Compiler

The recc compiler is far from being a production compiler and has too many bugs to list.  It was build with the intention of one day being fully ISO c89 compliant.  Much of the error handling currently exists in the form of assertion failures, and attempts to use many unimplemented features will result in silent failures.

The following things are supported:

-  signed and unsigned int, char, short, enum, multidimensional arrays, structures.
-  Nearly all arithmetic and logical operators for signed and unsigned types.
-  nested structure declarations.
-  function pointers.
-  Many initializers (like int arr[2][2] = {{1,2},{1,2}})
-  Variadic functions with simple word sized arguments.
-  Pointers.
-  while () {}
-  do {} while ();
-  for(;;)
-  for(;;;)
-  switch statements
-  break and continue statements
-  Global variables
-  Global variables linked from external symbols.
-  The preprocessor supports recursive function macros
-  typedef
-  Some constant initializers for globals like (arr[3] = {1,2,3})
-  anonymous structure instances
-  Complex declarators like int (*(*foo[6])(int))(void);
-  sizeof(...)

The following things are current not implemented/supported:

-  K&R C style function definitions
-  bitfields
-  goto
-  unions
-  long and long long types
-  float types (float, double, long double)
-  The standard library.  Only a small subset of printf and some filesystem operations are currently implemented.  malloc and free are partially supported.
-  The compiler is not yet completely self-hosting.  This is mainly due to the currently impractical nature of running large programs compiled with the RECC compiler.  Optimizations will need to be performed before self compilation can be done in a way that is useful.

# The Kernel

Although the compiler has its limitations, it has been used to build a functioning toy microkernel.  The kernel runs inside a javascript emulator of the 'One-Page CPU'.  You can build and run the kernel with

make run-c-emulator

If you have the testing API configured, you can run the kernel with

make kernel

#  Using The Compiler

I recently made many changes to the build process, and I'll likely make many more in the near future.  Consulting the Makefiles should be the authoritative source on how to use the compiler.  At the moment, compiling code with the recc compiler involves creating compiling a 'custom' compiler by specifying dependencies in your project, then running the resulting executable to cross compile your code.

*.i files are preprocessed c files that contain 'pure c' with no preprocessor directives.

*.l2 files are the recc equivalent of object files, but look like assembly files.  They contain all the assembly code for the final program, but many address and values may be stated in terms of abstract symbols.  A further linkage step is necessary to produce l1 files.

*.l1 files do no contain any symbol information.  They are the equivalent of a compiled binary, but they still look like assembly code and the syntax is much less forgiving that the l2 file format. 

*l0.<language extension> files are programming language specific files that are used to simplify the process of loading an executable into an emulator written in a given target language.

# Emulators

There are currently emulators for the op-cpu in 4 different languages.  C89, Javascript, Python and Java.  The javascript emulator runs as part of the unit tests.  The other 3 emulators can be run with these commands:

Then

- make run-c-emulator       # Runs the kernel inside the C emulator
- make run-python-emulator  # Runs the kernel inside the Python emulator
- make run-java-emulator    # Runs the kernel inside the Java emulator
