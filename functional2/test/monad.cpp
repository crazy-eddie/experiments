#define BOOST_TEST_MODULE monad
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/functional/monad.hpp"

namespace {

struct TestMonad
{
	template < typename T >
	struct test_monad
	{
		using constructor = TestMonad;

		test_monad(T t) : value(t) {}

		T value;
	};

	template < typename T >
	static auto construct(T && t)
	{
		return test_monad<typename std::remove_reference<T>::type>(std::forward<T>(t));
	}

	template < typename Callable
	         , typename Functor >
	static auto fmap(Callable && call, Functor && fun)
	{
		return construct(std::forward<Callable>(call)(std::forward<Functor>(fun).value));
	}

	template < typename WrappedCallable
	         , typename WrappedArgument >
	static auto apply(WrappedCallable && call, WrappedArgument && arg)
	{
		return fmap(call.value, std::forward<WrappedArgument>(arg));
	}

	template < typename Function
	         , typename WrappedArgument >
	static auto bind(Function && call, WrappedArgument && arg)
	{
		return call(arg.value);
	}
};

TestMonad::test_monad<int> test_fun(double d)
{
	return TestMonad::construct(static_cast<int>(d));
}

}

BOOST_AUTO_TEST_SUITE(easy_define)

BOOST_AUTO_TEST_CASE(bind)
{
	auto m0 = TestMonad::construct(42.);
	auto m1 = functional::bind(test_fun, m0);

	BOOST_CHECK_EQUAL(m1.value, 42);
}

BOOST_AUTO_TEST_SUITE_END()
