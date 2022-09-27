#include "sequence.hpp"

#include <iostream>
#include <vector>
#include <chrono>

#undef NDEBUG
#include <cassert>


void test_eq(const sequence<int> & s1, const sequence<int> &s2)
{
    assert(s1==s2);
}

void init(const sequence<std::string> & params)
{

}

const int N=1000000000;

int do_benchmark1()
{
    int sum=0;
    for(int i=0; i<=N; i++)
        if(i%2==0)
            sum += i*i;
    return sum;
}

void benchmark1()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int sum=do_benchmark1();
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << sum << " completed in " << std::chrono::duration<double, std::milli>(t2-t1).count() << " ms" << std::endl;
}

int do_benchmark2()
{
    return seq(0, N).
        where([](int n) { return n%2==0; }).
        select([](int n) { return n*n; }).sum();
}

void benchmark2()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    int sum = do_benchmark2();
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << sum << " completed in " << std::chrono::duration<double, std::milli>(t2-t1).count() << " ms" << std::endl;
}

int main()
{
    assert(seq(1,10).size()==10);
    assert(seq(1,10).where([](int x) { return x>4;}).size() == 6);
    assert(seq(1,10).select(
        [](int x) { return x+1; }
    ).size() == 10);

    assert(seq(1,10).select(
        [](int x) { return x+1; }
    ).where([](int x) { return x>4;}).size() == 7);

    // std::vector<int> x = seq(1,10).to_vec();

    assert(seq(1,10).take(2).size()==2);
    assert(seq(1,10).take(0).size()==0);

    assert(seq<int>().size()==0);
    
    // This has a bug in it!
    //for(auto x : seq(1,100).
    //    where([](int x) { return x%3!=0;}).
    //    select([](int x) { return "pika "; }))
    //    std::cout << x << std::endl;

    auto d = single(3);
    assert(d.size()==1);
    assert(list<int>().size()==0);

    auto e = list(1,2,3);
    assert(e.size()==3);
    for(int i : e)
        std::cout << i << std::endl;
    for(int i : e.where([](int x) { return x>1; }))
        std::cout << i << std::endl;

    for(int i : e.where([](int x) { return x>1; }).
        select([](int x) { return x*2; }))
        std::cout << i << std::endl;

    for(int i : list(1,2,3).where([](int x) { return x>1; }).
        select([](int x) { return x*2; }))
        std::cout << i << std::endl;

    for(int i : list(1,2,3))
        std::cout << i << std::endl;

    assert(list(1,2,3).sum() == 6);
    assert(list(1,2,3).select([](int x) { return x*2+1; }).sum() == 15);

    assert(list(1,2,3).first2()==1);
    assert(list(1,2,3).last()==3);
    assert(list(2,3,4).at(1)==3);
    assert(list(2,3,4).at(0)==2);
    assert(list(2,3,4).at(2)==4);

    assert(list(1,2,3)==list(1,2,3));
    assert(list(1,2)<list(1,2,3));

    test_eq(list(1,2,3), list(1,2,3));

    init(list<std::string>("a", "b", "c"));
    init(list("a","b","c").as<std::string>());

    auto primes = seq(2,1000).where([](int n) {
        for(int m : seq(2,n-1))
            if (n%m==0) return false;
        return true;
    });

    for(auto p : primes)
        std::cout << p << " ";
    std::cout << std::endl;

    std::vector<int> primes2;
    for(int n=2; n<=1000; ++n)
    {
        bool is_prime = true;
        for(int m=2; m<n; m++)
            if(n%m==0)
            {
                is_prime = false;
                break;
            }
        if(is_prime) primes2.push_back(n);
    }

    assert(seq(primes2) == primes);

    std::cout << "Running benchmarks...\n";
    benchmark1();
    benchmark2();

    return 0;
}