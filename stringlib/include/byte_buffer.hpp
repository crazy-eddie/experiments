#ifndef BYTE_BUFFER_HPP
#define BYTE_BUFFER_HPP

#include <atomic>
#include <memory>
#include <iosfwd>
#include <iostream>

namespace utility {

struct byte_buffer
{
    byte_buffer() : bytes{nullptr}, len{0} {}
    explicit byte_buffer(char const* str)
        : bytes{new char[strlen(str)]}
        , len{static_cast<int>(strlen(str))}
    {
        std::copy(str, str + len, bytes);
    }
    explicit byte_buffer(char *&& buf, int l) : bytes{buf}, len{l} { buf = nullptr; }

    byte_buffer(byte_buffer const& other)
        : bytes{new char[other.size()]}
        , len{other.size()}
    {
        std::copy(std::begin(other), std::end(other), bytes);
    }
    byte_buffer(byte_buffer && other)
        : bytes{other.bytes}
        , len{other.len}
    {
        other.bytes = nullptr;
        other.len = 0;
    }

    byte_buffer& operator = (byte_buffer const&) = delete;

    ~byte_buffer() { delete bytes; }

    char const* begin() const { return bytes; }
    char const* end() const { return bytes + size(); }

    int size() const { return len; }

    byte_buffer& append(byte_buffer const& buf)
    {
        return append(std::begin(buf), std::end(buf));
    }

    byte_buffer& append(char const* cstr)
    {
        return append(cstr, cstr + strlen(cstr));
    }

    template < typename T >
    byte_buffer& operator += (T const& str)
    {
        return append(str);
    }

private:
    char * bytes;
    int len;

    template < typename FwdIter >
    byte_buffer& append(FwdIter beg, FwdIter end)
    {
        // TODO: make this exception safe!
        auto new_buf = new char[std::distance(beg,end) + size()];

        std::copy(std::begin(*this), std::end(*this), new_buf);
        std::copy(beg, end, new_buf + size());
        std::swap(bytes, new_buf);

        len += std::distance(beg,end);

        return *this;
    }

    mutable std::atomic<int> ref_count;


    friend void intrusive_ptr_add_ref(byte_buffer const* x)
    {
      x->ref_count.fetch_add(1, std::memory_order_relaxed);
    }
    friend void intrusive_ptr_release(byte_buffer const* x)
    {
      if (x->ref_count.fetch_sub(1, std::memory_order_release) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete x;
      }
    }

};

bool operator == (byte_buffer const& b0, byte_buffer const& b1)
{
    if (b0.size() != b1.size()) return false;

    return std::equal(std::begin(b0), std::end(b0), std::begin(b1));
}

bool operator == (byte_buffer const& b0, char const* cstr)
{
    for (auto && byte : b0)
    {
        if (!*cstr || byte != *cstr++) return false;
    }
    return *cstr == '\0';
}

bool operator == (char const* cstr, byte_buffer const& b0)
{
    return b0 == cstr;
}

std::ostream& operator << (std::ostream & out, byte_buffer const& b)
{
    std::copy(std::begin(b), std::end(b), std::ostream_iterator<char>(out));
    return out;
}

}

#endif
