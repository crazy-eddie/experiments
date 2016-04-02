#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE invocation-list
#include <boost/test/unit_test.hpp>

#include "../include/invocation_list.hpp"

int fun0() { return 5; }
int fun1() { return 6; }
struct fun2_
{
    constexpr fun2_() {}
    int operator()() const { return 7; }
};
constexpr auto fun2 = fun2_{};

BOOST_AUTO_TEST_CASE(avoid_unused)
{
    BOOST_CHECK_EQUAL(fun2(), 7);
}

BOOST_AUTO_TEST_CASE(pair)
{
    constexpr auto p0 = make_invocation_pair(9, fun0);

    static_assert(p0.key == 9);
    BOOST_CHECK_EQUAL(p0.value(), 5);
}

BOOST_AUTO_TEST_CASE(simple)
{
    constexpr auto il0
        = make_invocation_list<int()>(
                make_invocation_pair(9, fun0)
              , make_invocation_pair(99, fun1)
              , make_invocation_pair(6, fun2));

    BOOST_CHECK_EQUAL(il0.invoke(9), 5);

    BOOST_CHECK_EQUAL(invoke(il0, 9), 5);
    BOOST_CHECK_EQUAL(invoke(il0, 99), 6);
    BOOST_CHECK_EQUAL(invoke(il0, 6), 7);

    BOOST_CHECK_THROW(invoke(il0, 666), std::domain_error);
}

BOOST_AUTO_TEST_CASE(void_invocation)
{
    constexpr auto il0
        = make_invocation_list<int()>(
                make_invocation_pair(9, fun0)
              , make_invocation_pair(99, fun1)
              , make_invocation_pair(6, fun2));

    BOOST_CHECK_EQUAL((void_invoke<decltype(il0), int>(static_cast<void const*>(&il0), 9)), 5);
}


BOOST_AUTO_TEST_CASE(pointer_invocation)
{
    constexpr auto il0
        = make_invocation_list<int()>(
                make_invocation_pair(9, fun0)
              , make_invocation_pair(99, fun1)
              , make_invocation_pair(6, fun2));

    using fun_type = int (*)(void const*, int const&);
    constexpr fun_type funs[] =
    {
        void_invoke<decltype(il0), int>
      , void_invoke<decltype(il0), int>
      , void_invoke<decltype(il0), int>
    };

    BOOST_CHECK_EQUAL(funs[0](&il0, 9), 5);
    BOOST_CHECK_EQUAL(funs[1](&il0, 9), 5);
    BOOST_CHECK_EQUAL(funs[0](&il0, 99), 6);

    // nope...
    //constexpr auto i1 = void_invoke<decltype(il0), int>(&il0, 9);
    //static_assert(i1 == 9);
}
