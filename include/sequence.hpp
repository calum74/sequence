// sequence.hpp
// Defines sequence<T> and associated functions.
//
// A sequence is an abstract 

#pragma once

#include "sequence_fwd.hpp"
#include "sequences/fwd.hpp"
#include "sequences/base_sequence.hpp"
#include "sequences/sequence.hpp"
#include "sequences/virtual_sequence.hpp"
#include "sequences/sequence_ref.hpp"
#include "sequences/empty_sequence.hpp"
#include "sequences/singleton_sequence.hpp"
#include "sequences/iterator_sequence.hpp"
#include "sequences/where_sequence.hpp"
#include "sequences/select_sequence.hpp"
#include "sequences/stored_array_sequence.hpp"
#include "sequences/pointer_sequence.hpp"
#include "sequences/take_sequence.hpp"
#include "sequences/skip_sequence.hpp"
#include "sequences/stored_sequence.hpp"
#include "sequences/concat_sequence.hpp"
#include "sequences/take_while_sequence.hpp"
#include "sequences/skip_until_sequence.hpp"
#include "sequences/merge_sequence.hpp"
#include "sequences/output_sequence.hpp"

template<typename Container, typename = typename Container::value_type>
sequences::iterator_sequence<typename Container::const_iterator> seq(const Container &c)
{
    return {std::begin(c), std::end(c)};
}

template<typename Container, typename = typename Container::value_type>
sequences::iterator_sequence<typename Container::const_iterator> seq(Container &c)
{
    return {std::begin(c), std::end(c)};
}

template<typename T, int Size>
pointer_sequence<T> seq(const T (&items)[Size])
{
    return {items, items+Size};
}

//template<typename T>
//void seq(const std::initializer_list<T> & list) = delete;
// Error - use list instead

template<typename T, typename...Ts>
sequences::stored_array_sequence<T, 1+sizeof...(Ts)> list(T t, Ts... ts)
{
    return {t, ts...};
}    

template<typename T>
sequences::singleton_sequence<T> single(const T &t) { return {t}; }

template<typename T>
sequences::empty_sequence<T> seq() { return {}; }

template<typename T>
sequences::empty_sequence<T> list() { return {}; }


namespace detail
{
    class int_iterator
    {
        int value;
    public:
        int_iterator(int value) : value(value) {}
        typedef int value_type;
        typedef int difference_type;
        typedef int * pointer;
        typedef int & reference;
        typedef std::random_access_iterator_tag iterator_category;
        const int &operator*() const { return value; }
        int_iterator & operator++() { ++value; return *this; }
        bool operator!=(const int_iterator & other) const { return value != other.value; }

        int operator-(int_iterator other) const { return value - other.value; }
    };
}

inline sequences::iterator_sequence<detail::int_iterator> seq(int a, int b)
{
    return {detail::int_iterator(a), detail::int_iterator(b+1)};
}

inline pointer_sequence<char> seq(const char * cstr) {
    return {cstr, cstr+strlen(cstr) };
}

template<typename T>
pointer_sequence<T> seq(T * ptr, int size) {
    return {ptr, ptr+size};
}

template<typename Ch>
pointer_sequence<Ch> seq(const std::basic_string<Ch> & str) {
    return {str.data(), str.data()+str.size() };   
}

template<typename T, typename Alloc>
pointer_sequence<T> seq(const std::vector<T, Alloc> & vec)
{
    return { vec.data(), vec.data()+vec.size() };
}

// This is really problematic!
// stored_sequences are pretty slow
template<typename T, typename = typename T::value_type>
sequences::stored_sequence<T> seq(T && src) { return {std::move(src)}; }

template<typename T>
pointer_sequence<T> seq(const T*a, const T *b) { return {a,b}; }

template<typename It>
sequences::iterator_sequence<It> seq(It a, It b) { return {a,b}; }
