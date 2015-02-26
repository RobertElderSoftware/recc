* Project Home *

http://recc.robertelder.org/

* Contributions *

Do not create pull requests for this project.  Contributing to this project is not currently permitted.

* License *

See LICENSE.txt

* Installation *

It is expected that you'll be developing on linux.  No other platform is currently supported.

The installation steps will depend on whether you plan to do development work on the compiler, or if you want to simply use the compiler to compile code.

If you just want to build and run the compiler, you can do so with

make bootstrap-datatypes && make recc

'make bootstrap-datatypes' is only necessary the first time you build, because this command creates makefiles that are required for building the compiler

If you want to do full testing of cross compiled code in the browser, it would be helpful to know the testing process.

In order to test the compiler on real C code, the following steps take place:

1)  The recc executable is built.
2)  The recc executable is used to preprocess .i files from all of the .c files in the 'test' directory.
3)  The recc executable is used to build .l2 files from all of the .i files in the 'test' directory.
4)  The recc executable is used to build .l1 files for each test in the 'test' directory.
5)  Once all .l1 files have been built, chrome is envoked directly from the makefile, and chrome is instructed to browse to a url served by localhost, which serves files in the test-api-endpoint directory.  This page contains javascript that interacts with the test API to obtain a list of all available test cases, then run each test and submit the result in the API.  The test result from running the compiled code in the javascript emulator is saved into the 'test' directory.
6)  Each of the .c files in the 'test' are compiled using gcc into standard desktop executables.
7)  The executables created using gcc are then run, and output is saved to the 'test' directory.
8)  The saved output from javascript and gcc are diff'ed.  If the diff is not empty, the test failed.

To set up the testing API, set up a web server and point it to the root directory of this project.  The API uses php as a scripting language, so you'll need to make sure this is set up to work with your web server.  You may need to adjust permissions on the 'test' directory since php will need to write files that contain test results here.

Additionally, you'll find that when you do test directly from the makefile using chrome, chrome does not automatically close when the tests are complete.  To solve this problem, a chrome plugin has been created that automatically closes the chrome window when javascript detects that all the test have finished running.  This plugin is located in 'chrome/chrome-plugin'.  Note that when chrome is run from the make file that a data dir is specified that resides in the project folder so that you'll be running a separate chrome profile from the one you use for personal use.  This is necessary so that the chrome session doesn't interfere with any casual browsing you're doing while you're developing, and avoids stacking your testing chrome session as a new tab in your already open windows.  Make sure you install the chrome plugin in the chrome instance that runs from the makefile.

Additionally, you will likely find that you need to set more open file premissions to allow php to write to test results to the 'test' directory:

chmod o+rwx test

* The Compiler *

The recc compiler is far from being a production compiler and has too many bugs to list.  It was build with the intention of one day being fully ISO c89 compliant.  Much of the error handling currently exists in the form of assertion failures, and attempts to use many unimplemented features will result in silent failures.

The following things are supported:

-  signed and unsigned int, char, short, enum, multidimensional arrays, structures.
-  Nearly all arithmetic and logical operators for signed and unsigned types.
-  nested structure declarations.
-  function pointers.
-  simpile initializers (like int i = 123).
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
-  The simplest cases of a few preprocessor directives (#define ONE 1, #include "inc.h", #ifndef, #endif)
-  typedef
-  Some constant initializers for globals like (arr[3] = {1,2,3})
-  anonymous structure instances
-  Complex declarators like int (*(*foo[6])(int))(void);

The following things are current not implemented/supported:

-  K&R C style function definitions
-  bitfields
-  goto
-  unions
-  long and long long types
-  float types (float, double, long double)
-  The standard library.  Only a small subset of printf is currently implemented. 
-  The compiler is not yet self-hosting.  This is mainly due to the unimplemented parts of the c standard library.  It may also be necessary to introduce some optimizations in order to reduce the size of the generated code.

* The Kernel *

Although the compiler has its limitations, it has been used to build a functioning toy microkernel.  The kernel runs inside a javascript emulator of the 'One-Page CPU'.  The kernel can be built with 

make build-kernel

If you have the testing API configured, you can run the kernel with

make kernel

* Using the compiler *

Typically when you use gcc or clang you'll pass file arguments in a way something like this:

gcc -c file.c

With recc, you pass a name of file that contains a build script.  recc only takes one argument.

./recc build-script

The build-script is a significantly less featureful alternative to a makefile, but unlike a makefile the compiler is aware of the build process instead of a separate program like with GNU make.

I have refrained from creating a specification for the build script syntax, since I may choose to change it in the future.

Current build script commands are simple declarative statements that look like this:

PREPROCESS main.c TO main.i
CODE GENERATE main.i TO main.l2
LINK library.l2, main.l2 TO test/basic-operations.l1 SYMBOLS TO main.symbols

*.i files are preprocessed c files that contain 'pure c' with no preprocessor directives.

*.l2 files are the recc equivalent of object files, but look like assembly files.  They contain all the assembly code for the final program, but many address and values may be stated in terms of abstract symbols.  A further linkage step is necessary to produce l1 files.

*.l1 files do no contain any symbol information.  They are the equivalent of a compiled binary, but they still look like assembly code and the syntax is much less forgiving that the l2 file format. 

* Emulators *

There are currently emulators for the op-cpu in 2 different languages.  C89 and javascript.  The javascript emulator runs as part of the unit tests, and a useful example of the C emulator on Linux can be built on linux with 

make emulators/linux-emulator-example

This emulator can 'run' .l1 files directly from the command line:

./linux-emulator-example file.l1

You can even run the full RECC microkernel from the command line:

make build-kernel && make ./emulators/linux-emulator-example && ./emulators/linux-emulator-example ./kernel/kernel.l1

