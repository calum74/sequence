namespace sequences
{
    template<typename Seq1, typename Seq2, typename Fn>
    class merge_sequence : public base_sequence<
        std::invoke_result_t<Fn, typename Seq1::value_type, typename Seq2::value_type>, 
        merge_sequence<Seq1, Seq2, Fn>>
    {
        Seq1 seq1;
        Seq2 seq2;
        Fn fn;
    public:
        merge_sequence(const Seq1 & s1, const Seq2 & s2, Fn fn) : seq1(s1), seq2(s2), fn(fn) {}

        typedef std::invoke_result_t<Fn, typename Seq1::value_type, typename Seq2::value_type> value_type;

        value_type current;

        const value_type * first()
        {
            auto r1 = seq1.first();
            auto r2 = seq2.first();
            if(r1 && r2)
            {
                current = fn(*r1, *r2);
                return &current;
            }
            return nullptr;
        }

        const value_type * next()
        {
            auto r1 = seq1.next();
            auto r2 = seq2.next();
            if(r1 && r2)
            {
                current = fn(*r1, *r2);
                return &current;
            }
            return nullptr;
        }

    };
}