#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Curry
#include <boost/test/unit_test.hpp>

#include "../inc/core/curry.hpp"

#include <boost/mpl/vector.hpp>
#include <type_traits>

#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/function.hpp>

#include <boost/preprocessor.hpp>

namespace fn = functional::core;

auto fun1 = fn::curry([](int i) { return i; });
auto fun2 = fn::curry([](int i, char c) { return i + c; });
auto fun3 = fn::curry([](int i, char c, double d) { return static_cast<int>(i + c + d); });


BOOST_AUTO_TEST_CASE(curry)
{
	BOOST_CHECK_EQUAL(fun1(5), 5);
	BOOST_CHECK_EQUAL(fun2(5,'d' - 'a'), 8);
	BOOST_CHECK_EQUAL(fun3(5, 'd' - 'a', 2), 10);

	BOOST_CHECK_EQUAL(fun2(5)('d' - 'a'), 8);

	BOOST_CHECK_EQUAL(fun3(5)('d' - 'a')(2), 10);
	BOOST_CHECK_EQUAL(fun3(5)('d' - 'a', 2), 10);
	BOOST_CHECK_EQUAL(fun3(5, 'd' - 'a')(2), 10);
}

BOOST_AUTO_TEST_CASE(curry_return)
{
	auto fun = []() { int i = 5; return fun3(i); };

	BOOST_CHECK_EQUAL(fun()('d' - 'a', 2), 10);
}

BOOST_AUTO_TEST_CASE(auto_curry)
{
	auto fun = fn::curry([](auto i, auto j) { return i + j; });

	BOOST_CHECK_EQUAL(fun(5,5), 10);
	BOOST_CHECK_EQUAL(fun(5)(5), 10);
}

int realfun3(int i, char c, double d) { return static_cast<int>(i + c + d); }

BOOST_AUTO_TEST_CASE(invoke)
{
    using functional::core::detail_::invoke_;

    auto args = std::make_tuple(23, 'e' - 'a', 32.6);

    BOOST_CHECK_EQUAL(invoke_(realfun3, args), 59);
}

#define PARM(z, n, data) auto BOOST_PP_CAT(a,n)
#define ARG(z, n, data) BOOST_PP_CAT(a,n)

#define ARGS(arity) BOOST_PP_ENUM(arity, ARG, ~)
#define PARMS(arity) BOOST_PP_ENUM(arity, PARM, ~)

#define CURRIED_MEMBER_CALLER(name, arity) \
	template < typename ... Args > \
	auto name(Args ... args) \
	{ \
		return fn::curry([this](PARMS(arity)){ return this->name ## _uncurried(ARGS(arity)); })(args...); \
	}

#define CURRIED_MEMBER(name, arity, rval, sig) \
	CURRIED_MEMBER_CALLER(name, arity) \
	rval name ## _uncurried sig

struct blah
{
	CURRIED_MEMBER(fun, 2, int, (int i,char c))
	{
		return i + c;
	}

	CURRIED_MEMBER(fun2, 2, int, (int,char));

	CURRIED_MEMBER_CALLER(fun3, 2);

	template < typename T >
	int fun3_uncurried(int i, T t) { return i + t + 5; }

};

int blah::fun2_uncurried(int i, char c) { return i + c + 1; }


BOOST_AUTO_TEST_CASE(hmm)
{
	blah b;

	BOOST_CHECK_EQUAL(b.fun_uncurried(5,'d' - 'a'), 8);
	BOOST_CHECK_EQUAL(b.fun(5, 'd' - 'a'), 8);
	BOOST_CHECK_EQUAL(b.fun(5)('d' - 'a'), 8);


	BOOST_CHECK_EQUAL(b.fun2(5, 'd' - 'a'), 9);
	BOOST_CHECK_EQUAL(b.fun2(5)('d' - 'a'), 9);

	BOOST_CHECK_EQUAL(b.fun3(5, 'd' - 'a'), 13);
	BOOST_CHECK_EQUAL(b.fun3(5)('d' - 'a'), 13);
}

