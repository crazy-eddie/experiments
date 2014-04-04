#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Curry members
#include <boost/test/unit_test.hpp>

#include "../inc/core/curried_member.hpp"
#include "../inc/core/fundamentals.hpp"

using functional::core::int_;
using functional::core::char_;


namespace {

struct whatever
{
	FUNCTIONAL_CORE_CURRIED_MEMBER(inlined, 2, int_, (int_ i, char_ c))
	{
		return i + c;
	}

	FUNCTIONAL_CORE_CURRIED_MEMBER(non_inlined, 2, int_, (int_, char_));

	FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(templated, 2);

	template < typename T >
	int_ templated_uncurried(int_ i, T t) const
	{
		return i + t + 5;
	}

	// Doing this of course provides ample opportunity for calling the wrong function
	// an overloaded that has both a 2 and 3 parameter overload being supplied 2
	// arguments will always call the 2 parameter overload, it will not return a possible curry.
	// for now anyway...something to work on (final curry result can itself be some sort callable object)
	FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(overloaded, 2);
	//FUNCTIONAL_CORE_CURRIED_MEMBER_CALLER(overloaded, 3); // not sure this will be possible.  Maybe a more complex object will do.

	int_ overloaded_uncurried(int_ i, char_ c) const
	{
		return i + c + 1;
	}

	int_ overloaded_uncurried(int_ i, char_ c, int_ last) const
	{
		return i + c + last;
	}

	int_ overloaded_uncurried(int_ i, char_ * ) const
	{
		return i + 19;
	}
};

int_ whatever::non_inlined_uncurried(int_ i, char_ c) const
{
	return i + c + 2;
}

}

BOOST_AUTO_TEST_CASE(curried_member)
{
	whatever what;

	BOOST_CHECK_EQUAL(what.inlined(4,'c' - 'a').value(), 6);
	BOOST_CHECK_EQUAL(what.inlined(4)('c' - 'a').value(), 6);
	BOOST_CHECK_EQUAL(what.inlined()(4)('c' - 'a').value(), 6);

	BOOST_CHECK_EQUAL(what.non_inlined(4,'c' - 'a').value(), 8);
	BOOST_CHECK_EQUAL(what.non_inlined(4)('c' - 'a').value(), 8);
	BOOST_CHECK_EQUAL(what.non_inlined()(4)('c' - 'a').value(), 8);

	BOOST_CHECK_EQUAL(what.templated(4,'c' - 'a').value(), 11);
	BOOST_CHECK_EQUAL(what.templated(4)('c' - 'a').value(), 11);
	BOOST_CHECK_EQUAL(what.templated()(4)('c' - 'a').value(), 11);

	BOOST_CHECK_EQUAL(what.overloaded(4, 'c' - 'a').value(), 7);
	BOOST_CHECK_EQUAL(what.overloaded(4, nullptr).value(), 23);
	//BOOST_CHECK_EQUAL(what.overloaded(4, 'c' - 'a', int_(5)).value(), 12);
	//BOOST_CHECK_EQUAL(what.overloaded(4, 'c' - 'a')(5).value(), 12);
}

struct what2
{
	FUNCTIONAL_CORE_CURRIED_MEMBER(fun, 2, int_, (int_ i0, int_ i1))
	{
		return mem + i0 + i1;
	}

	what2(int_ i) : mem(i) {}

private:
	int_ mem;
};

auto fun = [](){ what2 w(22); return w.fun(2); };

BOOST_AUTO_TEST_CASE(temporary)
{
	BOOST_CHECK_EQUAL(fun()(5).value(), 29);
}
