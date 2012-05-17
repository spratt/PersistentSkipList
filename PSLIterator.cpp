///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PSLIterator.cpp                                                  //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PSLITERATOR_CPP
#define PSLITERATOR_CPP

#include "PSLIterator.hpp"

using namespace persistent_skip_list;

template < class T >
PSLIterator<T>::PSLIterator(SmartPointer<ListNode<T> >& node,
			    int time,
			    int height)
: _node(node), _time(time), _height(height)
{
  assert(time >= 0);
  assert(height >= 0);
}

template < class T >
PSLIterator<T>::~PSLIterator() {
}

template < class T >
void PSLIterator<T>::getNext() {
  TimeStampedArray<SmartPointer<ListNode<T> > >* next = _node->getNext(_time);
  assert(next != NULL);
  _node = next->getElement(_height);
  assert(_node != NULL);
}

template < class T >
PSLIterator<T>& PSLIterator<T>::operator++() {
  getNext();
  return *this;
}

template < class T >
T PSLIterator<T>::getDatum() {
  return _node->getData();
}

template < class T >
T PSLIterator<T>::operator*() {
  return getDatum();
}

template < class T >
bool PSLIterator<T>::operator==(const PSLIterator<T>& other) {
  return _node == other._node;
}

template < class T >
bool PSLIterator<T>::operator!=(const PSLIterator<T>& other) {
  return !(operator==(other));
}

#endif
