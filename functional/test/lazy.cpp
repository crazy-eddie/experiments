#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Lazy
#include <boost/test/unit_test.hpp>

#include "../inc/core/util/lazy.hpp"

#include <chrono>

struct not_default_constructable
{
    not_default_constructable(int i) : val(i) {}

    int val;
};

struct test_object
{
    mutable int call_count;

    test_object() : call_count() {}

    int calculate_value() const
    {
        ++call_count;
        return 5;
    }

    not_default_constructable calc2() const
    {
        ++call_count;
        return not_default_constructable{42};
    }
};


BOOST_AUTO_TEST_CASE(lazy_evaluator)
{
    using namespace functional::core::util;

    test_object t;
    lazy_value< int
              , test_object
              , &test_object::calculate_value > v{&t};

    BOOST_CHECK_EQUAL(v.value(), 5);
    BOOST_CHECK_EQUAL(v.value(), 5);
    BOOST_CHECK_EQUAL(t.call_count, 1);
}


BOOST_AUTO_TEST_CASE(lazy_evaluator_no_default)
{
    using namespace functional::core::util;

    test_object t;
    lazy_value< not_default_constructable
              , test_object
              , &test_object::calc2 > v{&t};

    BOOST_CHECK_EQUAL(v.value().val, 42);
    BOOST_CHECK_EQUAL(v.value().val, 42);
    BOOST_CHECK_EQUAL(t.call_count, 1);
}

struct timed_object
{
    int value() const { return val.value(); }

    timed_object() : val(this) {}

private:
    int calculate() const { return 666; }
    functional::core::util::lazy_value< int
                                      , timed_object
                                      , &timed_object::calculate > val;
};

int value_fun(std::vector<timed_object> const& objects)
{
    int value = 0;


    for ( auto const& object : objects )
        value = object.value();
    for ( auto const& object : objects )
        value = object.value();
    for ( auto const& object : objects )
        value = object.value();

    return value;
}

BOOST_AUTO_TEST_CASE(performance)
{
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> clock_type;
    clock_type begin;
    clock_type end;

    int value = 0;

    std::vector<timed_object> objects(10000000);
    for (size_t i = 0; i < objects.size(); ++i)
        if (i % 2) value = objects[i].value();

    begin = std::chrono::high_resolution_clock::now();
    value = value_fun(objects);
    end = std::chrono::high_resolution_clock::now();

    BOOST_CHECK_EQUAL(value, 666);

    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    BOOST_MESSAGE("time elapsed: " << ms << " ms");
}
