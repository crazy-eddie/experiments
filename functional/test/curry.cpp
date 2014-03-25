#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Curry
#include <boost/test/unit_test.hpp>

#include "../inc/core/curry.hpp"

#include <boost/mpl/vector.hpp>
#include <type_traits>

int fun1(int i) { return i + 5; }
int fun2(int i, int j) { return i + j; }
int fun3(int i, char c, double d) { return static_cast<int>(i + c + d); }

#if 0
BOOST_AUTO_TEST_CASE(packer)
{
    typedef boost::mpl::vector<int,double,char> vtype;
    BOOST_CHECK((std::is_same< functional::core::sequence_to_pack<vtype>::type
                             , functional::core::packed<int,double,char> >::value));
}

BOOST_AUTO_TEST_CASE(curry)
{
    using functional::core::curry;

    BOOST_CHECK_EQUAL(curry(fun2)(5)(3), 8);
    BOOST_CHECK_EQUAL(curry(fun1)(5), 10);
    BOOST_CHECK_EQUAL(curry(fun3)(23)('e' - 'a')(32.6), 59);

    BOOST_CHECK_EQUAL(curry(fun3)(23, 'e' - 'a')(32.6), 59);
}

#endif

BOOST_AUTO_TEST_CASE(invoke)
{
    using functional::core::detail_::invoke;

    auto args = std::make_tuple(23, 'e' - 'a', 32.6);

    BOOST_CHECK_EQUAL(invoke(fun3, args), 59);
}
