#ifndef CONSTEXPR_MAP
#define CONSTEXPR_MAP

#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include "constexpr-str.hpp"

template < typename ElemT, typename KeyT = void >
struct constmap
{
    constexpr constmap(ElemT elem)
        : value(elem)
    {}

    template < typename K >
    constexpr ElemT operator[] (K const&) const
    {
        return value;
    }

    template < typename K2 >
    constexpr auto insert(K2 k, ElemT elem) const
    {
        return inserter<K2>::apply(k, elem);
    }

    template < size_t SZ >
    constexpr auto insert(char const (&k)[SZ], ElemT elem) const
    {
        return insert(make_string(k), elem);
    }

private:
    ElemT value;

    enum comparison { less, equal, greater };

    template < int L, int R >
    struct compare_mpl
        : boost::mpl::if_c
          <
            (L < R)
          , boost::mpl::int_<less>
          , typename boost::mpl::if_c
            <
              (R > L)
            , boost::mpl::int_<greater>
            , boost::mpl::int_<equal>
            >::type
          >
    {};

    template < typename K, typename Comparison = typename compare_mpl<KeyT::sz, K::sz>::type>
    struct inserter;

    template < typename K >
    struct inserter<K, boost::mpl::int_<less>>
    {
    };
};

template < typename ElemT >
struct constmap<ElemT, void>
{

    template < typename KeyT2 >
    constexpr auto insert(KeyT2, ElemT elem) const
    {
        return constmap<ElemT, KeyT2>(elem);
    }

    template < size_t SZ >
    constexpr auto insert(char const (&key)[SZ], ElemT elem) const
    {
        return insert(string<SZ-1>(key), elem);
    }
};



#endif
