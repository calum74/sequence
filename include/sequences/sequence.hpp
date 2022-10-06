// Implements a generic sequence using virtual functions
template<typename T>
class sequence : public sequences::base_sequence<T, sequence<T>, sequences::sequence_ref<T>>
{
public:
    typedef T value_type;
    virtual const value_type * first() =0;
    virtual const value_type * next() =0;
    virtual std::size_t size() const =0;
};
