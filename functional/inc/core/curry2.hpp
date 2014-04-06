#ifndef FUNCTIONAL_CORE_CURRY2_HPP
#define FUNCTIONAL_CORE_CURRY2_HPP

#include "util/index_sequence.hpp"
#include "util/meta/is_callable_with.hpp"

namespace functional { namespace core {

template < typename ... T >
struct type_pack {};

template < typename Fun
         , typename PreArgs
         , typename Args >
struct can_invoke2;

template < typename Fun
         , typename ... PreArgs
         , typename ... Args >
struct can_invoke2<Fun, std::tuple<PreArgs...>, type_pack<Args...>>
	: util::meta::is_callable_with
	  <
	  	  Fun, PreArgs..., Args...
	  >
{};

template < typename Fun
         , typename PreArgs
         , typename Args
         , bool can_call = can_invoke2<Fun, PreArgs, Args>::value >
struct invoker;

template < typename Fun, typename PreArgs = std::tuple<> >
struct curried2
{
	curried2(Fun f) : fun(std::move(f)) {}
	curried2(Fun f, PreArgs a) : fun(std::move(f)), pre_args(std::move(a)) {}

	template < typename ... Args >
	auto operator() (Args const& ... args) const
	{
		return invoker<Fun,PreArgs,type_pack<Args...>>::call(fun, pre_args, args...);
	}

private:
	Fun fun;
	PreArgs pre_args;
};


template < typename Fun, typename PreArgs, typename ... Args >
struct invoker<Fun,PreArgs,type_pack<Args...>, true>
{

	static auto call( Fun const& fun
			        , PreArgs const& pre
			        , Args const&... args)
	{
		using seq = util::make_index_sequence<std::tuple_size<PreArgs>::value>;

		return call(fun, pre, args..., seq());
	}

private:
	template < std::size_t ... I >
	static auto call( Fun const& fun
			        , PreArgs const& pre
			        , Args const& ... args
			        , util::index_sequence<I...> )
	{
		return fun(std::get<I>(pre)..., args...);
	}
};


template < typename Fun, typename PreArgs, typename ... Args >
struct invoker<Fun,PreArgs,type_pack<Args...>, false>
{
	static auto call(Fun const& fun, PreArgs const& pre, Args const&... args)
	{
		std::tuple<Args...> args_tuple(args...);
		auto full_tuple = std::tuple_cat(pre, args_tuple);

		return curried2<Fun, decltype(full_tuple)>(fun,full_tuple);
	}
};

template < typename Fun >
auto curry2(Fun f)
{
	return curried2<Fun>(f);
}

}}

#endif
