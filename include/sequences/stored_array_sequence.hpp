namespace sequences
{
    template<typename T, int Size>
    class stored_array_sequence : public base_sequence<T, stored_array_sequence<T, Size>>
    {
    public:
        T items[Size];
    public:

        typedef T value_type;

        stored_array_sequence(const T(&input)[Size])
        {
            for(int i=0; i<Size; i++)
                items[i] = input[i];
        }

        template<typename...Ts>
        stored_array_sequence(Ts...ts)
        {
            init(0, ts...);
        }

        void init(int i) {}

        template<typename...Ts>
        void init(int i, T t, Ts... ts)
        {
            items[i] = t;
            init(i+1, ts...);
        }

        const T * current;

        const value_type * first() {
            current = items;
            return Size==0 ? nullptr : current;
        }

        const value_type * next()
        {
            current++;
            return current == items+Size ? nullptr : current;
        }
    };
}
