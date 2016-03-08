#ifndef ARRAY
#define ARRAY

#include <boost/mpl/int.hpp>

namespace functional {

template<typename T, int Size>
struct array
{
    constexpr int size() const
    {
        return Size;
    }
    constexpr T const* data() const
    {
        return arr;
    }

    template<typename ... U>
    constexpr array(U ... t)
        : arr { t... }
    {}

private:
    T arr[Size];
};

template<typename T0, typename ... T>
struct pack_type
{
    typedef T0 type;
};

template<typename ... T>
struct pack_count;

template<typename T0, typename ... T>
struct pack_count<T0, T...>
    : boost::mpl::int_<pack_count<T...>::value + 1>
{};

template<>
struct pack_count<> : boost::mpl::int_<0> {};

template<typename ... T>
struct array_type
{
    typedef array< typename pack_type<T...>::type
                 , pack_count<T...>::value> type;
};

template<typename ... T>
constexpr typename array_type<T...>::type make_array(T ... t)
{
    typedef typename array_type<T...>::type arr_t;
    return arr_t(t...);
}

template<int ... I>
struct indices {};

template<int I, int ... Is>
struct build_indices
    : build_indices<I - 1, I - 1, Is...>
{};

template<int ... I>
struct build_indices<0, I...>
{
    typedef indices<I...> type;
};

template<typename T, int I, int ... Is>
constexpr
array<T, I + 1> push_back( array<T, I> arr
                         , T t
                         , indices<Is...>)
{
    return array<T, I + 1>(arr.data()[Is]..., t);
}

template<typename T, int I>
constexpr
array<T, I + 1> push_back(array<T, I> arr, T t)
{
    typedef typename build_indices<I>::type indices;
    return push_back(arr, t, indices());
}

template < typename T, int I, int ... Is >
constexpr array<T, I+1> push_front(array<T,I> arr, T t, indices<Is...>)
{
    return array<T,I+1>(t, arr.data()[Is]...);
}
template < typename T, int I >
constexpr array<T,I+1> push_front(array<T,I> arr, T t)
{
    using indices = typename build_indices<I>::type;
    return push_front(arr,t,indices());
}

template < typename T
         , int I1, int I2
         , int ... Is1
         , int ... Is2 >
constexpr
array<T, I1 + I2> append( array<T,I1> a1, indices<Is1...>
                        , array<T,I2> a2, indices<Is2...> )
{
    return array<T,I1+I2>( a1.data()[Is1]...
                         , a2.data()[Is2]... );
}

template < typename T
         , int I1
         , int I2 >
constexpr
array<T, I1 + I2> operator + ( array<T,I1> a1
                             , array<T,I2> a2 )
{
    typedef typename build_indices<I1>::type i1;
    typedef typename build_indices<I2>::type i2;

    return append(a1, i1(), a2, i2());
}

template < typename T, int I >
constexpr array<T,I+1> operator + (array<T,I> a, T t) { return push_back(a,t); }

template < typename T, int I >
constexpr array<T,I+1> operator + (T t, array<T,I> a) { return push_front(a,t); }

}

#endif
