#define BOOST_TEST_MODULE metamap
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/metamap.hpp"

BOOST_AUTO_TEST_CASE(derp) {}

BOOST_AUTO_TEST_CASE(start_empty)
{
	auto m0 = meta::map<std::string>();

	auto m1 = m0.insert<META_STRING("hello")>("world");

	BOOST_CHECK(m1.has("hello"));
	BOOST_CHECK_EQUAL(m1["hello"], "world");

	BOOST_CHECK(!m1.has("world"));
	BOOST_CHECK_THROW(m1["world"], std::logic_error);

	auto m2 = m1.insert<META_STRING("herp")>("derp");

	BOOST_CHECK(m2.has("hello"));
	BOOST_CHECK(m2.has("herp"));
	BOOST_CHECK(!m2.has("world"));

	BOOST_CHECK_EQUAL(m2["hello"], "world");
	BOOST_CHECK_EQUAL(m2["herp"], "derp");
	BOOST_CHECK_THROW(m2["world"], std::logic_error);
}
