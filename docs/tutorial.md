# Sequence tutorial

## Introduction

_Sequence_ is a C++ library for universal sequences. This has two main uses:

- Making it easier to pass lists into functions using a lightweight wrapper,
- Making it easier to manipulate and transform sequences without sacrificing performance,
- Make it easier to return and visit sequences.

Sequences are heavily inspired by C# [`IEnumerable<>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerable-1) of course.

## What is a sequence?

A _sequence_ is defined as a set of ordered elements of the same type. A sequence can even be infinite. Sequences can be used in any situation where a list or collection is required, and replaces the various alternatives in C++ with something simpler and more powerful.

Sequences can only be iterated in the forwards direction. They are C++ sequential containers, with an input iterator type.

Sequences are lightweight objects that wrap some underlying collection, for example with a pair of iterators. They are generally short-lived convenience objects, implementing an "adapter pattern" to provide a simple and consistent interface over a variety of different implementations and scenarios.

Sequences are generally evaluated lazily, meaning that elements are processed one and a time and we rarely need to store the entire sequence. In this example, we do not actually create an array of 1 billion integers, or a filtered array of 500 million integers (the result of the `where` call). Instead the code just iterates as if it were a normal `for` loop:

```c++
    int count = seq(1,1000000000).where([](int n) { return n%2==0; }).size();
```
<details>
<summary>Expand for arm64 disassembly
</summary>

```
0000000100001f94 <__Z13do_benchmark2v>:
100001f94: 0b 00 80 52 	mov	w11, #0
100001f98: 09 00 80 52 	mov	w9, #0
100001f9c: 00 00 80 52 	mov	w0, #0
100001fa0: 28 40 99 52 	mov	w8, #51713
100001fa4: 48 73 a7 72 	movk	w8, #15258, lsl #16
100001fa8: 03 00 00 14 	b	0x100001fb4 <__Z13do_benchmark2v+0x20>
100001fac: 4b 7d 0a 1b 	mul	w11, w10, w10
100001fb0: e9 03 0a aa 	mov	x9, x10
100001fb4: 00 00 0b 0b 	add	w0, w0, w11
100001fb8: 2a 05 00 11 	add	w10, w9, #1
100001fbc: 5f 01 08 6b 	cmp	w10, w8
100001fc0: a0 00 00 54 	b.eq	0x100001fd4 <__Z13do_benchmark2v+0x40>
100001fc4: 4a ff 07 36 	tbz	w10, #0, 0x100001fac <__Z13do_benchmark2v+0x18>
100001fc8: 2a 09 00 11 	add	w10, w9, #2
100001fcc: 5f 01 08 6b 	cmp	w10, w8
100001fd0: e1 fe ff 54 	b.ne	0x100001fac <__Z13do_benchmark2v+0x18>
100001fd4: c0 03 5f d6 	ret
```
</details>

## Creating sequences

The `seq()` function is used to create sequences for a variety of situations. It returns a lightweight wrapper for an appropriate sequence object.

1. `seq<T>()` called with no arguments returns an empty sequence. In this case, you need to specify the type `T` of the sequence.

2. `seq(const Container & c)` returns a sequence of the contents of `c`. By default, it will create a sequence that iterates from `c.begin()` to `c.end()`.

3. `seq(const T* array, int size)`, `seq(const T (&array[Size])` returns a sequence of the array contents.

4. `seq(const char* str)` returns the sequence of characters in the null-terminated C-style string `str`.

5. `seq(int a, int b)` returns a sequence of integers, in the inclusive range a-b.

The return type of `seq( )` is unspecified, but it can be stored in an `auto` variable, iterated using a `for` loop, or passed to a function taking a `const sequence<T> &`.

As a simple example, this `seq()` function wraps a `vector` to create a sequence:

```c++
    std::vector<std::string> params;

    init(seq(params));
```

Lists are created using the `list()` function.  A list is just another type of sequence. `list()` is variadic, taking any number of arguments.

Examples of list:

```c++
    auto numbers = list(1,2,3);
    auto animals = list("dog", "cat");
```

Other sequence can be created by transforming an existing sequence, using the variety of methods on a sequence such as:

* `take`, `skip`, `where`, `select`, `take_until`, `as`

We will talk about these later.

## Using sequences as containers

Sequences behave mostly like normal C++ containers, so can for example just be iterated:

```c++
    for(auto animal : list("dog", "cat"))
        ...
```

The normal container functions are supported: `begin()`, `end()`, `empty()`, `size()`, `front()`, `back()`, `at()`

The performance characteristics of these functions is O(1) with the exceptions:

* `size()` is O(1) or O(n)
* `back()` is O(n)
* `at(n)` is O(`n`)

Sequences are not modifiable, so you cannot alter an existing sequence or change the contents of it. To do that, you need to modify the underlying container. The other way to modify a sequence is to create a new sequence that adapts an existing sequence - see [Adapting sequences] on how to do this.

Sequences can only be iterated in the forwards direction, so `rbegin()` and `rend()` are not supported.

## Operations

`bool any()`, `bool any(Pred p)`
This is more efficient than `size()>0` because it only needs to check the first item.

front_or_default
back_or_default

size_type count(Pred)

aggregate
sum

Example: hashing a sequence

```c++
    int hash = s.aggregate([](int n1, int n2) { return n1*13 + n2; })
```


## Comparing sequences

Sequences support comparison operators, `==`, `!=`, `<`, `>`, `<=`, `>=`. These perform memberwise equality and lexographical comparison. A comparator can be passed to the `equals()` and `lexographical_compare()` methods in case you need something other than the default `==` and `<` operators.

Example:

```c++
    assert(seq(1,3)==list(1,2,3));
```

## Managing sequences

Sequences cannot be returned from functions, or stored in arrays, containers or fields.  This is to reduce the potential problems with dangling iterators, references and thread safety. They do not for example store any data themselves, so to do that, just use one of the standard C++ containers.

Sequences can simply be created as needed where they are used, since sequence creation is extremely cheap (and does not allocate any heap memory for example.)

Sequences can be stored in `auto` stack objects, for example

```c++
    auto options = params.
        where([](const char * arg) { return arg[0]=='-'; }).
        select([](const char * arg) { return arg+1; });

    for(auto opt : options)
        ...
```

This is generally just to aid readability. (Note: This use case has to be supported since `for`-loops implicitly create an `auto` variable.)

Beware that the underlying data for the sequence may change, so care must be taken when storing a sequence for a longer period.

## Writing sequences

Writing sequences generally has the same problem as reading them - we want code to be as agnostic and reusable as possible, but this isn't easy in C++.

Imagine we just want to return a list. Simple right?

```c++
    std::vector<std::string> getItems();
    std::list<std::string> getItems();
    void getItems(std::vector<std::string> & output);
    template<typename T> void getItems(It outputIterator);
    void getItems(std::string * array, int size);
    void getItems(ItemVisitor & visitor);
    template<typename Fn> void getItems(Fn receiver);
    void getItems(std::function<void(const std::string)> & receiver);
    void getItems(std::ostream<std::string> &os);
```

The problem is that we need to commit to one approach (say, `std::vector`), unless we go full-generic with an output iterator. Ideally we could use `sequence` for this too, for example

```c++
    sequence<std::string> getItems();  // This doesn't work
```

This doesn't quite work for a few reasons. Firstly, `sequence<>` is an abstract class that can't be instantiated or returned. Secondly, we don't really know how the sequence should be stored - if at all. Finally, if the return value is computed lazily, then it might contain dangling references or iterators.

The other question is, do we want the caller or the callee to decide how to store the output? With a signature like `std::vector<std::string> getItems()`, we have firmly decided that the caller decides how to store the items. This is neither flexible or optimal.

To unify this mess, we provide `class output_sequence<T>` which provides a generic wrapper for building lists. Like `sequence<>` it is designed to be a lightweight way to unify all of these approaches.

To use an output sequence, pass it in as an argument, 

```c++
    void getItems(const output_sequence<std::string> & output)
    {
        output << "foo" << "bar" << list("foo","bar");
    }
```

The caller uses the `writer()` function to create a writer that wraps either a container or a function.

1. `writer(container)` - creates a writer that appends to a container
2. `writer<T>(fn)` - creates a writer that calls a function.

```c++
    std::vector<std::string> vec;

    getItems(writer(vec));
    getItems(writer<const char*>([](const char * str) { std::cout << "Option is " << str << std::endl; });
```

The writer actually uses a virtual function, incurring a cost of one virtual function call per item. This really isn't much, but to speed things up slightly, we could instead use a template in the callee.

```c++
template<typename Writer, typename = is_writer<Writer>::type>
void getItems(Writer writer)
{
    writer << "foo";
}
```

## Transforming sequences

