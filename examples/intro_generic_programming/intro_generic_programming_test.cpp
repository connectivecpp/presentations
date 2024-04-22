// Copyright (c) 2024 by Cliff Green
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)


#include <algorithm>
#include <vector>
#include <list>
#include <functional> // std::ref
#include <complex>
#include <string>

#include "catch2/catch_test_macros.hpp"

////////////////////
// Slide 7
////////////////////

struct bidirectional_iterator_tag { }; 
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <typename RAIter>
void sort_alg (RAIter begin, RAIter end, random_access_iterator_tag) {
  std::sort(begin, end);
  REQUIRE (std::is_sorted(begin, end));
}

template <typename Iter>
void sort_alg (Iter begin, Iter end, bidirectional_iterator_tag) {
// a real bidirectional sort would be implemented here, this code
// is only for unit test coverage; see presentation for more info
  std::list lst(begin, end);
  lst.sort();
  REQUIRE (std::is_sorted(lst.begin(), lst.end()));
}


TEST_CASE ("Types as function overload tags", "[overload_tags]") {
  std::vector v { 50, 10, 1, 60, };
  sort_alg(v.begin(), v.end(), random_access_iterator_tag{});
  std::list ls { 50, 10, 1, 60, };
  sort_alg(ls.begin(), ls.end(), bidirectional_iterator_tag{});
}

////////////////////
// Slide 13 
////////////////////

struct cnt_cmp {
  int cnt {0};
  bool operator() (int a, int b) { ++cnt; return a < b; }
};

bool traditional_comp_func(int a, int b) {
  return a < b; 
}

// Note that Catch2 SECTIONs execute the code before the SECTION each time
TEST_CASE ("Function call operator overloading", "[function_call_overload]") {

  cnt_cmp obj;
  std::vector<int> v { 10, 3, 6, 0, -2 };

  SECTION ("Function call operator") {
    REQUIRE(obj(5, 10)); 
    REQUIRE(!obj(1000, 500));
    REQUIRE(!obj(0, 0));
    REQUIRE (obj.cnt == 3);
  }

  SECTION ("Sorting with default constructed compare obj") {
    std::sort(v.begin(), v.end(), cnt_cmp{});
    REQUIRE (std::is_sorted(v.begin(), v.end()));
    INFO("Number of compares: " << obj.cnt);
  }

  SECTION ("Sorting with compare obj passed by value") {
    std::sort(v.begin(), v.end(), obj);
    REQUIRE (std::is_sorted(v.begin(), v.end()));
    INFO("Number of compares (oops, will be 0 because of pass by value): " << obj.cnt);
    REQUIRE (obj.cnt == 0);
  }

  SECTION ("Sorting with compare obj passed by reference") {
    std::sort(v.begin(), v.end(), std::ref(obj));
    REQUIRE (std::is_sorted(v.begin(), v.end()));
    INFO("Number of compares (now is non 0 because of pass by ref): " << obj.cnt);
    REQUIRE (!(obj.cnt == 0));
  }

  SECTION ("Sorting with traditional compare function") {
    std::sort(v.begin(), v.end(), traditional_comp_func);
    REQUIRE (std::is_sorted(v.begin(), v.end()));
  }
}


////////////////////
// Slide 16 
////////////////////

namespace slide_16 { // namespace to distinguish functions with same name
int add_div_by_3 (int a, int b) {
  return (a + b) / 3;
}

constexpr float add_div_by_3 (float a, float b) {
  return (a + b) / 3;
}
} // end namespace

TEST_CASE ("Simple arithmetic function", "[simple_arithmetic_function]") {
  using namespace slide_16;

  SECTION ("Call integer function") {
    int tmp {20}; 
    int result = add_div_by_3 (tmp, 30);
    INFO("Result with integers: " << result);
    REQUIRE (result == 16);
  }

  SECTION ("Call float function") {
    float tmp {20.0f}; 
    auto result1 = add_div_by_3 (tmp, 30.0);
    // auto result2 = add_div_by_3 (20.0, 30.0); // error - ambiguous because of double -> float conversion
    auto result2 = add_div_by_3 (20.0f, 30.0f);
    INFO("Result1 with floats: " << result1);
    REQUIRE (result1 > 16.0f);
    INFO("Result2 with floats: " << result2);
    REQUIRE (result2 > 16.0f);
  }

}


////////////////////
// Slide 17 and 18
////////////////////

namespace slide_17_18 {

template <typename T>
constexpr T pre_20_add_div_by_3 (T a, T b) {
  return (a + b) / 3;
}

// in C++ 20, template syntax can be simplified (more on next slide)
constexpr auto add_div_by_3 (auto a, auto b) {
  return (a + b) / 3;
}

constexpr auto add_sub_div (auto a, auto b) {
  return (a + b) / (a - b);
}

} // end namespace

TEST_CASE ("Function template", "[function_template]") {
  using namespace slide_17_18;

  auto res1 = add_div_by_3 (20, 30); 
  REQUIRE (res1 == 16);
  auto res2 = add_div_by_3(20u, 30u);
  REQUIRE (res2 == 16u);
  auto res3 = add_div_by_3 (20.0, 30.0); 
  REQUIRE (res3 > 16.0);
  auto res4 = add_div_by_3(20.0f, 30.0f);
  REQUIRE (res4 > 16.0f);

  auto res5 = add_sub_div (15, 44);
  REQUIRE (res5 == -2);
  auto res6 = add_sub_div(3.3f, 22.1f);
  REQUIRE (res6 < -1.0f);
  auto res7 = add_sub_div(std::complex<double>{5.0, 2.0}, std::complex<double>{3.0, 4.0});

}

////////////////////
// Slide 21
////////////////////

template <typename T, int SZ>
constexpr T* gen_array () {
  return new T[SZ];
}

TEST_CASE ("Non type template parm intro", "[non_type_template_parm_intro]") {
  auto* my_arr1 = gen_array<double, 20>();
  REQUIRE (my_arr1 != nullptr);
  delete [] my_arr1;
  auto* my_arr2 = gen_array<std::string, 66>();
  REQUIRE (my_arr2 != nullptr);
  delete [] my_arr2;
  auto* my_arr3 = gen_array<double, 44>();
  REQUIRE (my_arr3 != nullptr);
  delete [] my_arr3;
}



