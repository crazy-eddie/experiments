#define BOOST_TEST_MODULE metamap
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/metamap.hpp"

BOOST_AUTO_TEST_CASE(start_empty)
{
	auto m0 = meta::map<std::string>();
}
