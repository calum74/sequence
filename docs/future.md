# Release plan

## Milestone 1

Tutorial and examples finished. Version 1.0a released
- Transforming
- String processing
- Rename _Sequence_ to "sequences"

Tidy up the unit tests
- Finish comparators
- does count exist()

Tidy up the code

## Milestone 2

Reference manual

## Outstanding issues

Avoid unnecessary copying, particularly of stored_sequence.

Complete the API by looking at IEnumerable

Implement all the comparators

Test on Windows.
Setup guide.

Think about namespaces of seq, receiver and writer.

## Possible extra features

Reverse
Sort
unique

Get a reverse list
More efficient back() and at()

generator functions - how to terminate??

words.join() - same as sum()

words.join(" ") - Constructs a string with spaces.


It would be great if the following code worked:


```
void setItems(const sequence<std::string> & items);

std::vector<std::string> vec;
setItems(vec);
```
