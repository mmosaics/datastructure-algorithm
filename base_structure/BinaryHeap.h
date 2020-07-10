//
// Created by 胡凌瑞 on 2020/7/4.
//

#ifndef DATA_STRUCTURE_CPP_BINARYHEAP_H
#define DATA_STRUCTURE_CPP_BINARYHEAP_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "dsexceptions.h"

template <typename Comparable>
class BinaryHeap
{
public:
    explicit BinaryHeap(int capacity = 100): array(capacity), currentSize(0) {}
    explicit BinaryHeap(const std::vector<Comparable> & items): array(items.size() + 10), currentSize(items.size())
    {
        for(int i = 0; i < items.size(); ++i)
            array[i + 1] = items[i];
        buildHeap();
    }

    bool isEmpty() const
    {
        return currentSize == 0;
    }

    const Comparable & findMin() const
    {
        if(isEmpty())
            throw UnderflowException();
        return array[1];
    }

    void insert(const Comparable & x)
    {
        if(currentSize == array.size() -1)
            array.resize(array.size() * 2);

        int newPos = ++currentSize;
        Comparable copy = x;

        array[0] = std::move(copy);
        for(; x < array[newPos/2]; newPos /= 2)
            array[newPos] = std::move(array[newPos/2]);
        array[newPos] = std::move(array[0]);
    }

    void insert(Comparable && x)
    {
        if(currentSize == array.size() -1)
            array.resize(array.size() * 2);

        int newPos = ++currentSize;

        array[0] = std::move(x);
        for(; array[0] < array[newPos/2]; newPos /= 2)
            array[newPos] = std::move(array[newPos/2]);
        array[newPos] = std::move(array[0]);
    }

    void deleteMin()
    {
        if(isEmpty())
            throw UnderflowException();

        array[1] = std::move(array[currentSize--]);

        percolateDown(1);

    }

    void deleteMin(Comparable & x)
    {
        if(isEmpty())
            throw UnderflowException();

        x = std::move(array[1]);
        array[1] = std::move(array[currentSize--]);
        percolateDown(1);

    }

    void makeEmpty()
    {
        currentSize = 0;
    }

private:
    int currentSize;
    std::vector<Comparable> array;

    void buildHeap()
    {
        for(int i = currentSize/2 ; i>0 ; --i)
            percolateDown(i);
    }
    void percolateDown(int hole) {

        int child;
        Comparable tmp = std::move(array[hole]);

        for(; hole <= currentSize/2; hole = child)
        {
            child = hole * 2;
            if(child  != currentSize && array[child] > array[child+1])
                ++child;
            if(array[child] < tmp)
                array[hole] = std::move(array[child]);
            else
                break;

        }

        array[hole] = std::move(tmp);

    }


};


#endif //DATA_STRUCTURE_CPP_BINARYHEAP_H
