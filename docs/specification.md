# Sequence reference manual

!!! This is work in progress !!!

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




# `class sequence<T>`

Containers, iterators, thread safety

`bool sequence<T>::any()`


`template<typename Predicate> bool sequence<T>::any(Predicate p)`


## Implementing new sequences

