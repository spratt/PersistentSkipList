///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentSkipList.cpp                                           //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTSKIPLIST_CPP
#define PERSISTENTSKIPLIST_CPP

#include "PersistentSkipList.hpp"

using namespace persistent_skip_list;

///////////////////////////////////////////////////////////////////////////////
// PersistentSkipList Implementation                                         //
///////////////////////////////////////////////////////////////////////////////

template <class T>
PersistentSkipList<T>::PersistentSkipList(int nodeSize)
  : node_size(nodeSize), present(0), head(), tail(), data_set()
{
  SmartPointer<ListNode<T> > negInf(new ListNode<T>(1,false));
  SmartPointer<ListNode<T> > posInf(new ListNode<T>(1,true));
  head.insert( pair<int,SmartPointer<ListNode<T> > >(0,negInf) );
  tail.insert( pair<int,SmartPointer<ListNode<T> > >(0,posInf) );
  // set next on negInf to posInf
  TSA* newNext = new TSA(0,1);
  newNext->setElement(0,posInf);
  negInf->addNext(newNext);
}

template <class T>
PersistentSkipList<T>::~PersistentSkipList() {
}

template <class T>
int PersistentSkipList<T>::getPresent() const {
  assert(this != NULL);
  return present;
}

template <class T>
PersistentSkipList<T>& PersistentSkipList<T>::operator++() {
  incTime();
  return this;
}

template <class T>
void PersistentSkipList<T>::incTime() {
  assert(this != NULL);
  ++present;
}

template <class T>
void PersistentSkipList<T>::drawPresent() {
  assert(this != NULL);
  draw(getPresent());
}

template <class T>
void PersistentSkipList<T>::draw(int t) {
  assert(this != NULL);
  assert(t >= 0);
  cout << "Drawing skip list at time " << t << "..." << endl;
  for(int i = 0; i < getHeight(t); ++i) {
    PSLIterator<T> next = begin(t,i);
    
    if(next == end(t))
      cout << "NULL" << endl;
    else
      cout << "Height: " << i+1 << endl;
    
    while(next != end(t)) {
      cout << "Node(data=" << *next
	   << ",height=" << next.getHeight() << ")" << endl;
      ++next;
    }
  }
}

template <class T>
int PersistentSkipList<T>::addHead(SmartPointer<ListNode<T> > new_head) {
  assert(this != NULL);
  assert(new_head != NULL);
  // delete old head if exists
  if(head.count(present) > 0)
    head.erase(present); // shouldn't leak using smart pointer
  // save the new head
  head.insert( pair< int, SmartPointer<ListNode<T> > >(present,new_head) );
  // success
  return 0;
}

template <class T>
SmartPointer<ListNode<T> >& PersistentSkipList<T>::getHead(int t) {
  // reverse linear search from starting time
  while(head.count(t) == 0)
    --t;
  return head[t];
}

template <class T>
int PersistentSkipList<T>::addTail(SmartPointer<ListNode<T> > new_tail) {
  assert(this != NULL);
  assert(new_tail != NULL);
  // delete old head if exists
  if(tail.count(present) > 0)
    tail.erase(present); // shouldn't leak using smart pointer
  // save the new head
  tail.insert( pair< int, SmartPointer<ListNode<T> > >(present,new_tail) );
  // success
  return 0;
}

template <class T>
SmartPointer<ListNode<T> >& PersistentSkipList<T>::getTail(int t) {
  // reverse linear search from starting time
  while(tail.count(t) == 0)
    --t;
  return tail[t];
}

template <class T>
void PersistentSkipList<T>::buildHeadAndTail(int new_height) {
  assert(new_height > getHeight(getPresent()));
  int present = getPresent();
  SmartPointer<ListNode<T> > old_head = getHead(present);
  SmartPointer<ListNode<T> > old_tail = getTail(present);
  int old_height = old_head->getHeight();
  assert(new_height > old_height);
  assert(old_height == old_tail->getHeight());
  SmartPointer<ListNode<T> > new_head(new ListNode<T>(new_height,false));
  SmartPointer<ListNode<T> > new_tail(new ListNode<T>(new_height,true));
  assert(new_head->getHeight() == new_tail->getHeight());
  TSA* new_next = new TSA(present,new_height);
  // make the tail the new next above the old height
  while(--new_height >= old_height) {
    new_next->setElement(new_height,new_tail);
  }
  while(new_height >= 0) {
    SmartPointer<ListNode<T> > next_node =
      old_head->getNext(present)->getElement(new_height);
    if(next_node == old_tail)
      new_next->setElement(new_height,new_tail);
    else
      new_next->setElement(new_height,next_node);
    --new_height;
  }
  new_head->addNext(new_next);
  addHead(new_head);
  // make all the nodes which pointed to the old tail point to the new tail
  --old_height;
  while(old_height >= 0) {
    int end = old_height;
    while(end > 0 &&
	  old_tail->getIncoming(end-1) == old_tail->getIncoming(old_height))
      --end;
    ListNode<T>* toChange = old_tail->getIncoming(old_height);
    if(old_head == toChange) {
      old_height = end-1;
      continue;
    }
    new_next = new TSA(present,
		       toChange->getHeight(),
		       *(toChange->getNext(present)));
    while(old_height >= end) {
      new_next->setElement(old_height,new_tail);
      new_tail->setIncoming(old_height,toChange);
      --old_height;
    }
    toChange->addNext(new_next);
  }
  addTail(new_tail);
}

template < class T >
PSLIterator<T> PersistentSkipList<T>::begin(int t, int h) {
  return ++(PSLIterator<T>(getHead(t),t,h));
}

template < class T >
PSLIterator<T> PersistentSkipList<T>::end(int t) {
  return PSLIterator<T>(getTail(t),t);
}

template < class T >
PSLIterator<T> PersistentSkipList<T>::find(const T& toFind, int t) {
  PSLIterator<T> iter = PSLIterator<T>(getHead(t),t,getHeight(t)-1);
  PSLIterator<T> next = iter.getNext();
  const PSLIterator<T> end = this->end(t);
  while( iter.getSearchHeight() > 0 || next != end ) {
    // loop invariant: we have already determined the value of iter
    //                 precedes the data for which we are searching.
    if(next <= toFind && next != end) {
      assert(iter.getSearchHeight() < next.getHeight());
      iter.next();
    } else if(next > toFind && iter.getSearchHeight() > 0) {
      iter.down();
    } else {
      return iter;
    }
    next = iter.getNext();
  }
  return iter;
}

template < class T >
int PersistentSkipList<T>::getHeight(int t) {
  return (getHead(t))->getHeight();
}

/////////////////////////////////////////////////////////////////////////////
// INSERT METHOD                                                           //
/////////////////////////////////////////////////////////////////////////////

template <class T>
const PersistentSkipList<T>& PersistentSkipList<T>::operator+=(const T& data) {
  if(insert(data) != 0) // error
    throw "Unable to insert data!";
  return this;
}

template <class T>
int PersistentSkipList<T>::insert(const T& data) {
  assert(this != NULL);
  // check if data exists already
  if(data_set.count(data)>0)
    return 1;
  int present = getPresent();
  // otherwise, create node
  SmartPointer<ListNode<T> > new_ln(new ListNode<T>(data));
  int height = new_ln->getHeight();
  // add node to list
  int start = height-1;
  // Taller than old head
  if(height > getHeight(getPresent()))
    buildHeadAndTail(height);
  // Add new node to list
  SmartPointer<ListNode<T> > old_ln = getHead(present);
  TSA* new_node_next = new TSA(present,height);
  while(start >= 0) {
    SmartPointer<ListNode<T> > next_ln =
      old_ln->getNext(present)->getElement(start);
    // find the elements between which we should insert the new node
    while(*new_ln > *next_ln) {
      old_ln = next_ln;
      next_ln = old_ln->getNext(present)->getElement(start);
    }
    TSA* old_ln_next = new TSA(present,
			       old_ln->getHeight(),
			       *(old_ln->getNext(present)));
    while(*new_ln < *next_ln) {
      new_node_next->setElement(start,next_ln);
      // point the old node to the new node
      old_ln_next->setElement(start,new_ln);
      // move to the next height
      --start;
      if(start < 0)
	break;
      next_ln = old_ln_next->getElement(start);
    }
    old_ln->addNext(old_ln_next);
    // move to next search height
    if(start < 0)
      break;
    // get node at new height
    old_ln = next_ln;
  }
  new_ln->addNext(new_node_next);
  // prevent duplicates by registering this datum
  data_set.insert(data);
  // success
  return 0;
}

#endif
