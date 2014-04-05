#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE List
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(list_functionality)

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(functor_laws)

// mapping id() over the functor returns the original functor.
BOOST_AUTO_TEST_CASE(law1_id_function)
{
}

// fmap(f * g)(x) == fmap(f)(x) * fmap(g)(x)
BOOST_AUTO_TEST_CASE(law2_composition)
{
}

BOOST_AUTO_TEST_CASE(fmap_curries)
{
}

BOOST_AUTO_TEST_SUITE_END()
