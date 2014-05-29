#include "test_pimpl.hpp"

#include <vector>

struct test_class::impl
{
	int value;

	impl(int i) : value(i) {}
	impl() : value() {}
};

CRAZY_DEFINE_DEFAULTS(test_class);

test_class::test_class()
	: pimpl()
{}

test_class::test_class(int i)
	: pimpl(i)
{}

int test_class::value() const { return pimpl->value; }

int test_vector::ref::value() const { return pimpl->value; }
test_vector::ref::ref(test_class::impl const* ptr) : pimpl(ptr) {}

struct test_vector::impl
{
	std::vector<test_class::impl> vect;
};

CRAZY_DEFINE_DEFAULTS(test_vector);

test_vector::test_vector()
	: pimpl()
{}

bool test_vector::empty() const { return pimpl->vect.empty(); }
size_t test_vector::size() const { return pimpl->vect.size(); }

test_vector::ref test_vector::operator [] (size_t i) const
{
	return ref(&pimpl->vect[i]);
}

void test_vector::push_back(test_class const& in)
{
	pimpl->vect.push_back(*in.pimpl.get());
}
