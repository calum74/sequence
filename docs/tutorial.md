# Sequence tutorial

## Introduction

_Sequence_ is a C++ library for universal sequences. This has two main uses:

- Make it easier, safer and more efficient to pass lists between functions,
- Make it easier, safer and more efficient to manipulate and transform lists.

Sequences are heavily inspired by C# [`IEnumerable<>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerable-1). `IEnumerable<>` provides a common interface to all sequences and is the standard way to pass any list to and from an API, and to work with lists. In C++ we can use templates to generate efficient code that avoids many of the potential overheads of abstraction such as virtual function calls and memory allocations. From the user's perspective this is all under the hood, and sequences provide a natural and high level metalanguage to manipulate lists.

## What is a sequence?

A _sequence_ is just a series of elements, that can even be infinite. Sequences can be used in any situation where a list or collection is required, and replaces the car crash of alternatives in C++ with something simpler and more powerful.

Sequences can only be iterated in the forwards direction. They are C++ sequential containers, with an input iterator type.

Sequences are lightweight wrappers over some underlying collection, that provide a consistent interface across a range of underlying implementations, and imbue collections with additional functionality in a consistent way.

Sequences are evaluated lazily, processing one element at a time, rather than needing to store the entire sequence. This is generally very efficient.

In the following example, we do not actually create an array of 1 billion integers, but instead just iterate as if it were a normal `for` loop:

```c++
    int count = seq(1,1000000000).where([](int n) { return n%2==0; }).size();
```
We can from the disassembly of the code that the compiler has done a reasonable job of optimizing this.

<details>
<summary>Expand for disassembly
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

## Your first sequence

In [Example 1](../samples/example1.cpp) we see the iteration of a sequence constructed from the arguments to the `main()` function. The `seq()` function creates a sequence for the underlying data, and `skip(1)` adapts the original sequence, removing the first element.

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

The return type of `seq` is unspecified, but it can be stored in an `auto` variable, iterated using a `for` loop, or passed to a function taking a `const sequence<T> &`.

As a simple example, this `seq()` function wraps a `vector` to create a sequence:

The example [creation.cpp](../samples/creation.cpp) shows the various ways that sequences can be created:

```c++
    // Create an empty sequence (or an empty list - they are the same thing)
    print(seq<const char*>());
    print(list<const char*>());

    // Create a sequence with the given items
    print(list("hello"));
    print(list("cat", "dog", "parrot"));

    // Create a sequence from a container
    std::vector<const char*> items { "Beethoven", "Chopin", "Liszt"};
    print(seq(items));

    // Create a sequence from iterators
    print(seq(items.rbegin(), items.rend()));

    // Create a sequence from C arrays
    const char *carray[] = { "Red", "White", "Rose" };
    print(seq(carray));
    print(seq(carray+1, 2));

    // Create an integer range
    auto r = seq(1,10);

    // Output: 55
    std::cout << r.sum() << std::endl;

    // Create a sequence of characters in a C string
    std::cout << seq("Bergerac").size() << std::endl;
```

Lists are created using the `list()` function.  A list is just another type of sequence, but this does copy its elements into an internal fixed-length array. `list()` is variadic, taking any number of arguments.

Other sequences can be created by transforming an existing sequence, using the variety of methods on a sequence such as `take()`, `skip()`, `where()`, `select()`, `take_while()` and `as()`. We will talk about these in [Transforming Sequences](#transforming-sequences).

## Using sequences as containers

Sequences behave mostly like normal C++ containers, so can for example just be iterated:

```c++
    for(auto animal : list("dog", "cat"))
        ...
```

The normal container functions are supported, such as `begin()`, `end()`, `empty()`, `size()`, `front()`, `back()`, `at()` etc.

The performance characteristics of these functions is O(1) with the exceptions:

* `size()` is O(1) or O(n)
* `back()` is O(1) or O(n)
* `at(n)` is O(1) or O(`n`)

Sequences are not modifiable, so you cannot alter an existing sequence or change the contents of it. To do that, you need to modify the underlying container. The other way to modify a sequence is to create a new sequence that adapts an existing sequence - see [Transformaing sequences](#transforming-sequences) on how to do this.

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

The drawback with all of these approaches is that they are incompatible - you need to pick a representation up front, provide multiple implementations, or implement your code in header files. They are not always efficient either - sometimes it's just not necessary to store the data in a container, and there is overhead in creating a list in a different container to the one you already have.

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

This design is safe and efficient. There is an overhead of one virtual function call per element of the sequence, with the benefits of a simpler and more generic implementation. Virtual function calls can be avoided if you use templates (see the section on [performance-considerations].)

## Transforming sequences

## String processing



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


# Performance

## Performance vs hand-written code

- [ ] TODO: Building up a string
- [ ] Repeat implementation
- [ ] Aggregate over a different datatype
- Example where we don't need to actually create a vector after all.

When we are in a loop that actually does some work, the overe

```c++
    std::string result = list('a').repeat(1000000).aggregate(std::string{}, [](const std::string & str, char ch) { return str+ch; });

    std::string result;
    for(int i=0; i<1000000; i++) result += 'a';

    std::string process(const sequence<char> & input)
    {
        return input.aggregate(std::string{}, [](const std::string & str, char ch) { return str+ch; });
    }

    std::string process1(const sequence<int> & input)
    {
        return input.where([](int i) { return i%2==0; ).select([](int i) { return i*i; }).sum();
    }

    template<typename T>
    std::string process2(T input)
    {
        return input.where([sum();
    }

```

_Sequence_ is a minimal-overhead library that does not use any containers or heap allocation internally.

There is once case where _Sequence_ is a bit more expensive, which is when passing arguments using `const sequence<T>&`. In this case, there is an additional overhead of one virtual function call per item in the sequence. If that is a problem, you can use the even more lightweight `pointer_sequence` instead which does not use virtual functions, but is limited to cases where the data is stored in an underlying array.

## Benchmarks

Remember to configure our project for a release build (`cmake .. -DCMAKE_BUILD_TYPE=Release`) for this.

```c++
    const int N=1000000000;

    int sum=0;
    for(int i=0; i<=N; i++)
        if(i%2==0)
            sum += i*i;
```

<details>
<summary>Click here for disassembly
</summary>

```
0000000100001d58 <__Z13do_benchmark1v>:
100001d58: 00 e4 00 6f 	movi.2d	v0, #0000000000000000
100001d5c: 81 04 00 4f 	movi.4s	v1, #4
100001d60: 1f 20 03 d5 	nop
100001d64: 62 e0 00 9c 	ldr	q2, 0x100003970 <_strlen+0x100003970>
100001d68: 03 05 00 4f 	movi.4s	v3, #8
100001d6c: 08 40 99 52 	mov	w8, #51712
100001d70: 48 73 a7 72 	movk	w8, #15258, lsl #16
100001d74: 84 05 00 4f 	movi.4s	v4, #12
100001d78: 25 04 00 4f 	movi.4s	v5, #1
100001d7c: 06 06 00 4f 	movi.4s	v6, #16
100001d80: 07 e4 00 6f 	movi.2d	v7, #0000000000000000
100001d84: 10 e4 00 6f 	movi.2d	v16, #0000000000000000
100001d88: 11 e4 00 6f 	movi.2d	v17, #0000000000000000
100001d8c: 52 84 a1 4e 	add.4s	v18, v2, v1
100001d90: 53 84 a3 4e 	add.4s	v19, v2, v3
100001d94: 54 84 a4 4e 	add.4s	v20, v2, v4
100001d98: 55 1c 25 4e 	and.16b	v21, v2, v5
100001d9c: b5 9a a0 4e 	cmeq.4s	v21, v21, #0
100001da0: 56 9c a2 4e 	mul.4s	v22, v2, v2
100001da4: 52 9e b2 4e 	mul.4s	v18, v18, v18
100001da8: 73 9e b3 4e 	mul.4s	v19, v19, v19
100001dac: 94 9e b4 4e 	mul.4s	v20, v20, v20
100001db0: d6 1e 35 4e 	and.16b	v22, v22, v21
100001db4: 52 1e 35 4e 	and.16b	v18, v18, v21
100001db8: 73 1e 35 4e 	and.16b	v19, v19, v21
100001dbc: 94 1e 35 4e 	and.16b	v20, v20, v21
100001dc0: c0 86 a0 4e 	add.4s	v0, v22, v0
100001dc4: 47 86 a7 4e 	add.4s	v7, v18, v7
100001dc8: 70 86 b0 4e 	add.4s	v16, v19, v16
100001dcc: 91 86 b1 4e 	add.4s	v17, v20, v17
100001dd0: 42 84 a6 4e 	add.4s	v2, v2, v6
100001dd4: 08 41 00 71 	subs	w8, w8, #16
100001dd8: a1 fd ff 54 	b.ne	0x100001d8c <__Z13do_benchmark1v+0x34>
100001ddc: e0 84 a0 4e 	add.4s	v0, v7, v0
100001de0: 00 86 a0 4e 	add.4s	v0, v16, v0
100001de4: 20 86 a0 4e 	add.4s	v0, v17, v0
100001de8: 00 b8 b1 4e 	addv.4s	s0, v0
100001dec: 08 00 26 1e 	fmov	w8, s0
100001df0: 89 ec b4 52 	mov	w9, #-1486618624
100001df4: 00 01 09 0b 	add	w0, w8, w9
100001df8: c0 03 5f d6 	ret
```
</details>

```c++
    int sum = seq(0, N).
        where([](int n) { return n%2==0; }).
        select([](int n) { return n*n; }).sum();
```

<details>
<summary>Click here for disassembly
</summary>
arm64
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

Sample output

* Regular C code: 112.628 ms
* Sequence: 347.1 ms

Here, we do see some slowdown (a factor of 3) but this is over an extremely tight loop. The only explanation I can think of is that the compiler just did a better job on the C code, perhaps due to branch prediction. The code generated for `sequence` is actually excellent.
