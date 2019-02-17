/*
 * Copyright Björn Fahller 2018
 *
 *  Use, modification and distribution is subject to the
 *  Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 *
 * Project home: https://github.com/rollbear/visit
 */

#include <catch2/catch.hpp>
#include <variant>

#include <visit.hpp>

template <typename ... F>
struct overload : F...
{
  using F::operator()...;
};
template <typename ...F>
overload(F...) -> overload<F...>;

struct evil
{
  operator int() const { throw 1;}
};

SCENARIO("single variant visit")
{
  GIVEN("a variant and a visitor")
  {
    int int_visits = 0;
    int string_visits = 0;
    int ptr_visits = 0;
    overload visitor{
      [&](int v)                -> int { ++int_visits; return v;},
      [&](const std::string& s) -> int { ++string_visits; return s.length();},
      [&](void*)                -> int { ++ptr_visits; return 0;}
    };
    std::variant<int, std::string, void*> v{3};
    WHEN("visited with an int")
    {
      auto r = rollbear::visit(visitor, v);
      THEN("the return value is that of the int")
      {
        REQUIRE(r == 3);
      }
      AND_THEN("only the int was visited")
      {
        REQUIRE(int_visits == 1);
        REQUIRE(string_visits == 0);
        REQUIRE(ptr_visits == 0);
      }
    }
    AND_WHEN("visited with a string")
    {
      v = std::string("foobar");
      auto r = rollbear::visit(visitor, v);
      THEN("return value is the length of the string")
      {
        REQUIRE(r == 6);
      }
      AND_THEN("only the string was visited")
      {
        REQUIRE(int_visits == 0);
        REQUIRE(string_visits == 1);
        REQUIRE(ptr_visits == 0);
      }
    }
    AND_WHEN("visited when empty with exception")
    {
      try {
        v.emplace<int>(evil{});
      }
      catch (...)
      {
      }
      THEN("visit throws bad_variant_access")
      {
        REQUIRE_THROWS_AS(rollbear::visit(visitor, v), std::bad_variant_access);
      }
    }
  }
}

SCENARIO("multi variant visit")
{
  GIVEN("two variants and a visitor")
  {
    using V =  std::variant<int, std::string>;

    using std::to_string;
    overload visitor{
      [](int i, int j)                  { return to_string(i) + to_string(j);},
      [](int i, std::string s)          { return to_string(i) + s;},
      [](std::string s, int i)          { return s + to_string(i);},
      [](std::string s1,std::string s2) { return s1+s2;}
    };
    WHEN("visited with values")
    {
      V v1{3};
      V v2{std::string("foo")};
      auto r1 = rollbear::visit(visitor, v1,v2);
      auto r2 = rollbear::visit(visitor, v2, v1);
      v1 = std::string("bar");
      auto r3 = rollbear::visit(visitor, v1,v2);
      v1 = 3;
      v2 = 4;
      auto r4 = rollbear::visit(visitor, v1,v2);
      THEN("the return value comes from the visitor of the value pair")
      {
        REQUIRE(r1 == "3foo");
        REQUIRE(r2 == "foo3");
        REQUIRE(r3 == "barfoo");
        REQUIRE(r4 == "34");
      }
    }
    AND_WHEN("visited when empty with exception")
    {
      V v1{3};
      V v2{4};
      try {
        v1.emplace<int>(evil{});
      }
      catch (...)
      {
      }
      THEN("visit throws bad_variant_access")
      {
        REQUIRE_THROWS_AS(rollbear::visit(visitor, v1,v2), std::bad_variant_access);
        REQUIRE_THROWS_AS(rollbear::visit(visitor, v2,v1), std::bad_variant_access);
      }
    }
  }
}
