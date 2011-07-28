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
using namespace timestamped_array;

template <class T>
void testTSA(const TimeStampedArray<T>& tsa) {
  /////////////////////////////////////////////////////////////////////////////
  // Test locking                                                            //
  /////////////////////////////////////////////////////////////////////////////
  if(tsa.isLocked())
    cout << "Structure is locked." << endl;
  else
    assert(false);
  
  /////////////////////////////////////////////////////////////////////////////
  // Test getTime                                                            //
  /////////////////////////////////////////////////////////////////////////////
  cout << "At time " << tsa.getTime() << ": "
       << tsa.getElement(0);
  
  /////////////////////////////////////////////////////////////////////////////
  // Test getElement                                                         //
  /////////////////////////////////////////////////////////////////////////////
  for(int i = 1; i < tsa.getSize(); ++i)
    cout << ", " << tsa.getElement(i);
  cout << endl;
}

int main(int argv, char** argc) {
  // Create an original TSA
  TimeStampedArray<int> tsa(0, 3);
  cout << "Structure created successfully." << endl;
  for(int i = 0; i < tsa.getSize(); ++i)
    tsa.setElement(i,i);
  cout << "Structure built successfully." << endl;
  tsa.lock();
  testTSA(tsa);

  // Create a TSA copy
  TimeStampedArray<int>* tsa_copy = new TimeStampedArray<int>(1, tsa);
  cout << "Structure copied successfully." << endl;
  tsa_copy->setElement(1, 3);
  tsa_copy->lock();
  testTSA(*tsa_copy);

  // test destructor
  delete tsa_copy;
  cout << "Structure deleted successfully." << endl;
  
  // done
  return 0;
}
