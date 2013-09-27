#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE virtual

#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include "config.hpp"
#include "../inc/virt.hpp"



BOOST_AUTO_TEST_CASE(virt)
{
    std::vector< std::shared_ptr<base_virt> > data
        = generate_data_virt(array_size);

    boost::timer::auto_cpu_timer timer;
    BOOST_CHECK_EQUAL(process_data(data, loop_num), expected);
}
