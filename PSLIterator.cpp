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

template < class T >
PSLIterator<T>::PSLIterator(SmartPointer<ListNode<T> >& node, int time)
  : _node(node), _time(time)
{
}

template < class T >
PSLIterator<T>::~PSLIterator() {
}

template < class T >
void PSLIterator<T>::getNext() {
  TimeStampedArray<SmartPointer<ListNode<T> > >* next = _node->getNext(_time);
  assert(next != NULL);
  _node = next->getElement(0);
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
bool PSLIterator<T>::operator==(PSLIterator& other) {
  return _node == other._node;
}

#endif
