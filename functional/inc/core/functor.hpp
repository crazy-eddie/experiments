#ifndef FUNCTIONAL_CORE_FUNCTOR_HPP
#define FUNCTIONAL_CORE_FUNCTOR_HPP

#include "curry.hpp"

namespace functional { namespace core {

auto fmap = curry([](auto const& f, auto const& a) { return fmap_impl(f, a); });

}}

#endif
