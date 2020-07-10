//
// Created by 胡凌瑞 on 2020/6/30.
//

#ifndef DATA_STRUCTURE_CPP_STACK_H
#define DATA_STRUCTURE_CPP_STACK_H

#include "List.h"
#include "dsexceptions.h"

template <typename Object>
class Stack
{
public:
    bool isEmpty( ) const
    { return theList.empty( ); }
    const Object & top( ) const
    { return theList.front( ); }
    void push( const Object & x )
    { theList.push_front( x ); }
    void pop( Object & x )
    { x = theList.front( ); theList.pop_front( ); }
private:
    List<Object> theList;
};

#endif //DATA_STRUCTURE_CPP_STACK_H
