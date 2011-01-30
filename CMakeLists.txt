cmake_minimum_required(VERSION 2.8)
project(kunjs)

find_package(GTest REQUIRED)
find_package(Boost 1.45.0 REQUIRED)

set(LLVM_ROOT "${PROJECT_SOURCE_DIR/build/llvm}" CACHE PATH "Root of LLVM install.")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/share/llvm/cmake")
include(LLVM)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${LLVM_ROOT}/include
    ${Boost_INCLUDES}
    ${GTest_INCLUDES})

link_directories(
    ${PROJECT_BINARY_DIR}
    ${LLVM_ROOT}/lib
    ${Boost_LIBRARIES}
    ${GTest_LIBRARIES})

file(GLOB COMPILER_SOURCES src/compiler/**/*.cc)
llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES jit native)

add_library(grammar src/kunjs/grammar.cc)
add_library(printer src/kunjs/printer.cc)
add_library(parser src/kunjs/parser.cc)
add_library(compiler src/kunjs/compiler.cc ${COMPILER_SOURCES})
target_link_libraries(parser printer grammar)
target_link_libraries(compiler parser ${REQ_LLVM_LIBRARIES})

add_executable(run-parser-tests test/parser_test.cc)
add_executable(run-compiler-tests test/compiler_test.cc)
target_link_libraries(run-parser-tests gtest_main parser)
target_link_libraries(run-compiler-tests gtest_main compiler)

enable_testing()
add_test(parser ${EXECUTABLE_OUTPUT_PATH}/run-parser-tests)

