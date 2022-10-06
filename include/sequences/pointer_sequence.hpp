// Implements a pointer sequence - a sequence defined by a pair of pointers
// This is potentially more efficient since it does not use virtual functions.

template<typename T>
class pointer_sequence : public sequences::base_sequence<T, pointer_sequence<T>>
{
    const T *a, *b, *current;
public:
    pointer_sequence(const T * a, const T *b) : a(a), b(b) {}

    template<int Size>
    pointer_sequence(const sequences::stored_array_sequence<T, Size> & seq) : a(seq.items), b(a+Size) {}

    pointer_sequence(const sequences::empty_sequence<T>&) : a(nullptr), b(nullptr) {}

    pointer_sequence(const sequences::singleton_sequence<T> &s) : a(&s.item), b(1+&s.item) {}

    const T * first()
    { 
        current = a;
        return current==b ? nullptr : current;
    }

    const T * next()
    {
        return ++current==b ? nullptr : current;
    }

    std::size_t size() const { return b-a; }
};
