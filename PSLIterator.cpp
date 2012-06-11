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
PSLIterator<T>::~PSLIterator(void) {
}

template < class T >
PSLIterator<T> PSLIterator<T>::getNext(void) {
  PSLIterator<T> next(_node,_time,_height);
  return ++next;
}

template < class T >
int PSLIterator<T>::getHeight(void) {
  return _node->getHeight();
}

template < class T >
int PSLIterator<T>::getSearchHeight(void) {
  return _height;
}

template < class T >
void PSLIterator<T>::down(void) {
  assert(_height > 0);
  --_height;
}

template < class T >
void PSLIterator<T>::next(void) {
  if(_node->isPositiveInfinity())
    return;
  TimeStampedArray<SmartPointer<ListNode<T> > >* next = _node->getNext(_time);
  assert(next != NULL);
  assert(_height < next->getSize());
  SmartPointer<ListNode<T> > nextNode = next->getElement(_height);
  assert(nextNode != NULL);
  assert(nextNode->getHeight() > _height);
  next = nextNode->getNext(_time);
  if(next != NULL)
    assert(next->getSize() == nextNode->getHeight());
  _node = nextNode;
}

template < class T >
PSLIterator<T>& PSLIterator<T>::operator++(void) {
  next();
  return *this;
}

template < class T >
T PSLIterator<T>::getDatum(void) {
  return _node->getData();
}

template < class T >
T PSLIterator<T>::operator*(void) {
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

template < class T >
bool PSLIterator<T>::operator<(const PSLIterator<T>& other) {
  return *_node < *(other._node);
}

template < class T >
bool PSLIterator<T>::operator>(const PSLIterator<T>& other) {
  return *_node > *(other._node);
}

template < class T >
bool PSLIterator<T>::operator<=(const PSLIterator<T>& other) {
  return !(*_node > *(other._node));
}

template < class T >
bool PSLIterator<T>::operator>=(const PSLIterator<T>& other) {
  return !(*_node < *(other._node));
}

// datum

template < class T >
bool PSLIterator<T>::operator<(const T& datum) {
  return *_node < datum;
}

template < class T >
bool PSLIterator<T>::operator>(const T& datum) {
  return *_node > datum;
}

template < class T >
bool PSLIterator<T>::operator<=(const T& datum) {
  return !(*_node > datum);
}

template < class T >
bool PSLIterator<T>::operator>=(const T& datum) {
  return !(*_node < datum);
}

template < class T >
const PSLIterator<T>& PSLIterator<T>::operator=(PSLIterator<T>& other) {
  this->_node = other._node;
  this->_time = other._time;
  this->_height = other._height;
  return *this;
}

template < class T >
const PSLIterator<T>& PSLIterator<T>::operator=(const PSLIterator<T>& other) {
  this->_node = other._node;
  this->_time = other._time;
  this->_height = other._height;
  return *this;
}

template < class T >
void PSLIterator<T>::remove(void) {
  SmartPointer<ListNode<T> > node = this->_node;
  next();
  node->remove(_time);
}

#endif
