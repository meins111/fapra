#ifndef INPLACEVECTORREORDER_H
#define INPLACEVECTORREORDER_H

#include <vector>
#include <stddef.h>

/* Utility method to reorder one vector using another */
/*template <class T>
void reorderVector (std::vector<T> &A, const std::vector<size_t> &oldIndex) {
    if(A.size() != oldIndex.size()) {
        //If both vectors do not have the same length, this method will not work
        return;
    }
    size_t i=0;
    std::vector<T> oldOrder (A);

    for (i=0; i<A.size(); i++) {
        A[i]=oldOrder[oldIndex[i]];
    }
}*/


/* Utility method to reorder vector A according to the indices of vector I
 * \param A The vector to be reordered in-place.
 * \param I The vector containing the new order.
 *  I[0]=x := After reordering A[0] holds the A[x] element of the old order.
 */
template <class T>
void inplaceReorder(std::vector<T> &A, std::vector<size_t> &I) {
    if (A.size()!= I.size()) {
        return;
    }
    T tmp;
    size_t j=0, k=0;
    for (size_t it=0; it<A.size(); it++) {
        tmp=A[it];
        j=it;
        while (true) {
            k=I[j];
            I[j]=j;
            if (k==it) {
                //Found finaly position
                break;
            }
            A[j]=A[k];
            j=k;
        }
        //Insert into final position
        A[j]=tmp;
    }
}

#endif // INPLACEVECTORREORDER_H
