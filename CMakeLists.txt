cmake_minimum_required(VERSION 2.8)
project(kunjs)

set(GTEST_DIR utils/tests/gtest-1.5.0)
set(BOOST_DIR utils/boost_1_45_0)

add_subdirectory(${GTEST_DIR})

include_directories(
    ${PROJECT_SOURCE_DIR}/${BOOST_DIR}
    ${PROJECT_SOURCE_DIR}/${GTEST_DIR}/include
    ${PROJECT_SOURCE_DIR}/src)

#add_library(grammar src/grammar.cc)

cxx_executable(run-tests test gtest_main test/parser_test.cc src/parser.cc)

enable_testing()
add_test(all ${EXECUTABLE_OUTPUT_PATH}/run-tests)

