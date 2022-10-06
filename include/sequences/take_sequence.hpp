// Implements a sequence that takes/limits the size of another sequence

namespace sequences
{
    template<typename T, typename Seq>
    class take_sequence : public base_sequence<T, take_sequence<T,Seq>>
    {
        Seq seq;
        int to_take;
        int index;
    public:
        take_sequence(const Seq & u, int count) : seq(u), to_take(count) {}

        const T * first()
        {
            return (index=0)<to_take ? seq.first() : nullptr;
        }

        const T * next()
        {
            return (++index)<to_take ? seq.next() : nullptr;
        }
    };
}
