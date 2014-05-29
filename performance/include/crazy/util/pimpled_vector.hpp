#ifndef CRAZY_UTIL_PIMPLED_VECTOR_HPP
#define CRAZY_UTIL_PIMPLED_VECTOR_HPP

#include "pimpl.hpp"

namespace crazy { namespace util {

template < typename Derived >
struct pimpled_vector
{
	bool empty() const { return true; }

protected:
	~pimpled_vector() {}
};

}}

#define DECLARE_VECTOR_OVERRIDES(classname) \
	classname(); \
	classname(classname const&); \
	classname(classname &&); \
	~classname(); \
	classname& operator=(classname)

#define DEFINE_VECTOR_OVERRIDES(classname) \
	classname::classname() : pimpl() {} \
	classname::classname(classname const& other) : pimpl(other.pimpl) {} \
	classname::classname(classname && other) : pimpl(std::move(other.pimpl)) {} \
	classname::~classname(){} \
	classname& classname::operator = (classname other) { pimpl.swap(other.pimpl); return *this; }


#endif
