#include <type_traits>

namespace sequences
{
    template<typename T, typename Seq, typename Fn>
    class select_sequence : public base_sequence<std::invoke_result_t<Fn, T>, select_sequence<T,Seq,Fn>>
    {
        Seq seq;
        Fn fn;
        typedef std::invoke_result_t<Fn, T> value_type;
        value_type current;
    public:
        select_sequence(const Seq &seq, Fn fn) : seq(seq), fn(fn) {}

        const value_type * first()
        {
            const T * result = seq.first();
            if(result)
            {
                current = fn(*result);
                return &current;
            }
            else
            {
                return nullptr;
            }
        }

        const value_type * next()
        {
            const T * result = seq.next();
            if(result)
            {
                current = fn(*result);
                return &current;
            }
            else
            {
                return nullptr;
            }
        }

        std::size_t size() const { return seq.size(); }
    };
}
