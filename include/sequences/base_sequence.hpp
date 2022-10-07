namespace sequences
{
    // The base class of all sequences
    // This implements most of the sequence functionality.
    //
    // T is the element type of the sequence
    // Derived is the derived class - the actual class being implemented
    // Stored is how to store this sequence if copied to another sequence
    template<typename T, typename Derived, typename Stored=Derived>
    class base_sequence
    {
    public:
        typedef std::size_t size_type;
        typedef T value_type;

        // A tag to enable functions to check if the type they are dealing with is a sequence
        typedef void is_sequence;

        // How this sequence should be stored
        typedef Stored stored_type;

        // Obtain a non-const reference to the derived class.
        // All of the const methods basically lie as they do actually modify some internal state.
        Derived & self() const { return *const_cast<Derived*>(static_cast<const Derived*>(this)); }

        // Create a virtual sequence from this sequence - one enumerated via virtual functions
        virtual_sequence<T, Stored> make_virtual() const { return {self()}; }

        operator virtual_sequence<T, Stored>() const { return make_virtual(); }

        // The default size() computation is O(n) but it can be overridden in
        // derived classes for an O(1) implementation
        size_type size() const
        {
            size_type c=0;
            for(auto i=self().first(); i; i=self().next()) ++c;
            return c;
        }

        struct iterator
        {
            typedef T value_type;
            Derived & underlying;
            const value_type * current;

            typedef value_type & reference;
            typedef value_type & pointer;
            typedef std::size_t size_type;
            typedef int difference_type;
            typedef std::input_iterator_tag iterator_category;

            const value_type & operator*() const { return *current; }

            iterator & operator++()
            {
                current = underlying.next();
                return *this;
            }

            bool operator!=(iterator other) const { return current!=other.current; }
        };

        typedef iterator const_iterator;

        iterator begin() const { return {self(), self().first()}; }

        iterator end() const { return {self(), nullptr}; }

        const_iterator cbegin() const { return begin(); }

        const_iterator cend() const { return end(); }

        template<typename Predicate>
        where_sequence<T, Stored, Predicate> where(Predicate p) const
        {
            return {self(), p};
        }

        template<typename Fn>
        select_sequence<T, Stored, Fn> select(Fn fn) const
        {
            return {self(), fn};
        }

        take_sequence<T, Stored> take(int n) const
        {
            return {self(), n};
        }

        skip_sequence<T, Stored> skip(int n) const
        {
            return {self(), n};
        }

        template<typename Aggregate>
        T aggregate(Aggregate agg) const
        {
            T result = {};
            for(const value_type * i=self().first(); i; i=self().next())
                result = agg(result, *i);
            return result;
        }

        template<typename Aggregate, typename U>
        U aggregate(U result, Aggregate agg) const
        {
            for(const value_type * i=self().first(); i; i=self().next())
                result = agg(result, *i);
            return result;
        }

        template<typename Aggregate, typename U>
        U accumulate(U result, Aggregate agg) const
        {
            for(const value_type * i=self().first(); i; i=self().next())
                agg(result, *i);
            return result;
        }

        T sum() const
        {
            return aggregate([](const T &i1, const T&i2) { return i1+i2; });
        }

        const T &at(size_type index) const
        {
            for(auto c = self().first(); c; c=self().next())
            {
                if(index--==0) return *c;
            }
            throw std::out_of_range("at() is out of range");
        }

        value_type at_or_default(size_type index, const value_type & value) const
        {
            for(auto c = self().first(); c; c=self().next())
            {
                if(index--==0) return *c;
            }
            return value;
        }

        const value_type &front() const
        {
            auto c = self().first();
            if(!c) throw std::out_of_range("front() called on an empty list");
            return *c;
        }

        const value_type &back() const
        {
            for(auto c = self().first(); c;)
            {
                auto c2 = self().next();
                if(!c2) return *c;
                c = c2;
            }
            throw std::out_of_range("back() called on an empty list");
        }

        template<typename T2, typename Derived2,typename Stored2>
        bool operator==(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return equals(other);
        }

        template<typename T2, typename Derived2,typename Stored2>
        bool operator!=(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return !(*this == other);
        }

        template<typename T2, typename Derived2,typename Stored2>
        bool operator<(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return lexographical_compare(other);
        }

        template<typename T2, typename Derived2,typename Stored2>
        bool operator>(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return other<*this;
        }

        template<typename T2, typename Derived2,typename Stored2>
        bool operator<=(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return !(other<*this);
        }

        template<typename T2, typename Derived2,typename Stored2>
        bool operator>=(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return !(*this<other);
        }

        template<typename T2, typename Derived2, typename Stored2, typename Eq = std::equal_to<T>>
        bool equals(const base_sequence<T2,Derived2,Stored2> & other, Eq eq = {}) const
        {
            auto i1 = self().first();
            auto i2 = other.self().first();

            while(i1 && i2)
            {
                if(!eq(*i1, *i2)) return false;
                i1 = self().next();
                i2 = other.self().next();
            }

            return !i1 && !i2;
        }

        template<typename T2, typename Derived2, typename Stored2,typename Less = std::less<T>>
        bool lexographical_compare(const base_sequence<T2,Derived2,Stored2> & other, Less lt = {}) const
        {
            auto i1 = self().first();
            auto i2 = other.self().first();

            while(i1 && i2)
            {
                if(lt(*i1, *i2)) return true;
                if(lt(*i2, *i1)) return false;
                i1 = self().next();
                i2 = other.self().next();
            }

            return i2;
        }

    private:
        // Helper function to convert sequence to a different type
        template<typename U>
        struct asFn
        {
            U operator()(const T &t) const { return {t}; }
        };

    public:

        template<typename U>
        select_sequence<T, Derived, asFn<U>> as() const
        {
            return {self(), {}};
        }

        select_sequence<T, Derived, helpers::project_first<value_type>> keys() const
        {
            return {self(), {}};
        }
        
        select_sequence<T, Derived, helpers::project_second<value_type>> values() const
        {
            return {self(), {}};
        }

        bool any() const { return self().first(); }

        template<typename Predicate>
        bool any(Predicate p) const { return where(p).any(); }

        bool empty() const { return !any(); }

        // Return the first item, or 'value'.
        // Returns by value (not by reference) to avoid dangers of dangling references.
        value_type front_or_default(const T & value) const
        {
            auto c = self().first();
            if(!c) return value;
            return *c;            
        }

        // Return the last item, or 'value'.
        // Returns by value (not by reference) to avoid dangers of dangling references.
        value_type back_or_default(const value_type & value) const
        {
            for(auto c = self().first(); c;)
            {
                auto c2 = self().next();
                if(!c2) return *c;
                c = c2;
            }
            return value;
        }

        template<typename Predicate>
        size_type count(Predicate p)
        {
            return where(p).size();
        }

        template<typename Seq2, typename Fn, typename = typename Seq2::is_sequence>
        merge_sequence<Stored, typename Seq2::stored_type, Fn> merge(Seq2 seq2, Fn fn) const
        {
            return {self(), seq2, fn};
        }

        template<typename Predicate>
        take_while_sequence<Stored, Predicate> take_while(Predicate p) const
        {
            return {self(), p};
        }

        template<typename Predicate>
        skip_until_sequence<Stored, Predicate> skip_until(Predicate p) const
        {
            return {self(), p};
        }

        template<typename Seq2, typename = typename Seq2::is_sequence>
        concat_sequence<stored_type, typename Seq2::stored_type> concat(Seq2 seq2) const
        {
            return {self(), seq2};
        }

        template<typename Seq2, typename = typename Seq2::is_sequence>
        concat_sequence<stored_type, typename Seq2::stored_type> operator+(Seq2 seq2) const
        {
            return {self(), seq2};
        }

        // Writes the sequence to the output sequence
        template<typename U>
        void write_to(const output_sequence<U> & out) const
        {
            for(auto &i: *this) out.add(i);
        }

        // Writes the sequence to the container
        template<typename Container>
        void write_to(Container &c) const
        {
            for(auto &i: *this) c.insert(c.end(), i);
        }

        // Creates a container containing the elements of the sequence
        template<typename Container>
        Container make()
        {
            return {begin(), end()};
        }

        repeat_sequence<Stored> repeat(int n) const
        {
            return {self(), n};
        }

        split_sequence<Stored> split(const T * splitChars) const
        {
            return {self(), splitChars};
        }
    };
}
