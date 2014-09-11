#define BOOST_TEST_MODULE metamap
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/string.hpp"

BOOST_AUTO_TEST_CASE(compare)
{
	BOOST_CHECK((std::is_same<meta::str_equal, meta::string_compare<META_STRING("hello"),META_STRING("hello")>::type>::value));
	BOOST_CHECK((std::is_same<meta::str_less, meta::string_compare<META_STRING("abc"),META_STRING("bcd")>::type>::value));
	BOOST_CHECK((std::is_same<meta::str_less, meta::string_compare<META_STRING("hell"),META_STRING("hello")>::type>::value));
	BOOST_CHECK((std::is_same<meta::str_greater, meta::string_compare<META_STRING("hello"),META_STRING("hell")>::type>::value));
}
