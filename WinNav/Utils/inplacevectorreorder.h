#ifndef INPLACEVECTORREORDER_H
#define INPLACEVECTORREORDER_H

#include <vector>
#include <stddef.h>

/* Utility method to reorder a vector of nodes according to a second vector containing the nex indizes in place.
 * \param A Vector of objects. This vector has to be reordered.
 * \param newIndex A vector of indizes. This vector holds the new positions of the elements in A.
 * Take from: http://stackoverflow.com/questions/22218698/reorder-vector-according-to-a-vector-of-indices-update
 */
template <class T>
void reorderVector (std::vector<T> &A, std::vector<size_t> newIndex) {
    if(A.size() != newIndex.size()) {
        //If both vectors do not have the same length, this method will not work
        return;
    }
    size_t i=0, j=0, k=0;
    T tmp;
    for (i=0; i<A.size(); i++) {
        if(i != newIndex[i]) {
            //The current object is not at the correct place ...
            tmp=A[i];
            k=i;
            j=newIndex[k];
            //So start switching ...
            while (i != j) {
                //... with the object that belongs at this place ...
                A[k]=A[j];
                //... and continue to do so, until all moved objects are in the right place
                newIndex[k]=k;
                k=j;
                j=newIndex[k];
            }
            //We broke up one 'displacement-cycle', now move to the next ...
            A[k] = tmp;
            newIndex[k]=k;
        }
    }
}


#endif // INPLACEVECTORREORDER_H
