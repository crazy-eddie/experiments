#ifndef MISSING_HPP
#define MISSING_HPP

// My compiler doesn't have these definitions.

template < std::size_t ... I >
struct index_sequence {};


template < typename T, std::size_t N > struct append;
template < std::size_t ... I, std::size_t N >
struct append<index_sequence<I...>, N>
{
    using type = index_sequence<I..., N>;
};

template < std::size_t N >
struct sequence_builder
{
    using type = typename append<typename sequence_builder<N-1>::type,N-1>::type;
};

template < >
struct sequence_builder<0U>
{
    using type = index_sequence<>;
};

template < std::size_t N >
using make_index_sequence = typename sequence_builder<N>::type;

#endif
