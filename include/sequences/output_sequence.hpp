#include "deduce_param.hpp"

template<typename T>
class output_sequence
{
public:
    typedef T value_type;
    virtual void add(const T & item) const =0;

    template<typename Seq>
    const output_sequence<T> & operator<<(const Seq & seq) const
    {
        for(auto &i: seq) add(i);
        return *this;
    }

    const output_sequence<T> & operator<<(const T & item) const
    {
        add(item);
        return *this;
    }
};

namespace sequences
{
    // TODO: Just use a function_inserter
    template<typename T, typename It>
    class back_inserter_sequence : public output_sequence<T>
    {
        mutable It back_inserter;
    public:
        back_inserter_sequence(It it) : back_inserter(it) {}
        void add(const T & item) const override { *back_inserter++ = item; }
    };

    template<typename T, typename Fn>
    class function_inserter : public output_sequence<T>
    {
        Fn fn;
    public:
        function_inserter(Fn fn) : fn(fn) {}
        void add(const T & item) const override { fn(item); }
    };
}

// TODO: Think about a namespace

template<typename Fn, typename T>
sequences::function_inserter<T, Fn> writer(Fn fn) { return {fn}; }

template<typename Fn, typename = decltype(&Fn::operator())>
sequences::function_inserter<typename helpers::deduce_param<Fn>::type, Fn> writer(Fn fn) { return {fn}; }

template<typename Container>
sequences::back_inserter_sequence<typename Container::value_type, std::back_insert_iterator<Container>>
writer(Container & c)
{
    return { std::back_inserter(c) };
}
