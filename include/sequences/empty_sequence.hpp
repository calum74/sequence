// Implements the empty sequence

namespace sequences
{
    template<typename T>
    class empty_sequence : public base_sequence<T, empty_sequence<T>, empty_sequence<T>>
    {
    public:
        typedef T value_type;
        const value_type * first() { return nullptr; }
        const value_type * next() { return nullptr; }
        std::size_t size() const { return 0; }
    };
}
