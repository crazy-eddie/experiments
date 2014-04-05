#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Functional composition
#include <boost/test/unit_test.hpp>

#include "../inc/core/curry.hpp"
#include "../inc/core/fundamentals.hpp"

namespace fn = functional::core;

BOOST_AUTO_TEST_SUITE(same_result_type)
namespace {

auto f = fn::curry([](fn::int_ i) { return i + fn::int_(5); });
auto g = fn::curry([](fn::int_ i) { return i * fn::int_(10); });

}

BOOST_AUTO_TEST_CASE(simple_compose)
{
	auto fg = f * g;
	auto gf = g * f;

	BOOST_CHECK_EQUAL(fg(3).value(), 35);
	BOOST_CHECK_EQUAL(gf(3).value(), 80);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(different_types)
namespace {

auto f = fn::curry([](fn::int_ i) { return i + 5; });
auto g = fn::curry([](fn::double_ d) { return static_cast<fn::int_>(d * fn::int_(10)); });

}

BOOST_AUTO_TEST_CASE(simple_compose)
{
	auto fg = f * g;

	BOOST_CHECK_EQUAL(fg(33.7), 342);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(convertable_types)

namespace {

struct int_convertable
{
	int_convertable(int i) : mem(i) {}

	operator fn::int_ () const { return fn::int_(mem); }

	int_convertable value() const { return *this; }
	int_convertable operator() () const { return *this; }

private:
	int mem;
};

auto f = fn::curry([](fn::int_ i) { return i + 5; });
auto g = fn::curry([](int i) { return int_convertable(i * 7); });

}

BOOST_AUTO_TEST_CASE(compose_convertable)
{
	auto fg = f * g;

	BOOST_CHECK_EQUAL(fg(3).value(), 26);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(curried_composition)

BOOST_AUTO_TEST_CASE(simple_curry)
{
	// Not sure this should be possible.  It is, but by at least Haskell standard
	// it shouldn't work.
	auto f = fn::curry([](fn::int_ i) { return i + 5; });
	auto g = fn::curry([](fn::int_ i, fn::int_ j) { return i + j; });

	auto fg = f * g;

	BOOST_CHECK_EQUAL(fg(3,6).value(), 14);

	BOOST_CHECK_EQUAL(fg(3)(6).value(), 14);
}

BOOST_AUTO_TEST_SUITE_END()
