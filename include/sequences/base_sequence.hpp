#include <iterator>
#include <stdexcept>

namespace sequences
{
    template<typename T, typename Derived, typename Stored=Derived>
    class base_sequence
    {
    public:
        typedef std::size_t size_type;
        typedef T value_type;

        Derived & self() const { return *const_cast<Derived*>(static_cast<const Derived*>(this)); }

        virtual_sequence<T, Stored> make_virtual() const { return {self()}; }

        operator virtual_sequence<T, Stored>() const { return make_virtual(); }

        size_type size() const
        {
            size_type c=0;
            for(auto i=self().first(); i; i=self().next()) ++c;
            return c;
        }

        struct iterator
        {
            Derived & underlying;
            const value_type * current;

            typedef value_type value_type;
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

        iterator begin() const { return { self(), self().first() }; }
        iterator end() const { return { self(), nullptr }; }

        template<typename Predicate>
        where_sequence<T, Stored, Predicate> where(Predicate p) const
        {
            return { self(), p };
        }

        template<typename Fn>
        select_sequence<T, Stored, Fn> select(Fn fn) const
        {
            return { self(), fn };
        }

        take_sequence<T, Stored> take(int n) const
        {
            return {self(),n};
        }

        skip_sequence<T, Stored> skip(int n) const
        {
            return {self(),n};
        }

        template<typename Aggregate>
        T aggregate(Aggregate agg, T result = {}) const
        {
            for(const value_type * i=self().first(); i; i=self().next())
                result = agg(result, *i);
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

        // TODO: Rename this
        const value_type &first2() const
        {
            auto c = self().first();
            if(!c) throw std::out_of_range("first() called on an empty list");
            return *c;
        }

        const value_type &last() const
        {
            for(auto c = self().first(); c;)
            {
                auto c2 = self().next();
                if(!c2) return *c;
                c = c2;
            }
            throw std::out_of_range("last() called on an empty list");
        }

        // Comparisons

        template<typename T2, typename Derived2,typename Stored2>
        bool operator==(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return equals(other);
        }

        template<typename T2, typename Derived2,typename Stored2>
        bool operator<(const base_sequence<T2,Derived2,Stored2> & other) const
        {
            return lexographical_compare(other);
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

            return !i1;
        }

        template<typename U>
        auto as() const
        {
            return select([](const T&t) { return U{t}; });
        }

        // typedef Derived stored_type;

        bool any() const { return self().first(); }

        template<typename Predicate>
        bool any(Predicate p) const { return where(p).any(); }
    };
}
