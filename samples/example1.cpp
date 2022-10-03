// This is a very simple example of some code using Sequence.

// The header file is "sequence.hpp"
// You can also include just "sequence_fwd.hpp" in header files to reduce the compilation size.
#include <sequence.hpp>

#include <iostream>

int main(int argc, const char**argv)
{
    // seq(argv, argc) creates a sequence that iterates over a C array
    // skip(1) skips the first element
    for(auto i : seq(argv, argc).skip(1))
        std::cout << "Hello " << i << std::endl;
    return 0;
}
