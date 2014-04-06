#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Curry
#include <boost/test/unit_test.hpp>

#include "../inc/core/curry.hpp"
#include "../inc/core/fundamentals.hpp"

#include <boost/mpl/vector.hpp>
#include <type_traits>

#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/function.hpp>

#include <boost/preprocessor.hpp>

namespace fn = functional::core;

auto fun1 = fn::curry([](fn::int_ i) { return i; });
auto fun2 = fn::curry([](fn::int_ i, fn::char_ c) { return static_cast<fn::int_>(i + c); });
auto fun3 = fn::curry([](fn::int_ i, fn::char_ c, fn::double_ d) { return static_cast<fn::int_>(i + c + d); });

BOOST_AUTO_TEST_CASE(curry)
{
	BOOST_CHECK_EQUAL(fun1(5).value(), 5);
	BOOST_CHECK_EQUAL(fun2(5,'d' - 'a').value(), 8);
	BOOST_CHECK_EQUAL(fun3(5, 'd' - 'a', 2).value(), 10);

	BOOST_CHECK_EQUAL(fun2(5)('d' - 'a').value(), 8);

	BOOST_CHECK_EQUAL(fun3(5)('d' - 'a')(2).value(), 10);
	BOOST_CHECK_EQUAL(fun3(5)('d' - 'a', 2).value(), 10);
	BOOST_CHECK_EQUAL(fun3(5, 'd' - 'a')(2).value(), 10);

	fn::int_ val = fun1(5);
	BOOST_CHECK_EQUAL(val, 5);
}

BOOST_AUTO_TEST_CASE(curry_return)
{
	auto fun = []() { int i = 5; return fun3(i); };

	BOOST_CHECK_EQUAL(fun()('d' - 'a', 2).value(), 10);
}

BOOST_AUTO_TEST_CASE(auto_curry)
{
	auto fun = fn::curry([](auto i, auto j) { return i + j; });

	BOOST_CHECK_EQUAL(fun(5,5).value(), 10);
	BOOST_CHECK_EQUAL(fun(5)(5).value(), 10);
}
// this is unsolveable so far as I can think of.
//fn::int_ fun(fn::int_ i) { return i; }
//fn::int_ fun(fn::int_ i0, fn::int_ i1) { return i0 + i1; }

struct
{
	fn::int_ operator() (fn::int_ i) const { return i; }
	fn::int_ operator() (fn::int_ i0, fn::int_ i1) const { return i0 + i1; }
} fun;
auto overloaded = fn::curry(fun);

BOOST_AUTO_TEST_CASE(overloaded_curry)
{
	auto x = overloaded(5);

	BOOST_CHECK_EQUAL(x.value(), 5);
	BOOST_CHECK_EQUAL(x(4).value(), 9);
}

#if 0

/*
 * Passing by value for everything could potentially ruin performance
 * for objects implemented with a shared instance impl.  Such shared
 * instances require shared reference counting, which will need to operate
 * on an atomic ref count.  If we're making copies even when unnecessary
 * this will pound the hell out of the cache.
 */
namespace {

int count = 0;

struct something
{
	something(int i_) : i(i_)
	{
		BOOST_MESSAGE("creating something");
		++count;
	}
	~something()
	{
		BOOST_MESSAGE("destroying something");
		--count;
	}
	something(something const& s) : i(s.i)
	{
		BOOST_MESSAGE("copyng something");
		++count;
	}

	int i;
};

auto fun4 = fn::curry([](something const& s0, something const& s1)
		              {
			 		 	 return fn::int_(s0.i + s1.i);
		              });

auto fun_return = [](){ return fun4(5); };
}

BOOST_AUTO_TEST_CASE(references)
{
	auto x = fun4(something(5),something(6));

	BOOST_CHECK_EQUAL(count, 0);

	auto y = fun4(something(3));

	BOOST_CHECK_EQUAL(count, 1);
}
#endif
