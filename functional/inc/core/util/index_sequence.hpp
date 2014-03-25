#ifndef FUNCTIONAL_CORE_UTIL_INDEX_SEQUENCE_HPP
#define FUNCTIONAL_CORE_UTIL_INDEX_SEQUENCE_HPP

namespace functional { namespace core { namespace util {

template < std::size_t ... I >
struct index_sequence
{
    using type = index_sequence;
};

template < typename Seq1
         , typename Seq2 >
struct append_indexes;

template < std::size_t ... I1
         , std::size_t ... I2 >
struct append_indexes< index_sequence<I1...>
                     , index_sequence<I2...> >
    : index_sequence<I1..., (I2 + sizeof...(I1))...>
{};

template < std::size_t N >
struct make_index_sequence
    : append_indexes< typename make_index_sequence<N/2>::type
                    , typename make_index_sequence<N - N/2>::type >
{};

template < >
struct make_index_sequence<0> : index_sequence<> {};

template < >
struct make_index_sequence<1> : index_sequence<0> {};

}}}

#endif
