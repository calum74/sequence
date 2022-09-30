# Release plan

## Milestone 1

Tutorial and examples finished. Version 1.0a released

## Outstanding issues

cloning sequences - clone().


```
writable_sequence<T>:
```

Pipe interface
```
seq(vec) | where(...) | select(...) | count() >> seq(vec2);
```

Returning sequences. sequence_writer<> maybe

Avoid unnecessary copying, particularly of stored_sequence.

Complete the API by looking at IEnumerable

Zip.

## Possible extra features

Use a shared_ptr internally
Reverse
Sort

Idea of an existing list - a deque() that wraps 

Get a reverse list
More efficient back() and at()