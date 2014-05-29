#ifndef TEST_PIMPL_HPP
#define TEST_PIMPL_HPP

#include "../include/crazy/util/pimpl.hpp"

struct test_class
{
	CRAZY_DECLARE_DEFAULTS(test_class);

	test_class();
	test_class(int);

	int value() const;

private:
	struct impl;
	crazy::util::pimpl_ptr<impl> pimpl;

	friend struct test_vector;
};

struct test_vector
{
	struct ref
	{
		ref(test_class::impl const*);

		int value() const;
	private:
		test_class::impl const* pimpl;
	};
	bool empty() const;
	size_t size() const;

	ref operator [] (size_t) const;

	void push_back(test_class const&);

	test_vector();

	CRAZY_DECLARE_DEFAULTS(test_vector);

private:
	struct impl;
	crazy::util::pimpl_ptr<impl> pimpl;
};


#endif
