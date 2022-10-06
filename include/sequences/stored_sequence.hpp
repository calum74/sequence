// Implements a sequence that stores an internal copy of the data

namespace sequences
{
    template<typename Container>
    class stored_sequence : public base_sequence<typename Container::value_type, stored_sequence<Container>>
    {
    public:
        Container container;
        typedef typename Container::value_type value_type;
        typename Container::const_iterator current;

        stored_sequence(Container && c) : container(std::move(c)) {}

        // TODO: Do we want this?
        // Efficiency warning
        // stored_sequence(const stored_sequence & other) = delete;

        const value_type * first()
        {
            current = container.begin();
            return current == container.end() ? nullptr : &*current;
        }

        const value_type * next()
        {
            ++current;
            return current == container.end() ? nullptr : &*current;
        }

        std::size_t size() const { return container.size(); }
    };
}
