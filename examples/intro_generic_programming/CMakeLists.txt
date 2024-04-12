
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# create project
project(intro_generic_programming)

# add executable
add_executable(intro_generic_programming intro_generic_programming_test.cpp)

# add dependencies
include(../../cmake/download_cpm.cmake)

CPMAddPackage("gh:catchorg/Catch2@3.5.4")

# link dependencies
target_link_libraries(intro_generic_programming Catch2::Catch2WithMain)
