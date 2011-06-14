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
// ListNode::printList(int t)                                                //
//                                                                           //
// PersistentList::PersistentList()                                          //
// int PersistentList::newHead(int t, ListNode<T>* ln)                       //
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
#include <assert.h>

using namespace std;

namespace persistent_list {
  /////////////////////////////////////////////////////////////////////////////
  // ListNode interface                                                      //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  class ListNode {
    vector<int> time;
    vector<ListNode<T>* > next;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextIndex                                           //
    //                                                                       //
    // PURPOSE:       Given a time t, gives the index of the nearest time.   //
    //                                                                       //
    // SECURITY:      private                                                //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time for which to search.                          //
    //                                                                       //
    // RETURN:        The nearest index in the next pointer vector.          //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getNextIndex(int t);
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
      : time(), next(), data(original_data) {
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNext                                                //
    //                                                                       //
    // PURPOSE:       Retrieves the next pointer at a given time.  If        //
    //                the given time is not set, returns the pointer         //
    //                which would directly precede it.                       //
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
  int ListNode<T>::getNextIndex(int t) {
    assert(t >= 0);
    int index = -1;
    int begin = 0, end = (int)next.size() -1;
    while(begin <= end) {
      index = (begin+end)/2;
      if(t == time[index]) {
	break;
      } else if(t < time[index]) {
	end = index -1;
      } else {
	begin = index +1;
      }
    }
    return index;
  }

  template <class T>
  ListNode<T>* ListNode<T>::getNext(int t) {
    assert(t >= 0);
    int index = getNextIndex(t);
    if(index == -1) return NULL;
    ListNode<T>* ln = NULL;
    bool overshot = time[index] > t;
    if(overshot) { // overshot
      if(index > 0)
	ln = next[index-1];
    } else {
      ln = next[index];
    }
    return ln;
  }

  template <class T>
  int ListNode<T>::setNext(int t, ListNode<T>* ln) {
    assert(t >= 0);
    int index = getNextIndex(t);
    if(index == -1) {
      time.push_back(t);
      next.push_back(ln);
    } else if(t == time[index]) {
      next[index] = ln;
    } else if(t < time[index]) {
      time.insert(time.begin()+index,t);
      next.insert(next.begin()+index,ln);
    } else { // greater
      time.insert(time.begin()+index+1,t);
      next.insert(next.begin()+index+1,ln);
    }
    // success
    return 0;
  }

  template <class T>
  int ListNode<T>::printList(int t) {
    assert(t >= 0);
    cout << data << "->";
    ListNode<T>* ln = getNext(t);
    if(ln != NULL)
      ln->printList(t);
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
    // FUNCTION NAME: setHead                                                //
    //                                                                       //
    // PURPOSE:       Sets a node to the head of the list at time t.         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: Time t at which to set                                 //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/ln                                        //
    //   Description: A pointer to the node to set                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         Assumes list at time t exists or is one past the end.  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setHead(int t, ListNode<T>* ln);

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
  int PersistentList<T>::setHead(int t, ListNode<T>* ln) {
    assert(t >= 0);
    assert(t <= (int)lists.size());
    lists[t] = ln;
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
