#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE type-indexed
#include <boost/test/unit_test.hpp>

#include "../include/indexed_storage.hpp"


BOOST_AUTO_TEST_CASE(simple)
{
    constexpr auto i0 = make_indexed_storage(make_pair<19>(42), make_pair<33>(66.6));

    static_assert(get<19>(i0) == 42);
    static_assert(static_cast<int>(get<33>(i0) * 10) == 666);

    static_assert(has_index<33>(i0));
    static_assert(!has_index<0>(i0));
}
