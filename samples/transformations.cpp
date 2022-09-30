// This example shows the various ways of transforming sequences

#include <sequence.hpp>
#include <iostream>

void print(const sequence<int> & s)
{
    for(auto i : s) std::cout << i;
    std::cout << std::endl;
}

int main()
{
    auto seq1 = seq(0,9);
    
    // where(p) filters the selection
    print(seq1.where([](int x) { return x%2==0; }));

    // select(p) transforms each element in the selection
    print(seq1.select([](int x) { return x*2; }));
    
    // Of course, you can combine transformations
    print(seq1.where([](int x) { return x%2==0; }).select([](int x) { return x*2; }));

    // The order in which transformations are applied matters!
    print(seq1.select([](int x) { return x*2; }).where([](int x) { return x%2==0; }));

    // take(n) limits the number of elements up to the given number
    print(seq1.take(3));

    // take_while(p) carries on taking elements until the condition is false
    print(seq1.take_while([](int n) { return n<=6; }));

    // concat(s) concatenates two sequences
    print(seq1.concat(seq1));

    // Also the + operator provides this
    print(seq1+seq(10,19));

    // merge(s,fn) merge two sequences, calling fn on each pair
//    print(seq1.merge(seq(10,19), [](int a, int b) { return a+b; }));

    return 0;
}