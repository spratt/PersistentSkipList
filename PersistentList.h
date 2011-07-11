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
// ListNode<T, Compare>* next           The next node in the list            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
// ListNode<T>::ListNode(int t, const T& original_data)                      //
// ListNode<T>::getNextIndex(int t)                                          //
// ListNode<T>::getNextAtIndex(int t)                                        //
// ListNode<T>::numberOfChangeIndices(int t)                                 //
// ListNode<T>::getNext(int t)                                               //
// ListNode<T>::setNext(int t, ListNode<T>* ln)                              //
// ListNode<T>::printList(int t)                                             //
//                                                                           //
// PersistentList<T, Compare>::PersistentList()                              //
// int PersistentList<T, Compare>::setHead(int t, ListNode<T>* ln)           //
// ListNode<T>* PersistentList<T, Compare>::getList(int t)                   //
// size_t PersistentList<T, Compare>::size()                                 //
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
  // PersistentList empty declaration                                        //
  // (for forward reference)                                                 //
  /////////////////////////////////////////////////////////////////////////////
  template <class T, class Compare = less<T> >
  class PersistentList;
  
  /////////////////////////////////////////////////////////////////////////////
  // ListNode interface                                                      //
  /////////////////////////////////////////////////////////////////////////////
  template <class T, class Compare = less<T> >
  class ListNode {
    int* time;
    ListNode<T, Compare>** next;
    int _SIZE;
    int _LAST;
  public:
    T data;
    PersistentList<T, Compare>* list;

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
    ListNode(const T& original_data, int size, PersistentList<T, Compare>* list)
      : _SIZE(size), _LAST(-1), data(original_data), list(list) {
      time = new int[size];
      next = new ListNode<T, Compare>*[size];
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ~ListNode                                              //
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
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ~ListNode() {
      delete time;
      delete next;
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getSize                                                //
    //                                                                       //
    // PURPOSE:       Returns the size of the node.                          //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The size of the node.                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getSize();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextIndex                                           //
    //                                                                       //
    // PURPOSE:       Given a time t, gives the change index of the nearest  //
    //                time.                                                  //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time for which to search.                          //
    //                                                                       //
    // RETURN:        The nearest change index in the next pointer vector.   //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getNextIndex(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextAtIndex                                         //
    //                                                                       //
    // PURPOSE:       Allows next pointer to be retrieved by change          //
    //                index.  This is useful for efficient                   //
    //                construction of persistent lists.                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/ci                                                 //
    //   Description: The change index at which to retrieve the next pointer.//
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T, Compare>*                                  //
    //   Description: The next pointer at the given change index.            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T, Compare>* getNextAtIndex(int ci);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: numberOfChangeIndices                                  //
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
    //   Description: The number of change indices of the current node.      //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int numberOfChangeIndices();

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
    //   Type/Name:   ListNode<T, Compare>*                                  //
    //   Description: The next pointer at time t.                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T, Compare>* getNext(int t);

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
    //   Type/Name:   ListNode<T, Compare>*/ln                               //
    //   Description: The pointer to which to assign next.                   //
    //                                                                       //
    // RETURN:        int return code.  0 means success                      //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    virtual int setNext(int t, ListNode<T, Compare>* ln);
    
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
  template <class T, class Compare>
  int ListNode<T, Compare>::getSize() { return _SIZE; }
  
  template <class T, class Compare>
  int ListNode<T, Compare>::getNextIndex(int t) {
    assert(this != NULL);
    assert(t >= 0);
    int index = -1;
    int begin = 0, end = _LAST;
    // binary search
    while(begin <= end) {
      index = (begin+end)/2;
      if(t == time[index]) {
	// done, break out of loop
	break;
      } else if(t < time[index]) {
	// repeat binary search on left half
	end = index -1;
      } else {
	// repeat binary search on right half
	begin = index +1;
      }
    }
    assert(index <= t);
    // report final index
    return index;
  }

  template <class T, class Compare>
  ListNode<T, Compare>* ListNode<T, Compare>::getNextAtIndex(int ci) {
    assert(this != NULL);
    assert(ci >= 0);
    assert(ci < numberOfChangeIndices());
    return next[ci];
  }

  template <class T, class Compare>
  int ListNode<T, Compare>::numberOfChangeIndices() {
    assert(this != NULL);
    return _LAST + 1;
  }
  
  template <class T, class Compare>
  ListNode<T, Compare>* ListNode<T, Compare>::getNext(int t) {
    assert(this != NULL);
    assert(t >= 0);
    // find nearest time
    int index = getNextIndex(t);
    // if there are no next pointers, bail
    if(index == -1) return NULL;
    // default to NULL
    ListNode<T, Compare>* ln = NULL;
    // while the time at index is greater than the desired time
    while(time[index] > t) {
      // decrement if not first
      --index;
      // if first, bail
      if(index < 0)
	return NULL;
    }
    // find and return next pointer at nearest time less than or equal to given
    ln = getNextAtIndex(index);
    return ln;
  }

  template <class T, class Compare>
  int ListNode<T, Compare>::setNext(int t, ListNode<T, Compare>* ln) {
    assert(this != NULL);
    // since NULL is the default
    if(ln == NULL)
      return -1; // bail if trying to set next to NULL
    
    // if trying to set next to previous next pointer, bail
    // since getNext will by default return the node at the largest
    // time before the requested time
    if(_LAST >= 0 && next[_LAST] == ln)
      return 1;
    // check if the node has room
    if(_LAST+1 < _SIZE) { // has room
      // since we have restricted point insertion to strictly increasing
      // time, we can simply push these values to the back of their arrays
      ++_LAST;
      time[_LAST] = t;
      next[_LAST] = ln;
    } else { // full
      // create new node
      ListNode<T, Compare>* new_ln = list->createNode(data);
      // save the new node for use in all future lists
      list->addToBuildTree(data,new_ln);
      // set next on new node (guaranteed not full) to given next node
      new_ln->setNext(t,ln);
      // if this node is head
      ListNode<T, Compare>* node = list->getList(t);
      if(this == node) {
        // set head at t to newly created node
	list->setHead(t,new_ln);
      } else { // otherwise there is a node which precedes this node
	// find node
	typename map< T, ListNode<T, Compare>*, Compare >::iterator it = 
	  list->findInBuildTree(data);
	// move to preceding node
	--it;
	// set next to new node
	it->second->setNext(t,new_ln);
      }
    }
    // success
    return 0;
  }

  template <class T, class Compare>
  int ListNode<T, Compare>::printList(int t) {
    assert(this != NULL);
    assert(t >= 0);
    cout << data << "->" << flush; // inefficient to flush every line
    ListNode<T, Compare>* ln = getNext(t);
    if(ln != NULL)
      ln->printList(t);
    else
      cout << "NULL" << flush;
    // success
    return 0;
  }

  /////////////////////////////////////////////////////////////////////////////
  // PersistentList interface                                                //
  /////////////////////////////////////////////////////////////////////////////
  template < class T, class Compare >
  class PersistentList {
    vector< ListNode<T, Compare>* > lists;
    map< T, ListNode<T, Compare>*, Compare >* build_tree;
    vector< ListNode<T, Compare>* > all_nodes;
    int _NODE_SIZE;
    bool _LOCKED;
    int nNodes;
    
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
    //   Type/Name:   ListNode<T, Compare>*/ln                               //
    //   Description: A pointer to the node to set                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         Assumes list at time t exists or is one past the end.  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setHead(int t, ListNode<T, Compare>* ln);
    
  public:
    friend class ListNode<T, Compare>;
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
      : lists(), all_nodes(), _NODE_SIZE(size), _LOCKED(false), nNodes(0)
    {
      build_tree = new map< T, ListNode<T, Compare>*, Compare >();
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
    //   Type/Name:   ListNode<T, Compare>*/node                             //
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
}

#endif
