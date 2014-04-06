#ifndef FUNCTIONAL_DATA_LIST_HPP
#define FUNCTIONAL_DATA_LIST_HPP

#include "../core/fundamentals.hpp"
#include "../core/curry.hpp"
#include "../core/util/ref_count_base.hpp"

#include <boost/intrusive_ptr.hpp>

namespace functional { namespace data {

namespace list_detail_ {

template < typename T >
struct list_impl : core::util::ref_count_base<list_impl<T>>
{
	list_impl(T t) : value(t), len(1) {}
	list_impl(T t, boost::intrusive_ptr<list_impl const> n)
		: value(t)
		, len(n ? n->len + 1:1)
		, next(n)
	{}

	T value;
	int len;
	boost::intrusive_ptr<list_impl const> next;
};

}

template < typename T >
struct list
{
	list() : pimpl() {}
	list(T t) : pimpl(new list_detail_::list_impl<T>(t)) {}
	list(T t, list next)
		: pimpl(new list_detail_::list_impl<T>(t, next.pimpl))
	{}

	T head() const
	{
		if (!pimpl)
			throw std::domain_error("Can't call head on empty.");

		return pimpl->value;
	}
	list tail() const
	{
		if (!pimpl)
			throw std::domain_error("Can't call tail on empty.");

		return list(pimpl->next);
	}

	core::int_ length() const
	{
		if (!pimpl) return 0;
		return pimpl->len;
	}

	list value() const
	{
		return *this;
	}
	list operator() () const
	{
		return *this;
	}

	static list<T> const empty;

private:
	boost::intrusive_ptr<list_detail_::list_impl<T> const> pimpl;

	list(boost::intrusive_ptr<list_detail_::list_impl<T> const>  impl) : pimpl(impl) {}
};
template < typename T >
list<T> const list<T>::empty;

template < typename T >
bool operator == (list<T> const lh, list<T> const rh)
{
	if (lh.length() != rh.length()) return false;
	if (lh.length() == 0) return true;

	if (lh.head() != rh.head()) return false;

	return lh.tail() == rh.tail();
}
template < typename T >
bool operator != (list<T> const lh, list<T> const rh)
{
	return !(lh == rh);
}

}}

#endif
