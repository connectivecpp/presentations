# Example Code for Presentations

This directory contains example source code from the presentations in the [presentations](<../presentations>) directory.

All of the example source code has [Catch2](<https://github.com/catchorg/Catch2>) unit test code. The Catch2 unit test presentation (of course) features the unit test code. Each directory corresponding to a presentation has a top level CMake file.

Each CMake file will download and build the Catch2 library as appropriate using the [CPM.cmake](<https://github.com/cpm-cmake/CPM.cmake>) dependency manager. If Catch2 (v3 or greater) is already installed using a different package manager (such as Conan or vcpkg), the `CPM_USE_LOCAL_PACKAGES` variable can be set which results in `find_package` being attempted. Note that v3 (or later) of Catch2 is required, which results in faster unit test build times (once the initial Catch2 library is compiled).


