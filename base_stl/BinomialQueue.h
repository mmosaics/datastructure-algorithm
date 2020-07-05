//
// Created by 胡凌瑞 on 2020/7/5.
//

#ifndef DATA_STRUCTURE_CPP_BINOMIALQUEUE_H
#define DATA_STRUCTURE_CPP_BINOMIALQUEUE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "dsexceptions.h"

template <typename Comparable>
class BinomialQueue {

public:

    BinomialQueue(): theTrees(DEFAULT_TREES)
    {
        for(auto & root: theTrees)
            root = nullptr;
        currentSize = 0;
    }

    explicit BinomialQueue(const Comparable & item): theTrees(1), currentSize(1)
    {
        theTrees[0] = new BinomialNode(item, nullptr, nullptr);
    }

    explicit BinomialQueue(Comparable && item): theTrees(1), currentSize(1)
    {
        theTrees[0] = new BinomialNode(std::move(item), nullptr, nullptr);
    }

    BinomialQueue(const BinomialQueue & rhs): theTrees(rhs.theTrees.size()), currentSize(rhs.currentSize)
    {
        for(int i = 0; i < rhs.theTrees.size(); ++i)
            theTrees[i] = clone(rhs.theTrees[i]);
    }

    BinomialQueue(BinomialQueue && rhs) noexcept : theTrees(std::move(rhs.theTrees)), currentSize(rhs.currentSize)
    {
    }

    ~BinomialQueue()
    {
        makeEmpty();
    }

    BinomialQueue & operator=(const BinomialQueue & rhs)
    {
        if(this == &rhs)
            return *this;
        BinomialQueue copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    BinomialQueue & operator=( BinomialQueue && rhs) noexcept
    {
        std::swap(currentSize, rhs.currentSize);
        std::swap(theTrees, rhs.theTrees);
        return *this;
    }


    bool isEmpty() const
    {
        return currentSize == 0;
    }

    const Comparable & findMin() const
    {
        if( isEmpty())
            throw UnderflowException();
        return theTrees[findMinIndex()]->element;
    }

    void insert( const Comparable & x)
    {
        BinomialQueue oneItem(x);
        merge(oneItem);
    }


    void insert( Comparable && x)
    {
        BinomialQueue oneItem( std::move(x));
        merge(oneItem);
    }

    void deleteMin()
    {
        Comparable x;
        deleteMin(x);
    }

    void deleteMin( Comparable & minItem)
    {
        if(isEmpty())
            throw UnderflowException();

        int minIndex = findMinIndex();
        minItem = theTrees[minIndex]->element;

        BinomialNode *oldRoot = theTrees[minIndex];
        BinomialNode *deletedTree = oldRoot->leftChild;
        delete oldRoot;

        //将删除之后的树构建成一个新的二项队列
        BinomialQueue deletedQueue;
        deletedQueue.theTrees.resize( minIndex + 1);
        deletedQueue.currentSize = (1 << minIndex) - 1;
        for(int j = minIndex - 1; j >= 0; --j)
        {
            deletedQueue.theTrees[j] = deletedTree;
            deletedTree = deletedTree->nextSibling;
            deletedQueue.theTrees[j]->nextSibling = nullptr;
        }

        //删除this中的deletedTree
        theTrees[minIndex] = nullptr;
        currentSize -= deletedQueue.currentSize + 1;

        //与剩下的节点合并
        merge(deletedQueue);

    }

    void makeEmpty()
    {
        currentSize = 0;
        for( auto & root: theTrees)
            makeEmpty(root);
    }

    void merge( BinomialQueue & rhs)
    {
        if(this == &rhs)
            return;

        currentSize += rhs.currentSize;

        if(currentSize > capacity())
        {
            int oldNumTrees = theTrees.size();
            int newNumTrees = std::max( theTrees.size(), rhs.theTrees.size()) + 1;
            theTrees.resize(newNumTrees);
            for(int i = oldNumTrees; i < newNumTrees; ++i)
                theTrees[i] = nullptr;
        }

        BinomialNode *carry = nullptr;
        for(int i = 0, j = 1; j <= currentSize; ++i, j *= 2)
        {
            BinomialNode *t1 = theTrees[i];
            BinomialNode *t2 = i < rhs.theTrees.size() ? rhs.theTrees[i] : nullptr;

            int whichCase = t1 == nullptr ? 0 : 1;
            whichCase += t2 == nullptr ? 0 : 2;
            whichCase += carry == nullptr ? 0 : 4;

            switch (whichCase)
            {
                case 0: //都没有树
                case 1: //只有this有树,这两种情形都不需要做什么
                    break;
                case 2: //只有rhs有树，这个时候只需要加入到this当中即可
                    theTrees[i] = t2;
                    rhs.theTrees[i] = nullptr;
                    break;
                case 4: //只有carry有的情形，也只需要把carry放入树中即可
                    theTrees[i] = carry;
                    carry = nullptr;
                    break;
                case 3: //t1和t2都有，这个时候需要存入carry
                    carry = combineTrees(t1, t2);
                    theTrees[i] = rhs.theTrees[i] = nullptr;
                    break;
                case 5: //t1和carry都有，这个时候需要合并t1和carry，然后存入carry
                    carry = combineTrees(t1, carry);
                    theTrees[i] = nullptr;
                    break;
                case 6: //t2和carry都有，这个时候合并t2和carry，存入carry
                    carry = combineTrees(t2, carry);
                    rhs.theTrees[i] = nullptr;
                    break;
                case 7: //t1,t2和carry都有，合并两个，保留一个
                    theTrees[i] = carry;
                    carry = combineTrees(t1, t2);
                    rhs.theTrees[i] = nullptr;
                    break;
                default:
                    break;

            }
        }

        for(auto & root : rhs.theTrees)
            root = nullptr;
        rhs.currentSize = 0;
    }

private:

    struct BinomialNode
    {
        Comparable element;
        BinomialNode *leftChild;
        BinomialNode *nextSibling;

        BinomialNode(const Comparable &e, BinomialNode *lt, BinomialNode *rt)
                : element(e), leftChild(lt), nextSibling(rt) {}

        BinomialNode(Comparable &&e, BinomialNode *lt, BinomialNode *rt)
                : element(std::move(e)), leftChild(lt), nextSibling(rt) {}
    };

    const static int DEFAULT_TREES = 1;

    std::vector<BinomialNode*> theTrees;
    int currentSize;

    int findMinIndex() const
    {
        int i;
        int minIndex;

        for( i = 0; theTrees[i] == nullptr; ++i)
            ;

        for( minIndex = i; i < theTrees.size(); ++i)
        {
            if(theTrees[i] != nullptr &&
                theTrees[i]->element < theTrees[minIndex]->element)
                minIndex = i;
        }

        return minIndex;
    }

    int capacity() const
    {
        return (1<<theTrees.size()) - 1;
    }

    BinomialNode * combineTrees(BinomialNode * t1, BinomialNode * t2)
    {
        if(t2->element < t1->element)
            return combineTrees(t2, t1);
        t2->nextSibling = t1->leftChild;
        t1->leftChild = t2;
        return t1;
    }

    void makeEmpty( BinomialNode * & t)
    {
        if( t != nullptr)
        {
            makeEmpty(t->leftChild);
            makeEmpty(t->nextSibling);
            delete t;
            t = nullptr;
        }

    }

    BinomialNode * clone( BinomialNode * t) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return new BinomialNode{ t->element, clone( t->leftChild ), clone( t->nextSibling ) };
    }






};

#endif //DATA_STRUCTURE_CPP_BINOMIALQUEUE_H
