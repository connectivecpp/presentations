
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# create project
project(intro_generic_programming)

# add executable
add_executable(intro_generic_programming_test intro_generic_programming_test.cpp)

# add dependencies
include(../../cmake/download_cpm.cmake)

CPMAddPackage("gh:catchorg/Catch2@3.5.4")

# link dependencies
target_link_libraries(intro_generic_programming_test Catch2::Catch2WithMain)

enable_testing()

add_test(NAME run_intro_generic_programming_test COMMAND intro_generic_programming_test)
set_tests_properties(run_intro_generic_programming_test 
  PROPERTIES PASS_REGULAR_EXPRESSION "All tests passed"
  )

