#include "deduce_param.hpp"

template<typename T>
class output_sequence
{
public:
    typedef T value_type;
    virtual void add(const T & item) const =0;

    template<typename Seq, typename = typename Seq::is_sequence>
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

    typedef void is_output_sequence;
};

namespace sequences
{
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
sequences::function_inserter<T, Fn> receiver(Fn fn) { return {fn}; }

template<typename Fn, typename = decltype(&Fn::operator())>
sequences::function_inserter<typename helpers::deduce_param<Fn>::type, Fn> receiver(Fn fn) { return {fn}; }

template<typename Container>
auto writer(Container & c)
{
    return receiver([&c](const typename Container::value_type&item) { c.insert(c.end(), item); });
}
