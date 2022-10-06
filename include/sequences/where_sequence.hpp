// Implements a sequence that filters another sequence

namespace sequences
{
    template<typename T, typename Seq, typename Predicate>
    class where_sequence : public base_sequence<T, where_sequence<T,Seq,Predicate>>
    {
        Seq seq;
        Predicate pred;
    public:
        where_sequence(const Seq &seq, Predicate pred) : seq(seq), pred(pred) {}

        const T * first()
        {
            const T * result = seq.first();
            while(result && !pred(*result))
                result = seq.next();
            return result;
        }

        const T * next()
        {
            const T * result;
            do
                result = seq.next();
            while(result && !pred(*result));
            return result;
        }
    };
}
