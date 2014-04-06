#ifndef FUNCTIONAL_CORE_UTIL_REF_COUNT_BASE_HPP
#define FUNCTIONAL_CORE_UTIL_REF_COUNT_BASE_HPP

#include <atomic>

namespace functional { namespace core { namespace util {

template < typename D > struct ref_count_base;
template < typename D > void intrusive_ptr_add_ref(ref_count_base< D > const* p) noexcept;
template < typename D > void intrusive_ptr_release(ref_count_base< D > const* p) noexcept;

template < typename Derived >
struct ref_count_base
{
	ref_count_base() : ref_count() {}
private:
	mutable std::atomic_ulong ref_count;

protected:
	~ref_count_base() {}


    friend void intrusive_ptr_add_ref< Derived >(ref_count_base< Derived > const* p) noexcept;
    friend void intrusive_ptr_release< Derived >(ref_count_base< Derived > const* p) noexcept;
};


template< typename Derived >
inline void intrusive_ptr_add_ref(ref_count_base< Derived > const* p) noexcept
{
	++(p->ref_count);
}
template< typename Derived >
inline void intrusive_ptr_release(ref_count_base< Derived > const* p) noexcept
{
    if (--(p->ref_count) == 0)
        delete static_cast< Derived const* >(p);
}


}}}

#endif
