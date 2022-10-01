# Sequence

Universal sequences in C++

## Getting started

_Sequence_ is a header-only C++ library compatible with C++ 11 and higher. It has no other dependencies and should work fine on all platforms. Read the [tutorial] to learn how to use this library. [setup] gives further build instructions if needed.

## Overview

_Sequence_ provides a simple, efficient and uniform way to create, pass and manipulate sequences in C++. It aims to do two things:

- Provide a unified & lightweight abstraction for creating and passing sequences
- Provide a [LINQ](https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/linq/)-style library for transforming and manipulating sequences

It is motivated by the very basic frustration of "how do I actually pass a list to a function". Unfortunately, C++ provides far too many options, and they all have their drawbacks.

To solve this, we introduce a universal sequence type, `sequence<T>` that represents any sequence of type `T`. `sequence<T>` unifies the many abstractions that exist in C++, such as: iterator-pairs, ranges, C strings, arrays, pointers, containers, initializer lists, varargs and variadic templates. _Sequence_ wraps all of these in a unified abstraction.

### Example 1

The first example ([source code](../samples/example2.cpp)) shows processing of command-line arguments. It shows the creation of a sequence using `seq()`, and then the filtering and transformation of sequences using `where()`, `select()`, `skip()`, `as<>()`, `take_while()` and `skip_until()`. It also shows how sequences are passed into functions using `sequence<T>`.

```c++
// This example demonstrates processing command line arguments as a sequence of strings.

#include <sequence.hpp>
#include <iostream>

// A function that processes a sequence of options and files
void processFiles(const sequence<std::pair<char, std::string>> & options, const sequence<std::string> & files)
{
    // Extract all options of the form "-Ixxx" into a sequence
    auto includes = options.
        where ([](const std::pair<char, std::string> &option) { return option.first=='I'; }).
        select([](const std::pair<char, std::string> &option) { return option.second; });

    // Display all includes
    for(auto & include : includes)
        std::cout << "Include " << include << std::endl;

    // Display all files
    for(auto & file : files)
        std::cout << "Open file " << file << std::endl;
}

int main(int argc, char**argv)
{
    // Define a sequence that convert the arguments to a list of std::string,
    // skipping argv[0] which is the program itself
    auto arguments = seq(argv, argc).skip(1).as<std::string>();

    // Define the sequence of arguments before "--"
    auto beforeDashes = arguments.take_while([](const std::string & arg) { return arg != "--"; });

    // Define the sequence of arguments after "--"
    auto afterDashes = arguments.skip_until([](const std::string & arg) { return arg == "--"; }).skip(1);

    // Define sequence of options as anything before "--" that begins with a "-"
    // Convert each option into a pair of option name and value.
    auto options = beforeDashes.
        where ([](const std::string &arg) { return !arg.empty() && arg[0]=='-'; }).
        select([](const std::string &arg) { return std::make_pair(arg[1], arg.substr(2)); });

    // Define sequence of files as all non-option arguments + anything after "--"
    auto files = beforeDashes.where([](const std::string &arg) { return arg[0]!='-'; }) + afterDashes;

    processFiles(options, files);

    return 0;
}
```

### Example 2

The second example ([source code](../samples/primes.cpp)) shows a naive algorithm to produce a list of prime numbers. In regular C++ we would probably implement it like

```c++
    std::vector<int> primes;
    for(int n=2; n<=1000; ++n)
    {
        bool is_prime = true;
        for(int m=2; m<n; m++)
            if(n%m==0)
            {
                is_prime = false;
                break;
            }
        if(is_prime) primes.push_back(n);
    }
```

There's not really a convenient way to represent a stream in C++, so we'd probably elect to store the values in a `vector<int>`. Using `sequence`, this can be written as

```c++
    auto primes = seq(2,1000).where([](int n) {
        return !seq(2,n-1).any([=](int m) { return n%m==0; });
    });
```
