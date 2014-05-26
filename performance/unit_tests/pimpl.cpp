#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE pimpl
#include <boost/test/unit_test.hpp>

#include "../include/crazy/util/pimpl.hpp"

int impl_count = 0;

template < typename TestPolicy >
struct test_class
{
	int fun() const { return pimpl->i; }
	void fun(int i) { pimpl->i = i; }

	int ref_count() const { return pimpl->ref_count(); }

	test_class() : pimpl() {}
	test_class(int i) : pimpl(i) {}

private:
	struct impl
	{
		impl() : i(), ref_count_()
		{
			++impl_count;
		}
		impl(int i_) : i(i_), ref_count_() { ++impl_count; }

		impl(impl const& other) : i(other.i), ref_count_() { ++impl_count; }

		~impl() { --impl_count; }

		int inc() const { return ++ref_count_; }
		int dec() const { return --ref_count_; }

		int ref_count() const { return ref_count_; }

		void destroy()
		{
			if (--ref_count_ == 0)
				delete this;
		}

		bool operator < (impl const& other) const
		{
			return i < other.i;
		}

		int i;
		mutable int ref_count_;
	};
	crazy::util::pimpl_ptr<impl, TestPolicy> pimpl;
};


BOOST_AUTO_TEST_CASE(basic)
{
	test_class<crazy::util::pimpl::basic> t0(15);
	test_class<crazy::util::pimpl::basic> t1 = t0;

	BOOST_CHECK_EQUAL(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 0);
	BOOST_CHECK_EQUAL(t1.ref_count(), 0);

	t1.fun(42);

	BOOST_CHECK_NE(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 0);
	BOOST_CHECK_EQUAL(t1.ref_count(), 0);

	t1.fun(t0.fun());

	BOOST_CHECK_EQUAL(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 0);
	BOOST_CHECK_EQUAL(t1.ref_count(), 0);
}


BOOST_AUTO_TEST_CASE(cow)
{
	test_class<crazy::util::pimpl::cow> t0(15);
	test_class<crazy::util::pimpl::cow> t1 = t0;

	BOOST_CHECK_EQUAL(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 2);
	BOOST_CHECK_EQUAL(t1.ref_count(), 2);

	BOOST_CHECK_EQUAL(impl_count, 1);

	t1.fun(42);

	BOOST_CHECK_NE(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 1);
	BOOST_CHECK_EQUAL(t1.ref_count(), 1);

	BOOST_CHECK_EQUAL(impl_count, 2);

	t1.fun(t0.fun());

	BOOST_CHECK_EQUAL(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 1);
	BOOST_CHECK_EQUAL(t1.ref_count(), 1);

	BOOST_CHECK_EQUAL(impl_count, 2);
}

BOOST_AUTO_TEST_CASE(fly)
{
	test_class<crazy::util::pimpl::fly> t0(15);
	test_class<crazy::util::pimpl::fly> t1 = t0;

	BOOST_CHECK_EQUAL(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 2);
	BOOST_CHECK_EQUAL(t1.ref_count(), 2);

	BOOST_CHECK_EQUAL(impl_count, 1);

	t1.fun(42);

	BOOST_CHECK_NE(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 1);
	BOOST_CHECK_EQUAL(t1.ref_count(), 1);

	BOOST_CHECK_EQUAL(impl_count, 2);

	t1.fun(t0.fun());

	BOOST_CHECK_EQUAL(t0.fun(), t1.fun());

	BOOST_CHECK_EQUAL(t0.ref_count(), 2);
	BOOST_CHECK_EQUAL(t1.ref_count(), 2);

	BOOST_CHECK_EQUAL(impl_count, 1);
}
