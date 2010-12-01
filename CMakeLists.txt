project(kungjs)
cmake_minimum_required(VERSION 2.8)

set(GTEST_DIR utils/tests/gtest-1.5.0)

add_subdirectory(${GTEST_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/${GTEST_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/src)

cxx_executable(kungjs_test test gtest_main)
