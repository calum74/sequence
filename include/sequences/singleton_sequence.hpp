// Implements a sequence of exactly one element
namespace sequences
{
    template<typename T>
    class singleton_sequence : public base_sequence<T, singleton_sequence<T>, singleton_sequence<T>>
    {
        const T value;
    public:
        typedef T value_type;
        singleton_sequence(const T & v) : value(v) {}
        const value_type * first() { return &value; }
        const value_type * next() { return nullptr; }
        std::size_t size() const { return 1; }
    };
}
