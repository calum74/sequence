# Sequence

Universal sequences in C++

## Overview

_Sequence_ provides a simple, efficient and uniform way to create, pass and manipulate sequences in C++. It aims to do two things:

- Provide a lightweight abstraction for creating and passing sequences
- Provide a [LINQ](https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/linq/)-style library for transforming and manipulating sequences

It was born out of the very basic frustration of "how do I actually pass a list to a function". Unfortunately, C++ provides far too many options, and they all have their drawbacks.

To solve this, we introduce a universal type, `sequence<T>` that represents any sequence of type `T`. `sequence<T>` unifies the plethora of abstractions that exist in C++, such as: iterator-pairs, ranges, C strings, arrays, pointers, containers, initializer lists, varargs and variadic templates.

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
    init(seq(argv, argc).take_until([](const char * str) { return strcmp(str, "--")==0; }));
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

As a final example of list manipulation, here's a sequence of prime numbers up to one thousand. Firstly using regular C++,

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

# Performance

_Sequence_ is a minimal-overhead library that does not use any containers or heap allocation under the hood.

There is once case where _Sequence_ is a bit more expensive, which is when passing arguments using `const sequence<T>&`. In this case, there is an additional overhead of one virtual function call per item in the sequence. If that is a problem, you can use a `pointer_sequence` instead which does not use virtual functions.

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

```
Regular C code: 112.628 ms
Sequence: 347.1 ms
```

Here, we do see some slowdown (a factor of 3) but this is over an extremely tight loop, and the overhead per iteration is 235ns on my CPU. The only explanation I can think of is that the compiler just did a better job on the C code.

