# Copyright (c) 2024-2025 by Cliff Green
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required ( VERSION 3.14 FATAL_ERROR )

# create project
project ( unit_test_with_catch2 LANGUAGES CXX )

# add executable
add_executable ( unit_test_with_catch2_test unit_test_with_catch2_test.cpp )
target_compile_features ( unit_test_with_catch2_test PRIVATE cxx_std_20 )

# add dependencies
include ( ../../cmake/download_cpm.cmake )

CPMAddPackage ( "gh:catchorg/Catch2@3.8.0" )

# link dependencies
target_link_libraries ( unit_test_with_catch2_test PRIVATE Catch2::Catch2WithMain )

enable_testing()

add_test ( NAME run_unit_test_with_catch2_test COMMAND unit_test_with_catch2_test )
set_tests_properties ( run_unit_test_with_catch2_test 
  PROPERTIES PASS_REGULAR_EXPRESSION "All tests passed"
  )

