#ifndef FUNCTIONAL_CORE_FUNDAMENTALS_HPP
#define FUNCTIONAL_CORE_FUNDAMENTALS_HPP

#include "curry.hpp"

namespace functional { namespace core {


template < typename RawType >
struct raw_wrapper
{
	raw_wrapper(RawType v) : val(v) {}

	template < typename T >
	explicit raw_wrapper(raw_wrapper<T> const& other)
		: val(other.raw_value())
	{}

	RawType raw_value() const { return val; }

	operator RawType () const { return raw_value(); }

	raw_wrapper value() const { return *this; }
	raw_wrapper operator() () const { return value(); }

private:
	RawType val;
};

template < typename T >
bool eq(raw_wrapper<T> const& lh, raw_wrapper<T> const& rh)
{
	return lh.raw_value() == rh.raw_value();
}
// variadic eq?

template < typename T >
bool operator == (raw_wrapper<T> const& lh, raw_wrapper<T> const& rh)
{
	return lh.raw_value() == rh.raw_value();
}

#define RAW_VALUE_OPERATOR(sym, op) \
	struct sym ## __{ \
		template < typename T0, typename T1 > \
		auto operator() (raw_wrapper<T0> const& a0, raw_wrapper<T1> const& a1) const \
		{ \
			using rtype = decltype(std::declval<T0>() op std::declval<T1>()); \
			return raw_wrapper<rtype>(a0.raw_value() op a1.raw_value()); \
		} \
		template < typename T0, typename T1, typename ... T > \
		auto operator() (raw_wrapper<T0> const& a0, raw_wrapper<T1> const& a1, raw_wrapper<T> const& ... a) const \
		{ \
			return (*this)((*this)(a0,a1), a...); \
		} \
	} sym ## _; \
	template < typename T0, typename T1 > \
	auto operator op (raw_wrapper<T0> const& a0, raw_wrapper<T1> const& a1) \
	{ \
		return sym ## _(a0,a1); \
	} \
	auto sym = curry(sym ## _)

RAW_VALUE_OPERATOR(sum, +);
RAW_VALUE_OPERATOR(div, /);
RAW_VALUE_OPERATOR(minus, -);
RAW_VALUE_OPERATOR(multiply, *);
RAW_VALUE_OPERATOR(land, &&);
RAW_VALUE_OPERATOR(band, &);
RAW_VALUE_OPERATOR(lor, ||);
RAW_VALUE_OPERATOR(bor, |);
RAW_VALUE_OPERATOR(bxor, ^);

template < typename T >
std::ostream& operator << (std::ostream & out, raw_wrapper<T> const& val)
{
	return out << val.raw_value();
}

using int_ = raw_wrapper<int>;
using double_ = raw_wrapper<double>;
using char_ = raw_wrapper<char>;

auto id = curry([](auto a) { return a; });

}}

#endif
