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
// ListNode::ListNode(const T& original_data)                                //
// ListNode::ListNode(const ListNode<T>* ln)                                 //
// ostream& operator<<(ostream& os, const ListNode<T>& ln)                   //
//                                                                           //
// PersistentList::PersistentList()                                          //
// int PersistentList::insert(int t, int index, ListNode<T>* ln)             //
// int PersistentList::insert(int t,                                         //
//                            ListNode<T>* old_ln,                           //
//                            ListNode<T>* new_ln)                           //
// int PersistentList::duplicateList(int t, int i)                           //
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
  public:
    T data;
    ListNode<T>* next;

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
      : data(original_data), next(NULL)
    {}
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ListNode                                               //
    //                                                                       //
    // PURPOSE:       Copy constructor                                       //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   ListNode/ln                                            //
    //   Description: The node to copy                                       //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode(const ListNode<T>* ln)
      : data(ln->data), next(NULL)
    {}
  };

  /////////////////////////////////////////////////////////////////////////////
  // ListNode implementation                                                 //
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: operator<<                                               //
  //                                                                         //
  // PURPOSE:       Formats a List nicely for an ostream.                    //
  //                                                                         //
  // SECURITY:      public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   ostream/os                                               //
  //   Description: An os to which to send the List.                         //
  //                                                                         //
  //   Type/Name:   ListNode<T>/ln                                           //
  //   Description: The List to format and send.                             //
  //                                                                         //
  // RETURN:        The ostream input.                                       //
  //                This is standard practice for chaining such as:          //
  //                cout << "List: " << list << endl;                        //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  ostream& operator<<(ostream& os, const ListNode<T>& ln) {
    os << ln.data << "->";
    if(ln.next != NULL)
      os << *(ln.next);
    else
      os << "NULL";
    return os;
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
    // FUNCTION NAME: set                                                    //
    //                                                                       //
    // PURPOSE:       Sets the node at index in list t's next pointer        //
    //                to point to ln.                                        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: Time t at which to set                                 //
    //                                                                       //
    //   Type/Name:   int/index                                              //
    //   Description: The position in the list at which to set               //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/ln                                        //
    //   Description: A pointer to the node to set                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int set(int t, int index, ListNode<T>* ln);

    ///////////////////////////////////////////////////////////////////////////
    // 
    // FUNCTION NAME: insert
    // 
    // PURPOSE:       Inserts a node at index i in list at time t.
    // 
    // SECURITY:      public
    // 
    // PARAMETERS
    //   Type/Name:   int/t
    //   Description: Time t at which to insert
    // 
    //   Type/Name:   int/index
    //   Description: The position in the list at which to insert
    // 
    //   Type/Name:   ListNode<T>*/ln
    //   Description: A pointer to the node to insert
    // 
    // RETURN:
    //   Type/Name:   int
    //   Description: A return code, 0 means success.
    // 
    // NOTES:         Assumes list at time t exists.
    // 
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
    // FUNCTION NAME: duplicateList                                          //
    //                                                                       //
    // PURPOSE:       Duplicates the list at time t up to element i, placing //
    //                the new list at t+1. Copied element i's next           //
    //                pointer will point to the next element in list t.      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time (version) of the list to copy                 //
    //                                                                       //
    //   Type/Name:   int/i                                                  //
    //   Description: Elements up to i will be copied, the rest will         //
    //                be pointed to.                                         //
    //                                                                       //
    // RETURN:        int return code, 0 means success.                      //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int duplicateList(int t, int i);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: newList                                                //
    //                                                                       //
    // PURPOSE:       Creates a new (empty) list at time t                   //
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
	  prev = dup;
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
    assert(t <= (int)lists.size());
    lists.insert(lists.begin()+t,NULL);
    return 0;
  }
  
  template <class T>
  int PersistentList<T>::set(int t, int index, ListNode<T>* ln) {
    if(index <= 0) {
      lists[t] = ln;
    } else {
      getNode(t,index-1)->next = ln;
    }
    return 0; // success
  }
  
  template <class T>
  int PersistentList<T>::insert(int t, int index, ListNode<T>* ln) {
    if(index <= 0) {
      ln->next = getNode(t,0);
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
    assert(old_ln != NULL);
    new_ln->next = old_ln->next;
    old_ln->next = new_ln;
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
      ln = ln->next;
    return ln;
  }

  template <class T>
  size_t PersistentList<T>::size() {
    return lists.size();
  }
}

#endif
