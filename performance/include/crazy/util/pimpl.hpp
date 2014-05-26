#ifndef CRAZY_UTIL_PIMPL_HPP
#define CRAZY_UTIL_PIMPL_HPP

namespace crazy { namespace util {

namespace pimpl {

struct basic
{
	template < typename T >
	struct apply
	{
		struct type
		{
			template < typename ... Arg >
			static T * create(Arg && ... arg)
			{
				return new T(std::forward<Arg>(arg)...);
			}

			static T * copy(T * other)
			{
				return new T(*other);
			}

			static void destroy(T * ptr)
			{
				delete ptr;
			}

			using mutate_ptr = T*;

			static mutate_ptr mutate(T * ptr)
			{
				return ptr;
			}
		};
	};
};

struct cow
{
	template < typename T >
	struct apply
	{
		struct type
		{
			template < typename ... Arg >
			static T * create(Arg && ... arg)
			{
				std::unique_ptr<T> ptr(new T(std::forward<Arg>(arg)...));
				ptr->inc();
				return ptr.release();
			}

			static T * copy(T * other)
			{
				other->inc();
				return other;
			}

			static void destroy(T * ptr)
			{
				ptr->destroy();
			}

			using mutate_ptr = T*;

			static mutate_ptr mutate(T *& ptr)
			{
				if (ptr->ref_count() == 1) return ptr;

				std::unique_ptr<T> ptr2(create(*ptr));
				ptr->destroy();
				return ptr = ptr2.release();
			}
		};
	};
};

struct fly
{
};

}

template < typename Impl
         , typename CopyPolicy = pimpl::basic >
struct pimpl_ptr
{
	using copy_policy = typename CopyPolicy::template apply<Impl>::type;

	template < typename ... Arg >
	pimpl_ptr(Arg && ... arg)
		: ptr(copy_policy::create(std::forward<Arg>(arg)...))
	{}

	pimpl_ptr(pimpl_ptr const& other)
		: ptr(copy_policy::copy(other.ptr))
	{}

	~pimpl_ptr() { copy_policy::destroy(ptr); }

	typename copy_policy::mutate_ptr operator -> ()
	{
		return copy_policy::mutate(ptr);
	}

	Impl const* operator -> () const
	{
		return ptr;
	}

private:
	Impl * ptr;
};

}}

#endif
