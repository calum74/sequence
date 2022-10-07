// Implements a sequence that splits a stream of characters into tokens

namespace sequences
{
    template<typename Seq>
    class split_sequence : public base_sequence<std::basic_string<typename Seq::value_type>, split_sequence<Seq>>
    {
        Seq seq;
        const typename Seq::value_type * splitChars;
        typedef typename Seq::value_type char_type;
    public:
        typedef std::basic_string<typename Seq::value_type> value_type;

        split_sequence(const Seq & seq, const char_type * chs) : seq(seq), splitChars(chs) {}

        bool eof;
        value_type token;

        bool isSplit(char_type ch) const
        {
            for(auto i=splitChars; *i; i++)
                if(ch == *i) return true;
            return false;
        }

        const value_type * first()
        {
            token.clear();
            eof = false;

            for(auto ch = seq.first(); ch; ch = seq.next())
            {
                if(isSplit(*ch))
                {
                    if(!token.empty())
                        return &token;
                }
                else
                    token += *ch;
            }
            eof = true;
            return token.empty() ? nullptr : &token;
        }

        const value_type * next()
        {
            if(eof) return nullptr;
            token.clear();
            const char_type * ch;
            do
            {
                ch = seq.next();
                if(ch)
                {
                    if(isSplit(*ch))
                    {
                        if(!token.empty())
                            return &token;
                    }
                    else
                        token += *ch;
                }
            }
            while(ch);
            eof = true;
            return token.empty() ? nullptr : &token;
        }
    };
}
