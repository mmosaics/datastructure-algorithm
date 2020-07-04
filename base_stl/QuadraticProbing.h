//
// Created by 胡凌瑞 on 2020/7/2.
//

#ifndef DATA_STRUCTURE_CPP_QUADRATICPROBING_H
#define DATA_STRUCTURE_CPP_QUADRATICPROBING_H
/**
 * 平方探测哈希表的实现
 * 有一个重要结论如下：
 * 如果使用平方探测，且表的大小是素数，那么当表至少有一半是空的时候，总能够插入一个新的元素
 */

#include <algorithm>
#include <vector>
#include <functional>
#include "dsexceptions.h"


using std::hash;

bool isPrime(int n);
int nextPrime(int n);

template <typename HashedObj>
class HashTable
{

public:

    explicit HashTable(int size = 101): array(nextPrime(size)) {
        makeEmpty();
    }

    void makeEmpty() {
        currentSize = 0;
        for(auto & item : array)
            item.info = EMPTY;
    }

    bool contains(const HashedObj & x) const {
        return isActive(findPos(x));
    }

    bool insert(const HashedObj & x) {

        int currentPos = findPos(x);
        if(isActive(currentPos))
            return false;

        array[currentPos].element = x;
        array[currentPos].info = ACTIVE;

        ++currentSize;

        if(currentSize > array.size() / 2)
            rehash();

        return true;
    }

    bool insert(HashedObj && x) {

        int currentPos = findPos(x);
        if(isActive(currentPos))
            return false;

        array[currentPos].element = std::move(x);
        array[currentPos].info = ACTIVE;

        ++currentSize;

        if(currentSize > array.size() / 2)
            rehash();

        return true;

    }

    bool remove(const HashedObj & x) {

        int currentPos = findPos(x);
        if(!isActive(currentPos))
            return false;

        array[currentPos].info = DELETED;
        return true;

    }

    enum EntryType {ACTIVE, EMPTY, DELETED};

private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        explicit HashEntry( const HashedObj & e = HashedObj{}, EntryType i = EMPTY): element(e), info(i) {}

        explicit HashEntry( HashedObj && e, EntryType i = EMPTY): element(std::move(e)), info(i) {}

    };

    std::vector<HashEntry> array;
    int currentSize = 0;

    bool isActive( int currentPos ) const
    {
        if(currentPos > array.size())
            throw ArrayIndexOutOfBoundsException();
        return array[currentPos].info == ACTIVE;
    }

    int findPos( const HashedObj & x) const
    {
        int offset = 1;
        int currentPos = myHash(x);

        while (array[currentPos].info != EMPTY && array[currentPos].element != x) {
            currentPos += offset;
            if(array[currentPos].info == EMPTY)
                break;

            offset += 2;

            if(currentPos >= array.size()) {
                currentPos -= array.size();
            }


        }

        return currentPos;
    }

    void rehash()
    {

        std::vector<HashEntry> oldArray = array;

        array.resize(nextPrime(array.size() * 2));

        for(auto & entry: array)
            entry.info = EMPTY;

        currentSize = 0;
        for(auto & entry: oldArray)
            if(entry.info == ACTIVE)
                insert(std::move(entry.element));
    }

    size_t myHash( const HashedObj & x) const
    {
        static hash<HashedObj> hashFunction;
        return hashFunction(x) % array.size();

    }

};

/**
 * 为了方便把两个工具函数写在头文件里，一个是判断质数，另一个是找出下一个质数
 * @param n
 * @return
 */
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}

int nextPrime(int n)
{
    if( n % 2 == 0 )
        ++n;

    for( ; !isPrime( n ); n += 2 )
        ;

    return n;
}

#endif //DATA_STRUCTURE_CPP_QUADRATICPROBING_H
