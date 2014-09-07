#define BOOST_TEST_MODULE expected
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/functional/expected.hpp"

BOOST_AUTO_TEST_SUITE(expected)

BOOST_AUTO_TEST_CASE(construct_from_t)
{
	functional::expected<int> result(5);

	BOOST_CHECK_EQUAL(result.get(), 5);
	auto x = static_cast<int>(result);
	BOOST_CHECK_EQUAL(x, 5);

	BOOST_CHECK(result.valid());
}

BOOST_AUTO_TEST_CASE(construct_from_exception)
{
	functional::expected<int> result = functional::expected<int>::from_exception(std::runtime_error("hello"));

	BOOST_CHECK_THROW(result.get(), std::runtime_error);
	try
	{
		auto x = static_cast<int>(result);
		x = 3;
		BOOST_FAIL("Expected exception.");
	}
	catch (std::runtime_error const&) {}
	catch (...)
	{
		BOOST_FAIL("Expected runtime_error");
	}

	BOOST_CHECK(!result.valid());
}

BOOST_AUTO_TEST_CASE(construct_from_unknown_exception)
{
	try
	{
		throw std::runtime_error("hello");
	}
	catch (...)
	{
		auto result = functional::expected<int>::from_exception();


		BOOST_CHECK_THROW(result.get(), std::runtime_error);
		try
		{
			auto x = static_cast<int>(result);
			x = 3;
			BOOST_FAIL("Expected exception.");
		}
		catch (std::runtime_error const&) {}
		catch (...)
		{
			BOOST_FAIL("Expected runtime_error");
		}

		BOOST_CHECK(!result.valid());
	}
}

BOOST_AUTO_TEST_CASE(can_move)
{
	struct test
	{
		test() {}
		test(test const&) = delete;
		test(test &&) {}
	};

	functional::expected<test> ex{test{}};

	BOOST_CHECK_EQUAL(ex.valid(), true);
}

BOOST_AUTO_TEST_CASE(try_wrapper)
{
	auto ex = functional::try_([]()->double{ throw std::runtime_error("hello"); });

	BOOST_CHECK(!ex.valid());
}

BOOST_AUTO_TEST_CASE(easy_initialization)
{
	functional::expected<int> ex = functional::make_error(std::runtime_error("hello"));
	BOOST_CHECK(!ex.valid());
}

BOOST_AUTO_TEST_SUITE(functional_bits)

namespace fn = functional;

int test_fun0(double d) { return static_cast<int>(d); }
fn::expected<int> test_fun1(double d) { return fn::expected<int>(static_cast<int>(d)); }

BOOST_AUTO_TEST_CASE(construction)
{
	auto e0 = fn::Expected::construct(42);
	auto e1 = fn::Expected::error<int>(std::runtime_error("hello"));

	BOOST_CHECK_EQUAL(true, e0.valid());
	BOOST_CHECK_EQUAL(false, e1.valid());

	BOOST_CHECK_EQUAL(42, e0.get());
}

BOOST_AUTO_TEST_CASE(mapping)
{
	auto e0 = fn::Expected::construct(42.);
	auto e1 = fn::Expected::error<double>(std::runtime_error("hello"));

	auto r0 = fn::fmap(test_fun0, e0);
	auto r1 = fn::fmap(test_fun0, e1);

	BOOST_CHECK_EQUAL(true, r0.valid());
	BOOST_CHECK_EQUAL(false, r1.valid());

	BOOST_CHECK_EQUAL(42, r0.get());
}

BOOST_AUTO_TEST_CASE(application)
{
	auto e0 = fn::Expected::construct(42.);
	auto e1 = fn::Expected::error<double>(std::runtime_error("hello"));

	auto fun = fn::Expected::construct(&test_fun1);

	auto r0 = fn::apply(fun, e0);
	auto r1 = fn::apply(fun, e1);

	BOOST_CHECK_EQUAL(true, r0.valid());
	BOOST_CHECK_EQUAL(false, r1.valid());

	BOOST_CHECK_EQUAL(42, r0.get());
}

BOOST_AUTO_TEST_CASE(binding)
{
	auto e0 = fn::Expected::construct(42.);
	auto e1 = fn::Expected::error<double>(std::runtime_error("hello"));

	auto r0 = fn::bind(test_fun1, e0);
	auto r1 = fn::bind(test_fun1, e1);

	BOOST_CHECK_EQUAL(true, r0.valid());
	BOOST_CHECK_EQUAL(false, r1.valid());

	BOOST_CHECK_EQUAL(42, r0.get());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
