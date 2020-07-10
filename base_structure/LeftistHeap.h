//
// Created by 胡凌瑞 on 2020/7/5.
//

#ifndef DATA_STRUCTURE_CPP_LEFTISTHEAP_H
#define DATA_STRUCTURE_CPP_LEFTISTHEAP_H

#include <algorithm>
#include <vector>
#include <iostream>
#include "dsexceptions.h"

/**
 * 左倾树
 * 左倾树稍加修改即可成为斜堆
 * 类似于AVL树和伸展树的关系
 * @tparam Comparable
 */
template <typename Comparable>
class LeftistHeap
{

    explicit LeftistHeap() : root(nullptr) {}

    LeftistHeap( const LeftistHeap & rhs ) : root{ nullptr }
    { root = clone( rhs.root ); }

    LeftistHeap( LeftistHeap && rhs )  noexcept : root{ rhs.root }
    {
        rhs.root = nullptr;
    }

    ~LeftistHeap( )
    { makeEmpty( ); }

    LeftistHeap & operator=( const LeftistHeap & rhs )
    {
        if(this == &rhs)
            return *this;
        LeftistHeap copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    LeftistHeap & operator=( LeftistHeap && rhs )
 noexcept     {
        std::swap( root, rhs.root );

        return *this;
    }

    bool isEmpty() const
    {
        return root == nullptr;
    }

    const Comparable & findMin() const
    {
        if(isEmpty())
            throw UnderflowException();
        return root->element;
    }


    void insert( const Comparable & x)
    {
        root = merge(new LeftistNode(x), root);
    }

    void insert( Comparable && x)
    {
        root = merge(new LeftistNode(std::move(x)), root);
    }

    void deleteMin()
    {
        if(isEmpty())
            throw UnderflowException();
        LeftistNode * oldNode = root;
        root = merge(root->left, root->right);
        delete oldNode;
    }

    void deleteMin( Comparable & minItem)
    {
        if(isEmpty())
            throw UnderflowException();
        minItem = findMin();
        deleteMin();
    }

    void makeEmpty()
    {
        reclaimMemory(root);
        root = nullptr;
    }

    void merge( LeftistHeap & rhs)
    {
        if(this == &rhs)
            return;
        root = merge(root, rhs.root);
        rhs.root = nullptr;
    }


private:
    struct LeftistNode
    {
        Comparable element;
        LeftistNode * left;
        LeftistNode * right;
        int npl;

        explicit LeftistNode(const Comparable & e, LeftistNode * lt = nullptr, LeftistNode * rt = nullptr, int npl = 0)
        :element(e), left(lt), right(rt), npl(npl){}

        explicit LeftistNode(Comparable && e, LeftistNode * lt = nullptr, LeftistNode * rt = nullptr, int npl = 0)
        : element(std::move(e)), left(lt), right(rt), npl(npl){}

    };

    LeftistNode * root;

    LeftistNode * merge(LeftistNode * h1, LeftistNode * h2)
    {
        if(h1 == nullptr)
            return h2;
        if(h2 == nullptr)
            return h1;
        if(h1->element < h2->element)
            return merge1(h1, h2);
        else
            return merge1(h2, h1);

    }
    LeftistNode * merge1(LeftistNode * h1, LeftistNode * h2)
    {
        if(h1->left == nullptr)
            h1->left = h2;
        else
        {
            h1->right = merge(h1->right, h2);
            if(h1->left->npl < h1->right->npl)
                swapChildren(h1);
            h1->npl = h1->right->npl + 1;
        }
        return h1;
    }

    void swapChildren(LeftistNode * t)
    {
        LeftistNode *tmp = t->left;
        t->left = t->right;
        t->right = t->left;
    }

    void reclaimMemory(LeftistNode * t)
    {
        if(t != nullptr)
        {
            reclaimMemory( t->left );
            reclaimMemory( t->right );
            delete t;
        }

    }

    LeftistNode * clone(LeftistNode * t) const
    {
        if(t == nullptr)
            return nullptr;
        else
            return new LeftistNode( t->element, clone(t->left), clone(t->right), t->npl);
    }

};

#endif //DATA_STRUCTURE_CPP_LEFTISTHEAP_H
