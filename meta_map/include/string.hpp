#ifndef STRING_HPP
#define STRING_HPP

#include <boost/mpl/string.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include <boost/mpl/front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/pop_front.hpp>

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
	using mpl_string = StrSeq;
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

struct str_equal { using type = str_equal; };
struct str_less { using type = str_less; };
struct str_greater { using type = str_greater; };

namespace detail_ {

template < typename Str1, typename Str2, typename Cmp >
struct stage2_cmp;

template < typename Str1, typename Str2 >
struct cmp_first
	: boost::mpl::eval_if
	  <
	    boost::mpl::less
	    <
	      typename boost::mpl::front<Str1>::type
	    , typename boost::mpl::front<Str2>::type
	    >
	  , str_less
	  , boost::mpl::if_
	    <
	      boost::mpl::less
	      <
	        typename boost::mpl::front<Str2>::type
	      , typename boost::mpl::front<Str1>::type
	      >
	    , str_greater
	    , str_equal
	    >
	  >
{};

template < typename Str1, typename Str2 >
struct compare_mpl_string : stage2_cmp<Str1,Str2, typename cmp_first<Str1,Str2>::type > {};

template < typename Str1, typename Str2 >
struct stage2_cmp<Str1,Str2,str_less> : str_less {};

template < typename Str1, typename Str2 >
struct stage2_cmp<Str1,Str2,str_greater> : str_greater {};

template < typename Str1, typename Str2 >
struct stage2_cmp<Str1,Str2,str_equal>
	: compare_mpl_string
	  <
	    typename boost::mpl::pop_front<Str1>::type
	  , typename boost::mpl::pop_front<Str2>::type
	  >
{};

template < typename Str1 >
struct compare_mpl_string<Str1, boost::mpl::string<>> : str_greater {};

template < typename Str2 >
struct compare_mpl_string<boost::mpl::string<>,Str2> : str_less {};

template < >
struct compare_mpl_string<boost::mpl::string<>, boost::mpl::string<>> : str_equal {};

}

template < typename Str1, typename Str2 >
struct string_compare : detail_::compare_mpl_string<typename Str1::mpl_string,typename Str2::mpl_string> {};

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
