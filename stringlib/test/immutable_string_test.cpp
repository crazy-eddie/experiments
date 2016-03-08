#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE immutable_string
#include <boost/test/unit_test.hpp>

#include "../include/string.hpp"

#include <string>

BOOST_AUTO_TEST_CASE(basic_creation)
{
    utilities::string str0{};

    BOOST_CHECK(str0.empty());
    BOOST_CHECK_EQUAL(str0.size(), 0);
    BOOST_CHECK(str0.size() == 0);

    utilities::string str1{"Hello world!"};

    BOOST_CHECK(!str1.empty());
    BOOST_CHECK_EQUAL(str1.size(), std::string("Hello world!").size());

    BOOST_CHECK(std::equal(str1.data(), str1.data() + str1.size(), "Hello world!"));
    BOOST_CHECK(std::equal(std::begin(str1), std::end(str1), "Hello world!"));
}

BOOST_AUTO_TEST_CASE(basic_copy)
{
    utilities::string str0 = "Hello World!";
    utilities::string str1 = str0;

    BOOST_CHECK_EQUAL(str0, str1);
    BOOST_CHECK_EQUAL(str0.data(), str1.data());

    utilities::string str2 = "Hello";
    BOOST_CHECK_NE(str0, str2);
}

BOOST_AUTO_TEST_CASE(substr)
{
    utilities::string str0 = "Hello World!";
    utilities::string str1 = str0.substr(0);
    utilities::string str2 = str0.substr(0, 4);
    utilities::string str3 = str0.substr(6);
    utilities::string str4 = str0.substr(6, 5);
    utilities::string str5 = str3.substr(0,4);

    BOOST_CHECK_EQUAL(str0, str1);
    BOOST_CHECK_EQUAL(str2, "Hell");
    BOOST_CHECK_EQUAL(str3, "World!");
    BOOST_CHECK_EQUAL(str4, "World");
    BOOST_CHECK_EQUAL(str5, "Worl");
}

BOOST_AUTO_TEST_CASE(algorithms)
{
    utilities::string str0 = "Hello World!";

    std::string str1{};

    std::copy(std::begin(str0), std::end(str0), std::back_inserter(str1));

    BOOST_CHECK(std::equal(std::begin(str0), std::end(str0), std::begin(str1)));
}
