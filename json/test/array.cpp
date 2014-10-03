#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE json-array
#include <boost/test/unit_test.hpp>

#include "../include/json.hpp"


BOOST_AUTO_TEST_CASE(read_value_type)
{
    auto arr = json::array(json::number.nonull());

    BOOST_CHECK((std::is_same<decltype(arr.read("")), boost::optional<std::vector<double>>>::value));
    BOOST_CHECK((std::is_same<decltype(arr.nonull().read("")), std::vector<double>>::value));
}

BOOST_AUTO_TEST_CASE(read_value)
{
    auto no_null = json::array(json::number.nonull()).nonull();

    auto a0 = json::read(no_null, "[666]");
    BOOST_CHECK_EQUAL(a0.size(),1U);
    BOOST_CHECK_EQUAL(static_cast<int>(a0[0]), 666);

    auto a1 = json::read(no_null, "[666,42]");
    BOOST_CHECK_EQUAL(a1.size(), 2U);
    BOOST_CHECK_EQUAL(static_cast<int>(a1[0]), 666);
    BOOST_CHECK_EQUAL(static_cast<int>(a1[1]), 42);

    auto a2 = json::read(no_null, "[]");
    BOOST_CHECK_EQUAL(a2.size(), 0U);

    auto a3 = json::read(json::array(json::boolean.nonull()), "null");
    BOOST_CHECK(!a3);

    auto a4 = json::read(json::array(json::string).nonull(), "[\"hello\", null, \"world\"]");
    BOOST_CHECK_EQUAL(a4.size(), 3U);
    BOOST_CHECK(a4[0]);
    BOOST_CHECK(!a4[1]);
    BOOST_CHECK(a4[2]);
}

