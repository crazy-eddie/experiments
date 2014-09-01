#ifndef FUNCTIONAL_CONSTRUCTOR_HPP
#define FUNCTIONAL_CONSTRUCTOR_HPP

namespace functional {

template < typename T >
struct constructor
{
	using raw_t = typename std::remove_reference<T>::type;
	using type = typename raw_t::constructor;
};

}

#endif
