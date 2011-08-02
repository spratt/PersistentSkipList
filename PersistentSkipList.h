//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentSkipList.h                                             //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// PURPOSE: Implements a persistent skip list data structure.                //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// 
// Public Variable:                     Description:
// ----------------                     ------------
// 
///////////////////////////////////////////////////////////////////////////////
// 
//                             Public Methods:
// 
// 
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTSKIPLIST_H
#define PERSISTENTSKIPLIST_H

// C++ libraries
#include <iostream>
#include <vector>
#include <set>

// C libraries
#include <assert.h>
#include <stdlib.h>

// My libraries
#include "TimeStampedArray.h"

using namespace std;
using namespace timestamped_array;

namespace persistent_skip_list {
  /////////////////////////////////////////////////////////////////////////////
  // ListNode interface                                                      //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  class ListNode {
  private:
    int height;
    vector<TimeStampedArray<ListNode<T>*>*> next;
    ListNode<T>** in_nodes;
    T data;
    int references;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ~ListNode                                              //
    //                                                                       //
    // PURPOSE:       Destructor                                             //
    //                                                                       //
    // SECURITY:      private                                                //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ~ListNode() {
      // clean up next
      for(int i = 0; i < (int)next.size(); ++i) {
	delete next[i];      // delete timestamped array
      }
      delete[] in_nodes;
    }
    
  public:
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
    //   Type/Name:   int/size                                               //
    //   Description: How many next pointers the node should have.  If this  //
    //                ever fills up, a new node will be created to           //
    //                store next pointers at times after the node was filled.//
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode(const T& original_data)
      : next(), data(original_data), references(0)
    {
      // pick height, modified from Pat Morin's Open Data Structures
      srand( time(0) );
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

      // We know that the number of incoming pointers will be at most height
      in_nodes = new ListNode<T>*[height];
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getData                                                //
    //                                                                       //
    // PURPOSE:       Returns the stored data at this node.                  //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   T                                                      //
    //   Description: The data stored at this node.                          //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    T getData();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextChangeIndex                                     //
    //                                                                       //
    // PURPOSE:       Given a time t, gives the change index of the nearest  //
    //                time when the next pointers were changed.              //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time for which to search.                          //
    //                                                                       //
    // RETURN:        The index of the change of next pointers nearest to the//
    //                given time.                                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getNextChangeIndex(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextAtIndex                                         //
    //                                                                       //
    // PURPOSE:       Returns the next pointers at a given change index.     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/ci                                                 //
    //   Description: The change index at which to retrieve the change       //
    //                structure.                                             //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   TimeStampedArray<ListNode<T>*>*                        //
    //   Description: The                                                    //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TimeStampedArray<ListNode<T>*>* getNextAtIndex(int ci);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: numberOfNextChangeIndices                              //
    //                                                                       //
    // PURPOSE:       Returns the number of change indices, i.e. the number  //
    //                of times this node's next pointer has been changed and //
    //                stored.                                                //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The number of times the next pointers have been        //
    //                changed.                                               //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int numberOfNextChangeIndices();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNext                                                //
    //                                                                       //
    // PURPOSE:       Retrieves the array of next pointers at the time       //
    //                at or immediately preceding the given time.            //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the pointer.             //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   TimeStampedArray<ListNode<T>*>*                        //
    //   Description: The array of next pointers at or immediately           //
    //                preceding the given time.                              //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TimeStampedArray<ListNode<T>*>* getNext(int t);

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
    int setNext(TimeStampedArray<ListNode<T>*>* next);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: addIncomingNode                                        //
    //                                                                       //
    // PURPOSE:       Adds a node to the list of incoming nodes.             //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/h                                                  //
    //   Description: The height at which to add this incoming node.         //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/in                                        //
    //   Description: The node which now has a pointer to this node.         //
    //                                                                       //
    // RETURN:        int return code.  0 indicates success.                 //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int addIncomingNode(int h, ListNode<T>* in);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: removeIncomingNode                                     //
    //                                                                       //
    // PURPOSE:       Removes a node from the list of incoming nodes.        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/h                                                  //
    //   Description: The height at which to remove the incoming node.       //
    //                                                                       //
    // RETURN:        int return code.  0 indicates success.                 //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int removeIncomingNode(int h);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: addReference                                           //
    //                                                                       //
    // PURPOSE:       Increments the reference counter.                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The value of the reference counter.                    //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int addReference();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: removeReference                                        //
    //                                                                       //
    // PURPOSE:       Decrements the reference counter.  If the              //
    //                counter drops to zero, it calls the destructor.        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The value of the reference counter.                    //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int removeReference();
  };

  /////////////////////////////////////////////////////////////////////////////
  // ListNode implementation                                                 //
  /////////////////////////////////////////////////////////////////////////////
  template<class T>
  T ListNode<T>::getData() {
    assert(this != NULL);
    return data;
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
    assert(index <= t);
    // report final index
    return index;
  }

  template <class T>
  TimeStampedArray<ListNode<T>*>* ListNode<T>::getNextAtIndex(int ci) {
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
  TimeStampedArray<ListNode<T>*>* ListNode<T>::getNext(int t) {
    assert(this != NULL);
    assert(t >= 0);
    // find nearest time
    int index = getNextChangeIndex(t);
    // if there are no pointers, bail
    if(index == -1) return NULL;
    // use this to store the node to be returned
    TimeStampedArray<ListNode<T>*>* tsa = getNextAtIndex(index);
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
  int ListNode<T>::setNext(TimeStampedArray<ListNode<T>*>* tsa) {
    assert(this != NULL);
    // since NULL is the default
    if(tsa == NULL)
      return -1; // bail if trying to set next to NULL
    // make sure time is strictly increasing
    if(((int)next.size())-1 >= 0)
      assert(tsa->getTime() > next[next.size()-1]->getTime());
    // finally, save the new set of next pointers
    next.push_back(tsa);
    // success
    return 0;
  }

  template <class T>
  int ListNode<T>::addIncomingNode(int h, ListNode<T>* in) {
    assert(h >= 0);
    assert(h < height);
    assert(in_nodes[h] == NULL);
    in_nodes[h] = in;
    // success
    return 0;
  }

  template <class T>
  int ListNode<T>::removeIncomingNode(int h) {
    assert(h >= 0);
    assert(h < height);
    in_nodes[h] = NULL;
    // success
    return 0;
  }

  template <class T>
  int ListNode<T>::addReference() {
    return ++references;
  }

  template <class T>
  int ListNode<T>::removeReference() {
    int toReturn = --references;
    if(references == 0)
      delete this;
    return toReturn;
  }

  /*

  /////////////////////////////////////////////////////////////////////////////
  // PersistentSkipList interface                                            //
  /////////////////////////////////////////////////////////////////////////////
  template < class T >
  class PersistentSkipList {
    int height;
    vector<TimeStampedArray<ListNode<T>*>*> head;
    set< T > data_set;
    
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
    //   Type/Name:   ListNode<T, Compare>* /ln                              //
    //   Description: A pointer to the node to set                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         Assumes list at time t exists or is one past the end.  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setHead(int t, TimeStampedArray<ListNode<T>*>* tsa);
    
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
    //   Type/Name:   int/size                                               //
    //   Description: The number of next pointers each node should have.     //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    PersistentList(int size)
      : height(0), head(), data_set()
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ~PersistentList                                        //
    //                                                                       //
    // PURPOSE:       Destructor                                             //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         Frees all allocated memory.                            //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    virtual ~PersistentList() {
      if(!_LOCKED)
	lock();
      for(int i = 0; i < (int)all_nodes.size(); ++i)
	delete all_nodes[i];
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: insert                                                 //
    //                                                                       //
    // PURPOSE:       Inserts a data node into the structure.                //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   const T&/data                                          //
    //   Description: The data to be inserted                                //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: 0 for success.                                         //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int insert(const T& data);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: addToBuildTree                                         //
    //                                                                       //
    // PURPOSE:       Saves a node to the build tree                         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   const T&/key                                           //
    //   Description: The key at which to save the node                      //
    //                                                                       //
    //   Type/Name:   ListNode<T, Compare>* /node                            //
    //   Description: The node to save.                                      //
    //                                                                       //
    // RETURN:        int return code.  0 means success.                     //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int addToBuildTree(const T& key, ListNode<T, Compare>* node);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: findInBuildTree                                        //
    //                                                                       //
    // PURPOSE:       Retrieves a node in the build tree.                    //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   T/datum                                                //
    //   Description: Item to find in build tree.                            //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   map<T, ListNode<T, Compare>*, Compare >::iterator      //
    //   Description: An iterator to the object in the build tree.           //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    typename map<T, ListNode<T, Compare>*, Compare >::iterator
    findInBuildTree(const T& datum);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: lock                                                   //
    //                                                                       //
    // PURPOSE:       Locks the list, preventing any further updates         //
    //                to the data and freeing temporary structures           //
    //                used in building the list.                             //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int lock();

    ///////////////////////////////////////////////////////////////////////////
    // 
    // FUNCTION NAME: createNode
    // 
    // PURPOSE:       Creates a node with the specified size
    //                containing a given value.
    // 
    // SECURITY:      public
    // 
    // PARAMETERS
    //   Type/Name:   T/data
    //   Description: The data which will be stored in the node.
    // 
    // RETURN:
    //   Type/Name:   ListNode<T, Compare>*
    //   Description: A pointer to a new node containing the given data.
    // 
    // NOTES:         Allocates the node on the heap.  It will belong
    //                to the invoker and it will be their
    //                responsibilty to free the space once it is no
    //                longer in use.
    // 
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T, Compare>* createNode(const T& data);

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
    ListNode<T, Compare>* getList(int t);

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

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: memoryUsage                                            //
    //                                                                       //
    // PURPOSE:       Returns the amount of memory used by the structure.    //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   size_t                                                 //
    //   Description: The amount of memory used by the structure.            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    size_t memoryUsage();
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // PersistentList implementation                                           //
  /////////////////////////////////////////////////////////////////////////////
  template <class T, class Compare>
  int PersistentList<T, Compare>::insert(const T& data) {
    assert(!_LOCKED);
    // check for duplicate
    if(build_tree->find(data) != build_tree->end())
      return -1;
    // determine the time
    int t = build_tree->size();
    // create a new node
    ListNode<T, Compare>* ln = createNode(data);
    // insert point into tree                  O(logn)
    addToBuildTree(data,ln);
    // find previous node                      O(logn)ish
    typename map<T, ListNode<T, Compare>*, Compare >::iterator it =
      build_tree->find(data);
    // if new node is at the beginning
    if(it == build_tree->begin()) {
      // set head to new node
      setHead(t,ln);
      // if this isn't the first node
      if(t > 0)
	// set next on the node to the previous head
	ln->setNext(t,getList(t-1));
    }
    // otherwise, the new node must not be at the beginning
    else {
      // create a new head for the list
      setHead(t,getList(t-1));
      // find previous node
      ListNode<T, Compare>* prev = (--it)->second;
      // set next on new node to next of previous node
      prev->setNext(t,ln);
      // put the iterator back into its initial state
      ++it;
      // if the new node was not at the end
      if(++it != build_tree->end()) {
	// set the next pointer on the new node
	ln->setNext(t,it->second);
      }
    }
    // success
    return 0;
  }

  template <class T, class Compare>
  int PersistentList<T, Compare>::addToBuildTree(const T& key,
						 ListNode<T, Compare>* node) {
    assert(!_LOCKED);
    (*build_tree)[key] = node;
    // success
    return 0;
  }

  template <class T, class Compare>
  typename map<T, ListNode<T, Compare>*, Compare >::iterator
  PersistentList<T, Compare>::findInBuildTree(const T& datum) {
    assert(!_LOCKED);
    return build_tree->find(datum);
  }
  
  template <class T, class Compare>
  int PersistentList<T, Compare>::lock() {
    if(_LOCKED)
      return 1; // already locked
    // otherwise
    _LOCKED = true;
    delete build_tree;
    // success
    return 0;
  }

  template <class T, class Compare>
  ListNode<T, Compare>* PersistentList<T, Compare>::createNode(const T& data) {
    ++nNodes;
    ListNode<T, Compare>* ln = new ListNode<T, Compare>(data,(int)_NODE_SIZE,this);
    all_nodes.push_back(ln);
    return ln;
  }
  
  template <class T, class Compare>
  int PersistentList<T, Compare>::setHead(int t, ListNode<T, Compare>* ln) {
    assert(t >= 0);
    assert(t <= (int)lists.size());
    if(t == (int)lists.size())
      lists.push_back(ln);
    else
      lists[t] = ln;
    return 0; // success
  }

  template <class T, class Compare>
  ListNode<T, Compare>* PersistentList<T, Compare>::getList(int t) {
    assert(t < (int)lists.size());
    return lists[t];
  }

  template <class T, class Compare>
  size_t PersistentList<T, Compare>::size() {
    return lists.size();
  }

  template <class T, class Compare>
  size_t PersistentList<T, Compare>::memoryUsage() {
    size_t s = nNodes * sizeof(ListNode<T, Compare>);
    if(!_LOCKED)
      s += nNodes * (sizeof(T) + sizeof(ListNode<T, Compare>));
    return s;
  }
  */
}
#endif
