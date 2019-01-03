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

#include <visit.hpp>
#include <benchmark/benchmark.h>

template <int>
struct type_;

template <>
struct type_<0> { using type = bool;};
template <>
struct type_<1> { using type = signed char;};
template <>
struct type_<2> { using type = char;};
template <>
struct type_<3> { using type = unsigned char;};
template <>
struct type_<4> { using type = short;};
template <>
struct type_<5> { using type = unsigned short;};
template <>
struct type_<6> { using type = int;};
template <>
struct type_<7> { using type = unsigned;};
template <>
struct type_<8> { using type = long; };
template <>
struct type_<9> { using type = unsigned long;};
template <>
struct type_<10> { using type = long long;};
template <>
struct type_<11> { using type = unsigned long long;};

template <int I>
using type = typename type_<I%12>::type;

template <int I>
struct V
{
  type<I> i;
  operator type<I>() const { return i;}
};

template <std::size_t ... I>
auto populate(std::index_sequence<I...>)
{
  std::vector<std::variant<V<I>...>> v;
  for (unsigned i = 0; i < 5000U/sizeof...(I); ++i)
  {
    (v.push_back(V<I>{(I&1) == 1}),...);
    (v.push_back(V<I>{(I&1) == 0}),...);
  }
  return v;
}

constexpr auto nonzero =[](auto x) -> bool { return x;};


template <size_t I>
void rollbear_visit(benchmark::State& state)
{
  auto values = populate(std::make_index_sequence<I>{});
  for (auto _ : state) {
    int sum = 0;
    for (auto& v : values)
    {
      sum += rollbear::visit(nonzero,v);
    }
    benchmark::DoNotOptimize(sum);
  }
}
template <size_t I>
void std_visit(benchmark::State& state)
{
  auto values = populate(std::make_index_sequence<I>{});
  for (auto _ : state) {
    int sum = 0;
    for (auto& v : values)
    {
      sum += std::visit(nonzero,v);
    }
    benchmark::DoNotOptimize(sum);
  }
}
static void rollbear_visit_1(benchmark::State& state) { rollbear_visit<1>(state);}
static void rollbear_visit_2(benchmark::State& state) { rollbear_visit<2>(state);}
static void rollbear_visit_3(benchmark::State& state) { rollbear_visit<3>(state);}
static void rollbear_visit_5(benchmark::State& state) { rollbear_visit<5>(state);}
static void rollbear_visit_8(benchmark::State& state) { rollbear_visit<8>(state);}
static void rollbear_visit_13(benchmark::State& state) { rollbear_visit<13>(state);}
static void rollbear_visit_21(benchmark::State& state) { rollbear_visit<21>(state);}
static void rollbear_visit_34(benchmark::State& state) { rollbear_visit<34>(state);}
static void rollbear_visit_55(benchmark::State& state) { rollbear_visit<55>(state);}
static void rollbear_visit_89(benchmark::State& state) { rollbear_visit<89>(state);}

static void std_visit_1(benchmark::State& state) { std_visit<1>(state);}
static void std_visit_2(benchmark::State& state) { std_visit<2>(state);}
static void std_visit_3(benchmark::State& state) { std_visit<3>(state);}
static void std_visit_5(benchmark::State& state) { std_visit<5>(state);}
static void std_visit_8(benchmark::State& state) { std_visit<8>(state);}
static void std_visit_13(benchmark::State& state) { std_visit<13>(state);}
static void std_visit_21(benchmark::State& state) { std_visit<21>(state);}
static void std_visit_34(benchmark::State& state) { std_visit<34>(state);}
static void std_visit_55(benchmark::State& state) { std_visit<55>(state);}
static void std_visit_89(benchmark::State& state) { std_visit<89>(state);}

// Register the function as a benchmark
BENCHMARK(rollbear_visit_1);
BENCHMARK(rollbear_visit_2);
BENCHMARK(rollbear_visit_3);
BENCHMARK(rollbear_visit_5);
BENCHMARK(rollbear_visit_8);
BENCHMARK(rollbear_visit_13);
BENCHMARK(rollbear_visit_21);
BENCHMARK(rollbear_visit_34);
BENCHMARK(rollbear_visit_55);
BENCHMARK(rollbear_visit_89);

BENCHMARK(std_visit_1);
BENCHMARK(std_visit_2);
BENCHMARK(std_visit_3);
BENCHMARK(std_visit_5);
BENCHMARK(std_visit_8);
BENCHMARK(std_visit_13);
BENCHMARK(std_visit_21);
BENCHMARK(std_visit_34);
BENCHMARK(std_visit_55);
BENCHMARK(std_visit_89);


