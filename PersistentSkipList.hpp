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

// Standard libraries
#include <set>
#include <map>
#include <iostream>
#include <cassert>
#include <cstddef>

// My libraries
#include "TimeStampedArray.hpp"
#include "lib/SmartPointer/SmartPointer.hpp"
#include "ListNode.hpp"
#include "PSLIterator.hpp"

using namespace std;
using namespace timestamped_array;
using namespace smart_ptr;

namespace persistent_skip_list {

  template < class T >
  class PersistentSkipList {
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
    PersistentSkipList();

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
    virtual ~PersistentSkipList();

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
    int getPresent() const;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: incTime                                                //
    //                                                                       //
    // PURPOSE:       Increments the time index of the skip list.            //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    void incTime();
    PersistentSkipList<T>& operator++();

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
    // FUNCTION NAME: begin                                                  //
    //                                                                       //
    // PURPOSE:       Returns an iterator to the first element in the        //
    //                list at time t.                                        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to find the first element.           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   PSLIterator<T>                                         //
    //   Description: An iterator pointing to the first element at the       //
    //                given time.                                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    PSLIterator<T> begin(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: end                                                    //
    //                                                                       //
    // PURPOSE:       Returns an iterator to the element past the end        //
    //                of the list at time t.                                 //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/time                                               //
    //   Description: The time at which to return the element past the       //
    //                end of the list.                                       //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   PSLIterator<T>                                         //
    //   Description: An iterator pointing to one element past the end of    //
    //                the list at the given time.                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    PSLIterator<T> end(int t);

    PSLIterator<T> find(const T& toFind, int t);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: insert                                                 //
    //                                                                       //
    // PURPOSE:       Inserts a data node into the present version of        //
    //                the structure.                                         //
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
    const PersistentSkipList<T>& operator+=(const T& data);

    ///////////////////////////////////////////////////////////////////////////
    // Kindly ignore my private parts                                        //
    ///////////////////////////////////////////////////////////////////////////
  private:
    typedef TimeStampedArray< SmartPointer< ListNode<T> > > TSA;
    int height;
    int present;
    map<int,SmartPointer<ListNode<T> > > head;
    map<int,SmartPointer<ListNode<T> > > tail;
    set< T > data_set;
    
    // Adds a head/tail to the map of heads/tails
    int addHead(SmartPointer<ListNode<T> > new_head);
    int addTail(SmartPointer<ListNode<T> > new_tail);

    // Gets the head/tail from the map of heads/tails at time t
    SmartPointer<ListNode<T> >& getHead(int t);
    SmartPointer<ListNode<T> >& getTail(int t);

    // Rebuilds current head and tail with increased height
    void buildHeadAndTail(int height);
  };
}

// finally, include the implementation because C++ requires that class
// templates have both interface and implementation together
#include "PersistentSkipList.cpp"
#endif
