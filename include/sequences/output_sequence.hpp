
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
};

/*
template<typename T>
class output_sequence_ref
{
    output_sequence<T> &r;

    template<typename Fn>
    output_sequence_ref(function_source & src)
};
*/

namespace sequences
{
    template<typename T, typename It>
    class back_inserter_sequence : public output_sequence<T>
    {
        mutable It back_inserter;
    public:
        back_inserter_sequence(It it) : back_inserter(it) {}
        void add(const T & item) const override { *back_inserter++ = item; }
    };

    template<typename Fn>
    struct arg_type;

    template<typename T>
    struct arg_type<void(T)>
    {
        typedef T type;
    };

    template<typename T, typename Fn>
    class function_inserter : public output_sequence<T>
    {
        Fn fn;
    public:
        function_inserter(Fn fn) : fn(fn) {}
        void add(const T & item) const override { fn(item); }
    };

    template<typename Fn>
    class function_source
    {
    public:
        Fn fn;

        template<typename T>
        operator function_inserter<T,Fn>() const { return {fn}; }
    };
}

// TODO: Think about a namespace

//template<typename T>
//sequences::function_inserter<T, void(*)(T)> writer(void (*fn)(T)) { return {fn}; }

// TODO: How to deduce this template parameter?
template<typename T, typename Fn>
sequences::function_inserter<T, Fn> writer(Fn fn) { return {fn}; }

template<typename Fn>
sequences::function_source<Fn> writer2(Fn fn) { return {fn}; }

//template<typename Fn>
// sequences::function_source<> writer2(const std::function<void(T)> fn) { return {fn}; }


//template<typename T>
// sequences::function_inserter<T>

template<typename Container>
sequences::back_inserter_sequence<typename Container::value_type, std::back_insert_iterator<Container>> writer(Container & c)
{
    return { std::back_inserter(c) };
}
