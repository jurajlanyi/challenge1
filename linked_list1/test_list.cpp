// Simple tests
#include <iostream>
#include <list>
#include <string>

#include <assert.h>

using namespace std;

#include "linked_list.h"

class Item
{
public:
    Item(int m) : memb(m) {};
    int  value() const { return memb; };
    bool operator==(const Item &it) {return (memb==it.memb); };
    bool operator<(const Item &it) {return (memb<it.memb); };
    bool operator>(const Item &it) {return (memb>it.memb); };
private:

    int memb;
};

std::ostream& operator<< (std::ostream &out, const Item &item)
{
    out << item.value();
    return out;
}

// whether dynamically allocated
//#define DYNAMIC_LIST

int main()
{
    cout << "Simple test for linked_list" << endl;

#ifdef DYNAMIC_LIST
    MyList<Item> *ml_ptr = new MyList<Item>;
    MyList<Item> &my_list = *ml_ptr;
#else
    MyList<Item> my_list;
#endif

    Item item(211);
    my_list.push_back(item);

    my_list.push_back(112);
    my_list.push_back(113);
    my_list.push_front(3);
    my_list.push_front(2);
    my_list.push_front(1);

    // add more elements
    const int X_SIZE = 20;
    int xarray[X_SIZE]={17, 24, 10, 28, 13, 21, 222, -6, 2, 15, 6, 99, 5, 4, 7, 7, 4, 1, 7, 1, };

    MyNode<Item> *where = my_list.first()->next(); // 2nd position

    for (int i=0; i<X_SIZE; i++) { // add them in 3 different ways
        my_list.push_back(xarray[i]);
        my_list.push_front(xarray[i]);
        my_list.insert_at(where, xarray[i]); // fancy effect, as the list item 'where' is moving
    }

    cout << "My list display. Forward direction. Size is " << my_list.items() << endl;

//    MyNode<Item> *mi = ml.first();
//    while (mi != nullptr) {
//        cout << "Member " << (**mi).value() << endl;
//        mi = mi->next();
//    }

    for (MyNode<Item> *it = my_list.first(); it != nullptr; it=it->next()) {
        cout << "Member " << (**it).value() << endl;
    }

    cout << "My list display. Reverse" << endl;
    MyNode<Item> *mi;
    mi = my_list.last();
    while(mi != nullptr) {
        cout << "Member " << (**mi) << endl;
        mi = mi->prev();
    }

    Item searchItemD(211);
    mi = my_list.find(searchItemD);
    if (mi)
        my_list.remove(mi);

    Item searchItem(3);
    mi = my_list.find(searchItem);

    Item searchItem2(113);
    MyNode<Item> *mi2;
    mi2 = my_list.find(searchItem2);
    my_list.swap(mi, mi2);

    cout << "My list display. AFTER SWAP. Forward" << endl;
    cout << my_list; // Pretty print

    my_list.rev_sort();
//    ml.quick_sort(); // not done yet

    cout << "After sort: pretty print" << endl << my_list << endl;

    MyList<Item> my_list2;

    cout << "Empty via pop_back()" << endl;
    while(my_list.items() > 0) {
        Item it = my_list.pop_back();
        cout << it << endl;
    }
#ifdef DYNAMIC_LIST
    delete ml_ptr;
#endif

    return 0;
}
