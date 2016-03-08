#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE mutable_string
#include <boost/test/unit_test.hpp>

#include "../include/string.hpp"

BOOST_AUTO_TEST_CASE(basic_construction)
{
    utilities::mutable_string str0{};

    BOOST_CHECK(str0.empty());
    BOOST_CHECK_EQUAL(str0.size(), 0);

    utilities::mutable_string str1 = "Hello World!";

    BOOST_CHECK(!str1.empty());
    BOOST_CHECK_EQUAL(str1.size(), std::string("Hello World!").size());
    BOOST_CHECK_EQUAL(str1.size(), 12);

    BOOST_CHECK_EQUAL(str1, "Hello World!");

    BOOST_CHECK(std::equal(str1.data(), str1.data() + str1.size(), std::string("Hello World!").begin()  ));
    BOOST_CHECK(std::equal(std::begin(str1), std::end(str1), "Hello World!"));
}


BOOST_AUTO_TEST_CASE(algorithms)
{
    utilities::mutable_string str0 = "Hello World!";

    std::string str1{};

    std::copy(std::begin(str0), std::end(str0), std::back_inserter(str1));

    BOOST_CHECK(std::equal(std::begin(str0), std::end(str0), std::begin(str1)));
}

BOOST_AUTO_TEST_CASE(appending)
{
    utilities::mutable_string str0 = "Hello";

    BOOST_CHECK_EQUAL(str0, "Hello");

    str0 += " World";

    BOOST_CHECK_EQUAL(str0, "Hello World");

    //str0 += '!';
    //BOOST_CHECK_EQUAL(str0, "Hello World!");
}
