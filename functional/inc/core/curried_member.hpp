#ifndef FUNCTIONAL_CORE_CURRIED_MEMBER_HPP
#define FUNCTIONAL_CORE_CURRIED_MEMBER_HPP

#include "curry.hpp"
#include <boost/preprocessor.hpp>


#define FUNCTIONAL_CORE_CURMEM_DETAIL_ARGN(z, n, data) BOOST_PP_CAT(a, n)
#define FUNCTIONAL_CORE_CURMEM_DETAIL_PARMN(z, n, data) auto BOOST_PP_CAT(a,n)

#define FUNCTIONAL_CORE_CURMEM_DETAIL_ARGS(arity) BOOST_PP_ENUM(arity, FUNCTIONAL_CORE_CURMEM_DETAIL_ARGN, ~)
#define FUNCTIONAL_CORE_CURMEM_DETAIL_PARMS(arity) BOOST_PP_ENUM(arity, FUNCTIONAL_CORE_CURMEM_DETAIL_PARMN, ~)

#define FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(name) \
		template < typename T > \
		struct name ## _caller \
		{ \
			name ## _caller(T t) : obj(t) {} \
			template < typename ... A > \
			decltype(std::declval<T>().name ## _uncurried(std::declval<A>()...)) \
				operator() (A ... a) const \
			{ \
				return obj.name ## _uncurried(a...); \
			} \
			T obj; \
		}; \
	template < typename ... Args > \
	auto name(Args ... args) const \
	{ \
		return functional::core::curry(name ## _caller<typename std::decay<decltype(*this)>::type>(*this))(args...); \
	}

#define FUNCTIONAL_CORE_CURRIED_MEMBER(name, rval, sig)	\
		FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(name)		\
		rval name ## _uncurried sig const

#endif
