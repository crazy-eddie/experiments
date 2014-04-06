#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Curry2
#include <boost/test/unit_test.hpp>

#include "../inc/core/curry2.hpp"
#include "../inc/core/fundamentals.hpp"

// This version doesn't support functions with overloads that
// have different parameter counts.  When called with all arguments
// the function will use the reference semantics of the original
// rather than pass by value.  This is so I can test the affects
// of shared impl reference counting on the cache when everything
// is pass-by-value vs. sometimes pass-by-reference.



namespace fn = functional::core;

auto fun1 = fn::curry2([](fn::int_ i) { return i; });
auto fun2 = fn::curry2([](fn::int_ i, fn::char_ c) { return static_cast<fn::int_>(i + c); });
auto fun3 = fn::curry2([](fn::int_ i, fn::char_ c, fn::double_ d) { return static_cast<fn::int_>(i + c + d); });

BOOST_AUTO_TEST_CASE(curry)
{
	BOOST_CHECK_EQUAL(fun1(5), 5);
	BOOST_CHECK_EQUAL(fun2(5,'d' - 'a'), 8);
	BOOST_CHECK_EQUAL(fun3(5, 'd' - 'a', 2), 10);

	BOOST_CHECK_EQUAL(fun2(5)('d' - 'a'), 8);

	BOOST_CHECK_EQUAL(fun3(5)('d' - 'a')(2), 10);
	BOOST_CHECK_EQUAL(fun3(5)('d' - 'a', 2), 10);
	BOOST_CHECK_EQUAL(fun3(5, 'd' - 'a')(2), 10);

	fn::int_ val = fun1(5);
	BOOST_CHECK_EQUAL(val, 5);
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

auto fun4 = fn::curry2([](something const& s0, something const& s1)
		               {
			 		  	 return fn::int_(s0.i + s1.i);
		               });

auto fun_return = [](){ return fun4(something(5)); };
}

BOOST_AUTO_TEST_CASE(references)
{
	// When called with the correct number of arguments it shouldn't copy
	auto x = fun4(something(5),something(6));

	BOOST_CHECK_EQUAL(count, 0);
	BOOST_CHECK_EQUAL(x, 11);

	// When applying partially a copy should be made to ensure
	// reference retention.
	auto y = fun4(something(3));

	BOOST_CHECK_EQUAL(count, 1);
	BOOST_CHECK_EQUAL(y(2), 5);

	auto z = fun_return();
	BOOST_CHECK_EQUAL(count, 2);
	BOOST_CHECK_EQUAL(z(9), 14);
}
