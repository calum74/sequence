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


namespace sequences
{
    namespace detail
    {
        template<typename Container>
        struct appender
        {
            Container & c;
            void operator()(const typename Container::value_type & item) const {
                c.insert(c.end(), item);
            }
        };
    }
}
