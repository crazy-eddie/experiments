#ifndef FUNCTIONAL_CORE_CURRY_HPP
#define FUNCTIONAL_CORE_CURRY_HPP

#include "util/index_sequence.hpp"
#include "util/meta/is_callable_with.hpp"

namespace functional { namespace core {

namespace detail_ {

template < typename Fun, typename Args, std::size_t ... I >
auto invoke_(Fun && fun, Args && args, util::index_sequence<I...>)
{
    return std::forward<Fun>(fun)(std::get<I>(std::forward<Args>(args))...);
}

template < typename Fun, typename Args >
auto invoke_(Fun && fun, Args && args)
{
    using decayed_args = typename std::decay<Args>::type;
    using seq =
        typename util::make_index_sequence
        <
        	std::tuple_size<decayed_args>::value
        >::type;

    return invoke_( std::forward<Fun>(fun)
                  , std::forward<Args>(args)
                  , seq() );
}

template < typename Fun, typename ArgTuple >
struct can_invoke;

template < typename Fun, typename ... Args >
struct can_invoke<Fun, std::tuple<Args...> >
	: util::meta::is_callable_with<Fun, Args...>
{};

}

template < typename Fun, typename PreArgs = std::tuple<> >
struct curried
{
	template < typename F, typename A, bool CanInvoke = detail_::can_invoke<F,A>::value >
	struct invoker;

	template < typename F, typename A >
	struct invoker<F,A,true>
	{
		static auto invoke(F * fun, A * args)
		{
			return detail_::invoke_(*fun,*args);
		}
	};

	template < typename F, typename A >
	struct invoker<F,A,false>
	{
		static auto invoke(Fun * fun, A * args)
		{
			return curried<F,A>(*fun,*args);
		}
	};

	curried(Fun f) : fun(f) {}
	curried(Fun f, PreArgs args)
		: fun(f)
		, pre_args(args)
	{}

	template < typename ... Args >
	auto operator() (Args && ... args)
	{
		auto call_tuple = std::tuple_cat(pre_args, std::make_tuple(std::forward<Args>(args)...));
		return invoker<Fun,decltype(call_tuple)>::invoke(&fun, &call_tuple);
	}
private:
	Fun fun;
	PreArgs pre_args;


};


template < typename Fun >
auto curry(Fun && fun)
{
	return curried<Fun>(fun);
}

}}

#endif
