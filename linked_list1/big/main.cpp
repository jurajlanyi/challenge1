#include <iostream>
#include <list>
#include <string>

#include <assert.h>

using namespace std;

#define USE_MY_LINKED_LIST

#ifdef USE_MY_LINKED_LIST

#include "linked_list.h"

class Item
{
public:
    Item(int m) : memb(m) {};
    int  value() const { return memb; };
    bool operator==(const Item &it) {return (memb==it.memb); };
    bool operator<(const Item &it) {return (memb<it.memb); };
    bool operator>(const Item &it) {return (memb>it.memb); };

    int memb;
};
#else

class Item : public std::string
{
public:
    Item() = default;
    Item(int n) : string(to_string(n)) {};
    Item(string s) : string(s) {};
//    bool operator ==(const Item &it)

public:
/*
    Item(int m) : memb(m) {};
    value() { return memb; };

    int memb;
*/
};
#endif

#ifdef USE_MY_LINKED_LIST
//std::ostream& operator<< (std::ostream &out, const Item &item);
std::ostream& operator<< (std::ostream &out, const Item &item)
{
    out << item.value();
    return out;
}
#endif

//void std_list_skuska();
//MyList<Item>;

#if 1
int main()
{
//    MyList<Item> *ml_ptr = new MyList<Item>();
//    MyList<Item> &ml (*ml_ptr);
    MyList<Item> ml;

//    Item itx(22);
//    MyNode<Item> mn(itx);


//    std_list_skuska();

#ifndef USE_MY_LINKED_LIST
    Item it(string("111"));
    ml.push_back(it);
    it = string("1234");
    ml.push_back(it);
#else
#endif

    ml.push_back(112);
    ml.push_back(113);
    ml.push_front(3);
    ml.push_front(2);
    ml.push_front(1);

    const int xarray_size = 12;
    int xarray[xarray_size]={17, 24, 10, 28, 13, 21, 222, -6, 2, 15, 6, 99};

    MyNode<Item> *where = ml.first()->next();
//    MyNode<Item> *where = ml.first();

    for (int i=0;i<12;i++) {
//        ml.push_front(xarray[i]);
        ml.insert_at(where, xarray[i]); // TODO does not work
//        ml.insert_at(nullptr, xarray[i]);
    }

    cout << "My list display. Forward" << endl;

    MyNode<Item> *mi = ml.first();
    while(mi != nullptr) {
#ifndef USE_MY_LINKED_LIST
        cout << "Member " << (**mi) << endl;
#else
        cout << "Member " << (**mi).value() << endl;
#endif
        mi = mi->next();
    }

    cout << "My list display. Reverse" << endl;
    mi = ml.last();
    while(mi != nullptr) {
//        cout << "Member " << (**mi).value() << endl;
        cout << "Member " << (**mi) << endl;
        mi = mi->prev();
    }

#if 0
// LANJ: todo implement this
// iterators would be reworked
// now neither of them works

    for(auto i : ml) {
        cout << *i << " " << endl;
    }

    for(auto &i : ml) {
        cout << *i << " " << endl;
    }

#endif

    Item searchItem(3);
    mi = ml.find(searchItem);
#if 0
    if (mi)
        ml.remove(mi);
#endif

    Item searchItem2(113);
    MyNode<Item> *mi2;
    mi2 = ml.find(searchItem2);

    ml.swap(mi, mi2);

    //cout << "My list display. AFTER DELETE. Forward" << endl;

    cout << "My list display. AFTER SWAP. Forward" << endl;

#if 0
    mi = ml.first();
    while(mi != nullptr) {
        cout << "Member " << (**mi).value() << endl;
        mi = mi->m_next;
    }
#endif

#if 0
    mi = ml.first();
    while(mi != nullptr) {
        cout << (**mi) << endl;
        mi = mi->m_next;
    }
#endif

    ml.rev_sort();
//    ml.quick_sort();

    cout << "After sort" << endl << ml << endl;

    cout << "Empty via pop_back()" << endl;
    while(ml.items() > 0) {
        Item it = ml.pop_back();
        cout << it << endl;
    }
//    delete ml_ptr;

    return 0;
}
#endif

