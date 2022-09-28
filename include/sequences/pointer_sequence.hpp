
template<typename T>
class pointer_sequence : public sequences::base_sequence<T, pointer_sequence<T>>
{
    const T *a, *b, *current;
public:
    pointer_sequence(const T * a, const T *b) : a(a), b(b) {}

    template<int Size>
    pointer_sequence(const sequences::stored_array_sequence<T, Size> & seq) : a(seq.items), b(a+Size) {}

    const T * first()
    { 
        current = a;
        return current==b ? nullptr : current;
    }

    const T * next()
    {
        return ++current==b ? nullptr : current;
    }
};
