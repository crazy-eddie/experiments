#ifndef METATRIE_HPP
#define METATRIE_HPP

#include "string.hpp"

namespace meta {

template < typename Key
         , typename Elem >
struct trie_entry_
{
	trie_entry_(Elem elem)
		: value(std::move(elem))
	{}

private:
	Elem value;
};

template < typename Key, typename Elem >
auto trie_entry(Elem e) { return trie_entry_<Key,Elem>(std::move(e)); }

namespace detail_ {

template < typename ElemT
         , typename KeyPrefix = META_STRING("")
         , typename ... SubNodes >
struct trie
{
};

}

template < typename ElemT, typename ... Entry >
auto trie(Entry ... entry)
{
	return detail_::trie<ElemT>();
}

}


#endif
