#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

template <class ItemType> class MyList; // forward declaration

template <class ItemType> class MyNode
{
public:
    MyNode(const MyNode &other) = delete;
    MyNode& operator=(const MyNode &other) = delete;

    MyNode(const ItemType &it) : item(it), m_next(nullptr), m_prev(nullptr) { };

    ItemType& operator*() {return item; };

    MyNode<ItemType> *next() const { return m_next; };
    MyNode<ItemType> *prev() const { return m_prev; };
private:
    friend class MyList<ItemType>;

    ItemType item;
    MyNode *m_next, *m_prev;
};


template <class ItemType> std::ostream& operator<< (std::ostream &out, const MyList<ItemType>  &item);

template <class ItemType> class MyList
{
public:
    MyList() : m_first(nullptr), m_last(nullptr), m_items(0) {};
    ~MyList();
    MyList(const MyList &other) = delete;                   // LANJ will be implemented
    MyList& operator=(const MyList &other) = delete;        // LANJ will be implemented

    typedef MyNode<ItemType>* iterator;

    void push_back(const ItemType& item);
    void push_front(const ItemType& item);
    int items() {return m_items; };

    MyNode<ItemType> *first() { return m_first; };
    MyNode<ItemType> *last() { return m_last; };

    MyNode<ItemType> *begin() { return m_first; };
    MyNode<ItemType> *end() { return m_last; };

    void insert_at(MyNode<ItemType> *iter, const ItemType& item);
//    void insert_at(int idx, const ItemType& item);

    MyNode<ItemType> *find(ItemType search);
    void remove(MyNode<ItemType> *element);
    void swap(MyNode<ItemType> *elem1, MyNode<ItemType> *elem2);

    ItemType pop_back();
    ItemType pop_front();

    void sort();
    void rev_sort();
    void quick_sort(iterator from = nullptr, iterator to = nullptr, int elements = 0);

//    void shuffle(); // TODO

private:

    MyNode<ItemType> *m_first, *m_last;
    int m_items;

    // LANJ: This signature includes a diamond <>
    friend std::ostream& operator<< <>(std::ostream &out, const MyList<ItemType>  &item);
};

// Needed for template instantiation
#include "linked_list.cpp"

#endif // LINKED_LIST_H
