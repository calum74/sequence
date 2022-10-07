namespace sequences
{
    // Wraps an int into an iterator, for use in a range.
    class int_iterator
    {
        int value;
    public:
        int_iterator(int value) : value(value) {}
        typedef int value_type;
        typedef int difference_type;
        typedef int * pointer;
        typedef int & reference;
        typedef std::random_access_iterator_tag iterator_category;
        const int &operator*() const { return value; }
        int_iterator & operator++() { ++value; return *this; }
        bool operator!=(const int_iterator & other) const { return value != other.value; }

        int operator-(int_iterator other) const { return value - other.value; }
    };
}
