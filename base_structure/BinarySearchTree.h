//
// Created by 胡凌瑞 on 2020/6/30.
//

#ifndef DATA_STRUCTURE_CPP_BINARYSEARCHTREE_H
#define DATA_STRUCTURE_CPP_BINARYSEARCHTREE_H

#include <algorithm>
#include <iostream>
#include "dsexceptions.h"

template <typename Comparable>
class BinarySearchTree
{
public:

    explicit BinarySearchTree(): root(nullptr) {}

    BinarySearchTree(const BinarySearchTree & rhs)
    {
        root = clone(rhs.root);
    }

    BinarySearchTree(BinarySearchTree && rhs) noexcept : root(rhs.root)
    {
        rhs.root = nullptr;
    }

    ~BinarySearchTree()
    {
        makeEmpty();
    }

    BinarySearchTree & operator=(const BinarySearchTree & rhs)
    {
        if(this == &rhs)
            return *this;

        BinarySearchTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    BinarySearchTree & operator=(BinarySearchTree && rhs) noexcept
    {
        std::swap(root, rhs.root);
        return *this;
    }

    const Comparable & findMin() const
    {
        if(isEmpty())
            throw UnderflowException();
        return findMin(root)->element;
    }

    const Comparable & findMax() const
    {
        if(isEmpty())
            throw UnderflowException();
        return findMax(root)->element;
    }

    bool contains(const Comparable & x) const
    {
        return contains(x,root);
    }

    bool isEmpty() const
    {
        return root == nullptr;
    }

    void printTree(std::ostream & out = std::cout) const
    {
        if(isEmpty())
            out << "Empty tree" << std::endl;
        else
            printTree(root, out);
    }

    void makeEmpty()
    {
        makeEmpty(root);
    }

    void insert(const Comparable & x)
    {
        insert(x, root);
    }

    void insert(Comparable && x)
    {
        insert(std::move(x), root);
    }

    void remove(const Comparable & x)
    {
        remove(x,root);
    }

private:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode * left;
        BinaryNode * right;

        BinaryNode(const Comparable & x, BinaryNode *lt, BinaryNode *rt)
        : element(x), left(lt), right(rt) {}

        BinaryNode(Comparable && x, BinaryNode *lt, BinaryNode *rt )
        : element(std::move(x)), left(lt), right(rt) {}

    };

    BinaryNode *root;

    void insert(const Comparable & x, BinaryNode * & t)
    {
        if(t == nullptr)
            t = new BinaryNode(x, nullptr, nullptr);
        else if(x < t->element)
            insert(x, t->left);
        else if(t->element < x)
            insert(x, t->right);
        else
            ; // 如果相等，什么都不做，暂时不考虑值相等
    }

    void insert(Comparable && x, BinaryNode * & t)
    {
        if(t == nullptr)
            t = new BinaryNode(std::move(x), nullptr, nullptr);
        else if(x < t->element)
            insert(std::move(x), t->left);
        else if(t->element < x)
            insert(std::move(x), t->right);
        else
            ; // 如果相等，什么都不做，暂时不考虑值相等
    }

    void remove( const Comparable & x, BinaryNode * & t)
    {
        if(t == nullptr)
            return;
        else if(t->element < x)
            remove(x, t->right);
        else if(x < t->element)
            remove(x, t->left);
        else if(t->left != nullptr && t->right != nullptr)
        {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }
        else
        {
            BinaryNode * oldNode = t;
            t = (t->left != nullptr)? t->left:t->right;
            delete oldNode;
        }

    }

    BinaryNode * findMin( BinaryNode *t) const
    {
        if(t == nullptr)
            return nullptr;
        if(t->left == nullptr)
            return t;
        return findMin(t->left);
    }

    BinaryNode * findMax( BinaryNode *t) const
    {
        if(t == nullptr)
            return nullptr;
        if( t->right == nullptr)
            return t;
        return findMax(t->right);
    }

    bool contains( const Comparable & x, BinaryNode *t) const
    {
        if(t == nullptr)
            return false;
        else if(x < t->element)
            return contains(x, t->left);
        else if(t->element < x)
            return contains(x, t->right);
        else
            return true;
    }

    void makeEmpty( BinaryNode * & t)
    {
        if(t == nullptr)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }

    void printTree( BinaryNode *t, std::ostream & out) const
    {
        if(t == nullptr)
            return;
        printTree(t->left, out);
        out<<t->element<<" ";
        printTree(t->right, out);

    }

    BinaryNode * clone( BinaryNode * t) const
    {
        if( t == nullptr)
            return nullptr;
        else
            return new BinaryNode(t->element, clone(t->left), clone(t->right));
    }



};


#endif //DATA_STRUCTURE_CPP_BINARYSEARCHTREE_H
