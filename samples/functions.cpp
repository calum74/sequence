// This example demonstrates how to pass sequences to functions.

// In the .cpp file itself, you still need to include the full library
#include <sequence.hpp>
#include <iostream>
#include "functions.hpp"

// This is a regular function that processes a sequence.
void process1(const sequence<int> & items)
{
    std::cout << "The sum is " << items.sum() << std::endl;
}

// pointer_sequence<> is ever so slightly faster than sequence<>
// as it assumes that the underlying data is stored in an array. 
void process2(const pointer_sequence<int> & items)
{
    process1(items);
}

int main()
{
    process1(list(1,2,3));
    process2(list(4,5,6));
    return 0;
}
