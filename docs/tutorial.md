# Sequence tutorial

## Introduction

_Sequence_ is a C++ library for universal sequences. This has two main uses:

- Make it easier, safer and more efficient to pass lists between functions,
- Make it easier, safer and more efficient to manipulate and transform lists.

Sequences are heavily inspired by C# [`IEnumerable<>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerable-1). `IEnumerable<>` provides a common interface to all sequences and is the standard way to pass any list to and from an API, and to work with lists. In C++ we can use templates to generate efficient code that avoids many of the potential overheads of abstraction such as virtual function calls and memory allocations. From the user's perspective this is all under the hood, and sequences provide a natural and high level metalanguage to manipulate lists.

## What is a sequence?

A _sequence_ is a series of elements of the same type. A sequence can be empty or infinite. Sequences can be used in any situation where a list or collection is required, and replaces the multitude of incompatible alternatives in C++ with something consistent, universal, simple and powerful.

Sequences can only be iterated in the forwards direction. Sequences are C++ sequential containers, with an input iterator type.

Sequences are lightweight wrappers over some underlying collection, that provide a consistent interface across a range of underlying implementations, and imbue collections with additional functionality in a consistent way.

Sequences are evaluated lazily, processing one element at a time, rather than needing to store the entire sequence. This is generally very efficient.

In the following example, we do not actually create an array of 1 billion integers, but instead just iterate as if it were a normal `for` loop:

```c++
    int count = seq(1,1000000000).where([](int n) { return n%2==0; }).size();
```

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

The `seq()` function is used to create sequences from a variety of data sources. It returns a lightweight wrapper around the underlying data.

1. `seq<T>()` called with no arguments returns an empty sequence. In this case, you need to specify the type `T` of the sequence.

2. `seq(const Container & c)` returns a sequence of the contents of `c`. It returns a sequence that iterates from `c.begin()` to `c.end()`.

3. `seq(const T* array, int size)`, `seq(const T (&array[Size])` returns a sequence of the array contents.

4. `seq(const char* str)` returns the sequence of characters in the null-terminated C-style string `str`.

5. `seq(int a, int b)` returns a sequence of integers, in the inclusive range `a` to `b`.

6. `seq(std::basic_istream<T> &)` returns a sequece that iterates the `streambuf` of a stream.

7. `list(...)` creates a sequence of the given elements.

All of these operations are lightweight and efficient, and do not iterate the underlying data until needed.

The return type of `seq` is unspecified, but it can be stored in an `auto` variable, iterated using a `for` loop, or passed to a function taking a `const sequence<T> &` argument.

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

Lists are created using the `list()` function. `list()` is variadic, taking any number of arguments. The template parameter can be specified explicitly to coerce the list to a given type, for example this is a sequence of type `std::string`:

```c++
    auto l = list<std::string>("cat", "dog");
```

Other sequences can be created by transforming an existing sequence, using the variety of methods on a sequence such as `take()`, `skip()`, `where()`, `select()`, `take_while()`, `as()` and `merge()`. These are discussed in [Transforming Sequences](#transforming-sequences).

## Using sequences as containers

Sequences are C++ containers, so can for example just be iterated:

```c++
    for(auto animal : list("dog", "cat"))
        ...
```

Sequences support many normal container functions, such as `begin()`, `end()`, `empty()`, `size()`, `front()`, `back()`, `at()`, comparators etc.

The performance characteristics of these functions are O(1) with the exceptions:

* `size()` is O(1) or O(n)
* `back()` is O(1) or O(n)
* `at(n)` is O(1) or O(`n`)

`front()`, `back()` and `at()` throw `std::out_of_bounds` if the sequence doesn't contain a value at the given position.

Sequences are read-only, so you cannot alter an existing sequence or change the contents of it. To do that, you need to modify the underlying container. When you transform a sequence, you create a new sequence without modifying the original.

Sequences can only be iterated in the forwards direction, so `rbegin()` and `rend()` are not supported.

## Operations

Sequences provide extra operations not found on normal containers:

* `any()` - tests if the sequence contains any element / element matching a predicate
* `count()` - counts the number of elements matching a predicate
* `front_or_default()`, `back_or_default()` - gets the item or returns a default value
* `at()` - gets an element at a given position
* `sum()` - sums all of the elements
* `aggregate()`, `accumulate()` - runs an arbitrary function over all elements and computes a result

See [operations.cpp](../samples/operations.cpp) for examples on how to use these functions.

```c++
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

    // A simple hash computation of a list of integers.
    int hash = list(1,2,3).aggregate(0, [](int n1, int n2) { return n1*13 + n2; });

```

## Transforming sequences

A sequence "transformation" adapts an existing sequence, leaving the original sequence unmodified.  Sequences provide a "fluent" API (similar to C# `IEnumerable<>`) to allow transformations to be easily composed.

Transformations are generally executed in a streaming way, processing one element at a time. The "creation" of a transformation means to define a new stream, but it does not actually process any elements.

For example the following code is efficient in that it doesn't create any arrays or iterate past the first element:

```c++
    auto s1 = seq(1,1000000000);
    auto s2 = s1.where([](int n) { return n%2==0; });
    auto s3 = s2.select([](int n) { return n*n; });
    bool b = s3.any();
```

The data types of `s1`, `s2` and `s3` are unspecified but they are stack objects.

Transformations include

* `where()` - filters a sequence
* `select()` - maps each element in the sequence
* `take()` - limits the size of the sequence
* `skip()` - skips the first elements of the sequence
* `take_while()` - limits the sequence while a condition is true
* `skip_until()` - skips the first elements while a condition is true
* `as<U>()` - converts all elements to a new type `U`
* `repeat()` - repeats the sequence
* `merge()` - merge/zip two sequences into one
* `+`/`concat` - concatenate two sequences

See [transformations.cpp](../samples/transformations.cpp) for examples of transforming sequences:

```c++
    // where(p) filters the selection
    print(seq1.where([](int x) { return x%2==0; }));

    // select(p) transforms each element in the selection
    print(seq1.select([](int x) { return x*2; }));
    
    // Of course, you can combine transformations
    print(seq1.where([](int x) { return x%2==0; }).select([](int x) { return x*2; }));

    // The order in which transformations are applied matters!
    print(seq1.select([](int x) { return x*2; }).where([](int x) { return x%2==0; }));

    // take(n) limits the number of elements up to the given number
    print(seq1.take(3));

    // take_while(p) carries on taking elements until the condition is false
    print(seq1.take_while([](int n) { return n<=6; }));

    // skip(n) skips a fixed number of elements
    print(seq1.skip(8));

    // skip_until skips until a condition is true
    print(seq1.skip_until([](int n) { return (n+1)%5==0; }));

    // concat(s) concatenates two sequences
    print(seq1.concat(seq1));

    // Also the + operator provides this
    print(seq1+seq(10,19));
    print(list(1,2,3)+list(4));

    // merge(s,fn) merge two sequences, calling fn on each pair
    print(seq1.merge(seq(10,19), [](int a, int b) { return a+b; }));

    // Cast each element to a new type using as<T>()
    print(list(true, false).as<int>());

    // Repeat sequence a number of times
    print(list(1,2).repeat(3));
```

## Writing sequences

Sequences don't actually store any data, so standard C++ containers should be used for storage.

There are a number of ways to write a sequence `s` to a container `vec`:

```c++
    // Pass the sequence into the constructor
    std::vector<std::string> vec { s.begin(), s.end() };

    // Create a generic writer to copy the contents to
    writer(vec) << s;

    // Copy the sequence to an existing container (using vec.insert())
    s.write_to(vec);

    // Construct a new container of the given type
    auto vec = s.make<std::vector<std::string>>(); 
```

`writer()` is an adapter (similar to `seq()`) that constructs a universal output sequence wrapping any implementation in a consistent interface. `writer()` is particularly useful to create output sequences which are necessary when returning sequences from functions (see next section.)

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

and also different possible ways of returning a list:

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

With sequences these methods can be written with the much simpler `sequence<T>` and `output_sequence<T>`:

```c++
    void setItems(const sequence<std::string> & items);
    void getItems(const output_sequence<std::string> & items);
```

`sequence<T>` and `output_sequence<T>` provide a consistent wrapper for all of these different approaches.

### Passing sequences into functions

Functions can use a `const sequence<T>&` parameter to receive a sequence. For example,

```c++
    // Somewhere to store the items
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

In the ideal world we could just return a sequence from a function, as follows:

```c++
    sequence<std::string> getItems();  // This does not work
```

This doesn't quite work for a few reasons. Firstly, `sequence<>` is an abstract class that can't be instantiated or returned. Secondly, we don't really know how the sequence should be stored - if at all. Finally, if the sequence is computed lazily, then it might contain dangling references or iterators that go out of scope when the function returns.

Instead we use `class output_sequence<T>` to receive a sequence of type `T`. This is a very simple class that works much like a visitor. `output_sequence<>` has only one method, `add()` to write an item to the sequence. There is also the `<<` operator to write an item or a sequence.

```c++
    void getItems(const output_sequence<std::string> & items)
    {
        seq(my_items).write_to(items);

        // Alternative syntax:
        items << seq(my_items);
    }
```

Output sequences are usually lambdas or sinks to write to containers. `writer()` is used to create an `output_sequence<>` that adds the elements to a container, for example:

```c++
    std::vector<std::string> options;

    getItems(writer(options));
```

This approach allows the caller to control how the output data is stored or processed. `getItems()` could equally work with a `std::list<std::string>` or a `std::unordered_set<std::string>` or any other suitable container.

`receiver()` is used to constuct an `output_sequence` from a lambda, to process elements one at a time:

```c++
    getItems(receiver([](const std::string & str) { std::cout << "The item was " << str << std::endl; }));
```

## String and stream processing

Sequences have another trick up their sleeve, which is the ability to read files and tokenize strings and streams. `seq(stream)` creates a sequence of the characters in the stream.

The `split()` function converts a sequence of characters into a sequence of `std::string`. It discards all of the given characters and groups anything else into tokens of `std::string`.

```c++
    std::ifstream file("data.txt");
    auto lines = seq(file).split("\r\n");
```

## Sequence lifetime

Sequences should only be created on the stack as short-lived temporary objects. (This is a slight departure from C#, where a field of type `IEnumerable<T>` is permitted.)

The reason for this is to avoid the dangers of invalid iterators or references. Recall that sequences are lightweight wrappers, so if the underlying data goes out of scope then the sequence is undefined and the program will crash.

Long term storage of sequence data should be done with a C++ container.

Sequences can be stored in `auto` stack objects, for example

```c++
    auto options = params.
        where([](const char * arg) { return arg[0]=='-'; }).
        select([](const char * arg) { return arg+1; });

    for(auto opt : options)
        ...
```

Sequences are only as thread-safe as the collections they are iterating. Generally, this offers very few guarantees, and it is a bad idea to modify the contents of a sequence you are currently iterating, due to iterators being invalidated. If the underlying data is `const` then multiple sequences (in possibly different threads) will not interfere with each other and can read the data safely. The same sequence must not be iterated reentrantly or in multiple threads.

To avoid these problems, create a different sequence in each thread, and do not pass references to sequences (`const sequence<T>&`) between threads.

As an example, `computeAsyncUnsafe()` exhibits undefined behaviour because the underlying sequence is iterated concurrently.

```c++
// This function should return 0, but actually does not because
// sum() is iterating over the same sequence concurrently which is
// undefined.
int computeAsyncUnsafe(const sequence<int> & values) {
    auto f1 = std::async(std::launch::async, [&]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [&]() { return values.sum(); });
    return f1.get() - f2.get();
}
```

The solution is to always capture the sequence by value (`[=]`), but this means changing the function signature to a type that can be copied by value, for example

```c++
// A safe version of computeAsyncUnsafe(), ensuring that the sequence
// is copied by value.
template<typename Seq>
int computeAsyncSafe1(const Seq & values) {
    auto f1 = std::async(std::launch::async, [=]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [=]() { return values.sum(); });
    return f1.get() - f2.get();
}

// A safe version of computeAsyncUnsafe(), ensuring that the sequence
// is copied by value.
int computeAsyncSafe2(const pointer_sequence<int> & values) {
    auto f1 = std::async(std::launch::async, [=]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [=]() { return values.sum(); });
    return f1.get() - f2.get();
}
```

## Performance

Performance of sequences is often equivalent to hand-written code. In [benchmarks.cpp](../test/benchmarks.cpp) we compare code written normally with code written using sequences. We observed a couple of circumstances where sequences are slower, particularly across function boundaries where we would naturally expect a performance cost of the function call, and whenever `sequence<T>&` is used, the code also incurs a cost of one virtual function call per element. This is usually quite acceptable.

From the disassembly of 

```c++
    int sum = seq(0, N).
        where([](int n) { return n%2==0; }).
        select([](int n) { return n*n; }).sum();
```

<details>
<summary>Click here for ARM64 disassembly</summary>

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

we see that the compiler has been able to optimize the code quite well.

Similarly, `writer` is a zero-overhead abstraction that only incurs additional virtual function calls when crossing function boundaries using `const output_sequence<T>&`.

### pointer_sequence

Functions can use a `const pointer_sequence<T> &` argument which is a more restricted sequence type, for better performance. This avoids virtual function calls, but it is more limited in its scope because not all sequences can be converted to a `pointer_sequence<>`. For example,

```c++
void setItems(const pointer_sequence<const char*> & p);
```

To avoid the overheads of function calls completely, we can template the function. i.e.

```c++
template<typename Seq>
void setItems(Seq seq);
```
