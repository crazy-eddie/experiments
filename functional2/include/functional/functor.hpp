#ifndef FUNCTIONAL_FUNCTOR_HPP
#define FUNCTIONAL_FUNCTOR_HPP

#include "constructor.hpp"

namespace functional {

template < typename Function, typename Functor >
auto fmap(Function && function, Functor && functor)
{
	using RawFunctor = typename std::remove_reference<Functor>::type;
	using Constructor = typename constructor<RawFunctor>::type;

	return Constructor::fmap( std::forward<Function>(function)
			                , std::forward<Functor>(functor) );
}

}

#endif
