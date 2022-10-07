// Benchmarking Sequence
// This compares the performance of Sequence against hand-written code.
// We also look at the overhead of using `sequence<T>&`, which uses
// virtual function calls.
//
// Benchmark 1: This calculates the sum of the squares of all
// even numbers between 0 and 1_000_000_000.
//
// Benchmark 2: This constructs a string consisting of 1000000 'a's.

#include <sequence.hpp>
#include <chrono>
#include <iostream>

const int N=1000000000;

// This is the canonical C implementation of Benchmark 1.
// This is the fastest implementation.
int benchmark1a()
{
    int sum=0;
    for(int i=0; i<=N; i++)
        if(i%2==0)
            sum += i*i;
    return sum;
}

// This is a Sequence implementation of Benchmark 1.
// It is somewhat slower than the C implementation even though
// it is fully inlined. The compiler was not able to optimize this
// as much as the C implementation, so it is around 3 times slower.
int benchmark1b()
{
    return seq(0, N).
        where([](int n) { return n%2==0; }).
        select([](int n) { return n*n; }).
        sum();
}

int processInts(const sequence<int> & items)
{
    return items.where([](int n) { return n%2==0; }).
        select([](int n) { return n*n; }).
        sum();
}

template<typename Seq>
int processInts2(Seq items)
{
    return items.where([](int n) { return n%2==0; }).
        select([](int n) { return n*n; }).
        sum();
}

// This is the Sequence implementation of Benchmark 1, with the added
// overhead of iterating the sequence using virtual function calls which
// is around 10 times slower than the inline C implementation due to the
// overhead of virtual function calls used internally.
int benchmark1c()
{
    return processInts(seq(0,N));
}

// This is the Sequence implementation of Benchmark 1, using a templated
// function to process the list. This shows the potential speedup of using templates
// if you don't mind all your code in header files.
int benchmark1d()
{
    return processInts2(seq(0,N));
}

const int N2 = 1000000;

// This is the C implementation of Benchmark 2.
// It is the joint-fastest implementation.
int aloop1()
{
    std::string result;
    for(int i=0; i<N2; ++i)
        result += 'a';
    return (int)result.size();
}

// This is the Sequence implementation of Benchmark 2.
// It is joint-fastest with the C implementation.
// It uses `accumulate()` which is faster than `aggregate` in this case.
int aloop2()
{
    return (int)list('a').repeat(N2).accumulate(std::string(), [](std::string & str, char ch) { str+=ch; }).size();
}

// This is the Sequence implementation of Benchmark 2.
// It uses `aggregate` which is significantly slower than `accumulate` in this case.
int aloop3()
{
    return (int)list('a').repeat(N2).aggregate(std::string(), [](const std::string & str, char ch) { return str+ch; }).size();
}

int processAs(const sequence<char> & items)
{
    return (int)items.accumulate(std::string(), [](std::string & str, char ch) { str+=ch; }).size();
}

// This is a Sequence implementation of Benchmark 2, passing
// the sequence as `sequence<char>&` which introduces virtual function calls.
// This is around 13% slower than the inline version that does not use virtual function calls.
int aloop4()
{
    return processAs(list('a').repeat(N2));
}

template<typename Fn>
void benchmark(Fn fn, const char * description)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int sum = fn();
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << description << ", " << std::chrono::duration<double, std::milli>(t2-t1).count() << std::endl;
}

int main()
{
#ifndef NDEBUG
    std::cout << "WARNING!!! Running in a debug build\n";
#endif
    benchmark(benchmark1a, "1a: C implementation");
    benchmark(benchmark1b, "1b: Sequence implementation");
    benchmark(benchmark1c, "1c: Sequence passed as reference");
    benchmark(benchmark1d, "1d: Sequence passed as template");
    benchmark(aloop1,      "2a: C implementation");
    benchmark(aloop2,      "2b: Sequence implementation");
    benchmark(aloop3,      "2c: Sequence using naive aggregate");
    benchmark(aloop4,      "2d: Sequence passed as reference");
    return 0;
}
