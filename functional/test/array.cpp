#define BOOST_TEST_MODULE array
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/range/irange.hpp>


#include "../inc/array.hpp"

using namespace functional;

BOOST_AUTO_TEST_CASE(initialize)
{
	constexpr auto arr = make_array(1,0,2,0,3,0);

	BOOST_CHECK_EQUAL(arr.size(), 6);

	BOOST_CHECK_EQUAL(arr.data()[0], 1);
	BOOST_CHECK_EQUAL(arr.data()[1], 0);
	BOOST_CHECK_EQUAL(arr.data()[2], 2);
	BOOST_CHECK_EQUAL(arr.data()[3], 0);
	BOOST_CHECK_EQUAL(arr.data()[4], 3);
	BOOST_CHECK_EQUAL(arr.data()[5], 0);
}

BOOST_AUTO_TEST_CASE(array_push_back)
{
	constexpr auto arr = make_array(1,2,3);

	constexpr auto arr2 = push_back(arr, 5);

	BOOST_CHECK_EQUAL(arr2.size(), 4);
	BOOST_CHECK_EQUAL(arr2.data()[3], 5);
}

BOOST_AUTO_TEST_CASE(array_append)
{
    constexpr auto arr1 = make_array(1,2,3);
    constexpr auto arr2 = make_array(4,5,6);
    constexpr auto arr3 = arr1 + arr2;

    BOOST_REQUIRE_EQUAL(arr3.size(), 6);

    for (auto i : boost::irange(1,6))
        BOOST_CHECK_EQUAL(arr3.data()[i-1], i);
}

template < int I > struct testit { enum { value = I }; };
BOOST_AUTO_TEST_CASE(array_as_template_param)
{
    constexpr auto arr1 = make_array(1,2,3);

    int i1 = testit<arr1.size()>::value;
    int i2 = testit<arr1.data()[1]>::value;

    BOOST_CHECK_EQUAL(i1, 3);
    BOOST_CHECK_EQUAL(i2, 2);

    constexpr auto arr2 = make_array(4,5,6);
    constexpr auto arr3 = arr1 + arr2;

    int i3 = testit<arr3.data()[4]>::value;
    BOOST_CHECK_EQUAL(i3,5);
}
