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
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////

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
    TimeStampedArray(int t, int s)
      : time(t), size(s), _LOCKED(false), data(new T[size]) {
    }
    TimeStampedArray(int t, TimeStampedArray<T> old_tsa) :
      : time(t), size(old_tsa.getSize()), data(new T[old_tsa.getSize()]) {
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
    // 
    // FUNCTION NAME: getTime
    // 
    // PURPOSE:       Returns the timestamp of this array.
    // 
    // SECURITY:      public
    // 
    // PARAMETERS
    //   Type/Name:   Void.
    //   Description: None.
    // 
    // RETURN:
    //   Type/Name:   int
    //   Description: The timestamp of the array.
    // 
    // NOTES:         None.
    // 
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
  int TimeStampedArray<T>::getTime() {
    return time;
  }

  template<class T>
  int TimeStampedArray<T>::getSize() {
    return size;
  }

  template<class T>
  void TimeStampedArray<T>::getElement(int i) {
    assert(i > 0);
    assert(i < size);
    return data[i];
  }

  template<class T>
  int TimeStampedArray<T>::setElement(int i, T datum) {
    assert(i > 0);
    assert(i < size);
    data[i] = datum;
    // success
    return 0;
  }
}
