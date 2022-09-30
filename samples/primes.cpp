#include <sequence.hpp>
#include <iostream>

int main()
{
    auto primes = seq(2,1000).where([](int n) {
        return !seq(2,n-1).any([=](int m) { return n%m==0; });
    });

    for(int p : primes) std::cout << p << std::endl;

    return 0;
}
