// Implements a sequence that skips until a condition is met.

namespace sequences
{
    template<typename Seq, typename Predicate>
    class skip_until_sequence : public base_sequence<typename Seq::value_type, skip_until_sequence<Seq, Predicate>>
    {
        Seq seq;
        Predicate predicate;
    public:
        skip_until_sequence(const Seq & s, Predicate p) : seq(s), predicate(p) {}

        typedef typename Seq::value_type value_type;

        const value_type * first()
        {
            const value_type * result;
            for(result = seq.first(); result && !predicate(*result); result = seq.next())
                ;
            return result;
        }

        const value_type * next()
        {
            return seq.next();
        }
    };
}
