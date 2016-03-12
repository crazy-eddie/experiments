#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE byte_buffer
#include <boost/test/unit_test.hpp>

#include "../include/byte_buffer.hpp"

BOOST_AUTO_TEST_CASE(construct_empty)
{
    auto buf0 = utility::byte_buffer{};

    BOOST_CHECK_EQUAL(buf0.size(), 0);
    BOOST_CHECK_EQUAL(std::begin(buf0), std::end(buf0));
}

BOOST_AUTO_TEST_CASE(construct_cstr)
{
    auto buf0 = utility::byte_buffer{"Hello World!"};

    BOOST_CHECK_EQUAL(buf0.size(), strlen("Hello World!"));
    BOOST_CHECK_NE(std::begin(buf0), std::end(buf0));
    BOOST_CHECK_EQUAL(buf0, "Hello World!");
}

char * create_buf()
{
    return new char[10];
}

BOOST_AUTO_TEST_CASE(construct_adopt)
{
    auto ptr0 = new char[10];
    auto ptr1 = ptr0;

    auto buf0 = utility::byte_buffer{std::move(ptr0), 10};

    BOOST_CHECK_EQUAL(buf0.size(), 10);
    BOOST_CHECK_EQUAL(std::begin(buf0), ptr1);
    BOOST_CHECK(ptr0 == nullptr);

    auto buf1 = utility::byte_buffer{create_buf(), 10};

    BOOST_CHECK_EQUAL(buf1.size(), 10);
}

BOOST_AUTO_TEST_CASE(copy_construct)
{
    auto buf0 = utility::byte_buffer{"Hello World"};
    auto buf1 = buf0;

    BOOST_CHECK_EQUAL(buf0.size(), buf1.size());
    BOOST_CHECK_EQUAL(buf0, buf1);
}

BOOST_AUTO_TEST_CASE(move_construct)
{
    auto buf0 = utility::byte_buffer{"Hello World!"};
    auto buf1 = std::move(buf0);

    BOOST_CHECK_EQUAL(buf0.size(), 0);
    BOOST_CHECK_NE(buf0, buf1);

    BOOST_CHECK_EQUAL(buf1, "Hello World!");
}

BOOST_AUTO_TEST_CASE(append)
{
    auto buf0 = utility::byte_buffer{"Hello"};

    buf0.append(" World");

    BOOST_CHECK_EQUAL(buf0, "Hello World");
}

