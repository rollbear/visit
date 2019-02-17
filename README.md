# visit
A fast, generalized, and not quite std conforming, version of visit for variants

This is the implementation for the `visit` written about in the blog post
 [When performance guarantees hurts performande - `std::visit`](https://playfulprogramming.blogspot.com/2018/12/when-performance-guarantees-hurts.html)

It is generalized in the way that it supports parameters that are not variants,
and they are just forwarded to the visitor, as-is. All variant parameters are
forwarded as their current member (if any).