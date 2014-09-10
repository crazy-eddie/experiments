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

struct nil {};

template < typename ElemT, typename KeySection >
struct trie_node
{
	trie_node(ElemT elem)
		: value(std::move(elem))
	{}

	bool has_elem(std::string const& e) const
	{
		return e == KeySection::value();
	}

	ElemT const& find(std::string const&) const { return value; }

	ElemT value;
};

template < typename ElemT, typename Root = nil >
struct trie
{
	bool has_elem(std::string const& e) const
	{
		return root.has_elem(e);
	}

	ElemT const& find(std::string const& e) const { return root.find(e); }

	trie(Root r) : root(std::move(r)) {}

private:
	Root root;
};

template < typename ElemT >
struct trie<ElemT, nil>
{
	bool has_elem(std::string const& e) const
	{
		return false;
	}

	template < typename Key >
	auto add_value(ElemT elem) const
	{
		return trie<ElemT,trie_node<ElemT,Key>>(std::move(elem));
	}
};

}

template < typename ElemT, typename ... Entry >
auto trie(Entry ... entry)
{
	return detail_::trie<ElemT>();
}

}


#endif
