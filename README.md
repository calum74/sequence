# Sequence

Universal sequences in C++

## Overview

_Sequence_ provides a simple, efficient and uniform way to create, pass and manipulate sequences in C++. It aims to do two things:

- Provide a unified & lightweight abstraction for creating and passing sequences
- Provide a [LINQ](https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/linq/)-style library for transforming and manipulating sequences

It was motivated by the very basic frustration of "how do I actually pass a list to a function". Unfortunately, C++ provides far too many options, and they all have their drawbacks.

To solve this, we introduce a universal sequence type, `sequence<T>` that represents any sequence of type `T`. `sequence<T>` unifies the many abstractions that exist in C++, such as: iterator-pairs, ranges, C strings, arrays, pointers, containers, initializer lists, varargs and variadic templates. _Sequence_ wraps all of these in a unified abstraction.

As a motivating example, consider an `init()` function to initialize some code using a list of options. It could be defined in any number of ways such as

```c++
    void init(const std::initializer_list<const char*> & items);
    template<typename It> void init(It a, It b);
    void init(const char **a, const char**b);
    void init(const char**items, int length);
    void init(int length, const char*items ...);
    void init(const std::vector<const char*> & items);
```
The only generic approach, using a pair of iterators, unfortunately requires a template implemented in a header file.

_Sequence_ replaces all of these unsatisfactory options with the much simpler

```c++
    void init(const sequence<const char*> & items);
```

that can be implemented in a `.cpp` file. We can then create a sequence (which is a thin wrapper) using the `seq()` and `list()` functions, and pass it to `init()` in a uniform way.

```c++
    const char * items[] = { "foo.c", "bar.c" };
    std::vector<std::string> vec;

    init(list("foo.c", "bar.c"));
    init(seq(argv, argc));
    init(seq(items));
    init(seq(vec));
```

We can create "computed sequences", for example

```c++
    init(seq(argv, argc).take_while([](const char * str) { return strcmp(str, "--")!=0; }));
```

passes in only the parameters before the `--` parameter.

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

Using sequences, we can instead use `where` and `select` to processes the sequence using lambdas.

```c++
    void init(const std::sequence<const char*> & params)
    {
        auto options = params.
            where ([](const char * arg) { return arg[0]=='-'; }).
            select([](const char * arg) { return arg+1; });

        for(auto p : options)
            handleOption(p);
    }
```

Granted, the `sequence` version is actually a little longer, but I think sequences are a little safer and more high level. 

This next example counts the number of spaces in a string using regular C++.
```c++
    int count=0;
    for(const char * ch = "The quick brown fox"; *ch; ++ch)
        if(ch==' ') count++;
```

Using sequences, we can use the `count()` method, specifying a predicate.

```c++
    auto count = seq("The quick brown fox").count([](char x) { return x==' '; });
```

As a final example, here's a sequence of prime numbers up to one thousand. Firstly using regular C++,

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
        return !seq(2,n-1).any([=](int m) { return n%m==0; });
    });
```

I think this is better. Not only is the code shorter, but the values are computed lazily and there is no need to store the results in a temporary vector.

## Installation

_Sequence_ is a header-only C++ library, so it can just be copied into your project, or installed via a submodule. For example

```bash
git submodule add https://github.com/calum74/sequence.git
```

The header files are in the `include` directory.

## Tests

You can build and run the tests in the `tests` directory using CMake.

```bash
git clone https://github.com/calum74/sequence.git
cd sequence
mkdir build
cd build
cmake ..
make all test
```

