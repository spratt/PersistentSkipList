///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PSLIterator.hpp                                                  //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// PURPOSE: None.                                                            //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////
#ifndef PSLITERATOR_HPP
#define PSLITERATOR_HPP

#include <cassert>

#include "ListNode.hpp"

namespace persistent_skip_list {
  template < class T >
  class PSLIterator {
  public:
    PSLIterator(SmartPointer<ListNode<T> >& node, int time=0, int height=0);
    ~PSLIterator();

    PSLIterator<T> getNext();
    
    void next();
    PSLIterator<T>& operator++();
    
    T getDatum();
    T operator*();

    bool operator==(const PSLIterator<T>& other);
    bool operator!=(const PSLIterator<T>& other);

    const PSLIterator<T>& operator=(PSLIterator<T>& other);
    const PSLIterator<T>& operator=(const PSLIterator<T>& other);
  private:
    SmartPointer<ListNode<T> > _node;
    int _time;
    int _height;
  };
}

#include "PSLIterator.cpp"

#endif
