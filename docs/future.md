# Release plan

## Milestone 1

Tidy up everything
- Docs
- Code

Test writing to std::map<> as an output_sequence.

Tidy up the unit tests

Tidy up the code

Actions to run PRs and generate benchmarks

## Milestone 2

Reference manual

## Outstanding issues

Test all the functors properly (not just lambdas)

Avoid unnecessary copying, particularly of stored_sequence.

Complete the API by looking at IEnumerable

Set up an action to run the tests and create the benchmarks
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
