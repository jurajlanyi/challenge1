// LANJ: Note - This file will be included from linked_list.h
using namespace std;

template <class ItemType> std::ostream& operator<< (std::ostream &out, const MyList<ItemType>  &list)
{
    MyNode<ItemType> *mi = list.m_first;
    int n = 1;
    out << "List size is " << list.m_items << " items:" << endl;
    while(mi != nullptr) {
        out << "Item " << n++ << "= " << (**mi) << endl;
        mi = mi->next();
    }
    return out;
}

template <class ItemType> MyList<ItemType>::~MyList()
{
    MyNode<ItemType> *next, *iter = first();
    while(iter != nullptr) {
        next = iter->m_next;
        iter->m_next = iter->m_prev = nullptr;
        delete iter;
        iter = next;
        m_items--;
    }
    m_first = nullptr;
    m_last = nullptr;
};

template <class ItemType> void MyList<ItemType>::push_front(const ItemType& item)
{
    MyNode<ItemType> *mn = new MyNode<ItemType>(item);

    if (m_first == nullptr) { // pushing first item
        m_last = mn;
    }
    else {
        m_first->m_prev = mn;
        mn->m_next = m_first;
    }
    m_first = mn;
    m_items++;
}

template <class ItemType> void MyList<ItemType>::push_back(const ItemType& item)
{
    MyNode<ItemType> *mn = new MyNode<ItemType>(item);

    if (m_last == nullptr) { // pushing first item
        m_first = mn;
    }
    else {
        m_last->m_next = mn;
        mn->m_prev = m_last;
    }
    m_last = mn;
    m_items++;
}

template <class ItemType> ItemType MyList<ItemType>::pop_back()
{
    assert(m_items > 0);
    ItemType retVal = **m_last;
    remove(m_last);
    return retVal;
}

template <class ItemType> ItemType MyList<ItemType>::pop_front()
{
    assert(m_items > 0);
    ItemType retVal = **m_first;
    remove(m_first);
    return retVal;
}

template <class ItemType> MyNode<ItemType> * MyList<ItemType>::find(ItemType search)
{
    MyNode<ItemType> *mi = first();
    while(mi != nullptr) {
        if ((**mi) == search)
            return mi;
        mi = mi->m_next;
    }
    return nullptr;
}

template <class ItemType> void MyList<ItemType>::remove(MyNode<ItemType> *element)
{
    MyNode<ItemType> *prev, *next;
    next = element->m_next;
    prev = element->m_prev;

    if (prev)
        prev->m_next = next;
    else
        m_first = next;

    if (next)
        next->m_prev = prev;
    else
        m_last = prev;

    element->m_next = element->m_prev = nullptr;
    delete element;
    m_items--;
}

#pragma message "TODO: check insert_at"

template <class ItemType> void MyList<ItemType>::insert_at(MyNode<ItemType> *iter, const ItemType& item)
{
    if ((m_first == nullptr) || (iter == nullptr)) { // inserting first item, or behind last in the list
        push_back(item);
        return;
    }

    MyNode<ItemType> *mn = new MyNode<ItemType>(item);

    mn->m_next = iter;
    mn->m_prev = iter->m_prev;

    if (iter->m_prev == nullptr) {
        assert(m_first == iter);
        m_first = mn;
    }
    else {
        iter->m_prev->m_next=mn;
    }
    iter->m_prev = mn;

    m_items++;
}

//template <class ItemType> void MyList<ItemType>::insert_at(int idx, const ItemType& item) { }

//LANJ - very inefficient sort

template <class ItemType> void MyList<ItemType>::sort()
{
    if (m_items < 2)
        return;
    iterator el1 = first() , el2;

    while ((el1 != nullptr) && (el1->m_next != nullptr)) {
        el2 = el1->m_next;
        while (el2 != nullptr) {
            if (**el2 < **el1)
                swap(el1, el2);
            el2 = el2->m_next;
        }
        el1 = el1->m_next;
    }
}

template <class ItemType> void MyList<ItemType>::rev_sort()
{
    sort();
    iterator el1 = first() , el2 = last();
    int n = items();
    while(n >= 2) {
        swap(el1, el2);
        el1 = el1->m_next;
        el2 = el2->m_prev;
        n-=2;
    }
}


// LANJ: Value swap - requires, that elements must be copy-able
template <class ItemType> void MyList<ItemType>::swap(MyNode<ItemType> *elem1, MyNode<ItemType> *elem2)
{
   ItemType temp = elem1->item;
   elem1->item = elem2->item;
   elem2->item = temp;
}

