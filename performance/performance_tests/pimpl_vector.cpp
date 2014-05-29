#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE performance:pimpl_vector
#include <boost/test/unit_test.hpp>

/*
 * So how expensive is the pimpl idiom when processing
 * vectors?  We assume here that locality matters a great deal.
 */

#include "../include/crazy/util/pimpl.hpp"

#include "../unit_tests/test_pimpl.hpp"

#include <vector>
#include <boost/timer/timer.hpp>

int const COUNT = 10000000;

struct non_pimpl
{
	non_pimpl(size_t s) : val(s) {}

	size_t value() const { return val; }

private:
	size_t val;
};

struct pimpled
{
	pimpled(size_t s) : pimpl(s) {}

	size_t value() const { return pimpl->value(); }

	struct impl
	{
		impl(size_t s) : val(s) {}

		size_t value() const { return val; }

	private:
		size_t val;
	};

private:
	crazy::util::pimpl_ptr<impl> pimpl;
};

template < typename T >
std::vector<T> create_vector(int num)
{
	std::vector<T> vect; vect.reserve(num);

	for (size_t i = 0; i != static_cast<size_t>(num); ++i)
		vect.push_back(T(i));

	return vect;
}


BOOST_AUTO_TEST_CASE(nonpimpled)
{
	auto vals = create_vector<non_pimpl>(COUNT);

	boost::timer::cpu_timer timer;
#if 1
	size_t sz = std::accumulate( std::begin(vals), std::end(vals)
	                           , size_t()
	                           , [](auto a, auto b)
	                             {
								     return a + b.value();
	                             } );
#else
	size_t sz = 0;
	for (size_t i = 0; i != vals.size(); ++i)
	{
		sz += vals[i].value();
	}
#endif
	timer.stop();

	BOOST_CHECK_EQUAL(sz, 49999995000000U);
	BOOST_MESSAGE("Time to run: " << timer.format());
}


BOOST_AUTO_TEST_CASE(pimpl)
{
	auto vals = create_vector<pimpled>(COUNT);

	boost::timer::cpu_timer timer;
#if 1
	size_t sz = std::accumulate( std::begin(vals), std::end(vals)
	                           , size_t()
	                           , [](auto a, auto b)
	                             {
								     return a + b.value();
	                             } );
#else
	size_t sz = 0;
	for (size_t i = 0; i != vals.size(); ++i)
	{
		sz += vals[i].value();
	}
#endif
	timer.stop();

	BOOST_CHECK_EQUAL(sz, 49999995000000U);
	BOOST_MESSAGE("Time to run: " << timer.format());
}

test_vector create_test_vector(int num)
{
	test_vector vect;

	for (size_t i = 0; i != static_cast<size_t>(num); ++i)
		vect.push_back(test_class(i));

	return vect;
}

BOOST_AUTO_TEST_CASE(pimpl_vector)
{
	auto vals = create_test_vector(COUNT);

	boost::timer::cpu_timer timer;
#if 0
	size_t sz = std::accumulate( std::begin(vals), std::end(vals)
	                           , 0
	                           , [](auto a, auto b)
	                             {
								     return a + b.value();
	                             } );
#else
	size_t sz = 0;
	for (size_t i = 0; i != vals.size(); ++i)
	{
		sz += vals[i].value();
	}
#endif
	timer.stop();

	BOOST_CHECK_EQUAL(sz, 49999995000000U);
	BOOST_MESSAGE("Time to run: " << timer.format());
}

/*
Running 2 test cases...
Time to run:  0.009282s wall, 0.010000s user + 0.000000s system = 0.010000s CPU (107.7%)

Time to run:  0.314920s wall, 0.310000s user + 0.000000s system = 0.310000s CPU (98.4%)


*** No errors detected
 *
 */
