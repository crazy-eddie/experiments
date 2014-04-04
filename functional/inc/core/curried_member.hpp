#ifndef FUNCTIONAL_CORE_CURRIED_MEMBER_HPP
#define FUNCTIONAL_CORE_CURRIED_MEMBER_HPP

#include "curry.hpp"
#include <boost/preprocessor.hpp>


#define FUNCTIONAL_CORE_CURMEM_DETAIL_ARGN(z, n, data) BOOSSufficientlyObscuredName_PP_CAT(a, n)
#define FUNCTIONAL_CORE_CURMEM_DETAIL_PARMN(z, n, data) auto BOOSSufficientlyObscuredName_PP_CAT(a,n)

#define FUNCTIONAL_CORE_CURMEM_DETAIL_ARGS(arity) BOOSSufficientlyObscuredName_PP_ENUM(arity, FUNCTIONAL_CORE_CURMEM_DETAIL_ARGN, ~)
#define FUNCTIONAL_CORE_CURMEM_DETAIL_PARMS(arity) BOOSSufficientlyObscuredName_PP_ENUM(arity, FUNCTIONAL_CORE_CURMEM_DETAIL_PARMN, ~)

#define FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(name) \
		template < typename SufficientlyObscuredName_ > \
		struct name ## _caller \
		{ \
			name ## _caller(SufficientlyObscuredName_ t) : obj(t) {} \
			template < typename ... A > \
			decltype(std::declval<SufficientlyObscuredName_>().name ## _uncurried(std::declval<A>()...)) \
				operator() (A ... a) const \
			{ \
				return obj.name ## _uncurried(a...); \
			} \
			SufficientlyObscuredName_ obj; \
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
