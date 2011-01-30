cmake_minimum_required(VERSION 2.8)
project(kunjs)

find_package(GTest REQUIRED)
find_package(Boost 1.45.0 REQUIRED)

set(LLVM_ROOT "${PROJECT_SOURCE_DIR/utils/llvm}" CACHE PATH "Root of LLVM install.")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/cmake/modules")
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

add_library(grammar src/kunjs/grammar.cc)
add_library(printer src/kunjs/printer.cc)
add_library(parser src/kunjs/parser.cc)
target_link_libraries(parser printer grammar)

add_executable(run-tests test/parser_test.cc)
target_link_libraries(run-tests gtest_main parser)

enable_testing()
add_test(all ${EXECUTABLE_OUTPUT_PATH}/run-tests)

