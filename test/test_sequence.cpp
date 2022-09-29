// This file forms the test suite for the Sequence library
// It also contains lots of examples used throughout the documentation.

// This is the main header file to include, which includes everything
// You can also #include <sequence_fwd.hpp> if you just need the forward declaration.

#include <sequence.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <map>

#undef NDEBUG
#include <cassert>


void test_eq(const sequence<int> & s1, const sequence<int> &s2)
{
    assert(s1==s2);
}

void handleOption(const char *opt) {
    std::cout << "Option is " << opt << std::endl;
}

// An example of a function that takes a sequence
void init(const pointer_sequence<const char *> & params)
{
    for(auto p : params.where([](const char * str) { return str[0]=='-'; }))
        handleOption(p+1);

    auto options = params.
        where ([](const char * arg) { return arg[0]=='-'; }).
        select([](const char * arg) { return arg+1; });

    for(auto p : options)
        handleOption(p);
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

class Element
{
    bool constructed;

public:
    Element() : constructed(true) {}
    ~Element() { constructed = false; assert(!valid()); }

    bool valid() const { return constructed; }
};

std::vector<Element> getVector()
{
    return std::vector<Element>{ 4 };
}

void testVector(const sequence<Element> & elements)
{
    assert(elements.size()==4);
    for(auto & i : elements)
        assert(i.valid());
}

void testVector2(const pointer_sequence<Element> & elements)
{
    assert(elements.size()==4);
    for(auto & i : elements)
        assert(i.valid());
}


void test_lifetimes()
{
    testVector(seq(getVector()));
    testVector(list(Element(), Element(), Element(), Element()));

    // testVector2(seq(getVector()));
    testVector2(list(Element(), Element(), Element(), Element()));

    testVector(seq({Element(), Element(), Element(), Element()}));

    // Here be dragons - we're creating a dangling reference to a temporary object
    // If in doubt, don't store temporary variables in auto variables.
    // auto tmp = seq(getVector());

    // This also breaks due to the dangling reference.
    for(auto i : seq(getVector()))
        assert(i.valid());   

    //testVector(tmp);
    //testVector2(tmp);
}

// template<typename T>
void copy(const sequence<const char*> & input, const output_sequence<const char*> & output)
{
    output << input;
}

void test_writers()
{
    std::vector<const char *> vec;

    copy(list("a", "b"), writer(vec));
    assert(seq(vec) == list("a", "b"));

    copy(list("writer1","writer2"), writer<const char*>([](auto str){std::cout << str << std::endl;}));
    copy(list("writer1","writer2"), writer<const char*>([&](auto str){vec.push_back(str);}));
}

int main()
{
    test_lifetimes();
    test_writers();
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

    assert(list(1,2,3).front()==1);
    assert(list(1,2,3).back()==3);
    assert(list(2,3,4).at(1)==3);
    assert(list(2,3,4).at(0)==2);
    assert(list(2,3,4).at(2)==4);

    assert(list(1,2,3)==list(1,2,3));
    assert(list(1,2)<list(1,2,3));

    test_eq(list(1,2,3), list(1,2,3));

    init(list("-a", "-b", "-c"));
    init(list("a","b","c"));

    auto primes = seq(2,1000).where([](int n) {
        return !seq(2,n-1).any([=](int m) { return n%m==0; });
    });

    // Example: iterate the keys or values in a map

    std::map<std::string, int> map1;
    assert(primes.any());

    // auto primes3 = seq(2,1000).where([=](int n) {
    //    return !primes3.take_while([=](int m) { return n<m; }).any([](int p) { return n%p==0; });

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

    // This should work!
    assert(seq(std::string("hello")).size()==5);
    assert(seq((const std::string&)std::string("hello")).size()==5);

    std::cout << "Running benchmarks...\n";
    benchmark1();
    benchmark2();

    return 0;
}