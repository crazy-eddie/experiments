#include "test_pimpl.hpp"

#include <vector>

int impl_count_ = 0;

struct test_class::impl
{
	int value;

	impl(int i) : value(i), ref_count_() { ++ impl_count_; }
	impl() : value(), ref_count_() { ++ impl_count_; }
	impl(impl const& other) : value(other.value), ref_count_() { ++impl_count_; }
	~impl() { --impl_count_; }

	int inc() const { return ++ref_count_; }
	int dec() const { return --ref_count_; }

	int ref_count() const { return ref_count_; }

	void destroy()
	{
		if (--ref_count_ == 0)
			delete this;
	}

	mutable int ref_count_;
};

CRAZY_DEFINE_DEFAULTS(test_class);

test_class::test_class()
	: pimpl()
{}

test_class::test_class(ref<test_class> && ref)
	: pimpl(ref.pimpl.clone())
{}

test_class::test_class(test_class::impl * impl)
	: pimpl(impl)
{
}

test_class::test_class(int i)
	: pimpl(i)
{}

int test_class::impl_count() { return ::impl_count_; }

int test_class::value() const { return pimpl->value; }

#if 0
int test_vector::ref::value() const { return pimpl->value; }
test_vector::ref::ref(test_class::impl const* ptr) : pimpl(ptr) {}
#endif

struct test_vector::impl
{
	std::vector<test_class::impl> vect;
};

CRAZY_DEFINE_DEFAULTS(test_vector);

test_vector::test_vector()
	: pimpl()
{}

test_class::impl const* test_vector::begin_elem() const
{
	return &*pimpl->vect.begin();
}

test_class::impl const* test_vector::end_elem() const
{
	return &*pimpl->vect.end();
}


size_t test_vector::size() const { return pimpl->vect.size(); }

test_vector::ref const test_vector::operator [] (size_t i) const
{
	return ref(const_cast<test_class::impl*>(&pimpl->vect[i]));
}

void test_vector::push_back(test_class const& in)
{
	pimpl->vect.push_back(*in.pimpl.get());
	pimpl->vect.back().inc();
}
