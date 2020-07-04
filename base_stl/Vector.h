//
// Created by 胡凌瑞 on 2020/6/28.
//

#ifndef DATA_STRUCTURE_CPP_VECTOR_H
#define DATA_STRUCTURE_CPP_VECTOR_H

#include <iostream>
#include "dsexceptions.h"

template <typename Object>
class Vector {

public:

    explicit Vector(int size = 0): theSize(size), theCapacity(size + SPARE_CAPACITY)
    {
        objects = new Object[ theCapacity ];
    }

    Vector(const Vector & rhs) : theSize(rhs.theSize), theCapacity(rhs.theCapacity)
    {
        objects = new Object[theCapacity];
        for(int i = 0; i < theSize; i++)
            objects[i] = rhs.objects[i];
    }

    Vector(Vector && rhs) noexcept : theSize(rhs.theSize), theCapacity(rhs.theCapacity), objects(rhs.objects)
    {
        rhs.objects = nullptr;
        rhs.theSize = 0;
        rhs.theCapacity = 0;
    }

    Vector & operator=(const Vector & rhs)
    {
        if(this == &rhs)
            return *this;
        Vector copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    Vector & operator=(Vector && rhs) noexcept
    {
        std::swap(objects, rhs.objects);
        std::swap(theSize, rhs.theSize);
        std::swap(theCapacity, rhs.theCapacity);

        return *this;
    }

    ~Vector()
    {
        delete [] objects;
    }

    bool empty( ) const
    {
        return size() == 0;
    }
    int size( ) const
    {
        return theSize;
    }
    int capacity( ) const
    {
        return theCapacity;
    }

    Object & operator[]( int index )
    {
        //其中一种写法，还有一种写法如下一个函数
        if(index < theSize && index >= 0)
            return objects[index];
        else
            throw ArrayIndexOutOfBoundsException();
    }

    const Object & operator[]( int index ) const
    {
        if(index < 0 || index >= theSize)
            throw ArrayIndexOutOfBoundsException();
        return objects[index];
    }


    void resize( int newSize )
    {
        if( newSize > theCapacity )
            reserve( newSize * 2 );
        theSize = newSize;
    }

    /**
     * 把Vector的容纳大小提高到newCapacity
     * @param newCapacity
     */
    void reserve( int newCapacity )
    {
        if(newCapacity < theSize)
            return;

        Object* newArray;
        newArray = new Object[newCapacity];

        for(int i = 0; i < theSize; i++)
            newArray[i] = std::move(objects[i]);

        theCapacity = newCapacity;
        std::swap(objects, newArray);
        delete [] newArray;
    }

    // Stacky stuff
    void push_back( const Object & x )
    {
        if(theSize == theCapacity)
            reserve(size() * 2 + 1); // +1 是为了应对size为0的场景

        objects[theSize++] = x;

    }
    // Stacky stuff
    void push_back( Object && x )
    {
        if(theSize == theCapacity)
            reserve(size() * 2 + 1);
        objects[theSize++] = std::move(x);
    }

    void pop_back( )
    {
        if(empty())
            throw UnderflowException();
        --theSize;
    }

    const Object & back ( ) const
    {
        if(empty())
            throw UnderflowException();
        return objects[ theSize - 1 ];
    }

    void printVector() const
    {
        for(int i = 0; i < size(); i++)
            std::cout<<objects[i] << " ";
    }

    // Iterator stuff: not bounds checked
    typedef Object * iterator;
    typedef const Object * const_iterator;

    iterator begin( )
    {
        return &objects[0];
    }
    const_iterator begin( ) const
    {
        return &objects[0];
    }
    iterator end( )
    {
        return &objects[theSize];

    }
    const_iterator end( ) const
    {
        return &objects[theSize];
    }

    const static int SPARE_CAPACITY = 2;

private:
    int theSize;
    int theCapacity;
    Object * objects;
};

#endif //DATA_STRUCTURE_CPP_VECTOR_H
