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
//                                                                           //
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTSKIPLIST_HPP
#define PERSISTENTSKIPLIST_HPP

// C++ libraries
#include <vector>
#include <set>
#include <iostream>
#include <iomanip>

// C libraries
#include <assert.h>
#include <stdlib.h>
#include <time.h>

// My libraries
#include "TimeStampedArray.hpp"

using namespace std;
using namespace timestamped_array;

namespace persistent_skip_list {
  // Set the following variable to true in order to receive debug info
  const bool PSL_DEBUG_MODE = false;
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
    static bool _SEEDED; // must be initialized to false

    static void seed() {
      if(_SEEDED)
	return;
      _SEEDED = true;
      time_t seed = (time_t)1312564825;
      seed = time(0); // comment this for non-random seeding
      srand( seed );
      if(PSL_DEBUG_MODE) {
	clog << "Seeding with value " << seed << endl;
      }
    }


    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ListNode                                               //
    //                                                                       //
    // PURPOSE:       Basic constructor                                      //
    //                                                                       //
    // SECURITY:      private                                                //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   T/original_data                                        //
    //   Description: The value to store in the data of the ListNode         //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode(const T& original_data)
      : next(), data(original_data), references(0)
    {
      seed();
      // pick height, modified from Pat Morin's Open Data Structures
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
      for(int i = 0; i < height; ++i)
	in_nodes[i] = NULL;
    }

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
	TimeStampedArray<ListNode<T>*>* tsa = next[i];
	if(tsa != NULL) {
	  for(int j = 0; j < tsa->getSize(); ++j) {
	    ListNode<T>* ln = NULL;
	    ln = tsa->getElement(j);
	    if(ln != NULL)
	      ln->removeReference();
	  }
	  delete tsa;      // delete timestamped array
	}
      }
      delete[] in_nodes;
    }
    
  public:
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: create                                                 //
    //                                                                       //
    // PURPOSE:       Guarantees the disciplined creation of ListNode        //
    //                objects.                                               //
    //                                                                       //
    //                The motivation for a static method rather than a       //
    //                normal constructor is that these nodes are             //
    //                managing themselves in memory by counting              //
    //                references and when the reference count drops to       //
    //                zero, deleting themselves.                             //
    //                                                                       //
    //                If the nodes are created using an unpredicted          //
    //                method of allocation (such as new[]), the memory       //
    //                management may fail and result in a memory leak.       //
    //                                                                       //
    //                This method guarantees that nodes will always be       //
    //                allocated and deleted in predictable ways.             //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   T/original_data                                        //
    //   Description: The value to store in the data of the ListNode         //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T>*                                           //
    //   Description: A pointer to the new ListNode object, initialized with //
    //                the given data.                                        //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    static ListNode<T>* create(const T& original_data);
    
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
    // FUNCTION NAME: getHeight                                              //
    //                                                                       //
    // PURPOSE:       Returns the height of the node.                        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The height of the node.                                //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getHeight();

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
    // FUNCTION NAME: getNext                                                //
    //                                                                       //
    // PURPOSE:       Retrieves the next pointer at the given height and the //
    //                time at or immediately preceding the given time.       //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the pointer.             //
    //                                                                       //
    //   Type/Name:   int/h                                                  //
    //   Description: The height at which to retrieve the pointer.           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T>*                                           //
    //   Description: The next pointer at the given height and the time at   //
    //                or immediately preceding the given time.               //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getNext(int t, int h);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: addNext                                                //
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
    int addNext(TimeStampedArray<ListNode<T>*>* next);

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
    // FUNCTION NAME: getIncomingNode                                        //
    //                                                                       //
    // PURPOSE:       Retrieves the incoming node at height h.               //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/h                                                  //
    //   Description: The height at which to retrieve the incoming node.     //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T>*                                           //
    //   Description: The incoming node at height h.                         //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getIncomingNode(int h);

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
  // PersistentSkipList interface                                            //
  /////////////////////////////////////////////////////////////////////////////
  template < class T >
  class PersistentSkipList {
    int height;
    vector<TimeStampedArray<ListNode<T>*>*> head;
    set< T > data_set;
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: addHead                                                //
    //                                                                       //
    // PURPOSE:       Adds a new array of pointers to the head of the        //
    //                skiplist at each height.                               //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   TimeStampedArray<ListNode<T>*>* /tsa                   //
    //   Description: A pointer to the node to set                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         Assumes list at time t exists or is one past the end.  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int addHead(TimeStampedArray<ListNode<T>*>* tsa);
    
  public:
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: PersistentSkipList                                     //
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
    PersistentSkipList()
      : height(0), head(), data_set()
    {
      if(PSL_DEBUG_MODE) {
	clog << "PSL " << this << " created." << endl;
      }
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
    virtual ~PersistentSkipList() {
      for(int i = 0; i < (int)head.size(); ++i) {
	TimeStampedArray<ListNode<T>*>* tsa = head[i];
	if(PSL_DEBUG_MODE) {
	  clog << "Deleting head at time " << i << endl;
	}
	for(int j = 0; j < tsa->getSize(); ++j) {
	  ListNode<T>* ln = tsa->getElement(j);
	  if(PSL_DEBUG_MODE) {
	    clog << "Removing head pointer to node(" << ln->getData() << ")" << endl;
	  }
	  ln->removeReference();
	}
	delete tsa;
      }
      if(PSL_DEBUG_MODE) {
	clog << "PSL " << this << " deleted." << endl;
      }
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getPresent                                             //
    //                                                                       //
    // PURPOSE:       Returns the latest time index of the skip list.        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The latest time index of the skip list.                //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getPresent();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: drawPresent                                            //
    //                                                                       //
    // PURPOSE:       Draws the data structure at present to cout.           //
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
    void drawPresent();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: draw                                                   //
    //                                                                       //
    // PURPOSE:       Draws the data structure at the given time to cout.    //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to draw the structure.               //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    void draw(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getHead                                                //
    //                                                                       //
    // PURPOSE:       Gets the head of the list at time t.                   //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the head.                //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   TimeStampedArray<ListNode<T>*>*                        //
    //   Description: The head at the given time.                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TimeStampedArray<ListNode<T>*>* getHead(int t);
    
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
  };

#include "PersistentSkipList.cpp"
}

#endif
