///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    test_timestamped_array.cpp                                       //
//                                                                           //
// MODULE:  Time Stamped Array                                               //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TimeStampedArray.h"

using namespace std;

template <class T>
void testTSA(timestamped_array::TimeStampedArray<T>& tsa) {
  if(tsa.isLocked())
    cout << "Structure is locked." << endl;
  else
    assert(false);
  cout << "At time " << tsa.getTime() << ": "
       << tsa.getElement(0);
  for(int i = 1; i < tsa.getSize(); ++i)
    cout << ", " << tsa.getElement(i);
  cout << endl;
}

int main(int argv, char** argc) {
  timestamped_array::TimeStampedArray<int> tsa(0, 3);
  for(int i = 0; i < tsa.getSize(); ++i)
    tsa.setElement(i,i);
  tsa.lock();
  testTSA(tsa);
  timestamped_array::TimeStampedArray<int> tsa_copy(1, tsa);
  tsa_copy.setElement(1, 3);
  tsa_copy.lock();
  testTSA(tsa_copy);
  // done
  return 0;
}
