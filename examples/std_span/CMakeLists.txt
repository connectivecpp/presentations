# Copyright (c) 2024 by Cliff Green
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required ( VERSION 3.14 FATAL_ERROR )

# create project
project ( std_span LANGUAGES CXX )

# add executable
add_executable ( std_span_test std_span_test.cpp )
target_compile_features ( std_span_test PRIVATE cxx_std_20 )

# add dependencies
include ( ../../cmake/download_cpm.cmake )

CPMAddPackage ( "gh:catchorg/Catch2@3.7.0" )

# link dependencies
target_link_libraries ( std_span_test PRIVATE Catch2::Catch2WithMain )

enable_testing()

add_test ( NAME run_std_span_test COMMAND std_span_test )
set_tests_properties ( run_std_span_test 
  PROPERTIES PASS_REGULAR_EXPRESSION "All tests passed"
  )

