// Implements a sequence that skips the first elements of another sequence

namespace sequences
{
    template<typename T, typename Seq>
    class skip_sequence : public base_sequence<T, skip_sequence<T,Seq>>
    {
        Seq seq;
        int to_skip;
    public:
        skip_sequence(const Seq & u, int count) : seq(u), to_skip(count) {}

        const T * first()
        {
            auto result = seq.first();
            for(int i=0; result && i<to_skip; i++)
                result = seq.next();
            return result;
        }

        const T * next()
        {
            return seq.next();
        }
    };
}
