//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    heap_sort.h                                                      //
//                                                                           //
// MODULE:  Sort                                                             //
//                                                                           //
// PURPOSE: Provides an implementation of heap sort.                         //
//                                                                           //
// NOTES:   - in-place                                                       //
//          - unstable                                                       //
//          - worst case time complexity: O(nlogn)                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
//                                                                           //
// heap_sort(T* array, int npoints)                                          //
// heap_sort(T* array, int begin, int end)                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include "../array_utilities.h"

namespace sort {
  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: leftChildOf                                              //
  //                                                                         //
  // PURPOSE:       Given the (base zero) index of an element in an array,   //
  //                return the index of the left child of that               //
  //                element, assuming the array represents a tree            //
  //                stored linearly.                                         //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   int/index                                                //
  //   Description: The index of the parent.                                 //
  //                                                                         //
  // RETURN:                                                                 //
  //   Type/Name:   int                                                      //
  //   Description: The index of the left child.                             //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  int leftChildOf(int index) {
    return 2*index+1;
  }
  /////////////////////////////////////////////////////////////////////////////
  // Same as above, but right child                                          //
  /////////////////////////////////////////////////////////////////////////////
  int rightChildOf(int index) {
    return 2*index+2;
  }

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: buildHeap                                                //
  //                                                                         //
  // PURPOSE:       Builds a heap from bottom up, starting at right-most     //
  //                lowest level and traversing up the heap in reverse       //
  //                order (right to left, bottom to top).                    //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   T*/array                                                 //
  //   Description: An array of type T, to be sorted.  The comparison        //
  //                operator must defined on this type.                      //
  //                                                                         //
  //   Type/Name:   int/begin                                                //
  //   Description: The index at which to begin sorting.                     //
  //                                                                         //
  //   Type/Name:   int/end                                                  //
  //   Description: The index at which to end sorting.                       //
  //                                                                         //
  // RETURN:        Void.                                                    //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  template<class T>
  void buildHeap(T* array, int begin, int end) {
    int n = 1 + end - begin;
    for(int v = n/2-1; v >= 0; v--)
      downHeap(array,v,begin,end);
  }

  ///////////////////////////////////////////////////////////////////////////////
  //                                                                           //
  // FUNCTION NAME: downHeap                                                   //
  //                                                                           //
  // PURPOSE:       builds a heap from top down                                //
  //                                                                           //
  // PARAMETERS                                                                //
  //   Type/Name:   T*/array                                                   //
  //   Description: The array of type T.                                       //
  //                                                                           //
  //   Type/Name:   int/v                                                      //
  //   Description: The offset from begin from which to begin building         //
  //                the heap.                                                  //
  //                                                                           //
  //   Type/Name:   int/begin                                                  //
  //   Description: The first element in the array to be sorted.               //
  //                                                                           //
  //   Type/Name:   int/end                                                    //
  //   Description: The last element in the array to be sorted.                //
  //                                                                           //
  // RETURN:        Void.                                                      //
  //                                                                           //
  // NOTES:         Note that indices are base zero, i.e. the first            //
  //                element has index 0, the second element has index 1...     //
  //                                                                           //
  ///////////////////////////////////////////////////////////////////////////////
  template<class T>
  void downHeap(T* array, int v, int begin, int end) {
    int w = leftChildOf(v);
    // invariant: element at index v has a left child
    while(begin + w <= end) {
      // if left child has a sibling
      if(begin + w+1 <= end)
	// if right child is greater than left child
	if(array[begin + w+1] > array[begin + w])
	  // use right child
	  w++;
      // if larger child is less than its parent
      if(!(array[begin + w] > array[begin + v]))
	// done
	return;
      // otherwise, swap child and parent
      array_utilities::swap(array,begin + w,begin + v);
      // continue with child
      v = w;
      w = leftChildOf(v);
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: heap_sort                                                //
  //                                                                         //
  // PURPOSE:       Sorts an array into ascending order using the heap sort  //
  //                algorithm.                                               //
  //                                                                         //
  // SECURITY:      Public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   T*/array                                                 //
  //   Description: An array of type T, to be sorted.  The comparison        //
  //                operator must defined on this type.                      //
  //                                                                         //
  //   Type/Name:   int/begin                                                //
  //   Description: The index at which to begin sorting.                     //
  //                                                                         //
  // RETURN:        Void.                                                    //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  template<class T>
  void heap_sort(T* array, int npoints) {
    heap_sort<T>(array,0,npoints-1);
  }

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: heap_sort                                                //
  //                                                                         //
  // PURPOSE:       Sorts an array using the heap sort algorithm.            //
  //                                                                         //
  // SECURITY:      Public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   T*/array                                                 //
  //   Description: An array of type T, to be sorted.  The comparison        //
  //                operator must defined on this type.                      //
  //                                                                         //
  //   Type/Name:   int/begin                                                //
  //   Description: The index at which to begin sorting.                     //
  //                                                                         //
  //   Type/Name:   int/end                                                  //
  //   Description: The index at which to end sorting.                       //
  //                                                                         //
  // RETURN:        Void.                                                    //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  template<class T>
  void heap_sort(T* array, int begin, int end) {
    // First arrange the array into a heap (root element is always higher
    // than both child elements
    buildHeap(array,begin,end);
    while(end > begin) {
      // Since the highest is first, move it to the end
      array_utilities::swap(array,begin,end);
      // Now that the highest element is last, it is sorted so don't
      // touch it again
      end--;
      // Since the smallest element is now first, rebuild the heap
      downHeap(array,0,begin,end);
    }
  }
}

#endif
