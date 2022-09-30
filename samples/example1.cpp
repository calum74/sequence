#include <sequence.hpp>
#include <iostream>

int main(int argc, char**argv)
{
    for(auto i : seq(argv, argc).skip(1))
        std::cout << "Hello " << i << std::endl;
    return 0;
}
