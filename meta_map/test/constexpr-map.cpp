#define BOOST_TEST_MODULE metamap
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/constexpr-map.hpp"

BOOST_AUTO_TEST_CASE(start_empty)
{
    constexpr constmap<char const*> m0{};

    constexpr auto m1 = m0.insert("hello", "world");
    //static_assert(std::is_same<decltype(m1["hello"]), char const*>::value, "Should be same.");

    //BOOST_CHECK_EQUAL(std::string(m1["hello"]), "world");

    //constexpr auto m2 = m1.insert("bob", "your momma");
}
