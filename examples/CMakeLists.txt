# Copyright (c) 2024 by Cliff Green
#
# This CMake file invokes all of the example code CMake files for
# the presentations repository. Each subdirectory can be built
# independently, as well. Each subdirectory builds a unit test
# (or multiple unit tests) using the Catch2 unit testing library.
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# create project
project(presentation_examples)

add_subdirectory(intro_generic_programming)


