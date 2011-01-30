cmake_minimum_required(VERSION 2.8)
project(kunjs)

find_package(GTest 1.5.0 REQUIRED)
find_package(Boost 1.45.0 REQUIRED)

set(LLVM_ROOT "${PROJECT_SOURCE_DIR}/build/llvm" CACHE PATH "Root of LLVM install.")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/share/llvm/cmake")
include(LLVM)

# required by LLVM
add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${GTEST_INCLUDE_DIRS}
    ${Boost_INCLUDES}
    ${LLVM_ROOT}/include)

link_directories(
    ${PROJECT_BINARY_DIR}
    ${Boost_LIBRARIES}
    ${LLVM_ROOT}/lib)

#file(GLOB_RECURSE COMPILER_SOURCES src/kunjs/compiler/*.cc)
llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES core jit native)

add_library(grammar src/kunjs/grammar.cc)
add_library(printer src/kunjs/printer.cc)
add_library(parser src/kunjs/parser.cc)
target_link_libraries(parser printer grammar)

add_library(compiler src/kunjs/compiler.cc src/kunjs/compiler/program_compiler.cc)
target_link_libraries(compiler parser ${REQ_LLVM_LIBRARIES})

add_executable(run-parser-tests test/parser_test.cc)
target_link_libraries(run-parser-tests ${GTEST_BOTH_LIBRARIES} parser)

add_executable(run-compiler-tests test/compiler_test.cc)
target_link_libraries(run-compiler-tests ${GTEST_BOTH_LIBRARIES} compiler ${REQ_LLVM_LIBRARIES})

enable_testing()
add_test(parser ${EXECUTABLE_OUTPUT_PATH}/run-parser-tests)

