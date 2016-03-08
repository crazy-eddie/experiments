#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE runtime_trie
#include <boost/test/unit_test.hpp>

#include "../include/trie.hpp"

BOOST_AUTO_TEST_CASE(empty_trie)
{
    auto t0 = utilities::trie{};

    BOOST_CHECK_EQUAL(t0.size(), 0);
}

BOOST_AUTO_TEST_CASE(insert)
{
    auto t0 = utilities::trie{};

    t0.insert(std::make_pair("hello", "world"));

    BOOST_CHECK_EQUAL(t0.size(), 1);
    BOOST_CHECK_EQUAL(t0["hello"], "world");

    t0.insert(std::make_pair("hell", "life"));

    BOOST_CHECK_EQUAL(t0.size(), 2);
    BOOST_CHECK_EQUAL(t0["hell"], "life");
}
