#ifndef CONST_ARRAY_HPP
#define CONST_ARRAY_HPP

template < typename T, int Size >
struct const_arr
{
    constexpr const_arr() : arr{} {}

    template < typename ... Pack >
    constexpr const_arr(Pack ... pack) : arr{pack...} {}

    constexpr int size() const { return Size; }
    constexpr bool empty() const { return false; }

    constexpr T operator [] (int i) const
    {
        return arr[i];
    }

    template < int I >
    constexpr T at() const
    {
        return arr[I];
    }

    template < int I >
    constexpr const_arr<T,Size> set(T val) const
    {
        auto result = const_arr<T,Size>{};

        for (int i = 0; i < Size; ++i)
        {
            result.arr[i] = i == I ? val : arr[i];
        }

        return result;
    }

private:
    T arr[Size];
};

template < typename T >
struct const_arr<T,0>
{
    constexpr const_arr() {}

    constexpr int size() const { return 0; }
    constexpr bool empty() const { return true; }
};

template < typename T >
constexpr auto make_array()
{
    return const_arr<T,0>{};
}

template < typename T, typename ... Pack >
constexpr auto make_array(Pack ... pack)
{
    return const_arr<T,sizeof...(Pack)>{pack...};
}


#endif
