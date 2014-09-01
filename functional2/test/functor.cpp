#define BOOST_TEST_MODULE functor
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/functional/functor.hpp"

#include <boost/optional.hpp>

namespace {

struct TestFunctor
{
	template < typename T >
	struct test_functor
	{
		using constructor = TestFunctor;

		test_functor(T t) : value(t) {}

		T value;
	};

	template < typename T >
	static auto construct(T && t)
	{
		return test_functor<typename std::remove_reference<T>::type>(std::forward<T>(t));
	}

	template < typename Callable
	         , typename Functor >
	static auto fmap(Callable && call, Functor && fun)
	{
		return construct(std::forward<Callable>(call)(std::forward<Functor>(fun).value));
	}
};

int test_fun(double d) { return static_cast<int>(d); }

}

BOOST_AUTO_TEST_SUITE(easy_define)

BOOST_AUTO_TEST_CASE(construct)
{
	auto functor = TestFunctor::construct(42);

	BOOST_CHECK_EQUAL(functor.value, 42);
}

BOOST_AUTO_TEST_CASE(mapping)
{
	auto functor = TestFunctor::construct(42.);


	auto mapped = functional::fmap(test_fun, functor);

	BOOST_CHECK_EQUAL(mapped.value, 42);
}

BOOST_AUTO_TEST_SUITE_END()

namespace {

struct Optional
{
	template < typename T >
	struct wrapped_type;

	template < typename T >
	struct wrapped_type<boost::optional<T>> { using type = T; };

	template < typename T >
	static auto construct(T && t)
	{
		using RawT = typename std::remove_reference<T>::type;
		return boost::optional<RawT>(std::forward<T>(t));
	}

	template < typename T >
	static auto construct()
	{
		using RawT = typename std::remove_reference<T>::type;
		return boost::optional<RawT>();
	}

	template < typename T >
	struct map_result { using type = T; };
	template < typename T >
	struct map_result<T&> { using type = std::reference_wrapper<T>; };

	template < typename Function
	         , typename Functor >
	static auto fmap(Function && function, Functor && functor)
	{
		using FunResult = decltype(std::forward<Function>(function)(*std::forward<Functor>(functor)));
		using MapResult = typename map_result<FunResult>::type;

		if (!functor) return construct<MapResult>();
		return construct<MapResult>(std::forward<Function>(function)(*std::forward<Functor>(functor)));
	}
};

int const& test_fun2(double d)
{
	static int i;
	i = static_cast<int>(d);
	return i;
}

}

namespace functional {

template < typename T >
struct constructor<boost::optional<T>>
{
	using type = Optional;
};

}

BOOST_AUTO_TEST_SUITE(existing_types)

BOOST_AUTO_TEST_CASE(construct)
{
	auto functor = Optional::construct(42);

	BOOST_CHECK(functor);
	BOOST_CHECK_EQUAL(*functor, 42);

	functor = Optional::construct<int>();

	BOOST_CHECK(!functor);
}

BOOST_AUTO_TEST_CASE(mapping)
{
	auto functor = Optional::construct(42.);

	auto result = functional::fmap(test_fun, functor);

	BOOST_CHECK(result);
	BOOST_CHECK_EQUAL(*result, 42);

	functor = Optional::construct<double>();
	result = functional::fmap(test_fun, functor);

	BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(reference_returns)
{
	double d = 42.;
	auto optional0 = Optional::construct(d);

	BOOST_CHECK_EQUAL(static_cast<int>(*optional0), static_cast<int>(d));
	d = 666.;
	BOOST_CHECK_NE(static_cast<int>(*optional0), static_cast<int>(d));

	auto optional1 = functional::fmap(test_fun2, optional0);

	BOOST_CHECK_EQUAL(*optional1, 42);
	test_fun2(666.);
	BOOST_CHECK_EQUAL(*optional1, 666);
}


BOOST_AUTO_TEST_SUITE_END()
