//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    TimeStampedArray.h                                               //
//                                                                           //
// MODULE:  Time Stamped Array                                               //
//                                                                           //
// PURPOSE: Provides a simple data structure for storing a fixed-size        //
//          array with an associated timestamp.                              //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
// TimeStampedArray<T>                  An array with an associated          //
//                                      timestamp.                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Constructors:                                 //
//                                                                           //
// TimeStampedArray(int t, int s)             - t is the timestamp           //
//                                              s is the size                //
// TimeStampedArray(int t, TimeStampedArray&) - t is the timestamp           //
//                                              the reference is copied      //
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
// lock()            - locks the structure                                   //
// isLocked()        - returns true if structure is locked                   //
// getTime()         - returns the timestamp of the structure                //
// getSize()         - returns the size of the structure                     //
// getElement(int)   - returns the element at the given 0 <= index < size    //
// setElement(int,T) - sets the element at the given 0 <= index < size       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef TIMESTAMPEDARRAY_H
#define TIMESTAMPEDARRAY_H

#include <assert.h>

namespace timestamped_array {
  /////////////////////////////////////////////////////////////////////////////
  // TimeStampedArray interface                                              //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  class TimeStampedArray {
  private:
    bool _LOCKED;
    int time;
    int size;
    T* data;
    
  public:
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: TimeStampedArray                                       //
    //                                                                       //
    // PURPOSE:       Basic constructor which takes a time and size.         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The timestamp to associate with the array.             //
    //                                                                       //
    //   Type/Name:   int/s                                                  //
    //   Description: The size of the array.                                 //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TimeStampedArray(int t, int s)
      : _LOCKED(false),
	time(t),
	size(s),
	data(new T[size]) 
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: TimeStampedArray                                       //
    //                                                                       //
    // PURPOSE:       Modified copy constructor which takes a                //
    //                timestamp and an existing TSA to copy.                 //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The timestamp to associate with the array.             //
    //                                                                       //
    //   Type/Name:   TimeStampedArray<T>/old_tsa                            //
    //   Description: The existing TSA to copy.  Copies everything but       //
    //                timestamp, does not modify original.                   //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TimeStampedArray(int t, TimeStampedArray<T>& old_tsa)
      : _LOCKED(false),
	time(t),
	size(old_tsa.getSize()),
	data(new T[old_tsa.getSize()]) 
    {
      // copy the old data
      for(int i = 0; i < size; ++i)
	setElement(i,old_tsa.getElement(i));
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: lock                                                   //
    //                                                                       //
    // PURPOSE:       Locks the data against further changes.                //
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
    void lock();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: isLocked                                               //
    //                                                                       //
    // PURPOSE:       Returns true if the structure is locked.               //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        true if the structure is locked,                       //
    //                false otherwise.                                       //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    bool isLocked();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getTime                                                //
    //                                                                       //
    // PURPOSE:       Returns the timestamp of this array.                   //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The timestamp of the array.                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getTime();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getSize                                                //
    //                                                                       //
    // PURPOSE:       Returns the size of this array.                        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int                                                    //
    //   Description: The size of the array.                                 //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getSize();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getElement                                             //
    //                                                                       //
    // PURPOSE:       Retrieves an element from the array at a given index.  //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/i                                                  //
    //   Description: The index at which to retrieve the array element.      //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   T                                                      //
    //   Description: The array element at index i.                          //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    T getElement(int i);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: setElement                                             //
    //                                                                       //
    // PURPOSE:       Sets element at a given index.                         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/i                                                  //
    //   Description: The index in the array at which to set the element.    //
    //                                                                       //
    //   Type/Name:   T/datum                                                //
    //   Description: The new element to store in the array.                 //
    //                                                                       //
    // RETURN:        An integer return code.  0 indicates success.          //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setElement(int i, T datum);
  };

  template<class T>
  void TimeStampedArray<T>::lock() {
    _LOCKED = true;
  }

  template<class T>
  bool TimeStampedArray<T>::isLocked() {
    return _LOCKED;
  }

  template<class T>
  int TimeStampedArray<T>::getTime() {
    return time;
  }

  template<class T>
  int TimeStampedArray<T>::getSize() {
    return size;
  }

  template<class T>
  T TimeStampedArray<T>::getElement(int i) {
    assert(i >= 0);
    assert(i < size);
    return data[i];
  }

  template<class T>
  int TimeStampedArray<T>::setElement(int i, T datum) {
    assert(! _LOCKED);
    assert(i >= 0);
    assert(i < size);
    data[i] = datum;
    // success
    return 0;
  }
}

#endif
