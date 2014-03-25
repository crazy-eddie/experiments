#ifndef LAZY_HPP
# define LAZY_HPP

#include <type_traits>
#include <boost/variant.hpp>
#include "delegate.hpp"

//#define SINGLE_DELEGATE 1
//#define VARIANT 1

namespace functional { namespace core { namespace util {

#ifdef SINGLE_DELEGATE
template < typename T
         , typename Object
         , T (Object::*calculate)() const >
struct lazy_value
{
    typedef delegate<T()> function_type;

    lazy_value(Object const* o)
        : fun( [=]()
                {
                    T v = (o->*calculate)();
                    fun = [=](){ return v; };
                    return v;
                } )
    {}

    T value() const
    {
        return fun();
    }

private:
    mutable function_type fun;
};
#elif defined(VARIANT)

template < typename T
         , typename Object
         , T (Object::*calculate)() const >
struct lazy_value
{
    typedef delegate<T()> function_type;

    lazy_value(Object const* o)
        : val(function_type::from(o,calculate))
    {}
    T value() const
    {
        if (val.which())
            val = boost::get<function_type>(val)();

        return boost::get<T>(val);
    }

private:
    typedef boost::variant<T, function_type> variant_t;
    mutable variant_t val;
};
#else
template < typename T
         , typename Object
         , T (Object::*calculate)() const >
struct lazy_value
{
    lazy_value(Object const* o)
        : object(o)
        , storage()
        , val(reinterpret_cast<T const*>(&storage))
        , calculated(false)
    {}
    T value() const
    {
        if (!calculated)
        {
            new (&storage) T((object->*calculate)());
            calculated = true;
        }
        return *val;
    }

private:
    Object const* object;
    typedef typename std::aligned_storage< sizeof(T)>::type storage_t;
    mutable storage_t storage;
    T const* val;
    mutable bool calculated;
};
#endif

}}}

#endif
