#ifndef INVOCATION_LIST_HPP
#define INVOCATION_LIST_HPP


template < typename Key, typename Value >
struct invocation_pair
{
    constexpr invocation_pair(Key k, Value v)
        : key{k}
        , value{v}
    {}

    Key key;
    Value value;
};

template < typename Key, typename Value >
constexpr auto make_invocation_pair(Key key, Value value)
{
    return invocation_pair<Key, Value>{key, value};
}

template < typename Sig >
struct invocation_nil;

template < typename R, typename ... Params >
struct invocation_nil<R(Params...)>
{
    constexpr invocation_nil() {}

    template < typename Key2 >
    constexpr R invoke(Key2, Params...) const
    {
        throw std::domain_error("Key doesn't exist in list.");
    }
};

template < typename Sig, typename Key, typename Value, typename Next = invocation_nil<Sig> >
struct invocation_list;

template < typename R, typename ... Params, typename Key, typename Value, typename Next >
struct invocation_list<R(Params...), Key, Value, Next> : Next
{
    constexpr invocation_list(Key k, Value v, Next n)
        : Next(n)
        , key(k)
        , value(v)
    {}

    template < typename Key2 >
    constexpr R invoke(Key2 k, Params ... params) const
    {
        if (k == key) return value(params...);
        return Next::invoke(k, params...);
    }

private:
    Key key;
    Value value;
};

template < typename Sig, typename Key, typename Value >
constexpr auto make_invocation_list(invocation_pair<Key,Value> head)
{
    return invocation_list<Sig, Key, Value>{head.key, head.value, invocation_nil<Sig>{}};
}


template < typename Sig, typename Key, typename Value, typename ... Tail >
constexpr auto make_invocation_list(invocation_pair<Key,Value> head, Tail ... tail)
{
    auto next = make_invocation_list<Sig>(tail...);
    return invocation_list<Sig, Key, Value, decltype(next)>{head.key, head.value, next};
}

template < typename List, typename Key, typename ... Params >
constexpr auto invoke(List const& list, Key const& key, Params ... params)
{
    return list.invoke(key, params...);
}

template < typename List, typename Key, typename ... Params >
constexpr auto void_invoke(void const* v, Key const& key, Params ... params)
{
    auto list = static_cast<List const*>(v);
    return list->invoke(key, params...);
}
#endif
