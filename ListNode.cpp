///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
// 
// FILE:    ListNode.cpp
// 
// MODULE:  
// 
// NOTES:   None.
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef LISTNODE_CPP
#define LISTNODE_CPP

#include "ListNode.hpp"

using namespace persistent_skip_list;

///////////////////////////////////////////////////////////////////////////////
// ListNode Implementation                                                   //
///////////////////////////////////////////////////////////////////////////////

template<class T>
bool ListNode<T>::_SEEDED = false;

template<class T>
void ListNode<T>::seed() {
      if(_SEEDED)
	return;
      _SEEDED = true;
      time_t seed = (time_t)1312564825;
      seed = time(0); // comment this for non-random seeding
      srand( seed );
}

template<class T>
void ListNode<T>::initializeNode() {
  seed();
  // pick height, modified from Pat Morin's Open Data Structures
  height = 1;
  int bitCheck = 1;
  int r = rand();
  // check each bit in the binary representation of r, from the
  // least significant to the most significant.  The number of 1's
  // in a row from the least significant position determines the
  // height of the node.
  while((r & bitCheck) != 0) {
    // if bit is 1, increment height
    ++height;
    // check next bit
    bitCheck <<= 1;
  }
}

template<class T>
ListNode<T>::ListNode(const T& original_data)
  : next(), data(original_data), isPositiveInfinity(false), isNegativeInfinity(false)
{
  initializeNode();
}

template<class T>
ListNode<T>::ListNode(const bool positive)
  : next(), data(),
    isPositiveInfinity(positive), isNegativeInfinity(!positive)
{
  initializeNode();
}

template<class T>
ListNode<T>::~ListNode() {
  // clean up next
  while(! next.empty() ) {
    TSA* back = next.back();
    next.pop_back();
    delete back;
  }
}

template<class T>
T ListNode<T>::getData() {
  assert(this != NULL);
  return data;
}

template<class T>
int ListNode<T>::getHeight() {
  assert(this != NULL);
  return height;
}
  
template<class T>
int ListNode<T>::getNextChangeIndex(int t) {
  assert(this != NULL);
  int index = -1;
  int begin = 0, end = numberOfNextChangeIndices() -1;
  int timeFound = 0;
  // binary search
  while(begin <= end) {
    index = (begin+end)/2;
    timeFound = getNextAtIndex(index)->getTime();
    if(timeFound == t) {
      // done, break out of loop
      break;
    } else if(timeFound > t) {
      // repeat binary search on left (earlier) half
      end = index -1;
    } else {
      // repeat binary search on right (later) half
      begin = index +1;
    }
  }
  // report final index
  return index;
}

template <class T>
TimeStampedArray< SmartPointer< ListNode<T> > >*
ListNode<T>::getNextAtIndex(int ci) {
  assert(this != NULL);
  assert(ci >= 0);
  assert(ci < numberOfNextChangeIndices());
  return next[ci];
}

template <class T>
int ListNode<T>::numberOfNextChangeIndices() {
  assert(this != NULL);
  return (int)next.size();
}
  
template <class T>
TimeStampedArray< SmartPointer< ListNode<T> > >*
ListNode<T>::getNext(int t) {
  assert(this != NULL);
  assert(t >= 0);
  // find nearest time
  int index = getNextChangeIndex(t);
  // if there are no pointers, bail
  if(index == -1) return NULL;
  // use this to store the node to be returned
  TSA* tsa = getNextAtIndex(index);
  // since the nearest time might be later than the desired time, we
  // go backwards in time until we reach the point which immediately
  // precedes the query time
  while(tsa->getTime() > t) {
    --index;
    if(index < 0)
      return NULL;
    tsa = getNextAtIndex(index);
  }
  return tsa;
}

template <class T>
int ListNode<T>::addNext(TimeStampedArray< SmartPointer< ListNode<T> > >* tsa) {
  assert(this != NULL);
  // since NULL is the default
  if(tsa == NULL)
    return -1; // bail if trying to set next to NULL
  // make sure time is strictly increasing
  int lastIndex = (int)next.size()-1;
  if(lastIndex >= 0)
    assert(tsa->getTime() > next[lastIndex]->getTime());
  // finally, save the new set of next pointers
  next.push_back(tsa);
  // success
  return 0;
}

template <class T>
bool ListNode<T>::operator<(ListNode<T>& other) {
  if(this->isPositiveInfinity || other.isNegativeInfinity)
    return false;
  if(this->isNegativeInfinity || other.isPositiveInfinity)
    return true;
  return data < other.data;
}

template <class T>
bool ListNode<T>::operator>(ListNode<T>& other) {
  return !(operator<(other));
}

#endif
