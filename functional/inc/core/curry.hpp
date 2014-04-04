#ifndef FUNCTIONAL_CORE_CURRY_HPP
#define FUNCTIONAL_CORE_CURRY_HPP

#include "util/index_sequence.hpp"
#include "util/meta/is_callable_with.hpp"

namespace functional { namespace core {

namespace detail_ {

template < typename Fun, typename Args, std::size_t ... I >
auto invoke_(Fun const* fun, Args const* args, util::index_sequence<I...>)
{
    return (*fun)(std::get<I>(*args)...);
}

template < typename Fun, typename Args >
auto invoke_(Fun const* fun, Args const* args)
{
    using decayed_args = typename std::decay<Args>::type;
    using seq =
        typename util::make_index_sequence
        <
        	std::tuple_size<decayed_args>::value
        >::type;

    return invoke_(fun, args, seq());
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
		static auto invoke(F const* fun, A const* args)
		{
			return detail_::invoke_(fun,args);
		}
	};

	template < typename F, typename A >
	struct invoker<F,A,false>
	{
		static auto invoke(Fun const* fun, A const* args)
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
	auto operator() (Args ... args) const
	{
		auto call_tuple = std::tuple_cat(pre_args, std::make_tuple(args...));
		return invoker<Fun,decltype(call_tuple)>::invoke(&fun, &call_tuple);
	}

	curried operator() () const { return *this; }
private:
	Fun fun;
	PreArgs pre_args;


};


template < typename Fun >
auto curry(Fun fun)
{
	return curried<Fun>(fun);
}

}}

#endif
