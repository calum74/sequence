// Support for sequences that are created by lambdas
// This feature is experimental
namespace sequences
{
    template<typename First, typename Next>
    class generated_sequence : public base_sequence<typename helpers::deduce_param<First>::type, generated_sequence<First, Next>>
    {
        First firstFn;
        Next nextFn;
    public:
        typedef typename helpers::deduce_param<First>::type value_type;

        generated_sequence(First f, Next n) : firstFn(f), nextFn(n) {}

        value_type result;

        const value_type * first()
        {
            return firstFn(result) ? &result : nullptr;
        }

        const value_type * next()
        {
            return nextFn(result) ? &result : nullptr;
        }
    };
}

// Experimental - maybe delete
template<typename Fn>
sequences::generated_sequence<Fn, Fn> generator(Fn fn) { return {fn, fn}; }

template<typename F1, typename F2>
sequences::generated_sequence<F1, F2> generator(F1 f1, F2 f2) { return {f1, f2}; }
