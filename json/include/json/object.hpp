#ifndef JSON_OBJECT_HPP
#define JSON_OBJECT_HPP


#include "detail_/string.hpp"

#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/for_each.hpp>

#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/at_key.hpp>

#include <map>

#define JSON_KEY(STR) META_STRING(STR)()

namespace json {

namespace detail_ {

template < typename ObjectValue
         , typename FieldKey
         , typename Reader
         , typename It >
struct field_reader
{
    static It read_field( ObjectValue & val
                        , It beg, It end )
    {
        auto inner = Reader::parse(boost::make_iterator_range(beg,end));
        val.set(FieldKey(), inner.first);
        return inner.second;
    }

    It operator () (ObjectValue & val, It beg, It end) const
    {
        return read_field(val,beg,end);
    }
};

}

template < typename ... Fields >
struct object_value
{
    template < typename FieldKey >
    auto get(FieldKey) const
    {
        return boost::fusion::at_key<FieldKey>(fields);
    }

    template < typename FieldKey, typename Value >
    void set(FieldKey, Value val)
    {
        boost::fusion::at_key<FieldKey>(fields) = val;
    }

    object_value() : fields() {}

private:
    template < typename Field >
    struct field_value
    {
        using type = boost::fusion::pair
                     <
                       typename Field::first
                     , typename std::remove_reference<decltype(Field::second::read(""))>::type
                     >;
    };

    using fields_type = boost::fusion::map<typename field_value<Fields>::type...>;
    fields_type fields;
};

template < typename Schema, typename ... Fields >
struct object_parser
{
    using value_type = object_value<Fields...>;
    using field_map = boost::mpl::map<Fields...>;
    using parser = object_parser;

    template < typename It >
    using expected_type = std::pair<value_type, It>;

    template < typename It >
    using reader_type = std::function<It(value_type&,It,It)>;

    static constexpr char const* schema()
    {
        return Schema().value();
    }

    template < typename FieldKey >
    static constexpr auto field_reader(FieldKey)
    {
        using reader = typename boost::mpl::at<field_map, FieldKey>::type;
        return reader();
    }

    template < typename ReaderMap, typename It >
    static expected_type<It> parse_field( ReaderMap const& reader_map
                                        , value_type val
                                        , It beg, It end)
    {
        beg = detail_::pass_white(beg, end);

        if (beg == end)
            throw std::runtime_error("Invalid object.");

        if (*beg == ',')
        {
            beg = detail_::pass_white(std::next(beg), end);
        }
        else if (*beg == '}')
            return std::make_pair(val, std::next(beg));

        auto key = string.nonull().parse(boost::make_iterator_range(beg,end));

        beg = key.second;
        auto fit = reader_map.find(key.first);
        if (fit == std::end(reader_map))
            throw std::runtime_error("Invalid key in object.");

        beg = detail_::pass_white(beg,end);
        if (*beg != ':')
            throw std::runtime_error("Invalid object.");
        beg = detail_::pass_white(std::next(beg), end);

        auto val_parse = fit->second(val, beg, end);

        return parse_field(reader_map, val, val_parse, end);
    }

    template < typename Range >
    static auto build_readers()
    {
        std::map<std::string, reader_type<typename Range::iterator>> readers;
        boost::mpl::for_each<field_map>(
                [&readers](auto v)
                {
                    using field = decltype(v);
                    readers[field::first::value()] = detail_::field_reader< value_type
                                                                 , typename field::first
                                                                 , typename field::second
                                                                 , typename Range::iterator >();
                });
        return readers;
    }

    template < typename Range >
    static expected_type<typename Range::iterator> parse(Range range)
    {
        auto reader_map = build_readers<Range>();

        auto it = detail_::pass_white(range);
        if (it == std::end(range) || *it++ != '{')
            throw std::runtime_error("Invalid object in stream.");

        return parse_field(reader_map, value_type(), it, std::end(range));
    }
};

namespace detail_ {

template < typename Field >
struct meta_field
{
    using type = boost::mpl::pair< typename Field::first_type
                                 , typename Field::second_type::reader >;
};

}

template < typename Schema, typename ... Fields >
struct object_type
    : reader<nullable<object_parser<Schema, typename detail_::meta_field<Fields>::type...>>>
{
    template < typename FieldKey >
    std::string docstring(FieldKey) const
    {
        return boost::fusion::at_key<FieldKey>(field_map);
    }

    object_type(Fields ... fields)
        : field_map(boost::fusion::make_pair<typename Fields::first_type>(fields.second.docstr)...)
    {}

private:
    boost::fusion::map<boost::fusion::pair< typename Fields::first_type
                                          , std::string >...> field_map;
};

template < typename Schema, typename ... Fields >
auto object(Schema, Fields ... fields)
{
    return object_type<Schema, Fields...>(fields...);
}

template < typename Key, typename Reader >
auto field(Key,Reader, std::string doc = "")
{
    struct field_desc
    {
        field_desc(std::string str)
            : docstr(str)
        {}
        std::string docstr;

        using reader = Reader;
    };
    return boost::fusion::make_pair<Key>(field_desc(doc));
}

}


#endif
