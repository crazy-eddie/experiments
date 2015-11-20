#ifndef CONSTEXPR_MAP
#define CONSTEXPR_MAP

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
    constexpr auto insert(ElemT elem) const
    {
    }

private:
    ElemT value;
};


template < typename ElemT >
struct constmap<ElemT, void>
{

    template < typename KeyT2 >
    constexpr auto insert(KeyT2, ElemT elem) const
    {
        return constmap<ElemT, KeyT2>(elem);
    }
};



#endif
