#ifndef FUNCTIONAL_APPLICATIVE_HPP
#define FUNCTIONAL_APPLICATIVE_HPP

#include "functor.hpp"

namespace functional {

template < typename WrappedCallable
         , typename WrappedArgument >
auto apply(WrappedCallable && callable, WrappedArgument && arg)
{
	using RawCallable = typename std::remove_reference<WrappedCallable>::type;
	using Constructor = typename constructor<RawCallable>::type;

	return Constructor::apply( std::forward<WrappedCallable>(callable)
			                 , std::forward<WrappedArgument>(arg) );
}

}

#endif
