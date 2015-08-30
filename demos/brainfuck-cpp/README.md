# Summary

This demo will use RECC's preprocessor to run a third party implementation of a Brainfuck interpreter that was implemented in the C preprocessor:

[https://github.com/orangeduck/CPP_COMPLETE](https://github.com/orangeduck/CPP_COMPLETE)

You should be able to go to your /tmp directory and run these commands verbatim to try out this example:

```
git clone https://github.com/RobertElderSoftware/recc.git
cd recc
git checkout f3a2033ccf2da44ad6186f6bb820930d8910d87b # Exact version of recc this demo was tested on
cd demos/brainfuck-cpp
git clone https://github.com/orangeduck/CPP_COMPLETE.git  #  Third party repo with C Preprocessor brainfuck interpreter
cd CPP_COMPLETE && git checkout 9ffa01b76d0723b687ca6c541935cf3b507e6e28  #  Exact version this was tested on
cd ../../../
make bootstrap-datatypes && make demos/brainfuck-cpp/build_brainfuck_cpp_demo
sed -i.bak 's/STDIN/(U,U,U,U,U,U,U,U,U,U,F,R,U,U,U,U,U,U,U,R,U,U,U,U,U,U,U,U,U,U,R,U,U,U,R,U,L,L,L,L,D,B,R,U,U,O,R,U,O,U,U,U,U,U,U,U,O,O,U,U,U,O,R,U,U,O,L,L,U,U,U,U,U,U,U,U,U,U,U,U,U,U,U,O,R,O,U,U,U,O,D,D,D,D,D,D,O,D,D,D,D,D,D,D,D,O,R,U,O)/' demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h
demos/brainfuck-cpp/build_brainfuck_cpp_demo  #  Takes 15 - 30 minutes
tail demos/brainfuck-cpp/result.i  # here is the result
```

# Details


To run this demo do:

```
git clone https://github.com/orangeduck/CPP_COMPLETE.git
```

inside of the 'demos/brainfuck-cpp/' folder.

Make sure we're talking about the same version of their code:

```
cd CPP_COMPLETE && git checkout 9ffa01b76d0723b687ca6c541935cf3b507e6e28 && cd ../
```

cd to RECC project root:

```
cd ../../
```

Build the RECC compiler and build process that will preprocess for us:

```
make demos/brainfuck-cpp/build_brainfuck_cpp_demo
```

Edit the value of 'STDIN' in the file

```
demos/brainfuck-cpp/CPP_COMPLETE/MAIN.h
```

to contain a valid brainfuck program, using the mapping described by the author here:

```
https://github.com/orangeduck/CPP_COMPLETE
```

For example, this will do 'Hello World!' (The RECC compiler is very slow, so this will take about 30 minutes to run!)  You might want to update the makefile to use the -O3 flag, this makes it about twice as fast.

```
(U,U,U,U,U,U,U,U,U,U,F,R,U,U,U,U,U,U,U,R,U,U,U,U,U,U,U,U,U,U,R,U,U,U,R,U,L,L,L,L,D,B,R,U,U,O,R,U,O,U,U,U,U,U,U,U,O,O,U,U,U,O,R,U,U,O,L,L,U,U,U,U,U,U,U,U,U,U,U,U,U,U,U,O,R,O,U,U,U,O,D,D,D,D,D,D,O,D,D,D,D,D,D,D,D,O,R,U,O)
```

Smaller tests can be found the project's TEST.h file
You can run now use the RECC compiler to preprocess MAIN.h by running the executable with this command:

```
demos/brainfuck-cpp/build_brainfuck_cpp_demo
```

The result will be stored in the demos/brainfuck-cpp/result.i file:

```
'H' 'e' 'l' 'l' 'o' ' ' 'W' 'o' 'r' 'l' 'd' '!'
```
