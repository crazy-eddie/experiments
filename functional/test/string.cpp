#define BOOST_TEST_MODULE functional_string
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../inc/string.hpp"

using namespace functional;

BOOST_AUTO_TEST_CASE(string_length)
{
	constexpr string s{"hello!"};

	BOOST_CHECK_EQUAL(s.length(), 6);
}

BOOST_AUTO_TEST_CASE(string_equality)
{
	constexpr string s0{"hello!"};
	constexpr string s1{"helLo!"};
	constexpr string s2{"hello!"};

	BOOST_CHECK(s0 != s1);
	BOOST_CHECK(s0 == s2);
}

BOOST_AUTO_TEST_CASE(string_compare)
{
	constexpr string s0{"01234"};
	constexpr string s1{"0123"};
	constexpr string s2{"01235"};

	BOOST_CHECK(s0 < s2);
	BOOST_CHECK(s1 < s2);
	BOOST_CHECK(!(s2 < s0));
	BOOST_CHECK(!(s2 < s1));

	BOOST_CHECK(s2 > s0);
	BOOST_CHECK(s2 > s1);
	BOOST_CHECK(!(s0 > s2));
	BOOST_CHECK(!(s1 > s2));
}

#if 0
BOOST_AUTO_TEST_CASE(string_append)
{
	constexpr string s0{"01234"};
	constexpr string s1{"56789"};

	constexpr string s2 = s0 + s1;

	BOOST_CHECK_EQUAL(s2.length(), 10);
}
#endif
