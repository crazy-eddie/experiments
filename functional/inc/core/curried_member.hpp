#ifndef FUNCTIONAL_CORE_CURRIED_MEMBER_HPP
#define FUNCTIONAL_CORE_CURRIED_MEMBER_HPP

#include "curry.hpp"
#include <boost/preprocessor.hpp>

#define FUNCTIONAL_CORE_CURMEM_DETAIL_ARGN(z, n, data) BOOST_PP_CAT(a, n)
#define FUNCTIONAL_CORE_CURMEM_DETAIL_PARMN(z, n, data) auto BOOST_PP_CAT(a,n)

#define FUNCTIONAL_CORE_CURMEM_DETAIL_ARGS(arity) BOOST_PP_ENUM(arity, FUNCTIONAL_CORE_CURMEM_DETAIL_ARGN, ~)
#define FUNCTIONAL_CORE_CURMEM_DETAIL_PARMS(arity) BOOST_PP_ENUM(arity, FUNCTIONAL_CORE_CURMEM_DETAIL_PARMN, ~)

#define FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(name, arity)												\
	template < typename ... Args >																		\
	auto name(Args ... args) const																		\
	{																									\
		using functional::core::curry;																	\
		return curry([this](FUNCTIONAL_CORE_CURMEM_DETAIL_PARMS(arity))									\
					 {																					\
					 	 return this->name ## _uncurried(FUNCTIONAL_CORE_CURMEM_DETAIL_ARGS(arity));	\
					 })(args...);																		\
	}

#define FUNCTIONAL_CORE_CURRIED_MEMBER(name, arity, rval, sig)	\
		FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(name, arity)		\
		rval name ## _uncurried sig const

#endif
