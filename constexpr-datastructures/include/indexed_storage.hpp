#ifndef INDEXED_STORAGE_HPP
#define INDEXED_STORAGE_HPP

#include "pair.hpp"

struct nil {};

template < int I, typename ValueType, typename Next = nil >
struct indexed_storage : Next
{
    constexpr indexed_storage(ValueType v, Next n)
        : Next(n)
        , value(v)
    {}

    constexpr ValueType get() const { return value; }

private:
    ValueType value;
};

template < typename Storage, int I >
struct cast_index;

template < int K, typename ValueType, typename Next, int I >
struct cast_index<indexed_storage<K,ValueType,Next>,I>
{
    using type = typename cast_index<Next, I>::type;
};

template < int K, typename ValueType, typename Next >
struct cast_index<indexed_storage<K,ValueType,Next>,K>
{
    using type = indexed_storage<K,ValueType,Next>;
};

template < int I, typename Storage >
struct has_index_;

template < int I, int K, typename ValueType, typename Next >
struct has_index_<I, indexed_storage<K,ValueType,Next>>
{
    constexpr static bool value = has_index_<I, Next>::value;
};

template < int I, typename ValueType, typename Next >
struct has_index_<I, indexed_storage<I,ValueType,Next>>
{
    constexpr static bool value = true;
};

template < int I >
struct has_index_<I, nil>
{
    constexpr static bool value = false;
};

template < int I, typename Storage >
constexpr bool has_index(Storage const& storage)
{
    return has_index_<I, Storage>::value;
}

template < int I, typename Storage >
constexpr auto get(Storage const& storage)
{
    // Interestingly, calling has_index<I>(storage) here does not work.
    // Unit test for has_index is within static_assert so not sure why.
    // Error claims not a constant integral expression.
    static_assert(has_index_<I,Storage>::value, "Storage doesn't contain specified index");
    return static_cast<typename cast_index<Storage,I>::type const*>(&storage)->get();
}

template < int I, typename ValueType >
constexpr auto make_indexed_storage(pair<I,ValueType> p)
{
    return indexed_storage<I,ValueType>{p.value, nil{}};
}

template < int I, typename ValueType, typename ... Pairs >
constexpr auto make_indexed_storage(pair<I,ValueType> head, Pairs ... tail)
{
    auto next = make_indexed_storage(tail...);
    return indexed_storage<I, ValueType, decltype(next)>{head.value, next};
}

#endif
