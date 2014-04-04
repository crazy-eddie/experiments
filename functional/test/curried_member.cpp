#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Curry members
#include <boost/test/unit_test.hpp>

#include "../inc/core/curried_member.hpp"

namespace {

struct whatever
{
	FUNCTIONAL_CORE_CURRIED_MEMBER(inlined, 2, int, (int i, char c))
	{
		return i + c;
	}

	FUNCTIONAL_CORE_CURRIED_MEMBER(non_inlined, 2, int, (int, char));

	FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(templated, 2);

	template < typename T >
	int templated_uncurried(int i, T t) const
	{
		return i + t + 5;
	}

	// Doing this of course provides ample opportunity for calling the wrong function
	// an overloaded that has both a 2 and 3 parameter overload being supplied 2
	// arguments will always call the 2 parameter overload, it will not return a possible curry.
	// for now anyway...something to work on (final curry result can itself be some sort callable object)
	FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(overloaded, 2);

	int overloaded_uncurried(int i, char c) const
	{
		return i + c + 1;
	}

	int overloaded_uncurried(int i, char * ) const
	{
		return i + 19;
	}
};

int whatever::non_inlined_uncurried(int i, char c) const
{
	return i + c + 2;
}

}

BOOST_AUTO_TEST_CASE(curried_member)
{
	whatever what;

	BOOST_CHECK_EQUAL(what.inlined(4,'c' - 'a'), 6);
	BOOST_CHECK_EQUAL(what.inlined(4)('c' - 'a'), 6);
	BOOST_CHECK_EQUAL(what.inlined()(4)('c' - 'a'), 6);

	BOOST_CHECK_EQUAL(what.non_inlined(4,'c' - 'a'), 8);
	BOOST_CHECK_EQUAL(what.non_inlined(4)('c' - 'a'), 8);
	BOOST_CHECK_EQUAL(what.non_inlined()(4)('c' - 'a'), 8);

	BOOST_CHECK_EQUAL(what.templated(4,'c' - 'a'), 11);
	BOOST_CHECK_EQUAL(what.templated(4)('c' - 'a'), 11);
	BOOST_CHECK_EQUAL(what.templated()(4)('c' - 'a'), 11);

	BOOST_CHECK_EQUAL(what.overloaded(4, 'c' - 'a'), 7);
	BOOST_CHECK_EQUAL(what.overloaded(4, nullptr), 23);
}
