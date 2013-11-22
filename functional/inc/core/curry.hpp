#ifndef FUNCTIONAL_CORE_CURRY_HPP
#define FUNCTIONAL_CORE_CURRY_HPP

#include <functional>
#include <utility>

#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/include/list.hpp>
#include <boost/function_types/function_arity.hpp>

#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>

namespace functional { namespace core {

/*
 * What we want here isn't complete "currying".  We want
 * to create an object that can be invoked by the first argument
 * returning another curried object taking the rest.  This will let
 * us create function bindings for functions taking more than one argument
 * and then calling the end result with more than one, all but the first.
 *
 * Example:
 *
 *    int fun(int,int,int);
 *
 *    auto f1 = curry(fun)(5,3,2);
 *    auto f2 = curry(fun)(5)(3,2);
 *    auto f3 = curry(fun)(5)(3)(2);
 */
template < typename Fun
         , typename Args
         , typename R
         , typename ... Rest >
struct curried_function;

template < typename Fun
         , typename Args
         , typename R
         , typename T >
struct curried_function<Fun,Args,R,T>
{
    typedef R result_type;

    curried_function(Fun f, Args a)
        : fun(f)
        , args(a)
    {}

    result_type operator () (T && arg) const
    {
        auto args_ = boost::fusion::push_back(args, std::forward<T>(arg));
        return boost::fusion::invoke(fun, args_);
    }



private:
    Fun fun;
    Args args;
};

template < typename Fun
         , typename Args
         , typename R
         , typename T
         , typename ... Rest >
struct curried_function<Fun,Args,R,T,Rest...>
{
    typedef R result_type;

    curried_function(Fun f, Args a)
        : fun(f)
        , args(a)
    {}

    auto operator () (T && arg) const
        -> curried_function< Fun
                           , decltype(boost::fusion::push_back( std::declval<Args>()
                                                              , std::forward<T>(arg) ))
                           , R
                           , Rest...>
    {
        auto args_ = boost::fusion::push_back(args, std::forward<T>(arg));
        return curried_function<Fun, decltype(args_), R, Rest...>(fun,args_);
    }

    // need an unpack.

private:
    Fun fun;
    Args args;

};

template < typename ... Pack > struct packed {};

template < typename Sequence >
struct sequence_to_pack
{
    template < typename Begin
             , typename End
             , typename ... Pack >
    struct pack_
        : pack_
          < typename boost::mpl::next<Begin>::type
          , End
          , Pack...
          , typename boost::mpl::deref<Begin>::type >
    {};

    template < typename End
             , typename ... Pack >
    struct pack_<End, End, Pack... >
    {
        typedef packed<Pack...> type;
    };

    typedef typename pack_< typename boost::mpl::begin<Sequence>::type
                          , typename boost::mpl::end<Sequence>::type >::type type;
};

template < typename Fun >
struct curried_initiator
{
    template < typename F, typename R, typename Pack >
    struct initiator_;

    template < typename F, typename R, typename ... Pack >
    struct initiator_<F,R,packed<Pack...>>
    {
        typedef curried_function< F
                                , boost::fusion::list<>
                                , R
                                , Pack... > type;
    };

    typedef typename sequence_to_pack
            <
              typename boost::function_types::parameter_types<Fun>::type
            >::type parameter_pack;

    typedef typename boost::function_types::result_type<Fun>::type result_type;

    typedef typename initiator_<Fun, result_type, parameter_pack>::type type;
};

template < typename Fun >
typename curried_initiator<Fun>::type curry(Fun fun)
{
    typedef typename curried_initiator<Fun>::type curried_type;
    return curried_type(fun, boost::fusion::list<>());
}

}}

#endif
