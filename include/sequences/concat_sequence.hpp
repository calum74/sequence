// Implements a sequence to concatenate two sequences

namespace sequences
{
    template<typename Seq1, typename Seq2>
    class concat_sequence : public base_sequence<typename Seq1::value_type, concat_sequence<Seq1,Seq2>>
    {
        Seq1 seq1;
        Seq2 seq2;
        bool inLeft;
    public:
        concat_sequence(const Seq1 &s1, const Seq2 & s2) : seq1(s1), seq2(s2) {}

        typedef typename Seq1::value_type value_type;

        const value_type * first()
        {
            inLeft = true;
            auto result = seq1.first();
            if(result) return result;
            inLeft = false;
            return seq2.first();
        }

        const value_type * next()
        {
            if(inLeft)
            {
                auto result = seq1.next();
                if(result) return result;
                inLeft = false;
                return seq2.first();
            }
            return seq2.next();
        }

        // Override for a more efficient implementation
        std::size_t size() const { return seq1.size() + seq2.size(); }
    };
}