# Copyright (c) 2024 by Cliff Green
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# create project
project(intro_generic_programming)

# add executable
add_executable(intro_generic_programming_test intro_generic_programming_test.cpp)
target_compile_features(intro_generic_programming_test PRIVATE cxx_std_20)

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

