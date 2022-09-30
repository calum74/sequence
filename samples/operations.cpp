// This example demonstrates the various operations
// that can be performed on sequences

#include <sequence.hpp>
#include <iostream>

int main()
{
    auto s = list("a", "b", "c");
    auto e = seq<const char*>();

    // front() gets the first element of a list
    std::cout << s.front() << std::endl;

    // back() gets the last element in the list
    std::cout << s.back() << std::endl;

    // at() gets a given element in the list
    std::cout << s.at(1) << std::endl;

    std::cout << e.front_or_default("<empty>") << std::endl;
    std::cout << e.back_or_default("<empty>") << std::endl;
    std::cout << s.at_or_default(100, "<empty>") << std::endl;

    // any() indicates if there are any elements in the sequence
    std::cout << s.any() << e.any() << std::endl;

    // any(p) indicates if there are any items that match the given predicate
    std::cout << s.any([](const char * str) { return std::strcmp(str, "b")==0; }) << std::endl;

    // empty() is the opposite of any()
    std::cout << s.empty() << e.empty() << std::endl;

    // size() gives the number of elements in the list
    std::cout << s.size() << std::endl;

    // count(p) counts the number of elements matching the given predicate
    std::cout << s.count([](const char * str) { return std::strcmp(str, "b")==0; }) << std::endl;

    return 0;
}