#include "../inc/core/util/lazy.hpp"

#include <vector>


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
        value += object.value();
    for ( auto const& object : objects )
        value += object.value();
    for ( auto const& object : objects )
        value += object.value();

    return value;
}


int main()
{
    int value = 0;

    std::vector<timed_object> objects(1000000);
    for (size_t i = 0; i < objects.size(); ++i)
        if (i % 2) value += objects[i].value();

    value += value_fun(objects);

    return value;
}
