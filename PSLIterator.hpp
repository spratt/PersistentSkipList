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

#include "ListNode.hpp"

namespace persistent_skip_list {
  template < class T >
  class PSLIterator {
  public:
    PSLIterator(SmartPointer<ListNode<T> >& node, int time);
    ~PSLIterator();
    
    void getNext();
    PSLIterator& operator++();
    
    T getDatum();
    T operator*();

    bool operator==(PSLIterator& other);
  private:
    SmartPointer<ListNode<T> > _node;
    int _time;
  };
}

#include "PSLIterator.cpp"

#endif
