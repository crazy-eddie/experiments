#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE List
#include <boost/test/unit_test.hpp>

#include "../inc/core/fundamentals.hpp"
#include "../inc/data/list.hpp"

using namespace functional;

BOOST_AUTO_TEST_SUITE(list_functionality)

BOOST_AUTO_TEST_CASE(empty_list)
{
	data::list<core::int_> empty;

	BOOST_CHECK_THROW(empty.head(), std::domain_error);

	BOOST_CHECK_THROW(empty.tail(), std::domain_error);

	BOOST_CHECK(empty == empty.value());
	BOOST_CHECK(empty == empty());

	BOOST_CHECK(empty == data::list<core::int_>::empty());
}

BOOST_AUTO_TEST_CASE(single_elem_list)
{
	data::list<core::int_> list(5);

	BOOST_CHECK_EQUAL(list.length(), 1);

	BOOST_CHECK_NO_THROW(list.head());
	BOOST_CHECK_NO_THROW(list.tail());

	BOOST_CHECK_EQUAL(list.head(), 5);
	BOOST_CHECK(list.tail() == list.empty());
}

BOOST_AUTO_TEST_CASE(list_push)
{
	data::list<core::int_> list(5, data::list<core::int_>(3));

	BOOST_CHECK_EQUAL(list.length(), 2);

	BOOST_CHECK_NO_THROW(list.head());
	BOOST_CHECK_NO_THROW(list.tail());
	BOOST_CHECK_NO_THROW(list.tail().head());
	BOOST_CHECK_NO_THROW(list.tail().tail());

	BOOST_CHECK_EQUAL(list.head(), 5);
	BOOST_CHECK_EQUAL(list.tail().length(), 1);
	BOOST_CHECK_EQUAL(list.tail().head(), 3);

	BOOST_CHECK(list.tail().tail() == list.empty());
}

BOOST_AUTO_TEST_CASE(list_append)
{
}

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
