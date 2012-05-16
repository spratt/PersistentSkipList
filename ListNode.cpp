///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    ListNode.cpp                                                     //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
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
      srand( time(0) );
}

template<class T>
void ListNode<T>::initializeNode() {
  incoming_nodes = new ListNode<T>*[height];
}

template<class T>
ListNode<T>::ListNode(const T& original_data)
  : height(1), next(), data(original_data), 
    _isPositiveInfinity(false), _isNegativeInfinity(false)
{
  if(!_SEEDED)
    seed();
  // pick height, modified from Pat Morin's Open Data Structures
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
  initializeNode();
}

template<class T>
ListNode<T>::ListNode(const bool positive, int h=1)
  : height(h), next(), data(),
    _isPositiveInfinity(positive), _isNegativeInfinity(!positive)
{
  assert(h > 0);
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
  delete [] incoming_nodes;
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
void ListNode<T>::setIncoming(int h, ListNode<T>* in) {
  assert(this != NULL);
  assert(h >= 0);
  assert(h < height);
  incoming_nodes[h] = in;
}

template <class T>
ListNode<T>* ListNode<T>::getIncoming(int h) {
  assert(this != NULL);
  assert(h >= 0);
  assert(h < height);
  return incoming_nodes[h];
}

template <class T>
int ListNode<T>::addNext(TimeStampedArray< SmartPointer< ListNode<T> > >* tsa) {
  assert(this != NULL);
  // since NULL is the default
  if(tsa == NULL)
    return -1; // bail if trying to set next to NULL
  // make sure time is strictly increasing
  int lastIndex = (int)next.size()-1;
  assert(lastIndex < 0 || tsa->getTime() >= next[lastIndex]->getTime());
  if(lastIndex >= 0 && tsa->getTime() == next[lastIndex]->getTime()) {
    TSA* prev = next[lastIndex];
    next[lastIndex] = tsa;
    delete prev;
  } else {
    // finally, save the new set of next pointers
    next.push_back(tsa);
  }
  for(int i = 0; i < height; ++i)
    tsa->getElement(i)->setIncoming(i,this);
  // success
  return 0;
}

template <class T>
bool ListNode<T>::operator<(ListNode<T>& other) {
  if(this->_isPositiveInfinity || other._isNegativeInfinity)
    return false;
  if(this->_isNegativeInfinity || other._isPositiveInfinity)
    return true;
  return data < other.data;
}

template <class T>
bool ListNode<T>::operator>(ListNode<T>& other) {
  return !(operator<(other));
}

template <class T>
bool ListNode<T>::isPositiveInfinity() {
  return _isPositiveInfinity;
}

template <class T>
bool ListNode<T>::isNegativeInfinity() {
  return _isNegativeInfinity;
}

#endif
