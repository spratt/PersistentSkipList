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
// T ListNode.data                      The data to be stored within a node  //
// ListNode<T>* next                    The next node in the list            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
// ListNode::ListNode(int t, const T& original_data)                         //
// ListNode::getNext(int t)                                                  //
// ListNode::setNext(int t, ListNode<T>* ln)                                 //
// ListNode::insertNext(int t, ListNode<T>* ln)                              //
// ListNode::printList(int t)                                                //
//                                                                           //
// PersistentList::PersistentList()                                          //
// int PersistentList::insert(int t, int index, ListNode<T>* ln)             //
// int PersistentList::insertAfterNode(int t,                                //
//                                     ListNode<T>* old_ln,                  //
//                                     ListNode<T>* new_ln)                  //
// int PersistentList::newList(int t)                                        //
// ListNode<T>* PersistentList::getList(int t)                               //
// ListNode<T>* PersistentList::getNode(int t, int index)                    //
// size_t PersistentList::size()                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTLIST_H
#define PERSISTENTLIST_H

#include <iostream>
#include <vector>
#include <map>
#include <assert.h>

using namespace std;

namespace persistent_list {
  /////////////////////////////////////////////////////////////////////////////
  // ListNode interface                                                      //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  class ListNode {
    typedef map<int, ListNode<T>* > NodeMapType;
    NodeMapType next;
  public:
    T data;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ListNode                                               //
    //                                                                       //
    // PURPOSE:       Basic constructor                                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   T/original_data                                        //
    //   Description: The value to store in the data of the ListNode         //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode(const T& original_data)
      : next(), data(original_data) {
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNext                                                //
    //                                                                       //
    // PURPOSE:       Retrieves the next pointer at a given time             //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the pointer              //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T>*                                           //
    //   Description: The next pointer at time t.                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getNext(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: setNext                                                //
    //                                                                       //
    // PURPOSE:       Set the next pointer at time t                         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to set the pointer.                  //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/ln                                        //
    //   Description: The pointer to which to assign next.                   //
    //                                                                       //
    // RETURN:        int return code.  0 means success                      //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setNext(int t, ListNode<T>* ln);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: printList                                              //
    //                                                                       //
    // PURPOSE:       Prints a list at time t to cout.                       //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to print the list.                   //
    //                                                                       //
    // RETURN:        int return code.  0 means success.                     //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int printList(int t);
  };

  /////////////////////////////////////////////////////////////////////////////
  // ListNode implementation                                                 //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  ListNode<T>* ListNode<T>::getNext(int t) {
    assert(t >= 0);
    if(next.find(t) == next.end())
      assert(false);
    return next[t];
  }

  template <class T>
  int ListNode<T>::setNext(int t, ListNode<T>* ln) {
    assert(t >= 0);
    next[t] = ln;
    // success
    return 0;
  }

  template <class T>
  int ListNode<T>::printList(int t) {
    assert(t >= 0);
    cout << data << "->";
    if(next.find(t) != next.end() && next[t] != NULL)
      next[t]->printList(t);
    else
      cout << "NULL";
    // success
    return 0;
  }

  /////////////////////////////////////////////////////////////////////////////
  // PersistentList interface                                                //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  class PersistentList {
    vector< ListNode<T>* > lists;
    
  public:   
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: PersistentList                                         //
    //                                                                       //
    // PURPOSE:       Empty constructor                                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    PersistentList()
      : lists()
    {}

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: insert                                                 //
    //                                                                       //
    // PURPOSE:       Inserts a node at index i in list at time t.           //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: Time t at which to insert                              //
    //                                                                       //
    //   Type/Name:   int/index                                              //
    //   Description: The position in the list at which to insert            //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/ln                                        //
    //   Description: A pointer to the node to insert                        //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         Assumes list at time t exists.                         //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int insert(int t, int index, ListNode<T>* ln);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: insertAfterNode                                        //
    //                                                                       //
    // PURPOSE:       Inserts a given ListNode<T>* before an already         //
    //                existing ListNode<T>* in the list.                     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: Time t at which to insert                              //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/old_ln                                    //
    //   Description: A pointer to a node which already exists in a list.    //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/new_ln                                    //
    //   Description: A pointer to the node to insert                        //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         Assumes list at time t exists.                         //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int insertAfterNode(int t, ListNode<T>* old_ln, ListNode<T>* new_ln);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: newList                                                //
    //                                                                       //
    // PURPOSE:       Creates a new list at time t with the same nodes       //
    //                as in t-1.                                             //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to insert the empty list             //
    //                                                                       //
    // RETURN:        int return code, 0 means success.                      //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int newList(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getList                                                //
    //                                                                       //
    // PURPOSE:       Gets the list at time t.                               //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the list.                //
    //                                                                       //
    // RETURN:        The first node in the list at time t.                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getList(int t);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNode                                                //
    //                                                                       //
    // PURPOSE:       Gets node at index in list t                           //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time (version) of the list to search               //
    //                                                                       //
    //   Type/Name:   int/index                                              //
    //   Description: The index'th node at time t will be returned           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T>*                                           //
    //   Description: A pointer to the index'th node in list at time t       //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getNode(int t, int index);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: size                                                   //
    //                                                                       //
    // PURPOSE:       Returns the number of points in the structure          //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   size_t                                                 //
    //   Description: The number of points in the structure                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    size_t size();
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // PersistentList implementation                                           //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  int PersistentList<T>::insert(int t, int index, ListNode<T>* ln) {
    assert(t >= 0);
    assert(t <= (int)lists.size());
    if(index <= 0) {
      ln->setNext(t,getNode(t,0));
      lists[t] = ln;
    } else {
      insertAfterNode(t,getNode(t,index-1),ln);
    }
    return 0; // success
  }
  
  template <class T>
  int PersistentList<T>::insertAfterNode(int t,
					 ListNode<T>* old_ln,
					 ListNode<T>* new_ln) {
    assert(t >= 0);
    assert(t <= (int)lists.size());
    assert(old_ln != NULL);
    new_ln->setNext(t,old_ln->getNext(t));
    old_ln->setNext(t,new_ln);
    return 0; // success
  }

  template <class T>
  int PersistentList<T>::newList(int t) {
    assert(t >= 0);
    assert(t <= (int)lists.size());
    // save the new head
    ListNode<T>* prev_head = NULL;
    if(t > 0)
      prev_head = lists[t-1];
    lists.insert(lists.begin()+t,prev_head);
    if(t > 0 && prev_head != NULL) {
      // create the new time in each node
      ListNode<T>* ln;
      ListNode<T>* next = prev_head;
      do {
	ln = next;
	next = ln->getNext(t-1);
	ln->setNext(t,next);
      } while(next != NULL);
    }
    return 0; // success
  }

  template <class T>
  ListNode<T>* PersistentList<T>::getList(int t) {
    assert(t < (int)lists.size());
    return lists[t];
  }

  template <class T>
  ListNode<T>* PersistentList<T>::getNode(int t, int index) {
    ListNode<T>* ln = getList(t);
    while(ln != NULL && index-- > 0)
      ln = ln->getNext(t);
    return ln;
  }

  template <class T>
  size_t PersistentList<T>::size() {
    return lists.size();
  }
}

#endif
