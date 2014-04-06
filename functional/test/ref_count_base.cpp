#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Ref counter base
#include <boost/test/unit_test.hpp>

#include "../inc/core/util/ref_count_base.hpp"

namespace fn = functional::core::util;

namespace {

bool exists = false;

struct derived : fn::ref_count_base<derived>
{
	derived() { exists = true; }
	~derived() { exists = false; }
};

}

BOOST_AUTO_TEST_CASE(ref_counting)
{
	derived * d = new derived();

	BOOST_CHECK(exists);

	intrusive_ptr_add_ref(d);
	intrusive_ptr_add_ref(d);

	intrusive_ptr_release(d);

	BOOST_CHECK(exists);

	intrusive_ptr_release(d);

	BOOST_CHECK(!exists);
}


