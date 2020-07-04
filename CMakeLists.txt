cmake_minimum_required(VERSION 3.16)
project(data_structure_cpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(data_structure_cpp main.cpp practice/list_stack_and_queue/practice.h base_stl/dsexceptions.h base_stl/Vector.h base_stl/List.h base_stl/Stack.h base_stl/Queue.h base_stl/BinarySearchTree.h base_stl/AvlTree.h base_stl/SplayTree.h base_stl/SeparateChaining.h base_stl/QuadraticProbing.h base_stl/CuckooHashTable.h base_stl/UniformRandom.h)
