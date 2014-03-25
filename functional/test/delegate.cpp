#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Delegate
#include <boost/test/unit_test.hpp>

#include "../inc/core/util/delegate.hpp"

#include <functional>


struct blah
{
    int f1() { return 5; }
    int f2() { return 12; }
};

BOOST_AUTO_TEST_CASE(members)
{
    blah b;

    auto f1 = delegate<int()>{&b, &blah::f1};
    auto f2 = delegate<int()>{&b, &blah::f2};

    BOOST_CHECK_EQUAL(f1(), 5);
    BOOST_CHECK_EQUAL(f2(), 12);
}

BOOST_AUTO_TEST_CASE(functors)
{
    delegate<int()> f1 = std::bind(&blah::f1, blah());
    delegate<int()> f2 = std::bind(&blah::f2, blah());

    BOOST_CHECK_EQUAL(f1(), 5);
    BOOST_CHECK_EQUAL(f2(), 12);
}

BOOST_AUTO_TEST_CASE(lambda)
{
    delegate<int()> f1 = [](){ return 6; };
    delegate<int()> f2 = [](){ return 7; };

    BOOST_CHECK_EQUAL(f1(), 6);
    BOOST_CHECK_EQUAL(f2(), 7);
}

int f() { return 666; }

BOOST_AUTO_TEST_CASE(functions)
{
    delegate<int()> f1 = f;

    BOOST_CHECK_EQUAL(f1(), 666);
}
