// Implements a sequence that takes from underlying sequence while a condition is true

namespace sequences
{
    template<typename Seq, typename Predicate>
    class take_while_sequence : public base_sequence<typename Seq::value_type, take_while_sequence<Seq, Predicate>>
    {
        Seq seq;
        Predicate predicate;
    public:
        take_while_sequence(const Seq & s, Predicate p) : seq(s), predicate(p) {}

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
            auto result = seq.next();
            return result && !predicate(*result) ? nullptr : result;
        }
    };
}
