project(sequence)
cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(test_sequence test/test_sequence.cpp)

add_executable(example1 samples/example1.cpp)
add_executable(templates samples/templates.cpp)
add_executable(functions samples/functions.cpp)
add_executable(creation samples/creation.cpp)
add_executable(operations samples/operations.cpp)
add_executable(writers samples/writers.cpp)
add_executable(transformations samples/transformations.cpp)
add_executable(benchmarks test/benchmarks.cpp)

enable_testing()
add_test(Unit-tests test_sequence)
add_test(example1 example1 a b)
