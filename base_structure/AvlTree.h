//
// Created by 胡凌瑞 on 2020/7/1.
//

#ifndef DATA_STRUCTURE_CPP_AVLTREE_H
#define DATA_STRUCTURE_CPP_AVLTREE_H

#include <algorithm>
#include <iostream>
#include "dsexceptions.h"

template <typename Comparable>
class AvlTree
{

public:

    explicit AvlTree(): root(nullptr) {}

    ~AvlTree()
    {
        makeEmpty();
    }

    AvlTree(const AvlTree & rhs)
    {
        root = clone(rhs.root);
    }

    AvlTree(AvlTree && rhs) noexcept : root(rhs.root)
    {
        rhs.root = nullptr;
    }

    AvlTree & operator=(const AvlTree & rhs)
    {
        if(this == &rhs)
            return *this;

        AvlTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    AvlTree & operator=( AvlTree && rhs) noexcept
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
        return contains(x, root);
    }

    bool isEmpty() const
    {
        return root == nullptr;
    }

    void printTree() const
    {
        if(isEmpty())
            std::cout<< "Empty Tree" <<std::endl;
        else
            printTree(root);
    }

    void makeEmpty()
    {
        makeEmpty(root);
    }

    void insert(const Comparable & x)
    {
        insert(x,root);
    }

    void insert( Comparable && x)
    {
        insert(std::move(x), root);
    }

    void remove( const Comparable & x)
    {
        remove(x, root);
    }

private:
    struct AvlNode
    {
        Comparable element;
        AvlNode *left;
        AvlNode *right;
        int height;

        AvlNode(const Comparable & ele, AvlNode *lt, AvlNode *rt, int h = 0): element(ele), left(lt), right(rt), height(h) {}

        AvlNode(Comparable && ele, AvlNode *lt, AvlNode *rt, int h = 0): element(std::move(ele)), left(lt), right(rt), height(h) {}

    };

    AvlNode * root;

    int height( AvlNode * t) const
    {
        return t == nullptr ? -1 : t->height;
    }

    int max(int  a, int  b) const
    {
        return a > b ? a : b;
    }

    AvlNode * clone(AvlNode * t)
    {
        if(t == nullptr)
            return nullptr;
        else
            return new AvlNode(t->element, clone(t->left), clone(t->right), t->height);
    }

    void printTree( AvlNode * t) const
    {
        if( t != nullptr)
        {
            printTree(t->left);
            std::cout << t->element << " ";
            printTree(t->right);
        }
    }

    AvlNode * findMin(AvlNode * t) const
    {
        if(t == nullptr)
            return nullptr;
        if(t->left == nullptr)
            return t;
        return findMin(t->left);
    }

    AvlNode * findMax(AvlNode * t) const
    {
        if(t == nullptr)
            return nullptr;
        if(t->right == nullptr)
            return t;
        return findMax(t->right);
    }

    bool contains(const Comparable & x, AvlNode *t) const
    {
        if(t == nullptr)
            return false;
        else if(x < t->element)
            return contains(x ,t->left);
        else if(t->element < x)
            return contains(x ,t->right);
        else
            return true;

    }

    void makeEmpty(AvlNode * & t)
    {
        if(t == nullptr)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }


    void insert(const Comparable & x, AvlNode * & t)
    {
        if( t == nullptr )
            t = new AvlNode{ x, nullptr, nullptr };
        else if( x < t->element )
            insert( x, t->left );
        else if( t->element < x )
            insert( x, t->right );

        balance(t);

    }

    void insert(Comparable && x, AvlNode * & t)
    {
        if(t == nullptr)
            t = new AvlNode(std::move(x), nullptr, nullptr);
        else if(x < t->element)
            insert(std::move(x), t->left);
        else if(x > t->element)
            insert(std::move(x), t->right);
        balance(t);
    }

    void remove(const Comparable & x, AvlNode * & t)
    {
        if(t == nullptr)
            return;
        if(x < t->element)
            remove(x, t->left);
        else if( t->element < x)
            remove(x, t->right);
        else if(t->left != nullptr && t->right != nullptr)
        {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }
        else
        {
            AvlNode * oldNode = t;
            t = (t->left != nullptr)? t->left:t->right;
            delete oldNode;
        }

        balance(t);
    }

    static const int ALLOWED_IMBALANCE = 1;

    // Assume t is balanced or within one of being balanced
    void balance( AvlNode * & t )
    {
        if( t == nullptr )
            return;

        if( height( t->left ) - height( t->right ) > ALLOWED_IMBALANCE )
            if( height( t->left->left ) >= height( t->left->right ) )
                rotateWithLeftChild( t );
            else
                doubleWithLeftChild( t );
        else
        if( height( t->right ) - height( t->left ) > ALLOWED_IMBALANCE )
            if( height( t->right->right ) >= height( t->right->left ) )
                rotateWithRightChild( t );
            else
                doubleWithRightChild( t );

        t->height = max( height( t->left ), height( t->right ) ) + 1;
    }




    /**
     * 单旋转
     * @param k2
     */
    void rotateWithLeftChild( AvlNode * & k2 )
    {
        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), k2->height ) + 1;
        k2 = k1;
    }

    /**
     * 单旋转
     * @param k1
     */
    void rotateWithRightChild( AvlNode * & k1 )
    {
        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k2->height = max( height( k2->right ), k1->height ) + 1;
        k1 = k2;
    }


    /**
     * 双旋转
     * @param k3
     */
    void doubleWithLeftChild( AvlNode * & k3 )
    {
        rotateWithRightChild( k3->left );
        rotateWithLeftChild( k3 );
    }

    /**
     * 双旋转
     * @param k1
     */
    void doubleWithRightChild( AvlNode * & k1 )
    {
        rotateWithLeftChild( k1->right );
        rotateWithRightChild( k1 );
    }





};

#endif //DATA_STRUCTURE_CPP_AVLTREE_H
