#include <sequence.hpp>
#include <chrono>
#include <iostream>

const int N=1000000000;

int do_benchmark1()
{
    int sum=0;
    for(int i=0; i<=N; i++)
        if(i%2==0)
            sum += i*i;
    return sum;
}

int benchmark1()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int sum=do_benchmark1();
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "C version completed in " << std::chrono::duration<double, std::milli>(t2-t1).count() << " ms" << std::endl;
    return sum;
}

int do_benchmark2()
{
    return seq(0, N).
        where([](int n) { return n%2==0; }).
        select([](int n) { return n*n; }).
        sum();
}

int benchmark2()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int sum = do_benchmark2();
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Sequence completed in " << std::chrono::duration<double, std::milli>(t2-t1).count() << " ms" << std::endl;
    return sum;
}

int main()
{
#ifndef NDEBUG
    std::cout << "WARNING!!! Running in a debug build\n";
#endif
    int sum1 = benchmark1();
    int sum2 = benchmark2();
    if(sum1!=sum2)
    {
        std::cout << "Error: Methods produced different results\n";
        return 1;
    }
    return 0;
}
