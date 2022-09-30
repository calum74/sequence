// This example shows some code using templates instead of
// sequence<T> and output_sequence<T>
// Do this if you really really care about performance
// (don't do this).

#include <sequence.hpp>
#include <iostream>

// Passing a sequence to a function
// You can use typename = typename Seq::is_sequence to ensure that you are handling
// a sequence and nothing else. This function operates on the underlying type of the sequence
// which could be more efficient.
template<typename Seq, typename = typename Seq::is_sequence>
void setItems(Seq s)
{
    for(auto i : s)
        std::cout << "Hello " << i << std::endl;
}

// Passing an output sequence to a function
// You can use typename = typename Seq::is_output_sequence to ensure that you are handling
// an output sequence and nothing else. This function operates on the underlying type of the sequence
// which could be more efficient.
template<typename Seq, typename = typename Seq::is_output_sequence>
void getItems(Seq s)
{
    s << list("a", "b", "c");
}

int main(int argc, char**argv)
{
    setItems(seq(argv, argc).skip(1));
    getItems(receiver([](const char * str) { std::cout << "Got " << str << std::endl;}));
    return 0;
}
