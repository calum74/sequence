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
        std::size_t size() const override { return seq.size(); }
    };
}
