// Implements output_sequence and supporting functions

// An output sequence, used for writing elements to an arbitrary sink.
// T is the element type of the sequence
template<typename T>
class output_sequence
{
public:
    typedef T value_type;

    // Add an element to the sequence
    virtual void add(const T & item) const =0;

    // Stream the contents of a sequence to the output
    template<typename Seq, typename = typename Seq::is_sequence>
    const output_sequence<T> & operator<<(const Seq & seq) const
    {
        for(auto &i: seq) add(i);
        return *this;
    }

    // Add an element to the sequence (alternative)
    const output_sequence<T> & operator<<(const T & item) const
    {
        add(item);
        return *this;
    }

    // Helper to identify this type as an output sequence
    typedef void is_output_sequence;
};

namespace sequences
{
    // An output sequence that calls the given functor for each element added.
    template<typename T, typename Fn>
    class function_inserter : public output_sequence<T>
    {
        Fn fn;
    public:
        function_inserter(Fn fn) : fn(fn) {}

        void add(const T & item) const override
        { 
            fn(item);
        }
    };

    namespace detail
    {
        // Functor to append elements to a given container
        template<typename Container>
        struct appender
        {
            Container & c;

            void operator()(const typename Container::value_type & item) const
            {
                c.insert(c.end(), item);
            }
        };
    }
}
