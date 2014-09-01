#ifndef FUNCTIONAL_MONAD_HPP
#define FUNCTIONAL_MONAD_HPP

#include "applicative.hpp"

namespace functional {

template < typename Function
         , typename WrappedArg >
auto bind(Function && function, WrappedArg && arg)
{
	using RawFunctor = typename std::remove_reference<WrappedArg>::type;
	using Constructor = typename constructor<RawFunctor>::type;

	return Constructor::bind( std::forward<Function>(function)
			                , std::forward<WrappedArg>(arg) );
}


}

#endif
