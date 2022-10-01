# Release plan

## Milestone 1

Tutorial and examples finished. Version 1.0a released

## Outstanding issues

Avoid unnecessary copying, particularly of stored_sequence.

Complete the API by looking at IEnumerable

Implement all the comparators

Consider C++11 compatibility?
Test on Windows.
Setup guide.
Think about namespaces of seq, receiver and writer.

## Possible extra features

Use a shared_ptr internally
Reverse
Sort
unique

Get a reverse list
More efficient back() and at()

generator functions - how to terminate??

int i=0;
seq([&](bool & finished) { finished = i<10; return i++; })

Parsing files - line by line for example

class transform
{
    int operator()(char input, std::string & output)
    {
        return: 0 = end
        1 = skip
        2 = result
    }
};

class lines
{
    bool operator()(const std::string & line)
    {

    }

};

class file_contents
{

};

generator(file_contents("foo.txt")).select(lines()). ...

file_sequence("foo.txt").select(lines()).   

Writing your own sequences

