# Example Code for Presentations

![GH Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/connectivecpp/presentations/build_run_unit_test_cmake.yml?branch=main&label=GH%20Actions%20build,%20unit%20tests%20on%20main)

![GH Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/connectivecpp/presentations/build_run_unit_test_cmake.yml?branch=develop&label=GH%20Actions%20build,%20unit%20tests%20on%20develop)

This directory contains example source code from the presentations in the [presentations](<../presentations>) directory.

All of the example source code has [Catch2](<https://github.com/catchorg/Catch2>) unit test code. The Catch2 unit test presentation (of course) features the unit test code. Each directory corresponding to a presentation has a top level CMake file.

Each CMake file will download and build the Catch2 library as appropriate using the [CPM.cmake](<https://github.com/cpm-cmake/CPM.cmake>) dependency manager. If Catch2 (v3 or greater) is already installed using a different package manager (such as Conan or vcpkg), the `CPM_USE_LOCAL_PACKAGES` variable can be set which results in `find_package` being attempted. Note that v3 (or later) of Catch2 is required, which results in faster unit test build times (once the initial Catch2 library is compiled) due to Catch2 changing from a "header only" library to a library with compiled objects as well as headers.

All of the example code builds and successfully runs (as unit tests) on Ubuntu (Linux), Windows, and macOS, using GitHub Actions (continuous integration). Specifically, `ubuntu-latest`, `windows-latest`, and `macos-latest` are the target runners specified in the build matrix in the workflow YAML file.

All of the CMake files have C++ 20 set as the required C++ standard, but only small portions require C++ 20, so most of the code can be used with older C++ standards.

