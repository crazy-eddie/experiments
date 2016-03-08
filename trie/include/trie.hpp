#ifndef UTILITIES_TRIE
#define UTILITIES_TRIE

#include <boost/optional.hpp>

namespace utilities {

namespace detail_
{
    struct trie_node
    {
        trie_node() : elements{}, prefix{}, data{} {}
        trie_node(std::string const& pre, std::string const& dat)
            : prefix{pre}
            , data{dat}
        {}
        trie_node(std::string const& pre, std::string const& dat, std::vector<trie_node> const& elems)
            : elements{elems}
            , prefix{pre}
            , data{dat}
        {}

        int size() const
        {
            auto child_count =  std::accumulate( std::begin(elements)
                                               , std::end(elements)
                                               , 0
                                               , [](int i, trie_node const& node)
                                                 { return node.size() + i; });

            return child_count + (data ? 1:0);
        }
        std::string const& find(std::string const& index) const
        {
            if (index == prefix) return *data;

            auto next_prefix = index.substr(prefix.size());

            auto fit = std::find_if( std::begin(elements)
                                   , std::end(elements)
                                   , [&next_prefix](trie_node const& node)
                                     {
                                         return node.prefix_match(next_prefix);
                                     });

            if (fit != std::end(elements)) return fit->find(next_prefix);

            static std::string not_found = "NOT FOUND";
            return not_found;
        }
        void insert(std::pair<std::string,std::string> const& element)
        {
            insert(trie_node{element.first, element.second});
        }

        void insert(trie_node new_node)
        {
            if (prefix.size() == new_node.prefix.size())
            {
                assert(new_node.prefix == prefix);
                // need to do a merge?  Probably not.
                swap(new_node);
            }

            if (!prefix.empty() && prefix_match(new_node.prefix))
            {
                // find shared length...
                auto my_len = 0;
                auto their_len = 0;
                while (prefix[my_len] == new_node.prefix[their_len])
                {
                    ++my_len;
                    ++their_len;
                }
                // possibilities:
                // * incomming key longer = push back on elements
                // * incomming key shorter = create new node, insert ourselves and children, swap.
                if (static_cast<size_t>(their_len) == new_node.prefix.size())
                {
                    new_node.insert(trie_node{prefix.substr(my_len), *data, elements});

                    swap(new_node);
                }
                return;
            }
            auto fit = std::find_if( std::begin(elements)
                                   , std::end(elements)
                                   , [&new_node] (trie_node const& node)
                                     {
                                         return node.prefix_match(new_node.prefix);
                                     });

            if (fit == std::end(elements))
            {
                elements.emplace_back(new_node);
                return;
            }

            fit->insert(trie_node(new_node.prefix, *new_node.data, new_node.elements));
        }

        void swap(trie_node & other)
        {
            std::swap(elements, other.elements);
            std::swap(prefix, other.prefix);
            std::swap(data, other.data);
        }

        bool prefix_match(std::string const& attempt) const
        {
            if (prefix.empty()) return attempt.empty();

            return prefix[0] == attempt[0];
        }

        std::vector<trie_node> elements;
        std::string prefix;
        boost::optional<std::string> data;
    };
}

struct trie
{
    trie() : root{} {}

    int size() const { return root.size(); }

    void insert(std::pair<std::string,std::string> const& element)
    {
        root.insert(element);
    }

    std::string const& operator[] (std::string const& index) const
    {
        return root.find(index);
    }

private:
    detail_::trie_node root;
};


}


#endif
