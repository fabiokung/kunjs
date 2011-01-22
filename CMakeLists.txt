cmake_minimum_required(VERSION 2.8)
project(kunjs)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED)

include_directories(
    ${Boost_INCLUDES}
    ${GTest_INCLUDES}
    ${PROJECT_SOURCE_DIR}/src)

link_directories(
    ${PROJECT_BINARY_DIR}
    ${GTest_LIBRARIES}
    ${Boost_LIBRARIES})

add_library(grammar src/grammar.cc)
add_library(parser src/parser.cc)

add_executable(run-tests test/parser_test.cc)
target_link_libraries(run-tests gtest_main grammar parser)

enable_testing()
add_test(all ${EXECUTABLE_OUTPUT_PATH}/run-tests)

