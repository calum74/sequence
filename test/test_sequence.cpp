// This file forms the test suite for the Sequence library
// It also contains lots of examples used throughout the documentation.

// This is the main header file to include, which includes everything
// You can also #include <sequence_fwd.hpp> if you just need the forward declaration.

#include <sequence.hpp>

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <future>

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

// Helper to validate object lifetimes.
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
    testVector2(list(Element(), Element(), Element(), Element()));
    testVector(seq({Element(), Element(), Element(), Element()}));

    // Here be dragons - we could be creating a dangling reference to a temporary object
    // If in doubt, don't store temporary variables in auto variables.
    auto tmp = seq(getVector());

    for(auto i : seq(getVector()))
        assert(i.valid());   

    testVector(tmp);
}

void copy(const sequence<const char*> & input, const output_sequence<const char*> & output)
{
    output << input;
}

void test_writers()
{
    std::vector<const char *> vec;

    copy(list("a", "b"), writer(vec));
    assert(seq(vec).as<std::string>() == list("a", "b"));

    copy(list("writer1","writer2"), receiver([](const char* str){std::cout << str << std::endl;}));
    copy(list("writer1","writer2"), receiver([&](const char* str){vec.push_back(str);}));

     copy(list("writer1","writer2"), receiver([&](const char * str){vec.push_back(str);}));
}

void test_repeat()
{
    assert(list(1,1,1)==list(1).repeat(3));
    assert(list(1,2)==list(1,2).repeat(1));
    assert(list(1,2,1,2)==list(1,2).repeat(2));
    assert(list<int>()==list(1).repeat(0));
    assert(list<int>()==list<int>().repeat(2));
    assert(list<int>()==list<int>().repeat(0));

    // Invalid input
    assert(list('a').repeat(-1) == list<char>());
}

void test_take()
{
    assert(list(1,2,3,4).take(-1)==list<int>());
    assert(list(1,2,3,4).take(0)==list<int>());
    assert(list(1,2,3,4).take(1)==list<int>(1));
    assert(list(1,2,3,4).take(2)==list<int>(1,2));
    assert(list(1,2,3,4).take(3)==list<int>(1,2,3));
    assert(list(1,2,3,4).take(4)==list<int>(1,2,3,4));
    assert(list(1,2,3,4).take(5)==list<int>(1,2,3,4));
}

void test_skip()
{
    assert(list(1,2,3,4).skip(-1)==list<int>(1,2,3,4));
    assert(list(1,2,3,4).skip(0)==list<int>(1,2,3,4));
    assert(list(1,2,3,4).skip(1)==list<int>(2,3,4));
    assert(list(1,2,3,4).skip(2)==list<int>(3,4));
    assert(list(1,2,3,4).skip(3)==list<int>(4));
    assert(list(1,2,3,4).skip(4)==list<int>());
    assert(list(1,2,3,4).skip(5)==list<int>());
}

void test_take_while()
{
    assert(list(1,2,3,4).take_while([](int x) { return false; }) == list<int>());
    assert(list(1,2,3,4,1).take_while([](int x) { return x<=3; }) == list(1,2,3));
    assert(list(1,2,3,4).take_while([](int x) { return true; }) == list(1,2,3,4));
}

void test_skip_until()
{
    assert(list(1,2,3,4).skip_until([](int x) { return false; }) == list<int>());
    assert(list(1,2,3,4,1).skip_until([](int x) { return x>=3; }) == list(3,4,1));
    assert(list(1,2,3,4).skip_until([](int x) { return true; }) == list(1,2,3,4));
}

void test_files()
{
    std::stringstream ss("abc\ndef\r\n   ghi   \n\n");
    assert(seq(ss).split("\r\n") == list("abc","def","   ghi   "));
}

// This function should return 0, but actually does not because
// sum() is iterating over the same sequence concurrently which is
// undefined.
int computeAsyncUnsafe(const sequence<int> & values) {
    auto f1 = std::async(std::launch::async, [&]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [&]() { return values.sum(); });
    return f1.get() - f2.get();
}

// A safe version of computeAsyncUnsafe(), ensuring that the sequence
// is copied by value.
template<typename Seq>
int computeAsyncSafe1(const Seq & values) {
    auto f1 = std::async(std::launch::async, [=]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [=]() { return values.sum(); });
    return f1.get() - f2.get();
}

// A safe version of computeAsyncUnsafe(), ensuring that the sequence
// is copied by value.
int computeAsyncSafe2(const pointer_sequence<int> & values) {
    auto f1 = std::async(std::launch::async, [=]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [=]() { return values.sum(); });
    return f1.get() - f2.get();
}

void test_async()
{
    // This hangs so don't call it.
    // std::cout << computeAsyncUnsafe(seq(1,10000000)) << std::endl;
    std::cout << computeAsyncSafe1(seq(1,10000000)) << std::endl;

    auto values = seq(1,1000000).make<std::vector<int>>();
    std::cout << computeAsyncSafe1(seq(values)) << std::endl;
}

void test_range()
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
}

void test_comparisons()
{
    auto e = list<int>();

    // Equality
    assert(e==e);
    assert(list(1)==list(1));
    assert(list(1,2)==list(1,2));

    // !=
    assert(e!=list(1));
    assert(list(1)!=e);
    assert(list(1)!=list(2));
    assert(list(1)!=list(1,2));

    // <
    assert(e < list(1));
    assert(list(1) < list(1,2));
    assert(list(1) < list(2));

    // <=
    assert(e<=e);
    assert(e<=list(1));
    assert(list(1)<=list(1));
    assert(list(1)<=list(1,2));
    assert(list(1)<=list(2));
    assert(list(1,2)<=list(1,2));
    assert(list(1,2)<=list(1,3));
    assert(list(1,2)<=list(2,1));

    // >
    assert(list(1) > e);
    assert(list(1,2) > list(1));
    assert(list(2) > list(1));

    // >=
    assert(e>=e);
    assert(list(1)>=e);
    assert(list(1)>=list(1));
    assert(list(1,2)>=list(1));
    assert(list(2)>=list(1));
    assert(list(1,2)>=list(1,2));
    assert(list(1,3)>=list(1,2));
    assert(list(2,1)>=list(1,2));
}

void test_single()
{
    assert(single(3)==list(3));
}

void test_list()
{
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
}

void test_primes()
{
    auto primes = seq(2,1000).where([](int n) {
        return !seq(2,n-1).any([=](int m) { return n%m==0; });
    });

    assert(primes.take(5) == list(2,3,5,7,11));

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
}

void test_generator()
{
    // Generated sequence
    int i=0;
    auto g = generator([&](int &x) { x=0; i=1; return true; }, [&](int &x) { x = i; return i++<10; });
    assert(g == seq(0, 9));
}

void test_keys_and_values()
{
    std::map<std::string, int> map1 = { {"a",1}, {"b",2}, {"c",3}};

    assert(seq(map1).keys() == list("a","b","c"));
    assert(seq(map1).values() == list(1,2,3));

    assert(seq(map1).keys().merge(seq(map1).values(), [](const std::string & str, int i) { return std::make_pair(str,i);}) == seq(map1));
}

void test_merge()
{
    auto sum = [](int a, int b) { return a+b; };
    auto e = list<int>();

    assert(list(1,2).merge(list(3,4), sum) == list(4,6));
    assert(e.merge(list(3,4), sum) == e);
    assert(list(1,2).merge(e, sum) == e);

    assert(list(1).merge(list(3,4), sum) == list(4));
    assert(list(1,2).merge(list(3), sum) == list(4));
}

void test_sum()
{
    assert(list<int>().sum()==0);
    assert(list(1).sum() == 1);
    assert(list(1,2,3).sum() == 6);

    assert(list<std::string>().sum() == "");
    assert(list<std::string>("a", "b", "c").sum() == "abc");

    assert(list(0.5, 1.5).sum() == 2.0);
}

void test_any()
{
    assert(!list<int>().any());
    assert(list<int>().empty());
    assert(list(1).any());
    assert(!list(1).empty());

    assert(!list(1,2).any([](int x) { return x==3; }));
    assert(list(1,2).any([](int x) { return x==2; }));
}

void test_count()
{
    assert(list<int>().count([](int x) { return x==1; })==0);
    assert(list<int>(2,3).count([](int x) { return x==1; })==0);
    assert(list<int>(1,2,3).count([](int x) { return x==1; })==1);
    assert(list<int>(1,2,3,1,1).count([](int x) { return x==1; })==3);
}

void test_aggregate()
{
    auto sum = [](int a, int b) { return a+b; };

    assert(list<int>().aggregate(0, sum)==0);
    assert(list<int>().aggregate(1, sum)==1);
    assert(list(3,4,5).aggregate(2, sum)==14);
    assert(list(3,4,5).aggregate(sum)==12);
    assert(list(3,4,5).aggregate(std::string(), [](const std::string & str, int n) { return str+'x'; })=="xxx");

}

void test_accumulate()
{
    assert(list<int>().accumulate(5, [](int & total, int n) { total+=n; })==5);
    assert(list(3,4,5).accumulate(5, [](int & total, int n) { total+=n; })==17);
    assert(list(3,4,5).accumulate(std::string(), [](std::string & str, int n) { str+='x'; })=="xxx");
}

int main()
{
    test_lifetimes();
    test_writers();
    test_range();
    test_comparisons();
    test_single();
    test_list();
    test_primes();
    test_keys_and_values();
    test_repeat();
    test_files();
    test_async();
    test_take();
    test_skip();
    test_take_while();
    test_skip_until();
    test_merge();
    test_sum();
    test_any();
    test_count();
    test_aggregate();
    test_accumulate();
    return 0;
}