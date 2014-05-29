#ifndef CRAZY_UTIL_PIMPL_HPP
#define CRAZY_UTIL_PIMPL_HPP

#include <memory>
#include <set>
#include <mutex>

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
				if (!other) return nullptr;
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
				if (!other) return nullptr;
				other->inc();
				return other;
			}

			static T * adopt(T * other)
			{
				return copy(other);
			}

			static void destroy(T * ptr)
			{
				if (ptr)
					ptr->destroy();
			}

			using mutate_ptr = T*;

			static mutate_ptr mutate(T *& ptr)
			{
				if (!ptr) return ptr;

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
	template < typename T >
	struct apply
	{
		struct type
		{
			template < typename ... Arg >
			static T * create(Arg && ... arg)
			{
				T temp(std::forward<T>(arg)...);
				return flyweight_set.make_ref(temp);
			}

			static T * copy(T * other)
			{
				if (!other) return other;
				other->inc();
				return other;
			}

			static void destroy(T * ptr)
			{
				if (ptr)
					flyweight_set.remove_ref(ptr);
			}

			struct mutate_ptr
			{
				mutate_ptr(T *& t)
					: ptr(&t)
					, temp(*t)
				{
				}

				~mutate_ptr()
				{
					T * tmp = *ptr;

					*ptr = flyweight_set.make_ref(temp);
					flyweight_set.remove_ref(tmp);
				}

				T * operator -> ()
				{
					return &temp;
				}

			private:
				T ** ptr;
				T temp;
			};

			static mutate_ptr mutate(T *& ptr)
			{
				return mutate_ptr(ptr);
			}
		};

		struct flyweight_set_type
		{
			T* make_ref(T const& t)
			{
				std::lock_guard<std::mutex> guard(mutex);

				auto it = set.insert(t).first;
				it->inc();
				return const_cast<T*>(&*it);
			}
			void remove_ref(T * ptr)
			{
				std::lock_guard<std::mutex> guard(mutex);

				auto it = set.find(*ptr);
				if (it == set.end()) throw std::runtime_error("WTF?");

				if (it->dec() == 0)
					set.erase(it);
			}

		private:
			std::mutex mutex;
			std::set<T> set;
		};
		static flyweight_set_type flyweight_set;
	};
};

template < typename T >
typename fly::apply<T>::flyweight_set_type fly::apply<T>::flyweight_set;

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

	pimpl_ptr(Impl * impl) : ptr(copy_policy::adopt(impl)) {}

	pimpl_ptr(pimpl_ptr const& other)
		: ptr(copy_policy::copy(other.ptr))
	{}
	pimpl_ptr(pimpl_ptr && other)
		: ptr()
	{
		std::swap(ptr, other.ptr);
	}

	pimpl_ptr& operator = (pimpl_ptr other)
	{
		std::swap(ptr, other.ptr);
		return *this;
	}

	~pimpl_ptr() { copy_policy::destroy(ptr); }

	typename copy_policy::mutate_ptr operator -> ()
	{
		return copy_policy::mutate(ptr);
	}

	Impl const* operator -> () const
	{
		return ptr;
	}

	void swap(pimpl_ptr & other) { std::swap(ptr, other.ptr); }

	Impl const* get() const { return ptr; }

	Impl * clone() const { return new Impl(*ptr); }

private:
	Impl * ptr;
};

}}

#define CRAZY_DECLARE_DEFAULTS(classname) \
	classname(classname const&); \
	classname(classname&&); \
	~classname(); \
	classname& operator = (classname)

#define CRAZY_DEFINE_DEFAULTS(classname) \
	classname::classname(classname const& other) : pimpl(other.pimpl) {} \
	classname::classname(classname && other) : pimpl(std::move(other.pimpl)) {} \
	classname::~classname() {} \
	classname& classname::operator = (classname other) { pimpl.swap(other.pimpl); return *this; }

#endif
