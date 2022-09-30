# Sequence tutorial

## Introduction

_Sequence_ is a C++ library for universal sequences. This has 2 main uses:

- Make it easier, safer and more efficient to pass lists between functions,
- Make it easier, safer and more efficient to manipulate and transform lists.

Sequences are heavily inspired by C# [`IEnumerable<>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerable-1) of course. `IEnumerable<>` provides a common interface to all sequences and is the de facto way to pass any list to and from an API, and to work with lists. Unlike C#, in C++ we can use C++ templates to generate efficient code that avoids many of the potential overheads of abstraction such as virtual function calls and memory allocations.

## What is a sequence?

A _sequence_ is defined as a set of ordered elements of the same type. A sequence can even be infinite. Sequences can be used in any situation where a list or collection is required, and replaces the various alternatives in C++ with something simpler and more powerful.

Sequences can only be iterated in the forwards direction. They are C++ sequential containers, with an input iterator type.

Sequences are lightweight objects that wrap some underlying collection, for example with a pair of iterators. Sequences are generally short-lived convenience objects, implementing an "adapter pattern" to provide a simple and consistent interface over a variety of different implementations and scenarios. This

- Provides a consistent interface across a range of underlying implementations,
- Adds extra functionality to all sequences in a consistent way.

Sequences are generally evaluated lazily, meaning that elements are processed one and a time, and we rarely need to store the entire sequence. In this example, we do not actually create an array of 1 billion integers, or a filtered array of 500 million integers (the result of the `where` call). Instead the code just iterates as if it were a normal `for` loop:

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

These computed, filtered and transformed sequences can be passed into any function taking a sequence, where they can be enumerated lazily without needing to create another array.

## Your first sequence

```c++
#include <sequence.hpp>

int main(int argc, char**argv)
{
    for(auto i : seq(argv, argc).skip(1))
        std::cout << "Hello " << i << std::endl;
    return 0;
}
```

## Creating sequences

The `seq()` function is used to create sequences for a variety of situations. It returns a lightweight wrapper for an appropriate sequence object.

1. `seq<T>()` called with no arguments returns an empty sequence. In this case, you need to specify the type `T` of the sequence.

2. `seq(const Container & c)` returns a sequence of the contents of `c`. It returns a sequence that iterates from `c.begin()` to `c.end()`. Of course, the container contents are not copied anywhere and iteration is only performed when it is needed.

3. `seq(const T* array, int size)`, `seq(const T (&array[Size])` returns a sequence of the array contents.

4. `seq(const char* str)` returns the sequence of characters in the null-terminated C-style string `str`.

5. `seq(int a, int b)` returns a sequence of integers, in the inclusive range a-b.

All of these operations are O(1) and efficient. All they really do is wrap the underlying collection in a different type.

The return type of `seq( )` is unspecified, but it can be stored in an `auto` variable, iterated using a `for` loop, or passed to a function taking a `const sequence<T> &`.

As a simple example, this `seq()` function wraps a `vector` to create a sequence:

```c++
    std::vector<std::string> params;

    init(seq(params));
```

Lists are created using the `list()` function.  A list is just another type of sequence, but this does copy its elements into an internal fixed-length array. `list()` is variadic, taking any number of arguments.

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

The normal container functions are supported, such as `begin()`, `end()`, `empty()`, `size()`, `front()`, `back()`, `at()`.

The performance characteristics of these functions is O(1) with the exceptions:

* `size()` is O(1) or O(n)
* `back()` is O(1) or O(n)
* `at(n)` is O(1) or O(`n`)

TODO: Implement these efficiently

Sequences are not modifiable, so you cannot alter an existing sequence or change the contents of it. To do that, you need to modify the underlying container. The other way to modify a sequence is to create a new sequence that adapts an existing sequence - see [Adapting sequences] on how to do this.

Sequences can only be iterated in the forwards direction, so `rbegin()` and `rend()` are not supported.

## Operations

In addition to 

`bool any()`, `bool any(Pred p)`
This is more efficient than `size()>0` because it only needs to check the first item.

front_or_default
back_or_default

size_type count(Pred)

aggregate
sum

write_to(vec)

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

## Sequence lifetime

Sequences should only be created on the stack as short-lived temporary objects. (This is a slight departure from C#, where a field of type `IEnumerable<T>` is permitted.)

The reason for this is to avoid the dangers of invalid iterators or references. Recall that sequences are lightweight wrappers, so if the underlying data goes out of scope then the sequence is undefined and the program will crash.

Long term storage of sequence data should always be done using a C++ container.

Sequences can be stored in `auto` stack objects, for example

```c++
    auto options = params.
        where([](const char * arg) { return arg[0]=='-'; }).
        select([](const char * arg) { return arg+1; });

    for(auto opt : options)
        ...
```

## Passing sequences to functions

A major use case for sequences is passing lists to and from functions. Recall the different ways of passing in a list in C++:

```c++
    void setItems(const std::vector<std::string> & items);
    void setItems(std::vector<std::string>&& items);
    void setItems(const std::list<std::string>& items);
    void setItems(std::list<std::string>&&);
    void setItems(std::initializer_list<std::string> items);
    template<typename It> void setItems(It a, It b);
    void setItems(const std::string * array, int length);
    void setItems(const std::string * array, const std::string * end);
```

and also different possible ways of returning lists:

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

The drawback with all of these approaches is that they are incompatible - you need to pick a representation up front, provide multiple implementations, or implement your code in header files. They are not always efficient either - sometimes it's just not necessary to store the data in a container.

_Sequence_ replaces these methods with the much simpler

```c++
    void setItems(const sequence<std::string> & items);
    void getItems(const output_sequence<std::string> & items);
```
### Passing sequences into functions

Functions can use a `const sequence<std::string>&` paramter to receive a sequence. This just behaves as you would imagine:

```c++
    // Somewhere we store the items
    std::vector<std::string> my_items;

    // A method that receives some items
    void setItems(const sequence<std::string> & items)
    {
        items.take(10).write_to(my_items);
    }

    // Call the method with some data
    setItems(list("dog", "cat").as<std::string>());
    setItems(seq(argv,argc).skip(1).as<std::string>());
```

### Returning sequences from functions

In the ideal world we could just return a sequence from a function as

```c++
    sequence<std::string> getItems();  // This does not work
```

This doesn't quite work for a few reasons. Firstly, `sequence<>` is an abstract class that can't be instantiated or returned. Secondly, we don't really know how the sequence should be stored - if at all. Finally, if the sequence is computed lazily, then it might contain dangling references or iterators that went out of scope when the function returns.

Instead we use `class output_sequence<T>` to receive a sequence of type `T`. This is a very simple concept that works much like a visitor. It has only one method, `add()` to write an item to the sequence. We also provide the `<<` operator to write to the sequence if that is preferred.

```c++
    void getItems(const output_sequence<std::string> & items)
    {
        seq(my_items).write_to(items);

        // Alternative syntax:
        items << seq(my_items);
    }
```

Output sequences are typically lambdas or write to containers. `writer()` is used to create an `output_sequence<>` that adds the element to a container, for example:

```c++
    std::vector<std::string> options;

    getItems(writer(options));
```

This provides a uniform way to add data to any container as determined by the caller, not the callee. The `getItems()` function could equally work with a `std::list<std::string>` or a `std::unordered_set<std::string>` or any other suitable container.

TODO: Test with maps and sets.

`receiver()` is used to pass the elements to a lambda:

```c++
    getItems(receiver([](const std::string & str) { std::cout << "The item was " << str << std::endl; }));
```

This design is safe and efficient. There is an overhead of one virtual function call per element of the sequence, with the benefits of a simpler and more generic implementation. Virtual function calls can be avoided if you use templates (see the section on [Performance considerations].)

## Transforming sequences

## Performance considerations

As suggested earlier, _Sequence_ is largely an overhead-free abstraction where the compiler is able to generate efficient code in most circumstances.

Sequence transformations are evaluated lazily, so are generally more efficient than creating an entire array.

There is overhead at function boundaries, where where types `sequence<T>` and `output_sequence<T>` use virtual functions. Here there is one virtual function call per element in the sequence, with the benefits of separate compilation where the function can be implemented in a `.cpp` file.

Where performance is critical, we can use `class pointer_sequence<T>` instead, which requires that the underlying data is stored in an array. Of course, this excludes computed sequences, but still covers many scenarios.

```c++
    // Example of pointer_sequence...
```

For even better performance across function boundaries, we can still resort to templated functions.

```c++
    Example here...
```
