# Sequence tutorial

## Introduction

_Sequence_ is a C++ library for universal sequences. This has 2 main uses:

- Making it easier to pass lists into functions using a lightweight wrapper,
- Making it easier to manipulate and transform sequences.

## What is a sequence?

A _sequence_ is defined as zero or elements of the same type. A sequence can even be infinite. Sequences can be used in any situation where a list or collection is required, and replaces the array of alternatives with something simpler and more powerful.

Sequences can only be iterated in the forwards direction. They conform to C++ sequential containers, with an input iterator type.

Sequences are lightweight objects that wrap some underlying collection, for example using a pair of iterators. They are generally short-lived convenience objects, implementing an "adapter pattern" to provide a consistent interface. 

## Creating sequences

The `seq()` function is used to create sequences for a variety of situations. It returns a lightweight wrapper that constructs and returns an appropriate sequence object.

1. `seq<T>()` called with no arguments returns an empty sequence. In this case, you need to specify the type `T` of the sequence.

2. `seq(const Container & c)` returns a sequence of the contents of `c`. By default, it will create a sequence that iterates from `c.begin()` to `c.end()`.

3. `seq(const T* array, int size)`, `seq(const T (&array[Size])` returns a sequence of the array contents.

4. `seq(const char* str)` returns the sequence of characters in the null-terminated C-style string `str`.

5. `seq(int a, int b)` returns a sequence of integers, in the inclusive range a-b.

Since the underlying collection could be a temporary object itself, care must be taken to avoid problems like dangling references or invalid iterators.

Lists are created using the `list()` function.  A list is just another type of sequence

```c++
    std::vector<std::string> params;

    init(seq(params));
```

Examples:

```c++
    auto numbers = list(1,2,3);
    auto animals = list("dog", "cat");
```

Other sequence can be created by transforming an existing sequence, using the variety of methods on a sequence such as:

* `take`, `skip`, `where`, `select`, `take_until`, `as`

## Using sequences

Sequences are used mostly like normal C++ containers, so can for example just be iterated:

```c++
    for(auto animal : list("dog", "cat"))
        ...
```

You can find the size of a list using the `size()` method - beware that this can be an O(n) operation.

## Passing sequences

Object lifetime

Thread safety


## Manipulating sequences