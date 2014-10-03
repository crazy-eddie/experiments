#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE json-fundamentals
#include <boost/test/unit_test.hpp>

#include "../include/json.hpp"

BOOST_AUTO_TEST_SUITE(numbers)


BOOST_AUTO_TEST_CASE(read_result_type)
{
    BOOST_CHECK((std::is_same<decltype(json::number.read("")), boost::optional<double>>::value));
    BOOST_CHECK((std::is_same<decltype(json::number.nonull().read("")), double>::value));
}

BOOST_AUTO_TEST_CASE(read_value)
{
    auto non_null = json::number.nonull();

    auto v0 = json::read(non_null, " 666 ");

    BOOST_CHECK_EQUAL(v0, 666);

    auto v1 = json::read(non_null, "66.6");
    BOOST_CHECK_EQUAL(static_cast<int>(v1 * 10), 666);

    auto v2 = json::read(non_null, "66.6e2");
    BOOST_CHECK_EQUAL(static_cast<int>(v2 + .5), 6660);

    auto v3 = json::read(non_null, "66600E-2");
    BOOST_CHECK_EQUAL(static_cast<int>(v3 + .5), 666);

    BOOST_CHECK_THROW(json::read(non_null, "null"), std::exception);
    BOOST_CHECK_THROW(json::read(non_null, ""), std::exception);

    BOOST_CHECK(!json::read(json::number, "null"));
    BOOST_CHECK_EQUAL(static_cast<int>(*json::read(json::number, "6.66e2")), 666);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(boolean)


BOOST_AUTO_TEST_CASE(read_value_type)
{
    BOOST_CHECK((std::is_same<decltype(json::boolean.read("")), boost::optional<bool>>::value));
    BOOST_CHECK((std::is_same<decltype(json::boolean.nonull().read("")), bool>::value));
}

BOOST_AUTO_TEST_CASE(read_value)
{
    auto non_null = json::boolean.nonull();

    auto b0 = json::read(non_null, "false");
    BOOST_CHECK(!b0);

    auto b1 = json::read(non_null, "true");
    BOOST_CHECK(b1);

    BOOST_CHECK_THROW(json::read(non_null, "null"), std::exception);

    auto b3 = json::read(json::boolean, "null");
    BOOST_CHECK(!b3);
}


BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(string)

BOOST_AUTO_TEST_CASE(read_value_type)
{
    BOOST_CHECK((std::is_same<decltype(json::string.read("")), boost::optional<std::string>>::value));
    BOOST_CHECK((std::is_same<decltype(json::string.nonull().read("")), std::string>::value));
}

BOOST_AUTO_TEST_CASE(read_value)
{
    auto no_null = json::string.nonull();

    BOOST_CHECK_THROW(json::read(no_null, "hello"), std::exception);

    auto s1 = json::read(no_null, "\"hello\"");
    BOOST_CHECK_EQUAL(s1, "hello");

    auto s2 = json::read(json::string, "null");
    BOOST_CHECK(!s2);

    auto s3 = json::read(json::string, "\"hello\"");
    BOOST_CHECK(s3);
    BOOST_CHECK_EQUAL(*s3, "hello");
}


BOOST_AUTO_TEST_SUITE_END()
