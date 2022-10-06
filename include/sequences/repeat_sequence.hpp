// Implements the sequence to repeat a sequence a given number of times.

namespace sequences
{
    template<typename Seq>
    class repeat_sequence : public base_sequence<typename Seq::value_type, repeat_sequence<Seq>>
    {
        Seq seq;
        int repeat, index;
    public:
        repeat_sequence(const Seq & seq, int repeat) : seq(seq), repeat(repeat) {}

        const typename Seq::value_type * first()
        {
            index=0;
            return repeat>0 ? seq.first() : nullptr;
        }

        const typename Seq::value_type * next()
        {
            auto result = seq.next();
            if(result) return result;
            return ++index<repeat ? seq.first() : nullptr;
        }
    };
}
