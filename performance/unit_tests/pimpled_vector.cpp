#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE pimpled vector
#include <boost/test/unit_test.hpp>

#include "../include/crazy/util/pimpl.hpp"
#include "../include/crazy/util/pimpled_vector.hpp"

#include "test_pimpl.hpp"

BOOST_AUTO_TEST_CASE(empty)
{
	test_vector vect;

	BOOST_CHECK(vect.empty());
}

BOOST_AUTO_TEST_CASE(push_back)
{
	{
		test_vector vect;

		vect.push_back(test_class(42));

		BOOST_CHECK(!vect.empty());
		BOOST_CHECK_EQUAL(vect.size(), 1U);

		BOOST_CHECK_EQUAL(vect[0].value(), 42);

		BOOST_CHECK_EQUAL(test_class::impl_count(), 1);

		test_class test = vect[0];

		BOOST_CHECK_EQUAL(test_class::impl_count(), 2);
	}
	BOOST_CHECK_EQUAL(test_class::impl_count(), 0);
}
