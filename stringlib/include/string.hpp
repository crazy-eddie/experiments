#ifndef CRAZY_CREW_STRING
#define CRAZY_CREW_STRING

#include <boost/intrusive_ptr.hpp>
#include <atomic>
#include <memory>

namespace utilities {

namespace detail_ {


struct storage_area
{
    int len;
    char * buffer;

    mutable std::atomic<int> ref_count;

    storage_area() : len(0), buffer(nullptr), ref_count(1) {}
    storage_area(char const* s)
        : len(strlen(s))
        , buffer(new char[len])
        , ref_count(1)
    {
        std::copy(s, s + len, buffer);
    }
    ~storage_area() { delete buffer; }


    friend void intrusive_ptr_add_ref(storage_area const* x)
    {
      x->ref_count.fetch_add(1, std::memory_order_relaxed);
    }
    friend void intrusive_ptr_release(storage_area const* x)
    {
      if (x->ref_count.fetch_sub(1, std::memory_order_release) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete x;
      }
    }
};

storage_area * detatch_storage(boost::intrusive_ptr<storage_area> && strg)
{
    intrusive_ptr_add_ref(strg.get());
    storage_area * tmp = strg.get();

    strg.reset();

    return tmp;
}


}

struct string
{
    string()
        : storage(new detail_::storage_area())
        , start(nullptr)
        , stop(nullptr)
    {}
    string(char const* s)
        : storage(new detail_::storage_area(s))
        , start(storage->buffer)
        , stop(storage->buffer + storage->len)
    {}

    bool empty() const { return size() == 0; }
    int size() const { return stop-start; }

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

    boost::intrusive_ptr<detail_::storage_area> storage;
    char const* start;
    char const* stop;


    string(boost::intrusive_ptr<detail_::storage_area> const& store, char const* b, char const* e)
        : storage(store)
        , start(b)
        , stop(e)
    {}

    friend struct mutable_string;
};

bool operator == (string const& s0, string const& s1)
{
    if (s0.data() == s1.data()) return true;
    if (s0.size() != s1.size()) return false;

    return std::equal(std::begin(s0), std::end(s0), std::begin(s1));
}

std::ostream & operator << (std::ostream & out, string const& s)
{
    std::copy(std::begin(s), std::end(s), std::ostream_iterator<char>(out));
    return out;
}




struct mutable_string
{
    mutable_string() : storage{new detail_::storage_area{}} {}
    mutable_string(char const* str) : storage{new detail_::storage_area{str}} {}

    mutable_string(string && str)
        : storage(detail_::detatch_storage(std::move(str.storage)))
    {
        assert(storage->ref_count.load() == 1 && "Please don't move a string that's not really a temporary!");
    }


    bool empty() const { return size() == 0; }
    int size() const { return storage->len; }
    char const* data() const { return storage->buffer; }

    char const* begin() const { return data(); }
    char const* end() const { return data() + size(); }

    mutable_string & operator += (char const* in)
    {
        return mutable_string(string(*this) + in);
    }

private:
    std::unique_ptr<detail_::storage_area> storage;
};

bool operator == (mutable_string const& lh, mutable_string const& rh)
{
    if (lh.size() != rh.size()) return false;
    return std::equal(std::begin(lh), std::end(lh), std::begin(rh));
}


std::ostream& operator << (std::ostream & out, mutable_string const& str)
{
    std::copy(std::begin(str), std::end(str), std::ostream_iterator<char>(out));
    return out;
}


}

#endif
