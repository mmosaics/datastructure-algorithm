//
// Created by 胡凌瑞 on 2020/7/4.
//

#include <vector>
#include <algorithm>
#include <string>
#include "UniformRandom.h"

template <typename AnyType>
class CuckooHashFamily
{
public:
    virtual size_t hash( const AnyType & x, int which) const = 0 ;
    virtual int getNumberOfFunction() const = 0;
    virtual void generateNewFunctions() = 0;
};

template <int count>
class StringHashFamily: public CuckooHashFamily<std::string>
{
public:
    StringHashFamily(): MULTIPLIERS(count) {}

    int getNumberOfFunction() const override {
        return count;
    }

    void generateNewFunctions() override {
        for(auto & mult: MULTIPLIERS)
            mult = r.nextInt();
    }

    size_t hash(const string &x, int which) const override {
        const int multiplier = MULTIPLIERS[which];
        size_t hashVal = 0;

        for(auto ch : x)
            hashVal = multiplier * hashVal + ch;

        return hashVal;
    }

private:
    vector<int> MULTIPLIERS;
    UniformRandom r;
};


bool isPrime(int n);
int nextPrime(int n);


#ifndef DATA_STRUCTURE_CPP_CUCKOOHASHTABLE_H
#define DATA_STRUCTURE_CPP_CUCKOOHASHTABLE_H

template <typename AnyType, typename HashFamily>
class CuckooHashTable
{
public:
    explicit CuckooHashTable( int size = 101) : array(nextPrime(size))
    {
        numHashFunctions = hashFunctions.getNumberOfFunctions();
        rehashes = 0;
        makeEmpty();
    }

    void makeEmpty()
    {
        currentSize = 0;
        for(auto & entry: array)
            entry.isActive= false;
    }

    bool contains( const AnyType & x) const
    {
        return findPos(x) != -1;
    }

    int size() const
    {
        return currentSize;
    }

    int capacity() const
    {
        return array.size();
    }



    bool remove(const AnyType & x)
    {
        int currentPos = findPos(x);
        if(!isActive(currentPos))
            return false;

        array[currentPos].isActive = false;
        return true;
    }

    bool insert( const AnyType & x)
    {
        if(contains(x))
            return false;

        if(currentSize >= array.size() * MAX_LOAD)
            expand();

        return insertHelper1(x);

    }

    bool insert( AnyType && x)
    {
        if(contains(x))
            return false;

        if(currentSize >= array.size() * MAX_LOAD)
            expand();

        return insertHelper1(std::move(x));
    }

private:

    struct HashEntry
    {
        AnyType element;
        bool isActive;

        explicit HashEntry( const AnyType & e = AnyType{}, bool a = false): element(e), isActive(a) {}

        explicit HashEntry( AnyType && e, bool a = false): element(std::move(e)), isActive(a) {}

    };

    constexpr static const double MAX_LOAD = 0.40;
    static const int ALLOWED_REHASHES = 5;

    vector<HashEntry> array;

    int currentSize = 0;
    int numHashFunctions;
    int rehashes;
    UniformRandom r;
    HashFamily hashFunctions;

    bool insertHelper1( const AnyType & xx)
    {
        const int COUNT_LIMIT = 100;
        AnyType x = xx;

        while (true)
        {
            int lastPos = -1;
            int pos;

            for( int count = 0; count < COUNT_LIMIT; ++count)
            {
                for(int i = 0; i < numHashFunctions; ++i)
                {
                    pos = myHash(x, i);

                    //直接找到了一个空位置,直接执行插入操作
                    if(!isActive(pos))
                    {
                        array[pos] = std::move(HashEntry{std::move(x), true});
                        ++currentSize;
                        return true;
                    }
                }

                //没有找到空位置，则需要进行一系列置换操作，随机选取一个元素
                int i = 0;
                do {
                    pos = myHash(x, r.nextInt(numHashFunctions));
                } while (pos == lastPos && i++ < 5);

                lastPos = pos;

                std::swap(x, array[pos].element);
            }

            if( ++rehashes > ALLOWED_REHASHES)
            {
                expand();
                rehashes = 0;
            }
            else
                rehash();
        }
    }

    bool insertHelper1(AnyType && xx)
    {

    }

    bool isActive(int currentPos) const
    {
        return currentPos != -1 && array[currentPos].isActive;
    }

    void expand()
    {
        rehash(static_cast<int>( array.size() / MAX_LOAD));
    }

    void rehash()
    {
        hashFunctions.generateNewFunctions();
        rehash(array.size());
    }

    void rehash(int newSize)
    {
        vector<HashEntry> oldArray = array;

        //把数组容量扩大一倍
        array.resize(nextPrime(newSize));
        for(auto & entry: array)
            entry.isActive = false;

        //复制数据到新数组中
        currentSize = 0;
        for(auto & entry : oldArray)
            if(entry.isActive)
                insert(std::move(entry.element));
    }

    size_t myHash( const AnyType & x, int which) const
    {
        return hashFunctions.hash(x, which) % array.size();
    }

    int findPos( const AnyType & x) const
    {
        for( int i = 0; i < numHashFunctions; ++i)
        {
            int pos = myHash(x, i);

            if(isActive(pos) && array[pos].element == x)
                return pos;

        }
        return -1;
    }



};

#endif //DATA_STRUCTURE_CPP_CUCKOOHASHTABLE_H



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