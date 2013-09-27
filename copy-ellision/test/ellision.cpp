#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ellision

#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include "../inc/ellision.hpp"

int const vec_size = 10000;
int const loop_count = 10000;

BOOST_AUTO_TEST_CASE(by_reference)
{
	boost::timer::auto_cpu_timer timer;

	bool check = false;

	for (int i = 0; i < loop_count; ++i)
	{
		std::vector<int> vec;
		generate_vector(vec, vec_size);

		sort_vector_ref(vec);

		check = sorted(vec);
	}

	BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(by_value)
{
	boost::timer::auto_cpu_timer timer;

	bool check = false;

	for (int i = 0; i < loop_count; ++i)
	{
		check = sorted(sort_vector_val(generate_vector(vec_size)));
	}
	BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(by_value2)
{
	boost::timer::auto_cpu_timer timer;

	bool check = false;

	for (int i = 0; i < loop_count; ++i)
	{
		std::vector<int> vec = generate_vector(vec_size);
		std::vector<int> svec = sort_vector_val(vec);
		check = sorted(svec);
	}
	BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(overwrite)
{
	boost::timer::auto_cpu_timer timer;

	bool check = false;

	std::vector<int> vec;
	for (int i = 0; i < loop_count; ++i)
	{
		generate_vector(vec, vec_size);
		sort_vector_ref(vec);
		check = sorted(vec);

		vec.clear();
	}
	BOOST_CHECK(check);
}
