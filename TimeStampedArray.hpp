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
#ifndef TIMESTAMPEDARRAY_HPP
#define TIMESTAMPEDARRAY_HPP

// DEBUG
#include <iostream>
using namespace std;
// END DEBUG

#include <cstddef>
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
    TimeStampedArray(int t, int s);

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
    //   Type/Name:   int/s                                                  //
    //   Description: The size of the new array.                             //
    //                                                                       //
    //   Type/Name:   TimeStampedArray<T>/old_tsa                            //
    //   Description: The existing TSA to copy.  Copies everything but       //
    //                timestamp, does not modify original.                   //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TimeStampedArray(int t, int s, const TimeStampedArray<T>& old_tsa);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ~TimeStampedArray                                      //
    //                                                                       //
    // PURPOSE:       Destructor.                                            //
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
    ~TimeStampedArray();
    
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
    bool isLocked() const;

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
    int getTime() const;

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
    int getSize() const;

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
    //   Type/Name:   T&                                                     //
    //   Description: The array element at index i.                          //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    T& getElement(int i) const;

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
    //   Type/Name:   T&/datum                                               //
    //   Description: The new element to store in the array.                 //
    //                                                                       //
    // RETURN:        An integer return code.  0 indicates success.          //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setElement(int i, T& datum);
  };

  #include "TimeStampedArray.cpp"
}

#endif
