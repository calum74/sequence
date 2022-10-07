# Building and installing Sequence

## Installation

For general use, Sequence does not have any build steps because it is a header-only library. The header files need to be available on the include path.

A simple way to add Sequence to your project is to include it as a git submodule. For example,

```bash
cd myproject
mkdir submodules
cd submodules
git submodule add https://github.com/calum74/sequence.git
```

The header files are in the [`include`](../include) directory, so in CMake for example add the line to `CMakeLists.txt`

```cmake
include_directories(submodules/sequence/include)
```

## Prerequisites

* A working C++11 build environment.

There are no other prerequisites. Sequence has been tested with clang, g++ and Microsoft Visual Studio. 

## Building the tests

Sequence uses CMake to build and run the test files. This should work out of the box with VSCode and Microsoft Visual Studio with the appropriate configuration. 

Prerequites:

* On Linux, `sudo apt install g++ cmake git`
* On OSX, `brew install cmake`. Install XCode developer tools.
* On Windows, VSCode or Visual Studio.

To build and run the tests, for example

```
git clone https://github.com/calum74/sequence.git
cd sequence
mkdir build
cd build
cmake ..
make all test
```

## Header files

The header file is `sequence.hpp`

```c++
#include <sequence.hpp>
```

A "light" header file, containing only forward declarations, is `sequence_fwd.hpp`.

```c++
#include <sequence_fwd.hpp>
```

This header file should be included in other header files to minimize compilation times.
