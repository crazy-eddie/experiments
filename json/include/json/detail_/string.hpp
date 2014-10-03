#ifndef JSON_DETAIL__STRING_HPP
#define JSON_DETAIL__STRING_HPP

#include <boost/mpl/string.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

namespace meta {

namespace detail_ {

template < typename Str, char Char, bool End >
struct push_back_if_c
    : boost::mpl::push_back<Str, boost::mpl::char_<Char>>
{};

template < typename Str, char Char >
struct push_back_if_c<Str,Char,true>
    : Str
{};

template < int N >
constexpr char char_at(char const(&str)[N], int i)
{
    return i >= N ? '\0':str[i];
}

}

template < typename StrSeq >
struct string
{
    constexpr string() {}
    static constexpr char const* value()
    {
        return boost::mpl::c_str<StrSeq>::value;
    }
    constexpr operator char const* () const
    {
        return value();
    }
    static constexpr size_t size()
    {
        return boost::mpl::size<StrSeq>::value;
    }
};

}

#define META_STRING_MAX_LEN BOOST_MPL_LIMIT_STRING_SIZE

#define META_STRING_PRE(z, n, s) meta::detail_::push_back_if_c<
#define META_STRING_POST(z, n, s) , meta::detail_::char_at(s,n), (n >= sizeof(s))>::type

#define META_STRING(s) \
    meta::string< \
    BOOST_PP_REPEAT(META_STRING_MAX_LEN, META_STRING_PRE, s) \
    boost::mpl::string<> \
    BOOST_PP_REPEAT(META_STRING_MAX_LEN, META_STRING_POST, s) \
    ::type >

#endif
