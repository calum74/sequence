namespace sequences
{
    template<typename T>
    class sequence_ref : public sequence<T>
    {
        sequence<T> & seq;
    public:
        sequence_ref(sequence<T> &ref) : seq(ref) {}
        const T * first() override { return seq.first(); }
        const T * next() override { return seq.next(); }
    };
}
