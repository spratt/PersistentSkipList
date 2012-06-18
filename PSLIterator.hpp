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
  class PersistentSkipList;
  
  template < class T >
  class PSLIterator {
  public:
    PSLIterator(SmartPointer<ListNode<T> >& node,
		PersistentSkipList<T>& psl,
		int time=0,
		int height=0);
    ~PSLIterator(void);

    PSLIterator<T> getNext(void);
    int getHeight(void);
    int getSearchHeight(void);
    
    void next(void);
    void down(void);
    PSLIterator<T>& operator++(void);
    
    T getDatum(void);
    T operator*(void);

    bool operator==(const PSLIterator<T>& other);
    bool operator!=(const PSLIterator<T>& other);

    bool operator<(const PSLIterator<T>& other);
    bool operator<=(const PSLIterator<T>& other);
    bool operator>(const PSLIterator<T>& other);
    bool operator>=(const PSLIterator<T>& other);

    bool operator==(const T& datum);
    bool operator!=(const T& datum);

    bool operator<(const T& datum);
    bool operator<=(const T& datum);
    bool operator>(const T& datum);
    bool operator>=(const T& datum);

    const PSLIterator<T>& operator=(PSLIterator<T>& other);
    const PSLIterator<T>& operator=(const PSLIterator<T>& other);

    void remove(void);
  private:
    PersistentSkipList<T>& _psl;
    SmartPointer<ListNode<T> > _node;
    int _time;
    int _height;
  };
}

#include "PSLIterator.cpp"

#endif
