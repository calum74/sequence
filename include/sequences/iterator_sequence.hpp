namespace sequences
{

    template<typename It>
    class iterator_sequence : public base_sequence<typename std::iterator_traits<It>::value_type, iterator_sequence<It>, iterator_sequence<It>>
    {
        It from, to, current;
    public:
        typedef typename std::iterator_traits<It>::value_type value_type;

        iterator_sequence(It from, It to) : from(from), to(to), current(from) {}
        const value_type * first()
        { 
            current = from;
            return current!=to ? &*current : nullptr;
        }
        const value_type * next()
        {
            ++current;
            return current!=to ? &*current : nullptr;
        }

        std::size_t size() const { return std::distance(from, to); }
    };
}
