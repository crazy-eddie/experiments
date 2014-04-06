#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Fundamentals
#include <boost/test/unit_test.hpp>

#include "../inc/core/fundamentals.hpp"

// All types are also functions that return themselves.
// On top of that functions have a value() member that evaluates their result.

namespace fn = functional::core;


BOOST_AUTO_TEST_CASE(construction)
{
	fn::int_ i = 5;

	BOOST_CHECK_EQUAL(i, 5); // checks both that == works and that
}

BOOST_AUTO_TEST_CASE(math)
{
	fn::int_ i0 = 5;
	fn::int_ i1 = 2;
	fn::int_ i2 = 6;

	BOOST_CHECK_EQUAL((i0 + i1).value(), 7);
	BOOST_CHECK_EQUAL(fn::sum(i0,i1,i2).value(), 13);
}

BOOST_AUTO_TEST_CASE(mixed)
{
	fn::int_ i = 5;
	fn::char_ c = 'c' - 'a';
	fn::double_ d = 6.7;

	fn::int_ result = static_cast<fn::int_>(i + c + d);

	BOOST_CHECK_EQUAL(result, 13);
}

BOOST_AUTO_TEST_CASE(id_fun)
{
	BOOST_CHECK_EQUAL(fn::int_(3), fn::id(fn::int_(3)).value());
}
