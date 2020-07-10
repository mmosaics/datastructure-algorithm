//
// Created by 胡凌瑞 on 2020/6/29.
//

#ifndef DATA_STRUCTURE_CPP_LIST_H
#define DATA_STRUCTURE_CPP_LIST_H

#include <algorithm>

template <typename Object>
class List{

private:
    struct Node
    {
        Object element;
        Node * next;
        Node * prev;
        explicit Node(const Object & ele = Object{}, Node * n = nullptr, Node * p = nullptr): element(ele), next(n), prev(p) {}

        explicit Node(Object && ele, Node * n = nullptr, Node * p = nullptr): element(std::move(ele)), next(n), prev(p) {}

    };

public:

    class const_iterator
    {
    public:

        explicit const_iterator() : currentNode(nullptr) {}

        const_iterator & operator++()
        {
            currentNode = currentNode->next;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator copy = *this;
            currentNode = currentNode->next;
            return copy;
        }

        const_iterator & operator--()
        {
            currentNode = currentNode->prev;
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator copy = *this;
            currentNode = currentNode->prev;
            return copy;
        }

        bool operator==(const_iterator & rhs)
        {
            return currentNode == rhs.currentNode;
        }

        bool operator!=(const_iterator & rhs)
        {
            return currentNode != rhs.currentNode;
        }

        const Object & operator* ( ) const
        { return retrieve( ); }


    protected:
        Node * currentNode;
        /**
         * 取到当前的iterator指向的Object;
         * @return
         */
        Object & retrieve()
        {
            return currentNode->element;
        }

        explicit const_iterator( Node *p ) :currentNode{ p }
        { }

        friend class List<Object>;

    };

    class iterator: public const_iterator
    {

    public:
        explicit iterator()= default;

        Object & operator* ( )
        { return const_iterator::retrieve( ); }

        const Object & operator* ( ) const
        { return const_iterator::operator*( ); }

        iterator & operator++()
        {
            this->currentNode = this->currentNode->next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator copy = *this;
            ++(*this);
            return copy;
        }

        iterator & operator--()
        {
            this->currentNode = this->currentNode->prev;
            return *this;
        }

        iterator operator-- ( int )
        {
            iterator old = *this;
            --( *this );
            return old;
        }

    protected:
        explicit iterator( Node *p ) : const_iterator{ p }
        { }

        friend class List<Object>;


    };

public:

    explicit List(int size = 0): theSize(size)
    {
        init();
    }

    List(const List & rhs): theSize(rhs.theSize)
    {
        init();
        for(auto & n : rhs)
        {
            push_front(n);
        }
    }

    List(List && rhs) noexcept : theSize(rhs.theSize), head(rhs.head), tail(rhs.tail)
    {
        rhs.theSize = 0;
        rhs.tail = nullptr;
        rhs.head = nullptr;
    }

    List & operator=(const List & rhs)
    {
        if(this == &rhs)
            return *this;

        List copy = rhs;
        std::swap(*this, copy);
        return *this;

    }

    List & operator=(List && rhs)
 noexcept     {
        std::swap(theSize, rhs.theSize);
        std::swap(head, rhs.head);
        std::swap(tail, rhs.tail);

        return *this;
    }

    ~List()
    {
        clear();
        delete head;
        delete tail;
    }

    iterator begin()
    {
        return iterator(head->next);
    }

    const_iterator begin() const
    {
        return const_iterator(head->next);
    }

    iterator end()
    {
        return iterator(tail);
    }

    const_iterator end() const
    {
        return const_iterator(tail);
    }

    int size() const
    {
        return theSize;
    }

    bool empty() const
    {
        return theSize == 0;
    }

    void clear()
    {
        erase(iterator(head->next), iterator(tail));
    }

    Object & front()
    {
        return head->next->element;
    }

    const Object & front() const
    {
        return head->next->element;
    }

    Object & back()
    {
        return tail->prev->element;
    }

    const Object & back() const
    {
        return tail->prev->element;
    }

    void push_front(const Object & x)
    {
        insert(iterator(head->next), x);
        theSize++;
    }

    void push_front(Object && x)
    {
        insert(iterator(head->next), std::move(x));
        theSize++;
    }

    void push_back(const Object & x)
    {
        insert(iterator(tail), x);
        theSize++;
    }

    void push_back(Object && x)
    {
        insert(iterator(tail), std::move(x));
        theSize++;
    }

    void pop_front()
    {
        erase(iterator(head->next));
        theSize--;
    }

    void pop_back()
    {
        erase(iterator(tail->prev));
        theSize--;
    }

    // Insert x before itr.
    iterator insert(iterator itr, const Object & x)
    {
        Node *p = itr.currentNode;
        Node * newNode = new Node(x, p, p->prev);
        p->prev->next = newNode;
        p->prev = newNode;
        return iterator(newNode);

    }

    // Insert x before itr.
    iterator insert(iterator itr, Object && x)
    {
        Node *p = itr.currentNode;
        Node * newNode = new Node(std::move(x), p, p->prev);
        p->prev->next = newNode;
        p->prev = newNode;
        return iterator(newNode);
    }

    iterator erase(iterator itr)
    {
        Node * p = itr.currentNode;

        p->prev->next = p->next;
        p->next->prev = p->prev;

        iterator ret(p->next);

        delete p;

        return ret;

    }

    iterator erase(iterator from, iterator to)
    {
        for(iterator itr = from; itr != to;)
        {
            itr = erase(itr);
        }

        return to;

    }


private:

    int theSize;
    Node * head;
    Node * tail;

    void init()
    {
        theSize = 0;
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }


};

#endif //DATA_STRUCTURE_CPP_LIST_H
