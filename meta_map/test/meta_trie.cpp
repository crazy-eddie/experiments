#define BOOST_TEST_MODULE metatrie
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/metatrie.hpp"

BOOST_AUTO_TEST_CASE(empty_trie)
{
	auto tr = meta::trie<std::string>();

	//BOOST_CHECK(!tr.has_elem("hello"));

	//auto tr_next = tr.add_value<META_STRING("hello")>("world");

	//BOOST_CHECK(tr_next.has_elem("hello"));
	//BOOST_CHECK_EQUAL(tr_next.find("hello"), "world");

	//auto tr_again = tr_next.add_value<META_STRING("herp")>("derp");
	//BOOST_CHECK(tr_again.has_elem("herp"));
	//BOOST_CHECK_EQUAL(tr_again.find("herp"), "derp");
	//BOOST_CHECK(tr_again.has_elem("hello"));
}

#if 0
BOOST_AUTO_TEST_CASE(single_element)
{
	auto tr = meta::trie(meta::trie_entry<META_STRING("hello")>(std::string("world")));

	BOOST_CHECK_EQUAL(tr.find("hello"), "world");
}
#endif
