#ifndef STRING_H
#define STRING_H

namespace functional
{

namespace detail_ {

constexpr int strlen(char const* str, int len)
{
	return *str == 0
	    ? len
		: strlen(str + 1, len + 1)
	;
}

constexpr bool string_eq( char const* str0
		                , char const* str1
		                , int len )
{
	return len == 0
		 ? true
		 : ( *str0 == *str1
		   ? string_eq(str0 + 1, str1 + 1, len - 1)
		   : false )
	;
}

constexpr bool string_eq( char const* str0, int len0
		                , char const* str1, int len1 )
{
	return len0 == len1
		 ? string_eq(str0, str1, len0)
		 : false
	;
}

constexpr int string_cmp( char const* str0
                        , char const* str1
                        , int len )
{
	return len == 0
		 ? 0
		 : ( *str0 == *str1
		   ? string_cmp(str0 + 1, str1 + 1, len - 1)
		   : *str0 - *str1 )
    ;
}

constexpr int string_cmp( char const* str0, int len0
                        , char const* str1, int len1 )
{
	return len0 == len1
	     ? string_cmp(str0, str1, len0)
	     : len0 - len1
	;
}

}

struct string
{
	constexpr string(char const* str)
		: buf(str)
		, len(detail_::strlen(str, 0))
	{}

	constexpr string(string const& str)
	    : buf(str.buf)
	    , len(str.len)
	{}

	constexpr int length() const
	{
		return len;
	}

	constexpr char const* data() const
	{
		return buf;
	}

private:
	char const* buf;
	int len;
};

constexpr bool operator == (string s0, string s1)
{
	return detail_::string_eq( s0.data(), s0.length()
				             , s1.data(), s1.length() );
}

constexpr bool operator != (string s0, string s1)
{
	return !(s0 == s1);
}

constexpr bool operator < (string s0, string s1)
{
	return detail_::string_cmp( s0.data(), s0.length()
			                  , s1.data(), s1.length() ) < 0;
}

constexpr bool operator > (string s0, string s1)
{
	return detail_::string_cmp( s0.data(), s0.length()
			                  , s1.data(), s1.length() ) > 0;
}

constexpr string operator + (string s0, string s1)
{
	return string("");
}

}

#endif
