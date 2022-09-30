// This example demonstrates how to pass sequences to functions.

// You can include this header file if you only need
// the forward declarations (for example in a header file)
#include <sequence_fwd.hpp>

void process1(const sequence<int> & items);
void process2(const pointer_sequence<int> & items);

// In the .cpp file itself, you need to include the full library
#include <sequence.hpp>
#include <iostream>

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
