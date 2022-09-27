// sequence.hpp
// Defines sequence<T> and associated functions.
//
// A sequence is an abstract 

#pragma once

#include <iterator>
#include <type_traits>
#include <stdexcept>

namespace sequences
{
    template<typename T, typename Seq, typename Predicate>
    class where_sequence;

    template<typename T, typename Seq, typename Fn>
    class select_sequence;

    template<typename T, typename Seq>
    class take_sequence;

    template<typename T, typename Seq>
    class skip_sequence;

    template<typename T, typename Seq>
    class virtual_sequence;

    template<typename T, typename Seq1, typename Seq2>
    class concat_sequence;

    template<typename T, typename Derived, typename Stored=Derived>
    class base_sequence
    {
    public:
        typedef std::size_t size_type;
        typedef T value_type;

        Derived & self() const { return *const_cast<Derived*>(static_cast<const Derived*>(this)); }

        virtual_sequence<T, Stored> make_virtual() const { return {self()}; }

        operator virtual_sequence<T, Stored>() const { return make_virtual(); }

        size_type size() const
        {
            size_type c=0;
            for(auto i=self().first(); i; i=self().next()) ++c;
            return c;
        }

        struct iterator
        {
            Derived & underlying;
            const value_type * current;

            typedef value_type value_type;
            typedef value_type & reference;
            typedef value_type & pointer;
            typedef std::size_t size_type;
            typedef int difference_type;
            typedef std::input_iterator_tag iterator_category;

            const value_type & operator*() const { return *current; }
            iterator & operator++()
            {
                current = underlying.next();
                return *this;
            }

            bool operator!=(iterator other) const { return current!=other.current; }
        };

        iterator begin() const { return { self(), self().first() }; }
        iterator end() const { return { self(), nullptr }; }

        template<typename Predicate>
        where_sequence<T, Stored, Predicate> where(Predicate p) const
        {
            return { self(), p };
        }

        template<typename Fn>
        select_sequence<T, Stored, Fn> select(Fn fn) const
        {
            return { self(), fn };
        }

        take_sequence<T, Stored> take(int n) const
        {
            return {self(),n};
        }

        skip_sequence<T, Stored> skip(int n) const
        {
            return {self(),n};
        }

        template<typename Aggregate>
        T aggregate(Aggregate agg, T result = {}) const
        {
            for(const value_type * i=self().first(); i; i=self().next())
                result = agg(result, *i);
            return result;
        }

        T sum() const
        {
            return aggregate([](const T &i1, const T&i2) { return i1+i2; });
        }

        const T &at(size_type index) const
        {
            for(auto c = self().first(); c; c=self().next())
            {
                if(index--==0) return *c;
            }
            throw std::out_of_range("at() is out of range");
        }

        // TODO: Rename this
        const value_type &first2() const
        {
            auto c = self().first();
            if(!c) throw std::out_of_range("first() called on an empty list");
            return *c;
        }

        const value_type &last() const
        {
            for(auto c = self().first(); c;)
            {
                auto c2 = self().next();
                if(!c2) return *c;
                c = c2;
            }
            throw std::out_of_range("last() called on an empty list");
        }

        // Comparisons

        template<typename T2, typename Derived2,typename Stored2>
        bool operator==(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return equals(other);
        }

        template<typename T2, typename Derived2,typename Stored2>
        bool operator<(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return lexographical_compare(other);
        }

        template<typename T2, typename Derived2, typename Stored2, typename Eq = std::equal_to<T>>
        bool equals(const base_sequence<T2,Derived2,Stored2> & other, Eq eq = {}) const
        {
            auto i1 = self().first();
            auto i2 = other.self().first();

            while(i1 && i2)
            {
                if(!eq(*i1, *i2)) return false;
                i1 = self().next();
                i2 = other.self().next();
            }

            return !i1 && !i2;
        }

        template<typename T2, typename Derived2, typename Stored2,typename Less = std::less<T>>
        bool lexographical_compare(const base_sequence<T2,Derived2,Stored2> & other, Less lt = {}) const
        {
            auto i1 = self().first();
            auto i2 = other.self().first();

            while(i1 && i2)
            {
                if(lt(*i1, *i2)) return true;
                if(lt(*i2, *i1)) return false;
                i1 = self().next();
                i2 = other.self().next();
            }

            return !i1;
        }

        template<typename U>
        auto as() const
        {
            return select([](const T&t) { return U{t}; });
        }

        typedef Derived stored_type;

        // ?? Operator virtual_sequence?
    };

    template<typename T>
    class sequence_ref;
}

template<typename T>
class sequence : public sequences::base_sequence<T, sequence<T>, sequences::sequence_ref<T>>
{
public:
    typedef T value_type;
    virtual const value_type * first() = 0;
    virtual const value_type * next() = 0;
};

namespace sequences
{
    template<typename T, typename Seq>
    class virtual_sequence : public sequence<T>
    {
        Seq seq;
    public:
        virtual_sequence(Seq seq) : seq(seq) {}
        typedef virtual_sequence<T, Seq> stored_type;

        const T * first() override { return seq.first(); }
        const T * next() override { return seq.next(); }
    };

    template<typename T>
    class sequence_ref : public sequence<T>
    {
        sequence<T> & seq;
    public:
        sequence_ref(sequence<T> &ref) : seq(ref) {}
        const T * first() override { return seq.first(); }
        const T * next() override { return seq.next(); }
    };

    template<typename T>
    class empty_sequence : public base_sequence<T, empty_sequence<T>, empty_sequence<T>>
    {
    public:
        typedef T value_type;
        const value_type * first() { return nullptr; }
        const value_type * next() { return nullptr; }
    };

    template<typename T>
    class singleton_sequence : public base_sequence<T, singleton_sequence<T>, singleton_sequence<T>>
    {
        const T value;
    public:
        typedef T value_type;
        singleton_sequence(const T & v) : value(v) {}
        const value_type * first() { return &value; }
        const value_type * next() { return nullptr; }
    };

    template<typename It>
    class iterator_sequence : public base_sequence<typename std::iterator_traits<It>::value_type, iterator_sequence<It>, iterator_sequence<It>>
    {
        It from, to, current;
    public:
        typedef typename std::iterator_traits<It>::value_type value_type;

        iterator_sequence(It from, It to) : from(from), to(to), current(from) {}
        const value_type * first()
        { 
            current = from;
            return current!=to ? &*current : nullptr;
        }
        const value_type * next()
        {
            ++current;
            return current!=to ? &*current : nullptr;
        }        
    };

    template<typename T, typename Seq, typename Predicate>
    class where_sequence : public base_sequence<T, where_sequence<T,Seq,Predicate>>
    {
        Seq seq;
        Predicate pred;
    public:
        where_sequence(const Seq &seq, Predicate pred) : seq(seq), pred(pred) {}

        const T * first()
        {
            const T * result = seq.first();
            while(result && !pred(*result))
                result = seq.next();
            return result;
        }

        const T * next()
        {
            const T * result;
            do
                result = seq.next();
            while(result && !pred(*result));
            return result;
        }
    };

    template<typename T, typename Seq, typename Fn>
    class select_sequence : public base_sequence<std::invoke_result_t<Fn, T>, select_sequence<T,Seq,Fn>>
    {
        Seq seq;
        Fn fn;
        typedef std::invoke_result_t<Fn, T> value_type;
        value_type current;
    public:
        select_sequence(const Seq &seq, Fn fn) : seq(seq), fn(fn) {}

        const value_type * first()
        {
            const T * result = seq.first();
            if(result)
            {
                current = fn(*result);
                return &current;
            }
            else
            {
                return nullptr;
            }
        }

        const value_type * next()
        {
            const T * result = seq.next();
            if(result)
            {
                current = fn(*result);
                return &current;
            }
            else
            {
                return nullptr;
            }
        }
    };

    template<typename Container>
    class stored_sequence : public base_sequence<typename Container::value_type, stored_sequence<Container>>
    {
        Container container;
        typedef typename Container::value_type value_type;
        typename Container::const_iterator current;
    public:
        stored_sequence(const Container & c) : container(c) {}

        const value_type * first()
        {
            current = container.begin();
            return current == container.end() ? nullptr : &*current;
        }

        const value_type * next()
        {
            ++current;
            return current == container.end() ? nullptr : &*current;
        }
    };

    template<typename T, int Size>
    class stored_array_sequence : public base_sequence<T, stored_array_sequence<T, Size>>
    {
        T items[Size];
    public:

        typedef T value_type;

        stored_array_sequence(const T(&input)[Size])
        {
            for(int i=0; i<Size; i++)
                items[i] = input[i];
        }

        template<typename...Ts>
        stored_array_sequence(Ts...ts)
        {
            init(0, ts...);
        }

        void init(int i) {}

        template<typename...Ts>
        void init(int i, T t, Ts... ts)
        {
            items[i] = t;
            init(i+1, ts...);
        }

        const T * current;

        const value_type * first() {
            current = items;
            return Size==0 ? nullptr : current;
        }

        const value_type * next()
        {
            current++;
            return current == items+Size ? nullptr : current;
        }
    };

    template<typename T, typename Seq>
    class take_sequence : public base_sequence<T, take_sequence<T,Seq>>
    {
        Seq seq;
        int to_take;
        int index;
    public:
        take_sequence(const Seq & u, int count) : seq(u), to_take(count) {}

        const T * first()
        {
            return (index=0)<to_take ? seq.first() : nullptr;
        }

        const T * next()
        {
            return (++index)<to_take ? seq.next() : nullptr;
        }
    };
}

template<typename Container>
sequences::iterator_sequence<typename Container::const_iterator> seq(const Container &c)
{
    return {c.begin(), c.end()};
}

template<typename T, int Size>
sequences::iterator_sequence<T*> seq(const T (&items)[Size])
{
    return {items, items+Size};
}

template<typename T>
void seq(const std::initializer_list<T> & list) {}
// Error - use array instead

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
        typedef std::forward_iterator_tag iterator_category;
        const int &operator*() const { return value; }
        int_iterator & operator++() { ++value; return *this; }
        bool operator!=(const int_iterator & other) const { return value != other.value; }
    };
}

inline sequences::iterator_sequence<detail::int_iterator> seq(int a, int b)
{
    return {detail::int_iterator(a), detail::int_iterator(b+1)};
}

inline
sequences::iterator_sequence<const char*> seq(const char * cstr) {
    return {cstr, cstr+strlen(cstr) };
}

template<typename T>
sequences::iterator_sequence<T*> seq(T * ptr, int size) {
    return {ptr, ptr+size};
}
