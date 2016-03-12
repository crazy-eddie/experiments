#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE immutable_string
#include <boost/test/unit_test.hpp>

#include "../include/string.hpp"

#include <string>

BOOST_AUTO_TEST_CASE(basic_creation)
{
    auto str0 = utility::string{};

    BOOST_CHECK(str0.empty());
    BOOST_CHECK_EQUAL(str0.size(), 0);
    BOOST_CHECK(str0.size() == 0);

    auto str1 = utility::string{"Hello world!"};

    BOOST_CHECK(!str1.empty());
    BOOST_CHECK_EQUAL(str1.size(), std::string("Hello world!").size());

    BOOST_CHECK(std::equal(str1.data(), str1.data() + str1.size(), "Hello world!"));
    BOOST_CHECK(std::equal(std::begin(str1), std::end(str1), "Hello world!"));
}

BOOST_AUTO_TEST_CASE(basic_copy)
{
    auto str0 = utility::string{"Hello World!"};
    auto str1 = str0;

    BOOST_CHECK_EQUAL(str0, str1);
    BOOST_CHECK_EQUAL(str0.data(), str1.data());

    auto str2 = utility::string{"Hello"};
    BOOST_CHECK_NE(str0, str2);
}

BOOST_AUTO_TEST_CASE(substr)
{
    auto str0 = utility::string{"Hello World!"};
    auto str1 = str0.substr(0);
    auto str2 = str0.substr(0, 4);
    auto str3 = str0.substr(6);
    auto str4 = str0.substr(6, 5);
    auto str5 = str3.substr(0,4);

    BOOST_CHECK_EQUAL(str0, str1);
    BOOST_CHECK_EQUAL(str2, "Hell");
    BOOST_CHECK_EQUAL(str3, "World!");
    BOOST_CHECK_EQUAL(str4, "World");
    BOOST_CHECK_EQUAL(str5, "Worl");
}


BOOST_AUTO_TEST_CASE(algorithms)
{
    auto str0 = utility::string{"Hello World!"};

    auto str1 = std::string{};

    std::copy(std::begin(str0), std::end(str0), std::back_inserter(str1));

    BOOST_CHECK(std::equal(std::begin(str0), std::end(str0), std::begin(str1)));
}

