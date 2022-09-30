// This example demonstrates output sequences

#include <sequence.hpp>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <list>

void moreItems(const output_sequence<std::string> & output)
{
    output << "Item 7";
}

// This is a function that returns a sequence
void getItems(const output_sequence<std::string> & output)
{
    // Call the add() function to output a single element
    output.add("Item 1");

    // You can also use the << operator
    output << "Item 2";

    // You can send a sequence to an output
    list("Item 3", "Item 4","Item 5").write_to(output);

    // You can also send sequences via the << operator
    output << list("Item 6");

    // You can build up the output over several functions
    moreItems(output);
}

int main()
{
    std::vector<std::string> vec;
    std::set<std::string> set;
    std::list<std::string> list;

    // writer() creates a writer that adds the items to a container
    getItems(writer(vec));

    // It doesn't matter what container you use
    getItems(writer(set));
    getItems(writer(list));

    // receiver() specifies a function that will receive the itens
    getItems(receiver([](const std::string & str) { std::cout << str << std::endl; }));

    return 0;
}
