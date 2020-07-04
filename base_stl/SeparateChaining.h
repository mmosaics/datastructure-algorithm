//
// Created by 胡凌瑞 on 2020/7/2.
//

#ifndef DATA_STRUCTURE_CPP_SEPARATECHAINING_H
#define DATA_STRUCTURE_CPP_SEPARATECHAINING_H

/**
 * 分离链接法的Hash表实现
 */
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <string>

using std::vector;
using std::list;
using std::hash;

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


/**
 * 哈希表实现
 * @tparam HashedObj
 */
template <typename HashedObj>
class HashTable
{
public:
    explicit HashTable(int size = 101): currentSize(0) 
    {
        theLists.resize(101);
    }

    bool contains( const HashedObj & x) const
    {
        auto & theList = theLists[ myHash(x) ];
        auto itr = std::find(std::begin(theList), std::end(theList), x);
        if(itr == std::end(theList))
            return false;
        else
            return true;
    }

    void makeEmpty()
    {
        for(auto & thisList: theLists)
            thisList.clear();
    }

    bool insert(const HashedObj & x)
    {
        size_t pos = myHash(x);
        auto & theList = theLists[pos];
        if(std::find(std::begin(theList), std::end(theList), x) == std::end(theList))
        {
            theList.push_back(x);
            return true;
        }
        else
            return false;
    }

    bool insert(HashedObj && x)
    {
        size_t pos = myHash(x);
        auto & theList = theLists[pos];
        if(std::find(std::begin(theList), std::end(theList), x) == std::end(theList))
        {
            theList.push_back(std::move(x));
            return true;
        }
        else
            return false;
    }

    bool remove(const HashedObj & x)
    {
        auto & theList = theLists[ myHash(x) ];
        auto itr = std::find(std::begin(theList), std::end(theList), x);
        if(itr != std::end(theList))
        {
            theList.erase(itr);
            return true;
        }
        else
            return false;
    }

private:

    vector<list<HashedObj>> theLists;
    int currentSize;

    void rehash()
    {
        auto copyLists = theLists;

        for(auto & whichList: theLists)
            whichList.clear();

        theLists.resize(nextPrime(2 * theLists.size() ));

        for(auto & whichList: copyLists)
            for(auto & element: whichList)
                insert(std::move(element));

    }

    size_t myHash(const HashedObj & x) const
    {
        static hash<HashedObj> hashFunction;
        return hashFunction(x) % theLists.size();
    }

};

#endif //DATA_STRUCTURE_CPP_SEPARATECHAINING_H
