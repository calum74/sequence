project(sequence)
cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(../include)

add_executable(test_sequence test_sequence.cpp)

enable_testing()
add_test(Unit-tests test_sequence)