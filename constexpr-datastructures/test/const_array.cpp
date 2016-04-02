#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE const-array
#include <boost/test/unit_test.hpp>

#include "../include/const_array.hpp"

BOOST_AUTO_TEST_CASE(empty)
{
    auto ar0 = make_array<int>();

    BOOST_CHECK_EQUAL(ar0.size(), 0);
    BOOST_CHECK_EQUAL(ar0.empty(), true);

    constexpr auto ar1 = make_array<int>();

    static_assert(ar1.size() == 0, "FAIL");
    static_assert(ar1.empty(), "FAIL");
}

BOOST_AUTO_TEST_CASE(non_empty)
{
    constexpr auto ar0 = make_array<int>(1,4,9,42,0, 16);

    static_assert(ar0.size() == 6, "FAIL");
    static_assert(!ar0.empty(), "FAIL");

    static_assert(ar0[0] == 1, "FAIL");
    static_assert(ar0.at<3>() == 42, "FAIL");
}

BOOST_AUTO_TEST_CASE(assign)
{
    constexpr auto ar0 = make_array<int>(0,1,2);

    constexpr auto ar1 = ar0.set<1>(42);

    static_assert(ar1.size() == 3, "FAIL");
    static_assert(!ar1.empty(), "FAIL");
    static_assert(ar1[1] == 42, "FAIL");
}
