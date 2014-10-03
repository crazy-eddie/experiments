#ifndef JSON_HPP
#define JSON_HPP

#include "json/fundamentals.hpp"
#include "json/array.hpp"
#include "json/object.hpp"

namespace json {


template < typename TypeContract
         , typename InputRange >
auto read(TypeContract contract, InputRange range)
{
	return contract.read(range);
}

template < typename TypeContract >
auto read(TypeContract contract, char const* str)
{
	return read(contract, boost::make_iterator_range(str, str+strlen(str)));
}

}


#endif
