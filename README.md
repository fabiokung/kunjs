Status
======

The parsing rules and grammar are based on the last ECMAScript specification.
Right now, the compiler is capable to parse a lot of valid JavaScript code. The current focus is on the AST generation.


How to compile and test
=======================

To build this project, you need [CMake](http://www.cmake.org/). It is fantastic btw. :-)

    cd <kunjs-project-dir>
    cmake .
    make

After compiling, to run the tests use:

    ./run-tests

Some of them may be still failing. Work in progress!

I strongly recommend you to take a look on some of the tests: `<kunjs-project-dir>/test/**/*_test.cc`

