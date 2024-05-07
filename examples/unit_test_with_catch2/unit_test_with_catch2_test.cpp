/** @file
 *
 * Example code from the "Unit Testing in C++ Using the Catch2 Library"
 * presentation.
 *
 * This presentation gives an introduction to unit testing in C++ with the
 * Catch2 library.
 *
 * @author Cliff Green
 *
 * @copyright (c) 2024 by Cliff Green
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <cstdint>
#include <cmath>
#include <cstddef> // std::size_t
#include <string>
#include <string_view>
#include <array>
#include <stdexcept>
#include <algorithm>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_template_test_macros.hpp"

////////////////////
// Slide 6
////////////////////

std::uint32_t factorial( std::uint32_t number ) {
//  return number <= 1 ? number : factorial(number-1) * number; // original code, incorrect logic when 0
  return number <= 1 ? 1 : factorial(number-1) * number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
  REQUIRE( factorial( 1) == 1 );
  REQUIRE( factorial( 2) == 2 );
  REQUIRE( factorial( 3) == 6 );
  REQUIRE( factorial(10) == 3'628'800 );
  REQUIRE( factorial(0) == 1 ); // now works with corrected logic above
}

////////////////////
// Slide 15
////////////////////

auto square(auto t) { return t * t; } // C++ 20 template syntax


TEST_CASE( "Square generic function, ints", "[square-int][square]" ) {
  REQUIRE( square(0) == 0 );
  REQUIRE( square(1) == 1 );
  REQUIRE( square(2) == 4 );
  REQUIRE( square(-1) == 1 );
  REQUIRE( square(-2) == 4);
//  REQUIRE( square(55) == 3); // should fail
  REQUIRE( square(55) == 3025);
}

////////////////////
// Slide 15
////////////////////

TEST_CASE( "Square, double", "[square-double][square]" ) {
  REQUIRE( square(3.0) == 9.0 );
}

TEMPLATE_TEST_CASE( "Square generic", "[square-generic][square]",
                     int, short, float, double, long double ) {
  REQUIRE( square<TestType>(3) == std::pow(3, 2));
}

////////////////////
// Slides 18 - 29
////////////////////

// this is the final, corrected code - see the presentation for
// the initial buggy code

template <std::size_t max_sz>
class f_str {
public:
  f_str() = default;
  f_str(std::string_view);
  void append(std::string_view);
  std::string get_str() const noexcept {
    return std::string(m_chars.data(), m_curr_size);
  }
  std::size_t size() const noexcept { return m_curr_size; }
  std::size_t max_size() const noexcept { return max_sz; }
private:
  std::array<char, max_sz> m_chars;
  std::size_t m_curr_size {0};
};


template <std::size_t max_sz>
f_str<max_sz>::f_str(std::string_view s) : m_chars{}, m_curr_size{0} {
  if (s.length() > max_sz) { throw std::range_error("str too big"); }
  std::copy(s.begin(), s.end(), m_chars.begin());
  m_curr_size = s.length();
}

template <std::size_t max_sz>
void f_str<max_sz>::append (std::string_view s) {
  if ((m_curr_size + s.length()) > max_sz) {
    throw std::range_error("appended len too big");
  }
  std::copy(s.begin(), s.end(), (m_chars.begin() + m_curr_size));
  m_curr_size += s.length();
}

//
// Testing functions
//
template <std::size_t sz>
void default_ctor_test() {
  f_str<sz> f_obj;
  REQUIRE( f_obj.size() == 0 );
  REQUIRE( f_obj.max_size() == sz );
  auto s = f_obj.get_str();
  REQUIRE( s.length() == 0 );
}

template <std::size_t sz>
void str_ctor_test(std::string_view s_parm) {
  f_str<sz> f_obj(s_parm);
  REQUIRE( f_obj.size() == s_parm.length() );
  REQUIRE( f_obj.max_size() == sz );
  auto s = f_obj.get_str();
  REQUIRE( s.length() == s_parm.length() );
}

template <std::size_t sz>
void append_test(std::string_view s_parm, std::string_view app_parm) {
  f_str<sz> f_obj(s_parm);
  REQUIRE( f_obj.size() == s_parm.length() );
  f_obj.append(app_parm);
  auto s = std::string(s_parm) + std::string(app_parm);
  REQUIRE( f_obj.size() == s.length() );
}

template <std::size_t sz>
void throw_test_ctr(std::string_view s_parm) {
  REQUIRE_THROWS (f_str<sz>(s_parm));
}

template <std::size_t sz>
void throw_test_append(std::string_view s_parm, std::string_view app_parm) {
  f_str<sz> f_obj(s_parm);
  REQUIRE_THROWS (f_obj.append(app_parm));
}

TEST_CASE( "f_str default ctor", "[f_str]" ) {
  default_ctor_test<0>();
  default_ctor_test<1>();
  default_ctor_test<10>();
  default_ctor_test<4499>();
}

TEST_CASE( "f_str str ctor", "[f_str]" ) {
  str_ctor_test<10>("Howdy!");
  str_ctor_test<7>("Podnah!"); // note - just enough space
  str_ctor_test<0>("");
  str_ctor_test<1>("");
  str_ctor_test<50>("yup");
}

// Note the corner cases being tested, in particular empty strings
TEST_CASE( "f_str append", "[f_str]" ) {
  append_test<13>("Howdy!", "Podnah!"); // note just enough space
  append_test<40>("I enjoyed", " the Balloon Fiesta!");
  append_test<20>("", "Append to empty");
  append_test<30>("Nothing will be appended", "");
  append_test<0>("","");
}

TEST_CASE( "f_str throw", "[f_str]" ) {
  throw_test_ctr<0>("A");
  throw_test_ctr<5>("Nine char");
  throw_test_append<5>("abcde", "x");
  throw_test_append<0>("", "A");
  throw_test_append<1>("M", "N");
  throw_test_append<2>("a", "ab");
}

