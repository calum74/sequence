namespace sequences
{
    template<typename Container>
    class stored_sequence : public base_sequence<typename Container::value_type, stored_sequence<Container>>
    {
        Container container;
        typedef typename Container::value_type value_type;
        typename Container::const_iterator current;
    public:
        stored_sequence(const Container & c) : container(c) {}

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
    };
}
