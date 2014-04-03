#ifndef FUNCTIONAL_CORE_UTIL_META_IS_CALLABLE_WITH
#define FUNCTIONAL_CORE_UTIL_META_IS_CALLABLE_WITH

#include <boost/mpl/bool.hpp>

#include "yes_no_types.hpp"

namespace functional { namespace core { namespace util { namespace meta {

namespace detail_ {


template < typename Fun, typename ... Args >
struct callable_with_
{
	template < typename F, typename T = decltype(std::declval<F>()(std::declval<Args>()...)) >
	struct decide {};

	template < typename F >
	static yes_type check(F*, decide<F>* = 0);

	static no_type check(...);

	enum { value = sizeof(check(static_cast<Fun*>(nullptr))) == sizeof(yes_type) };
};

}

template < typename Fun, typename ... Args >
struct is_callable_with
	: boost::mpl::bool_< detail_::callable_with_<Fun, Args...>::value >
{};


}}}}

#endif
