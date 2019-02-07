#include "PlainALSequence.hpp"
namespace hdt {

PlainALSequence::PlainALSequence(IntSequence *el, BitSequence375 *bit) :
        elements(el),
        bitmap(bit) {

}
PlainALSequence::PlainALSequence(){
    bitmap=new BitSequence375();
    elements=NULL;

}
PlainALSequence::~PlainALSequence() {
    if (bitmap!=NULL)
        delete bitmap;
    delete elements;
}

/**
 * Find pos of the list x
 * @param x
 * @return
 */
size_t PlainALSequence::find(size_t x) {
    if(x<=0) {
        return 0;
    }
#ifdef OLD_BITMAP
    unsigned int first1 = bitmap->select1(x);
    return bitmap->rank0(first1);
#else
    return bitmap->select1(x)+1;
#endif
}

size_t PlainALSequence::findNext(size_t pos)
{
    return bitmap->selectNext1(pos);
}

/**
 * Find element y, in the list x
 * @param x
 * @param y
 * @return
 */
size_t PlainALSequence::find(size_t x, size_t y) {
    size_t begin = find(x);
    size_t end = last(x);
    return binSearch(y, begin, end);
}

/**
 * Find to which list x does the element at globalpos belongs
 * @param globalpos
 * @return
 */
size_t PlainALSequence::findListIndex(size_t globalpos) {
#ifdef OLD_BITMAP
    size_t posz = bitmap->select0(globalpos+1);
    return bitmap->rank1(posz);
#else
    if(globalpos<=0) {
        return 0;
    }
    return bitmap->rank1(globalpos-1);
#endif
}

/**
 * Find last pos of the list x
 * @param x
 * @return
 */
size_t PlainALSequence::last(size_t x) {
#ifdef OLD_BITMAP
    size_t first1 = bitmap->select1(x+1);
    return bitmap->rank0(first1)-1;
#else
    return bitmap->select1(x+1);
#endif
}

/**
 * Count the number of lists in the AdjacencyList
 * @return
 */
size_t PlainALSequence::countListsX() {
#ifdef OLD_BITMAP
    return bitmap->countOnes();
#else
    return bitmap->countOnes();
#endif
}

/**
 * Count the number of items of the list x
 * @param x
 * @return
 */
size_t PlainALSequence::countItemsY(size_t x) {
    return last(x)-find(x)+1;
}

size_t PlainALSequence::search(unsigned int element, size_t begin, size_t end) {
    if(end-begin>10) {
        return binSearch(element,begin,end);
    } else {
        return linSearch(element,begin,end);
    }
}

size_t PlainALSequence::binSearch(unsigned int element, size_t begin, size_t end) {
    while (begin <= end) {
        int mid = (begin + end) / 2;

        unsigned int read = elements->get(mid);

        if (element > read)
            begin = mid + 1;
        else if (element < read)
            end = mid - 1;
        else
            return mid;
    }
    throw "Not found";
}

size_t PlainALSequence::linSearch(unsigned int element, size_t begin, size_t end) {
    while (begin <= end) {
        unsigned int read = elements->get(begin);
        //cout << "\t\tPos: " << begin << " Compare " << element << " with " << read << endl;

        if (element == read)
            return begin;

        begin++;
    }
    return end;
}


unsigned int PlainALSequence::get(size_t pos) {
    return elements->get(pos);
}

size_t PlainALSequence::getSize() {
    return elements->getNumberOfElements();
}

void PlainALSequence::dump() {
    for(unsigned int i=0; i<countListsX() && i<100; i++) {
        cout << " [";
        for(unsigned int j=0; j<countItemsY(i) && i<100; j++) {
            cout << get(find(i)+j);

            if(j!=countItemsY(i)-1)
                cout << ",";
        }
        cout << "] ";
    }
    cout << endl;

#if 0
    cout << "List has " << getSize() << " elements in " << countListsX() << " lists" << endl;

    for(unsigned int i=0; i<countListsX(); i++) {
        cout << "List " << i << " [" << find(i) << ", " << last(i) << "] (" << countItemsY(i)<< ") "<< endl;
        for(unsigned int j=0; j<countItemsY(i); j++) {
            cout << "\tItem " << i << ", "<< j << " => " << get(find(i)+j) << " Found pos: " << find(i, get(find(i)+j)) << " Found list: " << findListIndex(find(i)+j) << endl;
        }
    }
#endif
}

}

