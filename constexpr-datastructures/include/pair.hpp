#ifndef PAIR_HPP
#define PAIR_HPP

template < int I, typename Value >
struct pair
{
    constexpr pair(Value v) : value{v} {}

    Value value;
};

template <int I, typename Value >
constexpr auto make_pair(Value value) { return pair<I,Value>{value}; }

#endif
