# sequence

Universal sequences for C++.

## Overview

This project solves a few annoyances regarding collections in C++:

1. There are multiple and incompatible ways to specify collections of things in C++.
2. To make code generic you need to implement your code as templates in header files, which is just annoying.
3. Implement LINQ-style queries for efficient and concise code.
4. No simple numerical ranges

To solve this, we introduce a universal type, `sequence<T>` that represents any type of sequence. `sequence` unifies the plethora of abstractions that exist in C++, such as: iterator-pairs, ranges, C strings, arrays, pointers, containers, initializer lists, varargs and variadic templates.

Use cases for this are:

1. Any function that needs to be passed a list of some kind. `sequence` provides an efficient and universal abstraction that allows such a function to be implemented without needing templates or pointers.
2. To simplify code that needs to perform sequence operations.

A simple example of a function accepting a sequence of strings is:

```c++
    // Regular C++:
    void init(const std::initializer_list<const char*> & items);
    init({"foo.c", "bar.c"});  // Ok, but what if we want to switch to argv/argc?

    // Using sequences:
    void init(const std::sequence<const char*> & params);
    init(list("foo.c", "bar.c"));   // Constructs a list
    init(seq(argv, argc));          // This just works -- TODO: Test
```

Here we see the problem that we can't easily switch the signature of `init()` to accept a different type of array.

The next example shows the list processing of options, where we want to call `handleOption()` for every string beginning with `-`.

In regular C++, we would probably just loop over the array as follows:

```c++
    void init(char**argv, int argc)
    {
        for(int i=0; i<argc; i++)
            if(argv[i][0]=='-')
                handleOption(argv[i]+1);
    }
```

Using sequences, we can instead use `where`, `select` and `foreach` to processes the sequence using lambdas.

```c++
    void init(const std::sequence<const char*> & params)
    {
        params.where([](const char * str) { return str[0]=='-'; }).
            select([](const char * str) { return str+1; }).
            foreach([](const char * option) { handleOption(option); });
    }
```

The `sequence` version is actually a little longer, but mainly because we didn't fold `select` and `foreach` into one line. Sequences are I think a little safer and more high level. 

This next example counts the number of spaces in a string using regular C++.
```c++
    // Method 1
    int count=0;
    for(auto ch : std::string("The quick brown fox"))
        if(ch==' ') count++;

    // Method 2
    int count=0;
    for(const char * ch = "The quick brown fox"; *ch; ++ch)
        if(ch==' ') count++;
```

Using sequences, we can use the `count()` method, specifying a predicate.

```c++
    auto count = seq("The quick brown fox").count([](char x) { return x==' '; });
```

This example shows a rather simple loop:

```c++
    for(int i=1; i<=12; i++)
```

We can do the same using a sequence.

```c++
    for(int i : seq(1,12))
```

As a final example of list manipulation, here's a sequence of prime numbers up to one thousand. Firstly using regular C++

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

There's not really a convenient way to represent a stream in C++, so we'd probably elect to store the values in a `vector<int>`. Using `sequence`, this becomes

```c++
    auto primes = seq(2,1000).where([](int n) {
        for(int m : seq(2,n-1))
            if (n%m==0) return false;
        return true;
    });
```

I think this is a little better. Not only is the code shorter, but the values are computed lazily and there is no need to store the results in a vector.

## Installation

/Sequence/ is a header-only C++ library, so it can just be copied into your project, or installed via a submodule. For example

```bash
$ git submodule add ...
```

## Tests

## Benchmarks

## Thread safety

# Manual

## Header file

```c++
#include <sequence.hpp>
```

## Namespace

sequence is not in a namespace.

## Types

The library defines just one (abstract) type:

```
template<typename T> class sequence;
```

There are also various unspecified types that inherit from `sequence<T>`. These should be named anonymously (via `auto`) or by reference to `sequence<T>`.

For example

```c++
void init(const sequence<std::string> & params);
```

## Construction




## `class sequence<T>`

Containers, iterators, thread safety

## Implementing new sequences

