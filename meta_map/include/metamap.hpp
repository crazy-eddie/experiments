#ifndef METAMAP_HPP
#define METAMAP_HPP

#include "string.hpp"

namespace meta {

namespace detail_ {

template < typename ElemT >
struct map_nil
{
	template < typename Key >
	auto insert(ElemT val) const;

	bool has(std::string const&) const { return false; }

	ElemT const& operator [] (std::string const&) const { throw std::logic_error("Key not found in map"); }
};

template < typename Key
         , typename ElemT
         , typename Left = map_nil<ElemT>
         , typename Right = map_nil<ElemT> >
struct map_node
{
	bool has(std::string const& key) const
	{
		if (key < Key::value()) return left.has(key);
		if (key > Key::value()) return right.has(key);
		return key == Key::value();
	}

	ElemT const& operator [] (std::string const& key) const
	{
		if (key < Key::value()) return left[key];
		if (key > Key::value()) return right[key];

		return value;
	}

	template < typename Key2, typename cmp = typename string_compare<Key2,Key>::type >
	auto insert(ElemT val) const
	{
		return insert<Key2>(std::move(val), cmp());
	}

	template < typename Key2 >
	auto insert(ElemT val, str_less) const
	{
		auto new_left = left.template insert<Key2>(std::move(val));
		return map_node<Key,ElemT,decltype(new_left),Right>(value,std::move(new_left),right);
	}
	template < typename Key2 >
	auto insert(ElemT val, str_greater) const
	{
		auto new_right = right.template insert<Key2>(std::move(val));
		return map_node<Key,ElemT,Left,decltype(new_right)>(value,left,std::move(new_right));
	}
	template < typename Key2 >
	auto insert(ElemT val, str_equal) const
	{
		return map_node<ElemT,Key,Left,Right>(std::move(val),left,right);
	}

	map_node(ElemT val) : value(std::move(val)) {}
	map_node(ElemT val, Left l, Right r)
		: value(std::move(val))
	 	, left(std::move(l))
		, right(std::move(r))
	{}

private:
	ElemT value;
	Left left;
	Right right;
};



template < typename ElemT >
template < typename Key >
auto map_nil<ElemT>::insert(ElemT val) const
{
	return map_node<Key,ElemT>(val);
}

template < typename Key, typename Elem >
struct map_entry
{
	using key = Key;
	Elem elem;
	map_entry(Elem e) : elem(std::move(e)) {}
};

template < typename Map
         , typename ... Entries >
struct inserter;

template < typename Map, typename Entry, typename ... Entries >
struct inserter<Map, Entry, Entries...>
{
	static auto insert(Map const& m, Entry e, Entries ... entries)
	{
		auto new_map = m.template insert<typename Entry::key>(std::move(e.elem));
		return inserter<decltype(new_map),Entries...>::insert(new_map,std::move(entries)...);
	}
};

template < typename Map >
struct inserter<Map>
{
	static Map insert(Map const& m) { return m; }
};

}

template < typename ElemT >
auto map() { return detail_::map_nil<ElemT>{}; }

template < typename Key, typename Elem >
auto map_entry(Elem e) { return detail_::map_entry<Key,Elem>(std::move(e)); }

template < typename ElemT, typename ... Entries >
auto map(Entries && ... entries)
{
	auto map_start = map<ElemT>();
	return detail_::inserter<decltype(map_start), Entries...>::insert(map_start,entries...);
}

}

#endif
