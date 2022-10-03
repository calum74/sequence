// This example demonstrates the various operations
// that can be performed on sequences

#include <sequence.hpp>
#include <iostream>
#include <vector>

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

    // Sequences can be compared for element-wise equality
    // It uses the default == operator on the element type, so in this example we'll first
    // convert the sequences to std::string for sensible results.
    std::cout << (s.as<std::string>() == list("a","b","c").as<std::string>()) << std::endl;

    // The equals() function allows you to specify a comparator function
    std::cout << s.equals(list("a", "b", "c"), [](const char * s1, const char *s2) { return std::strcmp(s1,s2)==0; }) << std::endl;

    // Other comparators work fine as well
    std::cout << (s.as<std::string>() < list("a","b","c").as<std::string>()) << std::endl;

    // lexographical_compare allows you to specify a comparator function explicitly
    std::cout << s.lexographical_compare(list("a", "b", "c"), [](const char * s1, const char *s2) { return std::strcmp(s1,s2)<0; }) << std::endl;

    // Write the contents to a container
    std::vector<const char *> items;
    s.write_to(items);

    // Create a vector
    auto vec = s.make<std::vector<const char*>>();

    // A simple hash computation of a list of integers.
    int hash = list(1,2,3).aggregate(0, [](int n1, int n2) { return n1*13 + n2; });

    return 0;
}