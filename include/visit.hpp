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

#ifndef ROLLBEAR_VISIT_HPP
#define ROLLBEAR_VISIT_HPP

#include <variant>
#include <utility>

namespace rollbear
{
  namespace detail {
    template<std::size_t I, std::size_t ... Is>
    constexpr
    auto
    prepend(std::index_sequence<Is...>) {
      return std::index_sequence<I, Is...>{};
    }

    constexpr
    std::index_sequence<>
    next_seq(std::index_sequence<>, std::index_sequence<>) {
      return {};
    }

    template<
      std::size_t I, std::size_t ... Is,
      std::size_t J, std::size_t ... Js
    >
    constexpr
    auto
    next_seq(std::index_sequence<I, Is...>, std::index_sequence<J, Js...>) {
      if constexpr (I + 1 == J) {
        return prepend<0>(next_seq(std::index_sequence<Is...>{},
                                   std::index_sequence<Js...>{}));
      } else {
        return std::index_sequence<I + 1, Is...>{};
      }
    }

    template<std::size_t ... I>
    static
    constexpr
    std::size_t
    sum(std::index_sequence<I...>) { return (I + ...); }

    template<
      std::size_t ... Is,
      std::size_t ... Ms,
      typename F,
      typename ... Vs
    >
    constexpr
    auto
    visit(
      std::index_sequence<Is...> i,
      std::index_sequence<Ms...> m,
      F &&f,
      Vs &&... vs)
    {
      constexpr auto n = next_seq(i, m);
      if (std::tuple(vs.index()...) == std::tuple(Is...)) {
        return f(std::get<Is>(std::forward<Vs>(vs))...);
      }
      if constexpr (sum(n) > 0) {
        return visit(n, m, std::forward<F>(f), std::forward<Vs>(vs)...);
      } else {
        throw std::bad_variant_access{};
      }
    }

    template<typename>
    static constexpr std::size_t zero = 0;

    template <typename T>
    using remove_cv_ref_t = std::remove_const_t<std::remove_reference_t<T>>;
  }
  template <typename F, typename ... Vs>
  auto visit(F&& f, Vs&& ... vs)
  {
    return detail::visit(
      std::index_sequence<detail::zero<Vs>...>{},
      std::index_sequence<std::variant_size_v<detail::remove_cv_ref_t<Vs>>...>{},
      std::forward<F>(f),
      std::forward<Vs>(vs)...);
  }

}

#endif //ROLLBEAR_VISIT_HPP
