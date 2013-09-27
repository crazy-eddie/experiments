#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Non-virtual
#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include "config.hpp"
#include "../inc/novirt.hpp"

BOOST_AUTO_TEST_CASE(novirt)
{
    std::vector< std::shared_ptr<base_novirt> > data
        = generate_data_novirt(array_size);

    boost::timer::auto_cpu_timer timer;
    BOOST_CHECK_EQUAL(process_data(data, loop_num), expected);
}
