/** @file
 *
 * @brief Example code from the "A Tasty Intro to Generic Programming in C++"
 * presentation.
 *
 * This presentation gives a taste of how types and generic programming are used in C++.
 *
 * Types are fundamental to C++, and while not a "strictly" typed language, C++ strives
 * to be as strongly typed as possible. Types are used in many ways.
 *
 * Templates are fundamental to the generic design capabilities of C++, whether as function
 * templates or class templates.The capabilities of templates go beyond simple substitution
 * of types.
 *
 * @author Cliff Green
 *
 * @copyright (c) 2024 by Cliff Green
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
 *
 */


#include <algorithm>
#include <vector>
#include <list>
#include <array>
#include <functional> // std::ref
#include <complex>
#include <string>
#include <type_traits>
#include <tuple>
#include <optional>

#include "decimal.h" // library providing decimal point functionality

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_range_equals.hpp"

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

////////////////////
// Slide 24
////////////////////

TEST_CASE ("Decimal number type, third party", "[decimal_num_type]") {
  using namespace slide_17_18; // same function template

  auto a1 = decimal::decimal<2>{7.55};
  auto b1 = decimal::decimal<2>{14.44};

  auto res1 = add_div_by_3 (a1, b1);
  REQUIRE (res1 == decimal::decimal<2>{7.33});

  auto a2 = decimal::decimal<3>{5.111};
  auto b2 = decimal::decimal<3>{19.222};

  auto res2 = add_div_by_3 (a2, b2);
  REQUIRE (res2 == decimal::decimal<3>{8.111});
}

////////////////////
// Slide 25
////////////////////

namespace slide_25 {
  template <typename N1, typename N2>
  constexpr N1 add_div_by_3 (N1 a, N2 b) {
    return (a + b) / 3;
  }
}

TEST_CASE ("Two template parameters, first specified as return type", "[two_temp_parms]") {
  using namespace slide_25;
  REQUIRE (add_div_by_3 (20, 30.0) == 16); // result is of type int
  REQUIRE (add_div_by_3<double, float>(20, 31) == 17.0); // result is of type double
							 //
  auto a = decimal::decimal<3>{5.111};
  auto b = decimal::decimal<3>{19.222};
  REQUIRE (add_div_by_3 (a, b) == decimal::decimal<3>{8.111}); // both types are decimal<3>
}

////////////////////
// Slide 26
////////////////////
namespace slide_26 {
  template <typename N1, typename N2>
  constexpr auto add_div_by_3 (N1 a, N2 b) -> decltype((a+b)/3) {
    return (a + b) / 3;
  }
}

TEST_CASE ("Two template parameters, deduced return type from decltype", "[deduced_return_type]") {
  using namespace slide_26;

  REQUIRE (add_div_by_3 (20, 31.0) == 17.0); // result is of type double!
  REQUIRE (add_div_by_3<double, float>(20, 31) == 17.0); // result is also type double!
							 //
  auto a = decimal::decimal<3>{5.111};
  auto b = decimal::decimal<3>{19.222};
  REQUIRE (add_div_by_3 (a, b) == decimal::decimal<3>{8.111}); // both types are decimal<3>, as before
}


////////////////////
// Slide 27
////////////////////
template <typename T>
constexpr std::complex<T> some_complex_math(std::complex<T> a, T b) {
  return a + b;
}
template <typename C, typename T>
requires (std::is_same_v<C, std::complex<T>>)
constexpr C similar_complex_math(C a, T b) {
  return a + b;
}

TEST_CASE ("Function template with requires", "[requires]") {
  std::complex<float> x {3.0f, 4.0f};
  std::complex<float> res {8.0f, 4.0f};
  REQUIRE (some_complex_math(x, 5.0f) == res);
  REQUIRE (similar_complex_math(x, 5.0f) == res);
}

////////////////////
// Slide 28
////////////////////
template <typename T>
concept big_math_capable = std::is_copy_constructible_v<T> &&
                           requires (T x) {
  x + x;
  x / x;
};

void math_func_1 (big_math_capable auto a) {
  using namespace slide_26;
  REQUIRE ( ((a+a) / 2) == a); // unit testing is also requiring equality comp
}

template <big_math_capable T>
    T math_func_2(T a, T b) {
  using namespace slide_26;

  return add_div_by_3 (a, b);
}

TEST_CASE ("Concept, function templates using the concept", "[concept]") {
  auto a = decimal::decimal<3>{5.111};
  auto b = decimal::decimal<3>{19.222};
  math_func_1(a);
  REQUIRE (math_func_2(a, b) == decimal::decimal<3>{8.111});
}


////////////////////
// Slides 31 thru 35
////////////////////

template <typename Ctr, typename F>
void traverse(Ctr& container, F func) {
  for (auto& elem : container) {
    func(elem);
  }
}

void square_val(int& x) {
    x = x*x;
}
void incr_char(char& c) {
    c += 1;;
}

struct add_x {
  int x { 0 };
  void operator() (int& elem) { elem += x; x += 1; }
};

struct cmp_cnt { // count number of comparisons
  int cmp { 0 };
  bool operator() (auto a, auto b) { ++cmp; return a < b; }
};

TEST_CASE ("Traverse function", "[traverse]") {
  std::vector<int> v { 1, 3, 5, 7 };
  std::list<int> lst { 2, 4, 6, 8 };
  std::string str { "Howdy"};

  SECTION ("Simple functions passed in to traverse") {
    traverse(v, square_val);
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals(std::vector<int>{1, 9, 25, 49}));

    traverse(lst, square_val);
    REQUIRE_THAT(lst, Catch::Matchers::RangeEquals(std::vector<int>{4, 16, 36, 64}));

    traverse(str, incr_char);
    REQUIRE(str == std::string("Ipxez"));
  }

  SECTION ("Using add_x function object") {
    traverse(v, add_x{42});
    REQUIRE_THAT(v, Catch::Matchers::RangeEquals(std::vector<int>{43, 46, 49, 52}));
    traverse(lst, add_x{11});
    REQUIRE_THAT(lst, Catch::Matchers::RangeEquals(std::vector<int>{13, 16, 19, 22}));
  }

  SECTION ("Using cmp_cnt function object") {
    std::vector<int> v1 { 3, 5, 1, 7, -4, 55, 44 };
    std::vector<double> v2 { 26.0, -2.0, -1.4, 0.5, 8.0 };

    std::sort(v1.begin(), v1.end(), cmp_cnt{});
    REQUIRE_THAT(v1, Catch::Matchers::RangeEquals(std::vector<int>{-4, 1, 3, 5, 7, 44, 55}));
    cmp_cnt cntr{};
    std::sort(v2.begin(), v2.end(), std::ref(cntr));
    REQUIRE_THAT(v2, Catch::Matchers::RangeEquals(std::vector<double>{-2.0, -1.4, 0.5, 8.0, 26.0}));
    REQUIRE (cntr.cmp > 0);

  }
}


////////////////////
// Slides 36 thru 38
////////////////////

struct person {
  std::string name;
  unsigned int age;
};

bool other_alg(auto f, person a, person b) {
  return f(a, b);
}

TEST_CASE ("Lambda, closure", "[lambda_closure]") {

  std::vector<person> v { { "Cliff", 35u }, { "Lou", 77u }, { "Nathan", 23u } };
  std::sort(v.begin(), v.end(), // let's sort by age
            [] (auto a, auto b) { return a.age < b.age; } );
  REQUIRE (v[0].age == 23u);

  std::sort(v.begin(), v.end(), // now sort by name
            [] (auto a, auto b) { return a.name < b.name; } );
  REQUIRE (v[0].name == std::string("Cliff"));

  int cnt { 0 };
  std::sort(v.begin(), v.end(), // sort by name, but also count comparisons
            [&cnt] (auto a, auto b) { ++cnt; return a.name < b.name; } );
  REQUIRE (cnt > 0);

  auto lam = [] (auto a, auto b) { return a.age < b.age; };
  std::sort(v.begin(), v.end(), lam); // sort by age
  REQUIRE (v[0].age == 23u);

  REQUIRE (other_alg(lam, person {"Bozo", 42}, person {"Checkers", 100}));
  REQUIRE (!other_alg(lam, person {"Paul", 28}, person {"Irulan", 27}));

}

////////////////////
// Slides 40, 41
////////////////////

template <typename T1, typename T2>
struct two_items {
  T1 first;
  T2 second;
};

TEST_CASE ("Pair, typle", "[pair_tuple]") {
  using person = two_items<std::string, unsigned int>;
  person cliff { "Cliff", 36u };
  person lou { "Lou", 66u };
  REQUIRE (cliff.first == std::string("Cliff"));
  REQUIRE (cliff.second == 36u);

  lou.second += 1u; // Lou just aged a year
  REQUIRE (lou.second == 67u);

  std::tuple<int, std::string, double> foo1 { 42, "Howdy!", 44.0 };
  auto foo2 = std::make_tuple(42, std::string{"Howdy!"}, 44.0);

  REQUIRE (std::get<int>(foo1) == std::get<int>(foo2));
  REQUIRE (std::get<std::string>(foo1) == std::get<std::string>(foo2));
  REQUIRE (std::get<double>(foo1) == std::get<double>(foo2));

  REQUIRE (std::get<0>(foo1) ==42);
  REQUIRE (std::get<1>(foo1) == std::string("Howdy!"));
  REQUIRE (std::get<2>(foo1) == 44.0);

}

////////////////////
// Slides 42, 43
////////////////////

template <typename T>
class my_opt {
  T val;
  bool is_present;
public:
  my_opt() : val(), is_present(false) { }
  my_opt(const T& v) : val(v), is_present(true) { }
  T& operator*() { return val; }
  const T& operator*() const { return val; }
  T* operator->() { return &val; }
  const T* operator->() const { return &val; }
  operator bool() const { return is_present; }
  // ...
};

using my_opt_str = my_opt<std::string>;
my_opt_str my_func (bool return_value) {
  return return_value ? my_opt_str("Howdy!") : my_opt_str();
}

using std_opt_str = std::optional<std::string>;
std_opt_str std_opt_func (bool return_value) {
  return return_value ? std_opt_str("Howdy!") : std_opt_str();
}

TEST_CASE ("Optional type", "[optional_type]") {
  auto a = my_func(true);
  REQUIRE (a);
  REQUIRE (*a == std::string("Howdy!"));
  auto b = my_func(false);
  REQUIRE (!b);

  auto c = std_opt_func(true);
  REQUIRE (c);
  REQUIRE (*c == std::string("Howdy!"));
  auto d = std_opt_func(false);
  REQUIRE (!d);
}

////////////////////
// Slides 44, 45
////////////////////

// double as non-type template parm not yet supported on Apple Clang
/*
template <double Percentage>
constexpr double my_calc (double val) {
    return val * Percentage;
}
*/

TEST_CASE ("Non-type template parm", "[non-type-template-parm]") {
  std::array<int, 4> my_array { 46, 20, 44, 77 };
  std::sort(my_array.begin(), my_array.end());
  REQUIRE_THAT(my_array, Catch::Matchers::RangeEquals(std::vector<int>{ 20, 44, 46, 77 }));

  for (auto& i : my_array) {
    ++i;
  }
  REQUIRE_THAT(my_array, Catch::Matchers::RangeEquals(std::vector<int>{ 21, 45, 47, 78 }));
  my_array[2] = 10; // access 3rd element of array, same syntax as built-in arrays
  REQUIRE_THAT(my_array, Catch::Matchers::RangeEquals(std::vector<int>{ 21, 45, 10, 78 }));

  // note yet supported by Apple Clang
  /*
  REQUIRE (my_calc<0.10> (100.0) == 10.0);
  double x { 300.0 };
  REQUIRE (my_calc<0.50> (x) == 150.0);
  REQUIRE (my_calc<0.90> (100.0) == 90.0);
  */

}
