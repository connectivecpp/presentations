/** @file
 *
 * @brief Example code from the "std::span in C++" presentation.
 *
 * This presentation gives an overview of the @c std::span class template in the C++ 
 * library. It is meant to be for both C++ programmers as well as non C++ programmers
 * to give a short intro to the static compile time capabilities of the C++ language.
 *
 * @c std::span with it's non-type template parameter @c Extent shows a capability
 * not present in many languages - the ability to specify info that is present only
 * at compile time, and does not cost runtime space or time.
 *
 * @author Cliff Green
 *
 * @copyright (c) 2024 by Cliff Green
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <span>
#include <array>
#include <vector>
#include <string>

#include "catch2/catch_test_macros.hpp"

////////////////////
// Slide 15 - 17
////////////////////

template <typename T, std::size_t SZ>
constexpr bool is_dyn_ext (std::span<T, SZ> sp) {
  return (SZ == std::dynamic_extent);
}

constexpr int sum3 (std::span<int, 3> sp) {
    return sp[0] + sp[1] + sp[2];
}

TEST_CASE ("Spans with both dynamic and static extents", "[basic_usage]") {
  std::vector<int> vec_int;
  std::array<float, 10> fl_arr;
  double c_arr[20]; // C style array
  std::string str;

  REQUIRE (is_dyn_ext(std::span<int>(vec_int)));
  REQUIRE (!is_dyn_ext(std::span<float, 10>(fl_arr)));
  REQUIRE (!is_dyn_ext(std::span<double, 20>(c_arr)));
  REQUIRE (is_dyn_ext(std::span<char>(str)));

}

TEST_CASE ("Calling sum3, ensuring 3 contiguous elements", "[sum3]") {
  std::vector<int> vec { 10, 11, 12 };
  REQUIRE (sum3(std::span<int, 3>(vec.begin(), (vec.begin() + 3))) == 33);
  std::array<int, 3> arr { 10, 11, 12 };
  REQUIRE (sum3(arr) == 33);
  int c_arr[3] = { 10, 11, 12 };
  REQUIRE (sum3(c_arr) == 33);
}

////////////////////
// Slide 21
////////////////////

template <int N>
consteval int factorial () {
  if constexpr (N < 2) {
    return 1;
  }
  else {
    return factorial<N - 1>() * N; // recursive call
  }
}

TEST_CASE ("Compile time evaluated factorial function", "[factorial]") {
  REQUIRE (factorial<0>() == 1);
  REQUIRE (factorial<1>() == 1);
  REQUIRE (factorial<2>() == 2);
  REQUIRE (factorial<3>() == 6);
  REQUIRE (factorial<4>() == 24);
  REQUIRE (factorial<5>() == 120);
  REQUIRE (factorial<6>() == 720);
}


