#ifndef TEST_PIMPL_HPP
#define TEST_PIMPL_HPP

#include "../include/crazy/util/pimpl.hpp"

template < typename T >
struct ref : T
{
	ref(typename T::impl * impl) : T(impl) {}
	ref(ref && r) : T(std::move(r)) {}

private:
	ref(ref const&);
	ref& operator = (ref);
};



template < typename Impl, typename T >
struct pimpl_vector
{
	using ref = ref<T>;

	bool empty() const
	{
		return this_()->begin_elem() == this_()->end_elem();
	}

protected:
	~pimpl_vector() {}

	Impl const* this_() const { return static_cast<Impl const*>(this); }
};


struct test_class
{
	CRAZY_DECLARE_DEFAULTS(test_class);

	test_class(ref<test_class> &&);

	test_class();
	test_class(int);

	int value() const;

	static int impl_count();

private:
	struct impl;
	crazy::util::pimpl_ptr<impl, crazy::util::pimpl::cow> pimpl;

	friend struct test_vector;
	template < typename > friend struct ref;

	test_class(impl *);
};

struct test_vector : pimpl_vector<test_vector, test_class>
{
	using base = pimpl_vector<test_vector, test_class>;

	test_class::impl const* begin_elem() const;
	test_class::impl const* end_elem() const;

	size_t size() const;
	base::ref const operator [] (size_t) const;

	void push_back(test_class const&);

	test_vector();

	CRAZY_DECLARE_DEFAULTS(test_vector);

private:
	struct impl;
	crazy::util::pimpl_ptr<impl> pimpl;
};


#endif
