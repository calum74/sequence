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

int do_benchmark2()
{
    return seq(0, N).
        where([](int n) { return n%2==0; }).
        select([](int n) { return n*n; }).
        sum();
}

template<typename Fn>
void benchmark(Fn fn, const char * description)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int sum = fn();
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << description << ": " << std::chrono::duration<double, std::milli>(t2-t1).count() << " ms" << std::endl;
}

int main()
{
#ifndef NDEBUG
    std::cout << "WARNING!!! Running in a debug build\n";
#endif
    benchmark(do_benchmark1, "1a: C sequence");
    benchmark(do_benchmark2, "1b: Sequence  ");

    return 0;
}
