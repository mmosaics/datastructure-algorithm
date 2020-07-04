//
// Created by 胡凌瑞 on 2020/6/30.
//

#ifndef DATA_STRUCTURE_CPP_QUEUE_H
#define DATA_STRUCTURE_CPP_QUEUE_H

#include "List.h"

template <typename Object>
class Queue
{
public:
    bool isEmpty( ) const
    { return theList.empty( ); }
    const Object & getFront( ) const
    { return theList.front( ); }
    void enqueue( const Object & x )
    { theList.push_back( x ); }
    void dequeue( Object & x )
    { x = theList.front( ); theList.pop_front( ); }
private:
    List<Object> theList;
};


#endif //DATA_STRUCTURE_CPP_QUEUE_H
