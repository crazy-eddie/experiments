#ifndef JSON_FUNDAMENTALS_HPP
#define JSON_FUNDAMENTALS_HPP

#include <boost/optional.hpp>

#include <boost/range/iterator_range.hpp>

namespace json {


namespace detail_ {

template < typename It >
static It pass_white(It beg, It end)
{
    while (beg != end && std::isspace(*beg)) ++beg;
    return beg;
}

template < typename Range >
static auto pass_white(Range range)
{
    return pass_white(std::begin(range), std::end(range));
}

}


struct number_type
{
    using value_type = double;
    using parser = number_type;

    template < typename It >
    static std::pair<int, It> sign(It it)
    {
        if (*it == '-') return std::make_pair(-1, std::next(it));
        if (*it == '+') return std::make_pair(1, std::next(it));
        return std::make_pair(1, it);
    }

    template < typename It >
    static std::pair<int,It> parse_int(It beg, It end)
    {
        int val = 0;
        for (; beg != end && std::isdigit(*beg); ++beg)
        {
            val = val * 10 + (*beg - '0');
        }

        return std::make_pair(val, beg);
    }

    template < typename It >
    static std::pair<double, It> parse_fract(It beg, It end)
    {
        int num = 0;
        It it;

        std::tie(num, it) = parse_int(beg,end);

        double result = static_cast<double>(num) / std::pow(10, std::distance(beg,it));
        return std::make_pair(result, it);
    }

    template < typename Range >
    static std::pair<double, typename Range::iterator> parse(Range range)
    {
        double result = 0.;
        auto it = detail_::pass_white(range);
        int mult = 0;
        std::tie(mult, it) = sign(it);

        int num = 0;
        std::tie(num, it) = parse_int(it, std::end(range));

        result = num*mult;

        if (it != std::end(range) && *it == '.')
        {
            ++it;
            double fract = 0.;
            std::tie(fract, it) = parse_fract(it, std::end(range));
            result += fract;
        }

        if (it != std::end(range) && (*it == 'e' || *it == 'E'))
        {
            ++it;
            if (it == std::end(range))
                throw std::runtime_error("Invalid numeric string.");
            std::tie(mult, it) = sign(it);
            std::tie(num, it) = parse_int(it, std::end(range));

            result *= std::pow(10., num*mult);
        }

        if (it == detail_::pass_white(range)) // haven't advanced.
            throw std::runtime_error("Invalid numberic string.");

        return std::make_pair(result, it);
    }
};


struct boolean_type
{
    using value_type = bool;
    using parser = boolean_type;

    template < typename Range >
    static std::pair<bool,typename Range::iterator> parse(Range range)
    {
        static std::string const& true_ = "true";
        static std::string const& false_ = "false";

        auto it = detail_::pass_white(range);

        auto len = std::distance(it, std::end(range));
        if (len >= 4)
        {
            bool b = std::equal(std::begin(true_), std::end(true_), it);
            if (b) return std::make_pair(true, std::next(std::begin(range), 4));

            b = std::equal(std::begin(false_), std::end(false_), it);
            if (b) return std::make_pair(false, std::next(std::begin(range), 5));
        }

        throw std::runtime_error("Invalid boolean string");
    }
};


struct string_type
{
    using value_type = std::string;
    using parser = string_type;


    template < typename Range >
    static std::pair<value_type, typename Range::iterator> parse(Range range)
    {
        std::string result;
        auto it = detail_::pass_white(range);

        if (*it++ != '\"')
            throw std::runtime_error("String doesn't start with \"");

        while (it != std::end(range))
        {
            if (*it == '\"')
                return std::make_pair(result, std::next(it));

            result.push_back(*it++);
        }

        throw std::runtime_error("Unexpected end of stream in string.");
    }
};




template < typename Parser >
struct nullable : Parser
{
    using value_type = boost::optional<typename Parser::value_type>;
    using parser = Parser;

    template < typename Range >
    using parse_result_type = std::pair<value_type, typename Range::iterator>;

    template < typename Range >
    static parse_result_type<Range> parse(Range range)
    {
        auto it = detail_::pass_white(range);
        bool is_null = false;

        std::tie(is_null, it) = check_null(it, std::end(range));

        if (is_null) return std::make_pair(value_type(), it);

        return Parser::parse(boost::make_iterator_range(it, std::end(range)));
    }


    template < typename It >
    static auto check_null(It beg, It end)
    {
        static std::string const null = "null";
        It bbeg = beg;

        for (auto nbeg = std::begin(null); nbeg != std::end(null) && beg != end; ++nbeg, ++beg)
        {
            if (*nbeg != *beg) return std::make_pair(false, bbeg);
        }

        return std::make_pair(true, beg);
    }
};

template < typename Parser >
struct reader : Parser
{
    using value_type = typename Parser::value_type;

    template < typename Range >
    static auto end_asserter(Range const& r)
    {
        return [&r](auto parse_pair)
        {
            auto it = parse_pair.second;
            while (it != std::end(r) && std::isspace(*it)) ++it;
            if (it != std::end(r))
                throw std::runtime_error("Range not empty after parse.");

            return parse_pair.first;
        };
    }

    template < typename Range >
    static value_type read(Range range)
    {
        return end_asserter(range)(Parser::parse(range));
    }

    static reader<typename Parser::parser> nonull()
    {
        using true_parser = reader<typename Parser::parser>;
        return true_parser();
    }
};



namespace {

reader<nullable<number_type>> number;
reader<nullable<boolean_type>> boolean;
reader<nullable<string_type>> string;

}

}



#endif
