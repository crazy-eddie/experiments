#ifndef CRAZY_CREW_STRING
#define CRAZY_CREW_STRING

#include <boost/intrusive_ptr.hpp>
#include <atomic>
#include <memory>

#include "byte_buffer.hpp"

namespace utility {


struct string
{
    string()
        : string{byte_buffer{}}
    {}

    explicit string(char const* s)
        : string{byte_buffer{s}}
    {}

    string(byte_buffer && b)
        : string{new byte_buffer{std::move(b)}, std::begin(b), std::end(b)}
    {}

    bool empty() const { return size() == 0; }
    int size() const { return stop - start; }

    char const* data() const { return start; }

    char const* begin() const { return start; }
    char const* end() const { return stop; }

    string substr(int beg, int len = -1) const
    {
        char const* new_begin = begin() + beg;
        char const* new_end = len == -1 ? end() : new_begin + len;

        return string(storage, new_begin, new_end);
    }

private:

    boost::intrusive_ptr<byte_buffer> storage;
    char const* start;
    char const* stop;

    string(boost::intrusive_ptr<byte_buffer> const& store, char const* b, char const* e)
        : storage{store}
        , start{b}
        , stop{e}
    {}

    friend struct mutable_string;
};

bool operator == (string const& s0, string const& s1)
{
    if (s0.data() == s1.data()) return true;
    if (s0.size() != s1.size()) return false;

    return std::equal(std::begin(s0), std::end(s0), std::begin(s1));
}

bool operator == (string const& s0, char const* cstr)
{
    for (auto && c : s0)
    {
        if (!*cstr || c != *cstr++) return false;
    }
    return *cstr == '\0';
}
bool operator == (char const* cstr, string const& s0) { return s0 == cstr; }
std::ostream & operator << (std::ostream & out, string const& s)
{
    std::copy(std::begin(s), std::end(s), std::ostream_iterator<char>(out));
    return out;
}



struct mutable_string
{
    mutable_string() : storage{new byte_buffer{}} {}

    explicit mutable_string(char const* str) : storage{new byte_buffer{str}} {}

#if 0
    mutable_string(string && str)
        : storage(detail_::detatch_storage(std::move(str.storage)))
    {
        assert(storage->ref_count.load() == 1 && "Please don't move a string that's not really a temporary!");
    }
#endif


    bool empty() const { return !size(); }
    int size() const { return storage->size(); }
    char const* data() const { return begin(); }

    char const* begin() const { return std::begin(*storage); }
    char const* end() const { return std::end(*storage); }

    mutable_string & operator += (char const* in)
    {
        storage->append(in);
        return *this;
    }

private:
    std::unique_ptr<byte_buffer> storage;
};

bool operator == (mutable_string const& lh, mutable_string const& rh)
{
    if (lh.size() != rh.size()) return false;
    return std::equal(std::begin(lh), std::end(lh), std::begin(rh));
}

bool operator == (mutable_string const& lh, char const* cstr)
{
    for (auto && c : lh)
    {
        if (!cstr || c != *cstr++) return false;
    }
    return *cstr == '\0';
}
bool operator == (char const* cstr, mutable_string const& rh) { return rh == cstr; }

std::ostream& operator << (std::ostream & out, mutable_string const& str)
{
    std::copy(std::begin(str), std::end(str), std::ostream_iterator<char>(out));
    return out;
}

}

#endif
