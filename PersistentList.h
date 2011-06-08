//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentList.h                                                 //
//                                                                           //
// MODULE:  Persistent List                                                  //
//                                                                           //
// PURPOSE: Implements a persistent list data structure.                     //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
// persistent_list::coord_t             the type of a coordinate             //
// persistent_list::Point2d             the type of a point in a 2d plane    //
// persistent_list::PersistentList      an implementation of a linked        //
//                                      list, in which previous              //
//                                      versions may be accessed and         //
//                                      modified                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
// PersistentList::PersistentList()                                          //
// PersistentList::insertPoint(coord_t x, coord_t y)                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTLIST_H
#define PERSISTENTLIST_H

#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

namespace persistent_list {
  template <class T>
  class ListNode {
  public:
    T data;
    ListNode<T>* next;
    ListNode(const T& original_data)
      : data(original_data), next(NULL)
    {}
    ListNode(const ListNode<T>* ln)
      : data(ln->data), next(NULL)
    {}
    

  };

  template <class T>
  ostream& operator<<(ostream& os, const ListNode<T>& ln) {
    os << ln.data << "->";
    if(ln.next != NULL)
      os << *(ln.next);
    else
      os << "NULL";
    return os;
  }

  template <class T>
  class PersistentList {
    // Data
    vector< ListNode<T>* > lists;
    // Methods
  public:
    PersistentList()
      : lists()
    {}
    int insert(int t, int index, ListNode<T>* ln);
    int duplicateList(int t, int i);
    int newList(int t);
    ListNode<T>* getList(int t);
    ListNode<T>* getNode(int t, int index);
    size_t size();
  };

  
  template <class T>
  int PersistentList<T>::duplicateList(int t, int i=0) {
    assert(t >= 0);
    int n = (int)lists.size();
    assert(t < n);
    ListNode<T>* list = getList(t);
    newList(t+1);
    if(i <= 0) {
      lists[t+1] = list;
    } else {
      ListNode<T>* dup = new ListNode<T>(list);
      lists[t+1] = dup;
      if(list != NULL) {
	ListNode<T>* prev = dup;
	while(list != NULL && --i > 0) {
	  list = list->next;
	  dup = new ListNode<T>(list);
	  prev->next = dup;
	}
	if(list != NULL)
	  dup->next = list->next;
      }
    }
    // success
    return 0;
  }

  template <class T>
  int PersistentList<T>::newList(int t) {
    assert(t >= 0);
    int n = (int)lists.size();
    assert(t <= n);
    lists.push_back(NULL);
    block_permute(vectorToArray(lists),t,n-1,n);
    return 0;
  }
  
  template <class T>
  int PersistentList<T>::insert(int t, int index, ListNode<T>* ln) {
    if(index == 0) {
      ln->next = getNode(t,0);
      lists[t] = ln;
    } else {
      ListNode<T>* oldln = getNode(t,index-1);
      ln->next = oldln->next;
      oldln->next = ln;
    }
    return 0; // success
  }

  template <class T>
  ListNode<T>* PersistentList<T>::getList(int t) {
    return lists[t];
  }

  template <class T>
  ListNode<T>* PersistentList<T>::getNode(int t, int index) {
    ListNode<T>* ln = getList(t);
    while(ln != NULL && index-- > 0)
      ln = ln->next;
    return ln;
  }

  template <class T>
  size_t PersistentList<T>::size() {
    return lists.size();
  }
}

#endif
