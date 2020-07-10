cmake_minimum_required(VERSION 3.16)
project(data_structure_cpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(data_structure_cpp main.cpp base_structure/dsexceptions.h base_structure/Vector.h base_structure/List.h base_structure/Stack.h base_structure/Queue.h base_structure/BinarySearchTree.h base_structure/AvlTree.h base_structure/SplayTree.h base_structure/SeparateChaining.h base_structure/QuadraticProbing.h base_structure/CuckooHashTable.h base_structure/UniformRandom.h base_structure/BinaryHeap.h main.cpp base_structure/LeftistHeap.h practice/chapter2/figure1.h base_structure/BinomialQueue.h base_structure/Sort.h base_structure/DisjSets.h base_structure/Graph.h algorithm/pattern.h practice/chapter2/figure2.h)
