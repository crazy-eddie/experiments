#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE List
#include <boost/test/unit_test.hpp>

#include "../inc/core/fundamentals.hpp"
#include "../inc/data/list.hpp"
#include "../inc/core/functor.hpp"

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
	using list_t = data::list<core::int_>;

	list_t list0(2, list_t(3, list_t(7)));
	list_t list1(9, list_t(1));

	list_t list2 = list0 + list1;

	BOOST_CHECK_EQUAL(list2.length(), 5);

	BOOST_CHECK_EQUAL(list2.head(), 2);
	BOOST_CHECK_EQUAL(list2.tail().head(), 3);
	BOOST_CHECK_EQUAL(list2.tail().tail().head(), 7);
	BOOST_CHECK_EQUAL(list2.tail().tail().tail().head(), 9);
	BOOST_CHECK_EQUAL(list2.tail().tail().tail().tail().head(), 1);
}

BOOST_AUTO_TEST_CASE(map)
{
	using list_t = data::list<core::int_>;
	auto fun = [](core::int_ const& i) { return i * core::int_(10); };

	list_t list(4, list_t(2, list_t(7)));

	list_t mapped = list.map(fun);

	BOOST_CHECK_EQUAL(mapped.length(), 3);
	BOOST_CHECK_EQUAL(mapped.head(), 40);
	BOOST_CHECK_EQUAL(mapped.tail().head(), 20);
	BOOST_CHECK_EQUAL(mapped.tail().tail().head(), 70);

	BOOST_CHECK(mapped == core::fmap(fun, list).value());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(functor_laws)

// mapping id() over the functor returns the original functor.
BOOST_AUTO_TEST_CASE(law1_id_function)
{
	using list_t = data::list<core::int_>;

	list_t list(3, list_t(7, list_t(0)));

	BOOST_CHECK(list == core::fmap(core::id, list).value());
}

// fmap(f * g)(x) == fmap(f)(x) * fmap(g)(x)
BOOST_AUTO_TEST_CASE(law2_composition)
{
	auto f = core::curry([](core::int_ i) { return i * core::int_(10); });
	auto g = core::curry([](core::int_ i) { return i + core::int_(11); });

	using list_t = data::list<core::int_>;

	list_t list0(1, list_t(2, list_t(3)));

	list_t list1 = core::fmap(f * g)(list0);
	list_t list2 = (core::fmap(f) * core::fmap(g))(list0);

	BOOST_CHECK(list1 == list2);
}

BOOST_AUTO_TEST_CASE(fmap_curries)
{
}

BOOST_AUTO_TEST_SUITE_END()
