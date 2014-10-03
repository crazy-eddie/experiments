#ifndef JSON_ARRAY_HPP
#define JSON_ARRAY_HPP

#include "fundamentals.hpp"

namespace json {

template < typename Reader >
struct array_type
{
    using value_type = std::vector<typename Reader::value_type>;
    using parser = array_type;

    template < typename It >
    static std::pair<value_type, It> parse(value_type result, It beg, It end)
    {
        beg = detail_::pass_white(beg, end);

        if (beg == end)
            throw std::runtime_error("Unexpected end of stream in array.");

        if (*beg == ']')
            return std::make_pair(result, std::next(beg));

        if (!result.empty())
        {
            if (*beg != ',')
                throw std::runtime_error("Invalid character in array sequence.");
            beg = detail_::pass_white(std::next(beg), end);
        }

        auto next_pair = Reader::parse(boost::make_iterator_range(beg, end));

        result.push_back(next_pair.first);
        return parse(std::move(result), next_pair.second, end);
    }
    template < typename Range >
    static std::pair<value_type, typename Range::iterator> parse(Range range)
    {
        auto beg = detail_::pass_white(range);
        auto end = std::end(range);

        if (beg == end || *beg != '[')
            throw std::runtime_error("Steam doesn't start with '['.");

        return parse(value_type(), std::next(beg), end);
    }
};

template < typename Reader >
auto array(Reader)
{
    return reader<nullable<array_type<Reader>>>();
}

}


#endif
