//
// Created by 胡凌瑞 on 2020/7/1.
//

#ifndef DATA_STRUCTURE_CPP_SPLAYTREE_H
#define DATA_STRUCTURE_CPP_SPLAYTREE_H

#include <iostream>
#include <algorithm>
#include "dsexceptions.h"

/**
 * 自底向上伸展树
 * @tparam Comparable
 */
template<typename Comparable>
class SplayTree {

public:

    explicit SplayTree() : root(nullptr) {}

    ~SplayTree() {
        makeEmpty();
    }

    SplayTree(const SplayTree &rhs) {
        root = clone(rhs.root);
    }

    SplayTree(SplayTree &&rhs) noexcept: root(rhs.root) {
        rhs.root = nullptr;
    }

    SplayTree &operator=(const SplayTree &rhs) {
        if (this == &rhs)
            return *this;

        SplayTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    SplayTree &operator=(SplayTree &&rhs) noexcept {
        std::swap(root, rhs.root);
        return *this;
    }

    void makeEmpty() {
        makeEmpty(root);
    }

    void insert(const Comparable &x) {
        insert(x, root);
    }

    void insert(Comparable &&x) {
        insert(std::move(x), root);
    }

    void remove(const Comparable &x) {
        //此处已经执行了伸展操作，如果存在x，则已经到达根节点
        if (!contains(x))
            throw UnderflowException();
            //树中有x
        else {
            //左子树
            BinaryNode *lt = root->left;
            //右子树
            BinaryNode *rt = root->right;
            //要被删除的节点
            BinaryNode *oldNode = root;

            //只有根节点的情况
            if (lt == nullptr && rt == nullptr) {
                delete oldNode;
                root = nullptr;
            }
                //只有右子树
            else if (lt == nullptr) {
                root = rt;
                delete oldNode;
            }
                //只有左子树
            else if (rt == nullptr) {
                root = lt;
                delete oldNode;
            }
                //既有左子树，又有右子树
            else {
                //把左子树的最大值伸展到根节点处
                splay(findMax(lt)->element, lt);
                //伸展结束后，左子树已经没有右儿子
                lt->right = rt;
                root = lt;
                delete oldNode;
            }
        }
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    void printTree(std::ostream &out = std::cout) const {
        if (isEmpty())
            out << "Empty tree" << std::endl;
        else
            printTree(root, out);
    }

    bool contains(const Comparable &x) {
        if (isEmpty())
            return false;
        splay(x, root);
        return x == root->element;
    }

    const Comparable &findMin() const {
        if (isEmpty())
            throw UnderflowException{};

        BinaryNode *ptr = root;

        while (ptr->left != nullptr)
            ptr = ptr->left;

        splay(ptr->element, root);
        return ptr->element;
    }

    const Comparable &findMax() const {
        if (isEmpty())
            throw UnderflowException{};

        BinaryNode *ptr = root;

        while (ptr->right != nullptr)
            ptr = ptr->right;

        splay(ptr->element, root);
        return ptr->element;
    }

private:

    struct BinaryNode {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode(const Comparable &x, BinaryNode *lt, BinaryNode *rt) : element(x), left(lt), right(rt) {}

        BinaryNode(Comparable &&x, BinaryNode *lt, BinaryNode *rt) : element(std::move(x)), left(lt), right(rt) {}

    };

    BinaryNode *root;

    void insert(const Comparable &x, BinaryNode *&t) {
        if (t == nullptr)
            t = new BinaryNode(x, nullptr, nullptr);
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);
        else; // 如果相等，什么都不做，暂时不考虑值相等
    }

    void insert(Comparable &&x, BinaryNode *&t) {
        if (t == nullptr)
            t = new BinaryNode(std::move(x), nullptr, nullptr);
        else if (x < t->element)
            insert(std::move(x), t->left);
        else if (t->element < x)
            insert(std::move(x), t->right);
        else; // 如果相等，什么都不做，暂时不考虑值相等
    }

    void makeEmpty(BinaryNode *&t) {
        if (t == nullptr)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }

    void splay(const Comparable &x, BinaryNode *&t) {
        if (t == nullptr)
            return;
        else if (x < t->element)
            splay(x, t->left);
        else if (x > t->element)
            splay(x, t->right);

        //zig-zig
        if (t->left && t->left->left && x == t->left->left->element) {
            rotateWithLeftChild(t);
            rotateWithLeftChild(t);
        }
            //zag-zag
        else if (t->right && t->right->right && x == t->right->right->element) {
            rotateWithRightChild(t);
            rotateWithRightChild(t);
        }
            //zig-zag
        else if (t->left && t->left->right && x == t->left->right->element)
            doubleWithLeftChild(t);
            //zag-zig
        else if (t->right && t->right->left && x == t->right->left->element)
            doubleWithRightChild(t);

            //目标元素的父亲为根的情况
        else if (root->left && x == root->left->element)
            rotateWithLeftChild(root);
        else if (root->right && x == root->right->element)
            rotateWithRightChild(root);

            //其余均跳过
        else
            return;

    }

    void printTree(BinaryNode *t, std::ostream &out) const {
        if (t == nullptr)
            return;
        printTree(t->left, out);
        //中序遍历
        //out<<t->element<<" ";
        printTree(t->right, out);
        //后序遍历用于观察是否进行了正确的伸展
        out << t->element << " ";
    }

    BinaryNode *clone(BinaryNode *t) const {
        if (t == nullptr)
            return nullptr;
        else
            return new BinaryNode(t->element, clone(t->left), clone(t->right));
    }

    /**
    * 单旋转
    * @param k2
    */
    void rotateWithLeftChild(BinaryNode *&k2) {
        BinaryNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2 = k1;
    }

    /**
     * 单旋转
     * @param k1
     */
    void rotateWithRightChild(BinaryNode *&k1) {
        BinaryNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1 = k2;
    }

    /**
     * 双旋转
     * @param k3
     */
    void doubleWithLeftChild(BinaryNode *&k3) {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    /**
     * 双旋转
     * @param k1
     */
    void doubleWithRightChild(BinaryNode *&k1) {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }


};


#endif //DATA_STRUCTURE_CPP_SPLAYTREE_H
