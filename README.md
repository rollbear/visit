# visit
A fast, generalized, and not quite std conforming, version of
[`visit`](https://en.cppreference.com/w/cpp/utility/variant/visit) for
[`variant`](https://en.cppreference.com/w/cpp/utility/variant)

This is the implementation for the `visit` written about in the blog post
 [When performance guarantees hurts performande - `std::visit`](https://playfulprogramming.blogspot.com/2018/12/when-performance-guarantees-hurts.html)

The reason it is fast is because it does away with the [requirement of O(1)
lookup](http://eel.is/c++draft/variant.visit#5), essentially requiring calls through function pointers, and instead
allows perfect inlining through a series of if/else, which the compiler can
optimise better.


It is generalized in the way that it supports parameters that are not variants,
and they are just forwarded to the visitor, as-is. All variant parameters are
forwarded as their current member (if any). It also handles types inheriting
from `std::variant<>` as variants.

Example:
```Cpp
using V = std::variant<int, std::string>;
struct S : V
{
  using V::V;
};
using P = std::unique_ptr<int>;
using std::to_string;
using std::string;
struct visitor {
  auto operator()(int i1, P p, int i2) const       { return to_string(i1) + to_string(*p) + to_string(i2); }
  auto operator()(string s, P p, int i2) const     { return s + to_string(*p) + to_string(i2); }
  auto operator()(int i1, P p, string s) const     { return to_string(i1) + to_string(*p) + s; }
  auto operator()(string s1, P p, string s2) const { return s1 + to_string(*p) + s2; }
};

auto r = rollbear::visit(visitor{}, V{1}, std::make_unique<int>(3), S{string{"foo"}});
assert(r == "13foo");
```

See the test sources for more details.
