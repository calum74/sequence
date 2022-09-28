#pragma once

namespace sequences
{
    template<typename T, typename Seq, typename Predicate>
    class where_sequence;

    template<typename T, typename Seq, typename Fn>
    class select_sequence;

    template<typename T, typename Seq>
    class take_sequence;

    template<typename T, typename Seq>
    class skip_sequence;

    template<typename T, typename Seq>
    class virtual_sequence;

    template<typename T, typename Seq1, typename Seq2>
    class concat_sequence;

    template<typename T>
    class sequence_ref;

    template<typename T>
    class empty_sequence;

    template<typename T>
    class singleton_sequence;
}
