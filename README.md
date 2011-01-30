Status
======

Parsing rules and grammar are based on the last ECMAScript specification.
Right now, this compiler is capable of parsing a lot of valid JavaScript code 
and the parser generates a compiler-agnostic Abstract Syntax Tree. It is even possible 
to plug in other compiler implementations that do not use LLVM.

The current focus is on the native code generation.


How to compile and test
=======================

To build this project, you need [CMake](http://www.cmake.org/). It is fantastic btw. :-)

Then you need to build LLVM in your platform (just once):

    cd <kunjs-project-dir>
    mkdir -p build/llvm
    cd build/llvm
    cmake ../../utils/llvm
    make

After you have built the LLVM project, you will be able to build everything else:

    cd <kunjs-project-dir>
    cmake .
    make

After this first build, you do not need to generate CMake files again.
A simple `make` in the project root dir is enough.

I strongly recommend you to take a look on some of the tests: `<kunjs-project-dir>/test/**/*_test.cc`.
Executables for each of them are generated in the top level directory.

