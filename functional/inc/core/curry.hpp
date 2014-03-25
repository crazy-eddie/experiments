#ifndef FUNCTIONAL_CORE_CURRY_HPP
#define FUNCTIONAL_CORE_CURRY_HPP

#include "util/index_sequence.hpp"

namespace functional { namespace core {

namespace detail_ {

template < typename Fun, typename Args, std::size_t ... I >
auto invoke(Fun && fun, Args && args, util::index_sequence<I...>)
{
    return std::forward<Fun>(fun)(std::get<I>(std::forward<Args>(args))...);
}

template < typename Fun, typename Args >
auto invoke(Fun && fun, Args && args)
{
    using decayed_args = typename std::decay<Args>::type;
    using seq =
        typename util::make_index_sequence
            <std::tuple_size<decayed_args>::value>::type;

    return invoke( std::forward<Fun>(fun)
                 , std::forward<Args>(args)
                 , seq() );
}

}

}}

#endif
