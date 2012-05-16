///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    TimeStampedArray.cpp                                             //
//                                                                           //
// MODULE:  Time Stamped Array                                               //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef TIMESTAMPEDARRAY_CPP
#define TIMESTAMPEDARRAY_CPP

#include <ostream>
#include "TimeStampedArray.hpp"

using namespace timestamped_array;
using namespace std;

template<class T>
TimeStampedArray<T>::TimeStampedArray(int t, int s)
  : _LOCKED(false),
    time(t),
    size(s),
    data(new T[s]())
{
}

template<class T>
TimeStampedArray<T>::TimeStampedArray(int t, int s, const TimeStampedArray<T>& old_tsa)
  : _LOCKED(false),
    time(t),
    size(s),
    data(new T[s])
{
  // copy the old data
  for(int i = 0; i < old_tsa.getSize(); ++i)
    setElement(i,old_tsa.getElement(i));
}

template<class T>
TimeStampedArray<T>::~TimeStampedArray() {
  delete[] data;
}

template<class T>
void TimeStampedArray<T>::lock() {
  assert(this != NULL);
  _LOCKED = true;
}

template<class T>
bool TimeStampedArray<T>::isLocked() const {
  assert(this != NULL);
  return _LOCKED;
}

template<class T>
int TimeStampedArray<T>::getTime() const {
  assert(this != NULL);
  return time;
}

template<class T>
int TimeStampedArray<T>::getSize() const {
  assert(this != NULL);
  return size;
}

template<class T>
T& TimeStampedArray<T>::getElement(int i) const {
  assert(this != NULL);
  assert(i >= 0);
  assert(i < size);
  return data[i];
}

template<class T>
T& TimeStampedArray<T>::operator[](int i) const {
  return getElement(i);
}

template<class T>
int TimeStampedArray<T>::setElement(int i, T& datum) {
  assert(this != NULL);
  assert(! _LOCKED);
  assert(i >= 0);
  assert(i < size);
  data[i] = datum;
  // success
  return 0;
}

template<class T>
ostream& operator<<(ostream& o, TimeStampedArray<T>& tsa) {
  for(short unsigned int i = 0; i < tsa.getSize(); ++i)
    o << i << ": " << tsa.getElement(i) << endl;
  return o;
}

#endif
