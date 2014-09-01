#define BOOST_TEST_MODULE applicative
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/functional/applicative.hpp"

namespace {

struct TestApplicative
{
	template < typename T >
	struct test_applicative
	{
		using constructor = TestApplicative;

		test_applicative(T t) : value(t) {}

		T value;
	};

	template < typename T >
	static auto construct(T && t)
	{
		return test_applicative<typename std::remove_reference<T>::type>(std::forward<T>(t));
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
		return fmap(call.value, arg);
	}

};

int test_fun(double d)
{
	return static_cast<int>(d);
}

}

BOOST_AUTO_TEST_SUITE(easy_define)

BOOST_AUTO_TEST_CASE(apply)
{
	auto a0 = TestApplicative::construct(&test_fun);
	auto a1 = TestApplicative::construct(42.);

	auto a2 = functional::apply(a0, a1);

	BOOST_CHECK_EQUAL(a2.value, 42);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(existing_types)

BOOST_AUTO_TEST_SUITE_END()
