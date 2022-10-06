// Implements a sequence that is a reference to another sequence

namespace sequences
{
    template<typename T>
    class sequence_ref : public base_sequence<T, sequence_ref<T>>
    {
        sequence<T> & seq;
    public:
        sequence_ref(sequence<T> &ref) : seq(ref) {}
        const T * first() { return seq.first(); }
        const T * next() { return seq.next(); }
        std::size_t size() const  { return seq.size(); }
    };
}
