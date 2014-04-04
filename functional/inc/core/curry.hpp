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

template < typename Fun
         , typename PreArgs = std::tuple<>
         , bool can_invoke = detail_::can_invoke<Fun, PreArgs>::value >
struct curried;

template < typename Fun, typename PreArgs >
struct curried_base
{
	curried_base(Fun f) : fun(f) {}
	curried_base(Fun f, PreArgs args)
		: fun(f)
		, pre_args(args)
	{}

	template < typename ... Args >
	auto evaluate (Args ... args) const
	{
		auto call_tuple = std::tuple_cat(pre_args, std::make_tuple(args...));
		return curried<Fun, decltype(call_tuple)>(fun, call_tuple);
	}

protected:
	~curried_base() {}
	Fun fun;
	PreArgs pre_args;
};

template < typename Fun, typename PreArgs >
struct curried<Fun,PreArgs,true> : curried_base<Fun,PreArgs>
{
	curried(Fun f) : curried_base<Fun,PreArgs>(f) {}
	curried(Fun f, PreArgs args)
		: curried_base<Fun,PreArgs>(f,args)
	{}

	using base = curried_base<Fun,PreArgs>;

	template < typename ... Args >
	auto operator() (Args ... args) const
	{
		return this->evaluate(args...);
	}
	curried operator() () const { return *this; }

	using result_type = decltype(detail_::invoke_(std::declval<Fun const*>(), std::declval<PreArgs const*>()));

	result_type value() const
	{
		return detail_::invoke_( &this->fun
				               , &this->pre_args );
	}

	operator result_type() const { return value(); }
};

template < typename Fun, typename PreArgs >
struct curried<Fun,PreArgs,false> : curried_base<Fun,PreArgs>
{
	curried(Fun f) : curried_base<Fun,PreArgs>(f) {}
	curried(Fun f, PreArgs args)
		: curried_base<Fun,PreArgs>(f,args)
	{}

	template < typename ... Args >
	auto operator()(Args ... args) const
	{
		return this->evaluate(args...);
	}
	curried operator() () const { return *this; }

	using result_type = curried<Fun,PreArgs,false>;

	result_type value() const { return *this; }

	operator result_type() const { return value(); }
};


template < typename Fun >
auto curry(Fun fun)
{
	return curried<Fun>(fun);
}

}}

#endif
